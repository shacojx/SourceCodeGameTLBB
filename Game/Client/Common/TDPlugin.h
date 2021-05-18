#pragma once

#include "TDBasicType.h"


/****************************************\
*										*
*			    插件导出相关			*
*										*
\****************************************/

//插件必须导出的函数DllInit
//用于插件功能初始化
typedef void (__stdcall* FUNC_DLLINIT)(LPVOID);

//插件必须导出的函数DllRelease
//用于插件功能释放
typedef void (__stdcall* FUNC_DLLRELEASE)(void);

//插件必须导出一个名为DllMagicCode的U32整数,值等于 DLL_MAGIC_CODE|TD_KERNEL_VERSION
#define DLL_MAGIC_CODE	((UINT)0XC0DE0000)

