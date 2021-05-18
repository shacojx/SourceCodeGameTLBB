/////////////////////////////////////////////////////////////////////////////////
//文件名：ScanOperator_AEImpact.h
//开发者：
//功能描述：面积有效技能的搜索操作
//修改记录：2005-2-15创建
/////////////////////////////////////////////////////////////////////////////////


#ifndef SCANOPERATOR_AEIMPACT_H
#define SCANOPERATOR_AEIMPACT_H

#include "Type.h"
#include "ScanOperator.h"
#include "ImpactLogic.h"
#include "Obj_Character.h"
#include "Scene.h"

using namespace Impact_Module;

struct SCANOPERATOR_AEIMPACT_INIT : public SCANOPERATOR_INIT
{
	OBJLIST*	m_pTargets;
	FLOAT			m_fRadius;
	INT				m_nCount;
	OWN_IMPACT		m_Impact;
	ImpactLogic_T const*	m_pLogic;
	Obj_Character*	m_pMe;
	WORLD_POS		m_CentrePoint;
	SCANOPERATOR_AEIMPACT_INIT()
	{
		m_pTargets = NULL;
		m_fRadius = 0.0f ;
		m_nCount = 0;
		m_Impact.CleanUp();
		m_pLogic=NULL;
		m_pMe = NULL;
		m_CentrePoint.CleanUp();
	}
	VOID InitBase(VOID)
	{
		INT& rZoneSize = g_Config.m_ConfigInfo.m_ZoneSize;
		if(0!=rZoneSize)
		{
			m_nZoneRadius = INT(m_fRadius/rZoneSize)+1;
		}
		else
		{
			m_nZoneRadius = 0;
		}
		if(NULL==m_pScene)
		{
			AssertEx(FALSE,"[SCANOPERATOR_AEIMPACT_INIT::InitBase]:Scene point is NULL!!please initialize it.");
			return;
		}
		m_ZoneID = m_pScene->CalcZoneID(&m_CentrePoint);
	}
};



class ScanOperator_AEImpact : public ScanOperator
{
public :
	ScanOperator_AEImpact();
	virtual ~ScanOperator_AEImpact();

	//初始化扫描控制器
	virtual BOOL				Init(SCANOPERATOR_AEIMPACT_INIT* pInit) ;
	//扫描操作开始前回调
	//	virtual BOOL			On_BeforeScan();
	//判断参数中的Zone是否需要扫描
	//	virtual BOOL			On_IsNeedScan(ZoneID_t ZoneID) ;
	//搜索到一个Obj, 返回值见 SCANRETURN
	virtual UINT				On_FindObj(Obj* pObj) ;
	//扫描操作完成后回调
	//	virtual VOID			On_AfterScan() ;


public :
	OBJLIST*	m_pTargets;
	FLOAT			m_fRadius;
	INT				m_nCount;
	OWN_IMPACT		m_Impact;
	ImpactLogic_T const*	m_pLogic;
	Obj_Character*	m_pMe;
	WORLD_POS		m_CentrePoint;
	BOOL			m_bReady;
};




#endif //SCANOPERATOR_AEIMPACT_H
