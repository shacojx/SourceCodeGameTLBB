#include "stdafx.h"


#include "WorldManager.h"
#include "Config.h"


WorldManager* g_pWorldManager=NULL ;

WorldManager::WorldManager( )
{
__ENTER_FUNCTION
__LEAVE_FUNCTION
}

WorldManager::~WorldManager( )
{
__ENTER_FUNCTION
__LEAVE_FUNCTION
}

ID_t WorldManager::GetWorldID( )
{
__ENTER_FUNCTION

	return g_Config.m_WorldInfo.m_WorldID ;

__LEAVE_FUNCTION
	
	return INVALID_ID ;
}

BOOL WorldManager::Init( )
{
__ENTER_FUNCTION

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}