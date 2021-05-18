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
#include "OgreColourValue.h"

#if OGRE_COMPILER == OGRE_COMPILER_MSVC
#include "OgrePlatformInformation.h"

// disable function has no EMMS instruction warning.
#pragma warning(disable : 4799)

#include "OgreNoMemoryMacros.h"
#include <xmmintrin.h>
#include "OgreMemoryMacros.h"
#endif  // OGRE_COMPILER == OGRE_COMPILER_MSVC

namespace Ogre {

#if OGRE_COMPILER == OGRE_COMPILER_MSVC
    //---------------------------------------------------------------------
    // Hacky class for ensure stupid Visual C/C++ generate EMMS instruction at the end of the function.
    class MMXCleaner
    {
    public:
        ~MMXCleaner() { _mm_empty(); }
    };
    class MMXResult
    {
    protected:
        uint32 mValue;
    public:
        MMXResult(uint32 value) : mValue(value) {}
        ~MMXResult() { _mm_empty(); }
        operator uint32 () const { return mValue; }
    };
    //---------------------------------------------------------------------
    ColourValue::GetAsUint32 ColourValue::msGetAsRGBA = &ColourValue::getAsRGBA_Bootstrap;
    ColourValue::GetAsUint32 ColourValue::msGetAsARGB = &ColourValue::getAsARGB_Bootstrap;
    ColourValue::GetAsUint32 ColourValue::msGetAsBGRA = &ColourValue::getAsBGRA_Bootstrap;
    ColourValue::GetAsUint32 ColourValue::msGetAsABGR = &ColourValue::getAsABGR_Bootstrap;
    ColourValue::SetAsUint32 ColourValue::msSetAsRGBA = &ColourValue::setAsRGBA_Bootstrap;
    ColourValue::SetAsUint32 ColourValue::msSetAsARGB = &ColourValue::setAsARGB_Bootstrap;
    ColourValue::SetAsUint32 ColourValue::msSetAsBGRA = &ColourValue::setAsBGRA_Bootstrap;
    ColourValue::SetAsUint32 ColourValue::msSetAsABGR = &ColourValue::setAsABGR_Bootstrap;
    //---------------------------------------------------------------------
    static const __m128 sV255 = { 255.f, 255.f, 255.f, 255.f };
    static const __m128 sV1over255 = { 1.f/255.f, 1.f/255.f, 1.f/255.f, 1.f/255.f };
    //---------------------------------------------------------------------
    //---------------------------------------------------------------------
    RGBA ColourValue::getAsRGBA_Bootstrap(void) const
    {
        msGetAsRGBA = &ColourValue::getAsRGBA_General;
        if (PlatformInformation::hasCpuFeature(PlatformInformation::CPU_FEATURE_SSE) &&
            PlatformInformation::hasCpuFeature(PlatformInformation::CPU_FEATURE_MMXEXT) &&
            PlatformInformation::hasCpuFeature(PlatformInformation::CPU_FEATURE_MMX))
            msGetAsRGBA = &ColourValue::getAsRGBA_SSE;
        return getAsRGBA();
    }
    //---------------------------------------------------------------------
    ARGB ColourValue::getAsARGB_Bootstrap(void) const
    {
        msGetAsARGB = &ColourValue::getAsARGB_General;
        if (PlatformInformation::hasCpuFeature(PlatformInformation::CPU_FEATURE_SSE) &&
            PlatformInformation::hasCpuFeature(PlatformInformation::CPU_FEATURE_MMXEXT) &&
            PlatformInformation::hasCpuFeature(PlatformInformation::CPU_FEATURE_MMX))
            msGetAsARGB = &ColourValue::getAsARGB_SSE;
        return getAsARGB();
    }
    //---------------------------------------------------------------------
    BGRA ColourValue::getAsBGRA_Bootstrap(void) const
    {
        msGetAsBGRA = &ColourValue::getAsBGRA_General;
        if (PlatformInformation::hasCpuFeature(PlatformInformation::CPU_FEATURE_SSE) &&
            PlatformInformation::hasCpuFeature(PlatformInformation::CPU_FEATURE_MMXEXT) &&
            PlatformInformation::hasCpuFeature(PlatformInformation::CPU_FEATURE_MMX))
            msGetAsBGRA = &ColourValue::getAsBGRA_SSE;
        return getAsBGRA();
    }
    //---------------------------------------------------------------------
    ABGR ColourValue::getAsABGR_Bootstrap(void) const
    {
        msGetAsABGR = &ColourValue::getAsABGR_General;
        if (PlatformInformation::hasCpuFeature(PlatformInformation::CPU_FEATURE_SSE) &&
            PlatformInformation::hasCpuFeature(PlatformInformation::CPU_FEATURE_MMXEXT) &&
            PlatformInformation::hasCpuFeature(PlatformInformation::CPU_FEATURE_MMX))
            msGetAsABGR = &ColourValue::getAsABGR_SSE;
        return getAsABGR();
    }
    //---------------------------------------------------------------------
    void ColourValue::setAsRGBA_Bootstrap(const RGBA val)
    {
        msSetAsRGBA = &ColourValue::setAsRGBA_General;
        if (PlatformInformation::hasCpuFeature(PlatformInformation::CPU_FEATURE_SSE) &&
            PlatformInformation::hasCpuFeature(PlatformInformation::CPU_FEATURE_MMXEXT) &&
            PlatformInformation::hasCpuFeature(PlatformInformation::CPU_FEATURE_MMX))
            msSetAsRGBA = &ColourValue::setAsRGBA_SSE;
        return setAsRGBA(val);
    }
    //---------------------------------------------------------------------
    void ColourValue::setAsARGB_Bootstrap(const ARGB val)
    {
        msSetAsARGB = &ColourValue::setAsARGB_General;
        if (PlatformInformation::hasCpuFeature(PlatformInformation::CPU_FEATURE_SSE) &&
            PlatformInformation::hasCpuFeature(PlatformInformation::CPU_FEATURE_MMXEXT) &&
            PlatformInformation::hasCpuFeature(PlatformInformation::CPU_FEATURE_MMX))
            msSetAsARGB = &ColourValue::setAsARGB_SSE;
        return setAsARGB(val);
    }
    //---------------------------------------------------------------------
    void ColourValue::setAsBGRA_Bootstrap(const BGRA val)
    {
        msSetAsBGRA = &ColourValue::setAsBGRA_General;
        if (PlatformInformation::hasCpuFeature(PlatformInformation::CPU_FEATURE_SSE) &&
            PlatformInformation::hasCpuFeature(PlatformInformation::CPU_FEATURE_MMXEXT) &&
            PlatformInformation::hasCpuFeature(PlatformInformation::CPU_FEATURE_MMX))
            msSetAsBGRA = &ColourValue::setAsBGRA_SSE;
        return setAsBGRA(val);
    }
    //---------------------------------------------------------------------
    void ColourValue::setAsABGR_Bootstrap(const ABGR val)
    {
        msSetAsABGR = &ColourValue::setAsABGR_General;
        if (PlatformInformation::hasCpuFeature(PlatformInformation::CPU_FEATURE_SSE) &&
            PlatformInformation::hasCpuFeature(PlatformInformation::CPU_FEATURE_MMXEXT) &&
            PlatformInformation::hasCpuFeature(PlatformInformation::CPU_FEATURE_MMX))
            msSetAsABGR = &ColourValue::setAsABGR_SSE;
        return setAsABGR(val);
    }
    //---------------------------------------------------------------------
    //---------------------------------------------------------------------
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
    ABGR ColourValue::getAsABGR_SSE(void) const
#else
    RGBA ColourValue::getAsRGBA_SSE(void) const
#endif
    {
        __m128 v = _mm_loadu_ps(this->val);
        v = _mm_shuffle_ps(v, v, _MM_SHUFFLE(0,1,2,3));
        v = _mm_mul_ps(v, sV255);
        __m64 t = _mm_packs_pi32(_mm_cvtps_pi32(v), _mm_cvtps_pi32(_mm_movehl_ps(v, v)));
        return MMXResult(_mm_cvtsi64_si32(_mm_packs_pu16(t, _mm_setzero_si64())));
    }
    //---------------------------------------------------------------------
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
    BGRA ColourValue::getAsBGRA_SSE(void) const
#else
    ARGB ColourValue::getAsARGB_SSE(void) const
#endif
    {
        __m128 v = _mm_loadu_ps(this->val);
        v = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3,0,1,2));
        v = _mm_mul_ps(v, sV255);
        __m64 t = _mm_packs_pi32(_mm_cvtps_pi32(v), _mm_cvtps_pi32(_mm_movehl_ps(v, v)));
        return MMXResult(_mm_cvtsi64_si32(_mm_packs_pu16(t, _mm_setzero_si64())));
    }
	//---------------------------------------------------------------------
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
	ARGB ColourValue::getAsARGB_SSE(void) const
#else
	BGRA ColourValue::getAsBGRA_SSE(void) const
#endif
	{
        __m128 v = _mm_loadu_ps(this->val);
        v = _mm_shuffle_ps(v, v, _MM_SHUFFLE(2,1,0,3));
        v = _mm_mul_ps(v, sV255);
        __m64 t = _mm_packs_pi32(_mm_cvtps_pi32(v), _mm_cvtps_pi32(_mm_movehl_ps(v, v)));
        return MMXResult(_mm_cvtsi64_si32(_mm_packs_pu16(t, _mm_setzero_si64())));
	}
    //---------------------------------------------------------------------
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
    RGBA ColourValue::getAsRGBA_SSE(void) const
#else
    ABGR ColourValue::getAsABGR_SSE(void) const
#endif
    {
        __m128 v = _mm_loadu_ps(this->val);
        //v = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3,2,1,0));   // unnecessary
        v = _mm_mul_ps(v, sV255);
        __m64 t = _mm_packs_pi32(_mm_cvtps_pi32(v), _mm_cvtps_pi32(_mm_movehl_ps(v, v)));
        return MMXResult(_mm_cvtsi64_si32(_mm_packs_pu16(t, _mm_setzero_si64())));
    }
    //---------------------------------------------------------------------
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
    void ColourValue::setAsABGR_SSE(const ABGR val)
#else
    void ColourValue::setAsRGBA_SSE(const RGBA val)
#endif
    {
        MMXCleaner cleaner;

        __m128 v = _mm_cvtpu8_ps(_mm_cvtsi32_si64(val));
        v = _mm_shuffle_ps(v, v, _MM_SHUFFLE(0,1,2,3));
        v = _mm_mul_ps(v, sV1over255);
        _mm_storeu_ps(this->val, v);
    }
    //---------------------------------------------------------------------
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
    void ColourValue::setAsBGRA_SSE(const BGRA val)
#else
    void ColourValue::setAsARGB_SSE(const ARGB val)
#endif
    {
        MMXCleaner cleaner;

        __m128 v = _mm_cvtpu8_ps(_mm_cvtsi32_si64(val));
        v = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3,0,1,2));
        v = _mm_mul_ps(v, sV1over255);
        _mm_storeu_ps(this->val, v);
    }
	//---------------------------------------------------------------------
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
	void ColourValue::setAsARGB_SSE(const ARGB val)
#else
	void ColourValue::setAsBGRA_SSE(const BGRA val)
#endif
	{
        MMXCleaner cleaner;

        __m128 v = _mm_cvtpu8_ps(_mm_cvtsi32_si64(val));
        v = _mm_shuffle_ps(v, v, _MM_SHUFFLE(0,3,2,1));
        v = _mm_mul_ps(v, sV1over255);
        _mm_storeu_ps(this->val, v);
	}
    //---------------------------------------------------------------------
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
    void ColourValue::setAsRGBA_SSE(const RGBA val)
#else
    void ColourValue::setAsABGR_SSE(const ABGR val)
#endif
    {
        MMXCleaner cleaner;

        __m128 v = _mm_cvtpu8_ps(_mm_cvtsi32_si64(val));
        //v = _mm_shuffle_ps(v, v, _MM_SHUFFLE(3,2,1,0));   // unnecessary
        v = _mm_mul_ps(v, sV1over255);
        _mm_storeu_ps(this->val, v);
    }
    //---------------------------------------------------------------------
    //---------------------------------------------------------------------
#define getAsRGBA   getAsRGBA_General
#define getAsARGB   getAsARGB_General
#define getAsBGRA   getAsBGRA_General
#define getAsABGR   getAsABGR_General
#define setAsRGBA   setAsRGBA_General
#define setAsARGB   setAsARGB_General
#define setAsBGRA   setAsBGRA_General
#define setAsABGR   setAsABGR_General
#endif  // OGRE_COMPILER == OGRE_COMPILER_MSVC

    const ColourValue ColourValue::ZERO = ColourValue(0.0,0.0,0.0,0.0);
    const ColourValue ColourValue::Black = ColourValue(0.0,0.0,0.0);
    const ColourValue ColourValue::White = ColourValue(1.0,1.0,1.0);
    const ColourValue ColourValue::Red = ColourValue(1.0,0.0,0.0);
    const ColourValue ColourValue::Green = ColourValue(0.0,1.0,0.0);
    const ColourValue ColourValue::Blue = ColourValue(0.0,0.0,1.0);

    //---------------------------------------------------------------------
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
    ABGR ColourValue::getAsABGR(void) const
#else
    RGBA ColourValue::getAsRGBA(void) const
#endif
    {
        uint8 val8;
        uint32 val32 = 0;

        // Convert to 32bit pattern
        // (RGBA = 8888)

        // Red
        val8 = static_cast<uint8>(r * 255);
        val32 = val8 << 24;

        // Green
        val8 = static_cast<uint8>(g * 255);
        val32 += val8 << 16;

        // Blue
        val8 = static_cast<uint8>(b * 255);
        val32 += val8 << 8;

        // Alpha
        val8 = static_cast<uint8>(a * 255);
        val32 += val8;

        return val32;
    }
    //---------------------------------------------------------------------
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
    BGRA ColourValue::getAsBGRA(void) const
#else
    ARGB ColourValue::getAsARGB(void) const
#endif
    {
        uint8 val8;
        uint32 val32 = 0;

        // Convert to 32bit pattern
        // (ARGB = 8888)

        // Alpha
        val8 = static_cast<uint8>(a * 255);
        val32 = val8 << 24;

        // Red
        val8 = static_cast<uint8>(r * 255);
        val32 += val8 << 16;

        // Green
        val8 = static_cast<uint8>(g * 255);
        val32 += val8 << 8;

        // Blue
        val8 = static_cast<uint8>(b * 255);
        val32 += val8;


        return val32;
    }
	//---------------------------------------------------------------------
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
	ARGB ColourValue::getAsARGB(void) const
#else
	BGRA ColourValue::getAsBGRA(void) const
#endif
	{
		uint8 val8;
		uint32 val32 = 0;

		// Convert to 32bit pattern
		// (ARGB = 8888)

		// Blue
		val8 = static_cast<uint8>(b * 255);
		val32 = val8 << 24;

		// Green
		val8 = static_cast<uint8>(g * 255);
		val32 += val8 << 16;

		// Red
		val8 = static_cast<uint8>(r * 255);
		val32 += val8 << 8;

		// Alpha
		val8 = static_cast<uint8>(a * 255);
		val32 += val8;


		return val32;
	}
    //---------------------------------------------------------------------
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
    RGBA ColourValue::getAsRGBA(void) const
#else
    ABGR ColourValue::getAsABGR(void) const
#endif
    {
        uint8 val8;
        uint32 val32 = 0;

        // Convert to 32bit pattern
        // (ABRG = 8888)

        // Alpha
        val8 = static_cast<uint8>(a * 255);
        val32 = val8 << 24;

        // Blue
        val8 = static_cast<uint8>(b * 255);
        val32 += val8 << 16;

        // Green
        val8 = static_cast<uint8>(g * 255);
        val32 += val8 << 8;

        // Red
        val8 = static_cast<uint8>(r * 255);
        val32 += val8;


        return val32;
    }
    //---------------------------------------------------------------------
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
    void ColourValue::setAsABGR(const ABGR val)
#else
    void ColourValue::setAsRGBA(const RGBA val)
#endif
    {
        uint32 val32 = val;

        // Convert from 32bit pattern
        // (RGBA = 8888)

        // Red
        r = ((val32 >> 24) & 0xFF) / 255.0f;

        // Green
        g = ((val32 >> 16) & 0xFF) / 255.0f;

        // Blue
        b = ((val32 >> 8) & 0xFF) / 255.0f;

        // Alpha
        a = (val32 & 0xFF) / 255.0f;
    }
    //---------------------------------------------------------------------
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
    void ColourValue::setAsBGRA(const BGRA val)
#else
    void ColourValue::setAsARGB(const ARGB val)
#endif
    {
        uint32 val32 = val;

        // Convert from 32bit pattern
        // (ARGB = 8888)

        // Alpha
        a = ((val32 >> 24) & 0xFF) / 255.0f;

        // Red
        r = ((val32 >> 16) & 0xFF) / 255.0f;

        // Green
        g = ((val32 >> 8) & 0xFF) / 255.0f;

        // Blue
        b = (val32 & 0xFF) / 255.0f;
    }
	//---------------------------------------------------------------------
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
	void ColourValue::setAsARGB(const ARGB val)
#else
	void ColourValue::setAsBGRA(const BGRA val)
#endif
	{
		uint32 val32 = val;

		// Convert from 32bit pattern
		// (ARGB = 8888)

		// Blue
		b = ((val32 >> 24) & 0xFF) / 255.0f;

		// Green
		g = ((val32 >> 16) & 0xFF) / 255.0f;

		// Red
		r = ((val32 >> 8) & 0xFF) / 255.0f;

		// Alpha
		a = (val32 & 0xFF) / 255.0f;
	}
    //---------------------------------------------------------------------
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
    void ColourValue::setAsRGBA(const RGBA val)
#else
    void ColourValue::setAsABGR(const ABGR val)
#endif
    {
        uint32 val32 = val;

        // Convert from 32bit pattern
        // (ABGR = 8888)

        // Alpha
        a = ((val32 >> 24) & 0xFF) / 255.0f;

        // Blue
        b = ((val32 >> 16) & 0xFF) / 255.0f;

        // Green
        g = ((val32 >> 8) & 0xFF) / 255.0f;

        // Red
        r = (val32 & 0xFF) / 255.0f;
    }
    //---------------------------------------------------------------------
    bool ColourValue::operator==(const ColourValue& rhs) const
    {
        return (r == rhs.r &&
            g == rhs.g &&
            b == rhs.b &&
            a == rhs.a);
    }
    //---------------------------------------------------------------------
    bool ColourValue::operator!=(const ColourValue& rhs) const
    {
        return !(*this == rhs);
    }
	//---------------------------------------------------------------------
	void ColourValue::setHSB(Real hue, Real saturation, Real brightness)
	{
		// wrap hue
		if (hue > 1.0f)
		{
			hue -= (int)hue;
		}
		else if (hue < 0.0f)
		{
			hue += (int)hue + 1;
		}
		// clamp saturation / brightness
		saturation = std::min(saturation, (Real)1.0);
		saturation = std::max(saturation, (Real)0.0);
		brightness = std::min(brightness, (Real)1.0);
		brightness = std::max(brightness, (Real)0.0);

		if (brightness == 0.0f)
		{   
			// early exit, this has to be black
			r = g = b = 0.0f;
			return;
		}

		if (saturation == 0.0f)
		{   
			// early exit, this has to be grey

			r = g = b = brightness;
			return;
		}


		Real hueDomain  = hue * 6.0f;
		if (hueDomain >= 6.0f)
		{
			// wrap around, and allow mathematical errors
			hueDomain = 0.0f;
		}
		unsigned short domain = (unsigned short)hueDomain;
		Real f1 = brightness * (1 - saturation);
		Real f2 = brightness * (1 - saturation * (hueDomain - domain));
		Real f3 = brightness * (1 - saturation * (1 - (hueDomain - domain)));

		switch (domain)
		{
		case 0:
			// red domain; green ascends
			r = brightness;
			g = f3;
			b = f1;
			break;
		case 1:
			// yellow domain; red descends
			r = f2;
			g = brightness;
			b = f1;
			break;
		case 2:
			// green domain; blue ascends
			r = f1;
			g = brightness;
			b = f3;
			break;
		case 3:
			// cyan domain; green descends
			r = f1;
			g = f2;
			b = brightness;
			break;
		case 4:
			// blue domain; red ascends
			r = f3;
			g = f1;
			b = brightness;
			break;
		case 5:
			// magenta domain; blue descends
			r = brightness;
			g = f1;
			b = f2;
			break;
		}


	}

}

