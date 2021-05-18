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
#ifndef _COLOURVALUE_H__
#define _COLOURVALUE_H__

#include "OgrePrerequisites.h"

namespace Ogre {

    typedef uint32 RGBA;
    typedef uint32 ARGB;
    typedef uint32 ABGR;
	typedef uint32 BGRA;

    /** Class representing colour.
	    @remarks
		    Colour is represented as 4 components, each of which is a
		    floating-point value from 0.0 to 1.0.
	    @par
		    The 3 'normal' colour components are red, green and blue, a higher
		    number indicating greater amounts of that component in the colour.
		    The forth component is the 'alpha' value, which represents
		    transparency. In this case, 0.0 is completely transparent and 1.0 is
		    fully opaque.
    */
    class _OgreExport ColourValue
    {
#if OGRE_COMPILER == OGRE_COMPILER_MSVC
    protected:
        /// Method pointer to set/retrieves colour as uint32
        typedef uint32 (ColourValue::* GetAsUint32)(void) const;
        typedef void (ColourValue::* SetAsUint32)(const uint32 val);
        static GetAsUint32 msGetAsRGBA;
        static GetAsUint32 msGetAsARGB;
        static GetAsUint32 msGetAsBGRA;
        static GetAsUint32 msGetAsABGR;
        static SetAsUint32 msSetAsRGBA;
        static SetAsUint32 msSetAsARGB;
        static SetAsUint32 msSetAsBGRA;
        static SetAsUint32 msSetAsABGR;

        /// Bootstrap routines for set/retrieves colour as uint32
	    RGBA getAsRGBA_Bootstrap(void) const;
	    ARGB getAsARGB_Bootstrap(void) const;
		BGRA getAsBGRA_Bootstrap(void) const;
	    ABGR getAsABGR_Bootstrap(void) const;
        void setAsRGBA_Bootstrap(const RGBA val);
        void setAsARGB_Bootstrap(const ARGB val);
		void setAsBGRA_Bootstrap(const BGRA val);
        void setAsABGR_Bootstrap(const ABGR val);

        /// General C++ routines for set/retrieves colour as uint32
	    RGBA getAsRGBA_General(void) const;
	    ARGB getAsARGB_General(void) const;
		BGRA getAsBGRA_General(void) const;
	    ABGR getAsABGR_General(void) const;
        void setAsRGBA_General(const RGBA val);
        void setAsARGB_General(const ARGB val);
		void setAsBGRA_General(const BGRA val);
        void setAsABGR_General(const ABGR val);

        /// SSE optimised routines for set/retrieves colour as uint32
	    RGBA getAsRGBA_SSE(void) const;
	    ARGB getAsARGB_SSE(void) const;
		BGRA getAsBGRA_SSE(void) const;
	    ABGR getAsABGR_SSE(void) const;
        void setAsRGBA_SSE(const RGBA val);
        void setAsARGB_SSE(const ARGB val);
		void setAsBGRA_SSE(const BGRA val);
        void setAsABGR_SSE(const ABGR val);
#endif  // OGRE_COMPILER == OGRE_COMPILER_MSVC
    public:
        static const ColourValue ZERO;
        static const ColourValue Black;
        static const ColourValue White;
        static const ColourValue Red;
        static const ColourValue Green;
        static const ColourValue Blue;

	    explicit ColourValue( float red = 1.0f,
				    float green = 1.0f,
				    float blue = 1.0f,
				    float alpha = 1.0f ) : r(red), g(green), b(blue), a(alpha)
        { }

	    bool operator==(const ColourValue& rhs) const;
	    bool operator!=(const ColourValue& rhs) const;

        union {
            struct {
	            float r,g,b,a;
            };
            float val[4];
        };

	    /** Retrieves colour as RGBA.
	    */
	    RGBA getAsRGBA(void) const;

	    /** Retrieves colour as ARGB.
	    */
	    ARGB getAsARGB(void) const;

		/** Retrieves colour as BGRA.
		*/
		BGRA getAsBGRA(void) const;

		/** Retrieves colours as ABGR */
	    ABGR getAsABGR(void) const;

	    /** Sets colour as RGBA.
	    */
        void setAsRGBA(const RGBA val);

	    /** Sets colour as ARGB.
	    */
        void setAsARGB(const ARGB val);

		/** Sets colour as BGRA.
		*/
		void setAsBGRA(const BGRA val);

	    /** Sets colour as ABGR.
	    */
        void setAsABGR(const ABGR val);

        /** Clamps colour value to the range [0, 1].
        */
        void saturate(void)
        {
            if (r < 0)
                r = 0;
            else if (r > 1)
                r = 1;

            if (g < 0)
                g = 0;
            else if (g > 1)
                g = 1;

            if (b < 0)
                b = 0;
            else if (b > 1)
                b = 1;

            if (a < 0)
                a = 0;
            else if (a > 1)
                a = 1;
        }

        /** As saturate, except that this colour value is unaffected and
            the saturated colour value is returned as a copy. */
        ColourValue saturateCopy(void) const
        {
            ColourValue ret = *this;
            ret.saturate();
            return ret;
        }

        // arithmetic operations
        inline ColourValue operator + ( const ColourValue& rkVector ) const
        {
            ColourValue kSum;

            kSum.r = r + rkVector.r;
            kSum.g = g + rkVector.g;
            kSum.b = b + rkVector.b;
            kSum.a = a + rkVector.a;

            return kSum;
        }

        inline ColourValue operator - ( const ColourValue& rkVector ) const
        {
            ColourValue kDiff;

            kDiff.r = r - rkVector.r;
            kDiff.g = g - rkVector.g;
            kDiff.b = b - rkVector.b;
            kDiff.a = a - rkVector.a;

            return kDiff;
        }

        inline ColourValue operator * (const float fScalar ) const
        {
            ColourValue kProd;

            kProd.r = fScalar*r;
            kProd.g = fScalar*g;
            kProd.b = fScalar*b;
            kProd.a = fScalar*a;

            return kProd;
        }

        inline ColourValue operator * ( const ColourValue& rhs) const
        {
            ColourValue kProd;

            kProd.r = rhs.r * r;
            kProd.g = rhs.g * g;
            kProd.b = rhs.b * b;
            kProd.a = rhs.a * a;

            return kProd;
        }

        inline ColourValue operator / ( const ColourValue& rhs) const
        {
            ColourValue kProd;

            kProd.r = rhs.r / r;
            kProd.g = rhs.g / g;
            kProd.b = rhs.b / b;
            kProd.a = rhs.a / a;

            return kProd;
        }

        inline ColourValue operator / (const float fScalar ) const
        {
            assert( fScalar != 0.0 );

            ColourValue kDiv;

            float fInv = 1.0 / fScalar;
            kDiv.r = r * fInv;
            kDiv.g = g * fInv;
            kDiv.b = b * fInv;
            kDiv.a = a * fInv;

            return kDiv;
        }

        inline friend ColourValue operator * (const float fScalar, const ColourValue& rkVector )
        {
            ColourValue kProd;

            kProd.r = fScalar * rkVector.r;
            kProd.g = fScalar * rkVector.g;
            kProd.b = fScalar * rkVector.b;
            kProd.a = fScalar * rkVector.a;

            return kProd;
        }

        // arithmetic updates
        inline ColourValue& operator += ( const ColourValue& rkVector )
        {
            r += rkVector.r;
            g += rkVector.g;
            b += rkVector.b;
            a += rkVector.a;

            return *this;
        }

        inline ColourValue& operator -= ( const ColourValue& rkVector )
        {
            r -= rkVector.r;
            g -= rkVector.g;
            b -= rkVector.b;
            a -= rkVector.a;

            return *this;
        }

        inline ColourValue& operator *= (const float fScalar )
        {
            r *= fScalar;
            g *= fScalar;
            b *= fScalar;
            a *= fScalar;
            return *this;
        }

        inline ColourValue& operator /= (const float fScalar )
        {
            assert( fScalar != 0.0 );

            float fInv = 1.0 / fScalar;

            r *= fInv;
            g *= fInv;
            b *= fInv;
            a *= fInv;

            return *this;
        }

		/** Set a colour value from Hue, Saturation and Brightness.
		@param hue Hue value, scaled to the [0,1] range as opposed to the 0-360
		@param saturation Saturation level, [0,1]
		@param brightness Brightness level, [0,1]
		*/
		void setHSB(Real hue, Real saturation, Real brightness);


		/** Function for writing to a stream.
		*/
		inline _OgreExport friend std::ostream& operator <<
			( std::ostream& o, const ColourValue& c )
		{
			o << "ColourValue(" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ")";
			return o;
		}

    };

#if OGRE_COMPILER == OGRE_COMPILER_MSVC
    FORCEINLINE RGBA ColourValue::getAsRGBA(void) const
    {
        return (this->*msGetAsRGBA)();
    }
    FORCEINLINE ARGB ColourValue::getAsARGB(void) const
    {
        return (this->*msGetAsARGB)();
    }
    FORCEINLINE BGRA ColourValue::getAsBGRA(void) const
    {
        return (this->*msGetAsBGRA)();
    }
    FORCEINLINE ABGR ColourValue::getAsABGR(void) const
    {
        return (this->*msGetAsABGR)();
    }
    FORCEINLINE void ColourValue::setAsRGBA(const RGBA val)
    {
        (this->*msSetAsRGBA)(val);
    }
    FORCEINLINE void ColourValue::setAsARGB(const ARGB val)
    {
        (this->*msSetAsARGB)(val);
    }
    FORCEINLINE void ColourValue::setAsBGRA(const BGRA val)
    {
        (this->*msSetAsBGRA)(val);
    }
    FORCEINLINE void ColourValue::setAsABGR(const ABGR val)
    {
        (this->*msSetAsABGR)(val);
    }
#endif  // OGRE_COMPILER == OGRE_COMPILER_MSVC
} // namespace

#endif
