#ifndef _DB_EQUIP_BASE_LIST_H_
#define _DB_EQUIP_BASE_LIST_H_

#include "Type.h"
#include "ODBCBase.h"

class DBEquipBaseList:public ODBCBase
{
	GUID_t			m_CharGuid;
public:
	DBEquipBaseList(ODBCInterface* pInterface);

	virtual BOOL	Load();
	virtual BOOL	Save();
	virtual BOOL	ParseResult(VOID* pResult);

public:
	VOID			SetCharGuid(GUID_t guid);
	GUID_t			GetCharGuid();
};


#endif