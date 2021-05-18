

#ifndef __CGTEST_H__
#define __CGTEST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

#define MAX_SAY_SIZE 64
struct _PCGTEST
{
	SceneID_t	SceneID ;
	CHAR		szSay[MAX_SAY_SIZE] ;
	INT			EnterScene ;
	BOOL		Valid ;
	SOCKET		s ;
	UINT		Status ;
	PlayerID_t	PlayerID ;
	ID_t		PlayerManagerID ;
	ObjID_t		MonsterID;

	_PCGTEST()
	{
		SceneID = INVALID_ID ;
		memset( szSay, 0, MAX_SAY_SIZE ) ;
		EnterScene = -2 ;
		Valid = FALSE ;
		s = INVALID_SOCKET ;
		Status = PS_SERVER_EMPTY ;
		PlayerID = INVALID_ID ;
		PlayerManagerID = INVALID_ID ;
	};
};


namespace Packets
{


class CGTest : public Packet 
{
public:
	CGTest( ){} ;
	virtual ~CGTest( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_TEST ; }
	virtual UINT			GetPacketSize()const { return	sizeof(UINT)+
															sizeof(_PCGTEST); }
	
public:
	//使用数据接口
	UINT					GetType( ){ return m_Type ; } ;
	VOID					SetType( UINT type ){ m_Type = type ; } ;
	_PCGTEST*				GetTest( ){ return &m_Test ; } ;
	VOID					SetTest( _PCGTEST* test ){ m_Test = *test ; } ;

	//类型
	enum {
		TYPE_CG_ASK_ENTER_SCENE = 0 ,
		TYPE_CG_DO_ENTER_SCENE ,
		TYPE_CG_ASK_CHANGE_SCENE ,
		TYPE_CG_DO_CHANGE_SCENE ,
		TYPE_CG_SHUTDOWN_SERVER ,
		TYPE_CG_SAY_ALLSCENE ,
		TYPE_CG_DROP_ITEM_BOX,
		TYPE_CG_MONSTER_DIE,
	};
private:
	//数据
	UINT		m_Type ;
	_PCGTEST	m_Test ;

};


class CGTestFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGTest() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_TEST ; }
	UINT		GetPacketMaxSize()const { return	sizeof(UINT)+
													sizeof(_PCGTEST); }
};


class CGTestHandler 
{
public:
	static UINT Execute( CGTest* pPacket, Player* pPlayer ) ;
};


};

using namespace Packets ;


#endif
