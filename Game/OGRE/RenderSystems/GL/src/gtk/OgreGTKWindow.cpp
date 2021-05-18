/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://ogre.sourceforge.net/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/

#include "OgreGTKWindow.h"
#include "OgreGTKGLSupport.h"
#include "OgreRenderSystem.h"
#include "OgreRoot.h"
#include "OgreLogManager.h"

using namespace Ogre;

OGREWidget::OGREWidget(bool useDepthBuffer) : 
	Gtk::GL::DrawingArea()
{
    Glib::RefPtr<Gdk::GL::Config> glconfig;

    Gdk::GL::ConfigMode mode = Gdk::GL::MODE_RGBA | Gdk::GL::MODE_DOUBLE;
    if (useDepthBuffer)
        mode |= Gdk::GL::MODE_DEPTH;

    glconfig = Gdk::GL::Config::create(mode);
    if (glconfig.is_null())
    {
    	LogManager::getSingleton().logMessage("[gtk] GLCONFIG BLOWUP");
    }

    // Inherit GL context from Ogre main context
    set_gl_capability(glconfig, GTKGLSupport::getSingleton().getMainContext());

    add_events(Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
}

// OGREWidget TODO: 
// - resize events et al
// - Change aspect ratio

GTKWindow::GTKWindow():
	mGtkWindow(0)
{
    	//kit = Gtk::Main::instance();
	
	// Should  this move to GTKGLSupport?
    	// Gtk::GL::init(0, NULL);
	// Already done in GTKGLSupport	

	mWidth = 0;
	mHeight  = 0;
}

GTKWindow::~GTKWindow()
{
}
bool GTKWindow::pump_events()
{
    Gtk::Main *kit = Gtk::Main::instance();
    if (kit->events_pending())
    {
        kit->iteration(false);
        return true;
    }
    return false;
}

OGREWidget* GTKWindow::get_ogre_widget()
{
    return ogre;
}

void GTKWindow::create(const String& name, unsigned int width, unsigned int height, unsigned int colourDepth, 
                       bool fullScreen, int left, int top, bool depthBuffer, 
                       void* miscParam, ...)
{
   	mName = name;
	mWidth = width;
	mHeight = height;

	if(!miscParam) {
    		mGtkWindow = new Gtk::Window();
    		mGtkWindow->set_title(mName);

    		if (fullScreen)
    		{
        		mGtkWindow->set_decorated(false);
        		mGtkWindow->fullscreen();
    		}
    		else
    		{
        		mGtkWindow->set_default_size(mWidth, mHeight);
        		mGtkWindow->move(left, top); 
    		}
	} else {
		// If miscParam is not 0, a parent widget has been passed in,
		// we will handle this later on after the widget has been created.
	}

    	ogre = Gtk::manage(new OGREWidget(depthBuffer));
    	ogre->set_size_request(width, height);

	ogre->signal_delete_event().connect(SigC::slot(*this, &GTKWindow::on_delete_event));
	ogre->signal_expose_event().connect(SigC::slot(*this, &GTKWindow::on_expose_event));

	if(mGtkWindow) {
    		mGtkWindow->add(*ogre);
    		mGtkWindow->show_all();
	}
	if(miscParam) {
		// Attach it!
		// Note that the parent widget *must* be visible already at this point,
		// or the widget won't get realized in time for the GLinit that follows
		// this call. This is usually the case for Glade generated windows, anyway.
		reinterpret_cast<Gtk::Container*>(miscParam)->add(*ogre);
		ogre->show();
	}
	//ogre->realize();
}

void GTKWindow::destroy()
{
    	Root::getSingleton().getRenderSystem()->detachRenderTarget( this->getName() );
	// We could detach the widget from its parent and destroy it here too,
	// but then again, it is managed so we rely on GTK to destroy it.
	delete mGtkWindow;
	mGtkWindow = 0;

}

bool GTKWindow::isActive() const
{
    return ogre->is_realized();
}

bool GTKWindow::isClosed() const
{
    return ogre->is_visible();
}

void GTKWindow::reposition(int left, int top)
{
	if(mGtkWindow)
    		mGtkWindow->move(left, top);
}

void GTKWindow::resize(unsigned int width, unsigned int height)
{
	if(mGtkWindow)
    		mGtkWindow->resize(width, height);
}

void GTKWindow::swapBuffers(bool waitForVSync)
{
    	Glib::RefPtr<Gdk::GL::Window> glwindow = ogre->get_gl_window();
    	glwindow->swap_buffers();
}

void GTKWindow::writeContentsToFile(const String& filename)
{
    	// XXX impl me
}

void GTKWindow::getCustomAttribute( const String& name, void* pData )
{
	if( name == "GTKMMWINDOW" )
	{
		Gtk::Window **win = static_cast<Gtk::Window **>(pData);
		// Oh, the burdens of multiple inheritance
		*win = mGtkWindow;
		return;
	}
	else if( name == "GTKGLMMWIDGET" )
	{
	    	Gtk::GL::DrawingArea **widget = static_cast<Gtk::GL::DrawingArea **>(pData);
		*widget = ogre;
		return;
	}
	else if( name == "isTexture" )
	{
		bool *b = reinterpret_cast< bool * >( pData );
		*b = false;
		return;
	}
	RenderWindow::getCustomAttribute(name, pData);
}


bool GTKWindow::on_delete_event(GdkEventAny* event)
{
    Root::getSingleton().getRenderSystem()->detachRenderTarget( this->getName() );
    return false;
}

bool GTKWindow::on_expose_event(GdkEventExpose* event)
{
    // Window exposed, update interior
    //std::cout << "Window exposed, update interior" << std::endl;
    // TODO: time between events, as expose events can be sent crazily fast
    update();
    return false;
}
