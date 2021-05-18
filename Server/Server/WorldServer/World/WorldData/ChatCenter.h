
#ifndef _CHATCENTER_H_
#define _CHATCENTER_H_

#include "Type.h"

class Packet ;
class USER ;

//邮件数据
class ChatDB
{
public :
	ChatDB( ) ;
	~ChatDB( ) ;

	BOOL				Init( ) ;

	BOOL				LoadMail( GUID_t guid ) ;
	BOOL				SaveMail( GUID_t guid ) ;

public :

};

//聊天频道
class ChatChannel
{
public :
	ChatChannel( ) ;
	~ChatChannel( ) ;


public :
	void			CleanUp( ) ;

	BOOL			IsActive( ) ;
	
	BOOL			AddMember( GUID_t guid ) ;
	BOOL			DelMember( GUID_t guid ) ;
	BOOL			IsMember( GUID_t guid ) ;
	GUID_t			Leader( ) ;
	GUID_t			Member( int iIndex ) ;
	int				MemberCount( ){ return m_iNumber ; }
	BOOL			IsFull( ){ return (BOOL)(m_iNumber==MAX_CHATCHANNEL_MEMBER) ; }

	void			SetChannelID( ChannelID_t cid ){
		Assert( cid>=0 && cid<MAX_CHANNELS ) ;
		m_ChannelID = cid ;
	};
	ChannelID_t		GetChannelID( ){ return m_ChannelID ;	}

	BOOL			IsEmpty( ){	return m_Empty ; }
	void			SetEmpty( BOOL bEmpty=TRUE ){ m_Empty = bEmpty ; }


public :
	GUID_t			m_aMember[MAX_CHATCHANNEL_MEMBER] ;
	int				m_iNumber ;

	ChannelID_t		m_ChannelID ;
	BOOL			m_Empty ;
};

#define MAX_WORLD_HOLD_CHAT_PACKETS 512

//聊天中心
class ChatCenter
{
public :
	ChatCenter( ) ;
	~ChatCenter( ) ;

	BOOL				Init( ) ;
	void				CleanUp( ) ;
	
	BOOL				HeartBeat( DWORD dwTime ) ;

	ChatChannel*		GetChatChannel( ChannelID_t cid ){
		Assert( cid>=0 && cid<MAX_CHANNELS ) ;
		return &(m_pChannels[cid]) ;
	};
	ChannelID_t			CreateChatChannel( ) ;
	BOOL				DestoryChatChannel( ChannelID_t tid ) ;

	VOID				PushChatPacket( Packet* pPacket ) ;
	VOID				SendNeedPacket( USER* pUser, UINT uWorldChatID ) ;
	UINT				NextWorldChatID( ){ m_uWorldChatID++ ; return m_uWorldChatID ; }


public :
	ChatDB				m_MailDB ;
	
	ChatChannel*		m_pChannels ;
	int					m_iCount ;
	int					m_iCreateOffset ;

	Packet*				m_pChatPackets[MAX_WORLD_HOLD_CHAT_PACKETS] ;
	UINT				m_uCurrentChat ;

	UINT				m_uWorldChatID ;

};

extern ChatCenter* g_pChatCenter ;

#endif
