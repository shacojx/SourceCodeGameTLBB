// ActionSetMgr.h
//
//////////////////////////////////////////////////////

#pragma once

#include <string>
#include <hash_map>

struct _DBC_CHARACTER_ACTION_SET;
namespace DBC
{
	class DBCFile;
}

class CActionSetMgr
{
protected:
	static CActionSetMgr		*s_pActionSetMgr;
public:
	static CActionSetMgr *GetActionSetMgr( VOID ){
		return s_pActionSetMgr;
	}
public:
	CActionSetMgr( VOID );
	virtual ~CActionSetMgr( VOID );

	BOOL Init( STRING strResPath );
	VOID CleanUp( VOID );

	const _DBC_CHARACTER_ACTION_SET *GetActionSet( std::string strFileName, UINT dwID );
	const DBC::DBCFile *GetActionSetFile(std::string strFileName);

protected:
#ifdef __SGI_STL_PORT
	typedef std::hash_map< std::string, DBC::DBCFile* > CActionSetMap;
#else
	typedef stdext::hash_map< std::string, DBC::DBCFile* > CActionSetMap;
#endif
	CActionSetMap			m_mapActionSet;

	STRING					m_strResPath;
};

inline CActionSetMgr *GetActionSetMgr( VOID ){
	return CActionSetMgr::GetActionSetMgr();
}
