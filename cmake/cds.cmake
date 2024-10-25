# cds.cmake - Manages CD files


# add_cd_file() - Adds a new CD file
# DESTINATION - The folder to add the file to
# CDNAME (OPTIONAL) - The name on the CD the file should have
# TARGET (OPTIONAL IF FILE IS USED) - The target to use the filename of
# FILE (OPTIONAL IF TARGET IS USED) - The filename to use
# FOR (UNUSED) - The CD(s) to add the file to
function (add_cd_file)
    # Best way to implement this is to let arguments be specified in the function but not in its declaration
    cmake_parse_arguments(_CD "NOT_PACKED" "DESTINATION;CDNAME;TARGET" "FILE;FOR" ${ARGN})

    if (NOT _CD_FILE AND NOT _CD_TARGET )
        message(FATAL_ERROR "A target or file was not provided for CD installation")
    endif()

    # Did they specify a destination?
    if (NOT _CD_DESTINATION)
        message(FATAL_ERROR "A destination was not provided for CD installation")
    elseif (_CD_DESTINATION STREQUAL root)
        # If root was specified, blank it
        set(_CD_DESTINATION "")
    endif()


    # If _CD_FILE was not specified, they want to use a target 
    if (NOT _CD_FILE)
        # The user specified a target we should check
        set(_CD_FILE "$<TARGET_FILE:${_CD_TARGET}>")
        if (NOT _CD_CDNAME)
            set(_CD_CDNAME "$<TARGET_FILE_NAME:${_CD_TARGET}>")
        endif()
    endif()


    
    foreach (item ${_CD_FILE})
        if (_CD_CDNAME)
            set(__file_to_add ${_CD_CDNAME}) # Use a custom name
        else()
            get_filename_component(__file_to_add ${item} NAME)
        endif()

        message(STATUS "Adding file ${_CD_DESTINATION}/${__file_to_add}")

        set_property(GLOBAL APPEND PROPERTY BOOTCD_FILE_LIST "${_CD_DESTINATION}/${__file_to_add}=${item}")
        if (_CD_TARGET)
            add_dependencies(bootcd ${_CD_TARGET})
        endif()
    endforeach()
endfunction()

# create_cd_lists() - Generates a list of CD files (e.g. bootcd.cmake.lst)
function(create_cd_lists)

    # Add bootcd files
    get_property(_filelist GLOBAL PROPERTY BOOTCD_FILE_LIST)
    string(REPLACE ";" "\n" _filelist "${_filelist}") # Lists will use ; instead of \n
    file(APPEND ${MINT_BINARY_DIR}/boot/bootcd.cmake.lst "${_filelist}")
    unset(_filelist)

    # Generate a file list
    file(GENERATE
            OUTPUT ${MINT_BINARY_DIR}/boot/bootcd.$<CONFIG>.lst
            INPUT ${MINT_BINARY_DIR}/boot/bootcd.cmake.lst)
endfunction()