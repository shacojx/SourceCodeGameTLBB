#pragma once

#include "falmodule.h"
#include "ceguiwindowfactory.h"
#include "falstaticimage.h"
#include "FalMinimapProperties.h"
#include "IFalagardMinimapWindow.h"
#include <vector>
namespace CEGUI
{

	class FALAGARDBASE_API FalagardMiniMap :	public FalagardStaticImage, public IFalagardMinimapWindow
	{

		
	public:
		enum MINIMAP_IMAGE_TYPE
		{
			IMAGE_TYPE_Animy	= 0, // 敌人
			IMAGE_TYPE_ExpNpc	= 1, // 特殊npc
			IMAGE_TYPE_Team		= 2, // 队友
			IMAGE_TYPE_Player	= 3, // 别的玩家
			IMAGE_TYPE_ExpObj	= 4, // 生长点
			IMAGE_TYPE_Active	= 5, // 激活方向点
			IMAGE_TYPE_ScenePos = 6, // 场景跳转点
			IMAGE_TYPE_Flash	= 7, // 闪光点
			IMAGE_TYPE_Pet		= 8, // 宠物
			IMAGE_TYPE_NUM,
		};
	private:
		struct MINIMAP_FLAG_DATA
		{
			Point   pos;
			String  name;
			Size    size;
			MINIMAP_FLAG_DATA()
			{
				size = Size( 0,0 );
				pos = Point( 0,0 );
				name.clear();
			}
		};
		bool		d_fresh;			// 是否刷新了
		int 		d_displayFlag[ IMAGE_TYPE_NUM ];
		String		d_ImagetSetName;
		Imageset*	d_Imageset;
		Image*		d_MapImage;

		int			d_PlayerDirect;			// 人物的角度

		const Image*		d_AnimyImage;		// 显示敌人的图标
		const Image*		d_ExpNpcImage;		// 特殊npc的图标
		const Image*		d_TeammateImage;	// 队友的图标
		const Image*		d_PlayerImage;		// 别的玩家的图标
		const Image*		d_ExpObjImage;		// 特殊物品的图标
		const Image*		d_MyselfImage;		// 玩家自己的图标
		const Image*		d_PetImage;			// 玩家自己的图标
		const Image*		d_ScenePosImage;	// 场景激活点
	
		Rect				d_PlayerRect;
		String		d_MyselfAnimate;			// 玩家自己是个动画
		String		d_ActiveAnimate;			// 激活方向是个动画
		String      d_FlashAnimate;

		Size		d_RenderSize;				// 显示区域的尺寸
		Point		d_CenterPos;				// 显示区域的中心
		Point		d_OldCenterPos;				// 保存中心位置
		Point		d_MyselfPos;				// 人物得位置
		Point		d_PlayerCoordinate;			// 任务坐标
		
		Size        d_SceneToImageScale;		// 缩放比例
		Rect		d_RenderRect;				// 显示得范围
		
		
		Size		d_ScaleSceneToImage;		// 场景和地图得比例
		Size		d_ImageSize;				// 地图得尺寸

		float		d_ReturnTimer;				// 自动恢复的即使器
		typedef std::vector< MINIMAP_FLAG_DATA >  RENDER_POS;
		// 这里的位置代表的是在纹理上的坐标
		int			m_aPointerTimer[ IMAGE_TYPE_NUM ]; // 数组的管理指针
		RENDER_POS	m_aAnimyPos;
		RENDER_POS	m_aExpNpcPos;
		RENDER_POS	m_aTeammatePos;
		RENDER_POS	m_aPlayerPos;
		RENDER_POS	m_aExpObjPos;
		RENDER_POS  m_aScenePos;
		RENDER_POS  m_aActivePos;
		RENDER_POS  m_aFlashPos;
		RENDER_POS  m_aPetPos;
		
		
	public:
		static const utf8   WidgetTypeName[];       //!< type name for this widget.
		virtual void setMinimapCenter( const Point& pos );
		const Point getMinimapCenter() const;

		void SetSceneSize( const Size& size );
		const Size GetSceneSize() const { return m_SceneSize; };

		void setMinimapRenderSize( const Size& rect );
		const Size getMinimapRenderSize() const;
	
		void setImageset( const String& name );
		void setImagesetByFileName( const String& name );
		const String& getImageset() const;

		virtual void setPlayerDirection( int nDir ); // 人物的角度，
		const int getPlayerDirection() const ;
		// 开始重新设置数据
		void dataRest(); 

		void addAnimyPos( const Point& pos, const String& name );
		void addExpNpcPos( const Point& pos, const String& name );
		void addTeammatePos( const Point& pos, const String& name );
		void addPlayerPos( const Point& pos, const String& name );
		void addExpObjPos( const Point& pos, const String& name );
		void addScenePos( const Point& pos, const String& name );
		void addActiveObjPos( const Point& pos, const String& name );
		void addPetPos( const Point& pos, const String& name );
		


		void setPlayerImage( const Image* pImage );
		void setObjImage( const Image* pImage );
		void setTeammateImage( const Image* pImage );
		void setExpNpcImage( const Image* pImage );
		void setAnimyImage( const Image* pImage );
		void setScenePosImage( const Image* pImage );
		void setMyselfImage( const String& name );
		void setActiveImage( const String& name );
		void setPetmage( const Image* pImage );
		void setFlashImage( const String& name );
		

		const Image* getPlayerImage() const;
		const Image* getObjImage() const;
		const Image* getTeammateImage() const;
		const Image* getExpNpcImage() const;
		const Image* getAnimyImage() const;
		const Image* getScenePosImage()const;
		const Image* getActiveImage( Point& pos );
		const String getMyselfImage() const;
		const Image* getPetPosImage()const;
		const String getFlashImage() const;


		static FalagardMinimapProperties::RenderSize		d_RenderSizeProperty;
		static FalagardMinimapProperties::SceneSize			d_SceneSizeProperty;
		static FalagardMinimapProperties::MinimapImageSet	d_ImagesetProperty;
		static FalagardMinimapProperties::SetMinimapName	d_MapFileNameProperty;
		static FalagardMinimapProperties::PlayerImage		d_PlayerImageProperty;
		static FalagardMinimapProperties::ObjImage			d_ObjImageProperty;
		static FalagardMinimapProperties::MySelfImage		d_MySelfImageProperty;
		static FalagardMinimapProperties::TeammateImage		d_TeammateImageProperty;
		static FalagardMinimapProperties::ExpNpcImage		d_ExpNpcImageProperty;
		static FalagardMinimapProperties::ScenePosImage		d_ScenePosImageProperty;
		static FalagardMinimapProperties::AnimyImage		d_AnimyImageProperty;
		static FalagardMinimapProperties::OpenFlagType		d_OpenFlagTypeProperty;
		static FalagardMinimapProperties::CloseFlagType		d_CloseFlagTypeProperty;
		static FalagardMinimapProperties::IsFlagOpen		d_IsFlagOpenProperty;
		static FalagardMinimapProperties::ClearFlagList		d_ClearFlagListProperty;
		static FalagardMinimapProperties::GetCurCoordinate	d_GetCurCoordinateProperty;
		static FalagardMinimapProperties::ActiveImage		d_ActiveImageProperty;
		static FalagardMinimapProperties::PetImage			d_PetImageProperty;
		static FalagardMinimapProperties::FlashImage		d_FlashImageProperty;

		void setEnabel( int nIndex );
		void setDisable( int nIndex );

		const String getEnableType() const;

		virtual void  clearFlagList();

		const Point getActivePos( Point& pos );

		

	public:
		FalagardMiniMap(const String& type, const String& name);
		~FalagardMiniMap(void);

		virtual void	updateSelf(float elapsed); 
		

		virtual void    onMouseMove( MouseEventArgs& e );
		virtual void    onMouseButtonDown( MouseEventArgs& e );
		virtual void    onMouseButtonUp( MouseEventArgs& e );
		virtual void    onMouseDoubleClicked( MouseEventArgs& e );
		virtual void    onMouseEnters( MouseEventArgs& e );
		virtual void    onMouseLeaves( MouseEventArgs& e );
		virtual void	onSized(WindowEventArgs& e);

		void			drawImagePos( RENDER_POS& pImageList, int nType );
		void			drawImageFlash(); // 画场景激活点，比较特殊，提出来做

	protected:
		virtual	void	populateRenderCache();
	protected:
		bool			m_bDraping;
		bool			m_bSuperlook;
		bool			m_bMouseInWindow;
		Point			m_ptLastPos;

		Point			m_ptCoordinate; // 坐标

		// 场景得尺寸
		Size			m_SceneSize;
		Size			m_TextureSize;

		String			m_objTooltip;
		// 计算当前鼠标激活得tooltip
		void	SetMinimapToolTip( RENDER_POS& pImageList, Point& mousePos, const Image* image, int nType  );
		void	SetMinimapToolTipAction( Point& mousePos );
		// 计算当前鼠标所在位置坐标
		const Point GetMinimapCoordinate( Point& pos) const { return m_ptCoordinate ;}
		
		// 标准画显示区域
		void	NormalArea();	

		// 场景坐标转换到屏幕坐标
		Point ImagePosToWindowPos( Point& pos );
		// 屏幕坐标转换到图像坐标
		Point ScreenPosToImagePos( Point& pos );
		//  从场景坐标转换到图像坐标
		Point ScenePosToImagePos( Point& pos );
		//   从图像坐标转换到场景坐标
		Point ImagePosToScenePos( Point& pos );
		//   得到当前显示得坐标原点
		Point GetCurrentZero();
		// 判断一个象素坐标是否可见
		bool  IsPosVisible( Point& pos );

		// 
		// 得到箭头得显示位置
		Point GetActiveArrowRenderPos( Point& pos );
		// 得到箭头得象素位置
		Point GetActiveArrowImagePos( Point& pos );
		// 得到当前需要显示得箭头图标
		const Image* GetActiveArrowImage( Point& pos );
	public:
		const Point GetSceneCoordinate() const { return m_ptCoordinate; };

		virtual void setMinimapData( int nType, float fX, float fY, String& szName  );

	};
	
	
	class FALAGARDBASE_API FalagardMinimapFactory : public WindowFactory
    {
    public:
        FalagardMinimapFactory(void) : WindowFactory(FalagardMiniMap::WidgetTypeName) { }
        ~FalagardMinimapFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
}	
