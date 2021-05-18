#include "stdafx.h"
#include "UserDBManager.h"

UserDBManager g_UserDBManager;
UserDBManager::UserDBManager(void)
{
	__ENTER_FUNCTION
		__LEAVE_FUNCTION
}

BOOL UserDBManager::Init()
{
	__ENTER_FUNCTION

		m_pDBManager = new DBManager;
	if(m_pDBManager->Init())
	{	
		sprintf( m_strSql, "USE WXSJ");
		strcpy((char*)m_pDBManager->GetInterface(CHARACTER_DATABASE)->m_Query.m_SqlStr, m_strSql);
		m_pDBManager->GetInterface(CHARACTER_DATABASE)->Clear();
		if(m_pDBManager->GetInterface(CHARACTER_DATABASE)->Execute())
		{
			return TRUE;
		}
	}
	else
	{
		return FALSE;
	}

	__LEAVE_FUNCTION
		return FALSE;
}

UserDBManager::~UserDBManager(void)
{
	__ENTER_FUNCTION

		delete m_pDBManager;

	__LEAVE_FUNCTION
}

BOOL UserDBManager::AddUser(
							int UserId, 
							char* UserName, 
							char* PassWord, 
							int Authority, 
							int Type, 
							char *Sign, 
							int CardType, 
							char *CardId, 
							int PhoneType, 
							char *Phone)
{
	__ENTER_FUNCTION

		sprintf( m_strSql, "EXECUTE WorldWX_User_Add @UserId = %d, @UserName = '%s', @PassWord = '%s', @Authority = %d, @Type = %d, @Sign = %s, @CardType = %d, @CardId = %s, @PhoneType = %d, @Phone = %s",
		UserId, UserName, PassWord, Authority, Type, Sign, CardType, CardId, PhoneType, Phone);
	strcpy((char*)m_pDBManager->GetInterface(CHARACTER_DATABASE)->m_Query.m_SqlStr, m_strSql);
	m_pDBManager->GetInterface(CHARACTER_DATABASE)->Clear();
	if(m_pDBManager->GetInterface(CHARACTER_DATABASE)->Execute())
	{
		if(m_pDBManager->GetInterface(CHARACTER_DATABASE)->mAffectCount > 0)
			return TRUE;
	}
	else
	{
		return FALSE;
	}

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL UserDBManager::AmendPassWord(char* UserName, char* PassWord, char* NewPassWord)
{
	__ENTER_FUNCTION

		sprintf( m_strSql, "EXECUTE WorldWX_User_AmendPassWord @UserName = '%s', @PassWord = '%s', @NewPassWord = '%s'", 
		UserName, PassWord, NewPassWord );
	strcpy((char*)m_pDBManager->GetInterface(CHARACTER_DATABASE)->m_Query.m_SqlStr, m_strSql);
	m_pDBManager->GetInterface(CHARACTER_DATABASE)->Clear();
	if(m_pDBManager->GetInterface(CHARACTER_DATABASE)->Execute())
	{
		if(m_pDBManager->GetInterface(CHARACTER_DATABASE)->mAffectCount > 0)
			return TRUE;
	}
	else
	{
		return FALSE;
	}

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL UserDBManager::DeleteUser(char* UserName)
{
	__ENTER_FUNCTION

		sprintf( m_strSql, "EXECUTE WorldWX_User_Delete @UserName = '%s' ", UserName);
	strcpy((char*)m_pDBManager->GetInterface(CHARACTER_DATABASE)->m_Query.m_SqlStr, m_strSql);
	m_pDBManager->GetInterface(CHARACTER_DATABASE)->Clear();
	if(m_pDBManager->GetInterface(CHARACTER_DATABASE)->Execute())
	{
		if(m_pDBManager->GetInterface(CHARACTER_DATABASE)->mAffectCount > 0)
			return TRUE;
	}
	else
	{
		return FALSE;
	}

	__LEAVE_FUNCTION

		return FALSE;
}



INT  UserDBManager::GetUserSum(void)
{
	__ENTER_FUNCTION

		sprintf( m_strSql, "EXECUTE WorldWX_User_GetSum" );
	strcpy((char*)m_pDBManager->GetInterface(CHARACTER_DATABASE)->m_Query.m_SqlStr, m_strSql);
	m_pDBManager->GetInterface(CHARACTER_DATABASE)->Clear();
	if(m_pDBManager->GetInterface(CHARACTER_DATABASE)->Execute())
	{
		m_pDBManager->GetInterface(CHARACTER_DATABASE)->Fetch();
		if(m_pDBManager->GetInterface(CHARACTER_DATABASE)->Col[0][0] != 0)
		{
			return atoi(m_pDBManager->GetInterface(CHARACTER_DATABASE)->Col[0]);
		}
	}
	else
	{
		return FALSE;
	}

	__LEAVE_FUNCTION

		return FALSE;
}



BOOL UserDBManager::IsHaveUser(char* UserName)
{
	__ENTER_FUNCTION

		sprintf( m_strSql, "EXECUTE WorldWX_User_IsHave @UserName = '%s'", UserName );
	strcpy((char*)m_pDBManager->GetInterface(CHARACTER_DATABASE)->m_Query.m_SqlStr, m_strSql);
	m_pDBManager->GetInterface(CHARACTER_DATABASE)->Clear();
	if(m_pDBManager->GetInterface(CHARACTER_DATABASE)->Execute())
	{
		m_pDBManager->GetInterface(CHARACTER_DATABASE)->Fetch();
		if(m_pDBManager->GetInterface(CHARACTER_DATABASE)->Col[0][0] != 0)
		{
			return atoi(m_pDBManager->GetInterface(CHARACTER_DATABASE)->Col[0]);
		}
	}
	else
	{
		return FALSE;
	}

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL UserDBManager::IsRealUser(const char* UserName, const char* PassWord)
{
	__ENTER_FUNCTION

		sprintf( m_strSql, "EXECUTE WorldWX_User_IsReal @UserName = '%s', @PassWord = '%s'", UserName, PassWord );
	strcpy((char*)m_pDBManager->GetInterface(CHARACTER_DATABASE)->m_Query.m_SqlStr, m_strSql);
	m_pDBManager->GetInterface(CHARACTER_DATABASE)->Clear();
	if(m_pDBManager->GetInterface(CHARACTER_DATABASE)->Execute())
	{
		m_pDBManager->GetInterface(CHARACTER_DATABASE)->Fetch();
		if(m_pDBManager->GetInterface(CHARACTER_DATABASE)->Col[0][0] != 0)
		{
			return atoi(m_pDBManager->GetInterface(CHARACTER_DATABASE)->Col[0]);
		}
	}
	else
	{
		return FALSE;
	}

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL UserDBManager::WX_Shop_Add(
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
								CHAR* CreateTime)
{
	__ENTER_FUNCTION

		sprintf( m_strSql, " EXECUTE WX_Shop_Add "
		" @Id = %d, "
		" @UserId = %d, "
		" @Money  = %d, "
		" @Item_1  = %d, "
		" @ItemCount_1 = %d, "
		" @Item_2 = %d, "
		" @ItemCount_2 = %d, "
		" @Item_3 = %d, "
		" @ItemCount_3 = %d, "
		" @Item_4 = %d, "
		" @ItemCount_4 = %d, "
		" @Item_5 = %d, "
		" @ItemCount_5 = %d, "
		" @IsSucceed = %d, "
		" @CreateTime = '%s'",
		ShopId, 
		UserId, 
		Money, 
		Item_1,
		ItemCount_1,
		Item_2,
		ItemCount_2,
		Item_3,
		ItemCount_3,
		Item_4,
		ItemCount_4,
		Item_5,
		ItemCount_5,
		IsSucceed,
		CreateTime);

	strcpy((char*)m_pDBManager->GetInterface(CHARACTER_DATABASE)->m_Query.m_SqlStr, m_strSql);
	m_pDBManager->GetInterface(CHARACTER_DATABASE)->Clear();
	if(m_pDBManager->GetInterface(CHARACTER_DATABASE)->Execute())
	{
		INT aaa = m_pDBManager->GetInterface(CHARACTER_DATABASE)->mAffectCount;

		if(aaa > 0)
		{
			return TRUE;
		}
	}
	else
	{
		return FALSE;
	}

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL UserDBManager::WorldWX_Shop_IsHave(UINT ShopID)
{
	__ENTER_FUNCTION

		sprintf( m_strSql, "EXECUTE WorldWX_Shop_IsHave @Shop_Id = %d", ShopID );
	strcpy((char*)m_pDBManager->GetInterface(CHARACTER_DATABASE)->m_Query.m_SqlStr, m_strSql);
	m_pDBManager->GetInterface(CHARACTER_DATABASE)->Clear();
	if(m_pDBManager->GetInterface(CHARACTER_DATABASE)->Execute())
	{
		m_pDBManager->GetInterface(CHARACTER_DATABASE)->Fetch();
		if(m_pDBManager->GetInterface(CHARACTER_DATABASE)->Col[0][0] != 0)
		{
			return atoi(m_pDBManager->GetInterface(CHARACTER_DATABASE)->Col[0]);
		}
	}
	else
	{
		return FALSE;
	}

	__LEAVE_FUNCTION

		return FALSE;
}