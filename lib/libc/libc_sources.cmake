# libc sources


# ======== STDIO ========

list(APPEND __STDIO_SOURCES 
                        stdio/printf.c)


# ====== ALL SOURCES ========

list(APPEND __LIBC_SOURCES
                        ${__STDIO_SOURCES})