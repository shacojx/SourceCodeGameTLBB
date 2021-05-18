/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
LGPL like the rest of the engine.
-----------------------------------------------------------------------------
*/

#include "ExampleGTKApplication.h"
//#include "ExampleFrameListener.h"

//#include "GTKWindow.h"
#include <gtkmm/main.h>
#include <gtkmm/box.h>
#include <gtkmm/scale.h>
#include <gtkmm/window.h>
#include <gtkmm/frame.h>
#include <gtkglmm.h>

using namespace Ogre;

class GTKDemoFrameListener : public FrameListener
{
public:
    GTKDemoFrameListener() {
    }

    bool frameStarted(const FrameEvent& evt)
    {
        return true;
    }

    bool frameEnded(const FrameEvent& evt) 
    {
        return true;
    }
};

class GTKDemoApplication : public ExampleApplication, public SigC::Object
{
public:
    void go(void)
    {
        if (!setup())
            return;

        //gtk_win->show_all();
        //gtk_win->update();
	//std::cout << "Go!" << std::endl;
	mWindow->update();
	// Redraw viewport after scene update
	//owidget->queue_draw();

        Gtk::Main::run();
    }

protected:
    bool on_delete_event(GdkEventAny* event)
    {
        Gtk::Main::quit();
        return false;
    }

    void on_value_changed(void)
    {
        Real s = hscale->get_value();
        headNode->setScale(s, s, s);
        //mWindow->update();
	owidget->queue_draw();
    }

	/**
	* At the end of this routine, the window containing the OGRE widget must be
	* created and visible
	*/
    void setupGTKWindow(void)
    {
	gtk_win = new Gtk::Window();
	gtk_win->set_title("An Ogre Head in a box");
        gtk_win->signal_delete_event().connect(SigC::slot(*this,
            &GTKDemoApplication::on_delete_event));


        // Setup our window
        vbox = new Gtk::VBox;
        gtk_win->add(*vbox);

	int width = 640;
	int height = 480;

	// Create a bin in which we will attach the Ogre widget
	Gtk::Bin *bin = Gtk::manage(new Gtk::Frame());
	bin->show();
        vbox->pack_end(*bin, true, true);

	// Create a horizontal scaler to show normal GTK
	// widgets working together with Ogre
        hscale = Gtk::manage(new Gtk::HScale(1.0, 5.0, 0.1));
        hscale->signal_value_changed().connect(SigC::slot(*this,
            &GTKDemoApplication::on_value_changed));

        vbox->pack_end(*hscale, false, true);

	// Now show allAn Ogre in a box
	gtk_win->show_all();

	// Add our OGRE widget
	std::cout << "Creating OGRE widget" << std::endl;

	// Create OGRE widget and attach it
	// Note that the parent widget *must* be visible already at this point,
	// or the widget won't get realized in time for the GLinit that follows
	// this call. This is usually the case for Glade generated windows, anyway.
	mWindow = mRoot->createRenderWindow(
			"An Ogre in a box",
			width, height, 32,
			false, // non fullscreen
			0, 0, // left, top
			true, // depth buffer
			reinterpret_cast<RenderWindow*>(bin)
		);

    	mWindow->getCustomAttribute("GTKGLMMWIDGET", &owidget);
	std::cout << "Created OGRE widget" << std::endl;
	//gtk_win->show_all();
    }

    void createScene(void)
    {
        mSceneMgr->setAmbientLight(ColourValue(0.6, 0.6, 0.6));

        // Setup the actual scene
        Light* l = mSceneMgr->createLight("MainLight");
        l->setPosition(0, 100, 500);

	Entity* head = mSceneMgr->createEntity("head", "ogrehead.mesh");
        headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	headNode->attachObject(head);

        mCamera->setAutoTracking(true, headNode);
    }

    void createFrameListener(void)
    {
	// This is where we instantiate our own frame listener
        mFrameListener = new GTKDemoFrameListener();
        mRoot->addFrameListener(mFrameListener);
    }

private:
    	Gtk::Window* gtk_win;
    	Gtk::VBox* vbox;
    	Gtk::HScale* hscale;
    	SceneNode* headNode;
	Gtk::GL::DrawingArea *owidget;
//	RenderWindow* mWindow;
};

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{
    // Create application object
    GTKDemoApplication app;
 
    try {
        app.go();
    } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        std::cerr << "An exception has occured: " << e.getFullDescription();
#endif
    }


    return 0;
}
