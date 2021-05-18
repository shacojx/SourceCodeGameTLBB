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
#ifndef __Config_H_
#define __Config_H_

// configure options
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/** If set to 1, profiling code will be included in the application. When you
	are deploying your application you will probably want to set this to 0 */
#define OGRE_PROFILING 0

/** If set to 1, stack unwinding code is compiled into the library and called
    in case an exception is thrown in order to show the call stack.
*/
#define OGRE_STACK_UNWINDING 0

/** There are three modes for handling asserts in OGRE:
0 - STANDARD - Standard asserts in debug builds, nothing in release builds
1 - RELEASE_EXCEPTIONS - Standard asserts in debug builds, exceptions in release builds
2 - EXCEPTIONS - Exceptions in debug builds, exceptions in release builds
*/
#define OGRE_ASSERT_MODE 0

/** If set to >0, OGRE will always 'think' that the graphics card only has the
    number of texture units specified. Very useful for testing multipass fallback.
*/
#define OGRE_PRETEND_TEXTURE_UNITS 0

/** If set to 1, Real is typedef'ed to double. Otherwise, Real is typedef'ed
    to float. Setting this allows you to perform mathematical operations in the
	CPU (Quaternion, Vector3 etc) with more precision, but bear in mind that the
	GPU still operates in single-precision mode.
*/
#ifndef OGRE_DOUBLE_PRECISION
#define OGRE_DOUBLE_PRECISION 0
#endif

/** If set to 1, the strings are transforned to Unicode, and char is replaced
    with wchar_t when having to do with strings of any kind.
*/
#define OGRE_WCHAR_T_STRINGS 0

/** Define number of texture coordinate sets allowed per vertex.
*/
#define OGRE_MAX_TEXTURE_COORD_SETS 6

/** Define max number of texture layers allowed per pass on any card.
*/
#define OGRE_MAX_TEXTURE_LAYERS 16

/** Define max number of lights allowed per pass.
*/
#define OGRE_MAX_SIMULTANEOUS_LIGHTS 8

/** Define max number of blending weights allowed per vertex.
*/
#define OGRE_MAX_BLEND_WEIGHTS 4

/** Set this to zero if you want to link OGRE as a static lib.
*/
#define OGRE_DYNAMIC_LINKAGE 1


/** Set this to 0 if you want to use the standard memory manager in Debug builds
    Release builds always use the standard memory manager
*/
#ifndef OGRE_DEBUG_MEMORY_MANAGER
#define OGRE_DEBUG_MEMORY_MANAGER 0
#endif
/** Define max number of multiple render targets (MRTs) to render to at once.
*/
#define OGRE_MAX_MULTIPLE_RENDER_TARGETS 8

/** Indicate general support for multithreading.
	This will enable threading support in certain parts of the
	engine, mainly resource loading and SharedPtr handling.
	The places where threading is available are clearly
	marked, you should assume state is NOT thread safe unless otherwise
	stated in relation to this flag.
    WARNING: highly experimental, use with caution
*/
#ifndef OGRE_THREAD_SUPPORT
#define OGRE_THREAD_SUPPORT 0
#endif

/** Disables use of the DevIL image library for loading images.
    WARNING: Use only when you want to provide your own image loading code via codecs.
*/
#ifndef OGRE_NO_DEVIL
#define OGRE_NO_DEVIL 0
#endif


#endif
