#ifndef __GCCHARALLTITLES_H__
#define __GCCHARALLTITLES_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class	GCCharAllTitles : public Packet 
	{
	public:
		struct TitleId
		{
			BYTE	m_titleid;
			SHORT	m_titleData;
		};
		struct TitleString
		{
			BYTE	m_titleid;
			BYTE	m_strlength;					//字符串长度
			CHAR	m_szTitle[MAX_CHARACTER_TITLE];	// 玩家头衔
		};
	public:
		GCCharAllTitles( ){} ;
		virtual ~GCCharAllTitles( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHARALLTITLES ; }
		virtual UINT			GetPacketSize()const 
		{
			INT Lengthstr = 0;
			for(INT i=0; i<m_nTitleStr; i++)
			{
				Lengthstr += m_TitleStrList[i].m_strlength+sizeof(BYTE)*2;
			}
			int a =sizeof(ObjID_t)+2*sizeof(BYTE)+(sizeof(BYTE)+sizeof(SHORT))*m_nTitleId+Lengthstr;  
			return a;
		}

	public:
		//使用数据接口
		ObjID_t					getTargetID(VOID) { return m_ObjID; }
		VOID					setTargetID(ObjID_t idTarget) { m_ObjID = idTarget; }

		BYTE					GetTitleIdNum(VOID) const {return m_nTitleId;};
		VOID					SetTitleIdNum(BYTE nNumber) {m_nTitleId = nNumber;};

		TitleId*				GetTitleIdList(VOID) { return m_TitleIdList; }
		VOID					SetTitleIdList(TitleId* pSour)
		{
			memcpy(m_TitleIdList, pSour, m_nTitleId*sizeof(TitleId));
		}

		BYTE					GetTitleStrNum(VOID) const {return m_nTitleStr;};
		VOID					SetTitleStrNum(BYTE nNumber) {m_nTitleStr = nNumber;};

		TitleString*			GetTitleStrList(VOID) { return m_TitleStrList; }
		VOID					SetTitleStrList(TitleString* pSour)
		{
			memcpy(m_TitleStrList, pSour, m_nTitleStr*sizeof(TitleString));
		}


	private:

		//数据
		ObjID_t					m_ObjID;	
		//Title个数
		BYTE					m_nTitleId;
		//所有Titles列表
		TitleId					m_TitleIdList[MAX_TITLE_ID_SIZE];
		//Title个数
		BYTE					m_nTitleStr;
		//所有Titles字符串列表
		TitleString				m_TitleStrList[MAX_STRING_TITLE_SIZE];
	};


	class GCCharAllTitlesFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCCharAllTitles() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_CHARALLTITLES ; }
		UINT		GetPacketMaxSize()const { return sizeof(ObjID_t) + 2*sizeof(BYTE) + (sizeof(BYTE)+sizeof(SHORT))*MAX_TITLE_ID_SIZE + (sizeof(BYTE)+sizeof(BYTE)+sizeof(CHAR)*MAX_CHARACTER_TITLE)*MAX_STRING_TITLE_SIZE;  }
	};


	class GCCharAllTitlesHandler 
	{
	public:
		static UINT Execute( GCCharAllTitles* pPacket, Player* pPlayer ) ;
	};
};
using namespace Packets ;

#endif
