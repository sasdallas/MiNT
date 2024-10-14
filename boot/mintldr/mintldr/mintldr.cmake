# This file contains MiNT source definitions (not arch-specific)

list(APPEND MINTLDR_GENERAL_C_SOURCE
        mintldr.c)

list(APPEND MINTLDR_GENERAL_SOURCE
        ${MINTLDR_GENERAL_C_SOURCE})


list(APPEND MINTLDR_UI_SOURCE
        ui/ui.c
        ui/ui_print.c)

list(APPEND MINTLDR_SOURCE
        ${MINTLDR_UI_SOURCE}
        ${MINTLDR_GENERAL_SOURCE})