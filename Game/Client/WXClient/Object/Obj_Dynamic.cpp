#include "StdAfx.h"
#include <Math.h>
#include "Obj_Dynamic.h"
#include "ObjectManager.h"
#include "Character\Obj_PlayerMySelf.h"
#include "TDUtil.h"
#include "..\Global.h"
#include "..\World\Path.h"

WX_IMPLEMENT_DYNAMIC(CObject_Dynamic, GETCLASS(CObject_Map));

CObject_Dynamic::CObject_Dynamic()
{
	m_bServerPath	= FALSE;
	m_dwPathID		= UINT_MAX;

	m_posPathNodePrev		= fVector3( 0.f, 0.f, 0.f );
}

CObject_Dynamic::~CObject_Dynamic()
{
}

const fVector3 *CObject_Dynamic::GetBeginPathNode( VOID )const
{
	if ( m_listPathNode.empty() )
		return NULL;

	return &(*m_listPathNode.begin());
}

VOID CObject_Dynamic::AddPathNode( BOOL bServerPath, UINT dwPathID, const fVector3 *pfvNode )
{
	if ( m_listPathNode.empty() )
	{
		m_posPathNodePrev = GetPosition();
		m_bServerPath	= bServerPath;
	}
	else if ( !m_bServerPath && bServerPath )
	{
		return ;
	}
	else if ( m_bServerPath && !bServerPath )
	{
		m_bServerPath	= bServerPath;
		CleanupPathNode();
		m_posPathNodePrev = GetPosition();
	}
	else
	{
		if ( dwPathID != m_dwPathID )
		{
			CleanupPathNode();
			m_posPathNodePrev = GetPosition();
		}
	}
	m_dwPathID = dwPathID;
	m_listPathNode.push_back( *pfvNode );

}

VOID CObject_Dynamic::CleanupPathNode( VOID )
{
	m_listPathNode.erase( m_listPathNode.begin(), m_listPathNode.end() );
}

BOOL CObject_Dynamic::BeginPathNode( VOID )
{
	if ( m_listPathNode.empty() )
		return FALSE;

	CalcMoveDir();
	return TRUE;
}

BOOL CObject_Dynamic::NextPathNode( VOID )
{
	if ( m_listPathNode.empty() )
		return FALSE;

	m_posPathNodePrev = *(m_listPathNode.begin());
	m_listPathNode.pop_front();

	if ( m_listPathNode.empty() )
		return FALSE;

	CalcMoveDir();
	OnNextPathNode();
	return TRUE;
}

BOOL CObject_Dynamic::CalcMoveDir( VOID )
{
	if ( m_listPathNode.empty() )
		return FALSE;

	fVector3 fvNextPos;
	fVector2 fvNextPos2D;
	fvNextPos		= (*m_listPathNode.begin());
	fvNextPos2D.x	= fvNextPos.x;
	fvNextPos2D.y	= fvNextPos.z;

	FLOAT fYAngle = TDU_GetYAngle(
		fVector2(GetPosition().x, GetPosition().z), 
		fvNextPos2D);

	SetFaceDir( fYAngle );
	return TRUE;
}

BOOL CObject_Dynamic::DoMove( FLOAT fElapseTime, FLOAT fSpeed )
{
	const fVector3* pRrePos = GetPrevPathNode();
	if ( m_listPathNode.empty() )
		return FALSE;

	// 当前位置
	fVector3 fvCurPos = GetPosition();
	// 当前的目标路径点
	fVector3 fvBeginPathNode = (*m_listPathNode.begin());

	// 当前位置与当前的目标路径点的距离
	fVector3 fvDistToBeginPathNode = fvBeginPathNode - fvCurPos;
	FLOAT fDir = GetFaceDir();

	// 这一帧可移动的路径长度
	FLOAT fMoveDist = fSpeed * fElapseTime;
	FLOAT fMoveDistSq = fMoveDist * fMoveDist;

	// 当前位置与当前的目标路径点路径长度的平方
	FLOAT fDistToBeginPathNodeSq = fvDistToBeginPathNode.x * fvDistToBeginPathNode.x + fvDistToBeginPathNode.z * fvDistToBeginPathNode.z;

	// 如果这一帧可移动的路径长度小于当前位置到当前的目标路径点路径长度
	if ( fDistToBeginPathNodeSq > fMoveDistSq )
	{
		FLOAT fTarDist = sqrt(fDistToBeginPathNodeSq) ;
		FLOAT fDistX = (fMoveDist*(fvBeginPathNode.x-fvCurPos.x))/fTarDist ;
		FLOAT fDistZ = (fMoveDist*(fvBeginPathNode.z-fvCurPos.z))/fTarDist ;
		
		
		fVector2 fvPos;
		fvPos.x = fvCurPos.x + fDistX;
		fvPos.y = fvCurPos.z + fDistZ;
		
		//需要位置修正，防止因为误差走入阻挡区内部
		if( CObjectManager::GetMe()->GetMySelf() == this && CPath::IsPointInUnreachRegion(fvPos))
		{
			fvPos = TDU_GetReflect(
						fVector2(pRrePos->x, pRrePos->z),
						fVector2(fvBeginPathNode.x, fvBeginPathNode.z),
						fvPos);//

		}

		SetMapPosition( fvPos );
	}
	else
	{
		fVector2 fvPos;
		// 目标路径是最后一点
		if ( !NextPathNode() )
		{
			fvPos.x = fvBeginPathNode.x;
			fvPos.y = fvBeginPathNode.z;
			//需要位置修正，防止因为误差走入阻挡区内部
			if( CObjectManager::GetMe()->GetMySelf() == this && CPath::IsPointInUnreachRegion(fvPos))
			{
				//行走路线的镜像点， 点pRrePos 与 fvBeginPathNode不能是同一个点
				fvPos = TDU_GetReflect(
							fVector2(pRrePos->x, pRrePos->z),
							fVector2(fvBeginPathNode.x, fvBeginPathNode.z),
							fvPos);//
			}

			SetMapPosition( fvPos );
			OnMoveStop( );
		}
		else
		{
			FLOAT fDistToBeginPathNode = sqrt( fDistToBeginPathNodeSq );
			fMoveDist -= fDistToBeginPathNode;

			fvPos.x = fvBeginPathNode.x;
			fvPos.y = fvBeginPathNode.z;
			SetMapPosition( fvPos );
			CalcMoveDir();
			fDir = GetFaceDir();

			FLOAT fDistX = sin( fDir ) * fMoveDist;
			FLOAT fDistZ = cos( fDir ) * fMoveDist;
			fvCurPos = fvBeginPathNode;
			fvPos.x = fvCurPos.x + fDistX;
			fvPos.y = fvCurPos.z + fDistZ;
			
			//需要位置修正，防止因为误差走入阻挡区内部
			if( CObjectManager::GetMe()->GetMySelf() == this && CPath::IsPointInUnreachRegion(fvPos))
			{
				// 得到新的映射向量
				pRrePos = GetPrevPathNode();
				fvBeginPathNode = (*m_listPathNode.begin());

				//行走路线的镜像点， 点pRrePos 与 fvBeginPathNode不能是同一个点
				fvPos = TDU_GetReflect(
							fVector2(pRrePos->x, pRrePos->z),
							fVector2(fvBeginPathNode.x, fvBeginPathNode.z),
							fvPos);//

			}//
			
			SetMapPosition( fvPos );
		}

//		RegisterToZone();
	}
	return TRUE;
}

VOID CObject_Dynamic::OnNextPathNode( VOID )
{
}

VOID CObject_Dynamic::OnMoveStop( VOID )
{
}
