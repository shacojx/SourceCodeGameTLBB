#ifndef _DB_ITEMLIST_H_
#define _DB_ITEMLIST_H_

#include "Type.h"
#include "ODBCBase.h"

class DBItemList:public ODBCBase
{
	GUID_t			m_CharGuid;
	UINT			m_DBVersion;
public:
	DBItemList(ODBCInterface* pInterface);

	virtual BOOL	Load();
	virtual BOOL	Save(VOID* pSource);
	virtual BOOL	Delete();
	virtual BOOL	ParseResult(VOID* pResult);
	INT		ParseResult(VOID* pResult, UINT& OldVersion, UINT& NowVersion);

public:
	VOID			SetCharGuid(GUID_t guid);
	GUID_t			GetCharGuid();
	VOID			SetDBVersion(UINT version);
	UINT			GetDBVersion();
private:
	UINT			CalcCRC(VOID* pSource);
};


#endif