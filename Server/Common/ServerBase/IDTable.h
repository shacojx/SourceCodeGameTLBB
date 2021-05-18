

#ifndef __IDTABLE_H__
#define __IDTABLE_H__

#include "Type.h"


struct _TABLEITEM
{
	uint	m_ID ;
	VOID*	m_pPtr ;
	uint	m_Status ;
};

class IDTable
{
public :
	IDTable( ) ;
	~IDTable( ) ;

	//初始化表
	VOID		InitTable( uint MaxItem ) ;
	//增加一个表项
	BOOL		Add( uint id, VOID* pPtr ) ;
	//读取信息
	VOID*		Get( uint id ) ;
	//删除表项
	VOID		Remove( uint id ) ;
	//清除所有数据
	VOID		CleanUp( ) ;

protected :
	enum {
		IDTS_EMPTY = 0 ,
		IDTS_SET = 1 ,
		IDTS_USE = 2 ,
	};

	UINT		toIndex( uint id )
	{
		return (UINT)((id&0xffff)+(id>>6))%m_Count ;
	};

private :

	_TABLEITEM*		m_pTable ;
	uint			m_Count ;


};

#endif
