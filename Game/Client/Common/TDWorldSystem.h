#pragma once
#include "TDNode.h"
#include "TDGfxSystem.h"
#include "TDSound.h"
#include "TDObjectSystem.h"
//======================================================================
//游戏场景接口
struct _DBC_SCENE_DEFINE;
struct MAP_POS_DEFINE
{
	fVector3	pos;
	DWORD		dwSceneID; // 所在场景id
	char		name[ 128 ];
	INT			nServerID;
};

//对外提供的地形高度查询接口(使用渲染系统坐标，考虑地形和行走面)
typedef bool (__stdcall* FUNC_GETTERRAINHEIGHT)(float x, float z, float& y);

class tScene
{
public:
	//取得场景定义
	virtual const _DBC_SCENE_DEFINE*		GetSceneDefine(VOID) const  = 0;
	//场景位置是否合法
	virtual BOOL							IsValidPosition(const fVector2& fvPos) = 0;
	//根据逻辑坐标取得地形高度，考虑行走面，如果传入坐标非法，返回FALSE
	virtual BOOL							GetMapHeight(const fVector2& fvMapPos, FLOAT& fHeight) = 0;
};

//======================================================================
//游戏世界接口
//
class tWorldSystem : public tNode
{
public:
	enum WORLD_STATION
	{
		WS_NOT_ENTER,				//没有进入任何场景
		WS_ENTER_ING,				//正在进入一个场景(已经发送了CGEnterScene)
		WS_ENTER,					//成功进入一个场景
		WS_RELIVE,					//死亡复活
		WS_ASK_CHANGE_SCANE,		//要求切换到一个新的场景(已经发送了CGAskChangeScene)
	};	
public:
	/**
		进入某场景
		\param nSceneID	
			场景ID/玩家城市id

		\param nCityLevel
			玩家城市等级，如果小于0，表示是普通场景!
	*/
	virtual BOOL							EnterScene(INT nSceneID, INT nCityLevel)= 0;
	///取得当前激活的场景
	virtual tScene*							GetActiveScene(void)		= 0;
	///取得当前状态
	virtual WORLD_STATION					GetStation(void) const		= 0;
	//对外提供的地形高度查询接口(使用渲染系统坐标，考虑地形和行走面)
	virtual FUNC_GETTERRAINHEIGHT			GetTerrainHeightFunc(void)	= 0;
	//当前场景的ServerID
	virtual INT								GetActiveSceneID(void) const = 0;


	virtual std::list< MAP_POS_DEFINE >* GetObjectListByClass( INT nType ) = 0;
	virtual void setMinimapSerachRange( int nRange ) = 0;
	virtual void UpdateMinimapData() = 0;
	virtual void MinimapNeedFresh() = 0;


	virtual fVector3 GetMyselfPos() = 0;
	virtual int GetMyselfDir() = 0;
	virtual const CHAR* GetSceneName( INT nSceneID ) = 0;
	WX_DECLARE_DYNAMIC(tWorldSystem);
};