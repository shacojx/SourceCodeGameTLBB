#pragma once
#include "FalModule.h"
#include "CEGUIRenderableImage.h"
#include "CEGUIWindowFactory.h"
#include "fallistbox.h"
#include "falimagelistproperties.h"
#include "elements/ceguilistboxtextitem.h"
namespace CEGUI
{
	class FALAGARDBASE_API FalagardImageListBoxItem : public ListboxTextItem
	{
		int					d_State;  // 当前选项的状态
		Image*				d_Normal;
		Image*				d_Horver;
		Image*				d_Clicked;
		Image*				d_Disable;
	public:

		void SetNormalImage( Image* pImage );
		void SetHorverImage( Image* pImage );
		void SetCheckedImage( Image* pImage );
		void SetDisableImage( Image* pImage );

		void SetState( int nState );

		virtual void draw(RenderCache& cache,const Rect& targetRect, float zBase,  float alpha, const Rect* clipper) const;

		FalagardImageListBoxItem(const String& text, uint item_id = 0, void* item_data = NULL, bool disabled = false, bool auto_delete = true);

		~FalagardImageListBoxItem(){};

	};

	class FALAGARDBASE_API FalagardImageListBox :	public FalagardListbox
	{

		FalagardImageListBoxItem*	d_LastItem;
	public:
		static const utf8  WidgetTypeName[];       //!< type name for this widget.
	
		//virtual void drawSelf(float z);
		//virtual void updateSelf(float elapsed);
		virtual void onMouseMove( MouseEventArgs& e );

		void setNormalImage( const Image* image );
		void setHorverImage( const Image* image );
		void setCheckedImage( const Image* image );
		void setDisabledImage( const Image* image );

		const Image* getNormalImage() const;
		const Image* getHorverImage() const;
		const Image* getCheckedImage() const;
		const Image* getDisabledImage() const;

	protected:
		Image*		d_normalImage;		//!< RenderableImage used when rendering an image in the normal state.
		Image*		d_hoverImage;		//!< RenderableImage used when rendering an image in the highlighted state.
		Image*		d_pushedImage;		//!< RenderableImage used when rendering an image in the pushed state.
		Image*		d_disabledImage;	//!< RenderableImage used when rendering an image in the disabled state.


		static FalagardImageListBoxProperties::ImageNormal    d_ImageNormalProperty;
		static FalagardImageListBoxProperties::ImageHorver    d_ImageHorverProperty;
		static FalagardImageListBoxProperties::ImageChecked   d_ImageCheckedProperty;
		static FalagardImageListBoxProperties::ImageDisable   d_ImageDisableProperty;
		static FalagardImageListBoxProperties::AddItem		  d_AddItemProperty;

	public:
		FalagardImageListBox(const String& type, const String& name);
		~FalagardImageListBox(void);
	};

	class FALAGARDBASE_API FalagardImageListBoxFactory : public WindowFactory
    {
    public:
        FalagardImageListBoxFactory(void) : WindowFactory(FalagardImageListBox::WidgetTypeName) { }
        ~FalagardImageListBoxFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };

	enum FalagardImageListBoxItemState
	{
		LIST_ITEM_NORMAL,
		LIST_ITEM_HORVER,
		LIST_ITEM_PUSHED,
		LIST_ITEM_DISABLE,
	};
	
}
