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
octree.cpp  -  description
-------------------
begin                : Mon Sep 30 2002
copyright            : (C) 2002 by Jon Anderson
email                : janders@users.sf.net

Enhancements 2003 - 2004 (C) The OGRE Team

***************************************************************************/

#include <OgreOctree.h>
#include <OgreOctreeNode.h>

namespace Ogre
{

/** Returns true is the box will fit in a child.
*/
bool Octree::_isTwiceSize( const AxisAlignedBox &box ) const
{
    return ( ( box.getMaximum().x - box.getMinimum().x ) <= ( mBox.getMaximum().x - mBox.getMinimum().x ) / 2 ) &&
           ( ( box.getMaximum().y - box.getMinimum().y ) <= ( mBox.getMaximum().y - mBox.getMinimum().y ) / 2 ) &&
           ( ( box.getMaximum().z - box.getMinimum().z ) <= ( mBox.getMaximum().z - mBox.getMinimum().z ) / 2 ) ;

}

/** It's assumed the the given box has already been proven to fit into
* a child.  Since it's a loose octree, only the centers need to be
* compared to find the appropriate node.
*/
void Octree::_getChildIndexes( const AxisAlignedBox &box, int *x, int *y, int *z ) const
{
    Vector3 max = mBox.getMaximum();
    Vector3 min = box.getMinimum();

    Vector3 center = mBox.getMaximum().midPoint( mBox.getMinimum() );

    Vector3 ncenter = box.getMaximum().midPoint( box.getMinimum() );

    if ( ncenter.x > center.x )
        * x = 1;
    else
        *x = 0;

    if ( ncenter.y > center.y )
        * y = 1;
    else
        *y = 0;

    if ( ncenter.z > center.z )
        * z = 1;
    else
        *z = 0;

}

Octree::Octree( Octree * parent ) 
    : mWireBoundingBox(0),
      mHalfSize( 0, 0, 0 )
{
    //initialize all children to null.
    for ( int i = 0; i < 2; i++ )
    {
        for ( int j = 0; j < 2; j++ )
        {
            for ( int k = 0; k < 2; k++ )
            {
                mChildren[ i ][ j ][ k ] = 0;
            }
        }
    }

    mParent = parent;
    mNumNodes = 0;
}

Octree::~Octree()
{
    //initialize all children to null.
    for ( int i = 0; i < 2; i++ )
    {
        for ( int j = 0; j < 2; j++ )
        {
            for ( int k = 0; k < 2; k++ )
            {
                if ( mChildren[ i ][ j ][ k ] != 0 )
                    delete mChildren[ i ][ j ][ k ];
            }
        }
    }

    if(mWireBoundingBox)
        delete mWireBoundingBox;

    mParent = 0;
}

void Octree::_addNode( OctreeNode * n )
{
    mNodes.push_back( n );
    n -> setOctant( this );

    //update total counts.
    _ref();

}

void Octree::_removeNode( OctreeNode * n )
{
    mNodes.erase( std::find( mNodes.begin(), mNodes.end(), n ) );
    n -> setOctant( 0 );

    //update total counts.
    _unref();
}

void Octree::_getCullBounds( AxisAlignedBox *b ) const
{
    b->setExtents(mBox.getMinimum() - mHalfSize, mBox.getMaximum() + mHalfSize);
}

WireBoundingBox* Octree::getWireBoundingBox()
{
    // Create a WireBoundingBox if needed
    if(mWireBoundingBox == 0)
        mWireBoundingBox = new WireBoundingBox();

    mWireBoundingBox->setupBoundingBox(mBox);
    return mWireBoundingBox;
}

}
