#include "stdafx.h"
#include "SMULogicManager.h"
#include "TimeManager.h"
#include "DBCharFullData.h"
#include "Log.h"
#include "ShareDBManager.h"
#include "PlayerShopDefine.h"
#include "BBS.h"
#include "Config.h"
#include "DBItemSerial.h"
#include "DBMailInfo.h"
#include "DBGuildInfo.h"


using namespace PLAYER_SHOP;

GLOBAL_CONFIG	g_SMConfig;
extern BOOL g_LockTimeOutEnable;

//HumanSMU 的心跳

template<>
BOOL	SMULogicManager<HumanSMU>::DoSaveAll()
{
	__ENTER_FUNCTION
		if(!m_PoolSharePtr)
		{
			Assert(m_PoolSharePtr);
			return FALSE;
		}

		INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();
		//做预先统计
		INT		iHoldDataCount	=	0;		//已经占据位置的SMU个数
		INT		iCanSaveCount	=	0;		//服务器通知下线等待存盘档案数量
		INT		iHaveSaveCount	=	0;		//已经存盘等待服务器回收个数

		for(INT iIndex = 0;iIndex<MaxPoolSize;iIndex++)
		{
			HumanSMU*	pSMU = m_PoolSharePtr->GetPoolObj(iIndex);
			if(!pSMU)
			{
				Assert(pSMU);
				return FALSE;
			}
			INT		UseStatus	=	pSMU->GetUseStatus(SM_C_WRITE);

			if(UseStatus == SM_USE_HOLDDATA||UseStatus ==SM_USE_READYFREE) //本SMU 被合法占据
			{
				iHoldDataCount++;
			}

			if(UseStatus == SM_USE_READYFREE)
			{
				iCanSaveCount ++;
			}

		}

		for(INT iIndex = 0;iIndex<MaxPoolSize;iIndex++)
		{
			HumanSMU*	pSMU = m_PoolSharePtr->GetPoolObj(iIndex);
			if(!pSMU)
			{
				Assert(pSMU);
				return FALSE;
			}

			UINT    uTime		=	g_pTimeManager->RunTime();
			INT		UseStatus	=	pSMU->GetUseStatusFast();


			if(UseStatus == SM_USE_HOLDDATA||UseStatus ==SM_USE_READYFREE) //本SMU 被合法占据
			{
				FULLUSERDATA	FullData;
				GUID_t	guid = INVALID_ID;
				_MY_TRY
				{
					if(pSMU->GetLockValueFast() != SM_FREE)
					{
						
						if(g_LockTimeOutEnable)
						{
							Log::SaveLog("./Log/ShareMemory.log","ERROR: User SMU Locked Human GUID=%X",pSMU->m_HumanSM.m_GUID);
						}
						continue;
					}

					pSMU->Copy2FullData(SM_C_WRITE,&FullData);		//将ShareMemory 中对应的角色数据copy出来

					FullData.m_Human.m_LastLogoutTime = g_pTimeManager->CurrentDate();		//下线时间

					guid = FullData.m_Human.m_GUID;

					ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
					Assert(pInterface);
					DBCharFullData	CharFullDataObject(pInterface);
					CharFullDataObject.SetCharGuid(FullData.m_Human.m_GUID);

					BOOL bRet = CharFullDataObject.Save(&FullData); //保存基本档案
					if(bRet)
					{
						bRet  =	CharFullDataObject.ParseResult(&FullData);//Parse 档案内容
					}
					//修改下次保存时间
					pSMU->SetTime2Save(uTime+g_Config.m_ShareMemInfo.DATAInterval,SM_C_WRITE);
					if(!bRet)
					{
						Log::SaveLog("./Log/ShareMemory.log","CharFullDataObject.Save()...Get Errors: %s ",CharFullDataObject.GetErrorMessage());
					}
					else
					{
						if(UseStatus==SM_USE_READYFREE)
						{
							pSMU->SetUseStatus(SM_C_WRITE,SM_USE_FREEED);
							iHaveSaveCount++;
							Log::SaveLog("./Log/ShareMemory.log","离线存盘成功	PlayerPool Index=%d,Player Guid=%X",iIndex,guid);
						}
						else
						{
							Log::SaveLog("./Log/ShareMemory.log","普通存盘成功	PlayerPool Index=%d,Player Guid=%X",iIndex,guid);
						}
						Log::SaveLog("./Log/ShareMemory.log","系统信息: 档案总数 = %d,总等待存盘数量 =%d,已经存储数量 = %d",iHoldDataCount,iCanSaveCount,iHaveSaveCount);
					}
				}
				_MY_CATCH
				{
					CHAR filename[_MAX_PATH]={0} ;
					sprintf( filename, "./Users/%X_%d.%.4d-%.2d-%.2d-%.2d-%.2d-%.2d.usr", 
						guid, iIndex,
						g_pTimeManager->GetYear(), g_pTimeManager->GetMonth()+1, g_pTimeManager->GetDay(),
						g_pTimeManager->GetHour(), g_pTimeManager->GetMinute(), g_pTimeManager->GetSecond() ) ;
					FullData.OutputToDisk( filename ) ;
					Log::SaveLog("./Log/ShareMemory.log","系统信息: 存盘失败	PlayerPool Index=%d,Player Guid=%X",iIndex,guid);
				}
			}
		}
		return TRUE;
	__LEAVE_FUNCTION
		return FALSE;

}


template<>
BOOL SMULogicManager<HumanSMU>::DoNormalSave()
{
	__ENTER_FUNCTION

		if(!m_PoolSharePtr)
		{
			Assert(m_PoolSharePtr);
			return FALSE;
		}

		INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();
		//做预先统计
		INT		iHoldDataCount	=	0;		//已经占据位置的SMU个数
		INT		iCanSaveCount	=	0;		//服务器通知下线等待存盘档案数量
		INT		iHaveSaveCount	=	0;		//已经存盘等待服务器回收个数

		for(INT iIndex = 0;iIndex<MaxPoolSize;iIndex++)
		{
			HumanSMU*	pSMU = m_PoolSharePtr->GetPoolObj(iIndex);
			if(!pSMU)
			{
				Assert(pSMU);
				return FALSE;
			}
			
			UINT    uTime		=	g_pTimeManager->RunTime();				//取当前时间
			INT		UseStatus	=	pSMU->GetUseStatusFast();			//取当前内存块状态

			if(UseStatus == SM_USE_HOLDDATA||UseStatus ==SM_USE_READYFREE) //本SMU 被合法占据
			{
				iHoldDataCount++;
			}

			if(UseStatus == SM_USE_READYFREE)
			{
				iCanSaveCount ++;
			}

			if(UseStatus == SM_USE_HOLDDATA||UseStatus ==SM_USE_READYFREE) //本SMU 被合法占据 holddata 或者准备回收
			{
				//检查是否到达存盘时间
				UINT Time2Save	= pSMU->GetTime2SaveFast();
				if((uTime>Time2Save)||UseStatus ==SM_USE_READYFREE)
				{
					GUID_t	guid=INVALID_ID ;
					FULLUSERDATA	FullData;
					_MY_TRY
					{
						if(!pSMU->TryCopy2FullData(SM_C_WRITE,&FullData))		//将ShareMemory 中对应的角色数据copy出来
						{
							continue;
						}
						FullData.m_Human.m_LastLogoutTime = uTime;		//下线时间
						guid = FullData.m_Human.m_GUID;
						ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
						Assert(pInterface);
						DBCharFullData	CharFullDataObject(pInterface);
						CharFullDataObject.SetCharGuid(FullData.m_Human.m_GUID);
						BOOL bRet = CharFullDataObject.Save(&FullData); //保存基本档案
						if(bRet)
						{
							bRet  =	CharFullDataObject.ParseResult(&FullData);//Parse 档案内容
						}
						//修改下次保存时间
						pSMU->SetTime2Save(uTime+g_Config.m_ShareMemInfo.DATAInterval,SM_C_WRITE);
						if(!bRet)
						{
							Log::SaveLog("./Log/ShareMemory.log","CharFullDataObject.Save()...Get Errors: %s ",CharFullDataObject.GetErrorMessage());

							CHAR filename[_MAX_PATH]={0} ;
							sprintf( filename, "./Users/%X_%d.%.4d-%.2d-%.2d-%.2d-%.2d-%.2d.usr", 
								guid, iIndex,
								g_pTimeManager->GetYear(), g_pTimeManager->GetMonth()+1, g_pTimeManager->GetDay(),
								g_pTimeManager->GetHour(), g_pTimeManager->GetMinute(), g_pTimeManager->GetSecond() ) ;
							FullData.OutputToDisk( filename ) ;
						}
						else
						{
							if(UseStatus==SM_USE_READYFREE)
							{
								pSMU->SetUseStatus(SM_C_WRITE,SM_USE_FREEED);
								iHaveSaveCount++;
								Log::SaveLog("./Log/ShareMemory.log","离线存盘成功	PlayerPool Index=%d,Player Guid=%X",iIndex,guid);
							}
							else
							{
								Log::SaveLog("./Log/ShareMemory.log","普通存盘成功	PlayerPool Index=%d,Player Guid=%X",iIndex,guid);
							}
							Log::SaveLog("./Log/ShareMemory.log","系统信息: 档案总数 = %d,总等待存盘数量 =%d,已经存储数量 = %d",iHoldDataCount,iCanSaveCount,iHaveSaveCount);
						}
					}
					_MY_CATCH
					{
						CHAR filename[_MAX_PATH]={0} ;
						sprintf( filename, "./Users/%X_%d.%.4d-%.2d-%.2d-%.2d-%.2d-%.2d.usr", 
							guid, iIndex,
							g_pTimeManager->GetYear(), g_pTimeManager->GetMonth()+1, g_pTimeManager->GetDay(),
							g_pTimeManager->GetHour(), g_pTimeManager->GetMinute(), g_pTimeManager->GetSecond() ) ;
						FullData.OutputToDisk( filename ) ;
						Log::SaveLog("./Log/ShareMemory.log","系统信息: 存盘失败	PlayerPool Index=%d,Player Guid=%X",iIndex,guid);
					}
				}
			}
		}
		
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

template<>
BOOL SMULogicManager<HumanSMU>::DoClear()
{
	__ENTER_FUNCTION
	
	INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();
	for(INT iIndex = 0;iIndex<MaxPoolSize;iIndex++)
	{
		HumanSMU*	pSMU = m_PoolSharePtr->GetPoolObj(iIndex);
		if(!pSMU)
		{
			Assert(pSMU);
			return FALSE;
		}
		INT		UseStatus	=	pSMU->GetUseStatus(SM_C_WRITE);
		pSMU->SetUseStatus(SM_USE_FREE,SM_C_WRITE);
	}
	return TRUE;
	

	__LEAVE_FUNCTION

	return FALSE;
}

template<>
BOOL SMULogicManager<HumanSMU>::DoPostInit()
{
	__ENTER_FUNCTION

		if(!m_PoolSharePtr)
		{
			Assert(m_PoolSharePtr);
			return FALSE;
		}

		if(g_CmdArgv == CMD_MODE_CLEARALL)
		{
			return TRUE;
		}
		INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();

		for(INT iIndex = 0;iIndex<MaxPoolSize;iIndex++)
		{
			HumanSMU*	pSMU = m_PoolSharePtr->GetPoolObj(iIndex);
			if(!pSMU)
			{
				Assert(pSMU);
				return FALSE;
			}
			INT		UseStatus	=	pSMU->GetUseStatus(SM_C_WRITE);
			pSMU->SetUseStatus(SM_USE_FREE,SM_C_WRITE);
		}

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


template<>
BOOL	SMULogicManager<GuildSMU>::DoSaveAll()
{
	__ENTER_FUNCTION

	///存盘统计数据 
	INT		SMUCount			=		0;
	INT		TotalSMUSize		=		0;
	UINT    uTime		=	g_pTimeManager->RunTime();

	if(!m_PoolSharePtr)
	{
		Assert(m_PoolSharePtr);
		return FALSE;
	}

	SM_KEY key	= m_PoolSharePtr->GetKey();

	ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
	Assert(pInterface);

	DBGuildInfo	GuildInfoObject(pInterface);
	BOOL bRet	= GuildInfoObject.Save(m_PoolSharePtr);
	if(bRet)
	{
		bRet  =	GuildInfoObject.ParseResult(m_PoolSharePtr);//Parse 档案内容
	}
	Log::SaveLog("./Log/ShareMemory.log","End GuildSMU_%d SaveAll...OK!",key);
	Log::SaveLog("./Log/ShareMemory.log","SMUCount = %d",SMUCount);
	Log::SaveLog("./Log/ShareMemory.log","TotalSMUSize = %d",TotalSMUSize);

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

template<>
BOOL SMULogicManager<GuildSMU>::DoNormalSave()
{
	__ENTER_FUNCTION
		
	/*
	*	临时存文件方法
	*/
	UINT    uTime		=	g_pTimeManager->RunTime();
	if(uTime>m_FinalSaveTime+g_Config.m_ShareMemInfo.SMUInterval)
	{
		if(m_bReady)
		{
			DoSaveAll();
		}
		m_FinalSaveTime = uTime;
	}
		return TRUE;

	__LEAVE_FUNCTION
		
	return FALSE;

}

template<>
BOOL SMULogicManager<GuildSMU>::DoClear()
{
	__ENTER_FUNCTION

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

template<>
BOOL SMULogicManager<GuildSMU>::DoPostInit()
{
	__ENTER_FUNCTION

		if(!m_PoolSharePtr)
		{	
			Assert(m_PoolSharePtr);
			return FALSE;
		}

		if(g_CmdArgv == CMD_MODE_CLEARALL)
		{
			return TRUE;
		}

		m_FinalSaveTime	=	g_pTimeManager->RunTime();

		INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();

		for(INT iIndex = 0;iIndex<MaxPoolSize;iIndex++)
		{

			GuildSMU* pGuildSMU = m_PoolSharePtr->GetPoolObj(iIndex);
			if(!pGuildSMU)
			{
				Assert(pGuildSMU);
				return FALSE;
			}

			pGuildSMU->Init();
		}

		SM_KEY key  = m_PoolSharePtr->GetKey();

		BOOL bRet = FALSE;

		ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
		Assert(pInterface);

		DBGuildInfo	GuildInfoObject(pInterface);
		bRet = GuildInfoObject.Load();

		if(bRet)
		{
			bRet  =	GuildInfoObject.ParseResult(m_PoolSharePtr);//Parse 档案内容
		}
		if(!bRet)
		{
			Log::SaveLog("./Log/ShareMemory.log","PostInit GuildSMU_%d from database fails",key);
		}
		if(bRet)
		{
			m_bReady = TRUE;
			Log::SaveLog("./Log/ShareMemory.log","PostInit GuildSMU_%d from database  Ok!",key);
		}

		return TRUE;



	__LEAVE_FUNCTION

		return FALSE;
}


template<>
BOOL	SMULogicManager<MailSMU>::DoSaveAll()
{
	__ENTER_FUNCTION
		
	///存盘统计数据 
	INT		SMUCount			=		0;
	INT		TotalSMUSize		=		0;
	UINT    uTime		=	g_pTimeManager->RunTime();
	
	if(!m_PoolSharePtr)
	{
		Assert(m_PoolSharePtr);
		return FALSE;
	}

	SM_KEY key	= m_PoolSharePtr->GetKey();
	
	ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
	Assert(pInterface);

	DBMailInfo	MailInfoObject(pInterface);
	BOOL bRet	= MailInfoObject.Save(m_PoolSharePtr);
	if(bRet)
	{
		bRet  =	MailInfoObject.ParseResult(m_PoolSharePtr);//Parse 档案内容
	}

	Log::SaveLog("./Log/ShareMemory.log","End MailSMU_%d SaveAll...OK!",key);
	Log::SaveLog("./Log/ShareMemory.log","SMUCount = %d",SMUCount);
	Log::SaveLog("./Log/ShareMemory.log","TotalSMUSize = %d",TotalSMUSize);
	
	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;

}

template<>
BOOL SMULogicManager<MailSMU>::DoNormalSave()
{
	__ENTER_FUNCTION

		/*
		 *	临时存文件方法
		 */
		UINT    uTime	=	g_pTimeManager->RunTime();
		if(uTime>m_FinalSaveTime+g_Config.m_ShareMemInfo.SMUInterval)
		{
			if(m_bReady)
			{
				DoSaveAll();
			}
			m_FinalSaveTime = uTime;
		}
		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;

}

template<>
BOOL SMULogicManager<MailSMU>::DoClear()
{
	__ENTER_FUNCTION

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

template<>
BOOL SMULogicManager<MailSMU>::DoPostInit()
{
	__ENTER_FUNCTION

		if(!m_PoolSharePtr)
		{	Assert(m_PoolSharePtr);
			return FALSE;
		}

		if(g_CmdArgv == CMD_MODE_CLEARALL)
		{
			return TRUE;
		}

		m_FinalSaveTime	=	g_pTimeManager->RunTime();

		INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();
		for(INT iIndex = 0;iIndex<MaxPoolSize;iIndex++)
		{

			MailSMU* pMailSMU = m_PoolSharePtr->GetPoolObj(iIndex);
			if(!pMailSMU)
			{
				Assert(pMailSMU);
				return FALSE;
			}

			pMailSMU->Init();
		}
		
		SM_KEY key  = m_PoolSharePtr->GetKey();
	

		BOOL bRet = FALSE;

		ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
		Assert(pInterface);

		DBMailInfo	MailInfoObject(pInterface);
		bRet = MailInfoObject.Load();

		if(bRet)
		{
			bRet  =	MailInfoObject.ParseResult(m_PoolSharePtr);//Parse 档案内容
		}
		if(!bRet)
		{
			Log::SaveLog("./Log/ShareMemory.log","PostInit MailSMU_%d from database fails",key);
		}
		if(bRet)
		{
			m_bReady = TRUE;
			Log::SaveLog("./Log/ShareMemory.log","PostInit MailSMU_%d from database  Ok!",key);
		}

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

template<>
BOOL	SMULogicManager<PlayerShopSM>::DoSaveAll()
{
	__ENTER_FUNCTION

	///存盘统计数据 
	//INT		SMUCount			=		0;
	//INT		TotalSMUSize		=		0;
	//UINT    uTime		=	g_pTimeManager->RunTime();

	//if(!m_PoolSharePtr)
	//{
	//	Assert(m_PoolSharePtr);
	//	return FALSE;
	//}

	//SM_KEY	key					= m_PoolSharePtr->GetKey();
	//ID_t	ServerID			= g_Config.Key2ServerID(key);
	//if(ServerID == INVALID_ID)
	//{
	//	AssertEx(FALSE,"对应Key的服务器没有EnableShareMemory");
	//}

	//ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
	//Assert(pInterface);
	//
	//DBShopInfo	ShopInfoObject(pInterface);
	//ShopInfoObject.SetServerID(ServerID);

	//BOOL bRet = ShopInfoObject.Save(m_PoolSharePtr);
	//if(bRet)
	//{
	//	bRet  =	ShopInfoObject.ParseResult(m_PoolSharePtr);//Parse 档案内容
	//}

	//if(bRet)
	//{
	//	Log::SaveLog("./Log/ShareMemory.log","End PlayerShopSM_%d SaveAll...OK!",key);
	//	Log::SaveLog("./Log/ShareMemory.log","SMUCount = %d",SMUCount);
	//	Log::SaveLog("./Log/ShareMemory.log","TotalSMUSize = %d",TotalSMUSize);
	//}
	//else
	//{
	//	Log::SaveLog("./Log/ShareMemory.log","End PlayerShopSM_%d SaveAll...Get Errors!",key);
	//}
	

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;

}

template<>
BOOL SMULogicManager<PlayerShopSM>::DoNormalSave()
{
	__ENTER_FUNCTION
		
	/*
	*	临时存文件方法
	*/
	UINT    uTime		=	g_pTimeManager->RunTime();
	if(uTime>m_FinalSaveTime+g_Config.m_ShareMemInfo.SMUInterval)
	{
		if(m_bReady)
		{
			DoSaveAll();
		}
		m_FinalSaveTime = uTime;
	}
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;

}

template<>
BOOL SMULogicManager<PlayerShopSM>::DoClear()
{
	__ENTER_FUNCTION

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

template<>
BOOL SMULogicManager<PlayerShopSM>::DoPostInit()
{
	__ENTER_FUNCTION
	//////////////////////////////////////////////////////////////////////////
	//if(!m_PoolSharePtr)
	//{	
	//	Assert(m_PoolSharePtr);
	//	return FALSE;
	//}

	//if(g_CmdArgv == CMD_MODE_CLEARALL)
	//{
	//	return TRUE;
	//}

	//m_FinalSaveTime	=	g_pTimeManager->RunTime();

	//INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();

	//for(INT iIndex = 0;iIndex<MaxPoolSize;iIndex++)
	//{

	//	PlayerShopSM* pSMU = m_PoolSharePtr->GetPoolObj(iIndex);
	//	if(!pSMU)
	//	{
	//		Assert(pSMU);
	//		return FALSE;
	//	}

	//	pSMU->Init();
	//}

	//SM_KEY key  = m_PoolSharePtr->GetKey();
	//ID_t	ServerID			= g_Config.Key2ServerID(key);
	//
	//if(ServerID == INVALID_ID)
	//{
	//	AssertEx(FALSE,"对应Key的服务器没有EnableShareMemory");
	//}

	//BOOL bRet = FALSE;

	//ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
	//Assert(pInterface);

	//DBShopInfo	ShopInfoObject(pInterface);
	//ShopInfoObject.SetServerID(ServerID);

	//bRet = ShopInfoObject.Load();
	//if(bRet)
	//{
	//	bRet  =	ShopInfoObject.ParseResult(m_PoolSharePtr);//Parse 档案内容
	//}
	//if(!bRet)
	//{
	//	Log::SaveLog("./Log/ShareMemory.log","PostInit PlayerShopSM_%d from database fails",key);
	//}
	//if(bRet)
	//{
	//	m_bReady = TRUE;
	//	Log::SaveLog("./Log/ShareMemory.log","PostInit PlayerShopSM_%d from database  Ok!",key);
	//}

	return TRUE;




	__LEAVE_FUNCTION

	return FALSE;
}
//////////////////////////////////////////////////////////////////////////


template<>
BOOL	SMULogicManager<ItemSerialKeySMU>::DoSaveAll()
{
	__ENTER_FUNCTION

	///存盘统计数据 
	
	UINT	Serial = 0;
	UINT    uTime		=	g_pTimeManager->RunTime();
	if(!m_PoolSharePtr)
	{
		Assert(m_PoolSharePtr);
		return FALSE;
	}
	INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();
	Assert(MaxPoolSize == 1);
	ItemSerialKeySMU*	pSMU = m_PoolSharePtr->GetPoolObj(0);
	if(!pSMU)
	{
		Assert(pSMU);
		return FALSE;
	}
	Serial = pSMU->GetSerial(SM_C_READ);

	SM_KEY	key					= m_PoolSharePtr->GetKey();
	ID_t	ServerID			= g_Config.Key2ServerID(key);

	if(ServerID == INVALID_ID)
	{
		AssertEx(FALSE,"对应Key的服务器没有EnableShareMemory");
	}
	ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
	Assert(pInterface);
	DBItemSerial	ItemSerialObject(pInterface);
	ItemSerialObject.SetSID(ServerID);

	INT ErrorCode ;
	if(ItemSerialObject.Save(&Serial))
	{
		ItemSerialObject.ParseResult(&ErrorCode);
	}
	else
	{
		Log::SaveLog("./Log/ShareMemory.log","ItemSerial Save...Error! Serial = %d ,ServerID = %d",Serial,ServerID);
		Assert(FALSE);
	}

	
	Log::SaveLog("./Log/ShareMemory.log","ItemSerial Save...OK! Serial = %d ,ServerID = %d",Serial,ServerID);

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;

}

#define SERIALKEYTIME 30000

template<>
BOOL SMULogicManager<ItemSerialKeySMU>::DoNormalSave()
{
	__ENTER_FUNCTION

	/*
	*	临时存文件方法
	*/
	UINT    uTime		=	g_pTimeManager->RunTime();
	if(uTime>m_FinalSaveTime+SERIALKEYTIME)
	{
		if(m_bReady)
		{
			DoSaveAll();
		}
		m_FinalSaveTime = uTime;
	}
	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;

}

template<>
BOOL SMULogicManager<ItemSerialKeySMU>::DoClear()
{
	__ENTER_FUNCTION

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

template<>
BOOL SMULogicManager<ItemSerialKeySMU>::DoPostInit()
{
	__ENTER_FUNCTION

#define SERIAL_GROWUP 10000

		if(!m_PoolSharePtr)
		{	Assert(m_PoolSharePtr);
			return FALSE;
		}
		
		if(g_CmdArgv == CMD_MODE_CLEARALL)
		{
			return TRUE;
		}

		m_FinalSaveTime	=	g_pTimeManager->RunTime();
		INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();
		SM_KEY key  = m_PoolSharePtr->GetKey();
		
		ItemSerialKeySMU* pSMU = m_PoolSharePtr->GetPoolObj(0);
		if(!pSMU)
		{
			Assert(pSMU);
			return FALSE;
		}
		
		ID_t	ServerID			= g_Config.Key2ServerID(key);

		if(ServerID == INVALID_ID)
		{
			AssertEx(FALSE,"对应Key的服务器没有EnableShareMemory");
		}
		ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
		Assert(pInterface);
		UINT Serial	= 0;
		DBItemSerial	ItemSerialObject(pInterface);
		ItemSerialObject.SetSID(ServerID);

		
		if(ItemSerialObject.Load())
		{
			ItemSerialObject.ParseResult(&Serial);
		}
		if(Serial>0)
		{
			pSMU->SetSerial(SM_C_WRITE,Serial+SERIAL_GROWUP);
			Serial = pSMU->GetSerial(SM_C_READ);
		}
		else
		{
			Assert(FALSE);
		}
		m_bReady = TRUE;
		
		Log::SaveLog("./Log/ShareMemory.log","PostInit ItemSerialSMU=%d from database Ok!  Serial = %d",key,Serial);

		return TRUE;

		__LEAVE_FUNCTION

		return FALSE;
}

//////////////////////////////////////////////////////////////////////////


//template<>
//BOOL	SMULogicManager<PetProcreateItemSM>::DoSaveAll()
//{
//	__ENTER_FUNCTION
//
//	///存盘统计数据 
//	INT		SMUCount			=		0;
//	INT		TotalSMUSize		=		0;
//	UINT    uTime		=	g_pTimeManager->RunTime();
//
//	if(!m_PoolSharePtr)
//	{
//		Assert(m_PoolSharePtr);
//		return FALSE;
//	}
//
//	SM_KEY	key					= m_PoolSharePtr->GetKey();
//	ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
//	Assert(pInterface);
//
//	DBPetItemInfo	PetItemInfoObject(pInterface);
//	BOOL bRet	= PetItemInfoObject.Save(m_PoolSharePtr);
//	if(bRet)
//	{
//		bRet  =	PetItemInfoObject.ParseResult(m_PoolSharePtr);//Parse 档案内容
//	}
//	
//	if(bRet)
//	{
//		DiskLog(SHMEM_LOG_PATH,"End PetProcreateItemSM_%d SaveAll...OK!",key);
//		DiskLog(SHMEM_LOG_PATH,"SMUCount = %d",SMUCount);
//		DiskLog(SHMEM_LOG_PATH,"TotalSMUSize = %d",TotalSMUSize);
//	}
//	else
//	{
//		DiskLog(SHMEM_LOG_PATH,"End PetProcreateItemSM_%d SaveAll...Get Errors!",key);
//	}
//	
//
//	return TRUE;
//
//	__LEAVE_FUNCTION
//
//		return FALSE;
//
//}
//
//template<>
//BOOL SMULogicManager<PetProcreateItemSM>::DoNormalSave()
//{
//	__ENTER_FUNCTION
//
//	/*
//	*	临时存文件方法
//	*/
//	UINT    uTime		=	g_pTimeManager->RunTime();
//	if(uTime>m_FinalSaveTime+g_Config.m_ShareMemInfo.SMUInterval)
//	{
//		if(m_bReady)
//		{
//			DoSaveAll();
//		}
//		m_FinalSaveTime = uTime;
//	}
//	return TRUE;
//
//	__LEAVE_FUNCTION
//
//		return FALSE;
//
//}
//
//template<>
//BOOL SMULogicManager<PetProcreateItemSM>::DoClear()
//{
//	__ENTER_FUNCTION
//
//		return TRUE;
//
//	__LEAVE_FUNCTION
//
//		return FALSE;
//}
//
//template<>
//BOOL SMULogicManager<PetProcreateItemSM>::DoPostInit()
//{
//	__ENTER_FUNCTION
//		//////////////////////////////////////////////////////////////////////////
//		if(!m_PoolSharePtr)
//		{	
//			Assert(m_PoolSharePtr);
//			return FALSE;
//		}
//
//		if(g_CmdArgv == CMD_MODE_CLEARALL)
//		{
//			return TRUE;
//		}
//
//		m_FinalSaveTime	=	g_pTimeManager->RunTime();
//
//		INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();
//
//		for(INT iIndex = 0;iIndex<MaxPoolSize;iIndex++)
//		{
//
//			PetProcreateItemSM* pSMU = m_PoolSharePtr->GetPoolObj(iIndex);
//			if(!pSMU)
//			{
//				Assert(pSMU);
//				return FALSE;
//			}
//			pSMU->Init();
//		}
//
//		SM_KEY	key  = m_PoolSharePtr->GetKey();
//		BOOL	bRet = FALSE;
//		ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
//		Assert(pInterface);
//		
//		DBPetItemInfo	PetItemObject(pInterface);
//		
//		bRet = PetItemObject.Load();
//		if(bRet)
//		{
//			bRet  =	PetItemObject.ParseResult(m_PoolSharePtr);//Parse 档案内容
//		}
//		if(!bRet)
//		{
//			DiskLog(SHMEM_LOG_PATH,"PostInit PetProcreateItemSM_%d from database fails",key);
//		}
//		if(bRet)
//		{
//			m_bReady = TRUE;
//			DiskLog(SHMEM_LOG_PATH,"PostInit PetProcreateItemSM%d from database  Ok!",key);
//		}
//		return TRUE;
//
//		__LEAVE_FUNCTION
//
//		return FALSE;
//}

//////////////////////////////////////////////////////////////////////////
//template<>
//BOOL	SMULogicManager<CitySMU>::DoSaveAll()
//{
//	__ENTER_FUNCTION
//
//		///存盘统计数据 
//	INT		SMUCount			=		0;
//	INT		TotalSMUSize		=		0;
//	UINT    uTime		=	g_pTimeManager->RunTime();
//
//	if(!m_PoolSharePtr)
//	{
//		Assert(m_PoolSharePtr);
//		return FALSE;
//	}
//
//	SM_KEY	key					= m_PoolSharePtr->GetKey();
//	ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
//	Assert(pInterface);
//	
//	DBCityInfo	CityInfoObject(pInterface);
//	BOOL bRet	= CityInfoObject.Save(m_PoolSharePtr);
//	if(bRet)
//	{
//		bRet  =	CityInfoObject.ParseResult(m_PoolSharePtr);//Parse 档案内容
//	}
//
//	if(bRet)
//	{
//		DiskLog(SHMEM_LOG_PATH,"End CityInfoSM_%d SaveAll...OK!",key);
//		DiskLog(SHMEM_LOG_PATH,"SMUCount = %d",SMUCount);
//		DiskLog(SHMEM_LOG_PATH,"TotalSMUSize = %d",TotalSMUSize);
//	}
//	else
//	{
//		DiskLog(SHMEM_LOG_PATH,"End CityInfoSM_%d SaveAll...Get Errors!",key);
//	}
//	
//
//	return TRUE;
//
//	__LEAVE_FUNCTION
//
//		return FALSE;
//
//}
//
//template<>
//BOOL SMULogicManager<CitySMU>::DoNormalSave()
//{
//	__ENTER_FUNCTION
//
//	/*
//	*	临时存文件方法
//	*/
//	UINT    uTime		=	g_pTimeManager->RunTime();
//	if(uTime>m_FinalSaveTime+g_Config.m_ShareMemInfo.SMUInterval)
//	{
//		if(m_bReady)
//		{
//			DoSaveAll();
//		}
//		m_FinalSaveTime = uTime;
//	}
//	
//	return TRUE;
//
//	__LEAVE_FUNCTION
//
//	return FALSE;
//
//}
//
//template<>
//BOOL SMULogicManager<CitySMU>::DoClear()
//{
//	__ENTER_FUNCTION
//
//		return TRUE;
//
//	__LEAVE_FUNCTION
//
//		return FALSE;
//}
//
//template<>
//BOOL SMULogicManager<CitySMU>::DoPostInit()
//{
//	__ENTER_FUNCTION
//	//////////////////////////////////////////////////////////////////////////
//	if(!m_PoolSharePtr)
//	{	
//			Assert(m_PoolSharePtr);
//			return FALSE;
//	}
//
//	if(g_CmdArgv == CMD_MODE_CLEARALL)
//	{
//			return TRUE;
//	}
//
//	m_FinalSaveTime	=	g_pTimeManager->RunTime();
//
//	INT MaxPoolSize = m_PoolSharePtr->GetPoolMaxSize();
//
//	for(INT iIndex = 0;iIndex<MaxPoolSize;iIndex++)
//	{
//
//		CitySMU* pSMU = m_PoolSharePtr->GetPoolObj(iIndex);
//		if(!pSMU)
//		{
//			Assert(pSMU);
//			return FALSE;
//		}
//		pSMU->Init();
//	}
//
//	SM_KEY	key  = m_PoolSharePtr->GetKey();
//	BOOL	bRet = FALSE;
//	ODBCInterface* pInterface= 	g_pDBManager->GetInterface(CHARACTER_DATABASE);
//	Assert(pInterface);
//
//	
//	DBCityInfo	CityInfoObject(pInterface);
//
//	bRet = CityInfoObject.Load();
//	if(bRet)
//	{
//			bRet  =	CityInfoObject.ParseResult(m_PoolSharePtr);//Parse 档案内容
//	}
//	if(!bRet)
//	{
//			DiskLog(SHMEM_LOG_PATH,"PostInit CityInfoSM_%d from database fails",key);
//	}
//	if(bRet)
//	{
//			m_bReady = TRUE;
//			DiskLog(SHMEM_LOG_PATH,"PostInit CityInfoSM%d from database  Ok!",key);
//	}
//	
//	return TRUE;
//
//	__LEAVE_FUNCTION
//
//	return FALSE;
//}
//



