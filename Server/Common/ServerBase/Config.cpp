#include "stdafx.h"

#include "Config.h"
#include "Ini.h"
#include "Log.h"
#include "FileDef.h"

Config g_Config ;

Config::Config( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

Config::~Config( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

BOOL Config::Init( )
{
__ENTER_FUNCTION

	LoadConfigInfo( ) ;
	LoadLoginInfo();
	LoadWorldInfo( ) ;
	LoadShareMemInfo();
	LoadBillingInfo();
	LoadMachineInfo( ) ;
	LoadServerInfo( ) ;
	LoadSceneInfo( ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID Config::ReLoad( )
{
	LoadConfigInfo_ReLoad( ) ;
	LoadLoginInfo_Reload();
	LoadWorldInfo_ReLoad( ) ;
	LoadShareMemInfo_ReLoad();
	LoadBillingInfo_ReLoad();
	LoadMachineInfo_ReLoad( ) ;
	LoadServerInfo_ReLoad( ) ;
	LoadSceneInfo_ReLoad( ) ;
}

VOID Config::LoadConfigInfo( )
{
__ENTER_FUNCTION

	LoadConfigInfo_Only( ) ;
	LoadConfigInfo_ReLoad( ) ;

__LEAVE_FUNCTION
}
VOID Config::LoadConfigInfo_Only( )
{//不能被重复读取的数据
__ENTER_FUNCTION

	Ini ini( FILE_CONFIG_INFO ) ;

	m_ConfigInfo.m_SystemModel = ini.ReadInt( "SystemModel", "SystemModel" ) ;
	m_ConfigInfo.m_ServerID = (ID_t)(ini.ReadInt( "System", "ServerId" )) ;
	m_ConfigInfo.m_ZoneSize = ini.ReadInt( "Zone", "ZoneSize" ) ;
	m_ConfigInfo.m_MaxPortal = ini.ReadInt( "Portal", "MaxCount" ) ;
	m_ConfigInfo.m_MaxSkillObj = ini.ReadInt( "SkillObj", "MaxCount" ) ;
	m_ConfigInfo.m_MaxPlatform = ini.ReadInt( "Platform", "MaxCount" );
	m_ConfigInfo.m_MaxSpecial = ini.ReadInt( "SpecialObj", "MaxCount" );
	m_ConfigInfo.m_MaxPlayerShopNum = ini.ReadInt( "PlayerShop", "MaxCount" );
	m_ConfigInfo.m_MaxTimerCount = ini.ReadInt("SceneTimer","MaxCount");
	m_ConfigInfo.m_nMaxHumanUseTimer = ini.ReadInt("HumanTimer","MaxCount");

	Log::SaveLog( CONFIG_LOGFILE, "Load ConfigInfo.ini ...Only OK! " ) ;

__LEAVE_FUNCTION
}
VOID Config::LoadConfigInfo_ReLoad( )
{//可以重复读取的数据
__ENTER_FUNCTION

	Ini ini( FILE_CONFIG_INFO ) ;
	CHAR	szText[32];

	ini.ReadText( "Global", "DropParam", szText, sizeof( szText ) -1 ) ;
	m_ConfigInfo.m_DropParam = (FLOAT)(atof( szText ));

	ini.ReadText( "Global", "ExpParam", szText, sizeof( szText ) -1 ) ;
	m_ConfigInfo.m_ExpParam = (FLOAT)(atof( szText ));
	m_ConfigInfo.m_EquipmentDamagePoint = ini.ReadInt( "Global", "EquipmentDamagePoint" ) ;

	m_ConfigInfo.m_RecoverTime = ini.ReadInt( "TimeSetting", "RecoverTime" ) ;
	m_ConfigInfo.m_MaxMonster = ini.ReadInt( "Monster", "MaxCount" ) ;
	m_ConfigInfo.m_MaxPet = ini.ReadInt( "Pet", "MaxCount" ) ;
	m_ConfigInfo.m_PetBodyTime = ini.ReadInt( "Pet", "PetBodyTime" ) ;
	m_ConfigInfo.m_PetHappinessInterval = ini.ReadInt( "Pet", "PetHappinessInterval" );
	m_ConfigInfo.m_PetLifeInterval = ini.ReadInt( "Pet", "PetLifeInterval" );
	m_ConfigInfo.m_PetCallUpHappiness = ini.ReadInt( "Pet", "PetCallUpHappiness" );
	m_ConfigInfo.m_PetPlacardTime = (uint)(ini.ReadInt( "Pet", "PetPlacardTime" ));
	m_ConfigInfo.m_PetPlacardNeedLevel = ini.ReadInt( "Pet", "PetPlacardNeedLevel" );
	m_ConfigInfo.m_PetPlacardNeedHappiness = ini.ReadInt( "Pet", "PetPlacardNeedHappiness" );
	m_ConfigInfo.m_PetPlacardNeedLife = ini.ReadInt( "Pet", "PetPlacardNeedLife" );
	m_ConfigInfo.m_DefaultRespawnTime = ini.ReadInt( "Monster", "DefaultRespawnTime" ) ;
	m_ConfigInfo.m_DropBoxRecycleTime = ini.ReadInt("TimeSetting","DropBoxRecycle");
	m_ConfigInfo.m_TimeChangeInterval	= ini.ReadInt("TimeSetting","TimeChangeInterval");
	m_ConfigInfo.m_PositionRange = ini.ReadInt( "Monster", "DefaultPositionRange" ) ;
	m_ConfigInfo.m_AIType = ini.ReadInt( "Monster", "DefaultAIType" ) ;
	m_ConfigInfo.m_DisconnectTime = ini.ReadInt( "TimeSetting", "DisconnectTime" ) ;
	ini.ReadText( "Temp", "UserPath", m_ConfigInfo.m_UserPath, _MAX_PATH ) ;
	m_ConfigInfo.m_DefaultBodyTime = ini.ReadInt( "Monster", "DefaultBodyTime" ) ;
	m_ConfigInfo.m_DefaultRefuseScanTime = ini.ReadInt( "Monster", "DefaultRefuseScanTime");
	m_ConfigInfo.m_OutGhostTime = ini.ReadInt( "Human", "OutGhostTime" ) ;
	m_ConfigInfo.m_CanGetExpRange = (FLOAT)(ini.ReadInt( "Human", "CanGetExpRange" ));
	m_ConfigInfo.m_DefaultMoveSpeed = ini.ReadInt( "Human", "DefaultMoveSpeed" ) ;
	m_ConfigInfo.m_DefaultAttackSpeed = ini.ReadInt( "Human", "DefaultAttackSpeed" ) ;
	m_ConfigInfo.m_HumanVERecoverInterval = ini.ReadInt( "Human", "HumanVERecoverInterval" ) ;
	m_ConfigInfo.m_WallowAge = ini.ReadInt( "Human", "WallowAge" ) ;
	m_ConfigInfo.m_WallowStartTime = ini.ReadInt( "Human", "WallowStartTime" ) ;
	m_ConfigInfo.m_WallowStartTime2 = ini.ReadInt( "Human", "WallowStartTime2" ) ;
	m_ConfigInfo.m_WallowAwokeTime = ini.ReadInt( "Human", "WallowAwokeTime" ) ;
	m_ConfigInfo.m_nAvailableFollowDist = ini.ReadInt( "Team", "AvailableFollowDist" );
	m_ConfigInfo.m_nTimeForLoseFollow = ini.ReadInt( "Team", "TimeForLoseFollow" );
	m_ConfigInfo.m_nFoundDurationHour = ini.ReadInt( "Guild", "FoundDuration" );
	m_ConfigInfo.m_nDefaultMaxMemberCount = ini.ReadInt( "Guild", "DefaultMaxMemberCount" );
	m_ConfigInfo.m_nResponseUserCount = ini.ReadInt( "Guild", "ResponseUserCount" );
	m_ConfigInfo.m_nPasswdPoint = ini.ReadInt( "Relation", "PasswdPoint" );
	m_ConfigInfo.m_nPromptPoint = ini.ReadInt( "Relation", "PromptPoint" );
	m_ConfigInfo.m_nDeleteDelayTime = ini.ReadInt( "MinorPassword", "DeleteDelayTime" );
	m_ConfigInfo.m_nDeleteDelayTime *= 3600;
	m_ConfigInfo.m_nExpPoint = ini.ReadInt( "Relation", "ExpPoint" );
	//m_ConfigInfo.m_DefaultRefreshRate	=	ini.ReadInt("Obj_Human","DefaultRefreshRate")	;
	m_ConfigInfo.m_nHashOnlineUserCount = ini.ReadInt( "World", "HashOnlineUserCount" );
	m_ConfigInfo.m_nHashMailUserCount = ini.ReadInt( "World", "HashMailUserCount" );
	m_ConfigInfo.m_nMaxOfflineUserCount = ini.ReadInt( "World", "MaxOfflineUserCount" );
	ini.ReadText( "Global", "RespawnParam", szText, sizeof(szText)-1 ) ;
	m_ConfigInfo.m_fRespawnParam = (FLOAT)(atof(szText)) ;
	m_ConfigInfo.m_KickUserTime = ini.ReadInt( "TimeSetting", "KickUserTime" );
	m_ConfigInfo.m_nDefaultDamageFluctuation = ini.ReadInt("Combat","DefaultDamageFluctuation");

	m_ConfigInfo.m_nMinGoodBadValue = ini.ReadInt( "GoodBad", "MinGoodBadValue" );
	m_ConfigInfo.m_nMaxGoodBadValue = ini.ReadInt( "GoodBad", "MaxGoodBadValue" );
	m_ConfigInfo.m_nLevelNeeded = ini.ReadInt( "GoodBad", "LevelNeeded" );
	m_ConfigInfo.m_nMemberLevel = ini.ReadInt( "GoodBad", "MemberLevel" );
	m_ConfigInfo.m_fGoodBadRadius = (FLOAT)ini.ReadInt( "GoodBad", "GoodBadRadius" );
	m_ConfigInfo.m_nBonusPerMember = ini.ReadInt( "GoodBad", "BonusPerMember" );
	m_ConfigInfo.m_nMaxBonus = ini.ReadInt( "GoodBad", "MaxBonus" );
	m_ConfigInfo.m_nPenaltyWhenMemberDie = ini.ReadInt( "GoodBad", "PenaltyWhenMemberDie" );
	m_ConfigInfo.m_nWorldChatItemIndex = ini.ReadInt("WorldChat","ItemIndex");

	m_ConfigInfo.m_ThisRegion.m_nRegionIndex = ini.ReadInt("ThisRegion","RegionIndex");
	ini.ReadText( "ThisRegion", "RegionName", m_ConfigInfo.m_ThisRegion.m_RegionName, _MAX_PATH ) ;

	m_ConfigInfo.m_nOldRegionCount = ini.ReadInt("OldRegion","RegionCount");
	if( m_ConfigInfo.m_nOldRegionCount > 0 )
	{
		m_ConfigInfo.m_OldRegion = new _REGION_INFO[m_ConfigInfo.m_nOldRegionCount];
		for( INT i = 0; i < m_ConfigInfo.m_nOldRegionCount; i ++ )
		{
			CHAR szRegionIndex[100];
			CHAR szRegionName[100];
			tsnprintf( szRegionIndex, 99, "RegionIndex%03d", i+1 );
			tsnprintf( szRegionName, 99, "RegionName%03d", i+1 );
			m_ConfigInfo.m_OldRegion[i].m_nRegionIndex = ini.ReadInt( "OldRegion", szRegionIndex ) ;
			ini.ReadText( "OldRegion", szRegionName, m_ConfigInfo.m_OldRegion[i].m_RegionName, _MAX_PATH ) ;
		}
	}

	Log::SaveLog( CONFIG_LOGFILE, "Load ConfigInfo.ini ...ReLoad OK! " ) ;

__LEAVE_FUNCTION
}


VOID Config::LoadLoginInfo( )
{
	__ENTER_FUNCTION

	LoadLoginInfo_Only( ) ;
	LoadLoginInfo_Reload( ) ;

	__LEAVE_FUNCTION
}
VOID Config::LoadLoginInfo_Only( )
{//不能被重复读取的数据
	__ENTER_FUNCTION

	Ini ini( FILE_LOGIN_INFO ) ;

	m_LoginInfo.m_LoginID = (ID_t)(ini.ReadInt( "System", "LoginId" )) ;

	ini.ReadText( "System", "DBIP", m_LoginInfo.m_DBIP, IP_SIZE ) ;
	m_LoginInfo.m_DBPort	=	(UINT)(ini.ReadInt("System","DBPort"));
	ini.ReadText( "System", "DBName", m_LoginInfo.m_DBName, DATABASE_STR_LEN ) ;
	ini.ReadText( "System", "DBUser", m_LoginInfo.m_DBUser, DB_USE_STR_LEN ) ;
	ini.ReadText( "System", "DBPassword", m_LoginInfo.m_DBPassword, DB_PASSWORD_STR_LEN ) ;
	m_LoginInfo.m_AskAuthType	=	(BYTE)(ini.ReadInt("System","AskAuthType"));
//	ini.ReadText( "System", "ClientMainPath", m_LoginInfo.m_ClientMainPath, MAX_PATH ) ;

	Log::SaveLog( CONFIG_LOGFILE, "Load LoginInfo.ini ...Only OK! " ) ;

	__LEAVE_FUNCTION
}
VOID Config::LoadLoginInfo_Reload( )
{//可以重复读取的数据
	__ENTER_FUNCTION
		Log::SaveLog( CONFIG_LOGFILE, "Load LoginInfo.ini ...ReLoad OK! " ) ;
	__LEAVE_FUNCTION
}





VOID Config::LoadWorldInfo( )
{
__ENTER_FUNCTION

	LoadWorldInfo_Only( ) ;
	LoadWorldInfo_ReLoad( ) ;

__LEAVE_FUNCTION
}
VOID Config::LoadWorldInfo_Only( )
{
__ENTER_FUNCTION

	Ini ini( FILE_WORLD_INFO ) ;
	m_WorldInfo.m_WorldID	=		(ID_t)(ini.ReadInt( "System", "WorldId" )) ;
	m_WorldInfo.m_GuildKey	=		(SM_KEY)ini.ReadInt( "System","GuildSMKey");
	m_WorldInfo.m_MailKey	=		(SM_KEY)ini.ReadInt("System","MailSMKey");
	m_WorldInfo.m_EnableShareMem	= (BOOL)ini.ReadInt("System","EnableShareMem");
	Log::SaveLog( CONFIG_LOGFILE, "Load WorldInfo.ini ...Only OK! " ) ;

__LEAVE_FUNCTION
}
VOID Config::LoadWorldInfo_ReLoad( )
{
__ENTER_FUNCTION

	Log::SaveLog( CONFIG_LOGFILE, "Load WorldInfo.ini ...ReLoad OK! " ) ;

__LEAVE_FUNCTION
}

VOID	Config::LoadBillingInfo( ) 
{
	__ENTER_FUNCTION

		LoadBillingInfo_Only( ) ;
		LoadBillingInfo_ReLoad( ) ;

	__LEAVE_FUNCTION
}
VOID	Config::LoadBillingInfo_Only( ) 
{
	__ENTER_FUNCTION
		
		Ini ini( FILE_SERVER_INFO ) ;
		m_BillingInfo.m_BillingID = (uint)(ini.ReadInt( "Billing", "BillingId" )) ;
		ini.ReadText( "Billing", "OuterIP", m_BillingInfo.m_OuterIP, IP_SIZE ) ;
		ini.ReadText( "Billing", "IP", m_BillingInfo.m_IP, IP_SIZE ) ;
		m_BillingInfo.m_Port = (uint)(ini.ReadInt( "Billing", "Port" )) ;

		ini.ReadText( "Billing", "DBIP", m_BillingInfo.m_DBIP, IP_SIZE ) ;
		m_BillingInfo.m_DBPort = (uint)(ini.ReadInt( "Billing", "DBPort" )) ;
		ini.ReadText( "Billing", "DBName", m_BillingInfo.m_DBName, DATABASE_STR_LEN ) ;
		ini.ReadText( "Billing", "DBUser", m_BillingInfo.m_DBUser, DB_USE_STR_LEN ) ;
		ini.ReadText( "Billing", "DBPassword", m_BillingInfo.m_DBPassword, DB_PASSWORD_STR_LEN ) ;
		ini.ReadText( "Billing", "WebIP", m_BillingInfo.m_WebIP, IP_SIZE ) ;
		m_BillingInfo.m_WebPort = (uint)(ini.ReadInt( "Billing", "WebPort" )) ;
		m_BillingInfo.m_WebServerId = (uint)(ini.ReadInt( "Billing", "WebServerId" )) ;
		ini.ReadText( "Billing", "WebLinkWebKEY", m_BillingInfo.m_WebLinkWebKEY, KEY_SIZE ) ;
		Log::SaveLog( CONFIG_LOGFILE, "Load BillingInfo.ini ...Only OK! " ) ;

	__LEAVE_FUNCTION
}
VOID	Config::LoadBillingInfo_ReLoad( ) 
{
	__ENTER_FUNCTION

		Log::SaveLog( CONFIG_LOGFILE, "Load BillingInfo.ini ...ReLoad OK! " ) ;

	__LEAVE_FUNCTION
}


VOID Config::LoadShareMemInfo()
{
	__ENTER_FUNCTION
	
		LoadShareMemInfo_Only();
		LoadShareMemInfo_ReLoad();

	__LEAVE_FUNCTION
}

VOID	Config::LoadShareMemInfo_Only()
{
	__ENTER_FUNCTION

		Ini ini( FILE_SHARE_MEM_INFO );
		m_ShareMemInfo.m_SMUObjCount=(uint)ini.ReadInt( "ShareMem", "KeyCount" );
		//m_ShareMemInfo.m_SMUObjCount++; //添加ItemSerial固定Key
		m_ShareMemInfo.m_pShareData	= new _SHAREMEM_DATA[m_ShareMemInfo.m_SMUObjCount];
		

		uint i;
		for(i=0;i<m_ShareMemInfo.m_SMUObjCount;i++)
		{
			CHAR szKeyID[256] ;
			CHAR szTypeID[256];
			memset( szKeyID,	0,	256 );
			memset( szTypeID,	0,	256);
			sprintf( szKeyID, "Key%d" ,	i) ;
			sprintf(szTypeID, "Type%d",	i);
			m_ShareMemInfo.m_pShareData[i].m_Key	=	(SM_KEY)ini.ReadInt( "ShareMem", szKeyID );
			m_ShareMemInfo.m_pShareData[i].m_Type	=	(SHAREMEM_TYPE)ini.ReadInt( "ShareMem", szTypeID );
		}

		ini.ReadText( "System", "DBIP", m_ShareMemInfo.m_DBIP, IP_SIZE ) ;
		m_ShareMemInfo.m_DBPort	=	(UINT)(ini.ReadInt("System","DBPort"));
		ini.ReadText( "System", "DBName", m_ShareMemInfo.m_DBName, DATABASE_STR_LEN ) ;
		ini.ReadText( "System", "DBUser", m_ShareMemInfo.m_DBUser, DB_USE_STR_LEN ) ;
		ini.ReadText( "System", "DBPassword", m_ShareMemInfo.m_DBPassword, DB_PASSWORD_STR_LEN ) ;
		
		m_ShareMemInfo.SMUInterval	=	ini.ReadInt("System","SMUInterval");
		m_ShareMemInfo.DATAInterval	=	ini.ReadInt("System","DATAInterval");
		
		
		Log::SaveLog( CONFIG_LOGFILE, "Load ShareMemInfo.ini ...Only OK! " ) ;

	__LEAVE_FUNCTION
}

VOID	Config::LoadShareMemInfo_ReLoad()
{
	__ENTER_FUNCTION

	__LEAVE_FUNCTION
}


VOID Config::LoadMachineInfo( )
{
__ENTER_FUNCTION

	LoadMachineInfo_Only( ) ;
	LoadMachineInfo_ReLoad( ) ;

__LEAVE_FUNCTION
}
VOID Config::LoadMachineInfo_Only( )
{
__ENTER_FUNCTION

	Ini ini( FILE_MACHINE_INFO ) ;
	m_MachineInfo.m_MachineCount = ini.ReadInt( "System", "MachineNumber" ) ;

	m_MachineInfo.m_pMachine = new _MACHINE_DATA[m_MachineInfo.m_MachineCount] ;
	memset( m_MachineInfo.m_pMachine, 0, sizeof(_MACHINE_DATA)*m_MachineInfo.m_MachineCount ) ;

	for( uint i=0; i<m_MachineInfo.m_MachineCount; i++ )
	{
		CHAR szSection[256] ;
		memset( szSection, 0, 256 ) ;

		//初始化数据
		m_MachineInfo.m_pMachine[i].Init( ) ;

		//读取场景“i”
		sprintf( szSection, "Machine%d", i ) ;
		m_MachineInfo.m_pMachine[i].m_MachineID = (ID_t)(ini.ReadInt( szSection, "MachineId" )) ;
	}

	Log::SaveLog( CONFIG_LOGFILE, "Load MachineInfo.ini ...Only OK! " ) ;

__LEAVE_FUNCTION
}
VOID Config::LoadMachineInfo_ReLoad( )
{
__ENTER_FUNCTION

	Log::SaveLog( CONFIG_LOGFILE, "Load MachineInfo.ini ...ReLoad OK! " ) ;

__LEAVE_FUNCTION
}

VOID Config::LoadServerInfo( )
{
__ENTER_FUNCTION

	LoadServerInfo_Only( ) ;
	LoadServerInfo_ReLoad( ) ;

__LEAVE_FUNCTION
}
VOID Config::LoadServerInfo_Only( )
{
__ENTER_FUNCTION

	Ini ini( FILE_SERVER_INFO ) ;
	m_ServerInfo.m_ServerCount = ini.ReadInt( "System", "ServerNumber" ) ;

	m_ServerInfo.m_pServer = new _SERVER_DATA[m_ServerInfo.m_ServerCount] ;
	memset( m_ServerInfo.m_pServer, 0, sizeof(_SERVER_DATA)*m_ServerInfo.m_ServerCount ) ;

	for( uint i=0; i<m_ServerInfo.m_ServerCount; i++ )
	{
		CHAR szSection[256] ;
		memset( szSection, 0, 256 ) ;

		//初始化数据
		m_ServerInfo.m_pServer[i].Init( ) ;

		//读取场景“i”
		sprintf( szSection, "Server%d", i ) ;
		m_ServerInfo.m_pServer[i].m_ServerID = (ID_t)(ini.ReadInt( szSection, "ServerId" )) ;
		m_ServerInfo.m_pServer[i].m_MachineID = (ID_t)(ini.ReadInt( szSection, "MachineId" )) ;
		ini.ReadText( szSection, "IP0", m_ServerInfo.m_pServer[i].m_IP0, IP_SIZE ) ;
		m_ServerInfo.m_pServer[i].m_Port0 = (uint)(ini.ReadInt( szSection, "Port0" )) ;
		ini.ReadText( szSection, "IP1", m_ServerInfo.m_pServer[i].m_IP1, IP_SIZE ) ;
		m_ServerInfo.m_pServer[i].m_Port1 = (uint)(ini.ReadInt( szSection, "Port1" )) ;
		//enum SERVER_TYPE
		m_ServerInfo.m_pServer[i].m_Type = (uint)(ini.ReadInt( szSection, "Type" )) ;
		m_ServerInfo.m_pServer[i].m_HumanSMKey	=	(SM_KEY)(ini.ReadInt(szSection,"HumanSMKey"));
		m_ServerInfo.m_pServer[i].m_PlayShopSMKey = (SM_KEY)(ini.ReadInt(szSection,"PlayShopSMKey"));
		m_ServerInfo.m_pServer[i].m_ItemSerialKey = (SM_KEY)(ini.ReadInt(szSection,"ItemSerialKey"));
		m_ServerInfo.m_pServer[i].m_EnableShareMem = (BOOL)(ini.ReadInt(szSection,"EnableShareMem"));


	}

	ini.ReadText( "World", "IP", m_ServerInfo.m_World.m_IP, IP_SIZE ) ;
	m_ServerInfo.m_World.m_Port = (uint)(ini.ReadInt( "World", "Port" )) ;


	//
	for(UINT i=0; i<m_ServerInfo.m_ServerCount; i++ )
	{
		ID_t ServerID = m_ServerInfo.m_pServer[i].m_ServerID ;

		Assert( ServerID != INVALID_ID && ServerID < OVER_MAX_SERVER ) ;

		Assert( m_ServerInfo.m_HashServer[ServerID] == -1 ) ;

		m_ServerInfo.m_HashServer[ServerID] = i ;
	}

	Log::SaveLog( CONFIG_LOGFILE, "Load ServerInfo.ini ...Only OK! " ) ;

__LEAVE_FUNCTION
}
VOID Config::LoadServerInfo_ReLoad( )
{
__ENTER_FUNCTION

	Log::SaveLog( CONFIG_LOGFILE, "Load ServerInfo.ini ...ReLoad OK! " ) ;

__LEAVE_FUNCTION
}

VOID Config::LoadSceneInfo( )
{
__ENTER_FUNCTION

	LoadSceneInfo_Only( ) ;
	LoadSceneInfo_ReLoad( ) ;

__LEAVE_FUNCTION
}
VOID Config::LoadSceneInfo_Only( )
{
__ENTER_FUNCTION

	Ini ini( FILE_SCENE_INFO ) ;

	//读取场景数量
	m_SceneInfo.m_SceneCount = (uint)(ini.ReadInt( "system", "scenenumber" )) ;

	m_SceneInfo.m_pScene = new _SCENE_DATA[m_SceneInfo.m_SceneCount] ;
	memset( m_SceneInfo.m_pScene, 0, sizeof(_SCENE_DATA)*m_SceneInfo.m_SceneCount ) ;


	for( uint i=0; i<m_SceneInfo.m_SceneCount; i++ )
	{
		CHAR szSection[256] ;
		memset( szSection, 0, 256 ) ;

		//读取场景“i”
		sprintf( szSection, "scene%d", i ) ;
		m_SceneInfo.m_pScene[i].m_SceneID = (SceneID_t)i ;
		m_SceneInfo.m_pScene[i].m_IsActive = ini.ReadInt( szSection, "active" ) ;
		ini.ReadText( szSection, "name", m_SceneInfo.m_pScene[i].m_szName, _MAX_PATH ) ;
		ini.ReadText( szSection, "file", m_SceneInfo.m_pScene[i].m_szFileName, _MAX_PATH ) ;
		m_SceneInfo.m_pScene[i].m_ServerID = (ID_t)(ini.ReadInt( szSection, "serverId" )) ;
		m_SceneInfo.m_pScene[i].m_Type = (ID_t)(ini.ReadInt( szSection, "type" )) ;
		m_SceneInfo.m_pScene[i].m_ThreadIndex = (ID_t)(ini.ReadInt( szSection, "threadindex" )) ;
	}

	//
	for(UINT i=0; i<m_SceneInfo.m_SceneCount; i++ )
	{
		SceneID_t SceneID = m_SceneInfo.m_pScene[i].m_SceneID ;

		Assert( SceneID!=INVALID_ID && SceneID<MAX_SCENE ) ;

		Assert( m_SceneInfo.m_HashScene[SceneID]==-1 ) ;

		m_SceneInfo.m_HashScene[SceneID] = i ;
	}

	Log::SaveLog( CONFIG_LOGFILE, "Load SceneInfo.ini ...Only OK! " ) ;

__LEAVE_FUNCTION
}
VOID Config::LoadSceneInfo_ReLoad( )
{
__ENTER_FUNCTION

	Log::SaveLog( CONFIG_LOGFILE, "Load SceneInfo.ini ...ReLoad OK! " ) ;

__LEAVE_FUNCTION
}

ID_t Config::SceneID2ServerID(SceneID_t sID) const
{
	__ENTER_FUNCTION

	Assert(sID>=0);
	Assert(sID<(INT)m_SceneInfo.m_SceneCount);
	return	m_SceneInfo.m_pScene[m_SceneInfo.m_HashScene[sID]].m_ServerID;

	__LEAVE_FUNCTION

	return -1;
}

ID_t Config::Key2ServerID(SM_KEY key) const
{
	__ENTER_FUNCTION
	
		Assert(key>0);
		
			for( UINT i=0; i<m_ServerInfo.m_ServerCount; i++ )
			{

				if(m_ServerInfo.m_pServer[i].m_EnableShareMem)
				{
					if(m_ServerInfo.m_pServer[i].m_ItemSerialKey == key)
					{
						return	m_ServerInfo.m_pServer[i].m_ServerID;
					}
				}
				
			}

	return -1;
		
	__LEAVE_FUNCTION

	return -1;	
}

