list (APPEND HAL_GENERIC_SOURCE
        generic/port.c)


list (APPEND HAL_SOURCE
        ${HAL_GENERIC_SOURCE}
        hal_main.c)
