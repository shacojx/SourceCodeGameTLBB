#pragma once
#include "FalStaticText.h"

namespace CEGUI
{

	namespace FalagardResizeTextProperties 
	{
		////////////////////////////////////////////////////////////////////////
		// 瞄准的窗口名字,必须是同级的
		class AimWindow : public Property
		{
		public:
			AimWindow() : Property(
				"AimWindow",
				"Property to get/set empty status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		////////////////////////////////////////////////////////////////////////
		// 瞄准的窗口的角,必须是同级的
		class AimPosition : public Property
		{
		public:
			AimPosition() : Property(
				"AimPosition",
				"Property to get/set empty status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		
	}


	class FalagardResizeText :	public FalagardStaticText
	{
	public:

		static const utf8   WidgetTypeName[];       //!< type name for this widget.

		FalagardResizeText(const String& type, const String& name);
		~FalagardResizeText(void);


		void SetAimWindow( const String& strWindowName );
		void SetAimPosition( const String& strPositionName );

		Window* GetAimWindow(){ return m_pAimWindow; };
	protected:
		Window*			m_pAimWindow;
		enum POSITIONTYPE
		{
			InvalidPosition = -1,
			LeftTop = 0,
			RightTop = 1,
			RightBottom = 2,
			LeftBottom = 3,
		};
		enum LOCKTYPE
		{
			LockWidth  = 0,
			LockHeight = 1,
		};
		int				m_nAimPositionType;
		int				m_nSelfPositionType;
		int				m_nLockType;
	public:
		static FalagardResizeTextProperties::AimWindow     d_AimWindowPropety;
		static FalagardResizeTextProperties::AimPosition   d_AimPositionPropety;

		void	ResizeWindow();
	protected:
		virtual void populateRenderCache();
		virtual void onTextChanged( WindowEventArgs& e );
		virtual void onSized(WindowEventArgs& e);
		void		positionSelf();

		Size getTextSize() const; // 得到字符的尺寸
		float getTextHeight() const;
	};

	class FALAGARDBASE_API FalagardResizeTextWindowFactory : public WindowFactory
	{
	public:
		FalagardResizeTextWindowFactory(void) : WindowFactory(FalagardResizeText::WidgetTypeName) { }
		~FalagardResizeTextWindowFactory(void){}
		Window* createWindow(const String& name);
		void destroyWindow(Window* window);
	};
};