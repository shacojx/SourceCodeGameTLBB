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

#include "OgreLog.h"
#include "OgreLogManager.h"
#include "OgreString.h"

namespace Ogre {

    //-----------------------------------------------------------------------
    Log::Log( const String& name, bool debuggerOuput, bool suppressFile )
		: mLogLevel(LL_NORMAL), mDebugOut(debuggerOuput), mSuppressFile(suppressFile), 
		mName(name)
    {
		if (!mSuppressFile)
		{
			mfpLog.open(name.c_str());
		}
    }
    //-----------------------------------------------------------------------
    Log::~Log()
    {
		if (!mSuppressFile)
		{
	        mfpLog.close();
		}
    }
    //-----------------------------------------------------------------------
    void Log::logMessage( const String& message, LogMessageLevel lml, bool maskDebug )
    {
        if ((mLogLevel + lml) >= OGRE_LOG_THRESHOLD)
        {
			// Reroute to log manager for custom listeners.
			if ( LogManager::getSingletonPtr() ) 
			{
				LogManager::getSingleton()._routeMessage( mName,message,lml,maskDebug );
			}

			if (mDebugOut && !maskDebug)
                std::cerr << message << std::endl;

            // Write time into log
			if (!mSuppressFile)
			{
				struct tm *pTime;
				time_t ctTime; time(&ctTime);
				pTime = localtime( &ctTime );
				mfpLog << std::setw(2) << std::setfill('0') << pTime->tm_hour
					<< ":" << std::setw(2) << std::setfill('0') << pTime->tm_min
					<< ":" << std::setw(2) << std::setfill('0') << pTime->tm_sec 
					<< ": " << message << std::endl;

				// Flush stcmdream to ensure it is written (incase of a crash, we need log to be up to date)
				mfpLog.flush();
			}
        }
    }
    //-----------------------------------------------------------------------
    void Log::setLogDetail(LoggingLevel ll)
    {
        mLogLevel = ll;
    }
}
