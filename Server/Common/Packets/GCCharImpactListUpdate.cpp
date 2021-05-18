#include "stdafx.h"
// GCCharImpactListUpdate.cpp
// 
/////////////////////////////////////////////////////

#include "GCCharImpactListUpdate.h"

using namespace Packets;

BOOL GCCharImpactListUpdate::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION
	CleanUp();
	iStream.Read((CHAR*)(&m_nOwnerID), sizeof(m_nOwnerID));
	iStream.Read((CHAR*)(&m_nNumOfImpacts), sizeof(m_nNumOfImpacts));
	if ( m_nNumOfImpacts > 0 )
	{
		m_nNumOfImpacts>MAX_IMPACT_NUM?m_nNumOfImpacts=MAX_IMPACT_NUM:NULL;
		for(INT i=0; m_nNumOfImpacts>i; ++i)
		{
			iStream.Read((CHAR*)(&m_aImpact[i]), sizeof(GCCharImpactListUpdate::ImpactStruct_T));
		}
	}
	return TRUE ;
__LEAVE_FUNCTION
	return FALSE ;
}

BOOL GCCharImpactListUpdate::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION
	oStream.Write((CHAR*)(&m_nOwnerID), sizeof(m_nOwnerID));
	oStream.Write((CHAR*)(&m_nNumOfImpacts), sizeof(m_nNumOfImpacts));
	for(SHORT i=0; m_nNumOfImpacts>i; ++i)
	{
		oStream.Write((CHAR*)(&m_aImpact[i]), sizeof(m_aImpact[i]));
	}
	return TRUE ;
__LEAVE_FUNCTION
	return FALSE ;
}
VOID GCCharImpactListUpdate::SetImpactList(_IMPACT_LIST const& rList)
{
__ENTER_FUNCTION
	m_nNumOfImpacts = rList.m_Count;
	if(MAX_IMPACT_NUM<m_nNumOfImpacts)
	{
		m_nNumOfImpacts=MAX_IMPACT_NUM;
	}
	for(INT nIdx=0;m_nNumOfImpacts>nIdx;++nIdx)
	{
		m_aImpact[nIdx].m_nBuffID = rList.m_aImpacts[nIdx].GetImpactID();
		//m_aImpact[nIdx].m_nSN = rList.m_aImpacts[nIdx].m_nSN;
		//m_aImpact[nIdx].m_nContinuance = rList.m_aImpacts[nIdx].m_nContinuance;
	}
__LEAVE_FUNCTION
}
UINT GCCharImpactListUpdate::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCCharImpactListUpdateHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

