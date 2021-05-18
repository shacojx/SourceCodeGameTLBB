/////////////////////////////////////////////////////////////////////////////////
//文件名：
//功能描述：搜索周围的敌人obj
//修改记录：2005-11-9创建
/////////////////////////////////////////////////////////////////////////////////


#ifndef __SCANOPERATOR_TEAMMATE_H__
#define __SCANOPERATOR_TEAMMATE_H__

#include "Type.h"
#include "ScanOperator.h"
#include "AI_Monster_Def.h"

class Obj_Monster ;

struct SCANOPERATOR_TEAMMATE_INIT : public SCANOPERATOR_INIT
{
	Obj_Monster*				m_pMonster ;
	FLOAT						m_fRadius ;
	INT							m_nCount;
	INT							m_Type;
	BOOL						m_bOnlyNoEnemy; //只找没有敌人的同伴
	BOOL						m_bScanAllMonster; //扫描所以类型的怪

	SCANOPERATOR_TEAMMATE_INIT( )
	{
		m_pMonster = NULL ;
		m_fRadius = 0.0f ;
		m_nCount = 0;
		m_Type = MONSTERAI_SUMMON_REGION;
		m_bOnlyNoEnemy = TRUE;
		m_bScanAllMonster = FALSE;
	}
};



class ScanOperator_Teammate : public ScanOperator
{
public :
	ScanOperator_Teammate( ) ;
	virtual ~ScanOperator_Teammate( ) ;

	//初始化扫描控制器
	virtual BOOL				Init( SCANOPERATOR_TEAMMATE_INIT* pInit ) ;
	//扫描操作开始前回调
	//	virtual BOOL				On_BeforeScan( ) ;
	//判断参数中的Zone是否需要扫描
	//	virtual BOOL				On_IsNeedScan( ZoneID_t ZoneID ) ;
	//搜索到一个Obj, 返回值见 SCANRETURN
	virtual UINT				On_FindObj( Obj* pObj ) ;
	//扫描操作完成后回调
	//	virtual VOID				On_AfterScan( ) ;


public :
	Obj_Monster*				m_pMonster ;
	FLOAT						m_fRadius ;
	INT							m_nCount;
	INT							m_Type;
	BOOL						m_bOnlyNoEnemy; //只找没有敌人的同伴
	BOOL						m_bScanAllMonster; //扫描所以类型的怪

};




#endif
