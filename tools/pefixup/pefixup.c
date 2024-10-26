/*
 * PE Fixup Utility
 * Copyright (C) 2005 Filip Navara
 * Copyright (C) 2020 Mark Jansen
 *
 * The purpose of this utility is fix PE binaries generated by binutils and
 * to manipulate flags that can't be set by binutils.
 *
 * Currently one features is implemented:
 *
 * - Updating the PE header to use a LOAD_CONFIG,
 *   when the struct is exported with the name '_load_config_used'
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// host_includes
#include <host/reactos/typedefs.h>
#include <host/reactos/pecoff.h>
#include <host/reactos/dbghelp_compat.h>

static const char* g_ApplicationName;
static const char* g_Target;

enum fixup_mode
{
    MODE_NONE = 0,
    MODE_LOADCONFIG,
    MODE_KERNELDRIVER,
    MODE_WDMDRIVER,
    MODE_KERNELDLL,
    MODE_KERNEL
};

void *rva_to_ptr(unsigned char *buffer, PIMAGE_NT_HEADERS nt_header, DWORD rva)
{
    unsigned int i;
    PIMAGE_SECTION_HEADER section_header = IMAGE_FIRST_SECTION(nt_header);

    for (i = 0; i < nt_header->FileHeader.NumberOfSections; i++, section_header++)
    {
        if (rva >= section_header->VirtualAddress &&
            rva < section_header->VirtualAddress + section_header->Misc.VirtualSize)
        {
            return buffer + rva - section_header->VirtualAddress + section_header->PointerToRawData;
        }
    }

    return NULL;
}

static void error(const char* message, ...)
{
    va_list args;

    fprintf(stderr, "%s ERROR: '%s': ", g_ApplicationName, g_Target);

    va_start(args, message);
    vfprintf(stderr, message, args);
    va_end(args);
}

static void fix_checksum(unsigned char *buffer, size_t len, PIMAGE_NT_HEADERS nt_header)
{
    unsigned int checksum = 0;
    size_t n;

    nt_header->OptionalHeader.CheckSum = 0;

    for (n = 0; n < len; n += 2)
    {
        checksum += *(unsigned short *)(buffer + n);
        checksum = (checksum + (checksum >> 16)) & 0xffff;
    }

    checksum += (unsigned int)len;
    nt_header->OptionalHeader.CheckSum = checksum;
}

static int add_loadconfig(unsigned char *buffer, PIMAGE_NT_HEADERS nt_header)
{
    PIMAGE_DATA_DIRECTORY export_dir;
    PIMAGE_EXPORT_DIRECTORY export_directory;
    PDWORD name_ptr, function_ptr;
    PWORD ordinal_ptr;
    DWORD n;

    export_dir = &nt_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    if (export_dir->Size == 0)
    {
        error("No export directory\n");
        return 1;
    }

    export_directory = rva_to_ptr(buffer, nt_header, export_dir->VirtualAddress);
    if (export_directory == NULL)
    {
        error("Invalid rva for export directory\n");
        return 1;
    }

    name_ptr = rva_to_ptr(buffer, nt_header, export_directory->AddressOfNames);
    ordinal_ptr = rva_to_ptr(buffer, nt_header, export_directory->AddressOfNameOrdinals);
    function_ptr = rva_to_ptr(buffer, nt_header, export_directory->AddressOfFunctions);

    for (n = 0; n < export_directory->NumberOfNames; n++)
    {
        const char* name = rva_to_ptr(buffer, nt_header, name_ptr[n]);
        if (!strcmp(name, "_load_config_used"))
        {
            PIMAGE_DATA_DIRECTORY load_config_dir;
            DWORD load_config_rva = function_ptr[ordinal_ptr[n]];
            PDWORD load_config_ptr = rva_to_ptr(buffer, nt_header, load_config_rva);

            /* Update the DataDirectory pointer / size
               The first entry of the LOAD_CONFIG struct is the size, use that as DataDirectory.Size */
            load_config_dir = &nt_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG];
            load_config_dir->VirtualAddress = load_config_rva;
            load_config_dir->Size = *load_config_ptr;

            return 0;
        }
    }

    error("Export '_load_config_used' not found\n");
    return 1;
}

static int driver_fixup(enum fixup_mode mode, unsigned char *buffer, PIMAGE_NT_HEADERS nt_header)
{
    /* GNU LD just doesn't know what a driver is, and has notably no idea of paged vs non-paged sections */
    for (unsigned int i = 0; i < nt_header->FileHeader.NumberOfSections; i++)
    {
        PIMAGE_SECTION_HEADER Section = IMAGE_FIRST_SECTION(nt_header) + i;

        /* LD puts alignment crap that nobody asked for */
        Section->Characteristics &= ~IMAGE_SCN_ALIGN_MASK;

        /* LD overdoes it and puts the initialized flag everywhere */
        if (Section->Characteristics & IMAGE_SCN_CNT_CODE)
            Section->Characteristics &= ~IMAGE_SCN_CNT_INITIALIZED_DATA;

        if (strncmp((char*)Section->Name, ".rsrc", 5) == 0)
        {
            /* .rsrc is discardable for driver images, WDM drivers and Kernel-Mode DLLs */
            if (mode == MODE_KERNELDRIVER || mode == MODE_WDMDRIVER || mode == MODE_KERNELDLL)
            {
                Section->Characteristics |= IMAGE_SCN_MEM_DISCARDABLE;
            }

            /* For some reason, .rsrc is made writable by windres */
            Section->Characteristics &= ~IMAGE_SCN_MEM_WRITE;
            continue;
        }

        /* Known sections which can be discarded */
        if (strncmp((char*)Section->Name, "INIT", 4) == 0)
        {
            Section->Characteristics |= IMAGE_SCN_MEM_DISCARDABLE;
            continue;
        }

        /* Known sections which can be paged */
        if ((strncmp((char*)Section->Name, "PAGE", 4) == 0)
            || (strncmp((char*)Section->Name, ".rsrc", 5) == 0)
            || (strncmp((char*)Section->Name, ".edata", 6) == 0)
            || (strncmp((char*)Section->Name, ".reloc", 6) == 0))
        {
            continue;
        }

        /* If it's discardable, don't set the flag */
        if (Section->Characteristics & IMAGE_SCN_MEM_DISCARDABLE)
            continue;

        Section->Characteristics |= IMAGE_SCN_MEM_NOT_PAGED;
    }

    return 0;
}

/* NOTE: This function tokenizes its parameter in place.
 * Its format is: name[=newname][,[[!]{CDEIKOMPRSUW}][A{1248PTSX}]] */
static int change_section_attribs(char* section_attribs, unsigned char* buffer, PIMAGE_NT_HEADERS nt_header)
{
    char *sectionName, *newSectionName;
    char *attribsSpec, *alignSpec;
    char *ptr;
    DWORD dwAttribs[2]; /* Attributes to [0]: set; [1]: filter */
    unsigned int i;
    PIMAGE_SECTION_HEADER SectionTable, Section;

    if (!section_attribs || !*section_attribs)
    {
        error("Section attributes specification is empty.\n");
        return 1;
    }

    sectionName = section_attribs;

    /* Find the optional new section name and attributes specifications */
    newSectionName = strchr(section_attribs, '=');
    attribsSpec = strchr(section_attribs, ',');
    if (newSectionName && attribsSpec)
    {
        /* The attributes specification must be after the new name */
        if (!(newSectionName < attribsSpec))
        {
            error("Invalid section attributes specification.\n");
            return 1;
        }
    }
    if (newSectionName)
        *newSectionName++ = 0;
    if (attribsSpec)
        *attribsSpec++ = 0;

    /* An original section name must be specified */
    if (!*sectionName)
    {
        error("Invalid section attributes specification.\n");
        return 1;
    }
    /* If a new section name begins, it must be not empty */
    if (newSectionName && !*newSectionName)
    {
        error("Invalid section attributes specification.\n");
        return 1;
    }

    /* The alignment specification is inside the attributes specification */
    alignSpec = NULL;
    if (attribsSpec)
    {
        /* Check for the first occurrence of the 'A' separator, case-insensitive */
        for (ptr = attribsSpec; *ptr; ++ptr)
        {
            if (toupper(*ptr) == 'A')
            {
                alignSpec = ptr;
                break;
            }
        }
    }
    if (alignSpec)
        *alignSpec++ = 0;

    /* But it's not supported at the moment! */
    if (alignSpec && *alignSpec)
    {
        fprintf(stdout, "%s WARNING: '%s': %s", g_ApplicationName, g_Target,
                "Section alignment specification not currently supported! Ignoring.\n");
    }

    /* Parse the attributes specification */
    dwAttribs[0] = dwAttribs[1] = 0;
    if (attribsSpec && *attribsSpec)
    {
        for (i = 0, ptr = attribsSpec; *ptr; ++ptr)
        {
            if (*ptr == '!')
            {
                /* The next attribute needs to be removed.
                 * Any successive '!' gets collapsed. */
                i = 1;
                continue;
            }

            switch (toupper(*ptr))
            {
            case 'C':
                dwAttribs[i%2] |= IMAGE_SCN_CNT_CODE;
                break;
            case 'D':
                dwAttribs[i%2] |= IMAGE_SCN_MEM_DISCARDABLE;
                break;
            case 'E':
                dwAttribs[i%2] |= IMAGE_SCN_MEM_EXECUTE;
                break;
            case 'I':
                dwAttribs[i%2] |= IMAGE_SCN_CNT_INITIALIZED_DATA;
                break;
            case 'K': /* Remove the not-cached attribute */
                dwAttribs[(i+1)%2] |= IMAGE_SCN_MEM_NOT_CACHED;
                break;
            case 'M':
                dwAttribs[i%2] |= IMAGE_SCN_LNK_REMOVE;
                break;
            case 'O':
                dwAttribs[i%2] |= IMAGE_SCN_LNK_INFO;
                break;
            case 'P': /* Remove the not-paged attribute */
                dwAttribs[(i+1)%2] |= IMAGE_SCN_MEM_NOT_PAGED;
                break;
            case 'R':
                dwAttribs[i%2] |= IMAGE_SCN_MEM_READ;
                break;
            case 'S':
                dwAttribs[i%2] |= IMAGE_SCN_MEM_SHARED;
                break;
            case 'U':
                dwAttribs[i%2] |= IMAGE_SCN_CNT_UNINITIALIZED_DATA;
                break;
            case 'W':
                dwAttribs[i%2] |= IMAGE_SCN_MEM_WRITE;
                break;

            default:
                error("Invalid section attributes specification.\n");
                return 1;
            }

            /* Got an attribute; reset the state */
            i = 0;
        }
        /* If the state was not reset, the attributes specification is invalid */
        if (i != 0)
        {
            error("Invalid section attributes specification.\n");
            return 1;
        }
    }

    /* Find all sections with the given name, rename them and change their attributes */
    Section = NULL;
    SectionTable = IMAGE_FIRST_SECTION(nt_header);
    for (i = 0; i < nt_header->FileHeader.NumberOfSections; ++i)
    {
        if (strncmp((char*)SectionTable[i].Name, sectionName, ARRAY_SIZE(SectionTable[i].Name)) != 0)
            continue;

        Section = &SectionTable[i];

        if (newSectionName && *newSectionName)
        {
            memset(Section->Name, 0, sizeof(Section->Name));
            strncpy((char*)Section->Name, newSectionName, ARRAY_SIZE(Section->Name));
        }

        /* First filter attributes out, then add the new ones.
         * The new attributes override any removed ones. */
        Section->Characteristics &= ~dwAttribs[1];
        Section->Characteristics |=  dwAttribs[0];
    }

    /* If no section was found, return an error */
    if (!Section)
    {
        error("Section '%s' does not exist.\n", sectionName);
        return 1;
    }

    return 0;
}

static
void
print_usage(void)
{
    printf("Usage: %s <options> <filename>\n\n", g_ApplicationName);
    printf("<options> can be one of the following options:\n"
           "  --loadconfig          Fix the LOAD_CONFIG directory entry;\n"
           "  --kernelmodedriver    Fix code, data and resource sections for driver images;\n"
           "  --wdmdriver           Fix code, data and resource sections for WDM drivers;\n"
           "  --kerneldll           Fix code, data and resource sections for Kernel-Mode DLLs;\n"
           "  --kernel              Fix code, data and resource sections for kernels;\n"
           "\n"
           "and/or a combination of the following ones:\n"
           "  --section:name[=newname][,[[!]{CDEIKOMPRSUW}][A{1248PTSX}]]\n"
           "                        Overrides the attributes of a section, optionally\n"
           "                        changing its name and its alignment.\n");
}

int main(int argc, char **argv)
{
    int result = 1;
    enum fixup_mode mode = MODE_NONE;
    int i;
    FILE* file;
    size_t len;
    unsigned char *buffer;
    PIMAGE_DOS_HEADER dos_header;
    PIMAGE_NT_HEADERS nt_header;

    g_ApplicationName = argv[0];

    /* Check for options */
    for (i = 1; i < argc; ++i)
    {
        if (!(argv[i][0] == '-' && argv[i][1] == '-'))
        {
            /* We are out of options (they come first before
             * anything else, and cannot come after). */
            break;
        }

        if (strcmp(&argv[i][2], "loadconfig") == 0)
        {
            if (mode != MODE_NONE)
                goto mode_error;
            mode = MODE_LOADCONFIG;
        }
        else if (strcmp(&argv[i][2], "kernelmodedriver") == 0)
        {
            if (mode != MODE_NONE)
                goto mode_error;
            mode = MODE_KERNELDRIVER;
        }
        else if (strcmp(&argv[i][2], "wdmdriver") == 0)
        {
            if (mode != MODE_NONE)
                goto mode_error;
            mode = MODE_WDMDRIVER;
        }
        else if (strcmp(&argv[i][2], "kerneldll") == 0)
        {
            if (mode != MODE_NONE)
                goto mode_error;
            mode = MODE_KERNELDLL;
        }
        else if (strcmp(&argv[i][2], "kernel") == 0)
        {
            if (mode != MODE_NONE)
                goto mode_error;
            mode = MODE_KERNEL;
        }
        else if (strncmp(&argv[i][2], "section:", 8) == 0)
        {
            /* Section attributes override, will be handled later */
        }
        else
        {
            fprintf(stderr, "%s ERROR: Unknown option: '%s'.\n", g_ApplicationName, argv[i]);
            goto failure;
    mode_error:
            fprintf(stderr, "%s ERROR: Specific mode already set.\n", g_ApplicationName);
    failure:
            print_usage();
            return 1;
        }
    }
    /* Stop now if we don't have any option or file */
    if ((i <= 1) || (i >= argc))
    {
        print_usage();
        return 1;
    }

    g_Target = argv[i];

    /* Read the whole file to memory */
    file = fopen(g_Target, "r+b");
    if (!file)
    {
        fprintf(stderr, "%s ERROR: Can't open '%s'.\n", g_ApplicationName, g_Target);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    len = ftell(file);
    if (len < sizeof(IMAGE_DOS_HEADER))
    {
        fclose(file);
        error("Image size too small to be a PE image\n");
        return 1;
    }

    /* Add one byte extra for the case where the input file size is odd.
       We rely on this in our checksum calculation. */
    buffer = calloc(len + 1, 1);
    if (buffer == NULL)
    {
        fclose(file);
        error("Not enough memory available (Needed %lu bytes).\n", len + 1);
        return 1;
    }

    /* Read the whole input file into a buffer */
    fseek(file, 0, SEEK_SET);
    fread(buffer, 1, len, file);

    /* Check the headers and save pointers to them */
    dos_header = (PIMAGE_DOS_HEADER)buffer;
    if (dos_header->e_magic != IMAGE_DOS_SIGNATURE)
    {
        error("Invalid DOS signature: %x\n", dos_header->e_magic);
        goto Quit;
    }

    nt_header = (PIMAGE_NT_HEADERS)(buffer + dos_header->e_lfanew);
    if (nt_header->Signature != IMAGE_NT_SIGNATURE)
    {
        error("Invalid PE signature: %x\n", nt_header->Signature);
        goto Quit;
    }

    result = 0;

    /* Apply mode fixups */
    if (mode != MODE_NONE)
    {
        if (mode == MODE_LOADCONFIG)
            result = add_loadconfig(buffer, nt_header);
        else
            result = driver_fixup(mode, buffer, nt_header);
    }

    /* Apply any section attributes override */
    for (i = 1; (i < argc) && (result == 0); ++i)
    {
        /* Ignore anything but the section specifications */
        if (!(argv[i][0] == '-' && argv[i][1] == '-'))
            break;
        if (strncmp(&argv[i][2], "section:", 8) != 0)
            continue;

        result = change_section_attribs(&argv[i][10], buffer, nt_header);
    }

    if (!result)
    {
        /* Success. Recalculate the checksum only if this is not a reproducible build file */
        if (nt_header->OptionalHeader.CheckSum != 0)
            fix_checksum(buffer, len, nt_header);

        /* We could optimize by only writing the changed parts, but keep it simple for now */
        fseek(file, 0, SEEK_SET);
        fwrite(buffer, 1, len, file);
    }

Quit:
    free(buffer);
    fclose(file);

    return result;
}