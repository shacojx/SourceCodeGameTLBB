// Obj_PlayerMySelf.h
//
// 表示自己的角色类
//
//////////////////////////////////////////////////////

#pragma once

#include "Obj_PlayerOther.h"

/*
玩家自身数据
*/
class CObject_PlayerMySelf : public CObject_PlayerOther
{
public:
	//// 压入一条指令
	//virtual BOOL				PushCommand(const SCommand_Object *pCmd );

	// 左键指令的分析
	virtual BOOL				FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );
	// 右键指令的分析
	virtual BOOL				FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );

	virtual VOID				Tick_UpdateInfoBoard(VOID);

public:
	//返回角色类型
	virtual CHARACTER_TYPE	GetCharacterType( VOID ) const { return CT_PLAYERMYSELF;	}
	BOOL					IsInVisualFiled( CObject_Character* pObj );

	//------------------------------------------
	//逻辑数据发生改变
	//------------------------------------------
protected:
	friend class CCharacterData;
	virtual VOID			SetPosition(const fVector3& fvPosition);
	virtual VOID			OnDataChanged_RaceID(VOID);
	virtual VOID			OnDataChanged_HairMesh(VOID);
	virtual VOID			OnDataChanged_HairColor(VOID);
	virtual VOID			OnDataChanged_FaceMesh(VOID);
	virtual VOID			OnDataChanged_Equip(HUMAN_EQUIP point);
	virtual VOID			OnDataChanged_FaceImage(VOID);			// 头像改变事件函数, 2006-3-9
	virtual VOID			OnDataChanged_Die(VOID);
	virtual VOID			OnDataChanged_Level(VOID);
protected:
	//用于UI显示的逻辑对象
	CObject_PlayerOther*	m_pAvatar;
	//用于UI显示的逻辑对象
	CObject_PlayerOther*	m_pHeadAvatar;

	//------------------------------------------
	//
	//------------------------------------------
public:
	INT		GetSimulantLogicCount(VOID)const{return m_nSimulantLogicCount; }
	VOID	SetSimulantLogicCount(INT nLogicCount){ m_nSimulantLogicCount = nLogicCount; }

	virtual VOID	ResetLogicCount(VOID);

protected:
	INT		m_nSimulantLogicCount;

	//------------------------------------------
	//核心继承
	//------------------------------------------
public:
	CObject_PlayerMySelf( VOID );
	virtual ~CObject_PlayerMySelf( VOID );

	///根据初始化物体，并同步到渲染层
	virtual	VOID			Initial(VOID*);
	virtual VOID			Release(VOID);

protected:
	WX_DECLARE_DYNAMIC(CObject_PlayerMySelf);
	//TD_DECLARE_LOGICAL(true);
};
