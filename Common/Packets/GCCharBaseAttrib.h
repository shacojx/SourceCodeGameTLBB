// GCCharBaseAttrib.h
// 
/////////////////////////////////////////////////////////

#ifndef __GCCHARBASEATTRIB_H__
#define __GCCHARBASEATTRIB_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCCharBaseAttrib : public Packet
	{
	public:
		enum ECHAR_AI_TYPE
		{
			CHAR_AI_TYPE_INVALID		= -1,	// INVALID
			CHAR_AI_TYPE_SCANNPC		= 0,	// 主动NPC
			CHAR_AI_TYPE_NOTSCANNPC		= 1,	// 非主动NPC
			CHAR_AI_TYPE_CANNOTATTACK	= 2,	// 不可攻击NPC
		};

		GCCharBaseAttrib( )
		{
			Reset();
		}
		virtual ~GCCharBaseAttrib( ){}

		VOID Reset( VOID )
		{
			m_ObjID			= UINT_MAX;

			m_uFlags		= 0;
			m_wDataID		= 0xFFFF;
			m_HPPercent		= 0;
			m_MPPercent		= 0;
			m_nRage			= 0;
			m_fMoveSpeed	= 0.f;
			m_fAttackSpeed	= 1.f;
			m_CampData.CleanUp();
			//m_nObjectCampType = INVALID_CAMP;
			m_nModelID		= -1;
			m_nPortraitID	= INVALID_ID;
			m_nMountID		= INVALID_ID;
			m_nModelID		= INVALID_ID;
			m_nAIType		= -1;

			m_byNameSize	= 0;
			memset( m_szName, 0, sizeof( m_szName ) );
			m_byTitleSize	= 0;
			memset( m_szTitle, 0, sizeof( m_szTitle ) );
			m_uPlayerData	= 0;
			m_TitleType		= 0;

			m_bStallIsOpen = 0;
			m_nStallNameSize = 0;
			memset( m_szStallName, 0, MAX_STALL_NAME);

			m_OccupantGUID = INVALID_ID ;
			m_OwnerID = INVALID_ID ;
			m_IsNPC = 0;
			//m_nObjectCampType = INVALID_CAMP;
		}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHARBASEATTRIB; }
		virtual UINT			GetPacketSize()const 
		{
			UINT uAttribSize = sizeof(ObjID_t) + sizeof(UINT);

			if ( m_uFlags & (1 << UPDATE_CHAR_ATT_DATA_ID) )
				uAttribSize += sizeof(m_wDataID);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_NAME) )
			{
				uAttribSize += sizeof(m_byNameSize);
				uAttribSize += sizeof(CHAR) * m_byNameSize;
			}

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_TITLE) )
			{
				uAttribSize += sizeof(m_TitleType);
				uAttribSize += sizeof(m_byTitleSize);
				uAttribSize += sizeof(CHAR) * m_byTitleSize;
			}

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_LEVEL) )
				uAttribSize += sizeof(m_Level);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_HP_PERCENT) )
				uAttribSize += sizeof(m_HPPercent);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MP_PERCENT) )
				uAttribSize += sizeof(m_MPPercent);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_RAGE) )
				uAttribSize += sizeof(m_nRage);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_STEALTH_LEVEL) )
				uAttribSize += sizeof(m_nStealthLevel);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_SIT) )
				uAttribSize += sizeof(m_cMoodState);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MOVE_SPEED) )
				uAttribSize += sizeof(m_fMoveSpeed);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_ATTACK_SPEED) )
				uAttribSize += sizeof(m_fAttackSpeed);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_CAMP_ID) )
				uAttribSize += sizeof(m_CampData);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_PORTRAIT_ID) )
				uAttribSize += sizeof(m_nPortraitID);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MODEL_ID) )
				uAttribSize += sizeof(m_nModelID);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_MOUNT_ID) )
				uAttribSize += sizeof(m_nMountID);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_AITYPE) )
				uAttribSize += sizeof(m_nAIType);

			if ( m_uFlags & (1<<UPDATE_CHAR_ATT_PLAYER_DATA) )
			{
				uAttribSize += sizeof(m_uPlayerData);
				uAttribSize += sizeof(m_HairColor);
			}

			if ( m_uFlags & (1<<UPDATE_CHAR_IS_IN_STALL) )
				uAttribSize += sizeof(m_bStallIsOpen);

			if ( m_uFlags & (1<<UPDATE_CHAR_STALL_NAME) )
			{
				uAttribSize += sizeof(m_nStallNameSize) + sizeof(CHAR)*m_nStallNameSize;
			}

			if (m_uFlags & (1<<UPDATE_CHAR_OCCUPANT) )
			{
				uAttribSize += sizeof(GUID_t) ;
			}

			if (m_uFlags & (1<<UPDATE_CHAR_OWNER) )
			{
				uAttribSize += sizeof(ObjID_t) ;
			}

			if (m_uFlags & (1<<UPDATE_CHAR_ISNPC) )
			{
				uAttribSize += sizeof(BYTE) ;
			}

			return uAttribSize;
		}

	public:
		//使用数据接口
		VOID			setObjID(ObjID_t id) { m_ObjID = id; }
		ObjID_t			getObjID(VOID)const { return m_ObjID; }

		UINT			getFlags( VOID )const{ return m_uFlags; }

		BOOL			IsUpdateAttrib( ENUM_UPDATE_CHAR_ATT eAttrib )const{ return (m_uFlags & (1<<(INT)eAttrib))?(TRUE):(FALSE); }
		VOID			SetUpdateAttrib( ENUM_UPDATE_CHAR_ATT eAttrib, BOOL bUpdate )
		{
			if ( bUpdate )
				m_uFlags |= (1<<(INT)eAttrib);
			else
				m_uFlags &= (~(1<<(INT)eAttrib));
		}

		VOID			setDataID( WORD wID ){ m_wDataID = wID; SetUpdateAttrib(UPDATE_CHAR_ATT_DATA_ID,TRUE); }
		WORD			getDataID( VOID )const{ return m_wDataID; }

		VOID			setLevel( BYTE Level ){ m_Level = Level; SetUpdateAttrib(UPDATE_CHAR_ATT_LEVEL,TRUE); }
		BYTE			getLevel( VOID )const{ return m_Level; }

		VOID			setHPPercent( BYTE HPPercent ){ m_HPPercent = HPPercent; SetUpdateAttrib(UPDATE_CHAR_ATT_HP_PERCENT,TRUE); }
		BYTE			getHPPercent( VOID )const{ return m_HPPercent; }

		VOID			setMPPercent( BYTE MPPercent ){ m_MPPercent = MPPercent; SetUpdateAttrib(UPDATE_CHAR_ATT_MP_PERCENT,TRUE); }
		BYTE			getMPPercent( VOID )const{ return m_MPPercent; }

		VOID			setRage( INT nRage ){ m_nRage = nRage; SetUpdateAttrib(UPDATE_CHAR_ATT_RAGE,TRUE); }
		INT				getRage( VOID )const{ return m_nRage; }

		VOID			setStealthLevel( INT nStealthLevel ){ m_nStealthLevel = nStealthLevel; SetUpdateAttrib(UPDATE_CHAR_ATT_STEALTH_LEVEL,TRUE); }
		INT				getStealthLevel( VOID )const{ return m_nStealthLevel; }

		VOID			setMoodState(INT nState){ m_cMoodState = (char)nState; SetUpdateAttrib(UPDATE_CHAR_ATT_SIT,TRUE); }
		INT				getMoodState( VOID )const{ return (INT)(m_cMoodState); }

		VOID			setMoveSpeed( FLOAT fMoveSpeed ){ m_fMoveSpeed = fMoveSpeed; SetUpdateAttrib(UPDATE_CHAR_ATT_MOVE_SPEED,TRUE); }
		FLOAT			getMoveSpeed( VOID )const{ return m_fMoveSpeed; }

		VOID			setAttackSpeed( FLOAT fAttackSpeed ){ m_fAttackSpeed = fAttackSpeed; SetUpdateAttrib(UPDATE_CHAR_ATT_ATTACK_SPEED,TRUE); }
		FLOAT			getAttackSpeed( VOID )const{ return m_fAttackSpeed; }

		VOID					setCampData( const _CAMP_DATA *pCampData ){ m_CampData = *pCampData; SetUpdateAttrib(UPDATE_CHAR_ATT_CAMP_ID,TRUE); }
		const _CAMP_DATA		*getCampData( VOID )const{ return &m_CampData; }
		//VOID			setCampType( INT nObjectCampType ){ m_nObjectCampType = nObjectCampType; SetUpdateAttrib(UPDATE_CHAR_ATT_CAMP_ID,TRUE); }
		//const INT		*getCampType( VOID )const{ return &m_nObjectCampType; }

		VOID			setPortraitID( INT nPortraitID ){ m_nPortraitID =nPortraitID; SetUpdateAttrib(UPDATE_CHAR_ATT_PORTRAIT_ID,TRUE); }
		UINT			getPortraitID( VOID )const{ return m_nPortraitID;}

		VOID			setModelID( INT nModelID ){ m_nModelID =nModelID; SetUpdateAttrib(UPDATE_CHAR_ATT_MODEL_ID,TRUE); }
		UINT			getModelID( VOID )const{ return m_nModelID;}

		VOID			setMountID(INT nID) {m_nMountID = nID; SetUpdateAttrib(UPDATE_CHAR_ATT_MOUNT_ID, TRUE);}
		INT				getMountID(VOID) {return m_nMountID;}

		VOID			setAIType(INT nAIType) {m_nAIType = nAIType; SetUpdateAttrib(UPDATE_CHAR_ATT_AITYPE, TRUE);	}
		INT				getAIType(VOID) const { return m_nAIType; } 

		VOID			setName( const CHAR *pszName ){
			if ( !pszName )
				return ;
			m_byNameSize = (BYTE)(strlen( pszName ));
			strncpy( m_szName, pszName, MAX_CHARACTER_NAME-1 );
			SetUpdateAttrib(UPDATE_CHAR_ATT_NAME,TRUE);
		}

		const CHAR		*getName( VOID )const{ return m_szName; }

		VOID			setTitle( const CHAR *pszTitle ){
			if ( !pszTitle )
				return ;
			m_byTitleSize = (BYTE)(strlen( pszTitle ));
			strncpy( m_szTitle, pszTitle, MAX_CHARACTER_TITLE-1 );
			SetUpdateAttrib(UPDATE_CHAR_ATT_TITLE,TRUE);
		}

		const CHAR		*getTitle( VOID )const{ return m_szTitle; }

		VOID			setTitleType(BYTE nTitleType) {m_TitleType = nTitleType; SetUpdateAttrib(UPDATE_CHAR_ATT_TITLE, TRUE);	}
		BYTE			getTitleType(VOID) const { return m_TitleType; } 

		VOID			setStallName( const CHAR *pszName ){
			if ( !pszName )
				return ;
			m_nStallNameSize = (BYTE)(strlen( pszName ));
			strncpy( m_szStallName, pszName, MAX_STALL_NAME-1 );
			SetUpdateAttrib(UPDATE_CHAR_STALL_NAME,TRUE);
		}

		const CHAR		*getStallName( VOID )const{ return m_szStallName; }

		VOID			setFaceMeshID( UINT uMeshID ){
			m_uPlayerData |= ((uMeshID & 0XFF));
			SetUpdateAttrib(UPDATE_CHAR_ATT_PLAYER_DATA,TRUE);
		}
		UINT			getFaceMeshID( VOID )const
		{
			UINT uFaceMeshID = (m_uPlayerData & 0x000000FF);
			if(uFaceMeshID == (UINT)0xFF)
			{
				uFaceMeshID = 0xFFFFFFFF;
			}
			return uFaceMeshID;
		}

		VOID			setHairMeshID( UINT uMeshID ){
			m_uPlayerData |= ((uMeshID & 0XFF)<<8);
			SetUpdateAttrib(UPDATE_CHAR_ATT_PLAYER_DATA,TRUE);
		}
		UINT			getHairMeshID( VOID )const
		{
			UINT uHairMeshID = ((m_uPlayerData>>8) & 0x000000FF);
			if(uHairMeshID == (UINT)0xFF)
			{
				uHairMeshID = 0xFFFFFFFF;
			}
			return uHairMeshID;
		}

		VOID			setHairMatrixID( UINT uColor ){
			m_HairColor = uColor;
			SetUpdateAttrib(UPDATE_CHAR_ATT_PLAYER_DATA,TRUE);
		}
		UINT			getHairMatrixID( VOID )const{ return m_HairColor; }

		VOID			setIsOpen( BYTE StallIsOpen ){ m_bStallIsOpen = StallIsOpen; SetUpdateAttrib(UPDATE_CHAR_IS_IN_STALL,TRUE); }
		BYTE			getIsOpen( VOID )const{ return m_bStallIsOpen; }

		VOID			setOccupantGUID( GUID_t guid ){ m_OccupantGUID = guid ; SetUpdateAttrib(UPDATE_CHAR_OCCUPANT,TRUE); }
		GUID_t			getOccupantGUID( )const{ return m_OccupantGUID ; }

		VOID			setOwnerID( ObjID_t id ){ m_OwnerID = id ; SetUpdateAttrib(UPDATE_CHAR_OWNER,TRUE); }
		ObjID_t			getOwnerID( )const{ return m_OwnerID ; }

		VOID			setIsNPC( BOOL isNPC ){ m_IsNPC = isNPC ? 1 : 0; SetUpdateAttrib(UPDATE_CHAR_ISNPC,TRUE); }
		BOOL			getIsNPC( )const{ return (m_IsNPC==1) ? TRUE : FALSE; }

	private:
		ObjID_t			m_ObjID;		// ObjID

		// 每个位表示一个属性是否要刷新 ENUM_UPDATE_CHAR_ATT
		UINT			m_uFlags;		

		//数据部分	
		WORD			m_wDataID;			// 怪物的数据资源ID, 如果是玩家，则是性别
		BYTE			m_Level;			// 等级
		BYTE			m_HPPercent;		// 生命值百分比
		BYTE			m_MPPercent;		// 魔法值百分比
		INT				m_nRage;			// 怒气
		INT				m_nStealthLevel;	// 隐身级别
		CHAR			m_cMoodState;		// 表情状态
		FLOAT			m_fMoveSpeed;		// 移动的速度
		FLOAT			m_fAttackSpeed;		// 攻击速度
		_CAMP_DATA		m_CampData;			// 阵营
		//INT				m_nObjectCampType;	// 阵营类型
		INT				m_nPortraitID;		// 头像ID
		INT				m_nModelID;			// 变形
		INT				m_nMountID;			// 座骑
		INT				m_nAIType;			// AI类型
		// Player专有
		BYTE			m_byNameSize;					// 玩家姓名长度,不包括最后的'\0'
		CHAR			m_szName[MAX_CHARACTER_NAME];	// 玩家姓名
		BYTE			m_byTitleSize;					// 玩家头衔长度,不包括最后的'\0'
		CHAR			m_szTitle[MAX_CHARACTER_TITLE];	// 玩家头衔
		BYTE			m_TitleType;					// 称号类型

		UINT			m_uPlayerData;					//   FFFF|FF|FF
														//        |   |   
														//      头发 脸型
														//      模型 模型

		UINT			m_HairColor;					// 头发颜色

		//摊位状态
		BYTE			m_bStallIsOpen;					// 摊位是否已经打开
		BYTE			m_nStallNameSize;				// 摊位名长度
		CHAR			m_szStallName[MAX_STALL_NAME];	// 摊位名

		GUID_t			m_OccupantGUID ;
		ObjID_t			m_OwnerID ;
		BYTE			m_IsNPC;
	};


	class GCCharBaseAttribFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCCharBaseAttrib() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_CHARBASEATTRIB; }
		UINT		GetPacketMaxSize()const { return	sizeof(GCCharBaseAttrib) - sizeof(Packet); } //__
	};

	class GCCharBaseAttribHandler 
	{
	public:
		static UINT Execute( GCCharBaseAttrib* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;



#endif