#include "stdafx.h"
// Server.cpp : Defines the entry point for the console application.
// 

#include "Server.h"
#include "Log.h"
#include "Ini.h"
#include "FileDef.h"
#include "TimeManager.h"
#include "ClientManager.h"
#include "SceneManager.h"
#include "ThreadManager.h"
#include "PlayerPool.h"
#include "Config.h"
#include "Thread.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "WorldManager.h"
#include "MachineManager.h"
#include "ObjManager.h"
#include "GUIDManager.h"
#include "GameTable.h"
#include "MonsterPool.h"
#include "PetPool.h"
#include "PlatformPool.h"
#include "SpecialPool.h"
#include "ItemTable.h"
#include "ItemManager.h"
#include "AbilityManager.h"
#include "ShopManager.h"
#include "CopySceneManager.h"
#include "DynamicSceneManager.h"
#include "SMUManager.h"
#include "ShareMemManager.h"
#include "DiePenaltyManager.h"
#include "ItemBoxPool.h"
#include "Performance.h"
#include "DaemonThread.h"
#include "PlayerShopPool.h"


extern INT					g_Command_CommandDeny ;//控制参数，禁止使用GM指令
extern INT					g_Command_AlwaysLoadScript ;//每次执行脚本都重新Load
extern INT					g_Command_AlwaysLoadMonsterIni ;//每次都读取monster.ini文件
extern SMUPool<HumanSMU>	g_HumanSMUPool;

Server						g_Server;

INT main(INT argc, CHAR* argv[])
{

#if defined(__WINDOWS__)
	_CrtSetDbgFlag(_CrtSetDbgFlag(0) | _CRTDBG_LEAK_CHECK_DF);
#endif

__ENTER_FUNCTION
	if( argc>1 )
	{
		for( int i=1; i<argc; i++ )
		{
			if( strcmp(argv[i],"-ignoreassert")==0 )
			{
				g_Command_Assert=1 ;
			}
			else if( strcmp(argv[i],"-retryassert")==0 )
			{
				g_Command_Assert=2 ;
			}

			if( strcmp(argv[i],"-ignoremessagebox")==0 )
			{
				g_Command_IgnoreMessageBox=TRUE ;
			}
			if( strcmp(argv[i],"-commanddeny")==0 )
			{
				g_Command_CommandDeny=TRUE ;
			}
			if( strcmp(argv[i],"-loadscriptonce")==0 )
			{
				g_Command_AlwaysLoadScript=FALSE ;
			}
			if( strcmp(argv[i],"-alwaysloadmonsterini")==0  )
			{
				g_Command_AlwaysLoadMonsterIni=TRUE ;
			}
		}
	}

	BOOL ret ;

	//初始化配置文件模块
	ret = g_Server.InitConfig();
	Assert(ret) ;

//时间模块需要最开始的时候设置
	g_pTimeManager = new TimeManager ;
	Assert( g_pTimeManager ) ;
	ret = g_pTimeManager->Init( ) ;
	Assert(ret) ;

//日志模块初始化需要最开始设置
	g_pLog = new Log ;
	Assert( g_pLog ) ;
	ret = g_pLog->Init( ) ;
	Assert(ret) ;


	Log::SaveLog( SERVER_LOGFILE, "\r\n(###) main..." ) ;

	g_pTimeManager->SetTime( ) ;
	Log::SaveLog( SERVER_LOGFILE, "Server Starting... (%.10d)(%d)",
		g_pTimeManager->Time2DWORD(),
		g_pTimeManager->StartTime() ) ;

	Log::SaveLog( SERVER_LOGFILE, "_ITEM=%d _ITEM_VALUE=%d _ITEM_GUID=%d", 
		sizeof(_ITEM), sizeof(_ITEM_VALUE), sizeof(_ITEM_GUID) ) ;
	Log::SaveLog( SERVER_LOGFILE, "_OWN_IMPACT=%d", sizeof(_OWN_IMPACT) ) ;
	Log::SaveLog( SERVER_LOGFILE, "_HUMAN_DB_LOAD=%d \r\n\
_EQUIP_DB_LOAD=%d \r\n\
_BAG_DB_LOAD=%d \r\n\
_BANK_DB_LOAD=%d \r\n\
_SKILL_DB_LOAD=%d \r\n\
_COOLDOWN_DB_LOAD=%d \r\n\
_XINFA_DB_LOAD=%d \r\n\
_IMPACT_DB_LOAD=%d \r\n\
_ABILITY_DB_LOAD=%d \r\n\
_MISSION_DB_LOAD=%d \r\n\
_SETTING_DB_LOAD=%d \r\n\
_PET_DB_LIST_LOAD=%d \r\n\
_RELATION_DB_LOAD=%d \r\n\
_PRIVATE_INFO_DB_LOAD=%d \r\n\
FULLUSERDATA=%d", 
		sizeof(_HUMAN_DB_LOAD), 
		sizeof(_EQUIP_DB_LOAD), 
		sizeof(_BAG_DB_LOAD) ,
		sizeof(_BANK_DB_LOAD) ,
		sizeof(_SKILL_DB_LOAD) ,
		sizeof(_COOLDOWN_DB_LOAD_FOR_HUMAN) ,
		sizeof(_XINFA_DB_LOAD) ,
		sizeof(_IMPACT_DB_LOAD) ,
		sizeof(_ABILITY_DB_LOAD) ,
		sizeof(_MISSION_DB_LOAD) ,
		sizeof(_SETTING_DB_LOAD) ,
		sizeof(_PET_DB_LIST_LOAD) ,
		sizeof(_RELATION_DB_LOAD) ,
		sizeof(_PRIVATE_INFO_DB_LOAD),
		sizeof(FULLUSERDATA)
		) ;
	if( sizeof(FULLUSERDATA)!=36513)//34977)//34081)//35325)//34013)
//		                           ^技能结构 ^技能数
	{
		AssertEx(FALSE,"用户档案数据尺寸发生变化，请联系相关人员说明相关情况！") ;
	}
	

	ret = g_Server.InitServer( ) ;
	Assert( ret ) ;

	ret = g_Server.Loop( ) ;
	Assert( ret ) ;

	ret = g_Server.ExitServer( ) ;
	Assert( ret ) ;
	
	

__LEAVE_FUNCTION
	
	return 0;
}

Server::Server( )
{
__ENTER_FUNCTION

#if defined(__WINDOWS__)
	WORD wVersionRequested;
	WSADATA wsaData;
	INT err;
	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData ); 
#endif

	m_TimeToQuit.CleanUp() ;

__LEAVE_FUNCTION
}

Server::~Server( )
{
__ENTER_FUNCTION

#if defined(__WINDOWS__)
	WSACleanup();
#endif

__LEAVE_FUNCTION

}

//___________________________________
BOOL Server::InitConfig()
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;
	ret = g_Config.Init( ) ;
	Assert( ret ) ;
	return TRUE ;

__LEAVE_FUNCTION

	return TRUE ;
}
//___________________________________

BOOL Server::InitServer( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;
	Log::SaveLog( SERVER_LOGFILE, "InitServer FD_SETSIZE=%d...", FD_SETSIZE ) ;

	//ret = g_Config.Init( ) ;
	//Assert( ret ) ;


//分配内存
	ret = NewStaticServer( ) ;
	Assert( ret ) ;



//初始化各个模块
	ret = InitStaticServer( ) ;
	Assert( ret ) ;



__LEAVE_FUNCTION

	return TRUE ;
}

BOOL Server::Loop( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;

	Log::SaveLog( SERVER_LOGFILE, "\r\nLoop..." ) ;


	Log::SaveLog( SERVER_LOGFILE, "g_pThreadManager->Start( )..." ) ;
	ret = g_pThreadManager->Start( ) ;
	Assert( ret ) ;

	MySleep( 1500 ) ;

	//主线程调度资源分给ClientManager来执行；
	Log::SaveLog( SERVER_LOGFILE, "g_pClientManager->Loop( )..." ) ;
	g_pClientManager->start( ) ;


	Log::SaveLog( SERVER_LOGFILE, "g_pDaemonThread->Loop( )..." ) ;
	g_pDaemonThread->Loop( ) ;


__LEAVE_FUNCTION

	return TRUE ;
}

BOOL Server::ExitServer( )
{
__ENTER_FUNCTION

	Log::SaveLog( SERVER_LOGFILE, "\r\nExitServer..." ) ;


	//////////////////
	//等待所有线程完全推出后执行清除操作
	WaitForAllThreadQuit( ) ;



	Log::SaveLog( SERVER_LOGFILE, "Begin delete..." ) ;
	//动态数据
	SAFE_DELETE( g_pClientManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pClientManager delete...OK" ) ;
	SAFE_DELETE( g_pThreadManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pThreadManager delete...OK" ) ;


	//静态数据
	SAFE_DELETE( g_pSceneManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pSceneManager delete...OK" ) ;
	SAFE_DELETE( g_pPlayerPool ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pPlayerPool delete...OK" ) ;
	SAFE_DELETE( g_pPacketFactoryManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pPacketFactoryManager delete...OK" ) ;
	SAFE_DELETE( g_pServerManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pServerManager delete...OK" ) ;
	SAFE_DELETE( g_pWorldManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pWorldManager delete...OK" ) ;
	SAFE_DELETE( g_pMachineManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pMachineManager delete...OK" ) ;
	SAFE_DELETE( g_pGUIDManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pGUIDManager delete...OK" ) ;
	if ( g_pMonsterPool != NULL )
	{
		g_pMonsterPool->Term();
		SAFE_DELETE( g_pMonsterPool ) ;
	}
	Log::SaveLog( SERVER_LOGFILE, "g_pMonsterPool delete...OK" ) ;
	if ( g_pPetPool != NULL )
	{
		g_pPetPool->Term();
		SAFE_DELETE( g_pPetPool ) ;
	}
	Log::SaveLog( SERVER_LOGFILE, "g_pPetPool delete...OK" ) ;

	SAFE_DELETE( g_pItemBoxPool ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pItemBoxPool delete...OK" ) ;

	if ( g_pDiePenaltyManager != NULL )
	{
		g_pDiePenaltyManager->CleanUp();
		SAFE_DELETE( g_pDiePenaltyManager ) ;
	}
	Log::SaveLog( SERVER_LOGFILE, "g_pDiePenaltyManager delete...OK" ) ;

	if ( g_pPlayerShopPool != NULL )
	{
		SAFE_DELETE( g_pPlayerShopPool ) ;
	}
	Log::SaveLog( SERVER_LOGFILE, "g_pPlayerShopPool delete...OK" ) ;


	//if ( g_pPortalPool != NULL )
	//{
	//	g_pPortalPool->Term();
	//	SAFE_DELETE( g_pPortalPool ) ;
	//}
	//Log::SaveLog( SERVER_LOGFILE, "g_pPortalPool delete...OK" ) ;
	/*
	if ( g_pSkillObjPool != NULL )
	{
		g_pSkillObjPool->Term();
		SAFE_DELETE( g_pSkillObjPool ) ;
	}
	Log::SaveLog( SERVER_LOGFILE, "g_pSkillObjPool delete...OK" ) ;
	*/
	SAFE_DELETE( g_pPlatformPool ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pPlatformPool delete...OK" ) ;
	SAFE_DELETE( g_pSpecialPool ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pSpecialPool delete...OK" ) ;
	SAFE_DELETE( g_pAbilityManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pAbilityManager delete...OK" ) ;
	SAFE_DELETE( g_pStaticShopManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pStaticShopManager delete...OK" ) ;
	SAFE_DELETE( g_pCopySceneManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pCopySceneManager delete...OK" ) ;
	//SAFE_DELETE( g_pDynamicSceneManager ) ;//_________________________________
	//Log::SaveLog( SERVER_LOGFILE, "g_pDynamicSceneManager delete...OK" ) ;
	SAFE_DELETE( g_pPerformanceManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pPerformanceManager delete...OK" ) ;
	SAFE_DELETE( g_pDaemonThread ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pDaemonThread delete...OK" ) ;
	SAFE_DELETE( g_pTimeManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pTimeManager delete...OK" ) ;
	SAFE_DELETE( g_pLog ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pLog delete...OK" ) ;

__LEAVE_FUNCTION

	return TRUE ;
}

VOID Server::WaitForAllThreadQuit( )
{
__ENTER_FUNCTION


#define MAX_WAIT_QUIT 300

	INT iQuit ;
	//最长时间可以等待MAX_WAIT_QUIT秒
	for( INT i=0;i<MAX_WAIT_QUIT;i++ )
	{
		iQuit = g_QuitThreadCount ;
		printf( "Quit Thread:%d", iQuit ) ;
		MySleep( 1000 ) ;
		if( iQuit==g_pThreadManager->GetTotalThreads()+1 )
			break ;
	}

__LEAVE_FUNCTION
}

BOOL Server::NewStaticServer( )
{
__ENTER_FUNCTION


	BOOL ret = FALSE ;
	Log::SaveLog( SERVER_LOGFILE, "\r\nNewStaticServer( )...OK" ) ;


	//
	g_pMonsterPool = new MonsterPool ;
	Assert( g_pMonsterPool ) ;
	Log::SaveLog( SERVER_LOGFILE, "new MonsterPool...OK" ) ;

	//
	g_pPetPool = new PetPool ;
	Assert( g_pPetPool ) ;
	Log::SaveLog( SERVER_LOGFILE, "new PetPool...OK" ) ;

	//
	g_pSceneManager = new SceneManager ;
	Assert( g_pSceneManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "new SceneManager...OK" ) ;

	//
	g_pPlayerPool = new PlayerPool ;
	Assert( g_pPlayerPool ) ;
	Log::SaveLog( SERVER_LOGFILE, "new PlayerPool...OK" ) ;

	//
	g_pPacketFactoryManager = new PacketFactoryManager ;
	Assert( g_pPacketFactoryManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "new PacketFactoryManager...OK" ) ;

	//
	g_pServerManager = new ServerManager ;
	Assert( g_pServerManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "new ServerManager...OK" ) ;

	//
	g_pWorldManager = new WorldManager ;
	Assert( g_pWorldManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "new WorldManager...OK" ) ;

	//
	g_pMachineManager = new MachineManager ;
	Assert( g_pMachineManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "new MachineManager...OK" ) ;

	//
	g_pGUIDManager = new GUIDManager ;
	Assert( g_pGUIDManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "new GUIDManager...OK" ) ;

	//new 玩家商店池
	g_pPlayerShopPool	=	new PlayerShopPool;
	Assert(g_pPlayerShopPool);
	Log::SaveLog( SERVER_LOGFILE, "new g_pPlayerShopPool...OK" ) ;

	////
	//g_pPortalPool	=	new PortalPool;
	//Assert(g_pPortalPool);
	//Log::SaveLog( SERVER_LOGFILE, "new PortalPool...OK" ) ;

	//
	g_pItemBoxPool	=	new ItemBoxPool;
	Assert(g_pItemBoxPool);
	Log::SaveLog( SERVER_LOGFILE, "new ItemBoxPool...OK" ) ;

	//
	g_pPlatformPool	=	new PlatformPool;
	Assert(g_pPlatformPool);
	Log::SaveLog( SERVER_LOGFILE, "new PlatformPool...OK" ) ;

	g_pSpecialPool	=	new SpecialPool;
	Assert(g_pSpecialPool);
	Log::SaveLog( SERVER_LOGFILE, "new SpecialPool...OK" ) ;

	//
	/*
	g_pSkillObjPool	=	new SkillObjPool;
	Assert(g_pSkillObjPool);
	Log::SaveLog( SERVER_LOGFILE, "new SkillObjPool...OK" ) ;
	*/
	//
	g_pAbilityManager = new AbilityManager;
	Assert(g_pAbilityManager);
	Log::SaveLog( SERVER_LOGFILE, "new AbilityManager...OK" ) ;

	g_pStaticShopManager = new StaticShopManager;
	Assert(g_pStaticShopManager);
	Log::SaveLog( SERVER_LOGFILE, "new StaticShopManager...OK" ) ;

	g_pCopySceneManager = new CopySceneManager;
	Assert(g_pCopySceneManager);
	Log::SaveLog( SERVER_LOGFILE, "new CopySceneManager...OK" ) ;

	//________________________________
	//g_pDynamicSceneManager = new DynamicSceneManager;
	//Assert(g_pDynamicSceneManager);
	//Log::SaveLog( SERVER_LOGFILE, "new DynamicSceneManager...OK" ) ;

	//物品管理器
	g_pItemManager = new ItemManager;
	Assert(g_pItemManager);
	Log::SaveLog( SERVER_LOGFILE, "new ItemManager...OK" ) ;

	g_pDaemonThread = new DaemonThread;
	Assert(g_pDaemonThread);
	Log::SaveLog( SERVER_LOGFILE, "new DaemonThread...OK" ) ;

	// 死亡惩罚数据
	g_pDiePenaltyManager = new DiePenaltyManager;
	Assert(g_pDiePenaltyManager);
	Log::SaveLog( SERVER_LOGFILE, "new DiePenaltyManager...OK" ) ;

	// 数据监控管理器
	g_pPerformanceManager = new PerformanceManager;
	Assert(g_pPerformanceManager);
	Log::SaveLog( SERVER_LOGFILE, "new PerformanceManager...OK" ) ;


	//***注意***
	//
	//  以下两个模块放在最后生成
	//
	g_pThreadManager = new ThreadManager ;
	Assert( g_pThreadManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "new pThreadManager...OK" ) ;

	//
	g_pClientManager = new ClientManager ;
	Assert( g_pClientManager ) ;
	Log::SaveLog( SERVER_LOGFILE, "new ClientManager...OK" ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID Server::Stop( )
{
__ENTER_FUNCTION

	if( g_pThreadManager )
	{
		g_pThreadManager->Stop( )  ;
	}

	if( g_pClientManager )
	{
		g_pClientManager->stop( ) ;
	}

	if( g_pDaemonThread )
	{
		g_pDaemonThread->Stop( ) ;
	}

__LEAVE_FUNCTION
}

BOOL Server::InitStaticServer( )
{
__ENTER_FUNCTION

	BOOL ret = FALSE ;
	INT nTemp = 0;

	Log::SaveLog( SERVER_LOGFILE, "\r\nInitStaticServer( )...OK" ) ;

	//________________________________________________________
	if( g_Config.m_ConfigInfo.m_SystemModel == 0 )
	{
		nTemp = 10;
	}
	else
	{
		nTemp = MAX_POOL_SIZE;
	}
//ShareMemory 最先
	
	const _SERVER_DATA*	pCurrentSData =	g_pServerManager->GetCurrentServerInfo();
	
	if(pCurrentSData->m_EnableShareMem)
	{
		ret = g_HumanSMUPool.Init(/*g_pPlayerPool->GetPlayerPoolMaxCount()*//*________________*/nTemp,pCurrentSData->m_HumanSMKey,SMPT_SERVER);
		AssertEx(ret,"共享内存初始化错误,请先启动ShareMemory");

		if(g_HumanSMUPool.GetHeadVer()!=0)
		{
			AssertEx(ret,"ShareMemory 存盘尚未完成，请稍后启动Server");
		}

		ret = g_PlayerShopSMUPool.Init(MAX_PLAYER_SHOP_POOL_PER_SERVER,pCurrentSData->m_PlayShopSMKey,SMPT_SERVER);
		AssertEx(ret,"共享内存初始化错误,请先启动ShareMemory");

		if(g_PlayerShopSMUPool.GetHeadVer()!=0)
		{
			AssertEx(ret,"ShareMemory 存盘尚未完成，请稍后启动Server");
		}

		ret = g_ItemSerialSMUPool.Init(1,pCurrentSData->m_ItemSerialKey,SMPT_SERVER);
		AssertEx(ret,"共享内存初始化错误,请先启动ShareMemory");

		if(g_ItemSerialSMUPool.GetHeadVer()!=0)
		{
			AssertEx(ret,"ShareMemory 存盘尚未完成，请稍后启动Server");
		}


	}

//先表格数据
	//
	ret = g_ItemTable.Init();
	Assert(ret);
	Log::SaveLog( SERVER_LOGFILE, "g_ItemTable.Init()...OK" ) ;

	//
	ret = g_TableInit.Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_TableInit.Init...OK" ) ;



//后逻辑模块数据
	//________________________________________________________
	if( g_Config.m_ConfigInfo.m_SystemModel == 0 )
	{
		nTemp = 2000;
	}
	else
	{
		nTemp = g_Config.m_ConfigInfo.m_MaxMonster;
	}
	ret = g_pMonsterPool->Init( nTemp ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pMonsterPool->Init()...OK" ) ;

	//________________________________________________________
	if( g_Config.m_ConfigInfo.m_SystemModel == 0 )
	{
		nTemp = 10;
	}
	else
	{
		nTemp = g_Config.m_ConfigInfo.m_MaxPet;
	}
	ret = g_pPetPool->Init( nTemp ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pPetPool->Init()...OK" ) ;

	//________________________________________________________
	if( g_Config.m_ConfigInfo.m_SystemModel == 0 )
	{
		nTemp = 10;
	}
	else
	{
		nTemp = MAX_POOL_SIZE;
	}
	ret = g_pPlayerPool->Init( nTemp ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pPlayerPool->Init()...OK" ) ;

	//
	ret = g_pPacketFactoryManager->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pPacketFactoryManager->Init()...OK" ) ;

	//
	ret = g_pWorldManager->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pWorldManager->Init()...OK" ) ;

	//
	ret = g_pMachineManager->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pMachineManager->Init()...OK" ) ;

	//
	ret = g_pGUIDManager->Init( ) ;
	Assert( ret );
	Log::SaveLog( SERVER_LOGFILE, "g_pGUIDManager->Init()...OK" ) ;
	
	//物品盒初始化
	//________________________________________________________
	if( g_Config.m_ConfigInfo.m_SystemModel == 0 )
	{
		nTemp = 1000;
	}
	else
	{
		nTemp = MAX_ITEM_BOX_POOL;
	}
	ret  =  g_pItemBoxPool->Init( nTemp );
	Assert(ret);
	Log::SaveLog( SERVER_LOGFILE, "g_pItemBoxPool->Init()...OK" ) ;

	//玩家商店池初始化
	//________________________________________________________
	if( g_Config.m_ConfigInfo.m_SystemModel == 0 )
	{
		nTemp = 5;
	}
	else
	{
		nTemp = g_Config.m_ConfigInfo.m_MaxPlayerShopNum;
	}
	ret  =  g_pPlayerShopPool->Init( nTemp );
	Assert(ret);
	Log::SaveLog( SERVER_LOGFILE, "g_pPlayerShopPool->Init()...OK" ) ;

	////
	//ret = g_pPortalPool->Init( g_Config.m_ConfigInfo.m_MaxPortal ) ;
	//Assert( ret ) ;
	//Log::SaveLog( SERVER_LOGFILE, "g_pPortalPool->Init( )...OK" ) ;

	//
	ret = g_pPlatformPool->Init( g_Config.m_ConfigInfo.m_MaxPlatform );
	Assert( ret );
	Log::SaveLog( SERVER_LOGFILE, "g_pPlatformPool->Init( )...OK" );

	//
	ret = g_pSpecialPool->Init( g_Config.m_ConfigInfo.m_MaxSpecial );
	Assert( ret );
	Log::SaveLog( SERVER_LOGFILE, "g_pSpecialPool->Init( )...OK" );

	//
	ret = g_pAbilityManager->Init();
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pAbilityManager->Init()...OK" ) ;

	//
	ret = 	g_pStaticShopManager->Init();
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "m_pStaticShopManager->Init()...OK" ) ;

	//
	ret = g_pCopySceneManager->Init();
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pCopySceneManager->Init()...OK" ) ;

	//玩家自建城市等___________________________
	//ret = g_pDynamicSceneManager->Init();
	//Assert( ret ) ;
	//Log::SaveLog( SERVER_LOGFILE, "g_pDynamicSceneManager->Init()...OK" ) ;

	//
	/*
	ret = g_pSkillObjPool->Init( g_Config.m_ConfigInfo.m_MaxSkillObj ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pSkillObjPool->Init( )...OK" ) ;
	*/
	//
	//________________________________________________________
	if( g_Config.m_ConfigInfo.m_SystemModel == 0 )
	{
		nTemp = 10;
	}
	else
	{
		nTemp = g_Config.m_SceneInfo.m_SceneCount;
	}
	ret = g_pSceneManager->Init( nTemp ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_SceneManager->Init()...OK" ) ;
	
	//	
	ret = g_pItemManager->Init();
	Assert(ret);
	Log::SaveLog( SERVER_LOGFILE, "g_pItemManager->Init()...OK" ) ;

	//死亡惩罚
	ret = g_pDiePenaltyManager->Init( 1024 );
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pDiePenaltyManager->Init()...OK" ) ;

	//平台管理工具，保存和统计系统中各个模块执行效率及运行情况
	ret = g_pPerformanceManager->Init( );
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pPerformanceManager->Init()...OK" ) ;

	/************************************************************************/
	/*
	各个管理器,池之间相关数据的连接,有些数据是从sharemem中倒入到池中的，
	需要根据这些数据的属性连接到指定的管理器
    */
	/************************************************************************/
	ret = g_pPlayerShopPool->ConnectTo(g_pSceneManager);
	Assert( ret );
	Log::SaveLog( SERVER_LOGFILE, "g_pPlayerShopPool->ConnectTo(g_pSceneManager)...OK" ) ;

	//***注意***
	//
	//放在最后初始化的是线程数据
	//
	ret = g_pServerManager->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pServerManager->Init()...OK" ) ;

	//根据配置文件读取需要使用的场景，为每个场景分配一个线程；
	if( g_Config.m_ConfigInfo.m_SystemModel == 0 )
	{
		nTemp = 10;
	}
	else
	{
		nTemp = g_Config.m_SceneInfo.m_SceneCount;
	}
	ret = g_pThreadManager->Init( nTemp ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pThreadManager->Init()...OK" ) ;

	//
	ret = g_pClientManager->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pClientManager->Init()...OK" ) ;

	//
	ret = g_pDaemonThread->Init( ) ;
	Assert( ret ) ;
	Log::SaveLog( SERVER_LOGFILE, "g_pDaemonThread->Init()...OK" ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}


VOID INTHandler(INT)
{
	DumpStack("INT exception:\r\n");
	g_Server.Stop();
}
VOID TERMHandler(INT)
{
	DumpStack("TERM exception:\r\n");
	g_Server.Stop();

}
VOID ABORTHandler(INT)
{
	DumpStack("ABORT exception:\r\n");
	g_Server.Stop();
}

VOID ILLHandler(INT)
{
	DumpStack("ILL exception:\r\n");
	g_Server.Stop();
}

VOID FPEHandler(INT)
{
	DumpStack("FPE exception:\r\n");
	g_Server.Stop();
}

VOID SEGHandler(INT)
{
	DumpStack("SEG exception:\r\n");
	g_Server.Stop();
}
VOID XFSZHandler(INT)
{
	DumpStack("XFSZ exception:\r\n");
	g_Server.Stop();
}

ServerExceptionHandler::ServerExceptionHandler()
{
#ifdef __LINUX__
	signal(SIGSEGV, SEGHandler);
	signal(SIGFPE,  FPEHandler);
	signal(SIGILL,  ILLHandler);
	signal(SIGINT,  INTHandler);  
	signal(SIGTERM, TERMHandler); 
	signal(SIGABRT, ABORTHandler);
	signal(SIGXFSZ, XFSZHandler);
#endif
}

ServerExceptionHandler g_ServerExceptionHandler;
