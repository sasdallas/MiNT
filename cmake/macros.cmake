# Macros for images ;D

function(set_image_entrypoint MODULE ENTRY)
    target_link_options(${MODULE} PRIVATE "-Wl,--entry,${ENTRY}")
endfunction()