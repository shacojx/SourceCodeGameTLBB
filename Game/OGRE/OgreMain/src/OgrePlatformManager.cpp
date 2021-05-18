/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

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
#include "OgreStableHeaders.h"
#include "OgrePlatformManager.h"

#include "OgreDynLibManager.h"
#include "OgreDynLib.h"

#include "OgreConfigDialog.h"
#include "OgreErrorDialog.h"
#include "OgreWin32PlatformDll.h"


namespace Ogre {

    //-----------------------------------------------------------------------
    template<> PlatformManager* Singleton<PlatformManager>::ms_Singleton = 0;
    PlatformManager* PlatformManager::getSingletonPtr(void)
    {
        return ms_Singleton;
    }
    PlatformManager& PlatformManager::getSingleton(void)
    {  
        assert( ms_Singleton );  return ( *ms_Singleton );  
    }
    //-----------------------------------------------------------------------
    PlatformManager::PlatformManager()
    {
        // Load library

  //      DynLib* lib = DynLibManager::getSingleton().load(OGRE_PLATFORM_LIB);

  //      mpfCreateConfigDialog = (DLL_CREATECONFIGDIALOG)lib->getSymbol("createPlatformConfigDialog");
  //      mpfCreateErrorDialog = (DLL_CREATEERRORDIALOG)lib->getSymbol("createPlatformErrorDialog");
  //      mpfCreateInputReader = (DLL_CREATEINPUTREADER)lib->getSymbol("createPlatformInputReader");
		//mpfCreateTimer = (DLL_CREATETIMER)lib->getSymbol("createTimer");

  //      mpfDestroyConfigDialog = (DLL_DESTROYCONFIGDIALOG)lib->getSymbol("destroyPlatformConfigDialog");
  //      mpfDestroyErrorDialog = (DLL_DESTROYERRORDIALOG)lib->getSymbol("destroyPlatformErrorDialog");
  //      mpfDestroyInputReader = (DLL_DESTROYINPUTREADER)lib->getSymbol("destroyPlatformInputReader");
  //      mpfDestroyTimer = (DLL_DESTROYTIMER)lib->getSymbol("destroyTimer");

		//mpfMessagePump = (DLL_MESSAGEPUMP)lib->getSymbol("messagePump");

    }
    //-----------------------------------------------------------------------
    ConfigDialog* PlatformManager::createConfigDialog()
    {
        // Delegate
        ConfigDialog* pdlg;
        createPlatformConfigDialog(&pdlg);
        return pdlg;
    }
    //-----------------------------------------------------------------------
    ErrorDialog* PlatformManager::createErrorDialog()
    {
        // Delegate
        ErrorDialog* pdlg;
        createPlatformErrorDialog(&pdlg);
        return pdlg;
    }
    //-----------------------------------------------------------------------
    InputReader* PlatformManager::createInputReader()
    {
        // Delegate
        InputReader* pinput;
        createPlatformInputReader(&pinput);
        return pinput;
    }
    //-----------------------------------------------------------------------
    void PlatformManager::destroyConfigDialog(ConfigDialog*  dlg)
    {
        // Delegate
        destroyPlatformConfigDialog(dlg);
    }
    //-----------------------------------------------------------------------
    void PlatformManager::destroyErrorDialog(ErrorDialog* dlg)
    {
        // Delegate
        destroyPlatformErrorDialog(dlg);
    }
    //-----------------------------------------------------------------------
    void PlatformManager::destroyInputReader(InputReader* reader)
    {
        // Delegate
        destroyPlatformInputReader(reader);
    }
    //-----------------------------------------------------------------------
    Timer* PlatformManager::createTimer()
    {
        // Delegate
        Timer* pTimer;
		createMyTimer(&pTimer);
        return pTimer;
    }
    //-----------------------------------------------------------------------
    void PlatformManager::destroyTimer(Timer* timer)
    {
        destroyMyTimer(timer);
    }
    //-----------------------------------------------------------------------
    void PlatformManager::messagePump( RenderWindow* rw )
    {
        //Delegate -- If the platform does not export this function, then just return
        if( rw )
            MymessagePump(rw);
    }
    //-----------------------------------------------------------------------
    ConfigDialog::~ConfigDialog() {
    }

    //-----------------------------------------------------------------------
    ErrorDialog::~ErrorDialog() {
    }


}
