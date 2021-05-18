#include "stdafx.h"

#include "LCRetCharList.h"


BOOL LCRetCharList::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	
	iStream.Read((CHAR*)(&Result),sizeof(ASKCHARLIST_RESULT));
	
	if(Result == ASKCHARLIST_SUCCESS)
	{
		iStream.Read( (CHAR*)(&szAccount), sizeof(CHAR)*MAX_ACCOUNT);
		iStream.Read((CHAR*)(&uCharNumber),sizeof(UCHAR));
		if(uCharNumber>DB_CHAR_NUMBER) uCharNumber = DB_CHAR_NUMBER;

		for(UINT i =0;i<uCharNumber;i++)
		{
			iStream.Read((CHAR*)(&CharList[i]),sizeof(DB_CHAR_BASE_INFO));
		}
	}
	
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL LCRetCharList::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write((CHAR*)(&Result),sizeof(ASKCHARLIST_RESULT));

	if(Result == ASKCHARLIST_SUCCESS)
	{
		oStream.Write( (CHAR*)(&szAccount), sizeof(CHAR)*MAX_ACCOUNT);
		
		if(uCharNumber>DB_CHAR_NUMBER)
		{
			Assert(FALSE);
			UCHAR RealNumber = DB_CHAR_NUMBER;
			oStream.Write((CHAR*)(&RealNumber),sizeof(UCHAR));
			for(UINT i =0;i<RealNumber;i++)
			{	
				oStream.Write((CHAR*)(&CharList[i]),sizeof(DB_CHAR_BASE_INFO));
			}
		}
		else
		{
			oStream.Write((CHAR*)(&uCharNumber),sizeof(UCHAR));
			for(UINT i =0;i<uCharNumber;i++)
			{	
				oStream.Write((CHAR*)(&CharList[i]),sizeof(DB_CHAR_BASE_INFO));
			}
		}
	}
		
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT LCRetCharList::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return	LCRetCharListHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

ASKCHARLIST_RESULT	LCRetCharList::GetResult() const
{
	return Result;
}

const	CHAR*	LCRetCharList::GetAccount()	const
{
	return szAccount;
}
VOID			LCRetCharList::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(szAccount,pAccount,MAX_ACCOUNT*sizeof(CHAR));
	szAccount[MAX_ACCOUNT] = '\0' ;
}

VOID LCRetCharList::SetResult(ASKCHARLIST_RESULT result)
{
	Result = result;
}
