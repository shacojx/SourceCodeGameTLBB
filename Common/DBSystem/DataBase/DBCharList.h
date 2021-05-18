#ifndef _DB_CHAR_LIST_H_
#define _DB_CHAR_LIST_H_
#include "Type.h"
#include "ODBCBase.h"





class DBCharList:public ODBCBase
{
	CHAR			szAccount[MAX_ACCOUNT+1] ;	//ÓÃ»§Ãû³Æ
	UINT			m_CharNumber;
	GUID_t			m_GuidList[DB_CHAR_NUMBER];
public:
	DBCharList(ODBCInterface* pInterface);
	VOID			CleanUp();
	virtual BOOL	Load();
	virtual BOOL	Save();
	virtual BOOL	ParseResult(VOID* pResult);

public:
	const	CHAR*			GetAccount()	const;
	VOID					SetAccount(const CHAR*	pAccount);

	UINT					GetCharNumber();
	GUID_t					GetGuid(UINT Index);
	
};



#endif