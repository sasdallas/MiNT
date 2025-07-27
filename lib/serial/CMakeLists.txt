# Serial port library

add_library(serial serial.c)
target_link_libraries(serial hal)
