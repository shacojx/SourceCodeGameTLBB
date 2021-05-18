/****************************************\
*										*
*			  WXObject					*
*										*
\****************************************/

#pragma once
#include "RFObject.h"

#include "Core/WXObject.h"

class CWXObj_Object : public CWXObject
{
public:
	//从渲染层销毁节点
	virtual VOID	Destroy(VOID);

	//设置位置 (游戏世界坐标)
	virtual VOID	SetPosition(const fVector3& vPos);
	//设置旋转角度
	virtual VOID	SetOrientation(const fVector3& vRotate);

	virtual VOID	SetLeftWeaponEffect(LPCTSTR szEffectName, UINT color) {};
	virtual VOID	SetRightWeaponEffect(LPCTSTR szEffectName, UINT color) {};

	virtual VOID	SetRightWeaponMat(LPCTSTR szWeaponMat, LPCTSTR szMatName) {};
	virtual VOID	SetLeftWeaponMat(LPCTSTR szWeaponMat, LPCTSTR szMatName) {};

	//设置是否能被鼠标选中
	virtual VOID	SetRayQuery(BOOL bQuery) { m_bRayQuery = bQuery; }
	virtual	BOOL	GetRayQuery(VOID) const	{ return m_bRayQuery; }

	//直接设置属性
	virtual VOID	SetWXObjectProperty(const char* szPropertyName, const char* szPropertyValue);

public:
	CWXObj_Object(LPCTSTR szType);
	virtual ~CWXObj_Object();

protected:
	//引入WXObject
	WX::ObjectPtr					m_pWXObject;
	//是否能被鼠标选中
	BOOL							m_bRayQuery;
};