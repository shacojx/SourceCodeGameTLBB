
#include "FalagardMeshWindow.h"
#include "CEGUIImageset.h"
#include "CEGUISystem.h"
#include "CEGUIRenderer.h"
#include "CEGUITexture.h"
#include "OgreCEGUIRenderer.h"
#include "CEGUIWindowManager.h"
#include "..\Common\AxTrace.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	//===================================================================================
	//
	// FalagardMeshWindow
	//
	//===================================================================================
	const utf8 FalagardMeshWindow::WidgetTypeName[] = "Falagard/MeshWindow";

    // Event Strings
    const String FalagardMeshWindow::EventNamespace("Falagard/MeshWindow");

    const String FalagardMeshWindow::EventMeshHook("MeshHook");
    const String FalagardMeshWindow::EventMeshShown("MeshShown");
    const String FalagardMeshWindow::EventMeshHidden("MeshHidden");

    // Properties
	FalagardMeshWindowProperties::FakeObject	FalagardMeshWindow::d_fakeObjectProperty;
	FalagardMeshWindowProperties::ShowControl	FalagardMeshWindow::d_showControlProperty;

	FalagardMeshWindow::FalagardMeshWindow(const String& type, const String& name):
		StaticImage(type, name)
	{
		addMeshWindowProperties();
	}

	FalagardMeshWindow::~FalagardMeshWindow(void)
	{
	}

	void FalagardMeshWindow::addMeshWindowProperties(void)
	{
		CEGUI_START_ADD_STATICPROPERTY( FalagardMeshWindow )
		   CEGUI_ADD_STATICPROPERTY( &d_fakeObjectProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_showControlProperty );
		CEGUI_END_ADD_STATICPROPERTY
	}

	void FalagardMeshWindow::setShowControl(const String& windowName)
	{
		d_showControlWindowName = windowName;

		//Get the window point.
		Window* pControlWindow = WindowManager::getSingleton().getWindow(windowName);
		if(!pControlWindow) return;

		//Subscribe the ShownEvent and HiddenEvent.
		pControlWindow->subscribeEvent(EventShown,
			CEGUI::Event::Subscriber(&CEGUI::FalagardMeshWindow::handleControlShown, this));

		pControlWindow->subscribeEvent(EventHidden,
			CEGUI::Event::Subscriber(&CEGUI::FalagardMeshWindow::handleControlHidden, this));
	}

	void FalagardMeshWindow::onParentHidden(void)
	{
		StaticImage::onParentHidden();

		WindowEventArgs args(this);
		fireEvent(EventMeshHidden, args, EventNamespace);
	}

	void FalagardMeshWindow::setFakeObject(const String& fakeObject)
	{ 
		d_fakeObjectName = fakeObject;

		if(!d_fakeObjectName.empty() && isVisible())
		{
			WindowEventArgs args(this);
			fireEvent(EventMeshShown, args, EventNamespace);
		}
	}

	bool FalagardMeshWindow::handleControlShown(const CEGUI::EventArgs& e)
	{
		WindowEventArgs args(this);
		fireEvent(EventMeshShown, args, EventNamespace);

		requestRedraw();
		return false;
	}

	bool FalagardMeshWindow::handleControlHidden(const CEGUI::EventArgs& e)
	{
		WindowEventArgs args(this);
		fireEvent(EventMeshHidden, args, EventNamespace);

		return false;
	}

	void FalagardMeshWindow::subscribeShownEvent(Event::Subscriber subscriber)
	{
		subscribeEvent(EventMeshShown, subscriber);
	}
	
	void FalagardMeshWindow::subscribeHidenEvent(Event::Subscriber subscriber)
	{
		subscribeEvent(EventMeshHidden, subscriber);
	}
	
	void FalagardMeshWindow::subscribeHookEvent(Event::Subscriber subscriber)
	{
		subscribeEvent(EventMeshHook, subscriber);
	}

	void FalagardMeshWindow::populateRenderCache()
	{
//		StaticImage::populateRenderCache();
		d_image.setVertFormatting(RenderableImage::VertStretched);
		d_image.setHorzFormatting(RenderableImage::HorzStretched);

		d_image.draw(d_renderCache);
	}

	//////////////////////////////////////////////////////////////////////////
    Window* FalagardMeshWindowFactory::createWindow(const String& name)
    {
        return new FalagardMeshWindow(d_type, name);
    }

    void FalagardMeshWindowFactory::destroyWindow(Window* window)
    {
        delete window;
    }
};