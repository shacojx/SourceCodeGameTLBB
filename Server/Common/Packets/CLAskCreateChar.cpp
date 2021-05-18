#include "stdafx.h"

#include "CLAskCreateChar.h"

BOOL CLAskCreateChar::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	
		iStream.Read((CHAR*)m_Name,sizeof(CHAR)*MAX_CHARACTER_NAME);
		iStream.Read((CHAR*)&m_Sex,sizeof(BOOL));
		iStream.Read((CHAR*)&m_HairColor,sizeof(BYTE));
		iStream.Read((CHAR*)&m_FaceColor,sizeof(BYTE));
		iStream.Read((CHAR*)&m_HairModel,sizeof(BYTE));
		iStream.Read((CHAR*)&m_FaceModel,sizeof(BYTE));
		iStream.Read((CHAR*)&m_HeadID,sizeof(BYTE));
		iStream.Read((CHAR*)&m_GUID,sizeof(GUID_t));
		iStream.Read((CHAR*)&m_Camp,sizeof(CampID_t));
		iStream.Read((CHAR*)&m_MenPai,sizeof(MenPaiID_t));
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CLAskCreateChar::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write((CHAR*)m_Name,sizeof(CHAR)*MAX_CHARACTER_NAME);
		oStream.Write((CHAR*)&m_Sex,sizeof(BOOL));
		oStream.Write((CHAR*)&m_HairColor,sizeof(BYTE));
		oStream.Write((CHAR*)&m_FaceColor,sizeof(BYTE));
		oStream.Write((CHAR*)&m_HairModel,sizeof(BYTE));
		oStream.Write((CHAR*)&m_FaceModel,sizeof(BYTE));
		oStream.Write((CHAR*)&m_HeadID,sizeof(BYTE));
		oStream.Write((CHAR*)&m_GUID,sizeof(GUID_t));
		oStream.Write((CHAR*)&m_Camp,sizeof(CampID_t));
		oStream.Write((CHAR*)&m_MenPai,sizeof(CampID_t));
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CLAskCreateChar::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CLAskCreateCharHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


const	CHAR*	CLAskCreateChar::GetAccount()	const
{
	return szAccount;
}
VOID			CLAskCreateChar::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(szAccount,pAccount,MAX_ACCOUNT*sizeof(CHAR));
	szAccount[MAX_ACCOUNT] = '\0' ;
}