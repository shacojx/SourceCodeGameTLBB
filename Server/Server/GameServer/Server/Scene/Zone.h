// Zone.h
//
///////////////////////////////////////////////////////

#ifndef __ZONE_H__
#define __ZONE_H__

#include "Type.h"
#include "ObjList.h"
#include "AreaManager.h"

struct _ZONE_INFO
{
	WORD	m_wZoneW;
	WORD	m_wZoneH;
	WORD	m_wZoneSize;

	_ZONE_INFO( )
	{
		m_wZoneW = 0;
		m_wZoneH = 0;
		m_wZoneSize = 0;
	};
};

class Zone
{
public:
	Zone( );
	~Zone( );

	VOID CleanUp();
public:
	VOID SetZoneID( ZoneID_t id ){
		m_ZoneID = id;
	}

	ZoneID_t GetZoneID( ){
		return m_ZoneID;
	}

	ObjList *GetObjList( VOID ){
		return &m_ObjList;
	}

	ObjList *GetHumanList( VOID ){
		return &m_HumanList;
	}

	VOID OnObjectEnter( Obj *pObj );
	VOID OnObjectLeave( Obj *pObj );

	VOID AddArea(const Area* pArea)
	{
		if( m_AreaCount >= MAX_AREA_IN_ZONE )
		{
			Assert(FALSE && "这个 zone 的 area 也太多了吧！");
			return;
		}

		m_apArea[m_AreaCount++] = pArea;
	}

	const Area* GetCurrentArea( const WORLD_POS* pWorldPos );

	BYTE GetAreaCount() { return m_AreaCount; }

protected:
	ObjList				m_HumanList;
	ObjList				m_ObjList;		// 包含m_HumanList中的所有内容

	ZoneID_t			m_ZoneID;
	BYTE				m_AreaCount;
	const Area*			m_apArea[MAX_AREA_IN_ZONE];
};



#endif // __ZONE_H__
