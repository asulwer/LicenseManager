// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#include <stdlib.h>
//#include <afxwin.h>         // MFC core and standard components
//#include <afxext.h>         // MFC extensions

#include <process.h>

#define WIN32_LEAN_AND_MEAN
#pragma managed(push,off) //warning C4793: 'RtlSecureZeroMemory' : function compiled as native
# include <windows.h>
#pragma managed(pop)