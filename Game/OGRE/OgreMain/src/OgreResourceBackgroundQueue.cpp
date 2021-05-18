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
#include "OgreResourceBackgroundQueue.h"
#include "OgreLogManager.h"
#include "OgreException.h"
#include "OgreResourceGroupManager.h"
#include "OgreResourceManager.h"

namespace Ogre {

	//------------------------------------------------------------------------
    //-----------------------------------------------------------------------
    template<> ResourceBackgroundQueue* Singleton<ResourceBackgroundQueue>::ms_Singleton = 0;
    ResourceBackgroundQueue* ResourceBackgroundQueue::getSingletonPtr(void)
    {
        return ms_Singleton;
    }
    ResourceBackgroundQueue& ResourceBackgroundQueue::getSingleton(void)
    {  
        assert( ms_Singleton );  return ( *ms_Singleton );  
    }
    //-----------------------------------------------------------------------	
	//------------------------------------------------------------------------
	ResourceBackgroundQueue::ResourceBackgroundQueue()
		:mNextTicketID(0), mThread(0)
	{
	}
	//------------------------------------------------------------------------
	ResourceBackgroundQueue::~ResourceBackgroundQueue()
	{
		shutdown();
	}
	//------------------------------------------------------------------------
	void ResourceBackgroundQueue::initialise(void)
	{
#if OGRE_THREAD_SUPPORT
		OGRE_LOCK_AUTO_MUTEX
		mThread = new boost::thread(
			boost::function0<void>(&ResourceBackgroundQueue::threadFunc));
		LogManager::getSingleton().logMessage(
			"ResourceBackgroundQueue - threading enabled");
#else
		LogManager::getSingleton().logMessage(
			"ResourceBackgroundQueue - threading disabled");	
#endif
	}
	//------------------------------------------------------------------------
	void ResourceBackgroundQueue::shutdown(void)
	{
#if OGRE_THREAD_SUPPORT
		if (mThread)
		{
			// Put a shutdown request on the queue
			Request req;
			req.type = RT_SHUTDOWN;
			addRequest(req);
			// Wait for thread to finish
			mThread->join();
			delete mThread;
			mThread = 0;
			mRequestQueue.clear();
			mRequestTicketMap.clear();
		}
#endif
	}
	//------------------------------------------------------------------------
	BackgroundProcessTicket ResourceBackgroundQueue::initialiseResourceGroup(
		const String& name, ResourceBackgroundQueueListener* listener)
	{
#if OGRE_THREAD_SUPPORT
		if (!mThread)
		{
			OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, 
				"Thread not initialised",
				"ResourceBackgroundQueue::initialiseResourceGroup");
		}
		// queue a request
		Request req;
		req.type = RT_INITIALISE_GROUP;
		req.groupName = name;
		req.listener = listener;
		return addRequest(req);
#else
		// synchronous
		ResourceGroupManager::getSingleton().initialiseResourceGroup(name);
		return 0; 
#endif
	}
	//------------------------------------------------------------------------
	BackgroundProcessTicket 
	ResourceBackgroundQueue::initialiseAllResourceGroups( 
		ResourceBackgroundQueueListener* listener)
	{
#if OGRE_THREAD_SUPPORT
		if (!mThread)
		{
			OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, 
				"Thread not initialised",
				"ResourceBackgroundQueue::initialiseAllResourceGroups");
		}
		// queue a request
		Request req;
		req.type = RT_INITIALISE_ALL_GROUPS;
		req.listener = listener;
		return addRequest(req);
#else
		// synchronous
		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		return 0; 
#endif
	}
	//------------------------------------------------------------------------
	BackgroundProcessTicket ResourceBackgroundQueue::loadResourceGroup(
		const String& name, ResourceBackgroundQueueListener* listener)
	{
#if OGRE_THREAD_SUPPORT
		if (!mThread)
		{
			OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, 
				"Thread not initialised",
				"ResourceBackgroundQueue::loadResourceGroup");
		}
		// queue a request
		Request req;
		req.type = RT_LOAD_GROUP;
		req.groupName = name;
		req.listener = listener;
		return addRequest(req);
#else
		// synchronous
		ResourceGroupManager::getSingleton().loadResourceGroup(name);
		return 0; 
#endif
	}
	//------------------------------------------------------------------------
	BackgroundProcessTicket ResourceBackgroundQueue::load(
		const String& resType, const String& name, 
		const String& group, bool isManual, 
		ManualResourceLoader* loader, 
		const NameValuePairList* loadParams, 
		ResourceBackgroundQueueListener* listener)
	{
#if OGRE_THREAD_SUPPORT
		if (!mThread)
		{
			OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, 
				"Thread not initialised",
				"ResourceBackgroundQueue::load");
		}
		// queue a request
		Request req;
		req.type = RT_LOAD_RESOURCE;
		req.resourceType = resType;
		req.resourceName = name;
		req.groupName = group;
		req.isManual = isManual;
		req.loader = loader;
		req.loadParams = loadParams;
		req.listener = listener;
		return addRequest(req);
#else
		// synchronous
		ResourceManager* rm = 
			ResourceGroupManager::getSingleton()._getResourceManager(resType);
		rm->load(name, group, isManual, loader, loadParams);
		return 0; 
#endif
	}
	//------------------------------------------------------------------------
	bool ResourceBackgroundQueue::isProcessComplete(
			BackgroundProcessTicket ticket)
	{
		return mRequestTicketMap.find(ticket) == mRequestTicketMap.end();
	}
	//------------------------------------------------------------------------
#if OGRE_THREAD_SUPPORT
	BackgroundProcessTicket ResourceBackgroundQueue::addRequest(Request& req)
	{
		// Lock
		OGRE_LOCK_AUTO_MUTEX

		req.ticketID = ++mNextTicketID;
		mRequestQueue.push_back(req);
		Request* requestInList = &(mRequestQueue.back());
		mRequestTicketMap[req.ticketID] = requestInList;

		// Notify to wake up loading thread
		mCondition.notify_one();

		return req.ticketID;
	}
	//------------------------------------------------------------------------
	void ResourceBackgroundQueue::threadFunc(void)
	{
		// Background thread implementation 
		// Static (since no params allowed), so get instance
		ResourceBackgroundQueue& queueInstance = 
			ResourceBackgroundQueue::getSingleton();

		bool shuttingDown = false;
		// Spin forever until we're told to shut down
		while (!shuttingDown)
		{
			Request* req;
			// Manual scope block just to define scope of lock
			{
				// Lock; note that 'mCondition.wait()' will free the lock
				boost::recursive_mutex::scoped_lock queueLock(
					queueInstance.OGRE_AUTO_MUTEX_NAME);
				if (queueInstance.mRequestQueue.empty())
				{
					// frees lock and suspends the thread
					queueInstance.mCondition.wait(queueLock);
				}
				// When we get back here, it's because we've been notified 
				// and thus the thread as been woken up. Lock has also been
				// re-acquired.

				// Process one request
				req = &(queueInstance.mRequestQueue.front());
			} // release lock so queueing can be done while we process one request
			// use of std::list means that references guarateed to remain valid

			ResourceManager* rm = 0;
			switch (req->type)
			{
			case RT_INITIALISE_GROUP:
				ResourceGroupManager::getSingleton().initialiseResourceGroup(
					req->groupName);
				break;
			case RT_INITIALISE_ALL_GROUPS:
				ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
				break;
			case RT_LOAD_GROUP:
				ResourceGroupManager::getSingleton().loadResourceGroup(
					req->groupName);
				break;
			case RT_LOAD_RESOURCE:
				rm = ResourceGroupManager::getSingleton()._getResourceManager(
						req->resourceType);
				rm->load(req->resourceName, req->groupName, req->isManual, 
					req->loader, req->loadParams);
				break;
			case RT_SHUTDOWN:
				// That's all folks
				shuttingDown = true;
				break;
			};


			{
				// re-lock to consume completed request
				boost::recursive_mutex::scoped_lock queueLock(
					queueInstance.OGRE_AUTO_MUTEX_NAME);

				// Consume the ticket
				queueInstance.mRequestTicketMap.erase(req->ticketID);
				queueInstance.mRequestQueue.pop_front();
			}


		}

	
		
	}
#endif
	//------------------------------------------------------------------------

}



