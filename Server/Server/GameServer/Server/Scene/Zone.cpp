#include "stdafx.h"
// Zone.cpp

#include "Obj_Human.h"

#include "Zone.h"


Zone::Zone( )
{
__ENTER_FUNCTION

	CleanUp();

__LEAVE_FUNCTION
}

Zone::~Zone( )
{
}

VOID Zone::CleanUp()
{
__ENTER_FUNCTION

	m_HumanList.CleanUp() ;
	m_ObjList.CleanUp() ;
	m_AreaCount = 0;
	memset((VOID*)m_apArea, 0, sizeof(Area*) * MAX_AREA_IN_ZONE);

__LEAVE_FUNCTION
}

VOID Zone::OnObjectEnter( Obj *pObj )
{
__ENTER_FUNCTION

	Obj::ObjType eObjType = pObj->GetObjType();
	switch ( eObjType )
	{
	case Obj::OBJ_TYPE_HUMAN:
	case Obj::OBJ_TYPE_MONSTER:
	case Obj::OBJ_TYPE_PET:
	case Obj::OBJ_TYPE_ITEM_BOX:
	case Obj::OBJ_TYPE_PLATFORM:
	case Obj::OBJ_TYPE_SPECIAL:
		{
			if(Obj::OBJ_TYPE_HUMAN == eObjType)
			{
				Obj_Human *pHuman = (Obj_Human*)pObj;
				m_HumanList.AddNode( pHuman->HumanNode() );
			}
			m_ObjList.AddNode( pObj->ObjNode() );
		}
		break;
	default:
		Assert( FALSE && "Zone::OnObjectEnter Unknow obj type" );
		break;
	}

__LEAVE_FUNCTION
}

VOID Zone::OnObjectLeave( Obj *pObj )
{
__ENTER_FUNCTION

	Obj::ObjType eObjType = pObj->GetObjType();
	switch ( eObjType )
	{
	case Obj::OBJ_TYPE_HUMAN:
	case Obj::OBJ_TYPE_MONSTER:
	case Obj::OBJ_TYPE_PET:
	case Obj::OBJ_TYPE_ITEM_BOX:
	case Obj::OBJ_TYPE_PLATFORM:
	case Obj::OBJ_TYPE_SPECIAL:
		{
			if(Obj::OBJ_TYPE_HUMAN == eObjType)
			{
				Obj_Human *pHuman = (Obj_Human*)pObj;
				m_HumanList.DeleteNode( pHuman->HumanNode() );
			}
			m_ObjList.DeleteNode( pObj->ObjNode() );
		}
		break;
	default:
		Assert( FALSE && "Zone::OnObjectLeave Unknow obj type" );
		break;
	}

__LEAVE_FUNCTION
}

const Area* Zone::GetCurrentArea( const WORLD_POS* pWorldPos )
{
__ENTER_FUNCTION

	FLOAT x, z;
	x = pWorldPos->m_fX;
	z = pWorldPos->m_fZ;

	for( INT i = 0; i < m_AreaCount; ++i )
	{
		if( m_apArea[i] != NULL && m_apArea[i]->IsContain(x, z) )
		{
			return m_apArea[i];
		}
	}

__LEAVE_FUNCTION

	return NULL;
}
