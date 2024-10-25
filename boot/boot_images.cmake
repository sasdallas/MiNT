## Handles creation of boot images

# TODO: MOVE ME TO ANOTHER AREA!
file(REMOVE ${MINT_BINARY_DIR}/boot/bootcd.cmake.lst)

# File paths to the CD boot files
set(CDLDR_FILE      ${CMAKE_CURRENT_BINARY_DIR}/mintldr/bootsect/cdldr.bin)
set(CDBOOT_FILE     ${CMAKE_CURRENT_BINARY_DIR}/mintldr/bootsect/cdboot.bin)



file (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/empty)

set(ISO_SORT_FILE_DATA "\
${CMAKE_CURRENT_BINARY_DIR}/empty/boot.catalog 4
${CDBOOT_FILE} 3
")

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/bootfiles.sort ${ISO_SORT_FILE_DATA})


add_custom_target(bootcd
    COMMAND mkisofs -quiet -o ${MINT_BINARY_DIR}/bootcd.iso -iso-level 4
        -eltorito-boot loader/cdboot.bin -no-emul-boot -boot-load-size 4
        -no-cache-inodes -graft-points  -sort ${CMAKE_CURRENT_BINARY_DIR}/bootfiles.sort
        -path-list ${MINT_BINARY_DIR}/boot/bootcd.$<CONFIG>.lst
    COMMAND isohybrid -b ${CDLDR_FILE} -t 0x96 ${MINT_BINARY_DIR}/bootcd.iso
    DEPENDS cdboot cdldr
    )

message(STATUS "bootcd generation ready")
