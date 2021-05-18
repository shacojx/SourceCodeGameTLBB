

#ifndef __GWChat_H__
#define __GWChat_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "CGChat.h"


namespace Packets
{
	class GWChat : public Packet 
	{
	public:
		GWChat( ){} ;
		virtual ~GWChat( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GW_CHAT ; }
		virtual UINT			GetPacketSize()const 
		{ 
			Assert( m_ContexSize<=MAX_CHAT_SIZE ) ;
			INT iSize = sizeof(GUID_t)+
						sizeof(BYTE)+
						sizeof(BYTE)+
						sizeof(CHAR)*m_ContexSize ;

			switch( m_ChatType )
			{
			case CHAT_TYPE_NORMAL:
			case CHAT_TYPE_SCENE:
			case CHAT_TYPE_SYSTEM:
				break ;
			case CHAT_TYPE_TEAM:
				iSize += sizeof(TeamID_t) ;
				break ;
			case CHAT_TYPE_TELL:
				Assert( m_TargetSize<=MAX_CHARACTER_NAME ) ;
				iSize += (sizeof(BYTE)+m_TargetSize*sizeof(CHAR)) ;
				break ;
			case CHAT_TYPE_CHANNEL:
				iSize += sizeof(ChannelID_t) ;
				break ;
			case CHAT_TYPE_GUILD:
				iSize += sizeof(GuildID_t) ;
				break ;
			case CHAT_TYPE_MENPAI:
				iSize += sizeof(BYTE) ;
				break ;
			default :
				Assert(FALSE) ;
				break ;
			}

			return (UINT)iSize ; 
		}

	public :

	public:
		//使用数据接口
		VOID					SetSourGUID( GUID_t guid ){ m_SourGUID = guid ; } ;
		GUID_t					GetSourGUID( ){ return m_SourGUID ; } ;

		VOID					SetChatType( BYTE bType ){ m_ChatType = bType ; } ;
		BYTE					GetChatType( ){ return m_ChatType ; } ;

		VOID					SetContexSize( BYTE bSize ){ m_ContexSize = bSize ; } ;
		BYTE					GetContexSize( ){ return m_ContexSize ; } ;

		VOID					SetContex( CHAR* pContex ){
			strncpy( m_Contex, pContex, MAX_CHAT_SIZE-1 ) ;
			m_Contex[MAX_CHAT_SIZE] = 0 ;
		};
		CHAR*					GetContex(){ return (CHAR*)m_Contex ; } ;

		VOID					SetTargetSize( BYTE bSize ){ m_TargetSize = bSize ; } ;
		BYTE					GetTargetSize( ){ return m_TargetSize ; } ;

		VOID					SetTargetName( CHAR* pTargetName ){
			strncpy( m_TargetName, pTargetName, MAX_CHARACTER_NAME-1 ) ;
			m_TargetName[MAX_CHARACTER_NAME] = 0 ;
		};
		CHAR*					GetTargetName(){ return (CHAR*)m_TargetName ; } ;

		VOID					SetTeamID( TeamID_t tid ){ m_TeamID = tid ; } ;
		TeamID_t				GetTeamID(){ return m_TeamID ; } ;

		VOID					SetChannelID( ChannelID_t cid ){ m_ChannelID = cid ; };
		ChannelID_t				GetChannelID( ){ return m_ChannelID ; } ;

		VOID					SetGuildID( GuildID_t tid ){ m_GuildID = tid ; } ;
		GuildID_t				GetGuildID(){ return m_GuildID ; } ;

		VOID					SetMenpaiID( BYTE menpai ){ m_MenpaiID = menpai ; } ;
		BYTE					GetMenpaiID(){ return m_MenpaiID ; } ;

	private:
		//说话的玩家
		GUID_t					m_SourGUID ;

		//数据
		BYTE					m_ChatType ;//聊天消息类型 enum CHAT_TYPE
		
		//聊天内容数据
		BYTE					m_ContexSize ;
		CHAR					m_Contex[MAX_CHAT_SIZE] ;
		
		//私聊对象的角色名字，仅在CHAT_TYPE_TELL时有效
		BYTE					m_TargetSize ;
		CHAR					m_TargetName[MAX_CHARACTER_NAME] ;

		//队伍号，仅在队伍聊天时有效
		TeamID_t				m_TeamID ;
		
		//频道号，仅在自建聊天频道聊天时有效
		ChannelID_t				m_ChannelID ;

		//帮派号，仅属于此帮派的成员有效
		GuildID_t				m_GuildID ;

		//门派值，仅此门派内的成员有效
		BYTE					m_MenpaiID ;
	};


	class GWChatFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GWChat() ; }
		PacketID_t	GetPacketID()const { return PACKET_GW_CHAT ; }
		UINT		GetPacketMaxSize()const { return	sizeof(GUID_t)+
														sizeof(BYTE)+
														sizeof(BYTE)+
														sizeof(CHAR)*MAX_CHAT_SIZE+
														sizeof(BYTE)+
														sizeof(CHAR)*MAX_CHARACTER_NAME+
														sizeof(TeamID_t)+
														sizeof(ChannelID_t)+
														sizeof(GuildID_t)+
														sizeof(BYTE) ; }
	};


	class GWChatHandler 
	{
	public:
		static UINT Execute( GWChat* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
