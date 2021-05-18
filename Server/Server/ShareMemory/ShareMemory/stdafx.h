// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
#define _USE_32BIT_TIME_T

#include "Type.h"
#include "DB_Struct.h"
#include "Config.h"
#include "FileDef.h"
#include "Ini.h"
#include "Log.h"
#include "Thread.h"
#include "File.h"
#include "TimeManager.h"
#include "GameStruct_Finger.h"
#include "Packet.h"
#include "ODBCInterface.h"

#include "DBManager.h"
#include "SMUManager.h"
#include "ShareMemAO.h"
#include "ShareMemAPI.h"
#include "ShareDBManager.h"
#include "SMULogicManager.h"

#include "DBItemSerial.h"
#include "DBMailInfo.h"
#include "DBGuildInfo.h"
#include "PlayerShopDefine.h"
#include "BBS.h"
#include "DBCharFullData.h"
#include <windows.h>
#include <fileapi.h>
