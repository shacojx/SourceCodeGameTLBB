#include "stdafx.h"
#include "DBMailInfo.h"
#include "ODBCInterface.h"
#include "SqlTemplate.h"
#include "DB_Struct.h"
#include "DBManager.h"
#include "GameStruct.h"
#include "SMUManager.h"

DBMailInfo::DBMailInfo(ODBCInterface* pInterface)
{
	mDBName =		CHARACTER_DATABASE;
	mResult			= 0;
	mResultCount	= 0;
	Assert(pInterface);
	mInterface		=	pInterface;
}

BOOL DBMailInfo::Load()
{
	__ENTER_FUNCTION


	DB_QUERY* pQuery = GetInternalQuery();
	if(!pQuery)
	{
		Assert(FALSE);
	}
	pQuery->Clear();

	pQuery->Parse(LoadWorldMailInfo,MAIL_TABLE);

	return ODBCBase::Load();

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL DBMailInfo::Save(VOID* pSource)
{
	__ENTER_FUNCTION

	enum 
	{
		DB_pIndex	=	1,
		DB_Sender,
		DB_Reciever,
		DB_MailInfo,
		DB_MailCont,
	};

	INT Result;
	
	SMUPool<MailSMU>* pPoolPtr = static_cast<SMUPool<MailSMU>*>(pSource);
	Assert(pPoolPtr);
	//保存Mail数据
	do
	{
		INT SmuCount = pPoolPtr->GetPoolMaxSize();
		if(SmuCount>MAX_MAILNODE_SIZE)
		Assert(FALSE);


		MAIL::MailInfo	mInfo;
		CHAR SourName[MAX_CHARACTER_NAME];
		CHAR DestName[MAX_CHARACTER_NAME];
		CHAR Context[MAX_MAIL_CONTEX];

		INT UseStats;

		for(INT i = 0 ;i<SmuCount;i++)
		{
			MailSMU*	pSMU = pPoolPtr->GetPoolObj(i);
			if(!pSMU)
			{
				Assert(pSMU);
				return FALSE;
			}

			UseStats =pSMU->GetUseStatus(SM_C_READ);
			if(UseStats!=MAIL_SAVED)
			{
				pSMU->Lock(SM_C_READ);
				MAIL* pMail = &(pSMU->m_MailSM);
				if(pMail == NULL)
				{
					pSMU->UnLock(SM_C_READ);
					Assert(FALSE);
				}
				pSMU->m_MailSM.GetMailInfo(mInfo);
				memcpy(SourName,pSMU->m_MailSM.m_szSourName,MAX_CHARACTER_NAME);
				memcpy(DestName,pSMU->m_MailSM.m_szDestName,MAX_CHARACTER_NAME);
				memcpy(Context,pSMU->m_MailSM.m_szContex,MAX_MAIL_CONTEX);
				pSMU->m_SMUHead.UseStatus	= MAIL_SAVED;
				pSMU->UnLock(SM_C_READ);
				
			}
			else
			{
				continue;
			}

			if(DestName[0]=='\0'&&UseStats!=MAIL_DELETE)
			{
				continue;
			}
		
			CHAR	szMailInfo[200];
			memset(szMailInfo,0,200);
			Binary2String((CHAR*)(&mInfo),
							sizeof(mInfo),
							szMailInfo);
			DB_QUERY* pQuery = GetInternalQuery();
			if(!pQuery)
			{
				Assert(FALSE);
			}
			pQuery->Clear();
			pQuery->Parse(SaveWorldMailInfo,
						  SourName,
						  DestName,
						  szMailInfo,
						  Context,
						  i,
						  ((UseStats!=MAIL_DELETE)?1:0) );

			if(!ODBCBase::Save(&Result))
				return FALSE;
		}

	}while(0);
	
	
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL DBMailInfo::Delete()
{
	__ENTER_FUNCTION

	DB_QUERY* pQuery = GetInternalQuery();

	if(!pQuery)
	{
		Assert(FALSE);
	}

	pQuery->Clear();

	pQuery->Parse(DeleteWorldMailInfo);

	return ODBCBase::Delete();

	__LEAVE_FUNCTION

	return FALSE;
}





BOOL DBMailInfo::ParseResult(VOID* pResult)
{
	__ENTER_FUNCTION

	switch(mOPType)
	{
	case DB_LOAD:
		{
			SMUPool<MailSMU>* pPoolPtr = static_cast<SMUPool<MailSMU>*>(pResult);
			Assert(pPoolPtr);
			enum 
			{
				DB_pIndex	=	1,
				DB_Sender,
				DB_Reciever,
				DB_MailInfo,
				DB_MailCont,
			};
			//加载Email属性
			Assert(mInterface);
			INT ErrorCode;
			INT	SmuCount = pPoolPtr->GetPoolMaxSize();
			INT pIndex;
			for(INT i =0;i<MAX_MAILNODE_SIZE;i++)
			{
				if(!mInterface->Fetch())
					break;

				if(i>= SmuCount)
					Assert(FALSE);

				pIndex = mInterface->GetUInt(DB_pIndex,ErrorCode);

				if(pIndex>= SmuCount)
					Assert(FALSE);

				MailSMU*	pSMU = pPoolPtr->GetPoolObj(pIndex);
				if(!pSMU)
				{
					Assert(pSMU);
					return FALSE;
				}
				MAIL* pMail = &pSMU->m_MailSM;
				Assert(pMail);
				
				mInterface->GetString(DB_Sender,pMail->m_szSourName,MAX_CHARACTER_NAME,ErrorCode);
				mInterface->GetString(DB_Reciever,pMail->m_szDestName,MAX_CHARACTER_NAME,ErrorCode);
				mInterface->GetString(DB_MailCont,pMail->m_szContex,MAX_MAIL_CONTEX,ErrorCode);
				MAIL::MailInfo mailInfo;
				mInterface->GetField(DB_MailInfo,(CHAR*)(&mailInfo),sizeof(mailInfo),ErrorCode);
				pMail->SetMailInfo(mailInfo);

				pSMU->SetUseStatus(MAIL_SAVED,SM_C_WRITE);
			}
			mInterface->Clear();
		}
		break;
	case DB_DELETE:
		{
	
		}
		break;
	default:
		break;
	}
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
