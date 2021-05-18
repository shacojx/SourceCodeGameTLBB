

#pragma once

#include "falmodule.h"
#include "ceguiwindowfactory.h"
#include "falstaticimage.h"
#include "FalMinimapProperties.h"
#include "IFalagardSceneMapWindow.h"

namespace CEGUI
{

	namespace FalagardSceneMapProperties
	{
		class PlayerImage: public Property
		{
		public:
			PlayerImage() : Property(
				"PlayerImage",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
		class MySelfImage: public Property
		{
		public:
			MySelfImage() : Property(
				"MySelfImage",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};


	}
	/*
	设计思路
	把地图分成256X256的小块，地图和图片有一个固定的比例，比如，每256象素的图片对应地图上的64单位，这样可以用来定义所有的地图块的命名规则

	*/
	enum 
	{
		SCENEMAP_DISPLAY_MODE_FULLSCREEN = 0,	// 全屏
		SCENEMAP_DISPLAY_MODE_ABSOLTE,		// 真是象素比例
	};
	class FALAGARDBASE_API FalagardSceneMap :	public FalagardStaticImage, public IFalagardSceneMapWindow
	{
	public:

		static const utf8   WidgetTypeName[];       

		static FalagardSceneMapProperties::PlayerImage		d_PlayerImageProperty;		// 队友用得图标
		static FalagardSceneMapProperties::MySelfImage		d_MyselfImageProperty;		// 自己所需要得图片动画

		void   Close();

		virtual void onMouseMove( MouseEventArgs &e );
		virtual void onMouseButtonDown( MouseEventArgs &e );
		virtual void onMouseButtonUp( MouseEventArgs &e );
		virtual void onMouseLeaves( MouseEventArgs& e );
		virtual	void updateSelf(float elapsed); 
		virtual void onMouseEnters(MouseEventArgs& e);

		// 得到鼠标所点得位置
		virtual const Point GetMouseScenePos() const { return d_MouseScenePos; };
		//
		void setPlayerImage( const Image* pImage );

		void setMyselfImage( const String& name );

		//设置自己的方向
		virtual void setMyselfDir( const int nDirect );
		// 设置自己的位置
		virtual void setMyselfPos( const Point& pos );
		// 增加一个队友的位置
		virtual void addPlayerPos( const Point& pos, const String& name );
		// 清楚队友的数据
		virtual void Clear();
		// 设置场景的尺寸
		virtual void SetSceneSize( Size size ) { d_SceneSize = size ; };
		//  设置场景的文件
		virtual void SetSceneFileName( const String& str, const int nUserSharp );
		// 设置显示半径
		virtual void setMAXJuLi( const float Maxjuli ){d_MAXJuLi = Maxjuli;} 
		virtual void SetZoomMode( int nMode );// { m_nDisplayMode = nMode; };

		virtual void setMinimapData( int nType, float fX, float fY, String& szName  ){};
	protected:
		virtual void	onParentSized(WindowEventArgs& e);
		const Image*	getImageByName( const String& str, Rect rect , const char * Type); // 根据名字得到image
		void			createBackground( const String& strFileName, const int nUserSharp );
		void			SetViewByCenter( const Point& pos ); // 设置当前窗口可见范围为以这个位置为中心
		Rect			GetCurrentViewRect(); // 得到当前的可见范围
		Point			GetCurrentCenter();
		virtual	void	setSceneMapCenter();
	protected:

		struct SCENEMAP_FLAG_DATA
		{
			Point   pos;
			String  name;
		};
		typedef std::list< SCENEMAP_FLAG_DATA >  RENDER_POS;
		RENDER_POS	m_aPlayerPos;

		int			d_PlayerDirect;			// 人物的角度
		Point		d_MyseflPos;
		bool		d_fresh;
		float		d_MAXJuLi;
		Image*		d_PlayerImage;
		String		d_MyselfAnimate;
		
		Window*		m_pFriendWindow[ 5 ]; // 5个队友得窗口
		Window*		m_pMySelfWindow; // 自己的窗口
		Window*		m_pLTWindow; // 左上角得地图窗口
		Window*		m_pRBWindow;// 右下角得地图窗口
		Window*		m_pBackground;

		int		m_nXCount;
		int		m_nYCount;
		Point	d_MouseScenePos;
		Point	m_PlayerPos;			// 脱放时记录当前人物位置
		Point   m_WindowZero; // 窗口的坐标原点
		// 场景尺寸
		Size	d_SceneSize;
		// 创建场景地图
		virtual void CreateSceneMap( const String& strFileName, const int nUserSharp );
		void ResetPlayerWindow( int nIndex, SCENEMAP_FLAG_DATA& data );
		Point ScenePosToWindowPos( const Point& pos ); // 场景坐标转到窗口坐标
		Point ScreenPosToScenePos( const Point& pos ); // 屏幕坐标转换到场景坐标f
		Point ScreenPosToScenePosRelative( const Point& pos ); // 屏幕坐标转换到场景坐标
		Point ScreenPosToWindowPosRelative( const Point& pos ); // 屏幕坐标转换到场景坐标
		Point ScreenPosToPicPos( const Point& pos ); // 屏幕坐标转换到图片坐标
		void  ResetMyself();
		void  UpdateAllWindow();
		void  UpdateBackgroundItem();

		float m_fZoomValueX;		// 缩放比例，可变的
		float m_fZoomValueY;		// 缩放比例，可变的
		bool  m_bDraping;
		Point m_ptDrapOffset;
		int   m_nDisplayMode; // 显示模式，分为固定比例和全屏比例
	public:
		FalagardSceneMap( const String& type, const String& name );
		~FalagardSceneMap();

		static const String EventNamespace;
		static const String MouseRButtonDownCtrl;

	};

	class FALAGARDBASE_API FalagardScenemapFactory : public WindowFactory
    {
    public:
        FalagardScenemapFactory(void) : WindowFactory(FalagardSceneMap::WidgetTypeName) { }
        ~FalagardScenemapFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
};