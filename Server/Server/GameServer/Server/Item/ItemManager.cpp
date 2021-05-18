#include "stdafx.h"


#include "ItemManager.h"
#include "Type.h"
#include "Ini.h"
#include "WorldManager.h"
#include "ServerManager.h"
#include "ItemRuler.h"
#include "ItemTable.h"
#include "ItemHelper.h"
#include "ShareMemManager.h"
#include "SMUManager.h"
#include "FileDef.h"

ItemManager*	g_pItemManager = NULL;


BOOL	ItemManager::Init()
{
	BOOL bEnableShareMem = g_pServerManager->GetCurrentServerInfo()->m_EnableShareMem;
	if(bEnableShareMem)
	{
		ItemSerialKeySMU* pSMU =	g_ItemSerialSMUPool.NewObj();
		Assert(pSMU);
		m_pSerialSMU = pSMU;
		m_nLastGenItemSerial =  m_pSerialSMU->GetSerial(SM_S_READ);
	}
	else
	{
		Ini	SerialFile( FILE_ITEM_SERIAL );
		m_nLastGenItemSerial =	SerialFile.ReadInt("ItemConfig","ItemSerial");
		CHAR buf[255];
		sprintf(buf,"%d",m_nLastGenItemSerial+1000);
		SerialFile.Write("ItemConfig","ItemSerial",buf);
		SerialFile.Save( FILE_ITEM_SERIAL );
		
		m_pSerialSMU = NULL;
	}

	m_WorldId	=  g_pWorldManager->GetWorldID();
	m_ServerId	=	g_pServerManager->GetServerID();


	return TRUE;
}


INT	ItemManager::GenItemSerial()
{
	__ENTER_FUNCTION

	m_Lock.Lock();
	
	INT iSerialId = ++m_nLastGenItemSerial;
	
	m_Lock.Unlock();

	if(m_pSerialSMU)
	{
		m_pSerialSMU->SetSerial(SM_S_WRITE,iSerialId);
	}
	return	iSerialId;
	__LEAVE_FUNCTION

	m_Lock.Unlock();
	return -1;
}

BOOL	ItemManager::GenItemGuid(_ITEM_GUID& ig)
{
	
	__ENTER_FUNCTION
	
	ig.m_Server		= (BYTE)m_ServerId;
	ig.m_World		= (BYTE)m_WorldId;
	ig.m_Serial		= GenItemSerial();
	
	return TRUE;
	
	__LEAVE_FUNCTION

	return FALSE;
}


BOOL	ItemManager::_CreateEquipFromTable(ITEM_LOG_PARAM*	pLogParam,_ITEM_TYPE  it,INT iQuality,_ITEM& item)
{
	__ENTER_FUNCTION
	Assert(pLogParam);
	ItemCreateRuler::CreateEquipAttrib(pLogParam, it,0,item,iQuality);
	if(g_pItemManager->GenItemGuid(item.m_ItemGUID))
	{
		return TRUE;
	}

	__LEAVE_FUNCTION
	return FALSE;
}

BOOL	ItemManager::_CreateGemFromTable(_ITEM_TYPE it,_ITEM&	item)
{
	

	__ENTER_FUNCTION

	ItemCreateRuler::CreateGemAttrib(it,item);
	
	return g_pItemManager->GenItemGuid(item.m_ItemGUID);
	
	__LEAVE_FUNCTION
	
	return FALSE;
}
BOOL	ItemManager::_CreateGemFromTable(UINT TableIndex,_ITEM&	item)
{
	

	__ENTER_FUNCTION

	ItemCreateRuler::CreateGemAttrib(TableIndex,item);
	return g_pItemManager->GenItemGuid(item.m_ItemGUID);
	

	__LEAVE_FUNCTION
	
	return FALSE;
}


BOOL	ItemManager::_CreateMedicFromTable(_ITEM_TYPE it,_ITEM& item)
{
	
	__ENTER_FUNCTION

	ItemCreateRuler::CreateMedicAttrib(it,item);
	return g_pItemManager->GenItemGuid(item.m_ItemGUID);

	__LEAVE_FUNCTION
	
	return FALSE;
}

/*
 *	根据_ITEM_TYPE结构	创建藏宝图
 */
BOOL	ItemManager::_CreateStoreMapFromTable(_ITEM_TYPE it,_ITEM& item)
{
	
	__ENTER_FUNCTION

	ItemCreateRuler::CreateStoreMapAttrib(it,item);
	return g_pItemManager->GenItemGuid(item.m_ItemGUID);

	__LEAVE_FUNCTION
	
	return FALSE;
}


BOOL	ItemManager::CreateItem(ITEM_LOG_PARAM* pLogParam,uint ItemSerial, _ITEM& item, INT iQuality/* =1 */)
{
	__ENTER_FUNCTION
	
	Assert(pLogParam);
	TSerialHelper	help(ItemSerial);
	BOOL bRet	= CreateItem(pLogParam,help.GetItemTypeStruct(),item,iQuality);
	pLogParam->ItemGuid = item.m_ItemGUID;
	return bRet;
	__LEAVE_FUNCTION


	return FALSE;
}

BOOL	ItemManager::CreateItem(ITEM_LOG_PARAM* pLogParam,_ITEM_TYPE it,_ITEM& item,INT iQuality/* =1 */)
{

	__ENTER_FUNCTION
	Assert(pLogParam);

	Assert(it.isNull() == FALSE);
	switch(it.m_Class)
	{
	case ICLASS_EQUIP:
		return _CreateEquipFromTable(pLogParam,it, iQuality,item);
		break;
	case ICLASS_MATERIAL: 
		return _CreateMedicFromTable(it,item);
		break;
	case ICLASS_COMITEM:
		return _CreateMedicFromTable(it,item);
		break;
	case ICLASS_TASKITEM: 
		return _CreateMedicFromTable(it,item);
		break;
	case ICLASS_GEM:
		return _CreateGemFromTable(it,item);
		break;
	case ICLASS_STOREMAP:
		return _CreateStoreMapFromTable(it,item);
		break;
	case ICLASS_TALISMAN: 
		Assert(FALSE);
		break;
	case ICLASS_GUILDITEM: 
		Assert(FALSE);
		break;
	default:
		Assert(FALSE);
		return FALSE;
	}
	Assert(item.IsNullType() == FALSE); 

	__LEAVE_FUNCTION

	return FALSE;
}

