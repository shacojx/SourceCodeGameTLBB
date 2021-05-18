// Object.h
// 
// 纯逻辑类，与图像无关
// 
////////////////////////////////////////////////////

#pragma once
#include "..\GameCommand.h"
#include "TDObjectSystem.h"

/**
	游戏概念中的物体基类
*/
struct SObjectInit
{
	fVector3	m_fvPos;
	fVector3	m_fvRot;

	SObjectInit( VOID ){
		m_fvPos.x			= -1.f;
		m_fvPos.y			= -1.f;
		m_fvPos.z			= -1.f;

		m_fvRot.x			= -1.f;
		m_fvRot.y			= -1.f;
		m_fvRot.z			= -1.f;
	}

	virtual VOID Reset( VOID ){
		m_fvPos.x			= -1.f;
		m_fvPos.y			= -1.f;
		m_fvPos.z			= -1.f;

		m_fvRot.x			= -1.f;
		m_fvRot.y			= -1.f;
		m_fvRot.z			= -1.f;
	}
};

struct SCommand_Object;
struct SCommand_Mouse;
struct _CAMP_DATA;
class tActionItem;
//========================================================
class CObject : public tObject
{
public:
	//-----------------------------------------------------
	///根据初始化物体，并同步到渲染层
	virtual	VOID				Initial(VOID*);
	///销毁该物体,应该是最后一个被调用的函数
	virtual VOID				Destroy(VOID);

public:
	// 压入一条指令
	virtual BOOL				PushCommand(const SCommand_Object *pCmd );

protected:
	// 指令接收, 外界控制角色的唯一接口
	virtual RC_RESULT			OnCommand(const SCommand_Object *pCmd ){ return RC_SKIP; }

public:
	///得到物体的ID
	virtual INT					GetID(VOID) { return m_ID; }

	//-----------------------------------------------------
	///设置某项基本状态为Enable
	virtual VOID				Enable(UINT dwFlag);
	///设置某项基本状态为Disable
	virtual VOID				Disalbe(UINT dwFlag);
	///察看某项状态是否为Enable
	virtual	BOOL				IsEnable(UINT dwFlag)const{ return (m_dwStatusFlag&dwFlag) ? true : FALSE; }
	///察看某项状态是否为Disable
	virtual	BOOL				IsDisable(UINT dwFlag)const{ return (m_dwStatusFlag&dwFlag) ? FALSE : true; }


	//-----------------------------------------------------
	//特殊物体
public:
	//是否能够被作为主目标选择
	virtual BOOL				CanbeSelect(VOID) const { return FALSE; }
	//是否是特殊物体(player, target, pet, party1, ...);
	virtual BOOL				IsSpecialObject(STRING& strReturn);

	// 左键指令的分析
	virtual BOOL				FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );
	// 右键指令的分析
	virtual BOOL				FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );

	//-----------------------------------------------------
	//TripperObj
	//物体类型
	virtual TRIPPER_OBJECT_TYPE		Tripper_GetType(VOID) const	{ return TOT_NONE; }
	//能否鼠标操作
	virtual BOOL					Tripper_CanOperation(VOID) const { return FALSE; }
	//鼠标类型
	virtual ENUM_CURSOR_TYPE		Tripper_GetCursor(VOID) const { return CURSOR_NORMAL; }
	//进入激活
	virtual VOID					Tripper_Active(VOID) {}

public:
	//----------------------------------------------------
	//Object 逻辑相关
	//检查是否不再存活
	virtual VOID				Tick_CheckAlive(VOID);

public:

	//--------------------------------------------------------
	//服务器ID
	virtual VOID				SetServerID(INT nID)	{ m_idServer = nID; }
	virtual INT					GetServerID(VOID)const	{ return m_idServer; }

	//--------------------------------------------------------
	//最近一次接受服务器消息时间
	VOID						SetMsgTime(UINT dwTime);
	UINT						GetMsgTime(VOID)const { return m_timeMsg; }

	//--------------------------------------------------------
	//服务器端的当前OBJ逻缉号
	INT							GetLogicCount( VOID )const{ return m_nLogicCount; }
	VOID						SetLogicCount( INT nLogicCount ){ m_nLogicCount = nLogicCount; }
	virtual BOOL				IsLogicReady( INT nLogicCount )const{ return (m_nLogicCount >= nLogicCount)?(TRUE):(FALSE); }
	virtual VOID				ResetLogicCount(VOID){ m_nLogicCount = -1; }

	//--------------------------------------------------------
	//位置
	virtual VOID				SetPosition(const fVector3& fvPosition);
	const fVector3&				GetPosition(VOID)const					{ return m_fvPosition;			}
	//根据地形设置位置, fvPosition中分别是x和z
	virtual VOID				SetMapPosition(const fVector2& fvPosition);

	//--------------------------------------------------------
	//旋转角度 (弧度)
	virtual VOID				SetRotation(const fVector3& fvRotation);
	const fVector3&				GetRotation(VOID)const					{ return m_fvRotation;			}

	//--------------------------------------------------------
	//地表位置
	virtual VOID				SetFootPosition(const fVector3& fvPosition);
	const fVector3&				GetFootPosition(VOID)const				{ return m_fvFootPosition;		}

	//--------------------------------------------------------
	//面朝的方向，即y轴旋转角度 (弧度)
	virtual VOID				SetFaceDir(FLOAT fDir);
	virtual FLOAT				GetFaceDir(VOID)const					{ return m_fvRotation.y; }

	//--------------------------------------------------------
	//渲染层指针
	tEntityNode*				GetRenderInterface(VOID) { return m_pRenderInterface; }

	//--------------------------------------------------------
	//是否属于UI渲染的对象
	BOOL						GetFakeObjectFlag(VOID) const  { return m_bFakeObject; }
	VOID						SetFakeObjectFlag(BOOL bVisibleFlag) { m_bFakeObject = bVisibleFlag; }

	//--------------------------------------------------------
	//阵营ID
	virtual const _CAMP_DATA	*GetCampData(VOID)const{ return NULL; }
	virtual ObjID_t				GetOwnerID(VOID)const{ return INVALID_ID; }

public:
	CObject();
	virtual ~CObject();

protected:
	//--------------------------------------------------------
	//在渲染层中的接口
	tEntityNode*		m_pRenderInterface;


protected:
	//--------------------------------------------------------
	//该物体在服务器上的ID，由服务器传回，
	//对于完全不受服务器控制的静态物体，该值为INVALID_ID
	INT				m_idServer;

	//最近一次接受服务器消息时间,对于从不接受消息的本地静态物体,该值为创建时间
	UINT			m_timeMsg;

	// 服务器端的逻辑计数
	INT				m_nLogicCount;

	//--------------------------------------------------------
	//基本状态标志
	/*
    |    xxxxxxxx xxxxxxxx xxxxxxxx xxxxx000
    |                                    |||_____ 1:可见  0:不可见
    |                                    ||______ 1:渲染名字 0:不渲染名字
    |                                    |_______ 1:已经不在玩家的视野范围,如果该值维持一定时间,则会被删除 0:仍在玩家视野范围
    |           
	*/
	UINT			m_dwStatusFlag;
	
	//--------------------------------------------------------
	//如果有该标志，表明该物体只是用于UI渲染，受FakeObjectManager管理
	BOOL			m_bFakeObject;

	//--------------------------------------------------------
	///该物体在客户端的唯一ID,在Object生成时动态生成
	INT				m_ID;
	///该物体在场景中的浮点位置,初始化为<0.0f, 0.0f, 0.0f>
	fVector3		m_fvPosition;
	///该物体在场景中的旋转角度,初始化为<0.0f, 0.0f, 0.0f>
	fVector3		m_fvRotation;
	///此物体的地表位置，用于摄像机的瞄准
	fVector3		m_fvFootPosition;


	//--------------------------------------------------------
	WX_DECLARE_DYNAMIC(CObject);
	//TD_DECLARE_LOGICAL(FALSE);
	friend class CObjectManager;

public:
	//---- for Debug
	VOID PushDebugString(LPCTSTR szDebugString);
	std::deque< STRING > m_qDebugString;

	virtual STRING GetDebugDesc(VOID) { return STRING(""); }
	//---- for Debug
};
