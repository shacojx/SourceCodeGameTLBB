#include "StdAfx.h"
#include "UICreatureBoardSystem.h"
#include "UICreatureBoard.h"
#include "CEGUIPropertyHelper.h"
#include "ceguiwindowmanager.h"
#include "uiwindowmng.h"
#include "uiglobal.h"
#include "TDTimeSystem.h"
#include "uistring.h"
#include "..\CEGUIFalagardEx\Falagardbehitboard.h"
#include "..\CEGUIFalagardEx\IFalagardbehitboard.h"
#include "axtrace.h"
CEGUI::Window*  CUICreatureBoardSystem::m_pCreatureBoardGroundSheet = NULL; // 客户端窗口底板

CUICreatureBoardSystem* CUICreatureBoardSystem::s_pMe = NULL;

CBeHitBoardManager* CBeHitBoardManager::s_pMe = NULL;


CUICreatureBoardSystem::CUICreatureBoardSystem()
{
	s_pMe = this;
	m_pBeHitBoardManager = NULL;
	m_nMaxDistance = 324;
	m_nMaxDispearDistance = 625;
}

CUICreatureBoardSystem::~CUICreatureBoardSystem()
{
}

VOID CUICreatureBoardSystem::Initial(VOID)
{
	Release();
	m_nBeHitCounter = 0;
	m_pBeHitBoardManager = new CBeHitBoardManager;
	
	m_pCreatureBoardGroundSheet = CEGUI::WindowManager::getSingleton().createWindow((CEGUI::utf8*)"DefaultGUISheet", (CEGUI::utf8*)"__CREATUREBOARDSHEET__");
	m_pCreatureBoardGroundSheet->setMouseHollow(true);

	if(CUIWindowMng::GetClientScreen())
	{
		CUIWindowMng::GetClientScreen()->addChildWindow( m_pCreatureBoardGroundSheet );
		m_pCreatureBoardGroundSheet->setProperty("RiseOnClick","False");
		m_pCreatureBoardGroundSheet->moveToBack();
	}

}

VOID CUICreatureBoardSystem::Release(VOID)
{
	//清除所有信息板窗口
	std::list< CCreatureBoard* >::iterator it;
	for(it=m_listAllBoard.begin(); it!=m_listAllBoard.end(); it++)
	{
		if(*it)
		{
			delete (*it);
			*it = NULL;
		}
	}
	m_listAllBoard.clear();

	for(it=m_listBoardDataPool.begin(); it!=m_listBoardDataPool.end(); it++)
	{
		if(*it)
		{
			delete (*it);
			*it = NULL;
		}
	}
	m_listBoardDataPool.clear();

	if( m_pBeHitBoardManager )
	{
		delete m_pBeHitBoardManager;
		m_pBeHitBoardManager = NULL;
	}
}

CCreatureBoard*	CUICreatureBoardSystem::CreateCreatureBoard()
{
	CCreatureBoard* pNewBoard = NULL;

	//创建新的
	if(m_listBoardDataPool.empty())
	{
		static int index =0;
		pNewBoard = new CCreatureBoard(index++);
	}
	else
	{
		//尝试从池中取得
		pNewBoard = *(m_listBoardDataPool.begin());

		m_listBoardDataPool.erase(m_listBoardDataPool.begin());
	}

	pNewBoard->reset();

	m_listAllBoard.push_back(pNewBoard);

	CUICreatureBoardSystem::GetClientScreen()->addChildWindow( pNewBoard->GetMainWindow() );
	pNewBoard->Show(FALSE);

	return pNewBoard;
}

void CUICreatureBoardSystem::DestroyCreatureBoard(CCreatureBoard* pBoard)
{
	if(!pBoard) return;
	CUICreatureBoardSystem::GetClientScreen()->removeChildWindow( pBoard->GetMainWindow() );

	//从链表中删除
	std::list< CCreatureBoard* >::iterator it;
	for(it=m_listAllBoard.begin(); it!=m_listAllBoard.end(); it++)
	{
		if((*it) == pBoard)
		{
			m_listAllBoard.erase(it);

			//放入池中
			m_listBoardDataPool.push_back(pBoard);
			pBoard->Show(FALSE);
			break;
		}
	}
}

void CUICreatureBoardSystem::AddNewBeHit( bool bDoubleHit, LPCTSTR szValue, float nStartX, float nStartY, int nType, int nMoveType )
{
	if( m_listBeHitBoard.size() > 100 )
		return;
	CBeHitBoard* pBeHit = CBeHitBoardManager::GetMe()->GetFreeWindow();
	if( pBeHit )
	{
		if( bDoubleHit )
		{
			nType += 10;
			nMoveType += 10;
		}
		pBeHit->m_bDoubleHit		= bDoubleHit;
		pBeHit->m_ColorType			= CBeHitBoardManager::GetMe()->m_ColorType[ nType ];
		pBeHit->m_BackColorType		= CBeHitBoardManager::GetMe()->m_BackColorType[ nType ];
		STRUCT_BEHIT_DATA* pItem	= &CBeHitBoardManager::GetMe()->m_BeHitData[ nMoveType ];
		pBeHit->m_nXSpeed			= CBeHitBoardManager::GetMe()->m_BeHitData[ nMoveType ].m_nXSpeed;
		pBeHit->m_nYSpeed			= CBeHitBoardManager::GetMe()->m_BeHitData[ nMoveType ].m_nYSpeed; // Y象素级移动速度
		pBeHit->m_nXAcceleration	= CBeHitBoardManager::GetMe()->m_BeHitData[ nMoveType ].m_nXAcceleration;
		pBeHit->m_nYAcceleration	= CBeHitBoardManager::GetMe()->m_BeHitData[ nMoveType ].m_nYAcceleration; // 两个方向的加速度
		pBeHit->m_nMaxLife			= CBeHitBoardManager::GetMe()->m_BeHitData[ nMoveType ].m_nMaxLife;
		pBeHit->m_bAlphaMode		= CBeHitBoardManager::GetMe()->m_BeHitData[ nMoveType ].m_bAlphaMode;	
		pBeHit->m_nMoveMode			= CBeHitBoardManager::GetMe()->m_BeHitData[ nMoveType ].m_nMoveMode;	
		pBeHit->m_nUseTempPos		= CBeHitBoardManager::GetMe()->m_BeHitData[ nMoveType ].m_nUseTempPos;	
		pBeHit->m_nStartX			= nStartX + CBeHitBoardManager::GetMe()->m_BeHitData[ nMoveType ].m_fXPos;
		pBeHit->m_nStartY			= nStartY + CBeHitBoardManager::GetMe()->m_BeHitData[ nMoveType ].m_fYPos;
		pBeHit->m_nMoveType			= nMoveType;
		if( pBeHit->m_nUseTempPos )
		{
			// 判断是不是已经占用位置了
			std::list< CBeHitBoard* >::iterator it;
			int bTemp[ CEGUI::MAX_TEMP_TYPE_NUMBER ];
			for( int i = 0; i < CEGUI::MAX_TEMP_TYPE_NUMBER; i ++ )
				bTemp[ i ] = false;
			for( it = m_listBeHitBoard.begin(); it != m_listBeHitBoard.end(); it++)
			{
				CBeHitBoard *pItem = ( *it );
				// 如果同是自己的类型
				if( pBeHit->m_nMoveType == pItem->m_nMoveType )
				{
					if( abs( int( pItem->m_nStartY - pBeHit->m_nStartY ) ) < 50 )
					{
						bTemp[ pItem->m_nTempPosIndex ] = true;
					}
				}
			}
			for( int i = 0; i < CEGUI::MAX_TEMP_TYPE_NUMBER;i ++ )
			{
				if( !bTemp[ i ] )
				{
					pBeHit->m_nTempPosIndex = i;
					pBeHit->m_nStartX += CBeHitBoardManager::GetMe()->m_TempPoint[ i ].d_x;
					pBeHit->m_nStartY += CBeHitBoardManager::GetMe()->m_TempPoint[ i ].d_y;
					break;
				}
			}
		}
		pBeHit->InUse( true );
		pBeHit->ResetData( szValue );
		pBeHit->Update();
		m_listBeHitBoard.push_back( pBeHit );
		
			
	}
}
VOID CUICreatureBoardSystem::Tick()
{
	std::list< CBeHitBoard* >::iterator it;

	for( it = m_listBeHitBoard.begin(); it != m_listBeHitBoard.end();)
	{
		CBeHitBoard *pItem = ( *it );
		pItem->Update();
		if(pItem->m_nLife < 0)
		{
			std::list< CBeHitBoard* >::iterator it2 = it ++;
			CBeHitBoardManager::GetMe()->FreeWindow( pItem );
			m_listBeHitBoard.erase( it2 );
		}
		else
			it ++;
	}
	

	
}
CBeHitBoard::~CBeHitBoard()
{
	InUse( false );	
	CEGUI::WindowManager::getSingleton().destroyWindow( m_szWindowName );
}
CBeHitBoard::CBeHitBoard()
{
	m_nLife = 0;
	m_fAlpha = 1.0f;
	m_fDoubleTime = 1000;
	m_bInUse   = false;
	m_nTempPosIndex = 0;
}
void CBeHitBoard::ResetData( LPCTSTR szText )
{
	
	CEGUI::String strUtf32;
	char szTmp[ 128 ];
	if( m_BackColorType.getARGB() != 0 )
	{	
		CUIStringSystem::GetMe()->ParserString_Runtime(szText, strUtf32);
	}
	else
	{
		sprintf( szTmp,"#e%02X%02X%02X%s",int( m_BackColorType.getRed() * 255 ),int( m_BackColorType.getGreen() * 255) ,int( m_BackColorType.getBlue() * 255 ), szText );
		CUIStringSystem::GetMe()->ParserString_Runtime(szTmp, strUtf32);
	}
	
	
	if( m_bDoubleHit )
	{
		m_bZoomOn = true;
		m_fScale = CBeHitBoardManager::GetMe()->getDoubleEndSize();
	}
	else
	{
		m_bZoomOn = false;
		m_fScale = CBeHitBoardManager::GetMe()->getNormalSize();
	}
	
	((CEGUI::IFalagardBeHitBoard*)(CEGUI::FalagardBeHitBoard*)m_pWindow )->setTextScale(  m_fScale, m_fScale ); 
	m_pWindow->setText( strUtf32 );
	((CEGUI::IFalagardBeHitBoard*)(CEGUI::FalagardBeHitBoard*)m_pWindow )->SetTextColor( m_ColorType );
	m_nLife = m_nMaxLife;
	m_fDoubleTime = CBeHitBoardManager::GetMe()->getDoubleTime();
	
}
void CBeHitBoard::Update()
{
	m_nXSpeed += ( float( m_nXAcceleration ) / 1000 ) * g_pTimer->GetDeltaTime();
	m_nYSpeed += ( float( m_nYAcceleration ) / 1000 ) * g_pTimer->GetDeltaTime();

	m_nStartX += ( float( m_nXSpeed ) / 1000 ) * g_pTimer->GetDeltaTime();
	m_nStartY += ( float( m_nYSpeed ) / 1000 ) * g_pTimer->GetDeltaTime();

	m_nLife -= (int)g_pTimer->GetDeltaTime();

	if( m_bAlphaMode )
		m_fAlpha = float( m_nLife ) / float( m_nMaxLife );
	if( m_bDoubleHit )
	{
		if( m_bZoomOn )
		{
			m_fScale += m_fDoubleTime / g_pTimer->GetDeltaTime();
			if( m_fScale > CBeHitBoardManager::GetMe()->getDoubleSize() )
				m_bZoomOn = false;
		}
		else
		{
			m_fScale -= m_fDoubleTime / g_pTimer->GetDeltaTime();
		}
		
		if( m_fScale < CBeHitBoardManager::GetMe()->getDoubleEndSize() )
		{
			m_fScale = CBeHitBoardManager::GetMe()->getDoubleEndSize();
			m_bDoubleHit = false;
		}
		((CEGUI::IFalagardBeHitBoard*)(CEGUI::FalagardBeHitBoard*)m_pWindow )->setTextScale( m_fScale, m_fScale );	
		
	}
	if( m_nLife > 0 )
	{
		CEGUI::Point fPos;
//		CEGUI::Size size = CEGUI::System::getSingleton().getRenderer()->getSize();
		fPos.d_x = float( m_nStartX );// / size.d_width;
		fPos.d_y = float( m_nStartY );// / size.d_height;
		((CEGUI::IFalagardBeHitBoard*)(CEGUI::FalagardBeHitBoard*)m_pWindow )->setDisplayCenter( fPos.d_x, fPos.d_y );
		if( m_bAlphaMode )
			m_pWindow->setAlpha( m_fAlpha );
	}

}
void CBeHitBoard::InUse( bool bUse )
{
	m_bInUse = bUse; 
	if( m_bInUse )
	{
		CUICreatureBoardSystem::GetClientScreen()->addChildWindow( m_pWindow );
	}
	else
	{
		CUICreatureBoardSystem::GetClientScreen()->removeChildWindow( m_pWindow );
	}
}
// 用来管理头上冒的字得管理器
CBeHitBoardManager::CBeHitBoardManager()
{

	s_pMe = this;

	CEGUI::Window *pWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout( "Schema\\BeHit.layout.xml" );
	pWindow = CEGUI::WindowManager::getSingleton().getWindow( "auto_BeHitBoard" );
	CEGUI::IFalagardBeHitBoard* pBehitWindow = (CEGUI::IFalagardBeHitBoard*)(CEGUI::FalagardBeHitBoard*)pWindow;
	for( int i = 0; i < CEGUI::MAX_MOVE_TYPE_NUMBER; i ++ )
	{
		m_BeHitData[ i ].m_nXSpeed 			= float( pBehitWindow->getXSpeed( i ) );
		m_BeHitData[ i ].m_nYSpeed			= float( pBehitWindow->getYSpeed( i ) );
		m_BeHitData[ i ].m_fXPos 			= float( pBehitWindow->getXPos( i ) );
		m_BeHitData[ i ].m_fYPos			= float( pBehitWindow->getYPos( i ) );
		m_BeHitData[ i ].m_nXAcceleration 	= float( pBehitWindow->getXAcceleration( i ) );
		m_BeHitData[ i ].m_nYAcceleration 	= float( pBehitWindow->getYAcceleration( i ) );
		m_BeHitData[ i ].m_bAlphaMode		= pBehitWindow->getAlpha( i );
		m_BeHitData[ i ].m_nMaxLife			= pBehitWindow->getLife( i );
		m_BeHitData[ i ].m_nMoveMode		= pBehitWindow->getMoveMode( i );
		m_BeHitData[ i ].m_nUseTempPos		= pBehitWindow->getTempPosUse( i );
	}
	for( int i = 0; i < CEGUI::MAX_COLOR_TYPE_NUMBER; i ++ )
	{
		m_ColorType[ i ] = pBehitWindow->getColorType( i );
		m_BackColorType[ i ] = pBehitWindow->getBackColorType( i );
	}

	for( int i = 0; i < CEGUI::MAX_TEMP_TYPE_NUMBER; i ++ )
	{
		m_TempPoint[ i ] = pBehitWindow->getTempPos( i );
	}
	m_fNormalSize		= pBehitWindow->getNormalSize();
	m_fDoubleSize		= pBehitWindow->getDoubleSize();
	m_fDoubleEndSize	= pBehitWindow->getDoubleEndSize();
	m_fDoubleTime		= pBehitWindow->getDoubleTime();

	CEGUI::WindowManager::getSingleton().destroyWindow( "BeHitBoard" );

	m_nCounter = 0;
}
CBeHitBoardManager::~CBeHitBoardManager()
{
	std::list< CBeHitBoard* >::iterator it;
	for( it = m_aWindowList.begin(); it != m_aWindowList.end(); it ++)	
	{
		CBeHitBoard* pItem = (*it);
		if( pItem )
		{
			delete pItem;
			pItem = NULL;
		}
	}
	m_aWindowList.clear();
}
// 得到一个没有用得窗口
CBeHitBoard* CBeHitBoardManager::GetFreeWindow()
{
	std::list< CBeHitBoard* >::iterator it;
	for( it = m_aWindowList.begin(); it != m_aWindowList.end(); it ++)	
	{
		if( ( *it )->IsInUse() == false )
		{
			return ( *it );
		}
	}
	// 走到这里说明没有富裕得窗口了
	return NewWindow();
}
// 告诉管理器,有一个窗口释放了.
void CBeHitBoardManager::FreeWindow( CBeHitBoard* pWindow )
{
	std::list< CBeHitBoard* >::iterator it;
	for( it = m_aWindowList.begin(); it != m_aWindowList.end(); it ++)	
	{
		if( ( *it ) == pWindow  )
		{
			( *it )->InUse( false ); // 去掉这个窗口得使用标志
			break;
		}

	}
}
CBeHitBoard* CBeHitBoardManager::NewWindow()
{
	CBeHitBoard*  pBeHit = new CBeHitBoard();
	
	_snprintf( pBeHit->m_szWindowName, 128, "BeHitBoard_%d", m_nCounter );
	pBeHit->m_pWindow = CEGUI::WindowManager::getSingleton().createWindow( "WoWLook/BeHitBoard", pBeHit->m_szWindowName );
	pBeHit->m_pWindow->setProperty( "RiseOnClick", "False" );
	pBeHit->m_pWindow->setProperty( "HorzFormatting", "HorzCentred" );
	pBeHit->m_pWindow->setProperty( "MouseHollow", "True" );
	pBeHit->m_pWindow->setProperty( "FitType", "Bottom" );
	m_aWindowList.push_back( pBeHit );
	m_nCounter ++;
	return pBeHit;
}
