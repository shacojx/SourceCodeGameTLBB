#pragma once
#include "falagardscenemap.h"
#include "IFalagardMinimapWindow.h"
#include <vector>
namespace CEGUI
{
	namespace FalagardRadarProperties
	{
		class IconImage: public Property
		{
			public:
			IconImage() : Property(
				"IconImage",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		class AnimateImage: public Property
		{
			public:
			AnimateImage() : Property(
				"AnimateImage",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class AutoReset: public Property
		{
			public:
			AutoReset() : Property(
				"AutoReset",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
	}
	struct MINIMAP_FLAG_DATA
	{
		Point   pos;
		String  name;
		Size    size;
		Window* m_pWindow;
		MINIMAP_FLAG_DATA()
		{
			pos = Point( 0,0 );
			name.clear();
			size = Size( 0,0 );
		}
	};
	class FalagardRadar : public FalagardSceneMap
	{
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
			IMAGE_TYPE_MYSELF,
		};
		
		enum MINIMAP_POS_TYPE
		{
			POS_TYPE_INVALIDE = -1,
			POS_TYPE_IMAGE = 0,
			POS_TYPE_ANIMATE,
		};
		const Image*		d_AnimyImage;		// 显示敌人的图标
		const Image*		d_ExpNpcImage;		// 特殊npc的图标
		const Image*		d_TeammateImage;	// 队友的图标
		const Image*		d_PlayerImage;		// 别的玩家的图标
		const Image*		d_ExpObjImage;		// 特殊物品的图标
		const Image*		d_MyselfImage;		// 玩家自己的图标
		const Image*		d_PetImage;			// 宠物的图标
		const Image*		d_ScenePosImage;	// 场景激活点
		const Image*		d_FlashImage;		// 大黄点

		String				d_PlayerAnimateName;// 自己的方向动画名字
		String				d_ActiveAnimateName;// 方向点的动画名字
		String				d_FlashAnimateName;	// 闪光点的动画名字

		typedef std::vector< MINIMAP_FLAG_DATA* >  RENDER_POS;
// 所有点的列表
		int			m_aPointerTimer[ IMAGE_TYPE_NUM ]; // 数组的管理指针
		RENDER_POS			m_aAnimyPos;		// 敌人
		RENDER_POS			m_aExpNpcPos;		// 特殊npc
		RENDER_POS			m_aTeammatePos;		// 队友
		RENDER_POS			m_aPlayerPos;		// 人类玩家
		RENDER_POS			m_aExpObjPos;		// 特速物品，
		RENDER_POS  		m_aScenePos;		// 场景传送点
		RENDER_POS  		m_aActivePos;		// 激活方向点
		RENDER_POS  		m_aFlashPos;		// 闪光点
		RENDER_POS  		m_aPetPos;			// 宠物


		float				d_MAXJuLi;
		float				d_ReturnTimer;		// 自动恢复的即使器
		String				m_objTooltip;

		bool				m_bMouseInWindow;

		bool				m_bIsAutoReset;
		
	public:
		static const utf8   WidgetTypeName[];       
	public:
		FalagardRadar(const String& type, const String& name);
		~FalagardRadar(void);

	public:
		void setTypeImage( int nType, const Image* pImage ); //设置image
		const Image* getTypeImage( int nType );

		void setTypeName( int nType, const String& str ); // 设置动画名子
		String getTypeName( int nType );
		virtual void setMAXJuLi( const float Maxjuli ){d_MAXJuLi = Maxjuli;} // 设置显示半径
		virtual	void updateSelf(float elapsed); 
		
		void		UpdateViewRect(); // 根据当前人物位置重值可见范围

		int			GetTypeByName( const String& str );
		String		GetNameByType( int nType );

		void		setAutoReset( bool bAutoReset ){ m_bIsAutoReset = bAutoReset; };

		RENDER_POS* GetListByType( int nType );
	public:
		virtual void setMinimapData( int nType, float fX, float fY, String& szName  );
		virtual void Clear();
		// 设置自己的位置
		virtual void setMyselfPos( const Point& pos );
		virtual void setMyselfDir( const int nDirect );

		virtual void CreateSceneMap( const String& strFileName, const int nUserSharp );


		virtual void	onMouseMove( MouseEventArgs &e );
		virtual void    onMouseEnters( MouseEventArgs& e );
		virtual void    onMouseLeaves( MouseEventArgs& e );

		
	public:
		static FalagardRadarProperties::IconImage			d_IconImageProperty;
		static FalagardRadarProperties::AnimateImage		d_AnimateImageProperty;
		static FalagardRadarProperties::AutoReset			d_AutoRestProperty;
	private:
		void updateFlash();
		void updateActive();
		void updateMyselfDir();
		void updateMyselfPos();
		MINIMAP_FLAG_DATA*  getMiniMapFlagData( int nType, int nIndex );

		int GetType( int nType );

		Point GetActiveArrowImagePos( Rect& rcView, Point& posStart, Point& posEnd );
		const Image* GetActiveArrowImage( Rect& rcView, Point& posStart, Point& posEnd );
	};

	class FALAGARDBASE_API FalagardRadarFactory : public WindowFactory
    {
    public:
        FalagardRadarFactory(void) : WindowFactory(FalagardRadar::WidgetTypeName) { }
        ~FalagardRadarFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
}