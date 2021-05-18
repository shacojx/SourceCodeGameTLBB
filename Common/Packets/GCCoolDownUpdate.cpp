#include "stdafx.h"
// GCCoolDownUpdate.cpp
// 
/////////////////////////////////////////////////////

#include "GCCoolDownUpdate.h"

using namespace Packets;

BOOL GCCooldownUpdate::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION
	CleanUp();
	iStream.Read( (CHAR*)(&m_guidPet), sizeof(m_guidPet));
	iStream.Read( (CHAR*)(&m_nNumCooldown), sizeof(m_nNumCooldown));
	if ( m_nNumCooldown > 0 )
	{
		Cooldown_T CooldownInfo;
		for(INT nIdx=0; m_nNumCooldown>nIdx; ++nIdx)
		{
			iStream.Read((CHAR*)(&CooldownInfo), sizeof(CooldownInfo));
			m_aCooldowns[nIdx] = CooldownInfo;
		}
	}
	return TRUE ;
__LEAVE_FUNCTION
	return FALSE ;
}

BOOL GCCooldownUpdate::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_guidPet), sizeof(m_guidPet));
	oStream.Write( (CHAR*)(&m_nNumCooldown), sizeof(m_nNumCooldown));
	for(SHORT nIdx=0; LIST_SIZE>nIdx; ++nIdx)
	{
		if(-1 != m_aCooldowns[nIdx].GetID())
		{
			oStream.Write( (CHAR*)(&(m_aCooldowns[nIdx])), sizeof(m_aCooldowns[nIdx]));	
		}
	}
	return TRUE ;
__LEAVE_FUNCTION
	return FALSE ;
}
VOID	GCCooldownUpdate::CalcNumCooldown(VOID) const
{
__ENTER_FUNCTION
	SHORT* pCount = (SHORT*)&m_nNumCooldown;
	*pCount = 0;
	for(INT nIdx=0; LIST_SIZE>nIdx; ++nIdx)
	{
		if(-1 != m_aCooldowns[nIdx].GetID())
		{
			++(*pCount);
		}
	}
__LEAVE_FUNCTION
}
BOOL	GCCooldownUpdate::CooldownModified(Cooldown_T const& rCooldown)
{
	__ENTER_FUNCTION
	if(INVALID_ID==rCooldown.GetID())
	{
		return TRUE;
	}
	for(INT nIdx=0; LIST_SIZE>nIdx; ++nIdx)
	{
		if(INVALID_ID==m_aCooldowns[nIdx].GetID() || rCooldown.GetID()==m_aCooldowns[nIdx].GetID())
		{
			m_aCooldowns[nIdx] = rCooldown;
			return TRUE;
		}
	}
	AssertEx(FALSE, "[GCCooldownUpdate::CooldownModified]: List Over flow!! check it.");
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}
BOOL	GCCooldownUpdate::CooldownModified(CooldownID_t nID, Time_t nTotalCooldown)
{
	__ENTER_FUNCTION
	if(INVALID_ID==nID)
	{
		return TRUE;
	}
	Cooldown_T cooldownInfo;
	cooldownInfo.Reset();
	cooldownInfo.SetID(nID);
	cooldownInfo.SetCooldownTime(nTotalCooldown);
	return CooldownModified(cooldownInfo);
	__LEAVE_FUNCTION
	return FALSE;
}

VOID	GCCooldownUpdate::CleanUp(VOID)
{
	__ENTER_FUNCTION
	m_guidPet.Reset();
	m_nNumCooldown =0;
	for(INT nIdx=0;LIST_SIZE>nIdx;++nIdx)
	{
		m_aCooldowns[nIdx].Reset();
	}
	__LEAVE_FUNCTION
}
UINT GCCooldownUpdate::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCCooldownUpdateHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

