// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once


#ifdef __WINDOWS__
#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
#define _USE_32BIT_TIME_T
#include <tchar.h>
#endif

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <malloc.h>
#include <map>
#include "stdarg.h"


#include "Type.h"
#include "DB_Struct.h"
#include "Config.h"
#include "FileDef.h"
#include "Ini.h"
#include "Log.h"
#include "File.h"
#include "TimeManager.h"
#include "GameStruct_Finger.h"
#include "Packet.h"
//#include "fileapi.h"
// TODO: 在此处引用程序需要的其他头文件
