// Obj_Special.h
//
// 作用：技能所产生的OBJ， 类似于原来的SKILLOBJ，
//		但由于服务器端以这个名字来命名，那么客户端也就这样吧
// 
///////////////////////////////////////////////////////////
#pragma once

#ifndef	__OBJ_SPECIAL_H__
#define __OBJ_SPECIAL_H__

#include <list>
#include "Obj_Effect.h"

struct SObject_SpecialInit : public SObject_EffectInit
{
	ObjID_t			m_idOwner;		// 所有者
	INT				m_nDataID;		// 资源ID

	SObject_SpecialInit( VOID ){
		m_idOwner			= INVALID_ID;
		m_nDataID			= INVALID_ID;
	}

	virtual VOID Reset( VOID ){
		SObject_EffectInit::Reset();
		m_idOwner			= INVALID_ID;
		m_nDataID			= INVALID_ID;
	}
};

struct _DBC_SPECIAL_OBJ_DATA;
class CObject_Special :	public CObject_Effect
{
public:
	enum ENUM_SPECIAL_STATE
	{
		SPECIAL_STATE_INVALID	= -1,
		SPECIAL_STATE_NORMAL,
		SPECIAL_STATE_DIE,
		SPECIAL_STATE_NUMBERS
	};
public:
	CObject_Special( VOID );
	virtual ~CObject_Special( VOID );

	//-----------------------------------------------------
	///根据初始化物体，并同步到渲染层
	virtual	VOID Initial( VOID* pInit );
	virtual VOID Release( VOID );

	virtual VOID Tick( VOID );

	// 左键指令的分析
	virtual BOOL FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );
	// 右键指令的分析
	virtual BOOL FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );

public:
	// 压入一条指令
	virtual BOOL PushCommand(const SCommand_Object *pCmd );

protected:
	virtual RC_RESULT OnCommand(const SCommand_Object *pCmd );

	VOID DoTrigger( INT nLogicCount, INT nTargetCount, ObjID_t *paTargetIDs );
	VOID DoDie( VOID );

public:
	virtual BOOL IsEffectStopped( VOID )const;

public:
	ENUM_SPECIAL_STATE GetSpecialState( VOID )const
	{
		return m_eSpecialState;
	}

	const _DBC_SPECIAL_OBJ_DATA *GetSpecialObjData( VOID )const
	{
		return m_pSpecialObjData;
	}

	virtual const _CAMP_DATA *GetCampData(VOID) const { return NULL; }
	virtual ObjID_t		GetOwnerID(VOID)const{ return m_idOwner; }

protected:
	VOID SetSpecialState( ENUM_SPECIAL_STATE eState )
	{
		m_eSpecialState = eState;
	}

	VOID ReleaseCommandList( VOID );
	SCommand_Object *PopCommand( VOID );
	const SCommand_Object *GetNextCommand( VOID )const;

private:
	ENUM_SPECIAL_STATE				m_eSpecialState;
	ObjID_t							m_idOwner;			// 所有者
	const _DBC_SPECIAL_OBJ_DATA		*m_pSpecialObjData;

	// 消息缓存
	typedef std::list< SCommand_Object* > CCommandList;
	CCommandList					m_listCommand;

protected:
	WX_DECLARE_DYNAMIC(CObject_Special);
};

#endif // __OBJ_SPECIAL_H__
