/**
 * @file mint/include/mint/ddk/wdm.h
 * @brief WDM header
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system
 * However, it has been released into the public domain and is free to use
 * This file comes with no warranty.
 */

#pragma once

#ifndef _WDMDDK_
#define _WDMDDK_

#ifndef _MINTHALDLL_
#define MINTHALAPI  DECLSPEC_IMPORT 
#else
#define MINTHALAPI  DECLSPEC_EXPORT
#endif

#endif