#include "stdafx.h"
// DiePenaltyManager.cpp

#include "TLBB_DBC.h"
#include "FileDef.h"
#include "DiePenaltyManager.h"

DiePenaltyManager* g_pDiePenaltyManager=NULL ;

DiePenaltyManager::DiePenaltyManager()
{
__ENTER_FUNCTION

	m_nMaxID			= -1;
	m_papPenaltyInfo	= NULL;

__LEAVE_FUNCTION
}

DiePenaltyManager::~DiePenaltyManager()
{
__ENTER_FUNCTION

	Assert( m_papPenaltyInfo == NULL && "DiePenaltyManager::~DiePenaltyManager" );

__LEAVE_FUNCTION
}

BOOL DiePenaltyManager::Init( INT nMaxID )
{
__ENTER_FUNCTION

	if ( nMaxID < 0 )
	{
		Assert( nMaxID >= 0 && "DiePenaltyManager::Init" );
		return FALSE;
	}

	DBC::DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_DIE_PENALTY );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	Assert(iTableCount <= MAX_XINFA_LEVEL_NUM);

	m_nMaxID = nMaxID;
	m_papPenaltyInfo = new _DIE_PENALTY_INFO* [m_nMaxID];
	memset( m_papPenaltyInfo, 0, sizeof( _DIE_PENALTY_INFO* ) * m_nMaxID );

	const CHAR *pszTemp;
	INT nTempID, nTempSize;
	for(INT i=0; i<iTableCount; ++i)
	{
		nTempID = ThirdFile.Search_Posistion(i,0)->iValue;
		if ( nTempID < m_nMaxID )
		{
			m_papPenaltyInfo[nTempID] = new _DIE_PENALTY_INFO;
			_DIE_PENALTY_INFO *pInfo = m_papPenaltyInfo[nTempID];

			// MoneyMin
			pszTemp = ThirdFile.Search_Posistion(i,2)->pString;
			nTempSize = (INT)(strlen(pszTemp) - 1);
			pInfo->m_bPercent_MoneyMin = (pszTemp[nTempSize]=='%')?(TRUE):(FALSE);
			if ( pInfo->m_bPercent_MoneyMin )
			{
				pInfo->m_nPercent_MoneyMin	= atoi( &pszTemp[0] );
			}
			else
			{
				pInfo->m_nMoneyMin			= atoi( pszTemp );
			}

			// MoneyMax
			pszTemp = ThirdFile.Search_Posistion(i,3)->pString;
			nTempSize = (INT)(strlen(pszTemp) - 1);
			pInfo->m_bPercent_MoneyMax = (pszTemp[nTempSize]=='%')?(TRUE):(FALSE);
			if ( pInfo->m_bPercent_MoneyMax )
			{
				pInfo->m_nPercent_MoneyMax	= atoi( &pszTemp[0] );
			}
			else
			{
				pInfo->m_nMoneyMax			= atoi( pszTemp );
			}

			// ExpMin
			pszTemp = ThirdFile.Search_Posistion(i,4)->pString;
			nTempSize = (INT)(strlen(pszTemp) - 1);
			pInfo->m_bPercent_ExpMin = (pszTemp[nTempSize]=='%')?(TRUE):(FALSE);
			if ( pInfo->m_bPercent_ExpMin )
			{
				pInfo->m_nPercent_ExpMin	= atoi( &pszTemp[0] );
			}
			else
			{
				pInfo->m_nExpMin			= atoi( pszTemp );
			}

			// ExpMax
			pszTemp = ThirdFile.Search_Posistion(i,5)->pString;
			nTempSize = (INT)(strlen(pszTemp) - 1);
			pInfo->m_bPercent_ExpMax = (pszTemp[nTempSize]=='%')?(TRUE):(FALSE);
			if ( pInfo->m_bPercent_ExpMax )
			{
				pInfo->m_nPercent_ExpMax	= atoi( &pszTemp[0] );
			}
			else
			{
				pInfo->m_nExpMax			= atoi( pszTemp );
			}

			// EquipDurMin
			pszTemp = ThirdFile.Search_Posistion(i,6)->pString;
			nTempSize = (INT)(strlen(pszTemp) - 1);
			pInfo->m_bPercent_EquipDurMin = (pszTemp[nTempSize]=='%')?(TRUE):(FALSE);
			if ( pInfo->m_bPercent_EquipDurMin )
			{
				pInfo->m_nPercent_EquipDurMin	= atoi( &pszTemp[0] );
			}
			else
			{
				pInfo->m_nEquipDurMin			= atoi( pszTemp );
			}

			// EquipDurMax
			pszTemp = ThirdFile.Search_Posistion(i,7)->pString;
			nTempSize = (INT)(strlen(pszTemp) - 1);
			pInfo->m_bPercent_EquipDurMax = (pszTemp[nTempSize]=='%')?(TRUE):(FALSE);
			if ( pInfo->m_bPercent_EquipDurMax )
			{
				pInfo->m_nPercent_EquipDurMax	= atoi( &pszTemp[0] );
			}
			else
			{
				pInfo->m_nEquipDurMax			= atoi( pszTemp );
			}

			// ItemDropMin
			pszTemp = ThirdFile.Search_Posistion(i,8)->pString;
			nTempSize = (INT)(strlen(pszTemp) - 1);
			pInfo->m_bPercent_ItemDropMin = (pszTemp[nTempSize]=='%')?(TRUE):(FALSE);
			if ( pInfo->m_bPercent_ItemDropMin )
			{
				pInfo->m_nPercent_ItemDropMin	= atoi( &pszTemp[0] );
			}
			else
			{
				pInfo->m_nItemDropMin			= atoi( pszTemp );
			}

			// ItemDropMax
			pszTemp = ThirdFile.Search_Posistion(i,9)->pString;
			nTempSize = (INT)(strlen(pszTemp) - 1);
			pInfo->m_bPercent_ItemDropMax = (pszTemp[nTempSize]=='%')?(TRUE):(FALSE);
			if ( pInfo->m_bPercent_ItemDropMax )
			{
				pInfo->m_nPercent_ItemDropMax	= atoi( &pszTemp[0] );
			}
			else
			{
				pInfo->m_nItemDropMax			= atoi( pszTemp );
			}

			// EquipDropMin
			pszTemp = ThirdFile.Search_Posistion(i,10)->pString;
			nTempSize = (INT)(strlen(pszTemp) - 1);
			pInfo->m_bPercent_EquipDropMin = (pszTemp[nTempSize]=='%')?(TRUE):(FALSE);
			if ( pInfo->m_bPercent_EquipDropMin )
			{
				pInfo->m_nPercent_EquipDropMin	= atoi( &pszTemp[0] );
			}
			else
			{
				pInfo->m_nEquipDropMin			= atoi( pszTemp );
			}

			// EquipDropMax
			pszTemp = ThirdFile.Search_Posistion(i,11)->pString;
			nTempSize = (INT)(strlen(pszTemp) - 1);
			pInfo->m_bPercent_EquipDropMax = (pszTemp[nTempSize]=='%')?(TRUE):(FALSE);
			if ( pInfo->m_bPercent_EquipDropMax )
			{
				pInfo->m_nPercent_EquipDropMax	= atoi( &pszTemp[0] );
			}
			else
			{
				pInfo->m_nEquipDropMax			= atoi( pszTemp );
			}
		}
		else
		{
			Assert( nTempID < m_nMaxID && "DiePenaltyManager::Init" );
			break;
		}
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID DiePenaltyManager::CleanUp( VOID )
{
__ENTER_FUNCTION
	if ( m_papPenaltyInfo != NULL )
	{
		INT i;
		for ( i = 0; i < m_nMaxID; i++ )
		{
			if ( m_papPenaltyInfo[i] != NULL )
			{
				m_papPenaltyInfo[i]->Reset();
				SAFE_DELETE( m_papPenaltyInfo[i] );
			}
		}
		delete [] m_papPenaltyInfo;
		m_papPenaltyInfo = NULL;

		m_nMaxID = 0;
	}
__LEAVE_FUNCTION
}
