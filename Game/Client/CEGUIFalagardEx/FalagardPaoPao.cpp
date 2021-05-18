#include ".\falagardpaopao.h"
#include "ceguipropertyhelper.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
namespace CEGUI
{
	const utf8 FalagardPaoPao::WidgetTypeName[] = "Falagard/PaoPao";

	FalagardPaoPaoProperties::ArrowImage  FalagardPaoPao::d_ArrowProperty;
	FalagardPaoPao::FalagardPaoPao(const String& type, const String& name):
		FalagardSelfFitWindow(type, name)
	{
		CEGUI_START_ADD_STATICPROPERTY( FalagardPaoPao )
		   CEGUI_ADD_STATICPROPERTY( &d_ArrowProperty );

		CEGUI_END_ADD_STATICPROPERTY
		setAlpha( 0 );
	}

	FalagardPaoPao::~FalagardPaoPao(void)
	{
	}

	void FalagardPaoPao::populateRenderCache()
	{
		const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
        // try and get imagery for our current state
        const StateImagery* imagery = &wlf.getStateImagery(isDisabled() ? "Disabled" : "Enabled");
        // peform the rendering operation.
        imagery->render(*this);
		if( d_ArrowImage.getImage() )
		{
			colour col( 1,1,1, getAlpha() );
			d_ArrowImage.setColours( col );
			d_ArrowImage.draw(d_renderCache);
		}
	}

	void FalagardPaoPao::updateSelfSize()
	{
		FalagardSelfFitWindow::updateSelfSize();

		const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
		if( wlf.isNamedAreaDefined( "TextArea" ) )
		{
			const Rect rectBorder(wlf.getNamedArea("TextArea").getArea().getPixelRect(*this));
			Rect wndArea(getAbsoluteRect());
			Size size(getAbsoluteSize());

			size.d_width  += wndArea.getWidth() - rectBorder.getWidth();
			size.d_height += wndArea.getHeight() - rectBorder.getHeight();

			setSize(Absolute, size);
		}

		if( d_ArrowImage.getImage() )
		{
			Size size = d_ArrowImage.getImage()->getSize();
			Rect area( getAbsoluteWidth() / 2 - size.d_width / 2, getAbsoluteHeight() - size.d_height, getAbsoluteWidth() / 2 + size.d_width / 2, getAbsoluteHeight() );
			d_ArrowImage.setRect( area );
		}
	}

	void FalagardPaoPao::setArrowImage( const Image* image )
	{
		d_ArrowImage.setImage( image );
		d_ArrowImage.setHorzFormatting( RenderableImage::HorzStretched );
		d_ArrowImage.setVertFormatting( RenderableImage::VertStretched );
	}


	namespace FalagardPaoPaoProperties
	{
		String	ArrowImage::get(const PropertyReceiver* receiver) const
		{
			return "";//PropertyHelper::boolToString(static_cast<const FalagardPaoPao*>(receiver)->isFrameEnabled());
		}


		void	ArrowImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardPaoPao*>( receiver )->setArrowImage( PropertyHelper::stringToImage( value ) );
		}
	}
   
	Window* FalagardPaoPaoFactory::createWindow(const String& name)
    {
        return new FalagardPaoPao(d_type, name);
    }

    void FalagardPaoPaoFactory::destroyWindow(Window* window)
    {
		delete window;
    }

}