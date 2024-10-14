# This file contains MiNT source definitions (not arch-specific)

# Create the minilib library
list(APPEND MINILIB_SOURCE
        minilib/string.c)

add_library(
    minilib_library
    ${MINILIB_SOURCE}
)


list(APPEND MINTLDR_GENERAL_C_SOURCE
        mintldr.c)

list(APPEND MINTLDR_GENERAL_SOURCE
        ${MINTLDR_GENERAL_C_SOURCE})


list(APPEND MINTLDR_UI_SOURCE
        ui/ui.c
        ui/ui_print.c)


list(APPEND MINTLDR_SOURCE
        ${MINTLDR_UI_SOURCE}
        ${MINTLDR_GENERAL_SOURCE})