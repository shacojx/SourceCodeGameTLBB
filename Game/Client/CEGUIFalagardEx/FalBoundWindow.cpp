#include "FalBoundWindow.h"
#include <CEGUIWindowManager.h>

// Start of CEGUI namespace section
namespace CEGUI
{
	//===================================================================================
	//
	// FalagardBoundWindow
	//
	//===================================================================================
	const utf8  FalagardBoundWindow::WidgetTypeName[] = "Falagard/BoundWindow";
	
	FalagardBoundWindow::FalagardBoundWindow(const String& type, const String& name) :
        Window(type, name)
    {
		addBoundWindowProperties();
    }

	FalagardBoundWindow::~FalagardBoundWindow()
	{
	}

	void FalagardBoundWindow::addBoundWindowProperties(void)
    {
    }

	/*************************************************************************
		Move the window by the pixel offsets specified in 'offset'.	
	*************************************************************************/
	void FalagardBoundWindow::offsetPixelPosition(const Vector2& offset)
	{
		UVector2 uOffset;

		if (getMetricsMode() == Relative)
		{
			Size sz = getParentSize();

			uOffset.d_x = cegui_reldim((sz.d_width != 0) ? offset.d_x / sz.d_width : 0);
			uOffset.d_y = cegui_reldim((sz.d_height != 0) ? offset.d_y / sz.d_height : 0);
		}
		else
		{
			uOffset.d_x = cegui_absdim(PixelAligned(offset.d_x));
			uOffset.d_y = cegui_absdim(PixelAligned(offset.d_y));
		}

		setWindowPosition(d_area.getPosition() + uOffset);
	}

    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardBoundWindowFactory::createWindow(const String& name)
    {
        return new FalagardBoundWindow(d_type, name);
    }

    void FalagardBoundWindowFactory::destroyWindow(Window* window)
    {
		delete window;
    }

}