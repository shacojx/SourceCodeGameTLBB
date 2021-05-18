
#pragma once

#include "Obj_Character.h"

enum ENUM_NPC_TYPE
{
	NPC_TYPE_INVALID	= -1,
	NPC_TYPE_MONSTER,
	NPC_TYPE_NPC,
	NPC_TYPE_PET,

	NPC_TYPE_NUMBERS
};
/**
	NPC角色类，会继承出 PlayerOther
*/
struct _DBC_CREATURE_ATT;
class CObject_PlayerNPC : public CObject_Character
{
public:
	//返回角色类型
	virtual CHARACTER_TYPE		GetCharacterType( VOID ) const { return CT_MONSTER;	}
	ENUM_NPC_TYPE				GetNpcType(VOID)const;
	VOID						SetNpcType(ENUM_NPC_TYPE eType);
	const _DBC_CREATURE_ATT*	GetCreatureAtt(void) const { return m_pCreatureInfo; }

	// 服务器GUID
	virtual VOID				SetServerGUID(GUID_t guid) { m_GUIDServer = guid; }
	virtual GUID_t				GetServerGUID(VOID )const { return m_GUIDServer; }

	// 左键指令的分析
	virtual BOOL				FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );
	// 右键指令的分析
	virtual BOOL				FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );

protected:
	// RenderInterface创建/删除
	virtual VOID				CreateRenderInterface(VOID);

	virtual INT					CalcCharModelID( VOID )const;

	virtual UINT				GetIdleInterval(VOID)const;

	virtual VOID				UpdateCharRace(VOID);

protected:
	virtual VOID				OnDataChanged_RaceID(VOID);

protected:
	//在数据表中的逻辑数据结构
	const _DBC_CREATURE_ATT*	m_pCreatureInfo;

	// GUID 服务器传回
	GUID_t						m_GUIDServer; 

	ENUM_NPC_TYPE				m_eNpcType;

public:
	virtual INT					CannotBeAttack( VOID )const;

	/// 与移动NPC对话停下来后需要设置NPC的转向问题 frankwu
public:
	VOID						SetNeedFaceToPlayer(BOOL bValue) { m_bNeedFaceToPlayer = bValue; }
	BOOL						GetNeedFaceToPlayer(VOID) const { return m_bNeedFaceToPlayer; }
	/// 是否会转向玩家
	BOOL						IsCanFaceToPlayer(VOID);
	/// 是否会转向玩家
	BOOL						IsDisplayBoard(VOID);
	// 头顶信息板高度
	FLOAT						GetBoardHeight(VOID);
	// 选中环大小
	virtual FLOAT				GetProjtexRange(VOID);
	// 阴影大小
	virtual FLOAT				GetShadowRange(VOID);

	LPCSTR						GetPortrait(VOID);

protected:
	VOID						FaceToPlayer(VOID);
	VOID						StopFaceToPlayer(VOID);

	VOID						ProcessFaceToPlayer(VOID); // 处理NPC转向

private:
	BOOL						m_bNeedFaceToPlayer;
	BOOL						m_bFaceToPlayer;
	UINT						m_uStartFaceToPlayerTime;
	///该Obj旋转之前的角度,初始化为<0.0f, 0.0f, 0.0f>
	fVector3					m_fvPreRotation;

protected:
	virtual VOID				CharacterLogic_OnStopped(BOOL bFinished);
	virtual BOOL				Tick_Idle(UINT uElapseTime);
	const fVector3&				GetPreRotation(VOID)const				{ return m_fvPreRotation;			}

	//--------------------------------------------------------
	//核心继承
	//--------------------------------------------------------
public:
	CObject_PlayerNPC();
	virtual ~CObject_PlayerNPC();

	///根据初始化物体，并同步到渲染层
	virtual	VOID			Initial(VOID*);

public:
	WX_DECLARE_DYNAMIC(CObject_PlayerNPC);
	//TD_DECLARE_LOGICAL(true);
};
