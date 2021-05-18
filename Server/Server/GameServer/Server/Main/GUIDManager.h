
#ifndef __GUIDMANAGER_H__
#define __GUIDMANAGER_H__


#include "Type.h"


#define MAX_GUID_SIZE 102400

struct _GUIDITEM
{
	GUID_t	m_GUID ;
	VOID*	m_pPtr ;
	uint	m_Status ;
};


class GUIDManager
{
public :
	GUIDManager( ) ;
	~GUIDManager( ) ;

	BOOL					Init( ) ;


	BOOL					Add( GUID_t gid, VOID* pPtr ) ;
	VOID*					Get( GUID_t gid ) ;
	BOOL					Del( GUID_t gid, VOID* pPtr ) ;

	VOID					CleanUp( ) ;

	//临时使用
	GUID_t					CreateGUID( ){ return m_TempGUID++ ; } ;


public :
	enum 
	{
		GUIDTS_EMPTY = 0 ,
		GUIDTS_SET = 1 ,
		GUIDTS_USE = 2 ,
	};


private :
	_GUIDITEM				m_pGuidItem[MAX_GUID_SIZE] ;
	INT						m_nCount ;

	MyLock					m_Lock ;

	//临时使用
	GUID_t					m_TempGUID ;

};

extern GUIDManager* g_pGUIDManager ;


#endif
