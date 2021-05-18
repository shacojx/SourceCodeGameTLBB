// CGManipulatePet.h
// 
// 请求操作宠物返回结果
//
//////////////////////////////////////////////////////

#ifndef __CG_MANIPULATEPET_H__
#define __CG_MANIPULATEPET_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	enum ENUM_MANIPULATE_TYPE
	{
		MANIPULATE_INVALID = -1,	// 无效
		MANIPULATE_CREATEPET,		// 召唤宠物
		MANIPULATE_DELETEPET,		// 收回宠物
		MANIPULATE_FREEPET,			// 放生宠物
		MANIPULATE_ASKOTHERPETINFO, // 察看其他玩家的宠物信息(宠物征友等用...)


		MANIPULATE_NUMBERS,
	};

	class CGManipulatePet : public Packet
	{
	public:

		CGManipulatePet( )
		{
		}

		~CGManipulatePet( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_MANIPULATEPET ; }
		virtual UINT			GetPacketSize()const 
		{
				UINT uSize = sizeof(ObjID_t) + sizeof(PET_GUID_t) + sizeof(INT);
				return uSize;
		}

		
	public:
		VOID			SetObjID(const ObjID_t objID) { m_ObjID = objID; }
		ObjID_t			GetObjID(VOID) const { return m_ObjID; }

		VOID			SetGUID(PET_GUID_t guid){ m_PetGUID = guid; }
		PET_GUID_t		GetGUID(VOID)const{ return m_PetGUID; }

		// nType	:	ENUM_MANIPULATE_TYPE
		VOID			SetManipulateType(INT nType){ m_nType = nType; }
		INT				GetManipulateType(VOID)const{ return m_nType; }

	private:
		ObjID_t			m_ObjID;	// 宠物的ObjID;
		PET_GUID_t		m_PetGUID;
		INT				m_nType;
	};


	class CGManipulatePetFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGManipulatePet() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_MANIPULATEPET; }
		UINT		GetPacketMaxSize()const 
		{

			UINT uSize = sizeof(CGManipulatePet) - sizeof(Packet);
			return uSize;
		}
	};

	class CGManipulatePetHandler 
	{
	public:
		static UINT Execute( CGManipulatePet* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif // __GCPLAYERDIE_H__
