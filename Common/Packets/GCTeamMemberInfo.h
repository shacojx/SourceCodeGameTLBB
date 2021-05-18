// GCTeamMemberInfo.h

#ifndef __GCTEAMMEMBERINFO_H__
#define __GCTEAMMEMBERINFO_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCTeamMemberInfo : public Packet
	{
	public:
		GCTeamMemberInfo( )		{ m_Flags = 0;}
		virtual ~GCTeamMemberInfo( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream )const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_TEAMMEMBERINFO; }
		virtual UINT			GetPacketSize() const
		{
			UINT uSize = sizeof(GUID_t) + sizeof(m_Flags);

			for(INT i=0; i<TEAM_MEMBER_ATT_NUMBERS; ++i)
			{
				if( IsUpdateAttrib((ENUM_TEAM_MEMBER_ATT)i) )
				{
					switch( i )
					{
						break;
					case TEAM_MEMBER_ATT_FAMILY:
					case TEAM_MEMBER_ATT_LEVEL:
					case TEAM_MEMBER_ATT_MAX_HP:
					case TEAM_MEMBER_ATT_MAX_MP:
					case TEAM_MEMBER_ATT_WEAPON:
					case TEAM_MEMBER_ATT_CAP:
					case TEAM_MEMBER_ATT_ARMOR:
					case TEAM_MEMBER_ATT_CUFF:
					case TEAM_MEMBER_ATT_BOOT:
					case TEAM_MEMBER_ATT_FACEMESH:		// 面部模型
					case TEAM_MEMBER_ATT_HAIRMESH:		// 头发模型
					case TEAM_MEMBER_ATT_HAIRCOLOR:		// 头发颜色
						uSize += sizeof(UINT);
						break;
					case TEAM_MEMBER_ATT_POSITION:
						uSize += sizeof(WORLD_POS);
						break;
					case TEAM_MEMBER_ATT_HP:
					case TEAM_MEMBER_ATT_MP:
					case TEAM_MEMBER_ATT_ANGER:
						uSize += sizeof(INT);
						break;
					case TEAM_MEMBER_ATT_DEADLINK:
					case TEAM_MEMBER_ATT_DEAD:
						uSize += sizeof(UCHAR);
						break;
					default:
						// size 不变，例如两个标记量
						break;
					}
				}
			}

			return uSize;
		}

	public:
		//使用数据接口

		VOID					setGUID( GUID_t guid ) { m_GUID = guid; }
		GUID_t					getGUID( ) { return m_GUID; }

		INT						GetFlags( ) const { return m_Flags; }

		VOID					SetUpdateAttrib( ENUM_TEAM_MEMBER_ATT eAttrib, BOOL bUpdate )
		{
			if ( bUpdate )					{ m_Flags |= (1<<(INT)eAttrib); }
			else							{ m_Flags &= (~(1<<(INT)eAttrib)); }
		}
		BOOL					IsUpdateAttrib( ENUM_TEAM_MEMBER_ATT eAttrib ) const
		{
			if( m_Flags & (1<<(INT)eAttrib) )	{ return TRUE; }

			return FALSE;
		}

		VOID					SetFamily( UINT family )
		{
			m_uFamily = family;
			SetUpdateAttrib( TEAM_MEMBER_ATT_FAMILY, TRUE );
		}
		UINT					GetFamily( ) { return m_uFamily; }

		VOID					SetLevel( UINT uLevel )
		{
			m_uLevel =uLevel;
			SetUpdateAttrib( TEAM_MEMBER_ATT_LEVEL, TRUE );
		}
		UINT					GetLevel( VOID ) const { return m_uLevel; }

		VOID					SetWorldPos( const WORLD_POS* pWP)
		{
			m_WorldPos.m_fX = pWP->m_fX;
			m_WorldPos.m_fZ = pWP->m_fZ;
			SetUpdateAttrib( TEAM_MEMBER_ATT_POSITION, TRUE );
		}
		const WORLD_POS*		GetWorldPos( ) { return &m_WorldPos; }

		VOID					SetHP(INT nHp)
		{
			m_nHP = nHp;
			SetUpdateAttrib( TEAM_MEMBER_ATT_HP, TRUE );
		}
		INT						GetHP( ) const { return m_nHP; }

		VOID					SetMaxHP(UINT uMaxHp)
		{
			m_uMaxHP =uMaxHp;
			SetUpdateAttrib( TEAM_MEMBER_ATT_MAX_HP, TRUE );
		}
		UINT					GetMaxHP( ) const { return m_uMaxHP; }

		VOID					SetMP(INT nMp)
		{
			m_nMP = nMp;
			SetUpdateAttrib( TEAM_MEMBER_ATT_MP, TRUE );
		}
		INT						GetMP( ) const { return m_nMP; }

		VOID					SetMaxMP(UINT uMaxMp)
		{
			m_uMaxMP =uMaxMp;
			SetUpdateAttrib( TEAM_MEMBER_ATT_MAX_MP, TRUE );
		}
		UINT					GetMaxMP( ) const { return m_uMaxMP; }

		VOID					SetAnger(INT anger)
		{
			m_nAnger = anger;
			SetUpdateAttrib( TEAM_MEMBER_ATT_ANGER, TRUE );
		}
		INT						GetAnger( ) { return m_nAnger; }

		VOID					SetWeaponID(UINT weaponid)
		{
			m_WeaponID = weaponid;
			SetUpdateAttrib( TEAM_MEMBER_ATT_WEAPON, TRUE );
		}
		UINT					GetWeaponID() { return m_WeaponID; }

		VOID					SetCapID(UINT capid)
		{
			m_CapID = capid;
			SetUpdateAttrib( TEAM_MEMBER_ATT_CAP, TRUE );
		}
		UINT					GetCapID() { return m_CapID; }

		VOID					SetArmourID(UINT armorid)
		{
			m_ArmourID = armorid;
			SetUpdateAttrib( TEAM_MEMBER_ATT_ARMOR, TRUE );
		}
		UINT					GetArmourID() { return m_ArmourID; }

		VOID					SetCuffID(UINT cuffid)
		{
			m_CuffID = cuffid;
			SetUpdateAttrib( TEAM_MEMBER_ATT_CUFF, TRUE );
		}
		UINT					GetCuffID() { return m_CuffID; }

		VOID					SetFootID(UINT footid)
		{
			m_FootID = footid;
			SetUpdateAttrib( TEAM_MEMBER_ATT_BOOT, TRUE );
		}
		UINT					GetFootID() { return m_FootID; }

		VOID					SetDeadLink( BOOL flag )
		{
			m_DeadLinkFlag = flag;
			SetUpdateAttrib( TEAM_MEMBER_ATT_DEADLINK, TRUE );
		}
		BOOL					IsDeadLink( )
		{
			return (m_DeadLinkFlag>0);
		}

		VOID					SetDead( BOOL flag )
		{
			m_DeadFlag = flag;
			SetUpdateAttrib( TEAM_MEMBER_ATT_DEAD, TRUE );
		}
		BOOL					IsDead( )
		{
			return (m_DeadFlag>0);
		}

		VOID					SetFaceModel(UINT uFaceMeshID)
		{
			m_uFaceMeshID = uFaceMeshID;
			SetUpdateAttrib( TEAM_MEMBER_ATT_FACEMESH, TRUE );
		}
		UINT					GetFaceModel() const
		{
			return m_uFaceMeshID;
		}

		VOID					SetHairModel(UINT uHairMeshID)
		{
			m_uHairMeshID = uHairMeshID;
			SetUpdateAttrib( TEAM_MEMBER_ATT_HAIRMESH, TRUE );
		}
		UINT					GetHairModel() const
		{
			return m_uHairMeshID;
		}

		VOID					SetHairColor(UINT uHairColor)
		{
			m_uHairColor = uHairColor;
			SetUpdateAttrib( TEAM_MEMBER_ATT_HAIRCOLOR, TRUE );
		}
		UINT					GetHairColor() const
		{
			return m_uHairColor;
		}

	private:
		//数据
		GUID_t					m_GUID;

		INT						m_Flags;	// 每个位表示以下值是否要刷新 ENUM_DETAIL_ATTRIB
		UINT					m_uFamily;						// 1.门派
		UINT					m_uLevel;						// 2.等级
		WORLD_POS				m_WorldPos;						// 3.位置（坐标）
		INT						m_nHP;							// 4.HP
		UINT					m_uMaxHP;						// 5.HP上限
		INT						m_nMP;							// 6.MP
		UINT					m_uMaxMP;						// 7.MP上限
		INT						m_nAnger;						// 8.怒气
		UINT					m_WeaponID;						// 9.武器
		UINT					m_CapID;						// 10.帽子
		UINT					m_ArmourID;						// 11.衣服
		UINT					m_CuffID;						// 12.护腕
		UINT					m_FootID;						// 13.靴子
		// 14.buff，暂时不考虑
		UCHAR					m_DeadLinkFlag;					// 15.断线
		UCHAR					m_DeadFlag;						// 16.死亡
		UINT					m_uFaceMeshID;					// 17.面部模型
		UINT					m_uHairMeshID;					// 18.头发模型
		UINT					m_uHairColor;					// 19.头发颜色
	};


	class GCTeamMemberInfoFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCTeamMemberInfo(); }
		PacketID_t	GetPacketID()const { return PACKET_GC_TEAMMEMBERINFO; }
		UINT		GetPacketMaxSize()const { return	sizeof(GUID_t) +
														sizeof(INT) +
														sizeof(UINT) * 12 +
														sizeof(WORLD_POS) +
														sizeof(UCHAR) * 2+
														sizeof(INT) * 3; }
	};

	class GCTeamMemberInfoHandler 
	{
	public:
		static UINT Execute( GCTeamMemberInfo* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif // __GCTEAMMEMBERINFO_H__
