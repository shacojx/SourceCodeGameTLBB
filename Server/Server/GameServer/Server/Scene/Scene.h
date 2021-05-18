/////////////////////////////////////////////////////////////////////////////////
//文件名：Scene.h
//功能描述：游戏场景，单个场景游戏逻辑功能的执行着
//			拥有所有在游戏场景中的游戏逻辑数据，有独立的执行线程来驱动场景的执行
//			管理所有连接在场景中的客户端网络连接，能够独立处理客户端网络消息
//修改记录：2005-3-23创建
//修改记录：2005-11-8整理场景文件结构，之后，所有对场景做修改都需要留下记录
//修改记录：2005-11-14增加场景的事件响应
//修改记录：2005-11-15增加场景的副本系统相关数据和接口
//修改记录：2006-2-7加入摊位管理器
/////////////////////////////////////////////////////////////////////////////////

#ifndef __SCENE_H__
#define __SCENE_H__

#include "Type.h"
#include "Map.h"
#include "ScenePlayerManager.h"
#include "SceneFile.h"
#include "ObjManager.h"
#include "HumanManager.h"


#include "Zone.h"
#include "ItemManager.h"

#include "PlatformManager.h"
#include "SpecialManager.h"
#include "GameUtil.h"
#include "MisBuf.h"
#include "SkillLogic.h"
#include "CopySceneManager.h"
#include "GrowPointManager.h"
#include "StallInfoManager.h"
#include "PlayerShopManager.h"

#include "Performance.h"
#include "EventCore.h"

class Obj_Monster;
class Obj_Character;
class LuaInterface;
class CFileDataMgr;
class MonsterManager;
class PetManager;
class StoreManager;
class AreaManager;
class SceneTimer;
class PatrolPathMgr;
class ChatPipe ;
class Packet;
class ScanOperator;
struct MISSION_DATA;
class GamePlayer ;
class RecyclePlayerManager;
class ItemBoxManager ;

using Combat_Module::Skill_Module::SkillLogic_T;
using namespace Event_Module;

struct _OBJ_MONSTER_INIT ;
//
//// 场景类型
//enum ENUM_SCENE_TYPE
//{
//	SCENE_TYPE_INVALID	= -1,
//	SCENE_TYPE_NORMAL,			// 正常场景
//	SCENE_TYPE_COPY,			// 副本
//	SCENE_TYPE_BATTLEFIELD,		// 战场
//	SCENE_TYPE_ARENA,			// 擂台
//
//	SCENE_TYPE_NUMBERS
//};
//
//

class Scene
{
public :
/////////////////////////////////////////////////////////////////////////////////
//最底层资源及操作处理
/////////////////////////////////////////////////////////////////////////////////
	Scene( SceneID_t SceneID ) ;
	~Scene( ) ;

	//读取场景的数据信息
	BOOL					Load( SCENE_LOAD* load ) ;
	BOOL					SetLoadData( const CHAR* filename, SCENE_LOAD& SceneLoad ) ;

	//初始化
	BOOL					Init( ) ;
	//场景的执行接口，此接口会被场景线程定时调用
	BOOL					Tick( ) ;
	//控制当前场景状态的逻辑接口
	BOOL					StatusLogic( ) ;

/////////////////////////////////////////////////////////////////////////////////
//游戏逻辑底层处理
/////////////////////////////////////////////////////////////////////////////////
protected :
	//场景的逻辑接口
	BOOL					HeartBeat( ) ;

	//处理场景的缓存消息
	BOOL					ProcessCacheCommands( ) ;

	//读取当前场景缓存消息
	BOOL					RecvPacket( Packet*& pPacket, PlayerID_t& PlayerID, uint& Flag ) ;

	//重新调整缓存大小
	BOOL					ResizeCache( ) ;

	//删除某个Player的在场景消息缓存中的消息
	BOOL					MovePacket( PlayerID_t PlayerID ) ;


public :
/////////////////////////////////////////////////////////////////////////////////
//场景事件区
/////////////////////////////////////////////////////////////////////////////////
	//场景初始化完成事件
	VOID					OnSceneInit( ) ;
	//场景计时器事件
	VOID					OnSceneTimer( UINT uTime ) ;
	//场景退出前事件
	VOID					OnSceneQuit( ) ;
	//场景中进入一个玩家
	VOID					OnScenePlayerEnter( GamePlayer* pGamePlayer ) ;
	//场景中一个玩家断线
	VOID					OnScenePlayerDisconnect( GamePlayer* pGamePlayer ) ;
	//场景中一个玩家离开
	VOID					OnScenePlayerLeave( GamePlayer* pGamePlayer ) ;
	//某个副本场景或者城市场景已经创建好
	VOID					OnSceneNotify( SceneID_t DestSceneID ) ;
	//场景中一个玩家升级了
	VOID					OnSceneHumanLevelUp( Obj_Human* pHuman ) ;
	//场景中一个玩家死亡
	VOID					OnSceneHumanDie(Obj_Human* pHuman, ObjID_t idKiller);
	//场景中一个玩家接受任务条件判断
	BOOL					OnAcceptMissionCheck(Obj_Human* pHuman,ScriptID_t idMissionScript) ;
	//玩家创建角色后第一次登陆游戏, 此事件发生在OnScenePlayerEnter之后, OnScenePlayerLogin之前
	VOID					OnScenePlayerFirstLogin( Obj_Human* pHuman ) ;
	//场景中某个玩家的定时器
	VOID					OnScenePlayerTimer( Obj_Human* pHuman, UINT uTime ) ;
	//玩家登陆游戏事件(只调用一次), 此事件发生在OnScenePlayerEnter之后
	VOID					OnScenePlayerLogin( Obj_Human* pHuman ) ;

/////////////////////////////////////////////////////////////////////////////////
//通用基础接口
/////////////////////////////////////////////////////////////////////////////////
public :
	//读取当前场景状态
	INT						GetSceneStatus(){ return m_nSceneStatus ; }
	//设置当前场景状态
	VOID					SetSceneStatus( INT status ){ m_nSceneStatus = status ; }

	//读取当前场景的场景ID
	SceneID_t				SceneID(){ return m_SceneID ; } ;

	//关闭当前场景
	BOOL					CloseScene( ) ;

	//读取当前场景的各个数据模块指针
	Map*					GetMap(){ return m_pMap ; } ;
	ScenePlayerManager*		GetScenePlayerManager(){ return m_pScenePlayerManager ; } ;
	ScenePlayerManager*		GetPlayerManager(){ return m_pScenePlayerManager ; } ;
	ObjManager*				GetObjManager(){ return m_pObjManager ; } ;
	HumanManager*			GetHumanManager(){ return m_pHumanManager ; } ;
	MonsterManager*			GetMonsterManager(){ return m_pMonsterManager ; } ;
	PetManager*				GetPetManager(){ return m_pPetManager ; } ;
	ItemBoxManager*			GetItemBoxManager(){return m_pItemBoxManager;	};
	GrowPointManager*		GetGrowPointManager(){return &m_GrowPointGroup;}
	StallInfoManager*		GetStallInfoManager(){return m_pStallInfoManager;}
	PlayerShopManager*		GetPlayerShopManager(){return m_pPlayerShopManager;}
	StoreManager*			GetStoreManager(){return m_pStoreManager;}
	AreaManager*			GetAreaManager( VOID ){ return m_pAreaManager; }
	SceneTimer*				GetSceneTimer(){ return m_pSceneTimers ;	}
	ChatPipe*				GetChatPipe( ){ return m_pChatPipe ; } ;
	PatrolPathMgr*			GetPatrolPathMgr( ) const { return m_pPatrolPathMgr; } ;
	_ZONE_INFO*				GetZoneInfo( ){ return &m_ZoneInfo ; } ;
	LuaInterface*           GetLuaInterface() { return m_pLuaInterface;}
	CFileDataMgr*           GetScriptFileMgr()  { return m_pScriptFileMgr; }
	inline Zone*			GetZone( ZoneID_t ZoneID ) ;
	RecyclePlayerManager*	GetRecyclePlayerManager(){ return m_pRecyclePlayerManager; };
	EventCore_T&			GetEventCore(VOID) {return m_EventCore;};
	SpecialManager*			GetSpecialManager(){ return m_pSpecialManager ; }
	PlatformManager*		GetPlatformManager(){ return m_pPlatformManager ; }


public :
	//*********
	//*********
	//向此场景发送消息
	//此接口支持数据同步，即可以在不同线程内调用
	//此接口是不同场景间通讯的唯一接口
	//注意：pPacket消息需要用g_pPacketFactoryManager创建出来，用完后不能删除
	//如果 PlayerID为INVALID_ID, 则广播此消息到整个场景
	//如果 PlayerID为INVALID_ID_EX, 则此消息会被场景执行（Player为NULL）
	BOOL					SendPacket( Packet* pPacket, 
										PlayerID_t PlayerID, 
										uint Flag=PF_NONE ) ;
	//*********
	//*********


/////////////////////////////////////////////////////////////////////////////////
//场景扫描相关
public :
	//标准的场景扫描接口
	BOOL			Scan( ScanOperator* pScan ) ;

	//以pPos点为中心，范围为fRange米内的HumanList
	BOOL			ScanHuman( WORLD_POS* pPos, FLOAT fRange, HUMANLIST* pHumanList ) ;
	BOOL			ScanHuman( ZoneID_t idZone, INT nRadius_ZoneCount, HUMANLIST *pOutHumanList );
	
	//向当前场景内发送广播消息，广播的中心点位置为pOwnCharacter所在位置
	//广播距离为len个单位，如果len为-1，则广播到整个场景里
	//如果bSendMe设置为真，则消息同时也发给pOwnCharacter所在玩家
	BOOL			BroadCast_Scene( Packet* pPacket ) ;
	
	//向pOwnCharacter所在的位置广播一个消息
	//消息的范围为当前的ZoneA和ZoneA周围的N圈Zone
	//N为配置好的信息
	BOOL			BroadCast(	Packet* pPacket, 
								Obj_Character* pOwnCharacter,
								BOOL bSendMe=FALSE ) ;

	//向ZoneID所在区域为中心广播一个消息
	//范围为ZoneID所在区域以及周围N圈Zone
	//N为配置好的信息
	BOOL			BroadCast(Packet* pPacket,
							  ZoneID_t	zoneID);

	//广播聊天消息时使用
	//向ZoneID所在区域为中心广播一个消息
	//范围为ZoneID所在区域以及周围N圈Zone
	//N为配置好的信息
	//zoneID为INVALID_ID时，全场景广播
	BOOL			BroadCast_Chat(Packet* pPacket,
								  ZoneID_t	zoneID);

	BOOL			ScanHuman( ZoneID_t idZone, UINT uZoneRadius, HUMANLIST *plistOutHuman );
	// = idZoneA - (idZoneA & idZoneB)
	BOOL			ScanHuman_Sub( ZoneID_t idZoneA, ZoneID_t idZoneB, UINT uZoneRadius, HUMANLIST *plistOutHuman );

	BOOL			ScanObj( ZoneID_t idZone, UINT uZoneRadius, OBJLIST *plistOutObj );
	// = idZoneA - (idZoneA & idZoneB)
	BOOL			ScanObj_Sub( ZoneID_t idZoneA, ZoneID_t idZoneB, UINT uZoneRadius, OBJLIST *plistOutObj );

	// 向HumanList中的每个Obj_Human发送消息
	BOOL			SendPacket( Packet *pPacket, HUMANLIST *plistHuman );
	BOOL			SendPacket_CrateAttr( Obj *pObj, HUMANLIST *plistHuman );


/////////////////////////////////////////////////////////////////////////////////
//场景逻辑功能相关
public :
	//通用游戏接口
	Obj* 			GetSpecificObjByID(ObjID_t nID)const;
	BOOL			ObjectEnterScene( Obj *pObj );
	VOID			ObjectLeaveScene( Obj *pObj );

	// 将 Area 注册到场景上（注册到每个 zone 上面）
	BOOL			RegisterArea( const Area* pArea);
	const Area*		CheckArea( Obj_Human* pHuman );
	ZoneID_t		CalcZoneID( const WORLD_POS* pos ) ;

	// ZONE操作OBJECT相关
	BOOL			ObjZone_Register( Obj *pObj, ZoneID_t idZone );
	BOOL			ObjZone_Unregister( Obj *pObj, ZoneID_t idZone );
	BOOL			ObjZone_Changed( Obj *pObj, ZoneID_t idNew, ZoneID_t idOld );

	//判断是否人数达到饱和上限
	BOOL			IsCanEnter( ) ;
	//判断是否人数完全饱和
	BOOL			IsFull( ) ;
	//判断用户有无进入场景的权限, 0-不能进，1-可以进入，2-启动备份场景
	INT				CheckEnter( PlayerID_t PlayerID ) ;

	VOID			GetRectInRadius( VRECT* pRect, INT nRadius, ZoneID_t zid ) ;

	ObjID_t			CreateTempMonster(const	_OBJ_MONSTER_INIT* pMonsterInit ) ;
	BOOL			DeleteTempMonster(Obj_Monster* pMonster);

	Obj*			NewObject( Obj::ObjType eType );
	VOID			DeleteObject( Obj *pObj );

	inline VOID		SetSceneLoad_Map( const CHAR* szMap ) ;
	inline VOID		SetSceneLoad_Monster( const CHAR* szMonster ) ;
	inline VOID		SetSceneLoad_Platform( const CHAR* szPlatform ) ;
	inline VOID		SetSceneLoad_GrowPointData( const CHAR* szGrowPointData ) ;
	inline VOID		SetSceneLoad_GrowPointSetup( const CHAR* szGrowPointSetup ) ;
	inline VOID		SetSceneLoad_Area( const CHAR* szArea ) ;
	inline VOID		SetSceneLoad_Pet( const CHAR* szPet ) ;
	inline VOID		SetSceneLoad_PatrolPointData( const CHAR* szPatrolPointData ) ;
	
	inline VOID		SetCopySceneData_SourSceneID( const SceneID_t id ) ;
	inline VOID		SetCopySceneData_TeamLeader( const GUID_t leader ) ;
	inline VOID		SetCopySceneData_NoUserCloseTime( const UINT closetime ) ;
	inline VOID		SetCopySceneData_Timer( const UINT scenetimer ) ;
	inline VOID		SetCopySceneData_Param( const UINT Index, const INT value ) ;
	
	inline SceneID_t	GetCopySceneData_SourSceneID( ) ;
	inline GUID_t		GetCopySceneData_TeamLeader( ) ;
	inline UINT			GetCopySceneData_NoUserCloseTime( ) ;
	inline UINT			GetCopySceneData_Timer( ) ;
	inline INT			GetCopySceneData_Param( const UINT Index ) ;

	inline INT			GetCopyScene_HumanCount( ) ;
	inline ObjID_t		GetCopyScene_HumanObjID( INT nIndex ) ;
	// 0~99的随机数，每个场景就一个Rand Table.
	INT					GetRand100(VOID) {return m_Rand100.GetRand();};

	
	CHAR*			GetMapName(){ return m_szMapName ; }
	//判断当前场景是否为副本场景 普通游戏场景 城市场景, enum SCENE_TYPE
	VOID			SetSceneType( INT type ){ m_SceneType = type ; } ;
	INT				GetSceneType( )const{ return m_SceneType ; } ;

	VOID			BeginSceneTimer( UINT uTerm, UINT uNowTime ) ;
	VOID			EndSceneTimer( ) ;

	// 发送普通邮件
	VOID			SendNormalMail(const Obj_Human* pHuman, const CHAR* szReceiverName, const CHAR* szContent);

	// 发送系统邮件
	VOID			SendSystemMail(const CHAR* szReceiverName, const CHAR* szContent);

	// 发送可执行邮件
	VOID			SendScriptMail(const CHAR* szReceiverName, UINT uParam0, UINT uParam1, UINT uParam2, UINT uParam3 );

	//执行一个monster.ini文件，产生一群预定义好的怪
	BOOL			ExecuteMonsterIni( const CHAR* szFileName ) ;

	//外部获得此场景的城市信息
	CITYSCENE_DATA* GetCityData(){return &m_CityData;}

	CHAR*			FormatMissionString(const CHAR* strIn, CHAR* strOut, const Obj_Human* pHuman, const _MISSION_LIST_t* pMis, const _MISSION_DATA_t* pData, const _MISSION_REWARD_t* pReward, BOOL& bConvert);


/////////////////////////////////////////////////////////////////////////////////
//以下为数据区，所有成员变量定义在下面
/////////////////////////////////////////////////////////////////////////////////
protected :
	//当前场景的状态，见SCENE_STATUS
	INT						m_nSceneStatus ;
	//当前场景的地图信息
	Map*					m_pMap ;

	//当前地图中的区域信息
	Zone*					m_pZone ;
	_ZONE_INFO				m_ZoneInfo ;

	//当前场景的场景ID
	SceneID_t				m_SceneID ;

	//当前场景的用户群管理模块指针
	ScenePlayerManager*		m_pScenePlayerManager ;

	//当前场景等待回收的用户指针
	RecyclePlayerManager*	m_pRecyclePlayerManager;

	//OBJ Manager负责集中管理当前场景里的所有OBJ的信息，
	//可以通过ObjID查询到当前OBJ的指针数据
	//OBJ Manager不负责创建和销毁OBJ的数据内容
	ObjManager*				m_pObjManager ;

	//Obj_Human Manager负责处理当前场景里的所有角色的逻辑处理
	HumanManager*			m_pHumanManager ;

	//Obj_Monster Manager负责处理当前场景里的所有的怪物的逻辑处理
	MonsterManager*			m_pMonsterManager ;

	//Obj_Pet Manager负责处理当前场景里的所有的Obj_Pet的逻辑处理
	PetManager*				m_pPetManager ;

	//Obj_ItemBox Manager 处理当前场景中所有物品盒的逻辑
	ItemBoxManager*			m_pItemBoxManager;

	//Obj_Platform Manager 处理当前场景中所有的操作台
	PlatformManager*		m_pPlatformManager;

	//StallInfoManager		处理当前场景中摆摊的收费价格
	StallInfoManager*		m_pStallInfoManager;

	//PlayerShopManager		管理当前场景中所有的玩家商店,这只是一个指针数组，最大数量不用读了
	PlayerShopManager*		m_pPlayerShopManager;

	//Obj_Special Manager 处理当前场景中所有的法术OBJ
	SpecialManager*			m_pSpecialManager;

	StoreManager*			m_pStoreManager;
	
	// Event Zone Manager
	AreaManager*			m_pAreaManager;

	// Combat event 管理器
	EventCore_T				m_EventCore;

	GrowPointManager		m_GrowPointGroup;
	LuaInterface*           m_pLuaInterface;
	CFileDataMgr* 			m_pScriptFileMgr;
	SceneTimer*				m_pSceneTimers;
	PatrolPathMgr*			m_pPatrolPathMgr;
	ChatPipe*				m_pChatPipe;
	CMyTimer				m_QuitTimer ;
	CMyTimer				m_SceneTimer ;
	INT						m_SceneType ;//enum SCENE_TYPE
	CMyTimer				m_CopySceneQuitTimer ;


private :
	//当前场景的消息缓存
	ASYNC_PACKET*			m_PacketQue ;
	uint					m_QueSize ;
	uint					m_Head ;
	uint					m_Tail ;
	_100_PER_RANDOM_TABLE	m_Rand100 ; //每个线程唯一的随机数生成器
public:
	// 本场景的共用变量
	Packet					*m_pPacket_NewPlayer;
	Packet					*m_pPacket_NewPlayer_Move;
	Packet					*m_pPacket_NewPlayer_Death;
	Packet					*m_pPacket_NewMonster;
	Packet					*m_pPacket_NewMonster_Move;
	Packet					*m_pPacket_NewMonster_Death;
	Packet					*m_pPacket_NewPet;
	Packet					*m_pPacket_NewPet_Move;
	Packet					*m_pPacket_NewPet_Death;
	Packet					*m_pPacket_NewPlatform;
	Packet					*m_pPacket_NewSpecial;
	Packet					*m_pPacket_NewItemBox;
	Packet					*m_pPacket_DelObject;
	Packet					*m_pPacket_SysMsg; //系统广播;
	MisBuf					mMisBuf;
	_ITEM_LIST				mItemList;
	TID						m_ThreadID ;//场景的线程号
	MISSION_DATA			*m_pMissionData;
	SCENE_LOAD				m_SceneLoad ;//场景中的初始化信息
	SCENE_INIT_DATA			m_SceneInitData ;//场景中的附加信息
	CHAR					m_szMapName[_MAX_PATH] ;//地图名称
	COPYSCENE_DATA			m_CopyData ;
	CITYSCENE_DATA			m_CityData;//城市的信息
	SCENE_PERFOR			m_Perfor ;//死锁检测
	X_PARAM					m_XParam ;//UICommand参数
	HUMANLIST				m_aHumanList ;//用于脚本缓存扫描数据
	_ObjID_List				m_ObjIDList ;//用户脚本缓存扫描数据
};

#include "Scene.inl"


#endif
