/**
物理层obj
负责重力，碰撞的处理tick
*/
#pragma once

#include "Object.h"

//物理事件
#define	MAX_REGISTER_EVENTS_NUM		(20)
enum PHY_EVENT_ID
{
	PE_NONE	 = -1,									//空事件
	PE_COLLISION_WITH_GROUND,						//物体落地或与地面发生接触
};

class CObject_Phy : public CObject
{
public:
	CObject_Phy();
	virtual ~CObject_Phy();

public:
	//-----------------------------------------------------
	///根据初始化物理物体
	virtual	VOID	Initial(VOID*);

	//-----------------------------------------------------
	//根据地形设置位置, fvPosition中分别是x和z
	virtual VOID	SetMapPosition(const fVector2& fvPosition);

	//-----------------------------------------------------
	//注册一个物理事件,被注册的物理事件,会在发生时通知实现类
	virtual VOID	RegisterPhyEvent(PHY_EVENT_ID eventid);

	//-----------------------------------------------------
	//注销一个物理事件,此物理事件的发生将不会被通知给实现类
	virtual VOID	UnRegisterPhyEvent(PHY_EVENT_ID eventid);

	//-----------------------------------------------------
	//通知一个物理事件发生
	virtual VOID	NotifyPhyEvent(PHY_EVENT_ID eventid, VOID* pParam = NULL);

	//-----------------------------------------------------
	///给一个初始速度
	VOID			AddLinearSpeed(fVector3 vSpeed);

	//-----------------------------------------------------
	///此物体是否才与物理计算
	BOOL			IsPhyEnable() {return m_bIsEnable;};

	//-----------------------------------------------------
	///使此物体进入物理计算
	VOID			PhyEnable(BOOL bFlag);

private:
	//-----------------------------------------------------
	///分发物理事件
	VOID			DispatchPhyEvent(PHY_EVENT_ID eventid, VOID* pParam = NULL);
private:
	//-----------------------------------------------------
	//该物体的线性速度
	fVector3		m_fvLinearSpeed;

	//-----------------------------------------------------
	//该物体的旋转速度
	fVector3		m_fvRotSpeed;

	//-----------------------------------------------------
	//此物体是否参与物理计算
	BOOL			m_bIsEnable;

	//-----------------------------------------------------
	//上次记录的桢时间
	UINT			m_nLastTickTime;

	//-----------------------------------------------------
	//此物体关心的物理事件列表
	PHY_EVENT_ID	m_aEventList[MAX_REGISTER_EVENTS_NUM];

	//-----------------------------------------------------
	//此物体关心的物理事件数量
	UINT			m_nEventListNum;

	//-----------------------------------------------------
	//此物体是否在空中
	BOOL			m_bIsInAir;

protected:
	//-----------------------------------------------------
	//一次物理Tick
	virtual VOID	Tick(VOID);
	//
	WX_DECLARE_DYNAMIC(CObject_Phy);
	//TD_DECLARE_LOGICAL(TRUE);
};
