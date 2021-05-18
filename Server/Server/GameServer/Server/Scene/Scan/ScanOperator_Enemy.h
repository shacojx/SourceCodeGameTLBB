/////////////////////////////////////////////////////////////////////////////////
//文件名：
//功能描述：搜索周围的敌人obj
/////////////////////////////////////////////////////////////////////////////////


#ifndef __SCANOPERATOR_ENEMY_H__
#define __SCANOPERATOR_ENEMY_H__

#include "Type.h"
#include "ScanOperator.h"

class Obj_Monster ;

struct SCANOPERATOR_ENEMY_INIT : public SCANOPERATOR_INIT
{
	Obj_Monster*				m_pMonster ;
	FLOAT						m_fRadius ;

	SCANOPERATOR_ENEMY_INIT( )
	{
		m_pMonster = NULL ;
		m_fRadius = 0.0f ;
	}
};



class Monster_ScanOperator_Enemy : public ScanOperator
{
public :
	Monster_ScanOperator_Enemy( ) ;
	virtual ~Monster_ScanOperator_Enemy( ) ;

	//初始化扫描控制器
	virtual BOOL				Init( SCANOPERATOR_ENEMY_INIT* pInit ) ;
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

};




#endif
