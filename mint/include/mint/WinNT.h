// WinNT.h - Basic Windows datatype library
// This file is not protected and is released to the public domain
#pragma once

#include <BaseTsd.h>
#include <IntSafe.h>
#include <WinDef.h>

#if !defined(_M_IX86)
 typedef __int64 LONGLONG; 
#else
 typedef double LONGLONG;
#endif

#if !defined(_M_IX86)
 typedef unsigned __int64 ULONGLONG;
#else
 typedef double ULONGLONG;
#endif

typedef void *PVOID;
typedef wchar_t WCHAR;
typedef long LONG;
typedef BYTE BOOLEAN;
typedef char CCHAR;
typedef char CHAR;
typedef CHAR *PCHAR;
typedef short SHORT;
typedef PVOID HANDLE;
typedef LONG HRESULT;
typedef WORD LANGID;
typedef DWORD LCID;
typedef CONST CHAR *LPCSTR; // should be __nullterminated but gcc dont like that
typedef CONST WCHAR *LPCWSTR;
typedef CHAR *LPSTR;
typedef WCHAR *LPWSTR;
typedef BOOLEAN *PBOOLEAN;


#ifdef UNICODE
 typedef WCHAR TCHAR;
#else
 typedef char TCHAR;
#endif

#ifdef UNICODE
 typedef WCHAR TBYTE;
#else
 typedef unsigned char TBYTE;
#endif

typedef CONST CHAR *PCSTR;
typedef CONST WCHAR *PCWSTR;
typedef DWORDLONG *PDWORDLONG;
typedef HANDLE *PHANDLE;
typedef PDWORD PLCID;
typedef LONG *PLONG;
typedef LONGLONG *PLONGLONG;
typedef SHORT *PSHORT;
typedef CHAR *PSTR;
typedef TBYTE *PTBYTE;
typedef TCHAR *PTCHAR;
typedef WCHAR *PWCHAR;
typedef WCHAR *PWSTR;
typedef LONGLONG USN;
#define VOID void
typedef wchar_t WCHAR;

