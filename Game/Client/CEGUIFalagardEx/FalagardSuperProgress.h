#pragma once
#include "falprogressbar.h"
#include "ceguirenderableimage.h"
namespace CEGUI
{

	namespace FalagardSuperProgressProperties
	{
		// 进度条得图片
		class ProgressImage: public Property
		{
		public:
			ProgressImage() : Property(
				"ProgressImage",
				"Property to get/set empty status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		// 进度条特效得颜色
		class EffectColor: public Property
		{
		public:
			EffectColor() : Property(
				"EffectColor",
				"Property to get/set empty status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		// 设置一级效果,用于产生特效
		class EffectColorLevel: public Property
		{
		public:
			EffectColorLevel() : Property(
				"EffectColorLevel",
				"Property to get/set empty status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		// 设置变化速度, 毫秒
		class MoveSpeed: public Property
		{
		public:
			MoveSpeed() : Property(
				"MoveSpeed",
				"Property to get/set empty status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};


	};


	class FalagardSuperProgress : public ProgressBar
	{
	public:
		static const utf8   WidgetTypeName[];       //!< type name for this widget.
	public:
		FalagardSuperProgress( const String& type, const String& name );
		~FalagardSuperProgress(void);


		static FalagardSuperProgressProperties::ProgressImage			d_effectImageProperty;
		static FalagardSuperProgressProperties::EffectColor				d_effectAlphaProperty;
		static FalagardSuperProgressProperties::EffectColorLevel		d_EffectColorLevelProperty;

		static FalagardSuperProgressProperties::MoveSpeed				d_MoveSpeedProperty;


		void	setEffectImage( const Image* pImage );		// 设置进度条得底板图片
		void    setEffectColor( ColourRect& color );
		void	setEffectColorLevel( int nIndex, ColourRect& color);
		void	setMoveSpeed( float fMoveSpeed ) { d_MoveSpeed = fMoveSpeed; };
	protected:
		virtual void	populateRenderCache();
		virtual void	updateSelf(float elapsed);
	private:
		ColourRect			d_EffectColorLevel[ 11 ];// 进度条3级得颜色
		ColourRect			d_EffectColor;	// 特效颜色
		
		RenderableImage		d_BackImage;  // 底板得图片

		float				d_MoveSpeed;
		float				d_curPos;


		Rect				d_rectProgress;
		Rect				d_rectEffect;

		ColourRect&			getCurColor();

	};

	class FALAGARDBASE_API FalagardSuperProgressFactory : public WindowFactory
    {
    public:
        FalagardSuperProgressFactory(void) : WindowFactory( FalagardSuperProgress::WidgetTypeName ) { }
        ~FalagardSuperProgressFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
};