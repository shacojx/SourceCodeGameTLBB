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
#include "OgreWin32Timer.h"

namespace Ogre {
    //-------------------------------------------------------------------------
    void Win32Timer::reset()
    {
        Timer::reset();

        // Save the current process
        HANDLE mProc = GetCurrentProcess();

        // Get the current Affinity
        GetProcessAffinityMask(mProc, &mProcMask, &mSysMask);

        mThread = GetCurrentThread();

        // Set affinity to the first core
        SetThreadAffinityMask(mThread, 1);

        // Query the frequency and start timer
        QueryPerformanceFrequency(&mFrequency);
        QueryPerformanceCounter(&mStartTime);
        mStartTick = GetTickCount();
        mLastTime = 0;
		mQueryCount = 0;

        // Reset affinity
        SetThreadAffinityMask(mThread, mProcMask);
    }
    //-------------------------------------------------------------------------
    unsigned long Win32Timer::getMilliseconds()
    {
        LARGE_INTEGER curTime;

        // Set affinity to the first core
        SetThreadAffinityMask(mThread, 1);

        // Query the timer
        QueryPerformanceCounter(&curTime);

		// Resample the frequency
        mQueryCount++;
        if(mQueryCount == FREQUENCY_RESAMPLE_RATE)
        {
            mQueryCount = 0;
            QueryPerformanceFrequency(&mFrequency);
        }

        // Reset affinity
        SetThreadAffinityMask(mThread, mProcMask);

        LONGLONG newTime = curTime.QuadPart - mStartTime.QuadPart;
        
        // scale by 1000 for milliseconds
        unsigned long newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);

        // detect and compensate for performance counter leaps
        // (surprisingly common, see Microsoft KB: Q274323)
        unsigned long check = GetTickCount() - mStartTick;
        signed long msecOff = (signed long)(newTicks - check);
        if (msecOff < -100 || msecOff > 100)
        {
            // We must keep the timer running forward :)
            LONGLONG adjust = (std::min)(msecOff * mFrequency.QuadPart / 1000, newTime - mLastTime);
            mStartTime.QuadPart += adjust;
            newTime -= adjust;

            // Re-calculate milliseconds
            newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);
        }

        // Record last time for adjust
        mLastTime = newTime;

        return newTicks;
	}
    //-------------------------------------------------------------------------
	unsigned long Win32Timer::getMicroseconds()
	{
        LARGE_INTEGER curTime;

        // Set affinity to the first core
        SetThreadAffinityMask(mThread, 1);

        // Query the timer
        QueryPerformanceCounter(&curTime);

		// Resample the frequency
        mQueryCount++;
        if(mQueryCount == FREQUENCY_RESAMPLE_RATE)
        {
            mQueryCount = 0;
            QueryPerformanceFrequency(&mFrequency);
        }

        // Reset affinity
        SetThreadAffinityMask(mThread, mProcMask);

        LONGLONG newTime = curTime.QuadPart - mStartTime.QuadPart;
        
        // get milliseconds to check against GetTickCount
        unsigned long newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);
        
        // detect and compensate for performance counter leaps
        // (surprisingly common, see Microsoft KB: Q274323)
        unsigned long check = GetTickCount() - mStartTick;
        signed long msecOff = (signed long)(newTicks - check);
        if (msecOff < -100 || msecOff > 100)
        {
            // We must keep the timer running forward :)
            LONGLONG adjust = (std::min)(msecOff * mFrequency.QuadPart / 1000, newTime - mLastTime);
            mStartTime.QuadPart += adjust;
            newTime -= adjust;
        }

        // Record last time for adjust
        mLastTime = newTime;

        // scale by 1000000 for microseconds
        unsigned long newMicro = (unsigned long) (1000000 * newTime / mFrequency.QuadPart);

        return newMicro;
	}

}
