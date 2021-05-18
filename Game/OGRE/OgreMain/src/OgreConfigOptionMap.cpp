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
#include "OgreConfigOptionMap.h"

#include "OgreException.h"

namespace Ogre {

#ifdef __OBSOLETE__DO__NOT__DEFINE__THIS__
    ConfigOptionMap::ConfigOptionMap()
    {
    }

    ConfigOptionMap::~ConfigOptionMap()
    {
    }

    void ConfigOptionMap::insert(const String& key, const ConfigOption& value)
    {
        mImpl.insert(MapImpl::value_type(key, value));
    }

    ConfigOptionMap::iterator ConfigOptionMap::find(const String& key)
    {
        MapImpl::iterator i = mImpl.find(key);

        return iterator(i, mImpl.end());

    }


    ConfigOptionMap::iterator ConfigOptionMap::begin(void)
    {
        return iterator(mImpl.begin(), mImpl.end());
    }

    ConfigOptionMap::iterator::iterator(MapImpl::iterator startAt, MapImpl::iterator end)
    {
        mIter = startAt;
        mEnd = end;
    }

    bool ConfigOptionMap::iterator::end()
    {
        return (mIter == mEnd);
    }

    ConfigOptionMap::iterator& ConfigOptionMap::iterator::operator++()
    {
        mIter++;
        return *this;
    }

    const String& ConfigOptionMap::iterator::getKey()
    {
        return mIter->first;
    }

    ConfigOption& ConfigOptionMap::iterator::getValue()
    {
        return mIter->second;
    }
#endif

}
