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
#include "OgreWin32ConfigDialog.h"
#include "OgreWin32ErrorDialog.h"

#ifndef OGRE_NO_DX_INPUT
#ifdef DX7INPUTONLY
#include "OgreWin32Input.h"
#else
#include "OgreWin32Input8.h"
#endif 
#endif

#include "OgreWin32Timer.h"
#include "OgreRoot.h"
#include "OgreLogManager.h"
#include "OgreRenderWindow.h"

#if OGRE_COMPILER == OGRE_COMPILER_MSVC

#if defined(_M_IA64)
#pragma section(".base", long, read)
extern "C"
__declspec(allocate(".base"))
const IMAGE_DOS_HEADER __ImageBase;
#else
extern "C"
const IMAGE_DOS_HEADER __ImageBase;
#endif

#endif  // OGRE_COMPILER == OGRE_COMPILER_MSVC

namespace Ogre {

#ifdef DEBUG
    int g_iCreatedConfigDiag = 0;
    int g_iCreatedErrorDiag = 0;
    int g_iCreatedRenderWindow = 0;
    int g_iCreatedInputReader = 0;
#endif

    /// Retrieves an instance of a config dialog for this platform
    void createPlatformConfigDialog(ConfigDialog** ppDlg)
    {
        // Must get HINSTANCE
#if OGRE_COMPILER == OGRE_COMPILER_MSVC
        HINSTANCE hInst = (HINSTANCE) &__ImageBase;
#else
        HINSTANCE hInst = GetModuleHandle(OGRE_PLATFORM_LIB);
#endif
        *ppDlg = new Win32ConfigDialog(hInst);

#ifdef DEBUG
        g_iCreatedConfigDiag++;
#endif
    }

    /// Retrieves an instance of an error dialog for this platform
    void createPlatformErrorDialog(ErrorDialog** ppDlg)
    {
#if OGRE_COMPILER == OGRE_COMPILER_MSVC
        HINSTANCE hInst = (HINSTANCE) &__ImageBase;
#else
        HINSTANCE hInst = GetModuleHandle(OGRE_PLATFORM_LIB);
#endif
        *ppDlg = new Win32ErrorDialog(hInst);

#ifdef DEBUG
        g_iCreatedErrorDiag++;
#endif
    }

	/// Creates a Timer using default implementation
	void createMyTimer(Timer** ppTimer)
	{
		*ppTimer = new Win32Timer();
        (*ppTimer)->reset();
	}

	void destroyMyTimer(Timer* ppTimer)
	{
		delete ppTimer;
	}
    /// Retrieves an instance of an input reader for this platform
    void createPlatformInputReader(InputReader** ppReader)
    {
#ifndef OGRE_NO_DX_INPUT
#ifdef DX7INPUTONLY
		*ppReader = new Win32Input();
#else
        *ppReader = new Win32Input8();
#endif
#endif

#ifdef DEBUG
        g_iCreatedInputReader++;
#endif
    }

    /// Destroys
    void destroyPlatformConfigDialog(ConfigDialog* dlg)
    {
        delete dlg;

#ifdef DEBUG
        g_iCreatedConfigDiag--;
#endif
    }
    /// Destroys
    void destroyPlatformErrorDialog(ErrorDialog* dlg)
    {
        delete dlg;

#ifdef DEBUG
        g_iCreatedErrorDiag--;
#endif
    }
    /// Destroys
    void destroyPlatformRenderWindow(RenderWindow* wnd)
    {
        delete wnd;

#ifdef DEBUG
        g_iCreatedRenderWindow--;
#endif
    }
    /// Destroys
    void destroyPlatformInputReader(InputReader* reader)
    {
        delete reader;

#ifdef DEBUG
        g_iCreatedInputReader--;
#endif
    }

	void MymessagePump(RenderWindow* rw)
	{
		//A simple Win32 event pump
		MSG  msg;
		while( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

#ifdef DEBUG
    BOOL WINAPI DllMain( HINSTANCE hinstDLL,  // handle to DLL module
                         DWORD fdwReason,     // reason for calling function
                         LPVOID lpvReserved   // reserved
                       )
    {
        if( fdwReason == DLL_THREAD_DETACH ) {
            if( g_iCreatedConfigDiag )
                LogManager::logMessage( "Memory Leak: Not all platform configuration dialogs were destroyed!!!", LML_CRITICAL );
            if( g_iCreatedErrorDiag )
                LogManager::logMessage( "Memory Leak: Not all platform error dialogs were destroyed!!!", LML_CRITICAL );
            if( g_iCreatedRenderWindow )
                LogManager::logMessage( "Memory Leak: Not all platform render windows were destroyed!!!", LML_CRITICAL );
            if( g_iCreatedInputReader )
                LogManager::logMessage( "Memory Leak: Not all platform input readers were destroyed!!!", LML_CRITICAL );
        }
    }
#endif
}
