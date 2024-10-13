// =======================================================================================================
// FILE:         entry.S
// DESCRIPTION:  i386 entrypoint
// AUTHOR:       Samuel S.
// CREATED:      October 13th, 2024
// LICENSE:      Under the license of MiNT (BSD 3-clause)
// =======================================================================================================

#include <asm.inc>

.code32

.global _arch_entrypoint
_arch_entrypoint:
    cli
    hlt