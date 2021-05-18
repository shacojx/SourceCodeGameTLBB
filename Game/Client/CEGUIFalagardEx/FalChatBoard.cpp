
#include "FalChatBoard.h"


// Start of CEGUI namespace section
namespace CEGUI
{
	//===================================================================================
	//
	// FalagardChatBoard
	//
	//===================================================================================
    const utf8  FalagardChatBoard::WidgetTypeName[] = "Falagard/ChatBoard";

    FalagardChatBoard::FalagardChatBoard(const String& type, const String& name) :
        Window(type, name)
    {
        addChatBoardProperties();
    }

    FalagardChatBoard::~FalagardChatBoard()
    {
    }

	void FalagardChatBoard::addChatBoardProperties(void)
    {
    }

	float FalagardChatBoard::resizeSelf(float maxWidth)
	{
		const FontBase* font = getFont();
		// can't do anything without a font :)
		if (font == 0)
			return 0.0f;

		// get destination area for the text.
		Rect absarea(0, 0, maxWidth, getPixelRect().getHeight());

		return font->getFormattedSize(d_text, absarea, ((TextFormatting)WordWrapLeftAligned)).d_height 
			+ (FontBase::LineSpace);
	}

	void FalagardChatBoard::populateRenderCache()
	{
		const FontBase* font = getFont();
		// can't render text without a font :)
		if (font == 0)
			return;

		// get destination area for the text.
		Rect absarea(0, 0, getPixelRect().getWidth(), getPixelRect().getHeight());

	    ColourRect final_cols(colour(1.0f, 1.0f, 1.0f));
		d_renderCache.cacheText(this, 
			d_text, font, (TextFormatting)WordWrapLeftAligned, absarea, 0, final_cols);
	}

    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardChatBoardFactory::createWindow(const String& name)
    {
        return new FalagardChatBoard(d_type, name);
    }

    void FalagardChatBoardFactory::destroyWindow(Window* window)
    {
		delete window;
    }
}