# Macros for images ;D

function(set_image_entrypoint MODULE ENTRY)
    target_link_options(${MODULE} PRIVATE "-Wl,--entry,${ENTRY}")
endfunction()

function(set_image_base MODULE BASE)   
    target_link_options(${MODULE} PRIVATE "-Wl,--image-base,${BASE}")
endfunction()

function(set_image_subsystem MODULE SUBSYSTEM)
    target_link_options(${MODULE} PRIVATE "-Wl,--subsystem,${SUBSYSTEM}:5.01")
endfunction()