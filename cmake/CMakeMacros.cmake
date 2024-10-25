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

function(setEntrypoint MODULE ENTRYPOINT)
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