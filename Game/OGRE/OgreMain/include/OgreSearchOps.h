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

// Emulate _findfirst, _findnext on non-Windows platforms


#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "OgrePlatform.h"

#if OGRE_PLATFORM != OGRE_PLATFORM_WIN32

#include <dirent.h>
#include <unistd.h>
#include <fnmatch.h>
/* The max number of searches to allow at one time */
#define MAX_FIND_SEARCHES 10

struct _find_search_t
{
    int in_use;
    char *pattern;
    DIR *dirfd;
};

/* Our simplified data entry structure */
struct _finddata_t
{
    char *name;
    int attrib;
    unsigned long size;
};

#define _A_NORMAL 0x00  /* Normalfile-Noread/writerestrictions */
#define _A_RDONLY 0x01  /* Read only file */
#define _A_HIDDEN 0x02  /* Hidden file */
#define _A_SYSTEM 0x04  /* System file */
#define _A_SUBDIR 0x10  /* Subdirectory */
#define _A_ARCH   0x20  /* Archive file */

long _findfirst(const char *pattern, struct _finddata_t *data);
int _findnext(long id, struct _finddata_t *data);
int _findclose(long id);

#endif
