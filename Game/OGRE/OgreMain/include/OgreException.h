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
#ifndef __Exception_H_
#define __Exception_H_

// Precompiler options
#include "OgrePrerequisites.h"

#include "OgreString.h"

#include <exception>

#if OGRE_COMPILER == OGRE_COMPILER_MSVC
#define OGRE_NORETURN __declspec(noreturn)
#endif

// If compiler supports declare no-return function, we can optimise exception throwing a little.
#ifdef OGRE_NORETURN
#define OGRE_DECL_NORETURN OGRE_NORETURN
#define OGRE_EXCEPT( num, desc, src ) Ogre::Exception::_throw( num, desc, src, __FILE__, __LINE__ )
#else
#define OGRE_DECL_NORETURN
#define OGRE_EXCEPT( num, desc, src ) throw( Ogre::Exception( num, desc, src, __FILE__, __LINE__ ) )
#endif

// Stack unwinding options
// OgreUnguard and OgreUnguardRet are deprecated
#if OGRE_STACK_UNWINDING == 1
#   if OGRE_COMPILER != OGRE_COMPILER_BORL
#       define OgreGuard( a ) Ogre::AutomaticGuardUnguard _auto_guard_object( (a) )
#   else
#       define OgreGuard( a ) Ogre::AutomaticGuardUnguard _auto_guard_object( __FUNC__ )
#   endif

#   define OgreUnguard() 
#   define OgreUnguardRet( a ) return a

#else
#   define OgreGuard( a )
#   define OgreUnguard()
#   define OgreUnguardRet( a ) return a

#endif

// Backwards compatibility with old assert mode definitions
#if OGRE_RELEASE_ASSERT == 1
#   define OGRE_ASSERT_MODE 1
#endif

// Check for OGRE assert mode

// RELEASE_EXCEPTIONS mode
#if OGRE_ASSERT_MODE == 1
#   ifdef _DEBUG
#       define OgreAssert( a, b ) assert( (a) && (b) )

#   else
#       if OGRE_COMP != OGRE_COMPILER_BORL
#           define OgreAssert( a, b ) if( !(a) ) OGRE_EXCEPT( Ogre::Exception::ERR_RT_ASSERTION_FAILED, (b), "no function info")
#       else
#           define OgreAssert( a, b ) if( !(a) ) OGRE_EXCEPT( Ogre::Exception::ERR_RT_ASSERTION_FAILED, (b), __FUNC__ )
#       endif

#   endif

// EXCEPTIONS mode
#elif OGRE_ASSERT_MODE == 2
#   if OGRE_COMP != OGRE_COMPILER_BORL
#       define OgreAssert( a, b ) if( !(a) ) OGRE_EXCEPT( Ogre::Exception::ERR_RT_ASSERTION_FAILED, (b), "no function info")
#   else
#       define OgreAssert( a, b ) if( !(a) ) OGRE_EXCEPT( Ogre::Exception::ERR_RT_ASSERTION_FAILED, (b), __FUNC__ )
#   endif

// STANDARD mode
#else
#   define OgreAssert( a, b ) assert( (a) && (b) )

#endif

#define OGRE_CALL_STACK_DEPTH 512

namespace Ogre {
    /** When thrown, provides information about an error that has occurred inside the engine.
        @remarks
            OGRE never uses return values to indicate errors. Instead, if an
            error occurs, an exception is thrown, and this is the object that
            encapsulates the detail of the problem. The application using
            OGRE should always ensure that the exceptions are caught, so all
            OGRE engine functions should occur within a
            try{} catch(Ogre::Exception& e) {} block.
        @par
            The user application should never create any instances of this
            object unless it wishes to unify its error handling using the
            same object.
    */
    class _OgreExport Exception
        : public std::exception
    {
    protected:
        long line;
        int number;
        String description;
        String source;
        String file;
        ushort stackDepth;
        static Exception* last;

        static String msFunctionStack[ OGRE_CALL_STACK_DEPTH ];
        static ushort   msStackDepth;
    public:
        /** Static definitions of error codes.
            @todo
                Add many more exception codes, since we want the user to be able
                to catch most of them.
        */
        enum ExceptionCodes {
            UNIMPLEMENTED_FEATURE,
            ERR_CANNOT_WRITE_TO_FILE,
            ERR_NO_RENDERSYSTEM_SELECTED,
            ERR_DIALOG_OPEN_ERROR,
            ERR_INVALIDPARAMS,
            ERR_RENDERINGAPI_ERROR,
            ERR_DUPLICATE_ITEM,
            ERR_ITEM_NOT_FOUND,
            ERR_FILE_NOT_FOUND,
            ERR_INTERNAL_ERROR,
            ERR_RT_ASSERTION_FAILED, 
			ERR_NOT_IMPLEMENTED
        };

        /** Default constructor.
        */
        Exception( int number, const String& description, const String& source );

        /** Advanced constructor.
        */
        Exception( int number, const String& description, const String& source, const char* file, long line );

        /** Copy constructor.
        */
        Exception(const Exception& rhs);

        /** Assignment operator.
        */
        Exception& operator = (const Exception& rhs);

        /** Destructor
        */
#if OGRE_COMPILER == OGRE_COMPILER_MSVC
        ~Exception();
#else
        ~Exception() throw();
#endif

        /** Support std::exception method
        */
        const char *what() const throw();

        /** Returns a string with the full description of this error.
            @remarks
                The description contains the error number, the description
                supplied by the thrower, what routine threw the exception,
                and will also supply extra platform-specific information
                where applicable. For example - in the case of a rendering
                library error, the description of the error will include both
                the place in which OGRE found the problem, and a text
                description from the 3D rendering library, if available.
        */
        String getFullDescription(void) const;

        /** Gets the error code.
        */
        int getNumber(void) const throw();

        /** Gets the source function.
        */
        const String &getSource() const { return source; }

        /** Gets source file name.
        */
        const String &getFile() const { return file; }

        /** Gets line number.
        */
        long getLine() const { return line; }

		/** Returns a string with only the 'description' field of this exception. Use 
			getFullDescriptionto get a full description of the error including line number,
			error number and what function threw the exception.
        */
		const String &getDescription(void) const { return description; }

        /** Retrieves a pointer to the last exception created.
        */
        static Exception* getLastException(void) throw();

        /** Pushes a function on the stack.
        */
        static void _pushFunction( const String& strFuncName ) throw();
        /** Pops a function from the stack.
        */
        static void _popFunction() throw();

        /** Construct and throw an Exception object
        */
        OGRE_DECL_NORETURN static void _throw(int num, const String& desc, const String& src, const char* file, long line);
        OGRE_DECL_NORETURN static void _throw(int num, const String& desc, const char* src, const char* file, long line);
        OGRE_DECL_NORETURN static void _throw(int num, const char* desc, const String& src, const char* file, long line);
        OGRE_DECL_NORETURN static void _throw(int num, const char* desc, const char* src, const char* file, long line);
    };

    /** Internal class. Objects will automatically push/pop the function name for unwinding. */
    class _OgreExport AutomaticGuardUnguard {
    public:
        AutomaticGuardUnguard(const String& funcName) throw()
        {
            Exception::_pushFunction(funcName);
        }
        ~AutomaticGuardUnguard() throw()
        {
            Exception::_popFunction();
        }
    };

} // Namespace Ogre
#endif
