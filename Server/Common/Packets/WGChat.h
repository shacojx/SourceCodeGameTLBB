

#ifndef __WGCHAT_H__
#define __WGCHAT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "CGChat.h"

 
namespace Packets
{
	class WGChat : public Packet 
	{
	public:
		WGChat( ){ Reset() ; } ;
		virtual ~WGChat( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_WG_CHAT ; }
		virtual UINT			GetPacketSize()const 
		{ 
			if( m_ChatType == CHAT_TYPE_SYSTEM 
				|| m_ChatType == CHAT_TYPE_SCENE  )
			{
				return	sizeof(BYTE)+
						sizeof(BYTE)+
						m_ContexSize*sizeof(CHAR)+
						sizeof(BYTE)+
						m_SourNameSize*sizeof(CHAR)+
						sizeof(CampID_t)+
						sizeof(UINT) ; 
			}
			else if(	m_ChatType == CHAT_TYPE_CHANNEL
					 ||	m_ChatType == CHAT_TYPE_TEAM 
					 ||	m_ChatType == CHAT_TYPE_TELL
					 || m_ChatType == CHAT_TYPE_SELF )
			{
				return	sizeof(BYTE)+
						sizeof(BYTE)+
						m_ContexSize*sizeof(CHAR)+
						sizeof(BYTE)+
						m_SourNameSize*sizeof(CHAR)+
						sizeof(PlayerID_t)+
						sizeof(CampID_t)+
						sizeof(UINT) ; 
			}
			else if(	m_ChatType == CHAT_TYPE_GUILD )
			{
				return	sizeof(BYTE)+
						sizeof(BYTE)+
						m_ContexSize*sizeof(CHAR)+
						sizeof(BYTE)+
						m_SourNameSize*sizeof(CHAR)+
						sizeof(GuildID_t)+
						sizeof(CampID_t)+
						sizeof(UINT) ;
			}
			else if(	m_ChatType == CHAT_TYPE_MENPAI )
			{
				return	sizeof(BYTE)+
						sizeof(BYTE)+
						m_ContexSize*sizeof(CHAR)+
						sizeof(BYTE)+
						m_SourNameSize*sizeof(CHAR)+
						sizeof(BYTE)+
						sizeof(CampID_t)+
						sizeof(UINT) ;
			}
			else
			{
				Assert(FALSE) ;
				return	sizeof(BYTE)+
						sizeof(BYTE)+
						m_ContexSize*sizeof(CHAR)+
						sizeof(BYTE)+
						m_SourNameSize*sizeof(CHAR)+
						sizeof(CampID_t)+
						sizeof(UINT) ; 
			}
		}

		VOID	Reset()
		{
			m_ChatType = 0 ;
			m_ContexSize = 0 ;
			m_SourNameSize = 0 ;
			memset( m_Contex, 0, MAX_CHAT_SIZE ) ;
			memset( m_SourName, 0, MAX_CHARACTER_NAME ) ;
			m_PlayerID = INVALID_ID ;
			m_GuildID = INVALID_ID ;
			m_MenpaiID = MATTRIBUTE_WUMENPAI ;
			m_Camp = -1;
			m_uWorldChatID = 0 ;
		};

	public:
		//使用数据接口
		//设置、读取聊天类型 见：CHAT_TYPE
		VOID					SetChatType( BYTE bType ){ m_ChatType = bType ; } ;
		BYTE					GetChatType( ){ return m_ChatType ; } ;

		//设置、读取聊天内容
		VOID					SetContexSize( BYTE bSize ){ m_ContexSize = bSize ; } ;
		BYTE					GetContexSize( ){ return m_ContexSize ; } ;

		VOID					SetContex( CHAR* pContex ){
			strncpy( m_Contex, pContex, MAX_CHAT_SIZE-1 ) ;
			m_Contex[MAX_CHAT_SIZE] = 0 ;
		};
		CHAR*					GetContex(){ return (CHAR*)m_Contex ; } ;

		//设置、读取说话人的名字
		VOID					SetSourNameSize( BYTE bSize ){ m_SourNameSize = bSize ; } ;
		BYTE					GetSourNameSize( ){ return m_SourNameSize ; } ;

		VOID					SetSourName( const CHAR* pSourName ){
			strncpy( m_SourName, pSourName, MAX_CHARACTER_NAME-1 ) ;
			m_SourName[MAX_CHARACTER_NAME] = 0 ;
		};
		CHAR*					GetSourName(){ return (CHAR*)m_SourName ; } ;

		VOID					SetPlayerID( PlayerID_t pid ){ m_PlayerID = pid ; } ;
		PlayerID_t				GetPlayerID( ){ return m_PlayerID ; } ;

		VOID					SetGuildID( GuildID_t tid ){ m_GuildID = tid ; } ;
		GuildID_t				GetGuildID(){ return m_GuildID ; } ;

		VOID					SetWorldChatID( UINT uid ){ m_uWorldChatID = uid ; }
		UINT					GetWorldChatID( ){ return m_uWorldChatID ; }

		VOID					SetMenpaiID( BYTE menpai ){ m_MenpaiID = menpai ; } ;
		BYTE					GetMenpaiID(){ return m_MenpaiID ; } ;

		VOID					SetCampID( CampID_t id ) { m_Camp = id; }
		CampID_t				GetCampID() { return m_Camp; }

	private:
		//数据
		BYTE					m_ChatType ;//聊天消息类型 enum CHAT_TYPE
		
		//聊天内容数据
		BYTE					m_ContexSize ;
		CHAR					m_Contex[MAX_CHAT_SIZE] ;

		//说话者名字
		BYTE					m_SourNameSize ;
		CHAR					m_SourName[MAX_CHARACTER_NAME] ;

		PlayerID_t				m_PlayerID ;	//连接者: 私聊、队聊、自建频道聊
		GuildID_t				m_GuildID ;		//工会聊有效
		BYTE					m_MenpaiID ;	//门派值，仅此门派内的成员有效
		CampID_t				m_Camp;			//发送者的阵营
		UINT					m_uWorldChatID ; //聊天消息的序列号，用于纠正玩家转移场景消息丢失问题
	};


	class WGChatFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WGChat() ; }
		PacketID_t	GetPacketID()const { return PACKET_WG_CHAT ; }
		UINT		GetPacketMaxSize()const { return	sizeof(BYTE)+
														sizeof(BYTE)+
														MAX_CHAT_SIZE*sizeof(CHAR)+
														sizeof(BYTE)+
														MAX_CHARACTER_NAME*sizeof(CHAR)+
														sizeof(PlayerID_t)+
														sizeof(GuildID_t)+
														sizeof(BYTE)+
														sizeof(CampID_t)+
														sizeof(UINT) ; }
	};


	class WGChatHandler 
	{
	public:
		static UINT Execute( WGChat* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
