
#pragma once

#include "Obj_Map.h"

/**
	注册网格内,会移动的物体
*/

#define MINI_MOVE_DISTANCE		(0.05f)

typedef std::list< fVector3 > CPathNodeList;
class CObject_Dynamic : public CObject_Map
{
public:
	CObject_Dynamic();
	virtual ~CObject_Dynamic();

private:
	BOOL					m_bServerPath;		// 是否由SERVER发过来的路径
	UINT					m_dwPathID;

	CPathNodeList			m_listPathNode;
	fVector3				m_posPathNodePrev;

protected:
	BOOL IsServerPath( VOID )const{
		return m_bServerPath;
	}

	const fVector3 *GetPrevPathNode( VOID )const
	{
		return &m_posPathNodePrev;
	}
	const fVector3 *GetBeginPathNode( VOID )const;
	VOID AddPathNode( BOOL bServerPath, UINT dwPathID, const fVector3 *pfvNode );
	VOID CleanupPathNode( VOID );

	BOOL BeginPathNode( VOID );
	BOOL NextPathNode( VOID );
	BOOL CalcMoveDir( VOID );
	BOOL DoMove( FLOAT fElapseTime, FLOAT fSpeed );

	virtual VOID OnNextPathNode( VOID );
	virtual VOID OnMoveStop( VOID );

public:
	UINT GetPathID( VOID )const{
		return m_dwPathID;
	}

	const fVector3 *GetCurMoveTargetPos( VOID )const{
		return GetBeginPathNode();
	}

	const CPathNodeList *GetPathNodeList( VOID )const{
		return &m_listPathNode;
	}
protected:
	WX_DECLARE_DYNAMIC(CObject_Dynamic);
	//TD_DECLARE_LOGICAL(true);
};
