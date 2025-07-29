/**
 * @file include/mint/sal.h
 * @brief Windows formatting
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_SAL_H__
#define __MINT_SAL_H__ 1

__MINT_BEGIN_DECLS

#undef _In_
#define _In_

#undef _Out_
#define _Out_

#define STATIC static
#define EXTERN extern

#undef IN
#define IN _In_

#undef OUT
#define OUT _Out_

#undef OPTIONAL
#define OPTIONAL

#undef _Null_terminated_
#define _Null_terminated_

__MINT_END_DECLS

#endif