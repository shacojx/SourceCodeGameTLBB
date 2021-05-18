/////////////////////////////////////////////////////////////////////////////////
//文件名：GuildManager.h
//功能描述：帮会管理逻辑
/////////////////////////////////////////////////////////////////////////////////


#ifndef __GUILDMANAGER_H__
#define __GUILDMANAGER_H__

#include "Type.h"
#include "Guild.h"
#include "GameDefine_Guild.h"
#include "StrTable.h"




struct _GUILD_MANAGER_INIT
{
	INT					m_GuildCount;
	_GUILD_INIT*		m_pGuildInit;

	_GUILD_MANAGER_INIT( )
	{
		m_GuildCount = 0;
		m_pGuildInit = NULL;
	}
	~_GUILD_MANAGER_INIT( )
	{
		m_GuildCount = 0;
		SAFE_DELETE_ARRAY( m_pGuildInit );
	}
};


class GuildManager
{
public :
	GuildManager( );
	~GuildManager( );

	BOOL				Load( CHAR* pGuildFile );//从磁盘读取帮会信息（临时使用）
	VOID				Save( CHAR* pGuildFile );//将当前帮会信息保存到磁盘（临时使用）

	BOOL				Init( );
	//BOOL				Init( _GUILD_MANAGER_INIT* pInit );//初始化帮会信息
	VOID				CleanUp( );

	BOOL				HeartBeat( UINT uTime=0 );//逻辑处理

	GUILD_ERROR_TYPE	CreateGuild( USER* pUser, const CHAR* szName, CampID_t nCamp );//创建一个帮会
	GUILD_ERROR_TYPE	DestroyGuild( GUID_t userGUID, GuildID_t guildID );//删除一个帮会

	Guild*				GetGuild( GuildID_t guildID );//读取帮会
	Guild*				GetGuild( const CHAR* szName ); // 查找一个帮会

	// 根据一个 guildid 号码得到帮会名字，如果不存在则返回 ""
	const CHAR*			GetGuildName( GuildID_t guildID );

	// 获取帮会列表
	BOOL				GetGuildList( GUILD_WGC_LIST* pGuildList, INT nStartIndex, CampID_t nCamp );

	// 注册一个帮会名称（不允许被重复使用）
	VOID				RegisterGuildName( Guild* pGuild );

	// 注销一个帮会名称（可被再次使用）
	VOID				UnregisterGuildName( Guild* pGuild );

protected :
	Guild				m_aGuilds[MAX_GUILD_SIZE];
	INT					m_GuildCount;
	INT					m_GuildMallocIndex;
	StrTable			m_NameTable;


};

extern GuildManager* g_pGuildManager;


#endif
