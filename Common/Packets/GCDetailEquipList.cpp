#include "stdafx.h"
#include "GCDetailEquipList.h"

BOOL GCDetailEquipList::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	
	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
	iStream.Read((CHAR*)&m_Mode,sizeof(ASK_EQUIP_MODE));
	iStream.Read((CHAR*)&m_wPartFlags,sizeof(WORD));
	
	for(INT	i=0; i<HEQUIP_NUMBER; i++)
	{
		if( m_wPartFlags & (1<<i) ) 
		{
			m_ItemList[i].Read(iStream);

			if((m_ItemList[i].ItemClass()!=ICLASS_EQUIP)&&(!m_ItemList[i].IsNullType()))
			{
				Assert(FALSE);
			}
		}
	}
	
	return TRUE ;

	__LEAVE_FUNCTION
	
	return FALSE ;
}

BOOL GCDetailEquipList::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
	oStream.Write((CHAR*)&m_Mode,sizeof(ASK_EQUIP_MODE));
	oStream.Write((CHAR*)&m_wPartFlags,sizeof(WORD));

	

	for(INT	i=0; i<HEQUIP_NUMBER; i++)
	{
		if( m_wPartFlags & (1<<i) ) 
		{

			if((m_ItemList[i].ItemClass()==ICLASS_EQUIP))
			{
				m_ItemList[i].Write(oStream);
			}
			else
			{
				Assert(FALSE); //大概是往包里放数据写错了
			}
		}
	}
	

	  return TRUE ;
	__LEAVE_FUNCTION

	return FALSE ;
}

UINT	GCDetailEquipList::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
	
	return GCDetailEquipListHandler::Execute( this, pPlayer ) ;
	
	__LEAVE_FUNCTION
	return FALSE ;
}