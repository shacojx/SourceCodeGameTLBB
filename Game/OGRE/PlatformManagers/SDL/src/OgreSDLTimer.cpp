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

#include "OgreSDLTimer.h"
#include <sys/time.h>

namespace Ogre {
    
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
void gettimeofday(struct timeval* p, void* tz /* IGNORED */){
     union {
       long long ns100; /*time since 1 Jan 1601 in 100ns units */
       FILETIME ft;
     } _now;
                                                                                
     GetSystemTimeAsFileTime( &(_now.ft) );
     p->tv_usec=(long)((_now.ns100 / 10LL) % 1000000LL );
     p->tv_sec= (long)((_now.ns100-(116444736000000000LL))/10000000LL);
     return;
}
#endif

    
	void SDLTimer::reset()
	{
	    Timer::reset();
		gettimeofday(&start, NULL);
	}

	unsigned long SDLTimer::getMilliseconds()
	{
	    struct timeval now;
		gettimeofday(&now, NULL);
	    return (now.tv_sec-start.tv_sec)*1000+(now.tv_usec-start.tv_usec)/1000;
	}

	unsigned long SDLTimer::getMicroseconds()
	{
	    struct timeval now;
		gettimeofday(&now, NULL);
	    return (now.tv_sec-start.tv_sec)*1000000+(now.tv_usec-start.tv_usec);
	}	
}
