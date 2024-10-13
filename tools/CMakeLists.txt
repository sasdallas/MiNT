cmake_minimum_required(VERSION 3.11)
project("MiNT Tools")


message(STATUS "Configuring host tools")

function(add_host_tool _tool)
    add_executable(${_tool} ${ARGN})
    set_target_properties(${_tool} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${MINT_TOOLS_FOLDER})
endfunction()


include_directories("${MINT_SOURCE_DIR}/mint/include/")

add_host_tool(obj2bin obj2bin/obj2bin.c)

