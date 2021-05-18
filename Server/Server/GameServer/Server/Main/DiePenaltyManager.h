// DiePenaltyManager.h
// 
// 
// 作用：死亡惩罚数据管理器
// 
////////////////////////////////////////////////////////

#ifndef __DIEPENALTYMANAGER_H__
#define __DIEPENALTYMANAGER_H__

#include "Type.h"
#include "GameDefine.h"

struct _DIE_PENALTY_INFO
{
	BOOL			m_bPercent_MoneyMin;
	union
	{
		INT			m_nMoneyMin;
		INT			m_nPercent_MoneyMin;
	};

	BOOL			m_bPercent_MoneyMax;
	union
	{
		INT			m_nMoneyMax;
		INT			m_nPercent_MoneyMax;
	};

	BOOL			m_bPercent_ExpMin;
	union
	{
		INT			m_nExpMin;
		INT			m_nPercent_ExpMin;
	};

	BOOL			m_bPercent_ExpMax;
	union
	{
		INT			m_nExpMax;
		INT			m_nPercent_ExpMax;
	};

	BOOL			m_bPercent_EquipDurMin;
	union
	{
		INT			m_nEquipDurMin;
		INT			m_nPercent_EquipDurMin;
	};

	BOOL			m_bPercent_EquipDurMax;
	union
	{
		INT			m_nEquipDurMax;
		INT			m_nPercent_EquipDurMax;
	};

	BOOL			m_bPercent_ItemDropMin;
	union
	{
		INT			m_nItemDropMin;
		INT			m_nPercent_ItemDropMin;
	};

	BOOL			m_bPercent_ItemDropMax;
	union
	{
		INT			m_nItemDropMax;
		INT			m_nPercent_ItemDropMax;
	};

	BOOL			m_bPercent_EquipDropMin;
	union
	{
		INT			m_nEquipDropMin;
		INT			m_nPercent_EquipDropMin;
	};

	BOOL			m_bPercent_EquipDropMax;
	union
	{
		INT			m_nEquipDropMax;
		INT			m_nPercent_EquipDropMax;
	};

	_DIE_PENALTY_INFO( VOID )
	{
		m_bPercent_MoneyMin			= FALSE;
		m_nMoneyMin					= -1;
		m_bPercent_MoneyMax			= FALSE;
		m_nMoneyMax					= -1;
		m_bPercent_ExpMin			= FALSE;
		m_nExpMin					= -1;
		m_bPercent_ExpMax			= FALSE;
		m_nExpMax					= -1;
		m_bPercent_EquipDurMin		= FALSE;
		m_nEquipDurMin				= -1;
		m_bPercent_EquipDurMax		= FALSE;
		m_nEquipDurMax				= -1;
		m_bPercent_ItemDropMin			= FALSE;
		m_nItemDropMin					= -1;
		m_bPercent_ItemDropMax			= FALSE;
		m_nItemDropMax					= -1;
		m_bPercent_EquipDropMin			= FALSE;
		m_nEquipDropMin					= -1;
		m_bPercent_EquipDropMax			= FALSE;
		m_nEquipDropMax					= -1;
	}

	VOID Reset( VOID )
	{
		m_bPercent_MoneyMin			= FALSE;
		m_nMoneyMin					= -1;
		m_bPercent_MoneyMax			= FALSE;
		m_nMoneyMax					= -1;
		m_bPercent_ExpMin			= FALSE;
		m_nExpMin					= -1;
		m_bPercent_ExpMax			= FALSE;
		m_nExpMax					= -1;
		m_bPercent_EquipDurMin		= FALSE;
		m_nEquipDurMin				= -1;
		m_bPercent_EquipDurMax		= FALSE;
		m_nEquipDurMax				= -1;
		m_bPercent_ItemDropMin			= FALSE;
		m_nItemDropMin					= -1;
		m_bPercent_ItemDropMax			= FALSE;
		m_nItemDropMax					= -1;
		m_bPercent_EquipDropMin			= FALSE;
		m_nEquipDropMin					= -1;
		m_bPercent_EquipDropMax			= FALSE;
		m_nEquipDropMax					= -1;
	}
};

class DiePenaltyManager
{
public :
	DiePenaltyManager();
	~DiePenaltyManager();

	BOOL Init( INT nMaxID );
	VOID CleanUp( VOID );

	const _DIE_PENALTY_INFO *GetDiePenalty( INT nID )
	{
		if ( nID >= 0 && nID < m_nMaxID )
		{
			return m_papPenaltyInfo[nID];
		}
		return NULL;
	}

private :
	INT					m_nMaxID;
	_DIE_PENALTY_INFO	**m_papPenaltyInfo;
};

extern DiePenaltyManager* g_pDiePenaltyManager;

#endif	// __DIEPENALTYMANAGER_H__
