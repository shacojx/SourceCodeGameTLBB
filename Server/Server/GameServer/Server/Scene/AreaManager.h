/********************************************************************************
 *	文件名：	AreaManager.h
 *	全路径：	d:\Prj\Server\Server\Scene\AreaManager.h
 *	创建时间：	
 *
 *	功能说明：	
 *	修改记录：	2005 年 11 月 5 日	20:09，将 EventZone 都改成了 Area，以免概念混淆
*********************************************************************************/


#ifndef __AREAMANGER_H__
#define __AREAMANGER_H__

#include "Type.h"

struct _FRECT
{
	FLOAT		m_fLeft;
	FLOAT		m_fTop;
	FLOAT		m_fRight;
	FLOAT		m_fBottom;

	_FRECT( )
	{
		CleanUp( ) ;
	}

	VOID CleanUp( )
	{
		m_fLeft = 0 ;
		m_fTop =0 ;
		m_fRight =0 ;
		m_fBottom=0 ;
	}

	BOOL IsContain( FLOAT x, FLOAT y ) const
	{
		return ((x >= m_fLeft) && (x < m_fRight) && (y >= m_fTop) && (y < m_fBottom) );
	}
};

struct Area
{
	UINT		m_uAreaID;
	UINT		m_uScriptID;
	_FRECT		m_rcArea;

	Area()
	{
		CleanUp( ) ;
	}

	VOID CleanUp( )
	{
		m_uAreaID = INVALID_ID ;
		m_uScriptID = INVALID_ID ;
		m_rcArea.CleanUp( ) ;
	}

	BOOL IsContain( FLOAT x, FLOAT y ) const
	{
		return m_rcArea.IsContain( x, y );
	}
};

#define MAX_AREA_SCENE 128

class Scene;
class AreaManager
{
public :
	AreaManager( VOID );
	~AreaManager( VOID );

	BOOL Init( const CHAR *pszPathName );
	VOID Term( VOID );

	VOID SetScene( Scene* pScene ) { Assert( pScene ); m_pScene = pScene; }

	UINT GetZoneID( FLOAT x, FLOAT z );

protected:
	UINT			m_uAreaCount;
	Area*			m_paArea;
	Scene*			m_pScene;
};

#endif // __AREAMANGER_H__
