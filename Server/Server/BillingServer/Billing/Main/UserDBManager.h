#pragma once
#include "DBManager.h"
#include "ODBCInterface.h"
 
class UserDBManager
{
public:
#define		MAX_SQL_LENGTH				4096
	UserDBManager(void);
	virtual ~UserDBManager(void);
	BOOL Init();
	BOOL AddUser(
		int UserId, 
		char* UserName, 
		char* PassWord, 
		int Authority, 
		int Type, 
		char *Sign, 
		int CardType, 
		char *CardId, 
		int PhoneType, 
		char *Phone);
	BOOL AmendPassWord(char* UserName, char* PassWord, char* NewPassWord);
	BOOL DeleteUser(char* UserName);
	INT  GetUserSum(void);
	BOOL IsHaveUser(char* UserName);
	BOOL IsRealUser(const char* UserName, const char* PassWord);
	BOOL WX_Shop_Add(
		INT ShopId, 
		INT UserId, 
		INT Money, 
		INT Item_1,
		INT ItemCount_1,
		INT Item_2,
		INT ItemCount_2,
		INT Item_3,
		INT ItemCount_3,
		INT Item_4,
		INT ItemCount_4,
		INT Item_5,
		INT ItemCount_5,
		INT IsSucceed,
		CHAR* CreateTime);
	BOOL WorldWX_Shop_IsHave(UINT ShopID);
	DBManager *m_pDBManager;
	char m_strSql[MAX_SQL_LENGTH];
};
extern UserDBManager g_UserDBManager;
