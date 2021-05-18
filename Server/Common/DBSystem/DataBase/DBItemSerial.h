#ifndef _DB_ITEMSERIAL_H_
#define _DB_ITEMSERIAL_H_

#include "Type.h"
#include "ODBCBase.h"

class DBItemSerial:public ODBCBase
{
	ID_t			m_ServerID;
public:
	DBItemSerial(ODBCInterface* pInterface);
	virtual BOOL	Load();
	virtual BOOL	Save(VOID* pSource);
	virtual BOOL	Delete();
	virtual BOOL	ParseResult(VOID* pResult);

public:
	VOID			SetSID(ID_t sid);
	ID_t			GetSID();
};


#endif