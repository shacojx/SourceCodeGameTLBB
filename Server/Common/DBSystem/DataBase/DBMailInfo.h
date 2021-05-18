#ifndef _DB_MAILINFO_H_
#define _DB_MAILINFO_H_

#include "Type.h"
#include "ODBCBase.h"

class DBMailInfo:public ODBCBase
{
	
public:
	DBMailInfo(ODBCInterface* pInterface);

	virtual BOOL	Load();
	virtual BOOL	Save(VOID* pSource);
	virtual BOOL	Delete();
	virtual BOOL	ParseResult(VOID* pResult);

public:
	
};


#endif