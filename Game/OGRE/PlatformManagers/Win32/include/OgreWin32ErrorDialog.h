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
#ifndef __WIN32ERRORDIALOG_H__
#define __WIN32ERRORDIALOG_H__

#include "OgreWin32Prerequisites.h"
#include "OgreErrorDialog.h"
#include "windows.h"

namespace Ogre {

    /** Windows-specific class for displaying the error dialog if Ogre fails badly! */
    class Win32ErrorDialog : public ErrorDialog
    {
    public:
        Win32ErrorDialog(HINSTANCE hInst);
        /** Displays the error dialog.
            @param errorMessage The error message which has caused the failure.
            @param logName Optional name of the log to display in the detail pane.
        */
        void display(const String& errorMessage, String logName = "");

    protected:
        String mErrorMsg;
        /** Callback to process window events */
#if OGRE_ARCHITECTURE_64 == OGRE_ARCH_TYPE
        static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
#else
        static BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
#endif

        HINSTANCE mHInstance; // HInstance of application, for dialog

    };
}



#endif
