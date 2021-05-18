#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：CampAndStand.cpp
// 功能说明：阵营数据表结构和功能实现
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
/** \file CampAndStand.cpp
*		\brief the implement of camp and stand structure
*/
#include "CampAndStand.h"
#include "GameUtil.h"
#include "DB_Struct.h"
#include "TLBB_DBC.h"

namespace Combat_Module
{
	namespace Camp_Stand
	{
		using namespace DBC;

		//class CampStandRecord_T
		CampStandRecord_T::CampStandRecord_T() : m_bInited(FALSE)
		{
			memset((VOID*)m_aStands, (INT)FALSE, sizeof(m_aStands));
		}
		BOOL CampStandRecord_T::InitFromDB(const DBCFile * pDB, INT const nRow)
		{
			__ENTER_FUNCTION
			Assert(pDB);
			Assert(0<=nRow);
			INT column = 1;
			for(INT nIdx=0;MAX_ID>=nIdx;++nIdx)
			{
				m_aStands[nIdx]= pDB->Search_Posistion(nRow, column++)->iValue != 0;
			}
			m_bInited = TRUE;
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL CampStandRecord_T::StandByCampID(CampID_t const nCampID)const
		{
			__ENTER_FUNCTION
			if(0<=nCampID&&MAX_ID>=nCampID)
			{
				return m_aStands[nCampID];
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	}
}

//写死了的一些阵营逻辑，其实可以配表
//看看两个阵营，是否是敌对关系
//BOOL	CalcCamp_IfIsEnemy(CampID_t a, CampID_t b)
//{
//	if( a == -1 || b == -1 ) //阵营ID无效
//		return TRUE;
//
//	if( a == CAMP_1 )
//	{
//		if( b == CAMP_1 )		return FALSE;
//		else if( b == CAMP_2 )	return TRUE;
//		else if( b == CAMP_3 )	return FALSE;
//		else if( b == CAMP_4 )	return TRUE;
//	}
//	else if( a == CAMP_2 )
//	{
//		if( b == CAMP_1 )		return TRUE;
//		else if( b == CAMP_2 )	return FALSE;
//		else if( b == CAMP_3 )	return FALSE;
//		else if( b == CAMP_4 )	return TRUE;
//	}
//	else if( a == CAMP_3 )
//	{
//		return FALSE;
//	}
//	else if( a == CAMP_4 )
//	{
//		return TRUE;
//	}
//	else
//		Assert( FALSE && "怎么没有这个阵营类型呢？" ) ;
//
//	return FALSE;
//}
ENUM_RELATION CalcRelationType( INT nObj1Type, INT nObj2Type, CampAndStandDataMgr_T* pCampAndStandDataMgr )
{
	const CampStandRecord_T *pCampAndStand = pCampAndStandDataMgr->GetInstanceByID( nObj1Type );
	if( pCampAndStand == NULL )
		return RELATION_FRIEND;
	if( nObj2Type < 0 || nObj2Type > CampStandRecord_T::MAX_ID )
		return RELATION_FRIEND;

	if( pCampAndStand->StandByCampID(nObj2Type) )
		return RELATION_FRIEND;
	else
		return RELATION_ENEMY;







	//ENUM_RELATION eRelation_A, eRelation_B;

	//if ( pCamp_A == NULL || pCamp_B == NULL )
	//	return RELATION_ENEMY;

	//BOOL ret = CalcCamp_IfIsEnemy( pCamp_A->m_nCampID, pCamp_B->m_nCampID );
	//if( ret )
	//	return RELATION_ENEMY;
	//else
	//	return RELATION_FRIEND;



	//// 对A来说
	//if ( pCamp_A->IsActiveFlag_Dirty( pCamp_B->m_nCampID ) )
	//{
	//	if ( pCamp_A->IsRelationFlag_Dirty( pCamp_B->m_nCampID ) )
	//	{
	//		eRelation_A = RELATION_FRIEND;
	//	}
	//	else
	//	{
	//		eRelation_A = RELATION_ENEMY;
	//	}
	//}
	//else
	//{
	//	const CampStandRecord_T *pCampAndStand;
	//	pCampAndStand = pCampMgr->GetInstanceByID( pCamp_A->m_nCampID );
	//	if ( pCampAndStand != NULL )
	//	{
	//		if ( pCampAndStand->StandByCampID( pCamp_B->m_nCampID ) )
	//		{
	//			eRelation_A = RELATION_ENEMY;
	//		}
	//		else
	//		{
	//			eRelation_A = RELATION_FRIEND;
	//		}
	//	}
	//	else
	//	{
	//		eRelation_A = RELATION_INVALID;
	//	}
	//}

	//// 对B来说
	//if ( pCamp_B->IsActiveFlag_Dirty( pCamp_A->m_nCampID ) )
	//{
	//	if ( pCamp_B->IsRelationFlag_Dirty( pCamp_A->m_nCampID ) )
	//	{
	//		eRelation_B = RELATION_FRIEND;
	//	}
	//	else
	//	{
	//		eRelation_B = RELATION_ENEMY;
	//	}
	//}
	//else
	//{
	//	const CampStandRecord_T *pCampAndStand;
	//	pCampAndStand = pCampMgr->GetInstanceByID( pCamp_B->m_nCampID );
	//	if ( pCampAndStand != NULL )
	//	{
	//		if ( pCampAndStand->StandByCampID( pCamp_A->m_nCampID ) )
	//		{
	//			eRelation_B = RELATION_ENEMY;
	//		}
	//		else
	//		{
	//			eRelation_B = RELATION_FRIEND;
	//		}
	//	}
	//	else
	//	{
	//		eRelation_B = RELATION_INVALID;
	//	}
	//}

	//if ( eRelation_A == eRelation_B )
	//{
	//	return eRelation_A;
	//}
	//else
	//{
	//	if ( bHuman_A && bHuman_B )
	//	{
	//		return RELATION_ENEMY;
	//	}
	//	else if ( bHuman_A )
	//	{
	//		return eRelation_A;
	//	}
	//	else if ( bHuman_B )
	//	{
	//		return eRelation_B;
	//	}
	//	else
	//	{
	//		return RELATION_ENEMY;
	//	}
	//}
}
