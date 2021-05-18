#include "stdafx.h"


#include "MachineManager.h"
#include "ServerManager.h"

MachineManager* g_pMachineManager=NULL ;

MachineManager::MachineManager( )
{
__ENTER_FUNCTION
__LEAVE_FUNCTION
}

MachineManager::~MachineManager( )
{
__ENTER_FUNCTION
__LEAVE_FUNCTION
}


BOOL MachineManager::Init( )
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

ID_t MachineManager::GetMachineID( )
{
__ENTER_FUNCTION

	return g_pServerManager->GetCurrentServerInfo()->m_MachineID ;

__LEAVE_FUNCTION

	return INVALID_ID ;
}

ID_t MachineManager::GetMachineID( ID_t ServerID )
{
__ENTER_FUNCTION

	if( ServerID<0 || ServerID >= MAX_SERVER )
		return INVALID_ID ;

	return g_pServerManager->FindServerInfo(ServerID)->m_MachineID ;

__LEAVE_FUNCTION

	return INVALID_ID ;
}

_MACHINE_DATA* MachineManager::GetMachineInfo( )
{
__ENTER_FUNCTION

	ID_t mid = GetMachineID( ) ;
	for( uint i=0; i<g_Config.m_MachineInfo.m_MachineCount; i++ )
	{
		if( g_Config.m_MachineInfo.m_pMachine[i].m_MachineID == mid )
			return &(g_Config.m_MachineInfo.m_pMachine[i]) ;
	}

__LEAVE_FUNCTION

	return NULL ;
}

_MACHINE_DATA* MachineManager::GetMachineInfo( ID_t MachineID )
{
__ENTER_FUNCTION

	for( uint i=0; i<g_Config.m_MachineInfo.m_MachineCount; i++ )
	{
		if( g_Config.m_MachineInfo.m_pMachine[i].m_MachineID == MachineID )
			return &(g_Config.m_MachineInfo.m_pMachine[i]) ;
	}

__LEAVE_FUNCTION

	return NULL ;
}

