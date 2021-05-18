#include "falagardscenemap.h"
#include "ceguiPropertyHelper.h"
#include "ceguiimagesetmanager.h"
#include "ceguiimageset.h"
#include "ceguianimatemanager.h"
#include "ceguiwindowmanager.h"
#include "windows.h"
namespace CEGUI
{

	FalagardSceneMapProperties::PlayerImage			FalagardSceneMap::d_PlayerImageProperty;		// 队友用得图标
	FalagardSceneMapProperties::MySelfImage			FalagardSceneMap::d_MyselfImageProperty;		// 自己所需要得图片动画

	const String FalagardSceneMap::EventNamespace("Falagard/SceneMap");
	const String FalagardSceneMap::MouseRButtonDownCtrl("MouseRButtonDownCtrl");
	const utf8 FalagardSceneMap::WidgetTypeName[] = "Falagard/SceneMap";
	const int SCENE_MAP_PER_PIC = 64; // 每一块地图片对应多大的地图区域
	const int SCENE_MAP_ZOOMSCALE = 4;

	FalagardSceneMap::FalagardSceneMap(const String& type, const String& name) :
		FalagardStaticImage(type, name)
	{
		CEGUI_START_ADD_STATICPROPERTY( FalagardSceneMap )
		   CEGUI_ADD_STATICPROPERTY( &d_PlayerImageProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_MyselfImageProperty );
		CEGUI_END_ADD_STATICPROPERTY
		
		d_fresh = false;
		d_PlayerDirect = 0;
		d_MyselfAnimate.clear();// = "";
		m_pMySelfWindow = NULL;
		m_pLTWindow		= NULL;
		m_pRBWindow		= NULL;
		m_nXCount		= 0;
		m_nYCount		= 0;
		d_MouseScenePos = Point( 0, 0 );
		d_SceneSize		= Size( 128, 128 );
		d_PlayerImage	= NULL;
		for( int i = 0; i < 5; i ++ )
			m_pFriendWindow[ i ] = NULL;
		m_pBackground = NULL;
		m_WindowZero = Point( 0, 0 );
		m_aPlayerPos.clear();
		m_fZoomValueX = 1; // 默认为一乒全都显示
		m_fZoomValueY = 1; // 默认为一乒全都显示
		m_bDraping = false;
		m_nDisplayMode = SCENEMAP_DISPLAY_MODE_ABSOLTE;
		setMouseHollow( true );
	}

	FalagardSceneMap::~FalagardSceneMap()
	{
	}
	void FalagardSceneMap::Close()
	{
		Clear();

		uint child_count = getChildCount();
		for (uint i = 0; i < child_count; ++i)
		{
			d_children[i]->hide();

		}
		//WindowManager::getSingleton().destroyWindow( "SceneMapWindow_background" );
		if( m_pMySelfWindow )
		{
			m_pMySelfWindow = NULL;
		}
		for( int i = 0; i < 5; i ++ )
		{
			m_pFriendWindow[ i ] = NULL;
		}

		m_pLTWindow = NULL;
		m_pRBWindow = NULL;
		

	}
	void FalagardSceneMap::addPlayerPos( const Point& pos, const String& name )
	{
		SCENEMAP_FLAG_DATA  data;
		data.pos = pos;
		data.name = name;
		m_aPlayerPos.push_back( data );
		d_fresh = true;
	}
	void FalagardSceneMap::onMouseButtonDown( MouseEventArgs &e )
	{
		
		// 计算和原点之间的位置
		// 得到当前鼠标点在了相对于左上角的那个位置
		m_ptDrapOffset = ScreenPosToScenePosRelative( e.position );
		if( e.button == RightButton )
		{
			if( e.sysKeys & Control )
				return;
			m_bDraping = true;
			e.handled = true;
			captureInput();
		}
		Window::onMouseButtonDown( e );
	}
	void FalagardSceneMap::onMouseEnters(MouseEventArgs& e)
	{
		d_MouseScenePos = ScreenPosToScenePos( e.position );
		d_MouseScenePos.d_x /= m_fZoomValueX;
		d_MouseScenePos.d_y /= m_fZoomValueY;
		if( d_MouseScenePos.d_x < 0 || d_MouseScenePos.d_x > d_SceneSize.d_width ||
			d_MouseScenePos.d_y < 0 || d_MouseScenePos.d_y > d_SceneSize.d_height )
		{
			d_MouseScenePos.d_x = 0;
			d_MouseScenePos.d_y = 0;
		}
		Window::onMouseEnters( e );
	}
	void FalagardSceneMap::onMouseButtonUp( MouseEventArgs &e )
	{
		if( e.button == RightButton && ( e.sysKeys & Control ) )
		{
			WindowEventArgs args(this);
			fireEvent(MouseRButtonDownCtrl, args, EventNamespace);
			return;
		}
		m_bDraping = false;
		releaseInput();
		Window::onMouseButtonUp( e );
	}
	void    FalagardSceneMap::onMouseLeaves( MouseEventArgs& e )
	{
		m_bDraping = false;
		d_MouseScenePos = m_PlayerPos;
		Window::onMouseLeaves( e );
	}

	void FalagardSceneMap::onMouseMove( MouseEventArgs &e )
	{
		
		if( m_bDraping ) // 正在托动
		{
			Point tmp = ScreenPosToWindowPosRelative( e.position );
			m_WindowZero.d_x = tmp.d_x - m_ptDrapOffset.d_x;
			m_WindowZero.d_y = tmp.d_y - m_ptDrapOffset.d_y;
			
			UpdateAllWindow();
		}
		else
		{
			d_MouseScenePos = ScreenPosToScenePos( e.position );
			d_MouseScenePos.d_x /= m_fZoomValueX;
			d_MouseScenePos.d_y /= m_fZoomValueY;
			if( d_MouseScenePos.d_x < 0 || d_MouseScenePos.d_x > d_SceneSize.d_width ||
				d_MouseScenePos.d_y < 0 || d_MouseScenePos.d_y > d_SceneSize.d_height )
				return;
		}
		FalagardStaticImage::onMouseMove( e );
	}
	void FalagardSceneMap::updateSelf(float elapsed)
	{
		if( d_fresh && isVisible() )
		{
			int i = 0;
			// 更新自己得图片
			ResetMyself();
			// 更新队友得位置和信息

			for( i = 0; i < 5; i ++ )
				m_pFriendWindow[ i ]->hide();
			RENDER_POS::iterator   it;
			String strTooltip[ 5 ]; //5个人得tooltip
			for( i = 0, it = m_aPlayerPos.begin(); it != m_aPlayerPos.end(); it ++, i ++ )
			{
				ResetPlayerWindow( i, ( *it ) );

			}
			// 更新tooltip 
			
			for( int i = 0; i < 5; i ++ )
			{
				
				if( !m_pFriendWindow[ i ]->isVisible() ) // 如果可见才计算
					break;
				strTooltip[ i ] = m_pFriendWindow[ i ]->getTooltipText();
				for( int j = 0; j < 5; j ++ )
				{
					if( j == i )
						continue;
					if( !m_pFriendWindow[ j ]->isVisible() )
						break;
					if( Rect( 0, 0, 0, 0 ) != m_pFriendWindow[ i ]->getRect().getIntersection( m_pFriendWindow[ j ]->getRect() ) )
					{
						if( strTooltip[ i ].size() )
							strTooltip[ i ] += "\r\n";
						strTooltip[ i ] += m_pFriendWindow[ j ]->getTooltipText();
					}

				}
			}
			for( int i = 0; i < 5; i ++ )
			{
				if( !m_pFriendWindow[ i ]->isVisible() ) // 如果可见才计算
					break;
				m_pFriendWindow[ i ]->setTooltipText( strTooltip[ i ] );

			}
			d_fresh = false;
			requestRedraw();
		}
	}
	void FalagardSceneMap::setPlayerImage( const Image* pImage )
	{
		
		d_PlayerImage = ( Image* )pImage;
	}

	void FalagardSceneMap::setMyselfImage( const String& name )
	{
		d_MyselfAnimate = name;
	}
	void FalagardSceneMap::Clear()
	{
		m_aPlayerPos.clear();
	}
	void    FalagardSceneMap::SetViewByCenter( const Point& pos )
	{
		if( m_pBackground )
		{
			float fWidthWindowPerBackground = getAbsoluteWidth() / m_pBackground->getAbsoluteWidth();
			float fHeightWindowPerBackground = getAbsoluteHeight() / m_pBackground->getAbsoluteHeight();
			m_WindowZero.d_x = -( pos.d_x - fWidthWindowPerBackground / 2 ) / fWidthWindowPerBackground;
			m_WindowZero.d_y = -( pos.d_y - fHeightWindowPerBackground / 2 ) / fHeightWindowPerBackground;
			UpdateAllWindow();
		}
	}
	void FalagardSceneMap::setMyselfDir( const int nDirect )
	{
		d_PlayerDirect = ( nDirect ) % 360; 
		if( d_PlayerDirect < 0 )
			d_PlayerDirect += 360;
		d_fresh = true;
	}
	void FalagardSceneMap::setMyselfPos( const Point& pos )
	{
		m_PlayerPos = pos;
		d_MyseflPos = ScenePosToWindowPos( pos );	
		d_fresh = true;
	}
	void FalagardSceneMap::SetSceneFileName( const String& str, const int nUserSharp )
	{
		CreateSceneMap( str.c_str(), nUserSharp );
	}
	void FalagardSceneMap::onParentSized( WindowEventArgs &e )
	{
		FalagardStaticImage::onParentSized( e );
		UpdateAllWindow();
	}
	// 得到当前视口中心在背景上的位置
	Point	FalagardSceneMap::GetCurrentCenter()
	{
		Point pos;
		float fWidthWindowPerBackground = getAbsoluteWidth() / m_pBackground->getAbsoluteWidth();
		float fHeightWindowPerBackground = getAbsoluteHeight() / m_pBackground->getAbsoluteHeight();
		pos.d_x = -m_WindowZero.d_x * fWidthWindowPerBackground + fWidthWindowPerBackground / 2;
		pos.d_y = -m_WindowZero.d_y * fHeightWindowPerBackground + fHeightWindowPerBackground / 2 ;
		return pos;
	}
	// 得到当前视口在背景上的可见范围
	Rect FalagardSceneMap::GetCurrentViewRect() 
	{
		Rect rect;
		float fWidthWindowPerBackground = getAbsoluteWidth() / m_pBackground->getAbsoluteWidth();
		float fHeightWindowPerBackground = getAbsoluteHeight() / m_pBackground->getAbsoluteHeight();
		rect.d_left		= -m_WindowZero.d_x * fWidthWindowPerBackground;
		rect.d_top		= -m_WindowZero.d_y * fHeightWindowPerBackground;
		rect.d_right	= rect.d_left + fWidthWindowPerBackground;
		rect.d_bottom	= rect.d_top + fHeightWindowPerBackground;
		return rect;
	}
	void  FalagardSceneMap::UpdateAllWindow()
	{
		if( m_pBackground == NULL )
			return;
		// 根据缩放比例放置地图
		// 如果是1:1比例，正好全屏
		float fMax = ( d_SceneSize.d_width > d_SceneSize.d_height ) ? d_SceneSize.d_width : d_SceneSize.d_height;
		if( m_nDisplayMode == SCENEMAP_DISPLAY_MODE_FULLSCREEN ) 
		{
			float fScale = getAbsoluteHeight() / getAbsoluteWidth();
			m_fZoomValueX = fScale * d_SceneSize.d_width / fMax ;
			m_fZoomValueY = d_SceneSize.d_height / fMax;
		}
		else 
		{
			m_fZoomValueX = d_SceneSize.d_width * SCENE_MAP_ZOOMSCALE / getAbsoluteWidth(); 
			m_fZoomValueY = d_SceneSize.d_height * SCENE_MAP_ZOOMSCALE / getAbsoluteHeight(); 
		}
		
		Rect winRect;
		winRect = Rect( 0, 0, m_fZoomValueX, m_fZoomValueY );
		if( m_fZoomValueX < 1 ) // 说明宽度不够,就得居中显示了									
		{
			winRect.d_left = ( 1 - m_fZoomValueX ) / 2;
			winRect.d_right = winRect.d_left + m_fZoomValueX;
			m_WindowZero.d_x = winRect.d_left;
		}
		else
		{
			if( m_WindowZero.d_x < 1 - m_fZoomValueX  )
				m_WindowZero.d_x = 1 - m_fZoomValueX;
			if( m_WindowZero.d_x > 0 )
				m_WindowZero.d_x = 0;
		}
		if( m_fZoomValueY < 1 )
		{
			winRect.d_top = ( 1 - m_fZoomValueY ) / 2;
			winRect.d_bottom = winRect.d_top + m_fZoomValueY;
			m_WindowZero.d_y = winRect.d_top;
		}
		else
		{
			if( m_WindowZero.d_y < 1 - m_fZoomValueY )
				m_WindowZero.d_y = 1 - m_fZoomValueY;
			if( m_WindowZero.d_y > 0 )
				m_WindowZero.d_y = 0;
		}
		m_pBackground->setRect( Relative, winRect );
		m_pBackground->setSize( Relative, winRect.getSize() );
		
		m_pBackground->setPosition( Relative, m_WindowZero );
		m_pBackground->show();
		UpdateBackgroundItem();
		requestRedraw();
		d_fresh = true;
	}
	void FalagardSceneMap::UpdateBackgroundItem()
	{
		int i = 0;
		char szWindowName[ 128 ];
		float fOffsetX, fOffsetY; // 这是个比例值
	
		fOffsetX = float( ( int( d_SceneSize.d_width ) % SCENE_MAP_PER_PIC ) / float( SCENE_MAP_PER_PIC ) );  
		fOffsetY = float( ( int( d_SceneSize.d_height ) % SCENE_MAP_PER_PIC ) / float( SCENE_MAP_PER_PIC ) );
		m_nXCount =  int( d_SceneSize.d_width ) / SCENE_MAP_PER_PIC;
		m_nYCount =  int( d_SceneSize.d_height ) / SCENE_MAP_PER_PIC;
		if( fOffsetX ) // 如果有多余就多加一块
			m_nXCount += 1;
		else 
			fOffsetX = 1;
		if( fOffsetY )
			m_nYCount += 1;
		else 
			fOffsetY = 1;
		Size size;
		float fPicSizeX = 1280, fPicSizeY = 1280;
		if( m_nDisplayMode == SCENEMAP_DISPLAY_MODE_FULLSCREEN )
		{
			float fMax = ( d_SceneSize.d_width > d_SceneSize.d_height ) ? d_SceneSize.d_width : d_SceneSize.d_height;
			float fScale = getAbsoluteHeight() / getAbsoluteWidth();
			fPicSizeX *= fScale * getAbsoluteWidth() / ( fMax * SCENE_MAP_ZOOMSCALE ); 
			fPicSizeY *= getAbsoluteHeight() / ( fMax * SCENE_MAP_ZOOMSCALE );
		}
		Point pos( 0, 0);
		m_nXCount = 1;
		m_nYCount = 1;
		for( i = 0; i < m_nXCount; i ++ )
		{
			if( i == m_nXCount - 1 && fOffsetX != 1 ) // 如果是x最后一个,并且图有多余得
				size.d_width = float( fOffsetX * fPicSizeX );
			else
				size.d_width = fPicSizeX;
			
			for( int j = 0; j < m_nYCount; j ++ ) // 创建所有得图片，创建响应得窗口
			{
				
				if( j == m_nYCount - 1 && fOffsetY != 1 )
					size.d_height = float( fOffsetY * fPicSizeY);
				else
					size.d_height = fPicSizeY;

				sprintf( szWindowName, "%s_map_%d_%d", getName().c_str(),i, j ); 
				Window* pWindow = (StaticImage*)WindowManager::getSingleton().getWindow( szWindowName );
				if( pWindow )
				{
					pWindow->setPosition( Absolute, pos );
					pWindow->setSize( Absolute, size );
				}
				pos.d_y += size.d_height;

			}
			pos.d_x += size.d_width; 
			pos.d_y = 0;
		}
	}
	void FalagardSceneMap::SetZoomMode( int nMode )
	{ 
		m_nDisplayMode = nMode; 
		UpdateAllWindow();
	};
	const Image* FalagardSceneMap::getImageByName( const String& str, Rect rect, const char * Type) // 根据名字得到image
	{
		const String imagename = "Scenemap_view"; // 所有的地图都有这个名字，
		Imageset* pSet;
		char szFullFileName[ 128 ];
		sprintf( szFullFileName, "%s.%s", str.c_str() ,Type);
		Point offset( 0, 0 );
		if( !ImagesetManager::getSingleton().isImagesetPresent( str ) )
		{
			pSet = ImagesetManager::getSingleton().createImagesetFromImageFile( str.c_str(), szFullFileName, "General" );
		}
		pSet = ImagesetManager::getSingleton().getImageset( str );
		if( pSet ) 
		{
			if( !pSet->isImageDefined( imagename ) )
				pSet->defineImage( imagename, rect, offset );
			return &pSet->getImage( imagename );
		}
		return NULL;
		
	}
	void	FalagardSceneMap::createBackground( const String& strFileName, const int nUserSharp )
	{
		// 场景被分为128 X 128得小块，对应图片得256X256
		int i = 0;
		char szImagesetName[ 128 ];
		char szWindowName[ 128 ];
		sprintf( szWindowName, "%s_auto_background", getName().c_str() ); 
		if( WindowManager::getSingleton().isWindowPresent( szWindowName ) ) //如果窗口存在，就删掉
		{
			WindowManager::getSingleton().destroyWindow( szWindowName );
		}
		WindowManager::getSingleton().createWindow( "DefaultWindow", szWindowName );
		m_pBackground = WindowManager::getSingleton().getWindow( szWindowName );
		addChildWindow( m_pBackground );
		m_pBackground->setMaximumSize( Size( 100, 100 ) );
		m_pBackground->setRiseOnClickEnabled( false );
		m_pBackground->setMouseMoveHollow( true );
		
		float fOffsetX, fOffsetY; // 这是个比例值
		float fPicSizeX = 1024, fPicSizeY = 1024;
		fOffsetX = float( ( int( d_SceneSize.d_width ) % SCENE_MAP_PER_PIC ) / float( SCENE_MAP_PER_PIC ) );  
		fOffsetY = float( ( int( d_SceneSize.d_height ) % SCENE_MAP_PER_PIC ) / float( SCENE_MAP_PER_PIC ) );
		m_nXCount =  int( d_SceneSize.d_width ) / SCENE_MAP_PER_PIC;
		m_nYCount =  int( d_SceneSize.d_height ) / SCENE_MAP_PER_PIC;
		if( fOffsetX ) // 如果有多余就多加一块
			m_nXCount += 1;
		else 
			fOffsetX = 1;
		if( fOffsetY )
			m_nYCount += 1;
		else 
			fOffsetY = 1;
			

		float fWidthPerItem = 1 / d_SceneSize.d_width;
		float fHeightPerItem = 1 / d_SceneSize.d_height;
		m_nXCount = 1;
		m_nYCount = 1;
		for( i = 0; i < m_nXCount; i ++ )
		{
			for( int j = 0; j < m_nYCount; j ++ ) // 创建所有得图片，创建响应得窗口
			{
				sprintf( szImagesetName, "%s", strFileName.c_str() );
				
				Rect rect( 0, 0, float( fPicSizeX ), float( fPicSizeY ) );
				Point offset( 0, 0 );
				if( i == m_nXCount - 1 && fOffsetX != 1 ) // 如果是x最后一个,并且图有多余得
					rect.d_right = float( fOffsetX * fPicSizeX );
				if( j == m_nYCount - 1 && fOffsetY != 1 )
					rect.d_bottom = float( fOffsetY * fPicSizeY);
				// 创建这个图片所附属得窗口
				sprintf( szWindowName, "%s_map_%d_%d", getName().c_str(),i, j ); 
				Window* pWindow = NULL;
				if( !WindowManager::getSingleton().isWindowPresent( szWindowName ) ) //如果窗口不存在，就创建一个
				{
					pWindow = WindowManager::getSingleton().createWindow( "WoWLook/StaticImage", szWindowName );
					m_pBackground->addChildWindow( pWindow );
				}
				pWindow = WindowManager::getSingleton().getWindow( szWindowName );
				if( pWindow ) 
				{
					((StaticImage*)pWindow)->setImage( getImageByName( szImagesetName, rect ,"jpg") ); 
					pWindow->setRiseOnClickEnabled( false );
					pWindow->setMouseHollow( true );
				}
				if( i == 0 && j == 0 ) //左上角窗口
					m_pLTWindow = pWindow;
				if( i == m_nXCount - 1 && j == m_nYCount - 1 ) // 右下窗口
					m_pRBWindow = pWindow;

				if(nUserSharp == LookLikeRound)
					pWindow->setUserSharp(LookLikeRound);
				// 如果地图不是正方形的，
				CEGUI::Rect winRect;
				winRect.d_left= i * SCENE_MAP_PER_PIC / d_SceneSize.d_width;
				winRect.d_top = j * SCENE_MAP_PER_PIC / d_SceneSize.d_height;
				winRect.d_right = winRect.d_left + ( fOffsetX * SCENE_MAP_PER_PIC) / d_SceneSize.d_width;
				winRect.d_bottom = winRect.d_top + ( fOffsetY * SCENE_MAP_PER_PIC) / d_SceneSize.d_height;
				pWindow->setRect( Relative, winRect );
				pWindow->setSize( Relative, winRect.getSize() );
				pWindow->show();
				pWindow->setMouseHollow( true ); 
				pWindow->setMouseMoveHollow( true ); 
				pWindow->setMaximumSize( Size( 100, 100 ) );
				pWindow->setRiseOnClickEnabled( false );
			}
		}

	}
	void FalagardSceneMap::CreateSceneMap( const String& strFileName, const int nUserSharp )
	{
		char szWindowName[ 128 ];
		int i;
		createBackground( strFileName, nUserSharp );
		for( i = 0; i < 5; i ++ )
		{
			sprintf( szWindowName, "Scenemap_Player_%d", i );
			if( !WindowManager::getSingleton().isWindowPresent( szWindowName ) ) //如果窗口不存在，就创建一个
				WindowManager::getSingleton().createWindow( "WoWLook/StaticImage", szWindowName );
			m_pFriendWindow[ i ] = (StaticImage*)WindowManager::getSingleton().getWindow( szWindowName );
			m_pBackground->addChildWindow( m_pFriendWindow[ i ] );
			m_pFriendWindow[ i ]->hide();
			//m_pFriendWindow[ i ]->setAlwaysOnTop( true );
			((StaticImage*)m_pFriendWindow[ i ])->setImage( d_PlayerImage );
			

		}
		sprintf( szWindowName, "%s_Auto_MyselfWindow", getName().c_str() );  
		if( !WindowManager::getSingleton().isWindowPresent( szWindowName ) ) //如果窗口不存在，就创建一个
			WindowManager::getSingleton().createWindow( "WoWLook/StaticImage", szWindowName );
		m_pMySelfWindow = (StaticImage*)WindowManager::getSingleton().getWindow( szWindowName );
		m_pBackground->addChildWindow( m_pMySelfWindow );
		m_pMySelfWindow->setAlwaysOnTop( true );
		m_pMySelfWindow->show();
		m_pBackground->setMouseHollow( true );
		m_pBackground->setMouseMoveHollow( true );

		UpdateAllWindow();
		//SetViewByCenter( d_MyseflPos );
			// 创建5个队友得窗口
		
	}
	void	FalagardSceneMap::setSceneMapCenter()
	{
		SetViewByCenter( d_MyseflPos );
	}
	void FalagardSceneMap::ResetMyself()
	{
		Animate* pAnimate = ( Animate*)AnimateManager::getSingleton().getAnimate( d_MyselfAnimate );
		if( pAnimate && m_pBackground && m_pMySelfWindow )
		{
			int nCurFrame = ( d_PlayerDirect * pAnimate->getFrameNumber() ) / 360;
			Image* pPlayerImage = (Image*)( pAnimate->getFrame( nCurFrame ) );
			
			Point curPos = d_MyseflPos;
			Rect rect;
			float fWidth = ( pPlayerImage->getWidth() / m_pBackground->getAbsoluteWidth() ) / 2;
			float fHeight = ( pPlayerImage->getHeight() / m_pBackground->getAbsoluteHeight() ) / 2;
			rect.d_left   = curPos.d_x -  fWidth;
			rect.d_right  = curPos.d_x + fWidth;
			rect.d_top    = curPos.d_y - fHeight;
			rect.d_bottom = curPos.d_y + fHeight; 
			(( StaticImage* )m_pMySelfWindow)->setImage( pPlayerImage ) ;
			m_pMySelfWindow->setRect( Relative, rect );
			m_pMySelfWindow->setSize( Relative, rect.getSize() );
			
		}
	}
	// 重新计算玩家窗口得位置
	void FalagardSceneMap::ResetPlayerWindow( int nIndex, SCENEMAP_FLAG_DATA& data )
	{
		Point curPos = ScenePosToWindowPos( data.pos );
		Rect rect;
		float fWidth = ( d_PlayerImage->getSourceTextureArea().getWidth() / m_pBackground->getAbsoluteWidth() ) / 2;
		float fHeight = ( d_PlayerImage->getSourceTextureArea().getHeight() / m_pBackground->getAbsoluteHeight() ) / 2;
		rect.d_left   = curPos.d_x - fWidth / 2;
		rect.d_right  = curPos.d_x + fWidth / 2;
		rect.d_top    = curPos.d_y - fHeight / 2;
		rect.d_bottom = curPos.d_y + fHeight / 2;
	
		m_pFriendWindow[ nIndex ]->show();
		m_pFriendWindow[ nIndex ]->setRect( Relative, rect );
		m_pFriendWindow[ nIndex ]->setSize( Relative, rect.getSize() );

		m_pFriendWindow[ nIndex ]->setTooltipText( data.name );
	}

	Point FalagardSceneMap::ScreenPosToScenePos( const Point& pos ) // 屏幕坐标转换到场景坐标
	{

		Point tmp;
		float fMapScreenWidth = getPixelRect().getWidth();
		float fMapScreenHeight = getPixelRect().getHeight();
		// 显得到相对于左上角得屏幕偏移量,比例值
		tmp.d_x = ( pos.d_x - getPixelRect().d_left ) / fMapScreenWidth - m_WindowZero.d_x;
		tmp.d_y = ( pos.d_y - getPixelRect().d_top ) / fMapScreenHeight - m_WindowZero.d_y;

		tmp.d_x *= d_SceneSize.d_width;
		tmp.d_y *= d_SceneSize.d_height;
		return tmp;
	}

	Point FalagardSceneMap::ScreenPosToWindowPosRelative( const Point& pos ) // 屏幕坐标转换到场景坐标f
	{
		Point tmp;
		float fMapScreenWidth = getPixelRect().getWidth();
		float fMapScreenHeight = getPixelRect().getHeight();
		// 显得到相对于左上角得屏幕偏移量,比例值
		tmp.d_x = ( pos.d_x - getPixelRect().d_left ) / fMapScreenWidth;
		tmp.d_y = ( pos.d_y - getPixelRect().d_top ) / fMapScreenHeight;
		return tmp;
	}
	Point FalagardSceneMap::ScreenPosToScenePosRelative( const Point& pos ) // 屏幕坐标转换到场景坐标
	{
		Point tmp;
		float fMapScreenWidth = getPixelRect().getWidth();
		float fMapScreenHeight = getPixelRect().getHeight();
		// 显得到相对于左上角得屏幕偏移量,比例值
		tmp.d_x = ( pos.d_x - getPixelRect().d_left ) / fMapScreenWidth - m_WindowZero.d_x;
		tmp.d_y = ( pos.d_y - getPixelRect().d_top ) / fMapScreenHeight - m_WindowZero.d_y;
		return tmp;
	}
	Point FalagardSceneMap::ScenePosToWindowPos( const Point& pos )
	{
		Point tmp;
		tmp.d_x = pos.d_x / d_SceneSize.d_width;
		tmp.d_y = pos.d_y / d_SceneSize.d_height;
		return tmp;
	}

	namespace FalagardSceneMapProperties
	{
		//////////////////////////////////////////////////////////////////////////
		String PlayerImage::get(const PropertyReceiver* receiver) const
		{
			return "";//PropertyHelper::imageToString(static_cast<const FalagardSceneMap*>(receiver)->getObjImage());
		}

		void PlayerImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSceneMap*>(receiver)->setPlayerImage(PropertyHelper::stringToImage(value));
		}
		//////////////////////////////////////////////////////////////////////////
		String MySelfImage::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void MySelfImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSceneMap*>(receiver)->setMyselfImage( value ) ;
		}

	};
 //////////////////////////////////////////////////////////////////////////
    Window* FalagardScenemapFactory::createWindow(const String& name)
    {
        return new FalagardSceneMap(d_type, name);
    }

    void FalagardScenemapFactory::destroyWindow(Window* window)
    {
//        if (window->getType() == d_type)
            delete window;
    }
};