#ifndef _DEFINED_44E531B1_14D3_11d5_A025_006067718D04
#define _DEFINED_44E531B1_14D3_11d5_A025_006067718D04
#if _MSC_VER > 1000
#pragma once
#endif
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#ifdef _COMPILING_44E531B1_14D3_11d5_A025_006067718D04
#define LIBSPEC __declspec(dllexport)
#else
#define LIBSPEC __declspec(dllimport)
#endif // _COMPILING_44E531B1_14D3_11d5_A025_006067718D04
   LIBSPEC BOOL setMyHook(HWND hWnd);
   LIBSPEC BOOL clearMyHook(HWND hWnd);
#undef LIBSPEC
#ifdef __cplusplus
}
#endif // __cplusplus
#define UWM_MOUSEMOVE_MSG _T("UWM_MOUSEMOVE_MSG-44E531B1_14D3_11d5_A025_006067718D04")

// These declarations are because the latest SDK isn't installed...
#ifndef GET_X_LPARAM
#define GET_X_LPARAM(p) ((int)(short)LOWORD(p))
#define GET_Y_LPARAM(p) ((int)(short)HIWORD(p))
#endif

#endif // _DEFINED_44E531B1_14D3_11d5_A025_006067718D04

