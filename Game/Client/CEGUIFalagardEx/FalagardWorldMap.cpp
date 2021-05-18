#include ".\falagardworldmap.h"
#include "ceguiwindowmanager.h"
#include "windows.h"
#include "elements\ceguipushbutton.h"
#include "ceguiimagesetmanager.h"
#include "ceguiimageset.h"
#include "ceguipropertyhelper.h"
namespace CEGUI
{

	const utf8 FalagardWorldMap::WidgetTypeName[] = "Falagard/WorldMap";
	/*----------------------------------------------------------------------*/
	FalagardWorldMapProperties::CityImageNormal			FalagardWorldMap::d_CityImageNormalProperty;
	FalagardWorldMapProperties::CityImageHover			FalagardWorldMap::d_CityImageHoverProperty;
	FalagardWorldMapProperties::FriendImage1			FalagardWorldMap::d_FriendImage1Property;
	FalagardWorldMapProperties::FriendImage2			FalagardWorldMap::d_FriendImage2Property;
	FalagardWorldMapProperties::FriendImage3			FalagardWorldMap::d_FriendImage3Property;
	FalagardWorldMapProperties::FriendImage4			FalagardWorldMap::d_FriendImage4Property;
	FalagardWorldMapProperties::FriendImage5			FalagardWorldMap::d_FriendImage5Property;
	FalagardWorldMapProperties::BackgroundSize			FalagardWorldMap::d_BackgroundSizeProperty;
	FalagardWorldMapProperties::BackgroundName			FalagardWorldMap::d_BackgroundNameProperty;
	FalagardWorldMapProperties::MySelfImage				FalagardWorldMap::d_MySelfImageProperty;

	
	/*----------------------------------------------------------------------*/
	// FalagardWorldMap
	FalagardWorldMap::FalagardWorldMap(const String& type, const String& name) :
		FalagardStaticImage(type, name)
	{
		CEGUI_START_ADD_STATICPROPERTY( FalagardWorldMap )
		   CEGUI_ADD_STATICPROPERTY( &d_CityImageNormalProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_CityImageHoverProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_FriendImage1Property );
		   CEGUI_ADD_STATICPROPERTY( &d_FriendImage2Property );
		   CEGUI_ADD_STATICPROPERTY( &d_FriendImage3Property );
		   CEGUI_ADD_STATICPROPERTY( &d_FriendImage4Property );
		   CEGUI_ADD_STATICPROPERTY( &d_FriendImage5Property );
		   CEGUI_ADD_STATICPROPERTY( &d_BackgroundSizeProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_BackgroundNameProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_MySelfImageProperty );
		CEGUI_END_ADD_STATICPROPERTY
			m_nLastScene = -1;
		m_pBackground = NULL;
		m_MyselfImage = NULL;
		m_ptZero = Point( 0,0);
		m_bDraping = false;
		
	}

	FalagardWorldMap::~FalagardWorldMap(void)
	{
		m_CityDefineList.clear();
		CITY_DATA_MAP::iterator it;
		for( it = m_aCityList.begin(); it != m_aCityList.end(); it ++ )
		{
			CITY_DATA* pItem = it->second;
			delete pItem;
		}
		m_aCityList.clear();
	}

	void FalagardWorldMap::setMySelfImage( const Image* pImage )
	{
		m_MyselfImage = (Image*)pImage;
	}
	bool	FalagardWorldMap::cityHover(const EventArgs& e)
	{
		int nSceneID = PropertyHelper::stringToInt( (((const WindowEventArgs&) e).window)->getText() );
		SetCityHover( nSceneID, true );
		m_nCurrentScene = nSceneID;
		return true;
	}
	bool	FalagardWorldMap::cityNormal(const EventArgs& e)
	{
		int nSceneID = PropertyHelper::stringToInt( (((const WindowEventArgs&) e).window)->getText() );
		SetCityHover( nSceneID, false );
		m_nCurrentScene = -1;
		return true;
	}
	// 创建新得城市数据
	void FalagardWorldMap::createNewCity( int nCityID, CITY_DATA* data )
	{

		char  szWindowName[ 128 ];
		data->bIsCurrentScene = false;
		data->nFriendNumber = 0;
		// 创建名字窗口
		sprintf( szWindowName, "city_%d_Name_Window", nCityID );
		if( !WindowManager::getSingleton().isWindowPresent( szWindowName ) ) // 如果不存在就创建一个
			data->pCityNameWindow = WindowManager::getSingleton().createWindow( "WoWLook/StaticImage", szWindowName );
		else
			data->pCityNameWindow = WindowManager::getSingleton().getWindow( szWindowName );
		data->szFriendWindowName = szWindowName;
		data->pCityNameWindow->setMouseHollow( false );
		data->pCityNameWindow->setRiseOnClickEnabled( false );
		data->pCityNameWindow->setHookMode( Hook_Center );
		data->pCityNameWindow->setZoomMode( Absolute );
		data->pCityNameWindow->setAlwaysOnTop( true );
		data->pCityNameWindow->setText( PropertyHelper::intToString( nCityID ) ); 

		data->pCityNameWindow->subscribeEvent(Window::EventMouseEnters, Event::Subscriber(&CEGUI::FalagardWorldMap::cityHover, this));
		data->pCityNameWindow->subscribeEvent(Window::EventMouseLeaves, Event::Subscriber(&CEGUI::FalagardWorldMap::cityNormal, this));
		data->pCityNameWindow->hide();
		m_pBackground->addChildWindow( data->pCityNameWindow );

		// 创建城市标志窗口
		sprintf( szWindowName, "city_%d_Flag_Window",nCityID );
		if( !WindowManager::getSingleton().isWindowPresent( szWindowName ) ) // 如果不存在就创建一个
			data->pCityWindow = WindowManager::getSingleton().createWindow( "WoWLook/StaticImage", szWindowName );
		else
			data->pCityWindow = WindowManager::getSingleton().getWindow( szWindowName );
		data->szCityWindowName = szWindowName;
		data->pCityWindow->setMouseHollow( false );
		data->pCityWindow->setRiseOnClickEnabled( false );
		data->pCityWindow->setHookMode( Hook_Center );
		data->pCityWindow->setZoomMode( Absolute );
		data->pCityWindow->setAlwaysOnTop( true );
		data->pCityWindow->setText( PropertyHelper::intToString( nCityID ) ); 
		data->pCityWindow->subscribeEvent(Window::EventMouseEnters, Event::Subscriber(&CEGUI::FalagardWorldMap::cityHover, this));
		data->pCityWindow->subscribeEvent(Window::EventMouseLeaves, Event::Subscriber(&CEGUI::FalagardWorldMap::cityNormal, this));
		data->pCityWindow->hide();
		m_pBackground->addChildWindow( data->pCityWindow );

		// 创建队友窗口
		// 创建城市标志窗口
		sprintf( szWindowName, "Worldmap_city_%d_Friend_Window", nCityID );
		if( !WindowManager::getSingleton().isWindowPresent( szWindowName ) ) // 如果不存在就创建一个
			data->pFriendWindow = WindowManager::getSingleton().createWindow( "WoWLook/StaticImage", szWindowName );
		else
			data->pFriendWindow = WindowManager::getSingleton().getWindow( szWindowName );
		data->pFriendWindow->hide();
		data->pFriendWindow->setHookMode( Hook_Center );
		data->pFriendWindow->setZoomMode( Absolute );
		data->pFriendWindow->setAlwaysOnTop( true );
		data->szFriendWindowName = szWindowName;
		m_pBackground->addChildWindow( data->pFriendWindow );

		sprintf( szWindowName, "Worldmap_city_%d_MySelf_Window", nCityID );
		if( !WindowManager::getSingleton().isWindowPresent( szWindowName ) ) // 如果不存在就创建一个
			data->pMyselfWindow = WindowManager::getSingleton().createWindow( "WoWLook/StaticImage", szWindowName );
		else
			data->pMyselfWindow = WindowManager::getSingleton().getWindow( szWindowName );
		data->pMyselfWindow->hide();
		data->pMyselfWindow->setHookMode( Hook_Center );
		data->pMyselfWindow->setZoomMode( Absolute );
		data->pMyselfWindow->setAlwaysOnTop( true );
		data->szMyselfWindowName = szWindowName;
		m_pBackground->addChildWindow( data->pMyselfWindow );
	


	}

	FalagardWorldMap::CITY_DATA* FalagardWorldMap::getCity( int nCityID )
	{
		CITY_DATA_MAP::iterator it = m_aCityList.find( nCityID );
		if( it != m_aCityList.end() )
			return ( it->second );
		CITY_DATA  *data = new CITY_DATA;
		createNewCity( nCityID, data );
		m_aCityList[ nCityID ] = data;
		it = m_aCityList.find( nCityID );
		return it->second;
	}

	float FalagardWorldMap::AbsoluteToRelativeX( float fValue )
	{
		return fValue / ( m_BackGroundSize.d_width * 256 );
	}
	float FalagardWorldMap::AbsoluteToRelativeY( float fValue )
	{
		return fValue / ( m_BackGroundSize.d_height * 256 );
	}
	float FalagardWorldMap::AbsoluteToRelativeX2( float fValue )
	{
		CEGUI::Size size = CEGUI::System::getSingleton().getRenderer()->getSize();
		return fValue / size.d_width;
	}
	float FalagardWorldMap::AbsoluteToRelativeY2( float fValue )
	{
		CEGUI::Size size = CEGUI::System::getSingleton().getRenderer()->getSize();
		return fValue / size.d_height;
	}
	void FalagardWorldMap::updataBackground()
	{
		if( m_pBackground == NULL )
			return;
		// 根据缩放比例放置地图
		// 如果是1:1比例，正好全屏

		Size size;
		size.d_width = m_BackGroundSize.d_width * 256 / getAbsoluteWidth();
		size.d_height = m_BackGroundSize.d_height * 256 / getAbsoluteHeight();
		if( m_ptZero.d_x < 1 - size.d_width )
			m_ptZero.d_x = 1 - size.d_width;
		if( m_ptZero.d_y < 1 - size.d_height )
			m_ptZero.d_y = 1 - size.d_height;
		if( m_ptZero.d_x > 0 )
			m_ptZero.d_x = 0;
		if( m_ptZero.d_y > 0 )
			m_ptZero.d_y = 0;
		if( size.d_width < 1 )
			m_ptZero.d_x = ( 1 - size.d_width ) / 2;
		if( size.d_height < 1 )
			m_ptZero.d_y = ( 1 - size.d_height ) / 2;
		
		m_pBackground->setSize( Relative, size );
		m_pBackground->setPosition( Relative, m_ptZero );
		m_pBackground->show();
		requestRedraw();
	}
	void FalagardWorldMap::createBackground()
	{
		const int WidthPerPic = 256;
		const int HeightPerPic = 256;
		int nX = int( m_BackGroundSize.d_width );
		int nY = int( m_BackGroundSize.d_height ); 
		char strBackgroundName[ 128 ];
		char strImageName[ 128 ];
		char strImageFileName[ 128 ];

		sprintf( strBackgroundName, "%s_auto_background", getName().c_str() );
		// 创建背景底板
		if( !WindowManager::getSingleton().isWindowPresent(strBackgroundName) )
			m_pBackground = WindowManager::getSingleton().createWindow( "DefaultWindow", strBackgroundName );
		else
			m_pBackground = WindowManager::getSingleton().getWindow( strBackgroundName );
		m_pBackground->setMaximumSize( Size( 100, 100 ) );
		addChildWindow( m_pBackground );
		
		int nIndex = 0;
		for( int i = 0; i < nY; i ++ )
			for( int j = 0; j < nX; j ++ )
			{
				sprintf( strImageFileName, "LargeMap\\%s%02d.jpg", m_strBackgroundName.c_str(), nIndex );
				sprintf( strImageName, "%s%02d_auto_imageset", m_strBackgroundName.c_str(), nIndex );
				sprintf( strBackgroundName, "%s_auto_background_item%d", getName().c_str(), nIndex );
				Imageset* pImageset = NULL;
				const Image*	pImage;
				if( !ImagesetManager::getSingleton().isImagesetPresent( strImageName ) )
				{
					ImagesetManager::getSingleton().createImagesetFromImageFile( strImageName, strImageFileName );
				}
				pImageset = ImagesetManager::getSingleton().getImageset( strImageName );
				if( pImageset )
				{
					Rect rect( 0, 0, float( WidthPerPic ), float( HeightPerPic ) );
					Point offset( 0, 0 );
					if( !pImageset->isImageDefined( "view" ) )
						pImageset->defineImage( "view", rect, offset );
					pImage = &pImageset->getImage( "view" );
				}
				if( pImage && pImageset )
				{
					Window* pWindow = NULL;
					if( !WindowManager::getSingleton().isWindowPresent(strBackgroundName) ) 
						pWindow = WindowManager::getSingleton().createWindow( "WoWLook/StaticImage", strBackgroundName );
					else
						pWindow = WindowManager::getSingleton().getWindow( strBackgroundName );
					
					pWindow->setRiseOnClickEnabled( false );
					pWindow->setAlwaysOnTop( false );
					pWindow->setMouseRButtonHollow( true );
					pWindow->setMouseLButtonHollow( true );
					pWindow->setMouseHollow( true );
					m_pBackground->addChildWindow( pWindow );
					Rect rect;
					rect.d_left = j / m_BackGroundSize.d_width;
					rect.d_right = ( j + 1 ) / m_BackGroundSize.d_width;
					rect.d_top = i / m_BackGroundSize.d_height;
					rect.d_bottom = ( i + 1 ) / m_BackGroundSize.d_height;
					pWindow->setRect( Relative, rect );
					pWindow->setSize( Relative, rect.getSize() );
					(( StaticImage*)pWindow)->setImage( pImage );
				}
				nIndex ++;
			}
			m_pBackground->subscribeEvent(Window::EventMouseMove, Event::Subscriber(&CEGUI::FalagardWorldMap::onDrapMove, this));
			m_pBackground->subscribeEvent(Window::EventMouseRButtonDown, Event::Subscriber(&CEGUI::FalagardWorldMap::onDrapStart, this));
			m_pBackground->subscribeEvent(Window::EventMouseButtonUp, Event::Subscriber(&CEGUI::FalagardWorldMap::onDrapEnd, this));

			
	}

	bool FalagardWorldMap::onDrapMove( const EventArgs& e)
	{
		if( m_bDraping ) // 正在托动
		{
			Point pCurrent;
			pCurrent.d_x = ( m_ptDrapOffset.d_x - ((const MouseEventArgs&) e).position.d_x ) / getPixelRect().getWidth();
			pCurrent.d_y = ( m_ptDrapOffset.d_y - ((const MouseEventArgs&) e).position.d_y ) / getPixelRect().getHeight();
			m_ptZero.d_x = m_ptDrapOldPos.d_x - pCurrent.d_x;
			m_ptZero.d_y = m_ptDrapOldPos.d_y - pCurrent.d_y;
			updataBackground();
		}
		return true;
	}
	bool FalagardWorldMap::onDrapStart( const EventArgs& e)
	{
		if( m_nCurrentScene == -1  )
		{
			m_ptDrapOffset.d_x = ((const MouseEventArgs&) e).position.d_x;
			m_ptDrapOffset.d_y = ((const MouseEventArgs&) e).position.d_y;
			m_ptDrapOldPos = m_ptZero;
			m_bDraping = true;
		}
		return true;
	}
	bool FalagardWorldMap::onDrapEnd( const EventArgs& e)
	{
		m_bDraping = false;
		return true;
	}

	// 整理所有得窗口数据
	void FalagardWorldMap::resetWorldMap()
	{
		if( m_pBackground == NULL )
			return;
		CITY_DATA_MAP::iterator it;
		CITY_DEFINE::iterator itType;
		Point pos;
		Size size;
		Rect rect;
		updataBackground();
		float fBackgroundWidtgh = m_pBackground->getAbsoluteWidth();
		float fBackgroundHeight = m_pBackground->getAbsoluteHeight();
		for( it = m_aCityList.begin(); it != m_aCityList.end(); it ++ )
		{
			CITY_DATA *data = it->second;
			S_CITY_DEFINE city;
			itType = m_CityDefineList.find( data->szType );
			if( itType != m_CityDefineList.end() )
			{
				
				city = itType->second;
				// 计算图标得位置
				((StaticImage*)(data->pCityWindow))->setImage(city.pNormalImage);
				data->pCityWindow->setHookPosition( data->cityPos );
				Size size = city.pNormalImage->getSourceTextureArea().getSize();
				data->pCityWindow->setSize( Absolute, size ); 
				data->pCityWindow->show();

				((StaticImage*)(data->pCityNameWindow))->setImage(data->pNameNormalImage);
				data->pCityNameWindow->setHookPosition( data->namePos );
				size = data->pNameNormalImage->getSourceTextureArea().getSize();
				data->pCityNameWindow->setSize( Absolute, size );
				data->pCityNameWindow->show();
			}
			if( data->bIsCurrentScene )
			{
				if( m_MyselfImage )
				{
					data->pMyselfWindow->setHookPosition( data->cityPos );
					size = m_MyselfImage->getSourceTextureArea().getSize();
					((StaticImage*)data->pMyselfWindow)->setImage( m_MyselfImage );
					data->pMyselfWindow->setSize( Absolute, size );
					data->pMyselfWindow->show();
				}

			}
			// 计算用那个队友得图
			if( data->nFriendNumber > 0 )
			{
				data->pFriendWindow->setTooltipText( data->pTooltip );
				Image* pImage = m_FriendImage[ data->nFriendNumber - 1 ];
				if( pImage )
				{
					data->pFriendWindow->setHookPosition( data->cityPos );
					size = pImage->getSourceTextureArea().getSize();
					((StaticImage*)data->pFriendWindow)->setImage( pImage );
					data->pFriendWindow->setSize( Absolute, size );
					data->pFriendWindow->show();
				}
			}
			else
			{
				data->pFriendWindow->hide();
			}
		}
		
	}
	void	FalagardWorldMap::onParentSized(WindowEventArgs& e)
	{
		FalagardStaticImage::onParentSized( e );
		resetWorldMap();
	}
	void FalagardWorldMap::clearPlayerInfo()
	{
		CITY_DATA_MAP::iterator it;
		for( it = m_aCityList.begin(); it != m_aCityList.end(); it ++ )
		{
			CITY_DATA *data = it->second;
			data->nFriendNumber = 0;
			data->pTooltip.clear();
		}
	}
	// 往一个城市里防一个玩家
	void FalagardWorldMap::addPlayerToCity( int nCityID, const String& szPlayerName )
	{
		CITY_DATA* data = getCity( nCityID );
		data->nFriendNumber ++;
		if( !data->pTooltip.empty() )
			data->pTooltip += ( utf8* )"\n";
		data->pTooltip += szPlayerName;
	}
	void FalagardWorldMap::setFriendImage( int nFriend, const Image* pImage )
	{
		m_FriendImage[ nFriend ] = (Image*)pImage;
	}
	void FalagardWorldMap::setCityNameHoverImage( int nCityID, const String& szImageset, const String& szImage )
	{
		const Imageset* iset = ImagesetManager::getSingleton().getImageset( szImageset );
		CITY_DATA* data = getCity( nCityID );
		data->pNameNormalImage = (Image*)( &iset->getImage( szImage ) );
	}
	void FalagardWorldMap::setCityNameNormalImage( int nCityID, const String& szImageset, const String& szImage )
	{
		const Imageset* iset = ImagesetManager::getSingleton().getImageset( szImageset );
		CITY_DATA* data = getCity( nCityID );
		data->pNameHoverImage = (Image*)( &iset->getImage( szImage ) );
	}
	void FalagardWorldMap::setCityPos( int nCityID, const Point& pos )
	{
		CITY_DATA* data = getCity( nCityID );
		data->cityPos = pos;
	}
	void FalagardWorldMap::setCityNamePos( int nCityID, const Point& pos )
	{
		CITY_DATA* data = getCity( nCityID );
		data->namePos = pos;
	}
	void FalagardWorldMap::setCityType( int nCityID, const String& szType )
	{
		CITY_DATA* data = getCity( nCityID );
		data->szType = szType;
	}
	void FalagardWorldMap::setCityCurrentScene( int nCityID, int bCurrentScene )
	{
		CITY_DATA* data = getCity( nCityID );
		data->bIsCurrentScene = bCurrentScene;
		data = getCity( nCityID );
	}
	void FalagardWorldMap::setCityNormalImage( const String& szType, const Image* pImage )
	{
		CITY_DEFINE::iterator it = m_CityDefineList.find( szType );
		if( it != m_CityDefineList.end() ) //  没有找到
		{
			// 默认没有设置hover图片就按照普通设置
			it->second.pNormalImage = (Image*)pImage;
			it->second.pHoverImage = (Image*)pImage;
			return;
		}
		S_CITY_DEFINE data;
		data.pNormalImage = (Image*)pImage;
		m_CityDefineList[ szType ] = data;

	}
	void FalagardWorldMap::SetCityHover( int nSceneID, bool bHover )
	{
		CITY_DATA_MAP::iterator it;
		CITY_DEFINE::iterator itType;
		if( nSceneID != -1 )
		{
			it = m_aCityList.find( nSceneID );
			if( it != m_aCityList.end() )
			{
				CITY_DATA *data = it->second;
				S_CITY_DEFINE city;
				itType = m_CityDefineList.find( data->szType );
				if( itType != m_CityDefineList.end() )
				{
					city = itType->second;
					if( bHover )
					{
						((StaticImage*)(data->pCityWindow))->setImage(city.pHoverImage);
						((StaticImage*)(data->pCityNameWindow))->setImage( data->pNameHoverImage);
					}
					else
					{
						((StaticImage*)(data->pCityWindow))->setImage(city.pNormalImage);
						((StaticImage*)(data->pCityNameWindow))->setImage( data->pNameNormalImage);
					}
				}
			}
		}
	}
	

	void FalagardWorldMap::setCityHoverImage( const String& szType, const Image* pImage )
	{
		CITY_DEFINE::iterator it = m_CityDefineList.find( szType );
		if( it != m_CityDefineList.end() ) //  没有找到
		{
			it->second.pHoverImage = (Image*)pImage;
			return;
		}
		S_CITY_DEFINE data;
		data.pHoverImage = (Image*)pImage;
		m_CityDefineList[ szType ] = data;
	}
	void FalagardWorldMap::setBackgroundName( const String& strName )
	{
		m_strBackgroundName = strName;
	}
	void FalagardWorldMap::setBackgroundSize( const Size& size )
	{
		m_BackGroundSize = size;
	}
	/*----------------------------------------------------------------------*/
	// FalagardWorldMapFactory
    Window* FalagardWorldMapFactory::createWindow(const String& name)
    {
        return new FalagardWorldMap(d_type, name);
    }

    void FalagardWorldMapFactory::destroyWindow(Window* window)
    {
       delete window;
    }
};