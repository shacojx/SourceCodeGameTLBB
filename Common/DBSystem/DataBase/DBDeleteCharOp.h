#ifndef _DB_DELETECHAR_OP_H_
#define _DB_DELETECHAR_OP_H_

#include "Type.h"
#include "ODBCBase.h"



class DBDeleteCharOp:public ODBCBase
{
	UINT			m_AID;
	CHAR			m_Account[MAX_ACCOUNT+1];	//ÓÃ»§Ãû³Æ
	GUID_t			m_CharGuid;
public:
	DBDeleteCharOp(ODBCInterface* pInterface);

	virtual BOOL	Load();
	virtual BOOL	AddNew();
	virtual BOOL	Delete();
	virtual BOOL	ParseResult(VOID* pResult);

public:
	const	CHAR*	GetAccount()	const;
	VOID			SetAccount(const CHAR*	pAccount);

	VOID			SetCharGuid(GUID_t guid);
	GUID_t			GetCharGuid();

};


#endif