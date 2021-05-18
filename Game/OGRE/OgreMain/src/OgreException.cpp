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
#include "OgreException.h"

#include "OgreRoot.h"
#include "OgreLogManager.h"

#ifdef __BORLANDC__
    #include <stdio.h>
#endif

namespace Ogre {

    Exception* Exception::last = NULL;

    String Exception::msFunctionStack[ OGRE_CALL_STACK_DEPTH ];
    ushort   Exception::msStackDepth = 0;

    Exception::Exception(int num, const String& desc, const String& src) :
        line( 0 ),
        number( num ),
        description( desc ),
        source( src ),
        stackDepth( msStackDepth )
    {
        // Log this error - not any more, allow catchers to do it
        //LogManager::getSingleton().logMessage(this->getFullDescription());

        // Set last
        last = this;
    }

    Exception::Exception(int num, const String& desc, const String& src, const char* fil, long lin) :
        line( lin ),
        number( num ),
        description( desc ),
        source( src ),
        file( fil ),
        stackDepth( msStackDepth )
    {
        // Log this error, mask it from debug though since it may be caught and ignored
        if(LogManager::getSingletonPtr())
            LogManager::getSingleton().logMessage(this->getFullDescription(), 
                LML_CRITICAL, true);

        // Set last
        last = this;
    }

    Exception::Exception(const Exception& rhs)
        : line( rhs.line ), number( rhs.number ), description( rhs.description ), source( rhs.source ), file( rhs.file )
    {
    }

    Exception& Exception::operator = ( const Exception& rhs )
    {
        description = rhs.description;
        number = rhs.number;
        source = rhs.source;
        file = rhs.file;
        line = rhs.line;
        return *this;
    }

#if OGRE_COMPILER == OGRE_COMPILER_MSVC
    Exception::~Exception()
#else
    Exception::~Exception() throw()
#endif
    {
    }

    const char *Exception::what() const throw()
    {
        return description.c_str();
    }

    String Exception::getFullDescription(void) const
    {
		StringUtil::StrStreamType desc;

        desc <<  "An exception has been thrown!\n"
                "\n"
                "-----------------------------------\nDetails:\n-----------------------------------\n"
                "Error #: " << number
			<< "\nFunction: " << source
			<< "\nDescription: " << description 
			<< ". ";

        if( line > 0 )
        {
            desc << "\nFile: " << file;
            desc << "\nLine: " << line;
        }

#ifdef OGRE_STACK_UNWINDING
        desc << "\nStack unwinding: ";

        /* Will cause an overflow, that's why we check that it's smaller.
           Also note that the call stack index may be greater than the actual call
           stack size - that's why we begin unrolling with the smallest of the two. */
        for( 
            ushort stackUnroll = stackDepth <= OGRE_CALL_STACK_DEPTH ? ( stackDepth - 1 ) : ( OGRE_CALL_STACK_DEPTH - 1 ); 
            stackUnroll < stackDepth; stackUnroll-- )
        {
            desc << msFunctionStack[ stackUnroll ];
            desc << "(..) <- ";
        }

        desc << "<<beginning of stack>>";
#endif

        return desc.str();
    }

    int Exception::getNumber(void) const throw()
    {
        return number;
    }

    Exception* Exception::getLastException(void) throw()
    {
        return last;
    }

    //-----------------------------------------------------------------------
    void Exception::_pushFunction( const String& strFuncName ) throw()
    {
        if( msStackDepth < OGRE_CALL_STACK_DEPTH )
            msFunctionStack[ msStackDepth ] = strFuncName;
        msStackDepth++;
    }

    //-----------------------------------------------------------------------
    void Exception::_popFunction() throw()
    {
        msStackDepth--;
    }

    //-----------------------------------------------------------------------
    void Exception::_throw(int num, const String& desc, const String& src, const char* file, long line)
    {
        throw Ogre::Exception( num, desc, src, file, line );
    }
    //-----------------------------------------------------------------------
    void Exception::_throw(int num, const String& desc, const char* src, const char* file, long line)
    {
        throw Ogre::Exception( num, desc, src, file, line );
    }
    //-----------------------------------------------------------------------
    void Exception::_throw(int num, const char* desc, const String& src, const char* file, long line)
    {
        throw Ogre::Exception( num, desc, src, file, line );
    }
    //-----------------------------------------------------------------------
    void Exception::_throw(int num, const char* desc, const char* src, const char* file, long line)
    {
        throw Ogre::Exception( num, desc, src, file, line );
    }
}

