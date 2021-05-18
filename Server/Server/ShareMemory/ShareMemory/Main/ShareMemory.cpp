#include "stdafx.h"
#include "ShareMemory.h"
#include "Log.h"
#include "Config.h"
#include "TimeManager.h"
#include "SMUManager.h"
#include "SMULogicManager.h"
#include "ShareDBManager.h"
#include "ODBCInterface.h"
#include "CommandThread.h"
#include "PlayerShopDefine.h"

// addition 
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <fileapi.h>
//

#if __LINUX__
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <errno.h>
#elif __WINDOWS__
#include <WinBase.h>
#endif


ShareMemory			g_ShareMemory ;

using namespace PLAYER_SHOP;
INT					g_CmdInput;


BOOL				CheckSaveAllFile();
BOOL				CheckExitFile();

//int main(int argc, char* argv[])
int __cdecl _tmain(int argc, char* argv[])
{
#if defined(__WINDOWS__)
	_CrtSetDbgFlag(_CrtSetDbgFlag(0) | _CRTDBG_LEAK_CHECK_DF);
#endif
	g_CmdInput = FALSE;

	if( argc>1 )
	{
		for( int i=1; i<argc; i++ )
		{
			if( strcmp(argv[i],"-clear")==0 )
			{
				g_CmdArgv = CMD_MODE_CLEARALL ;
				printf("Cmd enter clear mode!\r\n");
			}
			if(strcmp(argv[i],"-loaddump")==0)
			{
				g_CmdArgv = CMD_MODE_LOADDUMP;
				printf("Cmd enter loaddump mode!\r\n");
			}
			if(strcmp(argv[i],"-enablecmd")==0)
			{	
				g_CmdInput	= TRUE;

			}
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
					
		}
	}
	
 
	g_pTimeManager = new TimeManager ;
	Assert( g_pTimeManager ) ;
	g_pTimeManager->Init( ) ;

	BOOL bRet ;

	Log::SaveLog( "./Log/ShareMemory.log", "(###) main...\r\n" ) ;
	Log::SaveLog( "./Log/ShareMemory.log", "ShareMemory Start...\r\n" ) ;

	bRet = g_ShareMemory.Init();
	if( bRet==FALSE )
	{
		Assert(FALSE) ;
		return 1 ;
	}
	
	bRet = g_ShareMemory.Loop();
	if( bRet==FALSE )
	{
		Assert(FALSE) ;
		return 1 ;
	}

	bRet = g_ShareMemory.Exit();
	if( bRet==FALSE )
	{
		Assert(FALSE) ;
		return 1 ;
	}
	
	SAFE_DELETE(g_pTimeManager);

	return 0 ;
}

ShareMemory::ShareMemory( ) : m_bExited(FALSE)
{

	
}

ShareMemory::~ShareMemory( )
{
}

BOOL ShareMemory::Init( )
{

	BOOL bRet ;

	Log::SaveLog( "./Log/ShareMemory.log", "Start Read Config Files..." ) ;
	bRet = g_Config.Init( ) ;
	Assert( bRet ) ;

	Log::SaveLog( "./Log/ShareMemory.log", "Read Config Files...OK!" ) ;
	
	
	Log::SaveLog( "./Log/ShareMemory.log", "Start New Managers..." ) ;
	bRet = NewStaticManager( ) ;
	Assert( bRet ) ;
	Log::SaveLog( "./Log/ShareMemory.log", "New Managers...OK!" ) ;

	Log::SaveLog( "./Log/ShareMemory.log", "Start Init Managers..." ) ;
	bRet = InitStaticManager( ) ;
	Assert( bRet ) ;
	Log::SaveLog( "./Log/ShareMemory.log", "Init Managers...OK!" ) ;
	return TRUE ;
}

BOOL ShareMemory::Loop( )
{


	if(g_CmdInput)
	{
		g_CommandThread.start();
		Log::SaveLog( "./Log/ShareMemory.log", "g_CommandThread.start()" ) ;
	}

	Log::SaveLog( "./Log/ShareMemory.log", "Loop...Start" ) ;
	while(TRUE)
	{
		DoWork();
		MySleep(1000);
	}
	Log::SaveLog( "./Log/ShareMemory.log", "Loop...End" ) ;

	return TRUE ;
}

BOOL ShareMemory::Exit( )
{
	DelStaticManager();
	m_bExited= FALSE;
	return TRUE ;
}

BOOL ShareMemory::DoWork()
{
	
	_MY_TRY
	{
		
		BOOL  bExit = FALSE;

		ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
		Assert(pInterface);

		if(!pInterface->IsConnected())
		{
			Log::SaveLog("./Log/ShareMemory.log","connect database is fails");
			BOOL bRet =	pInterface->Connect();
			if(!bRet)
			{
				Log::SaveLog("./Log/ShareMemory.log","Can't connect database");
				MySleep(5000);
				if(g_CommandThread.m_SMConfig.Commands.cmdType==CMD_UNKNOW)
				{
					return FALSE;
				}
			}
			else
			{
				Log::SaveLog("./Log/ShareMemory.log","Reconnect database");
			}
		}
		if(CheckSaveAllFile())
		{
			g_CommandThread.m_SMConfig.Commands.cmdType = CMD_SAVE_ALL;
		}

		if(CheckExitFile())
		{
			g_CommandThread.m_SMConfig.Commands.cmdType = CMD_SAVE_ALL;
			bExit = TRUE;
			Log::SaveLog("./Log/ShareMemory.log","Get Exit Commands");
		}
		if(g_CommandThread.m_SMConfig.Commands.cmdType!=CMD_UNKNOW)
		{
			g_SMConfig = g_CommandThread.m_SMConfig ;
			g_CommandThread.m_SMConfig.Commands.cmdType=CMD_UNKNOW;
		}

		for(uint i = 0;i<MAX_SM_OBJ_NUM;i++)
		{
			if(m_SMULogicManager[i].m_SMULogicManager)
			{
				SHAREMEM_TYPE	Type = m_SMULogicManager[i].m_Type;

				switch(Type) 
				{
				case ST_HUMAN_SMU:
					{
						SMULogicManager<HumanSMU>* pHumanSMULogicMgr = static_cast<SMULogicManager<HumanSMU>*>(m_SMULogicManager[i].m_SMULogicManager);
						if(pHumanSMULogicMgr)
							pHumanSMULogicMgr->HeartBeat();
						else
						{
							AssertEx(FALSE,"Runtime Type error" );
						}

						break;
					}
				case ST_GUILD_SMU:
					{
						SMULogicManager<GuildSMU>* pGuildSMULogicMgr = static_cast<SMULogicManager<GuildSMU>*>(m_SMULogicManager[i].m_SMULogicManager);
						if(pGuildSMULogicMgr)
							pGuildSMULogicMgr->HeartBeat();
						else
						{
							AssertEx(FALSE,"Runtime Type error" );
						}

						break;
					}
				case ST_MAIL_SMU:
					{
						SMULogicManager<MailSMU>* pMailSMULogicMgr = static_cast<SMULogicManager<MailSMU>*>(m_SMULogicManager[i].m_SMULogicManager);
						if(pMailSMULogicMgr)
							pMailSMULogicMgr->HeartBeat();
						else
						{
							AssertEx(FALSE,"Runtime Type error" );
						}

						break;	
					}
				case ST_PSHOP_SMU:
					{
						SMULogicManager<PlayerShopSM>* pPShopSMULogicMgr = static_cast<SMULogicManager<PlayerShopSM>*>(m_SMULogicManager[i].m_SMULogicManager);
						if(pPShopSMULogicMgr)
							pPShopSMULogicMgr->HeartBeat();
						else
						{
							AssertEx(FALSE,"Runtime Type error" );
						}
						break;
					}

				case ST_ITEMSERIAL_SMU:
					{
						SMULogicManager<ItemSerialKeySMU>* pItemSerialSMULogicMgr = static_cast<SMULogicManager<ItemSerialKeySMU>*>(m_SMULogicManager[i].m_SMULogicManager);
						if(pItemSerialSMULogicMgr)
							pItemSerialSMULogicMgr->HeartBeat();
						else
						{
							AssertEx(FALSE,"Runtime Type error" );
						}
						break;

					}
				case ST_USERDATA_SMU:
					{
						AssertEx(FALSE,"Unknow SMU Type");
						break;
					}
				case  ST_HUMANEXT_SMU:
					{
						AssertEx(FALSE,"Unknow SMU Type");
						break;
					}
				default:
					{
						AssertEx(FALSE,"Unknow SMU Type");
					}
				}

			}
		}

		if(g_SMConfig.Commands.cmdType == CMD_CLEAR_ALL)
		{
			exit(0);
		}

		
		g_SMConfig.Commands.cmdType = CMD_UNKNOW;

		if(bExit)
		{
			Log::SaveLog( "./Log/ShareMemory.log", "Exit ShareMemory Program");
			exit(0);
		}

		return TRUE;

	}
	_MY_CATCH
	{
		Log::SaveLog( "./Log/ShareMemory.log", "ShareMemory::DoWork() Get Error") ;
	
	}

	return FALSE;

}



BOOL ShareMemory::NewStaticManager( )
{
	__ENTER_FUNCTION

	g_pDBManager	=	 new	ShareDBManager();
	AssertEx(g_pDBManager,"分配数据库管理器失败!");
	Log::SaveLog("./Log/ShareMemory.log","new ShareDBManager ...OK");

	
	for(uint i =0;i< g_Config.m_ShareMemInfo.m_SMUObjCount;i++)
	{
		m_SMUPool[i].m_Data = g_Config.m_ShareMemInfo.m_pShareData[i];

		SHAREMEM_TYPE type = g_Config.m_ShareMemInfo.m_pShareData[i].m_Type;
		switch(type) 
		{
		case ST_HUMAN_SMU:
			{
				m_SMUPool[i].m_Pool = new SMUPool<HumanSMU>();
				m_SMULogicManager[i].m_SMULogicManager = new SMULogicManager<HumanSMU>();
				m_SMULogicManager[i].m_Type	=	ST_HUMAN_SMU;
			}
			break;
		case ST_GUILD_SMU:
			{
				m_SMUPool[i].m_Pool = new SMUPool<GuildSMU>();
				m_SMULogicManager[i].m_SMULogicManager = new SMULogicManager<GuildSMU>();
				m_SMULogicManager[i].m_Type	=	ST_GUILD_SMU;
			}
			break;
		case ST_MAIL_SMU:
			{
				m_SMUPool[i].m_Pool = new SMUPool<MailSMU>();
				m_SMULogicManager[i].m_SMULogicManager = new SMULogicManager<MailSMU>();
				m_SMULogicManager[i].m_Type	=	ST_MAIL_SMU;	
			}
			break;
		case ST_PSHOP_SMU:
			{
				m_SMUPool[i].m_Pool = new SMUPool<PlayerShopSM>();
				m_SMULogicManager[i].m_SMULogicManager = new SMULogicManager<PlayerShopSM>();
				m_SMULogicManager[i].m_Type	=	ST_PSHOP_SMU;	
			}
			break;
		case ST_ITEMSERIAL_SMU:
			{
				m_SMUPool[i].m_Pool = new SMUPool<ItemSerialKeySMU>();
				m_SMULogicManager[i].m_SMULogicManager = new SMULogicManager<ItemSerialKeySMU>();
				m_SMULogicManager[i].m_Type	=	ST_ITEMSERIAL_SMU;	

			}
			break;
		case ST_USERDATA_SMU:
			AssertEx(FALSE,"Unimplement Type: ST_USERDATA_SMU");
			break;
		case ST_HUMANEXT_SMU:
			AssertEx(FALSE,"Unimplement Type: ST_HUMANEXT_SMU");
			break;
		default:
			break;
		}
		
	}

	Log::SaveLog( "./Log/ShareMemory.log", "new SMUPool<HumanSMU>()...OK" ) ;

	return TRUE ;

	__LEAVE_FUNCTION	

	return FALSE;
}

BOOL ShareMemory::InitStaticManager( )
{

	__ENTER_FUNCTION

	BOOL bRet = FALSE;

	//DB 的初始化,连接数据库
	Assert(g_pDBManager);
	bRet = g_pDBManager->Init();
	if(!bRet)
	{
		Log::SaveLog("./Log/ShareMemory.log","g_pDBManager->Init()...Fails");
	}
	else
		Log::SaveLog("./Log/ShareMemory.log","g_pDBManager->Init()...OK");


			
	for(uint i =0;i< g_Config.m_ShareMemInfo.m_SMUObjCount;i++)
	{
		SHAREMEM_TYPE type = g_Config.m_ShareMemInfo.m_pShareData[i].m_Type;
		switch(type)
		{
		case ST_HUMAN_SMU:
			{
				
				SMUPool<HumanSMU>* pHumanSMUPool = static_cast<SMUPool<HumanSMU>*>(m_SMUPool[i].m_Pool);
				Assert(pHumanSMUPool);
				SM_KEY	key			=		m_SMUPool[i].m_Data.m_Key;
				bRet = pHumanSMUPool->Init(MAX_POOL_SIZE,key,SMPT_SHAREMEM);
				Assert(bRet);
				SMULogicManager<HumanSMU>* pHumanSMULogicMgr = static_cast<SMULogicManager<HumanSMU>*>(m_SMULogicManager[i].m_SMULogicManager);
				Assert(pHumanSMULogicMgr);
				bRet =	pHumanSMULogicMgr->Init(pHumanSMUPool);
				Assert(bRet);
			}
			break;
		case ST_GUILD_SMU:
			{
				SMUPool<GuildSMU>* pGuildSMUPool = static_cast<SMUPool<GuildSMU>*>(m_SMUPool[i].m_Pool);
				Assert(pGuildSMUPool);
				SM_KEY	key			=		m_SMUPool[i].m_Data.m_Key;
				bRet = pGuildSMUPool->Init(MAX_GUILD_SIZE,key,SMPT_SHAREMEM);
				Assert(bRet);
				SMULogicManager<GuildSMU>* pGuildSMULogicMgr = static_cast<SMULogicManager<GuildSMU>*>(m_SMULogicManager[i].m_SMULogicManager);
				Assert(pGuildSMULogicMgr);
				bRet =	pGuildSMULogicMgr->Init(pGuildSMUPool);
				Assert(bRet);
			}
			break;
		case ST_MAIL_SMU:
			{
				SMUPool<MailSMU>* pMailSMUPool = static_cast<SMUPool<MailSMU>*>(m_SMUPool[i].m_Pool);
				Assert(pMailSMUPool);
				SM_KEY	key			=		m_SMUPool[i].m_Data.m_Key;
				bRet = pMailSMUPool->Init(MAX_MAILNODE_SIZE,key,SMPT_SHAREMEM);
				Assert(bRet);
				SMULogicManager<MailSMU>* pMailSMULogicMgr = static_cast<SMULogicManager<MailSMU>*>(m_SMULogicManager[i].m_SMULogicManager);
				Assert(pMailSMULogicMgr);
				bRet =	pMailSMULogicMgr->Init(pMailSMUPool);
				Assert(bRet);
			}
			break;
		case ST_PSHOP_SMU:
			{
				SMUPool<PlayerShopSM>* pPlayerShopSMUPool = static_cast<SMUPool<PlayerShopSM>*>(m_SMUPool[i].m_Pool);
				Assert(pPlayerShopSMUPool);
				SM_KEY	key			=		m_SMUPool[i].m_Data.m_Key;
				bRet = pPlayerShopSMUPool->Init(MAX_PLAYER_SHOP_POOL_PER_SERVER,key,SMPT_SHAREMEM);
				Assert(bRet);
				SMULogicManager<PlayerShopSM>* pPlayerShopSMULogicMgr = static_cast<SMULogicManager<PlayerShopSM>*>(m_SMULogicManager[i].m_SMULogicManager);
				Assert(pPlayerShopSMULogicMgr);
				bRet =	pPlayerShopSMULogicMgr->Init(pPlayerShopSMUPool);
				Assert(bRet);
			}	
			break;

		case ST_ITEMSERIAL_SMU:
			{
				SMUPool<ItemSerialKeySMU>* pItemSerialSMUPool = static_cast<SMUPool<ItemSerialKeySMU>*>(m_SMUPool[i].m_Pool);
				Assert(pItemSerialSMUPool);
				SM_KEY	key			=		m_SMUPool[i].m_Data.m_Key;
				bRet = pItemSerialSMUPool->Init(1,key,SMPT_SHAREMEM);
				Assert(bRet);
				SMULogicManager<ItemSerialKeySMU>* pItemSerialSMULogicMgr = static_cast<SMULogicManager<ItemSerialKeySMU>*>(m_SMULogicManager[i].m_SMULogicManager);
				Assert(pItemSerialSMULogicMgr);
				bRet =	pItemSerialSMULogicMgr->Init(pItemSerialSMUPool);
				Assert(bRet);

			}
			break;
		case ST_USERDATA_SMU:
			AssertEx(FALSE,"Unimplement Type: ST_USERDATA_SMU");
			break;
		case ST_HUMANEXT_SMU:
			AssertEx(FALSE,"Unimplement Type: ST_HUMANEXT_SMU");
			break;
		default:
			AssertEx(bRet," ShareMemory::InitStaticManager fails");
			break;
		}
		
		
	}


	return bRet ;

	__LEAVE_FUNCTION	

	return FALSE;
}

BOOL ShareMemory::DelStaticManager( )
{

	__ENTER_FUNCTION

	for(uint i =0;i< g_Config.m_ShareMemInfo.m_SMUObjCount;i++)
	{

		SHAREMEM_TYPE type = g_Config.m_ShareMemInfo.m_pShareData[i].m_Type;
		switch(type)
		{
		case ST_HUMAN_SMU:
			{

				SMUPool<HumanSMU>* pHumanSMUPool = 
					static_cast<SMUPool<HumanSMU>*>(m_SMUPool[i].m_Pool);
				SMULogicManager<HumanSMU>* pHumanSMULogicMgr = 
					static_cast<SMULogicManager<HumanSMU>*>(m_SMULogicManager[i].m_SMULogicManager);

				SAFE_DELETE(pHumanSMUPool);
				SAFE_DELETE(pHumanSMULogicMgr);
			
			}
			break;
		case ST_GUILD_SMU:
			{
				SMUPool<GuildSMU>* pGuildSMUPool = 
					static_cast<SMUPool<GuildSMU>*>(m_SMUPool[i].m_Pool);
				SMULogicManager<GuildSMU>* pGuildSMULogicMgr = 
					static_cast<SMULogicManager<GuildSMU>*>(m_SMULogicManager[i].m_SMULogicManager);

				SAFE_DELETE(pGuildSMUPool);
				SAFE_DELETE(pGuildSMULogicMgr);

			}
			break;
		case ST_MAIL_SMU:
			{
				SMUPool<MailSMU>* pMailSMUPool = 
					static_cast<SMUPool<MailSMU>*>(m_SMUPool[i].m_Pool);
				SMULogicManager<MailSMU>* pMailSMULogicMgr = 
					static_cast<SMULogicManager<MailSMU>*>(m_SMULogicManager[i].m_SMULogicManager);

				SAFE_DELETE(pMailSMUPool);
				SAFE_DELETE(pMailSMULogicMgr);

			}
			break;
		case ST_ITEMSERIAL_SMU:
			{
				SMUPool<ItemSerialKeySMU>* pSMUPool = 
					static_cast<SMUPool<ItemSerialKeySMU>*>(m_SMUPool[i].m_Pool);
				SMULogicManager<ItemSerialKeySMU>* pSMULogicMgr = 
					static_cast<SMULogicManager<ItemSerialKeySMU>*>(m_SMULogicManager[i].m_SMULogicManager);

				SAFE_DELETE(pSMUPool);
				SAFE_DELETE(pSMULogicMgr);

			}
			break;
		case ST_USERDATA_SMU:
			AssertEx(FALSE,"Unimplement Type: ST_USERDATA_SMU");
			break;
		case ST_HUMANEXT_SMU:
			AssertEx(FALSE,"Unimplement Type: ST_HUMANEXT_SMU");
			break;
		default:
			AssertEx(FALSE," ShareMemory::InitStaticManager fails");
			break;
		}
		
		
	}

	Log::SaveLog( "./Log/ShareMemory.log", "new SMUPool<HumanSMU>()...OK" ) ;
	return TRUE ;

	__LEAVE_FUNCTION	
	return FALSE;
}

BOOL	CheckSaveAllFile()
{
	__ENTER_FUNCTION
	
	
	if( remove( "saveall.cmd" ) == -1 )
	{
				return FALSE;
	}
			
	return TRUE;


	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	CheckExitFile()
{
	__ENTER_FUNCTION


	if( remove( "exit.cmd" ) == -1 )
	{
			return FALSE;
	}
	extern	BOOL g_LockTimeOutEnable;
	g_LockTimeOutEnable = TRUE;
	return TRUE;


	__LEAVE_FUNCTION

	return FALSE;
}


VOID INTHandler(INT)
{
	DumpStack("INT exception:\r\n");
	exit(0);
}
VOID TERMHandler(INT)
{
	DumpStack("TERM exception:\r\n");
	exit(0);

}
VOID ABORTHandler(INT)
{
	DumpStack("ABORT exception:\r\n");
	exit(0);
}

VOID ILLHandler(INT)
{
	DumpStack("ILL exception:\r\n");
	exit(0);
}

VOID FPEHandler(INT)
{
	DumpStack("FPE exception:\r\n");
	exit(0);
}

VOID SEGHandler(INT)
{
	DumpStack("SEG exception:\r\n");
	exit(0);
}
VOID XFSZHandler(INT)
{
	DumpStack("XFSZ exception:\r\n");
	exit(0);
}

ShmExceptionHandler::ShmExceptionHandler()
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

ShmExceptionHandler g_ShmExceptionHandler;
