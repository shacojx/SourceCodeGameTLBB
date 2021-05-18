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
#ifndef __ResourceBackgroundQueue_H__
#define __ResourceBackgroundQueue_H__


#include "OgrePrerequisites.h"
#include "OgreCommon.h"
#include "OgreSingleton.h"

#if OGRE_THREAD_SUPPORT
#	include <boost/thread/thread.hpp>
#	include <boost/thread/condition.hpp>
#endif

namespace Ogre {

	/// Identifier of a background process
	typedef unsigned long BackgroundProcessTicket;

	/** This abstract listener interface lets you get immediate notifications of
		completed background processes instead of having to check ticket 
		statuses.
	@note
		These callbacks occur in the <i>background thread</i>, not the thread
		which you queued your request from. You should only use this method
		if you understand the implications of threading and the use of locks, 
		monitor objects or other such thread safety techniques. If you don't, 
		use the simpler 'ticket' approach and poll the isProcessComplete() 
		method
		to determine when your processes complete.
	*/
	class _OgreExport ResourceBackgroundQueueListener
	{
	public:
		/** Called when a requested operation completes. 
		@note Called in the <i>background thread</i>, not your queueing
		thread, so be careful!
		*/
		virtual void operationCompleted(BackgroundProcessTicket ticket) = 0;
	};
	
	/** This class is used to perform Resource operations in a
		background thread. 
	@remarks
		If threading is enabled, Ogre will create a single background thread
		which can be used to load / unload resources in parallel. Only one
		resource will be processed at once in this background thread, but it
		will be in parallel with the main thread. 
	@par
		The general approach here is that on requesting a background resource
		process, your request is placed on a queue ready for the background
		thread to be picked up, and you will get a 'ticket' back, identifying
		the request. Your call will then return and your thread can
		proceed, knowing that at some point in the background the operation wil 
		be performed. In it's own thread, the resource operation will be 
		performed, and once finished the ticket will be marked as complete. 
		You can check the status of tickets by calling isProcessComplete() 
		from your queueing thread. It is also possible to get immediate 
		callbacks on completion, but these callbacks happen in the background 
		loading thread (not your calling thread), so should only be used if you
		really understand multithreading. 
	@note
		This class will only perform tasks in a background thread if 
		OGRE_THREAD_SUPPORT is defined to be 1. Otherwise, all methods will
		call their exact equivalents in ResourceGroupManager synchronously. 
	*/
	class _OgreExport ResourceBackgroundQueue : public Singleton<ResourceBackgroundQueue>
	{
	protected:
		/** Enumerates the type of requests */
		enum RequestType
		{
			RT_INITIALISE_GROUP,
			RT_INITIALISE_ALL_GROUPS,
			RT_LOAD_GROUP,
			RT_LOAD_RESOURCE,
			RT_SHUTDOWN
		};
		/** Encapsulates a queued request for the background queue */
		struct Request
		{
			BackgroundProcessTicket ticketID;
			RequestType type;
			String resourceName;
			String resourceType;
			String groupName;
			bool isManual; 
			ManualResourceLoader* loader;
			const NameValuePairList* loadParams;
			ResourceBackgroundQueueListener* listener;
		};
		typedef std::list<Request> RequestQueue;
		typedef std::map<BackgroundProcessTicket, Request*> RequestTicketMap;
		
		/// Queue of requests, used to store and order requests
		RequestQueue mRequestQueue;
		
		/// Request lookup by ticket
		RequestTicketMap mRequestTicketMap;

		/// Next ticket ID
		unsigned long mNextTicketID;

#if OGRE_THREAD_SUPPORT
		/// The single background thread which will process loading requests
		boost::thread* mThread;
		/// Synchroniser token to wait / notify on queue
		boost::condition mCondition;
		/// Thread function
		static void threadFunc(void);
		/// Internal method for adding a request; also assigns a ticketID
		BackgroundProcessTicket addRequest(Request& req);
#else
		/// Dummy
		void* mThread;
#endif

		/// Private mutex, not allowed to lock from outside
		OGRE_AUTO_MUTEX

	public:
		ResourceBackgroundQueue();
		virtual ~ResourceBackgroundQueue();

		/** Initialise the background queue system. */
		virtual void initialise(void);
		
		/** Shut down the background queue system. */
		virtual void shutdown(void);

		/** Initialise a resource group in the background.
		@see ResourceGroupManager::initialiseResourceGroup
		@param name The name of the resource group to initialise
		@param listener Optional callback interface, take note of warnings in 
			the header and only use if you understand them.
		@returns Ticket identifying the request, use isProcessComplete() to 
			determine if completed if not using listener
		*/
		virtual BackgroundProcessTicket initialiseResourceGroup(
			const String& name, ResourceBackgroundQueueListener* listener = 0);

		/** Initialise all resource groups which are yet to be initialised in 
			the background.
		@see ResourceGroupManager::intialiseResourceGroup
		@param listener Optional callback interface, take note of warnings in 
			the header and only use if you understand them.
		@returns Ticket identifying the request, use isProcessComplete() to 
			determine if completed if not using listener
		*/
		virtual BackgroundProcessTicket initialiseAllResourceGroups( 
			ResourceBackgroundQueueListener* listener = 0);
		/** Loads a resource group in the background.
		@see ResourceGroupManager::intialiseResourceGroup
		@param listener Optional callback interface, take note of warnings in 
			the header and only use if you understand them.
		@returns Ticket identifying the request, use isProcessComplete() to 
			determine if completed if not using listener
		*/
		virtual BackgroundProcessTicket loadResourceGroup(const String& name, 
			ResourceBackgroundQueueListener* listener = 0);


		/** Load a single resource in the background. 
		@see ResourceManager::load
		@param resType The type of the resource 
			(from ResourceManager::getResourceType())
		@param name The name of the Resource
		@param group The resource group to which this resource will belong
		@param isManual Is the resource to be manually loaded? If so, you should
			provide a value for the loader parameter
		@param loader The manual loader which is to perform the required actions
			when this resource is loaded; only applicable when you specify true
			for the previous parameter. NOTE: must be thread safe!!
        @param loadParams Optional pointer to a list of name/value pairs 
            containing loading parameters for this type of resource. Remember 
			that this must have a lifespan longer than the return of this call!
		*/
		virtual BackgroundProcessTicket load(
			const String& resType, const String& name, 
            const String& group, bool isManual = false, 
			ManualResourceLoader* loader = 0, 
			const NameValuePairList* loadParams = 0, 
			ResourceBackgroundQueueListener* listener = 0);

		/** Returns whether a previously queued process has completed or not. 
		@param ticket The ticket which was returned when the process was queued
		@returns true if process has completed (or if the ticket is 
			unrecognised), false otherwise
		@note Tickets are not stored onced complete so do not accumulate over 
			time.
		This is why a non-existent ticket will return 'true'.
		*/
		virtual bool isProcessComplete(BackgroundProcessTicket ticket);

		/** Override standard Singleton retrieval.
        @remarks
        Why do we do this? Well, it's because the Singleton
        implementation is in a .h file, which means it gets compiled
        into anybody who includes it. This is needed for the
        Singleton template to work, but we actually only want it
        compiled into the implementation of the class based on the
        Singleton, not all of them. If we don't change this, we get
        link errors when trying to use the Singleton-based class from
        an outside dll.
        @par
        This method just delegates to the template version anyway,
        but the implementation stays in this single compilation unit,
        preventing link errors.
        */
        static ResourceBackgroundQueue& getSingleton(void);
        /** Override standard Singleton retrieval.
        @remarks
        Why do we do this? Well, it's because the Singleton
        implementation is in a .h file, which means it gets compiled
        into anybody who includes it. This is needed for the
        Singleton template to work, but we actually only want it
        compiled into the implementation of the class based on the
        Singleton, not all of them. If we don't change this, we get
        link errors when trying to use the Singleton-based class from
        an outside dll.
        @par
        This method just delegates to the template version anyway,
        but the implementation stays in this single compilation unit,
        preventing link errors.
        */
        static ResourceBackgroundQueue* getSingletonPtr(void);
		

	};


}

#endif

