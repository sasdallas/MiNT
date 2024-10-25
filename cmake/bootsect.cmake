include(${CMAKE_MODULE_PATH}/CMakeMacros.cmake)

function(GenerateBootSector bs_id bs_file bs_out)
    get_cmake_defines(_defines)
    get_cmake_includes(_includes)

    add_custom_command(
        OUTPUT ${bs_out}
        COMMAND nasm -i${MINT_SOURCE_DIR}/mint/include/asm/ -f bin -o ${bs_out}  -D__ASM__ ${_includes} ${_defines} ${bs_file}
        DEPENDS ${bs_file}
    )

    add_custom_target(${bs_id} ALL DEPENDS ${bs_out})
endfunction()

function(GenerateBootSectorGAS bs_id bs_file bs_out bs_addr)
    set(_object_file ${bs_out}.o)

    get_cmake_defines(_defines)
    get_cmake_includes(_includes)

    add_custom_command(
        OUTPUT ${_object_file}
        COMMAND ${CMAKE_ASM_COMPILER} -x assembler-with-cpp -o ${_object_file} -I ${MINT_SOURCE_DIR}/mint/include/asm ${_includes} ${_defines} -D__ASM__ -c ${bs_file}
        DEPENDS ${bs_file}
    )

    add_custom_command(
        OUTPUT ${bs_out}
        COMMAND mint-obj2bin ${_object_file} ${bs_out} ${bs_addr}
        DEPENDS ${_object_file} mint-obj2bin
    )

    set_source_files_properties(${_object_file} ${bs_out} PROPERTIES GENERATED TRUE)

    add_custom_target(${bs_id} ALL DEPENDS ${bs_out})

endfunction()