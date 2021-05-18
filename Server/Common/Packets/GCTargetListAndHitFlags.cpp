#include "stdafx.h"
// GCTargetListAndHitFlags.cpp
// 
/////////////////////////////////////////////////////

#include "GCTargetListAndHitFlags.h"

using namespace Packets;

BOOL GCTargetListAndHitFlags::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_nDataType), sizeof(m_nDataType));
	iStream.Read( (CHAR*)(&m_nObjID), sizeof(m_nObjID));
	iStream.Read( (CHAR*)(&m_nLogicCount), sizeof(m_nLogicCount));
	iStream.Read( (CHAR*)(&m_nSkillOrSpecialObjDataID), sizeof(m_nSkillOrSpecialObjDataID));
	iStream.Read( (CHAR*)(&m_posUser), sizeof(m_posUser));
	iStream.Read( (CHAR*)(&m_nTargetID), sizeof(m_nTargetID));
	iStream.Read( (CHAR*)(&m_posTarget), sizeof(m_posTarget));
	iStream.Read( (CHAR*)(&m_fDir), sizeof(m_fDir));
	iStream.Read( (CHAR*)(&m_nTargetNum), sizeof(m_nTargetNum));
	iStream.Read( (CHAR*)(m_HitFlagList.GetFlags()), m_HitFlagList.GetByteSize());
	for(INT nIdx=0; m_nTargetNum>nIdx; ++nIdx)
	{
		iStream.Read( (CHAR*)(&m_aTargetList[nIdx]), sizeof(m_aTargetList[nIdx]));
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCTargetListAndHitFlags::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_nDataType), sizeof(m_nDataType));
	oStream.Write( (CHAR*)(&m_nObjID), sizeof(m_nObjID));
	oStream.Write( (CHAR*)(&m_nLogicCount), sizeof(m_nLogicCount));
	oStream.Write( (CHAR*)(&m_nSkillOrSpecialObjDataID), sizeof(m_nSkillOrSpecialObjDataID));
	oStream.Write( (CHAR*)(&m_posUser), sizeof(m_posUser));
	oStream.Write( (CHAR*)(&m_nTargetID), sizeof(m_nTargetID));
	oStream.Write( (CHAR*)(&m_posTarget), sizeof(m_posTarget));
	oStream.Write( (CHAR*)(&m_fDir), sizeof(m_fDir));
	oStream.Write( (CHAR*)(&m_nTargetNum), sizeof(m_nTargetNum));
	oStream.Write( (CHAR*)(m_HitFlagList.GetFlags()), m_HitFlagList.GetByteSize());
	for(INT nIdx=0; m_nTargetNum>nIdx; ++nIdx)
	{
		oStream.Write( (CHAR*)(&m_aTargetList[nIdx]), sizeof(m_aTargetList[nIdx]));
	}
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCTargetListAndHitFlags::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCTargetListAndHitFlagsHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

