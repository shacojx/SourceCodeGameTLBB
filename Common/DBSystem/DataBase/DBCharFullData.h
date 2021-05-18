#ifndef _DB_CHARFULLDATA_H_
#define _DB_CHARFULLDATA_H_

#include "Type.h"
#include "ODBCBase.h"

class DBCharFullData:public ODBCBase
{
	GUID_t			m_CharGuid;
public:
	DBCharFullData(ODBCInterface* pInterface);

	virtual BOOL	Load();
	virtual BOOL	Save(VOID* pSource);
	virtual BOOL	ParseResult(VOID* pResult);
	INT		ParseResult(VOID* pResult, UINT& result1, UINT& result2, UINT& result3, UINT& result4);

public:
	VOID			SetCharGuid(GUID_t guid);
	GUID_t			GetCharGuid();

private:
	UINT			CalcCRC(VOID* pSource);
};


#endif