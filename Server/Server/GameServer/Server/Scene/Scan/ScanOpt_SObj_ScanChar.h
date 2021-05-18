/////////////////////////////////////////////////////////////////////////////////
//文件名：ScanOpt_SObj_ScanChar.h
//开发者：
//功能描述：特殊对象搜索陷阱的操作
//修改记录：2005-2-10创建
/////////////////////////////////////////////////////////////////////////////////


#ifndef SCANOPT_SOBJ_SCANCHAR_H
#define SCANOPT_SOBJ_SCANCHAR_H

#include "Type.h"
#include "ScanOperator.h"
#include "Obj_Special.h"
#include "SpecialObjLogic.h"
#include "Obj_Character.h"
#include "Scene.h"

using namespace Combat_Module;
using namespace Combat_Module::Special_Obj_Module;

struct SCANOPT_SOBJ_SCANCHAR_INIT : public SCANOPERATOR_INIT
{
	OBJLIST*		m_pTargets;
	FLOAT			m_fRadius;
	INT				m_nCount;
	Obj_Special*	m_pMe;
	WORLD_POS		m_CentrePoint;
	SCANOPT_SOBJ_SCANCHAR_INIT()
	{
		m_pTargets = NULL;
		m_fRadius = 0.0f ;
		m_nCount = 0;
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
			AssertEx(FALSE,"[SCANOPT_SOBJ_SCANTRAP_INIT::InitBase]:Scene point is NULL!!please initialize it.");
			return;
		}
		m_ZoneID = m_pScene->CalcZoneID(&m_CentrePoint);
	}
};



class ScanOpt_SObj_ScanChar : public ScanOperator
{
public :
	ScanOpt_SObj_ScanChar();
	virtual ~ScanOpt_SObj_ScanChar();

	//初始化扫描控制器
	virtual BOOL				Init(SCANOPT_SOBJ_SCANCHAR_INIT* pInit) ;
	//扫描操作开始前回调
	//	virtual BOOL			On_BeforeScan();
	//判断参数中的Zone是否需要扫描
	//	virtual BOOL			On_IsNeedScan(ZoneID_t ZoneID) ;
	//搜索到一个Obj, 返回值见 SCANRETURN
	virtual UINT				On_FindObj(Obj* pObj) ;
	//扫描操作完成后回调
	//	virtual VOID			On_AfterScan() ;


public :
	OBJLIST*		m_pTargets;
	FLOAT			m_fRadius;
	INT				m_nCount;
	SpecialObjLogic_T const*	m_pLogic;
	Obj_Special*	m_pMe;
	WORLD_POS		m_CentrePoint;
	BOOL			m_bReady;
};




#endif //SCANOPT_SOBJ_SCANCHAR_H

