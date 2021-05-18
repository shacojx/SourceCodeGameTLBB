/////////////////////////////////////////////////////////////////////////////////
//文件名：ScanOperator_AESkill.h
//开发者：
//功能描述：面积有效技能的搜索操作
//修改记录：2005-12-6创建
/////////////////////////////////////////////////////////////////////////////////


#ifndef SCANOPERATOR_AESKILL_H
#define SCANOPERATOR_AESKILL_H

#include "Type.h"
#include "ScanOperator.h"
#include "AI_Monster_Def.h"
#include "SkillLogic.h"
#include "Obj_Character.h"
#include "Scene.h"
#include "SkillInfo.h"

using namespace Combat_Module::Skill_Module;

struct SCANOPERATOR_AESKILL_INIT : public SCANOPERATOR_INIT
{
	OBJLIST*		m_pTargets;
	FLOAT			m_fRadius;
	INT				m_nCount;
	SkillInfo_T const*	m_pSkillInfo;
	Obj_Character*	m_pMe;
	WORLD_POS		m_CentrePoint;
	SCANOPERATOR_AESKILL_INIT()
	{
		m_pTargets = NULL;
		m_fRadius = 0.0f ;
		m_nCount = 0;
		m_pSkillInfo = NULL;
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
			AssertEx(FALSE,"[SCANOPERATOR_AESKILL_INIT::InitBase]:Scene point is NULL!!please initialize it.");
			return;
		}
		m_ZoneID = m_pScene->CalcZoneID(&m_CentrePoint);
	}
};



class ScanOperator_AESkill : public ScanOperator
{
public :
	ScanOperator_AESkill();
	virtual ~ScanOperator_AESkill();

	//初始化扫描控制器
	virtual BOOL				Init(SCANOPERATOR_AESKILL_INIT* pInit) ;
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
	SkillInfo_T const*	m_pSkillInfo;
	Obj_Character*	m_pMe;
	WORLD_POS		m_CentrePoint;
	BOOL			m_bReady;
};




#endif //SCANOPERATOR_AESKILL_H
