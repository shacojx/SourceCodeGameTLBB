// ObjectCommand.h
//
//////////////////////////////////////////////////////

#pragma once

#include "Type.h"

enum ENUM_OBJECT_COMMAND
{
	OBJECT_COMMAND_INVALID	= -1,
	OBJECT_COMMAND_ACTION,				// 动作
	OBJECT_COMMAND_STOP_ACTION,			// 停止动作
	OBJECT_COMMAND_MOVE,				// 移动
	OBJECT_COMMAND_STOP_MOVE,			// 中止移动
	OBJECT_COMMAND_MAGIC_SEND,			// 法术发招
	OBJECT_COMMAND_MAGIC_CHARGE,		// 法术聚气
	OBJECT_COMMAND_MAGIC_CHANNEL,		// 法术引导
	OBJECT_COMMAND_ABILITY,				// 生活技能

	OBJECT_COMMAND_NUMBERS
};

/////////////////////////////////////////////
// CObjectCommand
/////////////////////////////////////////////
class CObjectCommand
{
public:
	CObjectCommand(VOID);
	virtual ~CObjectCommand(VOID);

	virtual VOID CleanUp(VOID)
	{
		m_nCommandID = OBJECT_COMMAND_INVALID;
	}

	INT GetCommandID(VOID)const
	{
		return m_nCommandID;
	}

protected:
	VOID SetCommandID(INT nCommandID)
	{
		m_nCommandID = nCommandID;
	}

private:
	INT		m_nCommandID;		// ENUM_OBJECT_COMMAND
};

/////////////////////////////////////////////
// CObjectCommand_Logic 逻辑指令
/////////////////////////////////////////////
class CObjectCommand_StopLogic;
class CObjectCommand_Logic :
	public CObjectCommand
{
public:
	CObjectCommand_Logic(VOID);
	virtual ~CObjectCommand_Logic(VOID);

	virtual VOID CleanUp(VOID);

	virtual BOOL Modify(const CObjectCommand_StopLogic *pCmd);

	UINT GetStartTime(VOID)const
	{
		return m_uStartTime;
	}

	INT GetLogicCount(VOID)const
	{
		return m_nLogicCount;
	}

protected:
	VOID SetStartTime(UINT uTime)
	{
		m_uStartTime = uTime;
	}

	VOID SetLogicCount(INT nLogicCount)
	{
		m_nLogicCount = nLogicCount;
	}

private:
	UINT		m_uStartTime;		// 起始时间
	INT			m_nLogicCount;		// 逻辑计数
};

/////////////////////////////////////////////
// CObjectCommand_StopLogic 中断逻辑指令
/////////////////////////////////////////////
class CObjectCommand_StopLogic :
	public CObjectCommand
{
public:
	CObjectCommand_StopLogic(VOID);
	virtual ~CObjectCommand_StopLogic(VOID);

	virtual VOID CleanUp(VOID);

	INT GetLogicCount(VOID)const
	{
		return m_nLogicCount;
	}

protected:
	VOID SetLogicCount(INT nLogicCount)
	{
		m_nLogicCount = nLogicCount;
	}

private:
	INT			m_nLogicCount;		// 逻辑计数
};

/////////////////////////////////////////////
// CObjectCommand_Action 动作
/////////////////////////////////////////////
class CObjectCommand_Action :
	public CObjectCommand_Logic
{
public:
	CObjectCommand_Action(VOID);
	virtual ~CObjectCommand_Action(VOID);

	BOOL Init(UINT uStartTime, INT nLogicCount, INT nActionID);
	virtual VOID CleanUp(VOID);

	INT GetActionID(VOID)const
	{
		return m_nActionID;
	}

private:
	INT			m_nActionID;		// 动作ID
};

/////////////////////////////////////////////
// CObjectCommand_StopAction 中止动作
/////////////////////////////////////////////
class CObjectCommand_StopAction :
	public CObjectCommand_StopLogic
{
public:
	CObjectCommand_StopAction(VOID);
	virtual ~CObjectCommand_StopAction(VOID);

	BOOL Init(INT nLogicCount, UINT uEndTime);
	virtual VOID CleanUp(VOID);

	UINT GetEndTime(VOID)const
	{
		return m_uEndTime;
	}

private:
	UINT		m_uEndTime;			// 结束时间
};

/////////////////////////////////////////////
// CObjectCommand_Move 移动
/////////////////////////////////////////////
class CObjectCommand_StopMove;
class CObjectCommand_Move :
	public CObjectCommand_Logic
{
public:
	CObjectCommand_Move(VOID);
	virtual ~CObjectCommand_Move(VOID);

	BOOL Init(UINT uStartTime, INT nLogicCount, INT nNodeCount, const WORLD_POS *paposNode);
	virtual VOID CleanUp(VOID);

	virtual BOOL Modify(const CObjectCommand_StopLogic *pCmd);

	INT GetNodeCount(VOID)const
	{
		return m_nNodeCount;
	}

	const WORLD_POS *GetNodeList(VOID)const
	{
		return m_paposNode;
	}

	BOOL ModifyTargetPos(const BYTE numPos, const WORLD_POS* targetPos);

private:
	INT			m_nNodeCount;		// 节点数目
	WORLD_POS	*m_paposNode;		// 节点数据
};

/////////////////////////////////////////////
// CObjectCommand_StopMove 中止移动
/////////////////////////////////////////////
class CObjectCommand_StopMove :
	public CObjectCommand_StopLogic
{
public:
	CObjectCommand_StopMove(VOID);
	virtual ~CObjectCommand_StopMove(VOID);

	BOOL Init(INT nLogicCount, INT nEndNodeIndex, const WORLD_POS *pPos);
	virtual VOID CleanUp(VOID);

	INT GetEndNodeIndex(VOID)const
	{
		return m_nEndNodeIndex;
	}

	const WORLD_POS *GetEndPos(VOID)const
	{
		return &m_posEndPos;
	}

private:
	INT			m_nEndNodeIndex;		// 中止的节点索引(如果总节点为1，那么中止节点只可能为0)
	WORLD_POS	m_posEndPos;			// 中止的位置(在中止的节点索引后面的出现的坐标)
};

/////////////////////////////////////////////
// CObjectCommand_MagicSend 法术发招
/////////////////////////////////////////////
class CObjectCommand_MagicSend :
	public CObjectCommand_Logic
{
public:
	CObjectCommand_MagicSend(VOID);
	virtual ~CObjectCommand_MagicSend(VOID);

	BOOL Init(UINT uStartTime,
		INT nLogicCount,
		INT nMagicID,
		ObjID_t nTargetID,
		const WORLD_POS *pTargetPos,
		FLOAT fTargetDir);

	virtual VOID CleanUp(VOID);

	INT GetMagicID(VOID)const
	{
		return m_nMagicID;
	}

	ObjID_t GetTargetObjID(VOID)const
	{
		return m_nTargetObjID;
	}

	const WORLD_POS *GetTargetPos(VOID)const
	{
		return &m_posTarget;
	}

	FLOAT GetTargetDir(VOID)const
	{
		return m_fTargetDir;
	}

private:
	INT			m_nMagicID;			// 法术ID
	ObjID_t		m_nTargetObjID;		// 目标角色
	WORLD_POS	m_posTarget;		// 目标位置
	FLOAT		m_fTargetDir;		// 目标方向
};

/////////////////////////////////////////////
// CObjectCommand_MagicCharge 法术聚气
/////////////////////////////////////////////
class CObjectCommand_StopMagicCharge;
class CObjectCommand_MagicCharge :
	public CObjectCommand_Logic
{
public:
	CObjectCommand_MagicCharge(VOID);
	virtual ~CObjectCommand_MagicCharge(VOID);

	BOOL Init(UINT uStartTime,
		INT nLogicCount,
		INT nMagicID,
		ObjID_t nTargetID,
		const WORLD_POS *pTargetPos,
		FLOAT fTargetDir,
		UINT uTotalTime);

	virtual VOID CleanUp(VOID);

	virtual BOOL Modify(const CObjectCommand_StopLogic *pCmd);

	INT GetMagicID(VOID)const
	{
		return m_nMagicID;
	}

	ObjID_t GetTargetObjID(VOID)const
	{
		return m_nTargetObjID;
	}

	const WORLD_POS *GetTargetPos(VOID)const
	{
		return &m_posTarget;
	}

	FLOAT GetTargetDir(VOID)const
	{
		return m_fTargetDir;
	}

	UINT GetTotalTime(VOID)const
	{
		return m_uTotalTime;
	}

	UINT GetEndTime(VOID)const
	{
		return m_uEndTime;
	}

private:
	INT			m_nMagicID;			// 法术ID
	ObjID_t		m_nTargetObjID;		// 目标角色
	WORLD_POS	m_posTarget;		// 目标位置
	FLOAT		m_fTargetDir;		// 目标方向
	UINT		m_uTotalTime;		// 总时间
	UINT		m_uEndTime;			// 结束时间
};

/////////////////////////////////////////////
// CObjectCommand_MagicChannel 法术引导
/////////////////////////////////////////////
class CObjectCommand_StopMagicChannel;
class CObjectCommand_MagicChannel :
	public CObjectCommand_Logic
{
public:
	CObjectCommand_MagicChannel(VOID);
	virtual ~CObjectCommand_MagicChannel(VOID);

	BOOL Init(UINT uStartTime,
		INT nLogicCount,
		INT nMagicID,
		ObjID_t nTargetID,
		const WORLD_POS *pTargetPos,
		FLOAT fTargetDir,
		UINT uTotalTime);

	virtual VOID CleanUp(VOID);

	virtual BOOL Modify(const CObjectCommand_StopLogic *pCmd);

	INT GetMagicID(VOID)const
	{
		return m_nMagicID;
	}

	ObjID_t GetTargetObjID(VOID)const
	{
		return m_nTargetObjID;
	}

	const WORLD_POS *GetTargetPos(VOID)const
	{
		return &m_posTarget;
	}

	FLOAT GetTargetDir(VOID)const
	{
		return m_fTargetDir;
	}

	UINT GetTotalTime(VOID)const
	{
		return m_uTotalTime;
	}

	UINT GetEndTime(VOID)const
	{
		return m_uEndTime;
	}

private:
	INT			m_nMagicID;			// 法术ID
	ObjID_t		m_nTargetObjID;		// 目标角色
	WORLD_POS	m_posTarget;		// 目标位置
	FLOAT		m_fTargetDir;		// 目标方向
	UINT		m_uTotalTime;		// 总时间
	UINT		m_uEndTime;			// 结束时间
};

/////////////////////////////////////////////
// CObjectCommand_Ability 生活技能
/////////////////////////////////////////////
class CObjectCommand_StopAbility;
class CObjectCommand_Ability :
	public CObjectCommand_Logic
{
public:
	CObjectCommand_Ability(VOID);
	virtual ~CObjectCommand_Ability(VOID);

	BOOL Init(UINT uStartTime, INT nLogicCount, INT nAbilityID, INT nPrescriptionID, ObjID_t nTargetObjID);
	virtual VOID CleanUp(VOID);

	virtual BOOL Modify(const CObjectCommand_StopLogic *pCmd);

	INT GetAbilityID(VOID)const
	{
		return m_nAbilityID;
	}

	INT GetPrescriptionID(VOID)const
	{
		return m_nPrescriptionID;
	}

	ObjID_t GetTargetObjID(VOID)const
	{
		return m_nTargetObjID;
	}

	UINT GetEndTime(VOID)const
	{
		return m_uEndTime;
	}

private:
	INT			m_nAbilityID;		// 生活技能ID
	INT			m_nPrescriptionID;	// 配方ID
	ObjID_t		m_nTargetObjID;		// 目标角色ID
	UINT		m_uEndTime;			// 结束时间
};


/////////////////////////////////////////////////////////////////////////////////////////
// 转换函数
/////////////////////////////////////////////////////////////////////////////////////////
struct SCommand_Object;
extern CObjectCommand *NewObjectCommand(const SCommand_Object *pCmd);
extern VOID DeleteObjectCommand(CObjectCommand *pCmd);
