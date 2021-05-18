#include "stdafx.h"
// GCCharAllTitles.cpp
// 
/////////////////////////////////////////////////////

#include "GCCharAllTitles.h"

BOOL GCCharAllTitles::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_nTitleId), sizeof(BYTE));
	if( m_nTitleId>MAX_TITLE_ID_SIZE ) m_nTitleId=MAX_TITLE_ID_SIZE ;
	iStream.Read( (CHAR*)(&m_nTitleStr), sizeof(BYTE));
	if( m_nTitleStr>MAX_STRING_TITLE_SIZE ) m_nTitleStr=MAX_STRING_TITLE_SIZE ;
	
	for(int i =0;  i<m_nTitleId; i++)
	{
		iStream.Read( (CHAR*)(&m_TitleIdList[i].m_titleid), sizeof(BYTE));
		iStream.Read( (CHAR*)(&m_TitleIdList[i].m_titleData), sizeof(SHORT));
	}

	for(int i = 0; i<m_nTitleStr; i++)
	{
		iStream.Read( (CHAR*)(&(m_TitleStrList[i].m_titleid)), sizeof(BYTE));
		iStream.Read( (CHAR*)(&(m_TitleStrList[i].m_strlength)), sizeof(BYTE));
		iStream.Read( (CHAR*)(&(m_TitleStrList[i].m_szTitle)), sizeof(CHAR)*m_TitleStrList[i].m_strlength);
	}

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCCharAllTitles::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_nTitleId), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_nTitleStr), sizeof(BYTE));
	
	for(int i =0;  i<m_nTitleId; i++)
	{
		oStream.Write( (CHAR*)(&m_TitleIdList[i].m_titleid), sizeof(BYTE));
		oStream.Write( (CHAR*)(&m_TitleIdList[i].m_titleData), sizeof(SHORT));
	}

	for(int i = 0; i<m_nTitleStr; i++)
	{
		oStream.Write( (CHAR*)(&(m_TitleStrList[i].m_titleid)), sizeof(BYTE));
		oStream.Write( (CHAR*)(&(m_TitleStrList[i].m_strlength)), sizeof(BYTE));
		oStream.Write( (CHAR*)(&(m_TitleStrList[i].m_szTitle)), sizeof(CHAR)*m_TitleStrList[i].m_strlength);
	}

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCCharAllTitles::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GCCharAllTitlesHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

