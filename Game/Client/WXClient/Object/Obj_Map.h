
#pragma once

#include "Obj_Phy.h"
/**
	属于地表层的物体，这类物体受场景管理，需要注册到某个地表网格上
	当玩家远离该网格时会被销毁
*/
class CZone;
class CObject_Map : public CObject_Phy
{
public:
	//设置位置
	virtual VOID		SetPosition(const fVector3& fvPosition);
	const iVector2&		GetMapPosition(VOID) const { return m_ivGridPos; }

public:
	CObject_Map();
	virtual ~CObject_Map();

protected:
	///该物体所在的网格位置
	iVector2		m_ivGridPos;
	/// 该物体所在的Zone
	CZone*			m_pZone;

protected:
	WX_DECLARE_DYNAMIC(CObject_Map);

	friend class CZone;
};
