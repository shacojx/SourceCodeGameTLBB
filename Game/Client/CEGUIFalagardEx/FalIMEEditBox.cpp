
#include "FalIMEEditBox.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "CEGUIIme.h"

// Start of CEGUI namespace section
namespace CEGUI
{

	//===================================================================================
	//
	// FalagardIMEEditBox
	//
	//===================================================================================
    const utf8  FalagardIMEEditBox::WidgetTypeName[] = "Falagard/IMEEditBox";

    FalagardIMEEditBoxProperties::DefaultEditBox	FalagardIMEEditBox::d_defaultEditBoxProperty;

    FalagardIMEEditBox::FalagardIMEEditBox(const String& type, const String& name) :
        FalagardEditbox(type, name), 
		d_lastTextOffset(0),
		d_caretOn(true),
		d_lastTextLen(0)
    {
        addIMEEditBoxProperties();

		d_blinkTime = Ime::GetCaretBlinkTime()*0.001f;
		d_lastBlink = System::getSingleton().getCurTimeElapsed();
    }

    FalagardIMEEditBox::~FalagardIMEEditBox()
    {
    }

	void FalagardIMEEditBox::addIMEEditBoxProperties(void)
    {
		CEGUI_START_ADD_STATICPROPERTY( FalagardIMEEditBox )
		   CEGUI_ADD_STATICPROPERTY( &d_defaultEditBoxProperty );
		CEGUI_END_ADD_STATICPROPERTY
	}

	bool FalagardIMEEditBox::hasInputFocus(void) const
	{
		return Ime::GetActiveIMEEditBox() == this;
	}

	void FalagardIMEEditBox::resetCaretBlink(void)
	{
		d_caretOn = true;
		d_lastBlink = System::getSingleton().getCurTimeElapsed();
		requestRedraw();
	}

    void FalagardIMEEditBox::populateRenderCache()
    {
        const StateImagery* imagery;

        // draw container etc
        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
        // try and get imagery for the approprite state.
        imagery = &wlf.getStateImagery("Normal");

        // peform the rendering operation for the container.
        imagery->render(*this);

        // get destination area for text
        Rect textArea(wlf.getNamedArea("TextArea").getArea().getPixelRect(*this));

        //
        // Required preliminary work for text rendering operations
        //
        const FontBase* font = getFont();


        // no font == no more rendering
        if (!font)
            return;
		
		
        // This will point to the final string to be used for rendering.  Useful because it means we
        // do not have to have duplicate code or be copying d_text for handling masked/unmasked text.
        String* editText;

        // Create a 'masked' version of the string if needed.
        String maskedText;
        if (isTextMasked())
        {
            maskedText.insert(0, d_text.length(), getMaskCodePoint());
            editText = &maskedText;
        }
        // text not masked to editText will be the windows d_text String.
        else
        {
            editText = &d_text;
        }

		Rect absarea(Point(0,0), getAbsoluteSize());
		Size textSize = font->getFormattedSize( *editText, absarea, (TextFormatting)LeftAligned, 1);
		if(textSize.d_height == 0)
			textSize.d_height = font->getLineSpacing();
		
				
		textArea.d_top = PixelAligned((absarea.getHeight() - textSize.d_height) * 0.5f);
		textArea.d_bottom = textArea.d_top + textSize.d_height;
        // calculate best position to render text to ensure carat is always visible
        float textOffset;
        float extentToCarat = font->getTextExtent(editText->substr(0, getCaratIndex()));

        // get carat imagery
        const ImagerySection& caratImagery = wlf.getImagerySection("Carat");
        // store carat width
        float caratWidth = caratImagery.getBoundingRect(*this, textArea).getWidth();

        // if box is inactive
        if (!hasInputFocus())
        {
            textOffset = d_lastTextOffset;
        }
        // if carat is to the left of the box
        else if ((d_lastTextOffset + extentToCarat) < 0)
        {
            textOffset = -extentToCarat;
        }
        // if carat is off to the right.
        else if ((d_lastTextOffset + extentToCarat) >= (textArea.getWidth() - caratWidth))
        {
            textOffset = textArea.getWidth() - extentToCarat - caratWidth;
        }
        // else carat is already within the box
        else
        {
            textOffset = d_lastTextOffset;
        }

        ColourRect colours;
        float alpha_comp = getEffectiveAlpha();

        //
        // Draw label text
        //
        Rect text_part_rect(textArea);
        text_part_rect.d_left += textOffset;

        // draw pre-highlight text
        String sect = editText->substr(0, getSelectionStartIndex());
        colours.setColours(d_normalTextColour);
        colours.modulateAlpha(alpha_comp);
        d_renderCache.cacheText(this, sect, font, LeftAligned, text_part_rect, 0, colours, &textArea);

        // adjust rect for next section
        text_part_rect.d_left += font->getTextExtent(sect);

        // draw highlight text
        sect = editText->substr(getSelectionStartIndex(), getSelectionLength());
        colours.setColours(d_selectTextColour);
        colours.modulateAlpha(alpha_comp);
        d_renderCache.cacheText(this, sect, font, LeftAligned, text_part_rect, 0, colours, &textArea);

        // adjust rect for next section
        text_part_rect.d_left += font->getTextExtent(sect);

        // draw post-highlight text
        sect = editText->substr(getSelectionEndIndex());
        colours.setColours(d_normalTextColour);
        colours.modulateAlpha(alpha_comp);
        d_renderCache.cacheText(this, sect, font, LeftAligned, text_part_rect, 0, colours, &textArea);

        // remember this for next time.
        d_lastTextOffset = textOffset;

        // see if the editbox is active or inactive.
        bool active = (!isReadOnly()) && hasInputFocus();

        //
        // Render selection imagery.
        //
        if (getSelectionLength() != 0)
        {
            // calculate required start and end offsets of selection imagery.
            float selStartOffset = font->getTextExtent(editText->substr(0, getSelectionStartIndex()));
            float selEndOffset   = font->getTextExtent(editText->substr(0, getSelectionEndIndex()));

            // calculate area for selection imagery.
            Rect hlarea(textArea);
            hlarea.d_left += textOffset + selStartOffset;
            hlarea.d_right = hlarea.d_left + (selEndOffset - selStartOffset);

            // render the selection imagery.
            wlf.getStateImagery(active ? "ActiveSelection" : "InactiveSelection").render(*this, hlarea, 0, &textArea);
        }

        //
        // Render carat
        //
        if (active && d_caretOn)
        {
            Rect caratRect(textArea); 
            caratRect.d_left += extentToCarat + textOffset;

            caratImagery.render(*this, caratRect, 0, 0, &caratRect);
        }
	}

	void FalagardIMEEditBox::updateSelf(float elapsed)
	{
		float time_now = System::getSingleton().getCurTimeElapsed();

		int textLen = (int)getText().size();
		if(textLen != d_lastTextLen) 
		{
			resetCaretBlink();
			d_lastTextLen = textLen;
		}

		//
		// Blink the caret
		//
		if( time_now - d_lastBlink >= d_blinkTime )
		{
			d_caretOn = !d_caretOn;
			d_lastBlink = time_now;

			requestRedraw();
		}

		Editbox::updateSelf(elapsed);
	}

    size_t FalagardIMEEditBox::getTextIndexFromPosition(const Point& pt) const
    {
        //
        // calculate final window position to be checked
        //
        float wndx = screenToWindowX(pt.d_x);

        if (getMetricsMode() == Relative)
        {
            wndx = relativeToAbsoluteX(wndx);
        }

        wndx -= d_lastTextOffset;

        //
        // Return the proper index
        //
        if (isTextMasked())
        {
            return getFont()->getCharAtPixel(String(d_text.length(), getMaskCodePoint()), wndx);
        }
        else
        {
            return getFont()->getCharAtPixel(d_text, wndx);
        }
	}

	void FalagardIMEEditBox::onActivated(ActivationEventArgs& e)
	{
		FalagardEditbox::onActivated(e);

		Ime::SetActiveIMEEditBox(this, true);
	}
		
	void FalagardIMEEditBox::onDeactivated(ActivationEventArgs& e)
	{
		FalagardEditbox::onDeactivated(e);

		//Ime::SetActiveIMEEditBox(this, false);
	}
	
	void FalagardIMEEditBox::onParentHidden(void)
	{
		FalagardEditbox::onParentHidden();

		Ime::SetActiveIMEEditBox(this, false);
	}

	/*************************************************************************
		Handler for mouse button pushed events
	*************************************************************************/
	void FalagardIMEEditBox::onMouseButtonDown(MouseEventArgs& e)
	{
		// base class handling
		Editbox::onMouseButtonDown(e);

		Ime::SetActiveIMEEditBox(this, true);
	}

    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardIMEEditBoxFactory::createWindow(const String& name)
    {
        return new FalagardIMEEditBox(d_type, name);
    }

    void FalagardIMEEditBoxFactory::destroyWindow(Window* window)
    {
		delete window;
    }

}
