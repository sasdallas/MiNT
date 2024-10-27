# CMake macros

function(get_cmake_defines _output_var)
    get_directory_property(_defines COMPILE_DEFINITIONS)
    foreach (arg ${_defines}) 
        if ((NOT arg MATCHES [[^\$<.*>$]]))
            list(APPEND __tmp_list -D${arg}) # Using list of -D arguments
        endif()
    endforeach()
    set(${_output_var} ${__tmp_list} PARENT_SCOPE)
endfunction()

function(get_cmake_includes _output_var)
    get_directory_property(_includes INCLUDE_DIRECTORIES)
    foreach (arg ${_includes}) 
        list(APPEND __tmp_list -I${arg}) # Using list of -D arguments
    endforeach()
    set(${_output_var} ${__tmp_list} PARENT_SCOPE)
endfunction()

function(setImageBase MODULE IMAGE_BASE)
    target_link_options(${MODULE} PRIVATE "-Wl,--image-base,${IMAGE_BASE}")
endfunction()

function(setImageSubsystem MODULE SUBSYSTEM)
    target_link_options(${MODULE} PRIVATE "-Wl,--subsystem,${SUBSYSTEM}:5.01")
endfunction()

function(setImageEntrypoint MODULE ENTRYPOINT)
    if (${ENTRYPOINT} STREQUAL "0")
        target_link_options(${MODULE} PRIVATE "-Wl,-entry,0")
    elseif (ARCH STREQUAL "i386")
        set(_entrysymbol _${ENTRYPOINT})
        
        if (${ARGC} GREATER 2)
            set(_entrysymbol ${_entrysymbol}@${ARGV2})
        endif()

        target_link_options(${MODULE} PRIVATE "-Wl,-entry,${_entrysymbol}")
    else()
        target_link_options(${MODULE} PRIVATE "-Wl,-entry,${ENTRYPOINT}")
    endif()
endfunction()

function(setModuleType MODULE TYPE)
    # Parse for more arguments
    cmake_parse_arguments(__module "BASE" "ENTRY" ${ARGN})

    # Only support kernel objects at the moment
    if (NOT (TYPE STREQUAL kernel) AND NOT (TYPE STREQUAL kerneldll))
        message(FATAL "Unsupported module type ${TYPE}")
    endif()

    # Setup subsystem, entrypoint, and base
    setImageSubsystem(${MODULE} native)

    if (__module_ENTRYPOINT OR (__module_ENTRYPOINT STREQUAL "0"))
        setImageEntrypoint(${MODULE} ${__module_ENTRYPOINT})
    else()
        setImageEntryPoint(${MODULE} DllMain)
    endif()

    if (__module_IMAGEBASE)
        setImageBase(${MODULE} ${__module_IMAGEBASE})
    else()
        if (TYPE STREQUAL kernel)
            setImageBase(${MODULE} 0x00400000)
        else()
            setImageBase(${MODULE} 0x00010000)
        endif()
    endif()


    # Add exports for kernel
    if (TYPE STREQUAL kernel)
        set_target_properties(${MODULE}
            PROPERTIES
            ENABLE_EXPORTS TRUE
            DEFINE_SYMBOL "")
    endif()

    # Apply NT OS flags
    target_link_options(${MODULE}
        PRIVATE -Wl,--major-image-version,5 -Wl,--minor-image-version,01 -Wl,--major-os-version,5 -Wl,--minor-os-version,01)

    # Apply extra flags
    target_link_options(${MODULE} PRIVATE -Wl,--exclude-all-symbols,--file-alignment=0x1000,-section-alignment=0x1000)

    # Add in init-sections
    get_filename_component(_linker_script ${MINT_SOURCE_DIR}/cmake/init-sections.lds ABSOLUTE)
    target_link_options(${MODULE} PRIVATE "-Wl,-T,${_linker_script}")
    set_property(TARGET ${MODULE} APPEND PROPERTY LINK_DEPENDS ${_linker_script})

    
    # Run PE fixup to mess with sections
    add_custom_command(
        TARGET ${MODULE} POST_BUILD
        COMMAND mint-pefixup --kernel $<TARGET_FILE:${MODULE}> 
        DEPENDS mint-pefixup
    )

    # Link to libgcc
    target_link_libraries(${MODULE} -lgcc)

endfunction()