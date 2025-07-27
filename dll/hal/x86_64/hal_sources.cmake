list (APPEND __GENERIC_SOURCE
                        generic/io.c)

list (APPEND __HAL_SOURCE
                        hal_main.c
                        ${__GENERIC_SOURCE})