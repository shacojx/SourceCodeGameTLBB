#include "stdafx.h"

#include "GuildManager.h"
#include "OnlineUser.h"
#include "Config.h"
#include "City.h"

GuildManager* g_pGuildManager=NULL;

GuildManager::GuildManager( )
{
__ENTER_FUNCTION

	CleanUp( );

__LEAVE_FUNCTION
}

GuildManager::~GuildManager( )
{
__ENTER_FUNCTION

	CleanUp( );

__LEAVE_FUNCTION
}

BOOL GuildManager::Load( CHAR* pGuildFile )
{
__ENTER_FUNCTION


	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

VOID GuildManager::Save( CHAR* pGuildFile )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

BOOL GuildManager::Init( )
{
__ENTER_FUNCTION

	m_NameTable.InitTable(MAX_GUILD_SIZE, MAX_GUILD_NAME_SIZE);
	
	_WORLD_INFO& WorldInfo = g_Config.m_WorldInfo;
	
	if(WorldInfo.m_EnableShareMem)
	{
		_GUILD_INIT	GuildInit;
		INT i;

		for( i=0; i<MAX_GUILD_SIZE; i++ )
		{
			Guild* pGuild = GetGuild( i );
			if( pGuild==NULL )
			{
				Assert(FALSE);
				continue;
			}

			if( pGuild->InitFromShareMem() )
				m_GuildCount ++;
			
		}
		
	}

	
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}
/*
BOOL GuildManager::Init( _GUILD_MANAGER_INIT* pInit )
{
__ENTER_FUNCTION

	m_GuildCount = pInit->m_GuildCount;
	INT i;
	for( i=0; i<pInit->m_GuildCount; i++ )
	{
		GuildID_t guildID = pInit->m_pGuildInit[i].m_GuildID;
		Guild* pGuild = GetGuild( guildID );
		if( pGuild==NULL )
		{
			Assert(FALSE);
			continue;
		}

		pGuild->Init( &(pInit->m_pGuildInit[i]) );
	}

	for( i=0; i<MAX_GUILD_SIZE; i++ )
	{
		Guild* pGuild = GetGuild( i );
		if( pGuild==NULL )
		{
			Assert(FALSE);
			continue;
		}

		if( pGuild->GetGuildID( ) == INVALID_ID )
		{
			m_GuildMallocIndex = i;
			break;
		}
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}
*/
VOID GuildManager::CleanUp( )
{
__ENTER_FUNCTION


	for( INT i=0; i<MAX_GUILD_SIZE; i++ )
	{
		m_aGuilds[i].CleanUp();
	}
	m_GuildCount = 0;
	m_GuildMallocIndex = 0;

__LEAVE_FUNCTION
}

BOOL GuildManager::HeartBeat( UINT uTime )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_GUILD_SIZE; i++ )
	{
		if( m_aGuilds[i].GetGuildID()==INVALID_ID )
			continue;

		m_aGuilds[i].HeartBeat( uTime );
	}


	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

GUILD_ERROR_TYPE GuildManager::CreateGuild( USER* pUser, const CHAR* szName, CampID_t nCamp )
{
__ENTER_FUNCTION

	Guild* pGuild = NULL;

	// 验一下名字是否合法，不合法就返回 GUILD_ERROR_INVALID_NAME
	// 不过这一步也可以放到 Server 做
	
	if( m_GuildCount >= MAX_GUILD_SIZE )
	{ // 没有空地方了
		return GUILD_ERROR_GUILD_FULL;
	}
	
	INT i;
	for( i=0; i<MAX_GUILD_SIZE; ++i, ++m_GuildMallocIndex )
	{
		if( m_GuildMallocIndex >= MAX_GUILD_SIZE )
		{
			m_GuildMallocIndex = 0;
		}

		if( m_aGuilds[m_GuildMallocIndex].GetGuildID() == INVALID_ID )
		{
			break;
		}
	}

	if(i >= MAX_GUILD_SIZE )
	{ // 没有空地方了
		return GUILD_ERROR_GUILD_FULL;
	}

	pGuild = GetGuild(szName);
	if( pGuild != NULL )
	{ // 重名了
		return GUILD_ERROR_DUPLICATED_NAME;
	}

	GuildID_t guildID = m_GuildMallocIndex;  // 暂时不考虑加入世界、服务器编号

	pGuild = GetGuild(guildID);
	if( pGuild == NULL )
	{
		Assert(FALSE);
		return GUILD_ERROR;
	}

	_GUILD_INIT GuildInit;
	GuildInit.m_GuildID = guildID;
	pGuild->Init(&GuildInit);

	if( pGuild->OnGuildCreate(pUser, szName, nCamp) )
	{
		pUser->SetGuildID( guildID );
		++m_GuildMallocIndex;
		++m_GuildCount;
		return GUILD_ERROR_NOTHING;
	}
	else
	{
		pGuild->OnGuildCreateFailed();
		return GUILD_ERROR;
	}

__LEAVE_FUNCTION

	return GUILD_ERROR;
}

GUILD_ERROR_TYPE GuildManager::DestroyGuild( GUID_t userGUID, GuildID_t guildID )
{
__ENTER_FUNCTION

	Guild* pGuild = GetGuild(guildID);
	if( pGuild == NULL )
	{
		Assert(FALSE);
		return GUILD_ERROR_NOT_EXIST;
	}

	if( pGuild->OnGuildDestroy(userGUID) == FALSE )
	{
		return GUILD_ERROR;
	}

	UnregisterGuildName(pGuild);
	pGuild->CleanUp();
	pGuild->ValidateShareMem();
	--m_GuildCount;
	return GUILD_ERROR_NOTHING;

__LEAVE_FUNCTION

	return GUILD_ERROR;
}

Guild* GuildManager::GetGuild( GuildID_t guildID )
{
__ENTER_FUNCTION

	if( guildID >= 0 )
	{
		guildID %= MAX_GUILD_SIZE;
		return &(m_aGuilds[guildID]);
	}

__LEAVE_FUNCTION

	return NULL;
}

// 查找一个帮会
Guild* GuildManager::GetGuild( const CHAR* szName )
{
	return (Guild*)(m_NameTable.Get(szName));
}

// 根据一个 guildid 号码得到帮会名字，如果不存在则返回 ""
const CHAR* GuildManager::GetGuildName( GuildID_t guildID )
{
__ENTER_FUNCTION

	Guild* pGuild = GetGuild( guildID );
	if( pGuild != NULL )
	{
		return pGuild->GetGuildName();
	}

__LEAVE_FUNCTION

	return "";
}

// 获取帮会列表
BOOL GuildManager::GetGuildList( GUILD_WGC_LIST* pGuildList, INT nStartIndex, CampID_t nCamp )
{
__ENTER_FUNCTION

	int nMaxListCount; // pGuildList 最多能放几个帮会数据
	int nNowIndex = 0; // 遍历到的第 n 个帮会

	Assert( nStartIndex >= 0 );
	Assert( nStartIndex <= m_GuildCount );
	//nMaxListCount = (sizeof(pGuildList->m_uGuild) / sizeof(GUILD_WGC_LIST::s));
	nMaxListCount = MAX_GUILD_LIST_COUNT;
	Assert( nMaxListCount > 0 );

	for( INT i=0; i<MAX_GUILD_SIZE; ++i )
	{
		if( m_aGuilds[i].GetGuildID() == INVALID_ID )
		{
			continue;
		}

		if( m_aGuilds[i].GetAmbi() != (INT)nCamp )
		{
			continue;
		}

		if( nNowIndex >= nStartIndex )
		{ // 从这里开始记录
			if( nNowIndex >= nStartIndex + nMaxListCount )
			{ // 够数退出
				break;
			}

			GUILD_WGC_LIST::s* pGuildInfo = &(pGuildList->m_uGuild[pGuildList->m_uGuildListCount++]);
			//工会ID
			pGuildInfo->m_GuildID = m_aGuilds[i].GetGuildID();
			//工会名
			strncpy(pGuildInfo->m_GuildName, m_aGuilds[i].GetGuildName(), MAX_GUILD_NAME_SIZE);
			//工会宗旨
			strncpy(pGuildInfo->m_GuildDesc, m_aGuilds[i].GetGuildDesc(), MAX_GUILD_DESC_SIZE);
			//帮主名
			strncpy(pGuildInfo->m_ChiefName, m_aGuilds[i].GetGuildChiefName(), MAX_CHARACTER_NAME);
			//城市名
			if(m_aGuilds[i].GetCity())
			{
				strncpy(pGuildInfo->m_CityName, m_aGuilds[i].GetCity()->GetCityName(), MAX_CITY_NAME_SIZE);
				pGuildInfo->m_PortSceneID = m_aGuilds[i].GetCity()->GetPortSceneID();
			}
			else
			{
				memset(pGuildInfo->m_CityName, 0, MAX_CITY_NAME_SIZE);
				pGuildInfo->m_PortSceneID = -1;
			}
			//工会状态
			pGuildInfo->m_uGuildStatus		=	m_aGuilds[i].GetGuildStatus();
			//工会级别
			pGuildInfo->m_bGuildLevel		=	m_aGuilds[i].GetGuildLevel();
			//帮众数量
			pGuildInfo->m_uGuildUserCount	=	m_aGuilds[i].GetGuildUserCount();
			//成立时间
			pGuildInfo->m_nFoundedTime		=	m_aGuilds[i].GetFoundedTime();
		}

		++nNowIndex;
	}

	pGuildList->m_uGuildCount = m_GuildCount;

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;

}

// 注册一个帮会名称（不允许被重复使用）
VOID GuildManager::RegisterGuildName( Guild* pGuild )
{
	m_NameTable.Add( pGuild->GetGuildName(), pGuild );
}

// 注销一个帮会名称（可被再次使用）
VOID GuildManager::UnregisterGuildName( Guild* pGuild )
{
	m_NameTable.Remove( pGuild->GetGuildName() );
}
