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