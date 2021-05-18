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
#ifndef __AxisAlignedBox_H_
#define __AxisAlignedBox_H_

// Precompiler options
#include "OgrePrerequisites.h"

#include "OgreVector3.h"
#include "OgreMatrix4.h"

namespace Ogre {

    /** A 3D box aligned with the x/y/z axes.
	    @remarks
		    This class represents a simple box which is aligned with the
		    axes. Internally it only stores 2 points as the extremeties of
		    the box, one which is the minima of all 3 axes, and the other
		    which is the maxima of all 3 axes. This class is typically used
		    for an axis-aligned bounding box (AABB) for collision and
		    visibility determination.
    */
    class _OgreExport AxisAlignedBox
    {
    protected:
	    Vector3 mMinimum;
	    Vector3 mMaximum;
	    bool mNull;

    public:
        // Default construct to null box
	    inline AxisAlignedBox()
            : mMinimum(-0.5f, -0.5f, -0.5)
            , mMaximum(+0.5f, +0.5f, +0.5)
            , mNull(true)
	    {
	    }

	    inline AxisAlignedBox( const Vector3& min, const Vector3& max )
            : mMinimum(min)
            , mMaximum(max)
            , mNull(false)
	    {
	    }

	    inline AxisAlignedBox(
		    Real mx, Real my, Real mz,
		    Real Mx, Real My, Real Mz )
            : mMinimum(mx, my, mz)
            , mMaximum(Mx, My, Mz)
            , mNull(false)
	    {
	    }

	    /** Gets the minimum corner of the box.
	    */
	    inline const Vector3& getMinimum(void) const
	    { 
		    return mMinimum; 
	    }

	    /** Gets the maximum corner of the box.
	    */
	    inline const Vector3& getMaximum(void) const
	    { 
		    return mMaximum;
	    }

	    /** Sets the minimum corner of the box.
	    */
	    inline void setMinimum( const Vector3& vec )
	    {
		    mNull = false;
		    mMinimum = vec;
	    }

	    inline void setMinimum( Real x, Real y, Real z )
	    {
		    mNull = false;
		    mMinimum.x = x;
		    mMinimum.y = y;
		    mMinimum.z = z;
	    }

	    /** Sets the maximum corner of the box.
	    */
	    inline void setMaximum( const Vector3& vec )
	    {
		    mNull = false;
		    mMaximum = vec;
	    }

	    inline void setMaximum( Real x, Real y, Real z )
	    {
		    mNull = false;
		    mMaximum.x = x;
		    mMaximum.y = y;
		    mMaximum.z = z;
	    }

	    /** Sets both minimum and maximum extents at once.
	    */
	    inline void setExtents( const Vector3& min, const Vector3& max )
	    {
		    mNull = false;
		    mMinimum = min;
		    mMaximum = max;
	    }

	    inline void setExtents(
		    Real mx, Real my, Real mz,
		    Real Mx, Real My, Real Mz )
	    {
		    mNull = false;

		    mMinimum.x = mx;
		    mMinimum.y = my;
		    mMinimum.z = mz;

		    mMaximum.x = Mx;
		    mMaximum.y = My;
		    mMaximum.z = Mz;
	    }

	    /** Gets 8 corner points of the box, useful for collision vs. non-aligned objects.
		    @remarks
			    If the order of these corners is important, they are as
			    follows: The 4 points of the minimum Z face (note that
			    because Ogre uses right-handed coordinates, the minimum Z is
			    at the 'back' of the box) starting with the minimum point of
			    all, then anticlockwise around this face (if you are looking
			    onto the face from outside the box). Then the 4 points of the
			    maximum Z face, starting with maximum point of all, then
			    anticlockwise around this face (looking onto the face from
			    outside the box). Like this:
			    <pre>
			       1-----2
			      /|    /|
			     / |   / |
			    5-----4  |
			    |  0--|--3
			    | /   | /
			    |/    |/
			    6-----7
			    </pre>
            @param corners An array of 8 Vector3 to receive corner points.
	    */
	    inline void getAllCorners(Vector3 corners[8]) const
	    {
		    assert( !mNull && "Can't get corners of a null AAB" );

            // The order of these items is, using right-handed co-ordinates:
            // Minimum Z face, starting with Min(all), then anticlockwise
            //   around face (looking onto the face)
            // Maximum Z face, starting with Max(all), then anticlockwise
            //   around face (looking onto the face)
            corners[0] = mMinimum;
            corners[1].x = mMinimum.x; corners[1].y = mMaximum.y; corners[1].z = mMinimum.z;
            corners[2].x = mMaximum.x; corners[2].y = mMaximum.y; corners[2].z = mMinimum.z;
            corners[3].x = mMaximum.x; corners[3].y = mMinimum.y; corners[3].z = mMinimum.z;

            corners[4] = mMaximum;
            corners[5].x = mMinimum.x; corners[5].y = mMaximum.y; corners[5].z = mMaximum.z;
            corners[6].x = mMinimum.x; corners[6].y = mMinimum.y; corners[6].z = mMaximum.z;
            corners[7].x = mMaximum.x; corners[7].y = mMinimum.y; corners[7].z = mMaximum.z;
	    }

	    friend std::ostream& operator<<( std::ostream& o, AxisAlignedBox aab )
	    {
		    if (aab.isNull())
		    {
			    o << "AxisAlignedBox(null)";
		    }
		    else
		    {
			    o << "AxisAlignedBox(min=" << aab.mMinimum << ", max=" << aab.mMaximum << ")";
		    }
		    return o;
	    }

	    /** Merges the passed in box into the current box. The result is the
		    box which encompasses both.
	    */
	    void merge( const AxisAlignedBox& rhs )
	    {
		    // Do nothing if rhs null
		    if (rhs.mNull)
		    {
			    return;
		    }
		    // Otherwise if current null, just take rhs
		    else if (mNull)
		    {
			    setExtents(rhs.mMinimum, rhs.mMaximum);
		    }
		    // Otherwise merge
		    else
		    {
			    mMaximum.makeCeil(rhs.mMaximum);
			    mMinimum.makeFloor(rhs.mMinimum);
		    }

	    }
		
		/** Extends the box to encompass the specified point (if needed).
		*/
		void merge( const Vector3& point )
		{
			if (mNull){ // if null, use this point
				setExtents(point, point);
			} else {
				mMaximum.makeCeil(point);
				mMinimum.makeFloor(point);
			}
		}

	    /** Transforms the box according to the matrix supplied.
		    @remarks
			    By calling this method you get the axis-aligned box which
			    surrounds the transformed version of this box. Therefore each
			    corner of the box is transformed by the matrix, then the
			    extents are mapped back onto the axes to produce another
			    AABB. Useful when you have a local AABB for an object which
			    is then transformed.
	    */
	    void transform( const Matrix4& matrix )
	    {
		    // Do nothing if current null
		    if( mNull )
			    return;

		    Vector3 min, max, tmp, corner;

            corner = mMinimum; min = max = matrix * corner;
            corner.x = mMaximum.x; tmp = matrix * corner; min.makeFloor(tmp); max.makeCeil(tmp);
            corner.y = mMaximum.y; tmp = matrix * corner; min.makeFloor(tmp); max.makeCeil(tmp);
            corner.x = mMinimum.x; tmp = matrix * corner; min.makeFloor(tmp); max.makeCeil(tmp);
            corner.z = mMaximum.z; tmp = matrix * corner; min.makeFloor(tmp); max.makeCeil(tmp);
            corner.x = mMaximum.x; tmp = matrix * corner; min.makeFloor(tmp); max.makeCeil(tmp);
            corner.y = mMinimum.y; tmp = matrix * corner; min.makeFloor(tmp); max.makeCeil(tmp);
            corner.x = mMinimum.x; tmp = matrix * corner; min.makeFloor(tmp); max.makeCeil(tmp);

		    setExtents( min,max );

	    }

	    /** Transforms the box according to the affine matrix supplied.
		    @remarks
			    By calling this method you get the axis-aligned box which
			    surrounds the transformed version of this box. Therefore each
			    corner of the box is transformed by the matrix, then the
			    extents are mapped back onto the axes to produce another
			    AABB. Useful when you have a local AABB for an object which
			    is then transformed.
            @note
                The matrix must be an affine matrix. @see Matrix4::isAffine.
	    */
	    void transformAffine(const Matrix4& m)
	    {
            assert(m.isAffine());

		    // Do nothing if current null
		    if (mNull)
			    return;

            Vector3 centre = getCenter();
            Vector3 halfSize = getHalfSize();

            Vector3 newCentre = m.transformAffine(centre);
            Vector3 newHalfSize(
                Math::Abs(m[0][0]) * halfSize.x + Math::Abs(m[0][1]) * halfSize.y + Math::Abs(m[0][2]) * halfSize.z, 
                Math::Abs(m[1][0]) * halfSize.x + Math::Abs(m[1][1]) * halfSize.y + Math::Abs(m[1][2]) * halfSize.z,
                Math::Abs(m[2][0]) * halfSize.x + Math::Abs(m[2][1]) * halfSize.y + Math::Abs(m[2][2]) * halfSize.z);

            setExtents(newCentre - newHalfSize, newCentre + newHalfSize);
        }

	    /** Sets the box to a 'null' value i.e. not a box.
	    */
	    inline void setNull()
	    {
		    mNull = true;
	    }

	    /** Returns true if the box is null i.e. empty.
	    */
	    bool isNull(void) const
	    {
		    return mNull;
	    }

        /** Returns whether or not this box intersects another. */
        inline bool intersects(const AxisAlignedBox& b2) const
        {
            // Early-fail for nulls
            if (this->isNull() || b2.isNull())
                return false;

            // Use up to 6 separating planes
            if (mMaximum.x < b2.mMinimum.x)
                return false;
            if (mMaximum.y < b2.mMinimum.y)
                return false;
            if (mMaximum.z < b2.mMinimum.z)
                return false;

            if (mMinimum.x > b2.mMaximum.x)
                return false;
            if (mMinimum.y > b2.mMaximum.y)
                return false;
            if (mMinimum.z > b2.mMaximum.z)
                return false;

            // otherwise, must be intersecting
            return true;

        }

		/// Calculate the area of intersection of this box and another
		inline AxisAlignedBox intersection(const AxisAlignedBox& b2) const
		{
            if (this->isNull() || b2.isNull())
			{
                return AxisAlignedBox();
            }

			Vector3 intMin = mMinimum;
            Vector3 intMax = mMaximum;

            intMin.makeCeil(b2.getMinimum());
            intMax.makeFloor(b2.getMaximum());

            // Check intersection isn't null
            if (intMin.x < intMax.x &&
                intMin.y < intMax.y &&
                intMin.z < intMax.z)
            {
                return AxisAlignedBox(intMin, intMax);
            }

            return AxisAlignedBox();
		}

		/// Calculate the volume of this box
		Real volume(void) const
		{
			if (mNull)
			{
				return 0.0f;
			}
			else
			{
				Vector3 diff = mMaximum - mMinimum;
				return diff.x * diff.y * diff.z;
			}

		}

        /** Scales the AABB by the vector given. */
        inline void scale(const Vector3& s)
        {
            // NB assumes centered on origin
            Vector3 min = mMinimum * s;
            Vector3 max = mMaximum * s;
            setExtents(min, max);
        }

		/** Tests whether this box intersects a sphere. */
		bool intersects(const Sphere& s) const
		{
			return Math::intersects(s, *this); 
		}
		/** Tests whether this box intersects a plane. */
		bool intersects(const Plane& p) const
		{
			return Math::intersects(p, *this);
		}
        /** Tests whether the vector point is within this box. */
        bool intersects(const Vector3& v) const
        {
			return(v.x >= mMinimum.x  &&  v.x <= mMaximum.x  && 
			    v.y >= mMinimum.y  &&  v.y <= mMaximum.y  && 
    			v.z >= mMinimum.z  &&  v.z <= mMaximum.z);
        }
		/// Gets the centre of the box
		Vector3 getCenter(void) const
		{
		    return Vector3(
		        (mMaximum.x + mMinimum.x) * 0.5,
		        (mMaximum.y + mMinimum.y) * 0.5,
		        (mMaximum.z + mMinimum.z) * 0.5);
		}
		/// Gets the size of the box
		Vector3 getSize(void) const
		{
		    return Vector3(
		        (mMaximum.x - mMinimum.x),
		        (mMaximum.y - mMinimum.y),
		        (mMaximum.z - mMinimum.z));
		}
		/// Gets the half-size of the box
		Vector3 getHalfSize(void) const
		{
		    return Vector3(
		        (mMaximum.x - mMinimum.x) * 0.5,
		        (mMaximum.y - mMinimum.y) * 0.5,
		        (mMaximum.z - mMinimum.z) * 0.5);
		}

        bool contains(const Vector3& v) const
        {
            if (mNull)
                return false;

            return mMinimum.x <= v.x && v.x <= mMaximum.x &&
                   mMinimum.y <= v.y && v.y <= mMaximum.y &&
                   mMinimum.z <= v.z && v.z <= mMaximum.z;
        }

        bool contains(const AxisAlignedBox& other) const
        {
            if (other.mNull)
                return true;

            if (this->mNull)
                return false;

            return this->mMinimum.x <= other.mMinimum.x &&
                   this->mMinimum.y <= other.mMinimum.y &&
                   this->mMinimum.z <= other.mMinimum.z &&
                   other.mMaximum.x <= this->mMaximum.x &&
                   other.mMaximum.y <= this->mMaximum.y &&
                   other.mMaximum.z <= this->mMaximum.z;
        }

        bool operator== (const AxisAlignedBox& rhs) const
        {
            if (this->mNull != rhs.mNull)
                return false;

            if (this->mNull)
                return true;

            return this->mMinimum == rhs.mMinimum &&
                   this->mMaximum == rhs.mMaximum;
        }

        bool operator!= (const AxisAlignedBox& rhs) const
        {
            return !(*this == rhs);
        }

    };

} // namespace Ogre

#endif
