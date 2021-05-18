
#include "FalDragTitle.h"
#include "CEGUIExceptions.h"
#include "CEGUIWindowManager.h"
#include "CEGUIPropertyHelper.h"
#include "FalBoundWindow.h"
// Start of CEGUI namespace section
namespace CEGUI
{

	//===================================================================================
	//
	// FalagardDragTitle
	//
	//===================================================================================
    const utf8  FalagardDragTitle::WidgetTypeName[] = "Falagard/DragTitle";

	FalagardDragTitleProperties::DragTarget		FalagardDragTitle::d_dragTargetProperty;

    FalagardDragTitle::FalagardDragTitle(const String& type, const String& name) :
        Window(type, name),
		d_dragging(false)
    {
        addDragTitleProperties();

        setAlwaysOnTop(true);
	}

    FalagardDragTitle::~FalagardDragTitle()
    {
    }

	void FalagardDragTitle::addDragTitleProperties(void)
    {
		CEGUI_START_ADD_STATICPROPERTY( FalagardDragTitle )
		   CEGUI_ADD_STATICPROPERTY( &d_dragTargetProperty );
		CEGUI_END_ADD_STATICPROPERTY
	}

	String FalagardDragTitle::getDragTarget(void) const				
	{ 
		return d_dragTarget ? d_dragTarget->getName() : (utf8*)"";
	}

	void FalagardDragTitle::setDragTarget(const String& strName)
	{ 
		Window* pWindow = WindowManager::getSingleton().getWindow(strName);
		if(pWindow && pWindow->testClassName((utf8*)"FalagardBoundWindow"))
		{
			d_dragTarget = (CEGUI::FalagardBoundWindow*)pWindow;
		}
		else
		{
			d_dragTarget = 0;
		}
	}

	void FalagardDragTitle::onMouseMove(MouseEventArgs& e)
	{
		// Base class processing.
		Window::onMouseMove(e);

		if (d_dragging && (d_dragTarget != NULL))
		{
			Vector2 delta(screenToWindow(e.position));

			if (getMetricsMode() == Relative)
			{
				delta = relativeToAbsolute(delta);
			}

			// calculate amount that window has been moved
			delta -= d_dragPoint;

			// move the window.  *** Again: Titlebar objects should only be attached to FrameWindow derived classes. ***
			((FalagardBoundWindow*)d_dragTarget)->offsetPixelPosition(delta);

			e.handled = true;
		}
	}

	void FalagardDragTitle::onMouseButtonDown(MouseEventArgs& e)
	{
		// Base class processing
		Window::onMouseButtonDown(e);

		if (e.button == LeftButton)
		{
			if (d_dragTarget != NULL)
			{
				// we want all mouse inputs from now on
				if (captureInput())
				{
					// initialise the dragging state
					d_dragging = true;
					d_dragPoint = screenToWindow(e.position);

					if (getMetricsMode() == Relative)
					{
						d_dragPoint = relativeToAbsolute(d_dragPoint);
					}
				}
			}
		}

		e.handled = true;
	}
	
	void FalagardDragTitle::onMouseButtonUp(MouseEventArgs& e)
	{
		// Base class processing
		Window::onMouseButtonUp(e);

		if (e.button == LeftButton)
		{
			releaseInput();
			e.handled = true;
		}
	}

	/*************************************************************************
		Handler for if the window loses capture of the mouse.
	*************************************************************************/
	void FalagardDragTitle::onCaptureLost(WindowEventArgs& e)
	{
		// Base class processing
		Window::onCaptureLost(e);

		// when we lose out hold on the mouse inputs, we are no longer dragging.
		d_dragging = false;
	}


    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardDragTitleFactory::createWindow(const String& name)
    {
        return new FalagardDragTitle(d_type, name);
    }

    void FalagardDragTitleFactory::destroyWindow(Window* window)
    {
		delete window;
    }

}