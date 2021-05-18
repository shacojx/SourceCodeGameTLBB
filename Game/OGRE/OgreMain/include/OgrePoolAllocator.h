/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 The OGRE Team
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
#ifndef __PoolAllocator_H__
#define __PoolAllocator_H__

#include "OgrePrerequisites.h"

#ifndef OGRE_USE_POOL_ALLOCATOR
#define OGRE_USE_POOL_ALLOCATOR 1
#endif

#if OGRE_USE_POOL_ALLOCATOR

    #include "OgreNoMemoryMacros.h"

    #if OGRE_COMPILER == OGRE_COMPILER_MSVC
    #pragma warning (push)
    #pragma warning (disable : 4800)    // forcing value to bool 'true' or 'false' (performance warning)
    #endif

    #include <boost/pool/pool_alloc.hpp>

    #ifdef max
    #undef max
    #endif
    #ifdef min
    #undef min
    #endif

    #if OGRE_COMPILER == OGRE_COMPILER_MSVC
        #pragma warning (pop)
    #endif

    //---------------------------------------------------------------------
    #if defined(InterlockedExchange)

    # define BOOST_INTERLOCKED_EXCHANGE InterlockedExchange

    #elif defined( BOOST_MSVC ) || defined( BOOST_INTEL_WIN )

    extern "C" long __cdecl _InterlockedExchange( long volatile *, long );
    # pragma intrinsic( _InterlockedExchange )
    # define BOOST_INTERLOCKED_EXCHANGE _InterlockedExchange

    #endif
    //---------------------------------------------------------------------

    //---------------------------------------------------------------------
    // Based on boost::object_pool<> implementation, allows allocate/deallocate
    // in O(1) and sharing boost::singleton_pool<>.
    //
    // WARNING: Unlike boost::object_pool<>, even though allocated memory are
    // free automatically on program exit, but constructed objects never destruct
    // automatically, you always need to destruct them manually.
    //
    //---------------------------------------------------------------------

    // The following code will be put into Boost.Config in a later revision
    #if defined(BOOST_MSVC) || defined(__KCC)
    #   define BOOST_NO_TEMPLATE_CV_REF_OVERLOADS
    #endif

    // The following code might be put into some Boost.Config header in a later revision
    #ifdef __BORLANDC__
    #   pragma option push -w-inl
    #endif

    namespace boost {

        namespace details {
        namespace pool {

#if defined(BOOST_INTERLOCKED_EXCHANGE)
            class spin_mutex
            {
            private:
                spin_mutex(const spin_mutex &);
                void operator=(const spin_mutex &);

                long value_;

            public:
                spin_mutex() : value_(0) { }

                void lock()
                {
                    while (BOOST_INTERLOCKED_EXCHANGE(&value_, 1) != 0)
                        /* empty loop */;
                }

                void unlock()
                {
                    BOOST_INTERLOCKED_EXCHANGE(&value_, 0);
                }
            };

            typedef spin_mutex fast_mutex;
#else
            typedef default_mutex fast_mutex;
#endif

        } // namespace pool
        } // namespace details

        // Forward declaration
        template <typename T,
                  typename UserAllocator = default_user_allocator_new_delete,
                  typename Mutex = details::pool::fast_mutex>
        class fast_object_pool;

        // T must have a non-throwing destructor
        template <typename T, typename UserAllocator, typename Mutex>
        class fast_object_pool
        {
        public:
            typedef T element_type;
            typedef UserAllocator user_allocator;
            typedef typename fast_pool_allocator<T, UserAllocator, Mutex> allocator;
            typedef typename allocator::size_type size_type;
            typedef typename allocator::difference_type difference_type;

        public:
            // Constructor
            fast_object_pool() {}

            // Destructor
            ~fast_object_pool() {}

            // Allocate memory for the object but didn't construct it
            element_type * malloc()
            {
                return allocator::allocate();
            }

            // Deallocate memory of the object but didn't destruct it
            void free(element_type * const chunk)
            {
                allocator::deallocate(chunk);
            }

            // Allocate memory and construct default object
            element_type * construct()
            {
                element_type * const ret = malloc();
                if (ret == 0) return ret;
                try { new (ret) element_type(); }
                catch (...) { free(ret); throw; }
                return ret;
            }

            // Include automatically-generated file for family of template construct()
            //  functions
            #ifndef BOOST_NO_TEMPLATE_CV_REF_OVERLOADS
            #   include <boost/pool/detail/pool_construct.inc>
            #else
            #   include <boost/pool/detail/pool_construct_simple.inc>
            #endif

            // Destruct object and deallocate memory
            void destroy(element_type * const chunk)
            {
                chunk->~T();
                free(chunk);
            }
        };

    } // namespace boost

    // The following code might be put into some Boost.Config header in a later revision
    #ifdef __BORLANDC__
    #   pragma option pop
    #endif

    //---------------------------------------------------------------------
    //---------------------------------------------------------------------

    #include "OgreMemoryMacros.h"

    #define OgrePoolAllocator                                               boost::pool_allocator
    #define OgreFastPoolAllocator                                           boost::fast_pool_allocator
    #define OGRE_DECLARE_LOCAL_OBJECT_POOL_FRIENDLY(object_type)            \
        friend boost::fast_object_pool<object_type, boost::default_user_allocator_new_delete, boost::details::pool::null_mutex>
    #define OGRE_DECLARE_LOCAL_OBJECT_POOL(storage, pool_name, object_type) \
        storage boost::fast_object_pool<object_type, boost::default_user_allocator_new_delete, boost::details::pool::null_mutex> pool_name
    #define OGRE_DECLARE_OBJECT_POOL_FRIENDLY(object_type)                  friend boost::fast_object_pool<object_type>
    #define OGRE_DECLARE_OBJECT_POOL(storage, pool_name, object_type)       storage boost::fast_object_pool<object_type> pool_name
    #define OGRE_POOL_OBJECT_NEW(pool_name, object_type, params)            (pool_name).construct params
    #define OGRE_POOL_OBJECT_DELETE(pool_name, object)                      (pool_name).destroy (object)

#else
    #define OgrePoolAllocator                                               std::allocator
    #define OgreFastPoolAllocator                                           std::allocator
    #define OGRE_DECLARE_LOCAL_OBJECT_POOL_FRIENDLY(object_type)            /* noop */
    #define OGRE_DECLARE_LOCAL_OBJECT_POOL(storage, pool_name, object_type) /* noop */
    #define OGRE_DECLARE_OBJECT_POOL_FRIENDLY(object_type)                  /* noop */
    #define OGRE_DECLARE_OBJECT_POOL(storage, pool_name, object_type)       /* noop */
    #define OGRE_POOL_OBJECT_NEW(pool_name, object_type, params)            new object_type params
    #define OGRE_POOL_OBJECT_DELETE(pool_name, object)                      delete (object)

#endif

#endif  // __PoolAllocator_H__
