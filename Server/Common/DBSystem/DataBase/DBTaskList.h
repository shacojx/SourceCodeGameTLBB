#ifndef _DB_TASKLIST_H_
#define _DB_TASKLIST_H_

#include "Type.h"
#include "ODBCBase.h"

class DBTaskList:public ODBCBase
{
	GUID_t			m_CharGuid;
	UINT			m_DBVersion;
public:
	DBTaskList(ODBCInterface* pInterface);

	virtual BOOL	Load();
	virtual BOOL	Save(VOID* pSource);
	virtual BOOL	Delete();

	virtual BOOL	ParseResult(VOID* pResult);

public:
	VOID			SetCharGuid(GUID_t guid);
	GUID_t			GetCharGuid();
	VOID			SetDBVersion(UINT version);
	UINT			GetDBVersion();
};


#endif