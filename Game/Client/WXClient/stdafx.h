// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
//
// _WIN32_WINNT version constants
//
#define _WIN32_WINNT_NT4     0x0400
#define _WIN32_WINNT_WIN2K     0x0500
#define _WIN32_WINNT_WINXP     0x0501
#define _WIN32_WINNT_WS03     0x0502
#define _WIN32_WINNT_WIN6     0x0600
#define _WIN32_WINNT_VISTA     0x0600
#define _WIN32_WINNT_WS08     0x0600
#define _WIN32_WINNT_LONGHORN    0x0600
#define _WIN32_WINNT_WIN7     0x0601

#define _WIN32_WINNT _WIN32_WINNT_WINXP
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#define _WIN32_WINDOWS	(0x0410)	//Windows 98
//#define _WIN32_WINNT	(0x0500)	//Windows 2000

// Windows Header Files:
#include <windows.h>
#include <windowsx.h>

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// C RunTime Header Files
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// Standard C++ Library
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <hash_map>
#include <algorithm>

// TODO: reference additional headers your program requires here
#include "TDBasicType.h"
#include "AxTrace.h"
#include "Type.h"
#include "DB_Struct.h"
#include "GameStruct_Finger.h"
#include "BaseType.h"

#include "Procedure\GameProcedure.h"
#include "Object\ObjectManager.h"
#include "Object\Object.h"
#include "GameCommand.h"
#include "Network\NetManager.h"
#include "Interface\GMInterface_Script_Talk.h"
#include "Object\Character\Obj_PlayerMySelf.h"
#include "DataPool\GMDP_CharacterData.h"
#include "Object\ObjectCommandDef.h"
#include "DataPool\GMUIDataPool.h"
#include "DataPool\DataPoolCommandDef.h"
#include "Action\GMActionSystem.h"
#include "TDUISystem.h"
#include "Event\GMEventSystem.h"
#include "Sound\GMSoundSystem.h"
#include "TDUIDataPool.h"
#include "OResultDef.h"
#include "Object\Item\Obj_Item.h"
#include "Interface\GMGameInterface.h"
#include "DataPool\GMDataPool.h"
#include "TDTimeSystem.h"
#include "Object\Character\AI\GMAICommandDef.h"
#include "Object\Character\AI\GMAI_Base.h"
#include "Object\Character\Obj_Character.h"
#include "Global.h"
#include "World\WorldManager.h"
#include "TDException.h"
#include "event\GMEventSystem.h"
#include "..\WxRender\RenderSystem.h"
#include "Object\Character\Obj_PlayerOther.h"
#include "Object\GMObjectLoadQueue.h"
#include "AxProfile.h"
#include "TDVariable.h"
#include "DataPool\TeamOrGroup.h"
#include "DBC\GMDataBase.h"
#include "Global.h"
#include "Procedure\GamePro_Login.h"
#include "Procedure\Gamepro_ChangeScene.h"
#include "TDDBC_Struct.h"
#include "DataPool\GMDP_Struct_Pet.h"
#include "Event/GMEventSystem.h"
#include "TDDEbuger.h"
#include "Object\Character\Obj_PlayerOther.h"
#include "Procedure\GamePro_Enter.h"
#include "Procedure\GamePro_Main.h"
#include "Object\Item\Obj_Item_Equip.h"
#include "DataPool\GMDP_Struct_Mail.h"
#include "DataPool\GMDP_Struct_Relation.h"
#include "procedure\GamePro_CharSel.h"
#include "procedure\GamePro_CharCreate.h"
#include "Object\Character\Obj_PlayerNPC.h"
#include "Object\TripperObj\GMTripperObj_ItemBox.h"
#include "Object\TripperObj\GMTripperObj_Resource.h"
#include "Procedure\Gameprocedure.h"

#include "Interface\GMGameInterface.h"
#include "Procedure\GameProcedure.h"
#include "world\worldmanager.h"
#include "network\netmanager.h"
#include "object\Object.h"
#include "object\Objectmanager.h"
#include "object\character\Obj_playermyself.h"
#include "global.h"
#include "TDinputsystem.h"
#include "TDUtil.h"
#include "Procedure\GamePro_main.h"
#include "TDworldsystem.h"
#include "TDobjectsystem.h"
#include "tdsound.h"
#include "packets\cgpickboxitem.h"
#include "packets\cguseequip.h"
#include "object\ObjectCommand.h"
#include "object\ObjectCommanddef.h"
#include "DataPool\GMUIDataPool.h"
#include "DataPool\GMDataPool.h"
#include "Object\Character\AI\GMAICommandDef.h"
#include "Object\Character\AI\GMAI_Base.h"
#include "CGCommand.h"
#include "World\Path.h"
#include "Object\ProjTex\Obj_ProjTex.h"
#include "Object\Item\Obj_Item_Equip.h"
#include "CGUseGem.h"
#include "CGUseItem.h"
#include "CGDiscardItem.h"
#include "CGDiscardEquip.h"
#include "CGUnEquip.h"
#include "CGReqResetAttr.h"
#include "CGCommand.h"
#include "CGReqLevelUp.h"
#include "Action\GMActionSystem.h"
#include "DataPool\DetailAttrib_Player.h"
#include "cgchat.h"
#include "DBC\GMDataBase.h"
#include "World\WorldManager.h"
#include "Event\GMEventSystem.h"
#include "DataPool\GMDP_CharacterData.h"
#include "Input\GMInputSystem.h"
#include "CampAndStand.h"
#include "DB_Struct.h"