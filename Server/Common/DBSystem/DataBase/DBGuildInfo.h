#ifndef _DB_GUILDINFO_H_
#define _DB_GUILDINFO_H_

#include "Type.h"
#include "ODBCBase.h"

class DBGuildInfo:public ODBCBase
{
	
public:
	DBGuildInfo(ODBCInterface* pInterface);
	virtual BOOL	Load();
	virtual BOOL	Save(VOID* pSource);
	virtual BOOL	Delete();
	virtual BOOL	ParseResult(VOID* pResult);

public:
	
};


#endif