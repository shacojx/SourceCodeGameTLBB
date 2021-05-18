// AI_Base.h
//
//////////////////////////////////////////////////////

#pragma once

#include "..\..\..\GameCommand.h"

class CObject_Character;
class CAI_Base
{
public:
	//逻辑桢
	virtual BOOL		Tick( VOID )							= 0;

	// 压入一条指令
	virtual BOOL		PushCommand( const SCommand_AI *pCmd )	= 0;
	virtual BOOL		PushCommand_MoveTo(FLOAT fDestX, FLOAT fDestZ)	= 0;
	virtual BOOL		PushCommand_AutoHit(INT isAutoHit) = 0;
	virtual BOOL		PushCommand_Jump(VOID)	= 0;
	virtual BOOL		PushCommand_UseSkill(INT idSkill, GUID_t guidTarget, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir)	= 0;

	// 中断当前 AI
	virtual VOID		OnAIStopped( VOID )						= 0;

	// 重置
	virtual VOID		Reset(VOID)								= 0;

protected:
	//AI指令
	virtual RC_RESULT	OnCommand( const SCommand_AI *pCmd )	= 0;

public:
	CAI_Base(CObject_Character* pCharObj);
	virtual ~CAI_Base(VOID);

	CObject_Character*	GetCharacter(VOID) { return m_pCharacterObj; }

protected:
	CObject_Character*	m_pCharacterObj;
};
