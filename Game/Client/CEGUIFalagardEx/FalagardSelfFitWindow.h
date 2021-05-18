#pragma once
#include "falsimpletext.h"
#include "IFalagardSelfFitWindow.h"

namespace CEGUI
{
	namespace FalagardSelfFitProperties
	{
		class MaxWidth : public Property
		{
		public:
			MaxWidth() : Property(
				"MaxWidth",
				"",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};

		class FitType: public Property
		{
		public:
			FitType() : Property(
				"FitType",
				"",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};
		class FitPosition: public Property
		{
		public:
			FitPosition() : Property(
				"FitPosition",
				"",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};
		// 生存周期
		class Life : public Property
		{
		public:
			Life() : Property(
				"Life",
				"",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};
		// FadeOutTime
		class FadeOutTime : public Property
		{
		public:
			FadeOutTime() : Property(
				"FadeOutTime",
				"",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};
		// FadeInTime
		class FadeInTime : public Property
		{
		public:
			FadeInTime() : Property(
				"FadeInTime",
				"",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};
	};
	class FALAGARDBASE_API FalagardSelfFitWindow : public FalagardSimpleText, public IFalagardSelfFitWindow
	{
	public:
		enum SELF_FIT_TYPE
		{
			SELF_FIT_CENTER = 0,
			SELF_FIT_LEFT,
			SELF_FIT_RIGHT,
			SELF_FIT_TOP,
			SELF_FIT_BOTTOM,
			SELF_FIT_LEFTTOP,
			SELF_FIT_LEFTBOTTOM,
			SELF_FIT_RIGHTTOP,
			SELF_FIT_RIGHTBOTOM,
		};
	public:
		static const utf8   WidgetTypeName[];       //!< type name for this widget.
		virtual void setText( const String& text );

		virtual void	updateSelf(float elapsed);
		/*!
		\brief
			Handler called when this window's parent window has been resized.  If this window is the root / GUI Sheet
			window, this call will be made when the screen size changes.

		\param e
			WindowEventArgs object whose 'window' pointer field is set the the window that caused the event; this is typically either
			this window's parent window, or NULL to indicate the screen size has changed.
		*/
		virtual void	onParentSized(WindowEventArgs& e);

		void setMaxWidth( int fWidth ) { d_maxWidth = fWidth; updateSelfSize(); };
		int getMaxWidth(void) const { return d_maxWidth; }

		void setFitType( int nType ) { m_nFitType = nType; updateSelfPosition(); };
		virtual void setCenter(const Point& pos ) { m_ptCenter = pos; updateSelfPosition(); };
		Point getCenter( void ) const { return m_ptCenter; }

		void setLife( float fLife ) { m_fLife = fLife; m_bFadeMode = true; };
		void setFadeInTime( float fTime ) { m_fFadeInTime = fTime; m_bFadeMode = true; };
		void setFadeOutTime( float fTime ) { m_fFadeOutTime = fTime; m_bFadeMode = true; };
	public:
		FalagardSelfFitWindow(const String& type, const String& name);
		~FalagardSelfFitWindow(void);

		static FalagardSelfFitProperties::FitType			d_FitTypeProperty;
		static FalagardSelfFitProperties::FitPosition		d_FitPosProperty;
		static FalagardSelfFitProperties::MaxWidth			d_MaxWidthProperty;
		static FalagardSelfFitProperties::Life				d_LifeProperty;
		static FalagardSelfFitProperties::FadeOutTime		d_FadeOutProperty;
		static FalagardSelfFitProperties::FadeInTime		d_FadeInProperty;

	
	private:
		int			d_maxWidth;		//!< allowed text max pixel width.
		Point m_ptCenter;		// 窗口的锁定位置，以这个点为锁定点在进行自适应
		int   m_nFitType;		// 哪种自适应方式

		bool  m_bFadeMode;
		float m_fCurLife;
		float m_fMaxLife;
		float m_fLife;
		float m_fFadeInTime;
		float m_fFadeOutTime;
		
	protected:
//		virtual void ResizeSelf();
//		virtual Size GetRenderSize();

		virtual void updateSelfSize(void);
		virtual void updateSelfPosition(void);
	};

	class FALAGARDBASE_API FalagardSelfFitFactory : public WindowFactory
    {
    public:
        FalagardSelfFitFactory(void) : WindowFactory(FalagardSelfFitWindow::WidgetTypeName) { }
        ~FalagardSelfFitFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
}; 