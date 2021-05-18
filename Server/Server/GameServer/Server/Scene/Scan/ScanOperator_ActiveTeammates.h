/////////////////////////////////////////////////////////////////////////////////
//文件名：
//功能描述：搜索周围的敌人obj
//修改记录：2005-11-8创建
/////////////////////////////////////////////////////////////////////////////////


#ifndef __SCANOPERATOR_ACTIVETEAMMATES_H__
#define __SCANOPERATOR_ACTIVETEAMMATES_H__

#include "Type.h"
#include "ScanOperator.h"

class Obj_Human ;

struct SCANOPERATOR_ACTIVETEAMMATES_INIT : public SCANOPERATOR_INIT
{
	WORLD_POS					m_Position ;
	TeamID_t					m_TeamID ;
	GUID_t						m_MemberGUID ;
	FLOAT						m_fRadius ;

	SCANOPERATOR_ACTIVETEAMMATES_INIT( )
	{
		m_TeamID = INVALID_ID ;
		m_MemberGUID = INVALID_ID ;
		m_fRadius = 0.0f ;
	}
};



class ScanOperator_ActiveTeammates : public ScanOperator
{
public :
	ScanOperator_ActiveTeammates( ) ;
	virtual ~ScanOperator_ActiveTeammates( ) ;

	//初始化扫描控制器
	virtual BOOL				Init( SCANOPERATOR_ACTIVETEAMMATES_INIT* pInit ) ;
	//扫描操作开始前回调
//	virtual BOOL				On_BeforeScan( ) ;
	//判断参数中的Zone是否需要扫描
//	virtual BOOL				On_IsNeedScan( ZoneID_t ZoneID ) ;
	//搜索到一个Obj, 返回值见 SCANRETURN
	virtual UINT				On_FindObj( Obj* pObj ) ;
	//扫描操作完成后回调
//	virtual VOID				On_AfterScan( ) ;


public :
	WORLD_POS					m_Position ;
	TeamID_t					m_TeamID ;
	GUID_t						m_MemberGUID ;
	FLOAT						m_fRadius ;

	Obj_Human*					m_aActiveTeammate[MAX_TEAM_MEMBER] ;
	INT							m_nActiveTeammateCount ;

};




#endif
