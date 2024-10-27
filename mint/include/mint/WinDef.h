/**
 * @file mint/include/mint/WinDef.h
 * @brief Windows definitions
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system
 * However, it has been released into the public domain and is free to use
 * This file comes with no warranty.
 */

#ifndef WINDEF_H
#define WINDEF_H

#pragma once


/* Global Includes */
#include <msvctarget.h>
#include <Sal.h>


/* DECLSPEC definitions */
#define DECLSPEC_IMPORT __declspec(dllimport)
#define DECLSPEC_EXPORT __declspec(dllexport)
#define DECLSPEC_NORETURN __declspec(noreturn)

/* API definitions */
#define MINTAPI __stdcall 

/* Calling conventions */
#define APIENTRY MINTAPI


/* Boolean values */
#define TRUE 1
#define FALSE 0

/* Other values */
#define NULL 0

/* Types */
typedef unsigned short wchar_t; // Shouldn't be here

#include <IntSafe.h>

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef WORD ATOM;
typedef WORD *PWORD;
typedef DWORD *PDWORD;
typedef int BOOL;
#define CONST const

#include <WinNT.h>
#include <BaseTsd.h>


#define CALLBACK __stdcall
typedef DWORD COLORREF;

typedef unsigned char UCHAR;
typedef unsigned int UINT;
typedef unsigned long ULONG;
typedef unsigned short USHORT;
typedef float FLOAT;
typedef HANDLE HACCEL;
typedef HANDLE HBITMAP;
typedef HANDLE HBRUSH;
typedef HANDLE HCOLORSPACE;
typedef HANDLE HICON;
typedef HICON HCURSOR;
typedef HANDLE HDC;
typedef HANDLE HDESK;
typedef HANDLE HENHMETAFILE;
typedef int HFILE;
typedef HANDLE HFONT;
typedef HANDLE HGDIOBJ;
typedef HANDLE HGLOBAL;
typedef HANDLE HHOOK;
typedef HANDLE HINSTANCE;
typedef HANDLE HKEY;
typedef HANDLE HKL;
typedef HANDLE HLOCAL;
typedef HANDLE HMENU;
typedef HANDLE HMETAFILE;
typedef HINSTANCE HMODULE;
typedef HANDLE HMONITOR;
typedef HANDLE HPALETTE;
typedef HANDLE HPEN;
typedef HANDLE HRGN;
typedef HANDLE HRSRC;
typedef HANDLE WINSTA;
typedef HANDLE HWND;
typedef int INT;
typedef LONG_PTR LPARAM;
typedef BOOL *LPBOOL;
typedef BYTE *LPBYTE;
typedef DWORD *LPCOLORREF;
typedef CONST void *LPCVOID;
typedef DWORD *LPDWORD;
typedef HANDLE *LPHANDLE;
typedef int *LPINT;
typedef long *LPLONG;
typedef void *LPVOID;
typedef WORD *LPWORD;
typedef LONG_PTR LRESULT;
typedef BOOL *PBOOL;
typedef BYTE *PBYTE;
typedef FLOAT *PFLOAT;
typedef HKEY *PHKEY;
typedef int *PINT;
typedef UCHAR *PUCHAR;
typedef UINT *PUINT;
typedef ULONG *PULONG;
typedef ULONGLONG *PULONGLONG;
typedef USHORT *PUSHORT;
typedef UINT_PTR WPARAM;

#endif