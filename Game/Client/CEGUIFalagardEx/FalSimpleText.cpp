
#include "FalSimpleText.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	//===================================================================================
	//
	// FalagardSimpleText
	//
	//===================================================================================
    const utf8  FalagardSimpleText::WidgetTypeName[] = "Falagard/SimpleText";

	FalagardSimpleTextProperties::VertFormatting	FalagardSimpleText::d_vertFormattingProperty;
	FalagardSimpleTextProperties::HorzFormatting	FalagardSimpleText::d_horzFormattingProperty;
	FalagardSimpleTextProperties::TextScale			FalagardSimpleText::d_textScaleProperty;
	FalagardSimpleTextProperties::TextColor			FalagardSimpleText::d_textColorProperty;

    FalagardSimpleText::FalagardSimpleText(const String& type, const String& name) :
        Window(type, name),
		d_horzFormatting(LeftAligned),
		d_vertFormatting(VertCentred)
    {
		d_fScaleX = 1;
		d_fScaleY = 1;
		d_TextColour = colour( 1,1,1,1 );
        addSimpleTextProperties();
    }

    FalagardSimpleText::~FalagardSimpleText()
    {
    }

	void FalagardSimpleText::addSimpleTextProperties(void)
    {
		CEGUI_START_ADD_STATICPROPERTY( FalagardSimpleText )
		   CEGUI_ADD_STATICPROPERTY( &d_vertFormattingProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_horzFormattingProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_textScaleProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_textColorProperty );
		CEGUI_END_ADD_STATICPROPERTY
    }

	/*************************************************************************
		Set the formatting required for the text.	
	*************************************************************************/
	void FalagardSimpleText::setVerticalFormatting(VertFormatting v_fmt)
	{
		d_vertFormatting = v_fmt;
		requestRedraw();
	}
	

	/*************************************************************************
		Set the formatting required for the text.	
	*************************************************************************/
	void FalagardSimpleText::setHorizontalFormatting(HorzFormatting h_fmt)
	{
		d_horzFormatting = h_fmt;
		requestRedraw();
	}

	/*************************************************************************
		Perform the actual rendering for this Window.
	*************************************************************************/
	void FalagardSimpleText::populateRenderCache()
	{
		const FontBase* font = getFont();
		// can't render text without a font or empty text
		if (font == 0 || d_text.empty())
			return;
		
		// get destination area for the text.
		Rect textArea(getTextRenderArea());
		Rect clipper(textArea);

		float textHeight = font->getFormattedSize(d_text, textArea, (TextFormatting)d_horzFormatting,d_fScaleX, d_fScaleY ).d_height;

		// adjust y positioning according to formatting option
		switch(d_vertFormatting)
		{
		case TopAligned:
			textArea.d_top += 1; //extern border
			break;

		case VertCentred:
			textArea.d_top += PixelAligned((textArea.getHeight() - textHeight) * 0.5f);
			break;

		case BottomAligned:
			textArea.d_top = textArea.d_bottom - textHeight;
			break;
		}

	    ColourRect final_cols( d_TextColour );
		final_cols.setAlpha( d_alpha );
		d_renderCache.cacheText(this, 
			d_text, font, (TextFormatting)d_horzFormatting, textArea, 0, final_cols, &clipper, true, d_fScaleX, d_fScaleY);
	}

	void FalagardSimpleText::setTextScale( Size  scale )
	{
		d_fScaleX = scale.d_width;
		d_fScaleY = scale.d_height; 
		requestRedraw();
	}

    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardSimpleTextFactory::createWindow(const String& name)
    {
        return new FalagardSimpleText(d_type, name);
    }

    void FalagardSimpleTextFactory::destroyWindow(Window* window)
    {
		delete window;
    }

}