# ISO image file manipulations

function (add_iso_file)
    cmake_parse_arguments(_ISO "NOT_PACKED" "DESTINATION;ISONAME;TARGET" "FILE;FOR" ${ARGN})

    if (NOT _ISO_FILE AND NOT _ISO_TARGET)
        message(FATAL_ERROR "FILE or TARGET was not specified in add_iso_file")
    endif()

    if (NOT _ISO_DESTINATION)
        message(FATAL_ERROR "DESTINATION not provided in add_iso_file")
    elseif (_ISO_DESTINATION STREQUAL "root")
        set(_ISO_DESTINATION "")
    endif()

    if (NOT _ISO_FILE)
        # Get from the target
        set(_ISO_FILE "$<TARGET_FILE:${_ISO_TARGET}>")
        if (NOT _ISO_ISONAME)
            set(_ISO_ISONAME "$<TARGET_FILE_NAME:${_ISO_TARGET}>")
        endif()
    endif()

    foreach(item ${_ISO_FILE})
        if (_ISO_ISONAME)
            set(__FILE_TO_ADD ${_ISO_ISONAME}) # Custom name specified
        else()
            get_filename_component(__FILE_TO_ADD ${item} NAME)
        endif()

        message(STATUS "Adding file ${_ISO_DESTINATION}/${__FILE_TO_ADD}")

        set_property(GLOBAL APPEND PROPERTY MINT_ISO_FILE_LIST "${_ISO_DESTINATION}/${__FILE_TO_ADD}=${item}")
        
        if (_ISO_TARGET)
            add_dependencies(iso ${_ISO_TARGET})
        endif()
    endforeach()
endfunction()

function(create_iso_list)
    message(STATUS "Generating ISO lists")

    get_property(_ISO_FILELIST GLOBAL PROPERTY MINT_ISO_FILE_LIST)
    string(REPLACE ";" "\n" _ISO_FILELIST "${_ISO_FILELIST}")
    file(WRITE ${MINT_BINARY_DIR}/boot/iso.cmake.lst "${_ISO_FILELIST}")
    unset(_ISO_FILELIST)

    file(GENERATE
            OUTPUT ${MINT_BINARY_DIR}/boot/iso.$<CONFIG>.lst
            INPUT ${MINT_BINARY_DIR}/boot/iso.cmake.lst)
endfunction()