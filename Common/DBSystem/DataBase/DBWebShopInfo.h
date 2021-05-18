#ifndef _DB_WEBSHOPINFO_H_
#define _DB_WEBSHOPINFO_H_

#include "Type.h"
#include "ODBCBase.h"

class DBWebShopInfo:public ODBCBase
{
	GUID_t			m_CharGuid;
	UINT			m_DBVersion;
	UINT			m_IsSucceed;

	UINT			m_Id;
public:
	DBWebShopInfo(ODBCInterface* pInterface);

	virtual BOOL	Load();
	virtual BOOL	Save(VOID* pSource);
	virtual BOOL	Delete();

	virtual BOOL	ParseResult(VOID* pResult);

public:
	VOID			SetCharGuid(GUID_t guid);
	GUID_t			GetCharGuid();
	VOID			SetIsSucceed(UINT succ);
	UINT			GetIsSucceed();
	VOID			SetWebShopInfoId(UINT id);
	UINT			GetWebShopInfoId();
	VOID			SetDBVersion(UINT version);
	UINT			GetDBVersion();
};


#endif