#include "stdafx.h"
#include "DBDeleteCharOp.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBItemList.h"
#include "DBSkillList.h"
#include "DBXinFaList.h"
#include "DBAbilityList.h"
#include "DBTaskList.h"
#include "DBRelationList.h"
#include "DBImpactList.h"
#include "DBPetList.h"
#include "DBManager.h"

DBDeleteCharOp::DBDeleteCharOp(ODBCInterface* pInterface)
{
	__ENTER_FUNCTION

	mDBName				= CHARACTER_DATABASE;
	mResult				= 0;
	mResultCount		= 0;
	m_Account[0]		= '\0';
	m_AID				= 0;
	Assert(pInterface);
	mInterface		=	pInterface;

	__LEAVE_FUNCTION

}

BOOL DBDeleteCharOp::Load()
{
	return TRUE;
}

BOOL DBDeleteCharOp::AddNew()
{
	return TRUE;
}

BOOL DBDeleteCharOp::Delete()
{
	__ENTER_FUNCTION

	DB_QUERY* pQuery = GetInternalQuery();

	if(!pQuery)
	{
		Assert(FALSE);
	}

	pQuery->Clear();

	if(m_Account[0]=='\0')
	{
		return FALSE;
	}
	
	if(!StrSafeCheck(m_Account,MAX_ACCOUNT+1))
		return FALSE;

	pQuery->Parse(DeleteBaseChar,
				  CHAR_TABLE,
				   m_CharGuid,
				   m_Account,
				   m_CharGuid );

	return ODBCBase::Delete();

	__LEAVE_FUNCTION

	return FALSE;

	
}

BOOL DBDeleteCharOp::ParseResult(VOID* pResult)
{	
	__ENTER_FUNCTION

	ASKDELETECHAR_RESULT* pAskResult = static_cast<ASKDELETECHAR_RESULT*>(pResult);
	Assert(pAskResult);

	if(GetInternalAffectCount()>0)
	{
		*pAskResult = ASKDELETECHAR_SUCCESS;
	}
	else
	{
		*pAskResult = ASKDELETECHAR_INTERNAL_ERROR;
	}

	if(*pAskResult == ASKDELETECHAR_SUCCESS	)
	{
		//删除装备
		DBItemList	ItemListObject(mInterface);
		ItemListObject.SetCharGuid(m_CharGuid);
		if(!ItemListObject.Delete())
			return FALSE;
		ItemListObject.ParseResult(pAskResult);

		//删除技能
		DBSkillList	SkillListObject(mInterface);
		SkillListObject.SetCharGuid(m_CharGuid);
		if(!SkillListObject.Delete())
			return FALSE;
		SkillListObject.ParseResult(pAskResult);

		//删除心法
		DBXinFaList	XinFaListObject(mInterface);
		XinFaListObject.SetCharGuid(m_CharGuid);
		if(!XinFaListObject.Delete())
			return FALSE;
		XinFaListObject.ParseResult(pAskResult);

		//删除生活技能
		DBAbilityList AbilityListObject(mInterface);
		AbilityListObject.SetCharGuid(m_CharGuid);
		if(!AbilityListObject.Delete())
			return FALSE;
		AbilityListObject.ParseResult(pAskResult);

		//删除任务信息
		DBTaskList	TaskListObject(mInterface);
		TaskListObject.SetCharGuid(m_CharGuid);
		if(!TaskListObject.Delete())
			return FALSE;
		TaskListObject.ParseResult(pAskResult);

		//删除关系列表
		DBRelationList	RelationListObject(mInterface);
		RelationListObject.SetCharGuid(m_CharGuid);
		if(!RelationListObject.Delete())
			return FALSE;
		RelationListObject.ParseResult(pAskResult);

		//删除Impact 列表
		DBImpactList	ImpactListObject(mInterface);
		ImpactListObject.SetCharGuid(m_CharGuid);
		if(!ImpactListObject.Delete())
			return FALSE;
		ImpactListObject.ParseResult(pAskResult);
		//删除宠物列表
		DBPetList	PetListObject(mInterface);
		PetListObject.SetCharGuid(m_CharGuid);
		if(!PetListObject.Delete())
			return FALSE;
		PetListObject.ParseResult(pAskResult);
	}


	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

const	CHAR*	DBDeleteCharOp::GetAccount()	const
{
	return m_Account;
}
VOID	DBDeleteCharOp::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(m_Account,pAccount,MAX_ACCOUNT*sizeof(CHAR));
	m_Account[MAX_ACCOUNT] = '\0' ;
}
GUID_t	DBDeleteCharOp::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBDeleteCharOp::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}

