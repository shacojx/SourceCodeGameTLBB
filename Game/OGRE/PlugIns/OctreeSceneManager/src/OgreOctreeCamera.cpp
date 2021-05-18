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
/***************************************************************************
octreecamera.cpp  -  description
-------------------
begin                : Fri Sep 27 2002
copyright            : (C) 2002 by Jon Anderson
email                : janders@users.sf.net

Enhancements 2003 - 2004 (C) The OGRE Team

***************************************************************************/
#include <OgreMath.h>
#include <OgreAxisAlignedBox.h>
#include <OgreRoot.h>

#include <OgreOctreeCamera.h>

namespace Ogre
{
OctreeCamera::OctreeCamera( const String& name, SceneManager* sm ) : Camera( name, sm )
{
                                                                      
}

OctreeCamera::~OctreeCamera()
{
}

OctreeCamera::Visibility OctreeCamera::getVisibility( const AxisAlignedBox &bound )
{

    // Null boxes always invisible
    if ( bound.isNull() )
        return NONE;

    // Make any pending updates to the calculated frustum planes
    updateFrustumPlanes();

    // Get centre of the box
    Vector3 centre = bound.getCenter();
    // Get the half-size of the box
    Vector3 halfSize = bound.getHalfSize();

    bool all_inside = true;

    for ( int plane = 0; plane < 6; ++plane )
    {

        // Skip far plane if infinite view frustum
        if (plane == FRUSTUM_PLANE_FAR && mFarDist == 0)
            continue;

        Plane::Side side = mFrustumPlanes[plane].getSide(centre, halfSize);
        if (side == Plane::NEGATIVE_SIDE)
        {
            return NONE;
        }
        else if (side == Plane::NO_SIDE)
        {
            all_inside = false;
        }
    }

    if ( all_inside )
        return FULL;
    else
        return PARTIAL;

}

}




