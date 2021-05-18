
#pragma once

#include "Obj_Static.h"


/**
	建筑物，根据自身等级显示不同模型，支持Model,Mesh,和Obj
*/

struct _DBC_CITY_BUILDING;
struct _DBC_BUILDING_DEFINE;

class CObject_Building : public CObject_Static
{
	/********************************************
		公有接口
	*********************************************/
public:
	//模型数据类型
	enum ResType
	{
		RT_Mesh,	//静态模型
		RT_Model,	//动态模型
		RT_Object,	//逻辑模型
	};

	/********************************************
		逻辑数据发生改变
	*********************************************/
	//资源定义ID发生改变
	void	SetLogicData_Define(const _DBC_CITY_BUILDING* pDefine);
	//等级发生改变
	void	SetLogicData_Level(INT nLevel);

	/********************************************
		内部接口
	*********************************************/
protected:
	//在渲染层创建渲染指针
	virtual VOID CreateRenderInterface(VOID);

public:
	CObject_Building();
	virtual ~CObject_Building();
	virtual VOID Release();

protected:
	/********************************************
		逻辑数据
	*********************************************/
	const _DBC_CITY_BUILDING*		m_pBuildingDefine;		//!< 建筑物定义(DBC_CITY_BUILDING)
	const _DBC_BUILDING_DEFINE*		m_pResDefine;			//!< 资源定义(DBC_BUILDING_DEINFE)

	ResType				m_resType;					//资源类型
	int					m_nLevel;					//当前逻辑等级

protected:
	WX_DECLARE_DYNAMIC(CObject_Building);
};
