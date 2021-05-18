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


#include "OgreSearchOps.h"
#include <stdio.h>
#include <ctype.h>

/* Win32 directory operations emulation */
#if OGRE_PLATFORM != OGRE_PLATFORM_WIN32

#include "OgreNoMemoryMacros.h"

/* If we've initialized yet */
static int G_searches_initialized = 0;

/* The possible searches */
static struct _find_search_t G_find_searches[MAX_FIND_SEARCHES];

long _findfirst(const char *pattern, struct _finddata_t *data)
{
    long find_key = 0;
    
    /* Initialize the system if it's needed */
    if (!G_searches_initialized)
    {
        int x;
        
        for (x = 0; x < MAX_FIND_SEARCHES; x++)
        {
            G_find_searches[x].in_use = 0;
        }

        G_searches_initialized = 1;
    }

    /* See if we have an available search slot */
    for (find_key = 0; find_key < MAX_FIND_SEARCHES; find_key++)
    {
        if (!G_find_searches[find_key].in_use)
            break;
    }

    if (find_key == MAX_FIND_SEARCHES)
    {
        /* uhoh, no more slots available */
        return -1;
    }
    else
    {
        /* We're using the slot */
        G_find_searches[find_key].in_use = 1;
    }

    if ( !(G_find_searches[find_key].dirfd = opendir(".")) )
        return -1;

    /* Hack for *.* from DOS/Windows */
    if (strcmp(pattern, "*.*") == 0)
        G_find_searches[find_key].pattern = strdup("*");
    else
        G_find_searches[find_key].pattern = strdup(pattern);

    /* Get the first entry */
    if (_findnext(find_key, data) < 0)
    {
        data = NULL;
        _findclose(find_key);
        return -1;
    }

    return find_key;
}

int _findnext(long id, struct _finddata_t *data)
{
    struct dirent *entry;
    struct stat stat_buf;

    /* Loop until we run out of entries or find the next one */
    do
    {
        entry = readdir(G_find_searches[id].dirfd);

        if (entry == NULL)
            return -1;

        /* See if the filename matches our pattern */
        if (fnmatch(G_find_searches[id].pattern, entry->d_name, 0) == 0)
            break;
    } while ( entry != NULL );

    data->name = entry->d_name;

    /* Default type to a normal file */
    data->attrib = _A_NORMAL;
    
    /* stat the file to get if it's a subdir */
    stat(data->name, &stat_buf);
    if (S_ISDIR(stat_buf.st_mode))
    {
        data->attrib = _A_SUBDIR;
    }

    data->size = stat_buf.st_size;

    return 0;
}

int _findclose(long id)
{
    int ret;
    
    ret = closedir(G_find_searches[id].dirfd);
    free(G_find_searches[id].pattern);
    G_find_searches[id].in_use = 0;

    return ret;
}

#include "OgreMemoryMacros.h"
#endif
