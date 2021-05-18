//
//文件名称：	Type.h
//功能描述：	类型定义，区分不同操作系统间系统调用的设置，所有当前工程
//				内的代码都必须第一个引用此文件
//版本说明：	Windows操作系统需要定义宏：__WINDOWS__
//				Linux操作系统需要定义宏：__LINUX__
//
//

#ifndef __TYPE_H__
#define __TYPE_H__

//当前位置可以定义操作系统使用情况，或者也可以在工程文件内预定义
//#ifndef __WINDOWS__
//	#define __WINDOWS__
//#endif
//#ifndef __LINUX__
//	#define __LINUX__
//#endif

//

#include <vector>
#include "BaseType.h"




///////////////////////////////////////////////////////////////////////
//基本数据类型定义
///////////////////////////////////////////////////////////////////////


typedef SHORT	ID_t;
typedef USHORT	SMUID_t;
typedef	ULONG	SM_KEY;
typedef UINT	ObjID_t;			//场景中固定的所有OBJ拥有不同的ObjID_t
									//

typedef INT				PrescriptionID_t;	//合成配方的每一条有一个 ID
typedef ID_t			AbilityID_t;		//生活技能 ID 号
typedef ID_t			TeamID_t;			//队伍ID
typedef ID_t			SceneID_t;			//场景ID
typedef ID_t			PlayerID_t;			//连接玩家
typedef ID_t			ZoneID_t;			//区域
typedef ID_t			SkillID_t;			//技能
typedef ID_t			ActionID_t;			//动作的ID
typedef ID_t			ImpactID_t;			//效果ID
typedef ID_t			ImpactClassID_t;	//效果分组ID
typedef ID_t			Coord_t;			//网格坐标点
typedef INT				ScriptID_t;			//脚本
typedef INT				MissionID_t;		//任务
typedef ID_t			GroupID_t;			//团ID
typedef ID_t			ChannelID_t;		//频道ID
typedef ID_t			CooldownID_t;   //冷却时间ID
//typedef ID_t			XinFaID_t;			//心法ID
typedef ID_t			CampID_t;				//阵营ID
typedef ID_t			MenPaiID_t;			//门派ID
typedef ID_t			GuildID_t;			//帮派ID
typedef ID_t			CityID_t;			//城市ID


typedef INT				Time_t; //时间类型

typedef struct _Int64
{
	LONG				a;
	ULONG				b;
}Int64;





typedef UINT			GUID_t;	//32位唯一编号。
								//如果用于表示用户唯一ID号，用户部分为9位
								//前三位为世界号,世界号0～200为正式数据，
								//					   201～MAX_WORLD-1为测试数据
								//世界号不能超过或等于 MAX_WORLD
								//通过 GETWORLD 宏可以从GUID_t中取得世界号

#define INVALID_GUID			((GUID_t)(-1))
#define INVALID_INDEX			(-1)
#define INVALID_ID_EX			-2

#if defined(__LINUX__) || defined(__WIN_CONSOLE__)
	typedef UCHAR  BYTE;
	typedef USHORT WORD;
	typedef	ULONG  DWORD;
#endif


#if defined(__WINDOWS__)
	typedef DWORD		TID ;
	//用来定义玩家各个子模型是否可见 ###
	typedef unsigned __int64 MODEL_PART;
#elif defined(__LINUX__)
	typedef pthread_t	TID ;
	typedef unsigned long long MODEL_PART;
	typedef const char* LPCSTR;
#endif

#if defined(__LINUX__)
	typedef		INT		SMHandle;
#elif defined(__WINDOWS__)
	typedef		VOID*	SMHandle;
#endif

typedef		VOID*	DBHandle;




//判断某位是否被置
//15.14....3.2.1.0 
#define ISSET0(x) ((x)&0x1)
#define ISSET1(x) ((x)&0x2)
#define ISSET2(x) ((x)&0x4)
#define ISSET3(x) ((x)&0x8)
#define ISSET4(x) ((x)&0x10)
#define ISSET5(x) ((x)&0x20)
#define ISSET6(x) ((x)&0x40)
#define ISSET7(x) ((x)&0x80)
#define ISSET8(x) ((x)&0x100)
#define ISSET9(x) ((x)&0x200)
#define ISSET10(x) ((x)&0x400)
#define ISSET11(x) ((x)&0x800)
#define ISSET12(x) ((x)&0x1000)
#define ISSET13(x) ((x)&0x2000)
#define ISSET14(x) ((x)&0x4000)
#define ISSET15(x) ((x)&0x8000)


///////////////////////////////////////////////////////////////////////
//基本头文件引用
///////////////////////////////////////////////////////////////////////

#include "Assertx.h"
#include "PlayerStatus.h"
#include "GameDefine.h"
#include "GameDefine2.h"
#include "GameDefine_Attr.h"
#include "GameDefine_Item.h"
#include "GameDefine_Result.h"
#include "GameDefine_Scene.h"
#include "GameDefine_Guild.h"
#include "GameDefine_City.h"
#include "GameDefine_PlayerShop.h"
#include "GameStruct.h"
#include "GameUtil.h"
#include "ResultDefine.h"
#include "PacketDefine.h"

#if defined(GAME_CLIENT)
	#define __ENTER_FUNCTION 
	#define __LEAVE_FUNCTION
#elif 1

#if defined(__WINDOWS__)
	#if defined(NDEBUG)
		#define __ENTER_FUNCTION {try{
		#define __LEAVE_FUNCTION }catch(...){AssertSpecial(FALSE,__FUNCTION__);}}
	#else
		#define __ENTER_FUNCTION {try{
		#define __LEAVE_FUNCTION }catch(...){AssertSpecial(FALSE,__FUNCTION__);}}
	#endif
#else	//linux
	#define __ENTER_FUNCTION {try{
	#define __LEAVE_FUNCTION }catch(...){AssertSpecial(FALSE,__PRETTY_FUNCTION__);}}
#endif 
#endif



#endif
