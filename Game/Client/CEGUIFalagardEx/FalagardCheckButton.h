#pragma once
#include "falbutton.h"
namespace CEGUI
{
	namespace FalagardCheckButtonProperties
	{
		class SelectImage : public Property
		{
		public:
			SelectImage() : Property(
				"SelectImage",
				"Property to get/set empty status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class Selected: public Property
		{
		public:
			Selected() : Property(
				"Selected",
				"Property to get/set empty status.",
				"False")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class CheckMode : public Property
		{
		public:
			CheckMode() : Property(
				"CheckMode",
				"Property to get/set empty status.",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
	};
	class FALAGARDBASE_API FalagardCheckButton :	public FalagardButton
	{
	public:
		  static const utf8  WidgetTypeName[];       //!< type name for this widget.
	public:
		FalagardCheckButton( const String& type, const String& name );
		~FalagardCheckButton(void);

		void	setSelectImage(const RenderableImage* image);


		static FalagardCheckButtonProperties::SelectImage	d_selectImageProperty;
		static FalagardCheckButtonProperties::Selected		d_selectedProperty;
		static FalagardCheckButtonProperties::CheckMode		d_checkModeProperty;

		virtual void	setCustomImageryAutoSized(bool setting);
		void	setCheck( bool bCheck );
		const bool	isCheck() const { return d_bIsSelected; } ;


		void			setCheckMode( int nMode );
		const int		getCheckMode() const;
	protected:
		/*!
		\brief
		Return whether this window was inherited from the given class name at some point in the inheritance heirarchy.

		\param class_name
		The class name that is to be checked.

		\return
		true if this window was inherited from \a class_name. false if not.
		*/
		virtual bool	testClassName_impl(const String& class_name) const
		{
			if (class_name==(const utf8*)"FalagardCheckButton")	return true;
			return Window::testClassName_impl(class_name);
		}

		bool				d_bIsSelected; // 是否被选中了
		
		int					d_nCheckMode;  // 分为0 普通checkbox , 1 radio box
		
		RenderableImage		d_selectImage;

		virtual void	onMouseButtonDown(MouseEventArgs& e);
		virtual void	onMouseButtonUp(MouseEventArgs& e);
		virtual	void	drawSelf(float z);
		void			drawCheck();

	};
	
	class FALAGARDBASE_API FalagardCheckButtonFactory : public WindowFactory
    {
    public:
        FalagardCheckButtonFactory(void) : WindowFactory(FalagardCheckButton::WidgetTypeName) { }
        ~FalagardCheckButtonFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };

}