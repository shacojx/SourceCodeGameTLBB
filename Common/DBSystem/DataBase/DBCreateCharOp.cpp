#include "stdafx.h"
#include "DBCreateCharOp.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"


DBCreateCharOp::DBCreateCharOp(ODBCInterface*	pInterface)
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

BOOL DBCreateCharOp::Load()
{
		return TRUE;
}

BOOL DBCreateCharOp::AddNew()
{
	__ENTER_FUNCTION


	do 
	{
		//////////////////////////////////////////////////////////////////////////
		//处理guid 操作
		//////////////////////////////////////////////////////////////////////////
		enum	
		{
			DB_VAR_CHARGUID	= 1,
		};

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
		pQuery->Parse(FetchNewGuid);
		
		if(!ODBCBase::Load())
			return FALSE;
		
		//Assert(mResultCount==1);
		Assert(mInterface);
		if(!mInterface->Fetch())	
			return FALSE;
		
		INT ErrorCode;

		m_CharGuid = mInterface->GetUInt(DB_VAR_CHARGUID,ErrorCode);

		mInterface->Clear();
	} 
	while(0);
	

	//////////////////////////////////////////////////////////////////////////
	//处理 Camp
	CHAR CharCamp[100];
	memset(CharCamp,0,100);
	_CAMP_DATA TempCamp;
	TempCamp.m_nCampID = m_Camp;
	Binary2String((CHAR*)(&TempCamp), sizeof(_CAMP_DATA), CharCamp);



	//////////////////////////////////////////////////////////////////////////
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

	if(!StrSafeCheck(m_CharName,MAX_CHARACTER_NAME))
		return FALSE;


	pQuery->Parse(CreateNewChar,
				 CHAR_TABLE,
				 m_Account,
				 m_CharGuid,
				 m_CharName,
				 "",
				 "",
				 m_Sex,		  //性别
				 1,           //级别
				 0,			  //enegry
				 0,			  //外观
				 0,			  //场景
				 100,		  //xPos
				 100,		  //zPos
				 m_MenPai,	  //门派
				 5000,		  //hp
				 5000,		  //mp
				 0,			  //strikerpoint
				 CharCamp,	  //阵营
				 5,			  //str
				 5,			  //con
				 5,			  //dex
				 5,		      //spr
				 5,			  //ipr
				 0,			  //remainpoints
				 0,			  //logouttime
				 0,			  //logintime
				 0,			  //createtime
				 0,			  //dbversion
				 m_HairColor, //haircolor
				 m_HairModel, //hairmodel
				 m_FaceColor, //facecolor
				 m_FaceModel, //facemodel
				 0,			  //vmoney
				 "",		  //settings
				 1,			  //isvalid
				 0,			  //exp
				 "",		  //pres
				 "",		  //shopinfo
				 "",		  //carrypet
				 -1,		  //guldid
				 -1,		  //teamid
				 m_HeadID,	  //headid
				 0,			  //erecover
				 0,			  //RMB
				 0,			  //BankRMB
				 0,			  //vrecover
				 0,			  //energymax
				 0,			  //pwdeltime
				 "",		  //pinfo,
				 0,			  //bkscene,
				 0,			  //bkxpos,
				 0,			  //bkzpos,
				 "",		  //titleinfo
				 0,			  //dietime	
				 0,			  //bankmoney
				 20,		  //bankend
				 ""			  //cooldown
				 );

	return ODBCBase::AddNew();

	return TRUE;

	__LEAVE_FUNCTION	

	return FALSE;
}

BOOL DBCreateCharOp::ParseResult(VOID* pResult)
{

	ASKCREATECHAR_RESULT* pAskResult = static_cast<ASKCREATECHAR_RESULT*>(pResult);
	Assert(pAskResult);
	
	Assert(mInterface);

	

    if(GetInternalAffectCount()>0)
	{
		*pAskResult = ASKCREATECHAR_SUCCESS;
	}
	else
	{
		*pAskResult = ASKCREATECHAR_INTERNAL_ERROR;
	}

	//if(!mInterface->Fetch())	
	//{
	//	*pAskResult = ASKCREATECHAR_INTERNAL_ERROR;
	//}
	mInterface->Clear();

	
	return TRUE;
}

const	CHAR*	DBCreateCharOp::GetAccount()	const
{
	return m_Account;
}
VOID	DBCreateCharOp::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(m_Account,pAccount,MAX_ACCOUNT*sizeof(CHAR));
	m_Account[MAX_ACCOUNT] = '\0' ;
}

GUID_t	DBCreateCharOp::GetCharGuid()
{
	return m_CharGuid;
}

VOID	DBCreateCharOp::SetCharGuid(GUID_t guid)
{
	m_CharGuid	 = guid;
}

VOID	DBCreateCharOp::SetCharName(const CHAR* pName)
{
	strncpy(m_CharName,pName,MAX_CHARACTER_NAME);
	m_CharName[MAX_CHARACTER_NAME] = '\0';
}

const CHAR* DBCreateCharOp::GetCharName()
{
	return m_CharName;
}

VOID DBCreateCharOp::SetSex(BOOL bSex)
{
	m_Sex = bSex;
}

BOOL DBCreateCharOp::GetSex()
{
	return m_Sex;
}

VOID	DBCreateCharOp::SetHairColor(BYTE Color)
{
	m_HairColor	= Color;
}
BYTE	DBCreateCharOp::GetHairColor()
{
	return m_HairColor;
}

VOID	DBCreateCharOp::SetFaceColor(BYTE Color)
{
	m_FaceColor	= Color;
}
BYTE	DBCreateCharOp::GetFaceColor()
{
	return m_FaceColor;
}

VOID	DBCreateCharOp::SetHairModel(BYTE Model)
{
	m_HairModel = Model;
}
BYTE	DBCreateCharOp::GetHairModel()
{
	return m_HairModel;
}

VOID	DBCreateCharOp::SetFaceModel(BYTE Model)
{
	m_FaceModel = Model;
}
BYTE	DBCreateCharOp::GetFaceModel()
{
	return m_FaceModel;
}

VOID	DBCreateCharOp::SetHeadID(BYTE Head)
{
	m_HeadID = Head;
}
BYTE	DBCreateCharOp::GetHeadID()
{
	return m_HeadID;
}

VOID	DBCreateCharOp::SetCampID(CampID_t Camp)
{
	m_Camp = Camp;
}
CampID_t	DBCreateCharOp::GetCampID()
{
	return m_Camp;
}

VOID	DBCreateCharOp::SetMenPaiID(MenPaiID_t MenPai)
{
	m_MenPai = MenPai;
}
CampID_t	DBCreateCharOp::GetMenPaiID()
{
	return m_MenPai;
}


