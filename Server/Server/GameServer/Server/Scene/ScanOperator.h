
/////////////////////////////////////////////////////////////////////////////////
//文件名：ScanOperator.h
//功能描述：提供场景内obj或者human搜索的定制功能
//修改记录：2005-11-8 创建
/////////////////////////////////////////////////////////////////////////////////

#ifndef __SCANOPERATOR_H__
#define __SCANOPERATOR_H__


#include "Type.h"


enum SCANRETURN
{
	SCANRETURN_CONTINUE = 0 ,//继续扫描下一个obj
	SCANRETURN_BREAK ,//停止在当前Zone里的扫描，转到下一个Zone上去
	SCANRETURN_RETURN ,//中断扫描操作

	SCANRETURN_NUMBER ,
};


class Scene ;
class Obj ;

struct SCANOPERATOR_INIT
{
	//场景指针
	Scene*						m_pScene ;
	//搜索的中心ZoneID
	ZoneID_t					m_ZoneID ;
	//搜索的半径（以Zone数量为单位）
	INT							m_nZoneRadius ;
	//是否搜索HumanList
	//TRUE搜索HumanList,FALSE搜索ObjList
	BOOL						m_bScanHuman ;

	SCANOPERATOR_INIT( )
	{
		m_pScene = NULL ;
		m_ZoneID = INVALID_ID ;
		m_nZoneRadius = 0 ;
		m_bScanHuman = FALSE ;
	};
};

class ScanOperator
{
friend class Scene ;

public :
	ScanOperator( ) ;
	virtual ~ScanOperator( ) ;

	//初始化扫描控制器
	virtual BOOL				Init( SCANOPERATOR_INIT* pInit ) ;
	//扫描操作开始前回调
	virtual BOOL				On_BeforeScan( ){ return TRUE ; }
	//判断参数中的Zone是否需要扫描
	virtual BOOL				On_IsNeedScan( ZoneID_t ZoneID ){ return TRUE ; }
	//搜索到一个Obj, 返回值见 SCANRETURN
	virtual UINT				On_FindObj( Obj* pObj ){ return SCANRETURN_CONTINUE ; }
	//扫描操作完成后回调
	virtual VOID				On_AfterScan( ){ ; } ;

	//读取场景指针
	Scene*						GetScene( ){ return m_pScene ; }

protected :
	//场景指针
	Scene*						m_pScene ;
	//搜索的中心ZoneID
	ZoneID_t					m_ZoneID ;
	//搜索的半径（以Zone数量为单位）
	INT							m_nZoneRadius ;
	//是否搜索HumanList
	//TRUE搜索HumanList,FALSE搜索ObjList
	BOOL						m_bScanHuman ;
};











#endif
