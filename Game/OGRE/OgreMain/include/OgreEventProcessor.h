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
OgreEventProcessor.h  -  
	The EventProcessor controls getting events, storing them in a queue, and 
	dispatching events.
	It contains an InputDevice which creates InputEvents. The events are then
	stored FIFO in the EventQueue. 

	The EventProcessor is a frame listener, so each frame, it empties the entire
	queue to the list of dispatchers.

	Each dispatcher corresponds to a registered TargetManager.

	The TargetManagers need to be registered with the Processor before initialise is called.

	After intialise is called, the Processor will start processing events once startProcessingEvents is called.

	The Processor acts like a default EventTarget, so it can process events that no dispatcher consumes.
	You can listen default actions to the processor by e.g.
		mProcessor->addMouseListener(defaultMouseMovement);

	WARNING: The event objects are created in InputReader when they are submitted to the queue
	, yet destroyed in the Event Processor when they are taken off the queue.
	Deleting objects in different locations to where they are created is usually undesirable, however,
	since the Processor, queue and InputReader are tightly coupled (only the Processor is visible to the outside)
	this can be an exception.
	The reason for this is for performance... The alternative, is to do 2 more event copies when sending events to the queue,
	so the queue manages creating (copying the event from the inputReader) and deleting objects once popped - however
	this would require the events to be copied twice.. So I have avoided this by having the same event object passed around
	and not copied.
-------------------
begin                : Nov 19 2002
copyright            : (C) 2002 by Kenny Sabir
email                : kenny@sparksuit.com
***************************************************************************/

#ifndef __EventProcessor_H__
#define __EventProcessor_H__

#include <list>
#include "OgrePrerequisites.h"
#include "OgreSingleton.h"
#include "OgreFrameListener.h"
#include "OgreMouseTarget.h"
#include "OgreKeyTarget.h"
#include "OgreMouseMotionTarget.h"

namespace Ogre {
	/** The EventProcessor controls getting events, storing them in a queue, and 
		dispatching events.
	@remarks
		An application can create an instance of this class to receive buffered input, as opposed
		to creating an InputReader directly and retrieving snapshot state only. This class
		contains an InputReader which it uses to create InputEvents which are then
		stored FIFO in the EventQueue. 
	@par
		The EventProcessor is a frame listener, so each frame, it empties the entire
		queue to the list of dispatchers. Each dispatcher corresponds to a registered TargetManager.
		The TargetManagers need to be registered with the Processor before initialise is called.
		After intialise is called, the Processor will start processing events once 
		startProcessingEvents is called.
	@par
		The Processor acts like a default EventTarget, so it can process events that no dispatcher consumes.
		You can listen default actions to the processor by e.g.
			mProcessor->addMouseListener(defaultMouseMovement);
	*/
    class _OgreExport EventProcessor : public FrameListener, public MouseTarget, public MouseMotionTarget, public Singleton<EventProcessor>, public KeyTarget
    {
    protected:
		EventQueue* mEventQueue;
		/**
		 * empty queue and cleanup objects 
		 *
		 */
		void cleanup();
		InputReader* mInputDevice;
		typedef std::list<EventDispatcher*> DispatcherList;
		typedef std::list<EventTarget*> EventTargetList;
		DispatcherList mDispatcherList;
        EventTargetList mEventTargetList;
        bool mRegisteredAsFrameListener;

    public:
        EventProcessor();
        virtual ~EventProcessor();

		/**
		 * Registers FrameListener, and activates the queue
		 */
		void startProcessingEvents(bool registerListener=true);

		/**
		 * Removes this from being a FrameListener,
		 * and deactivates the queue
		 */
		void stopProcessingEvents();

		/**
		 * Creates the Queue object,
		 * Creates the InputReader object
		 * initialises the InputReader to use buffered input 
		 */
		void initialise(RenderWindow* ren);

		/**
		 * Processes default events,
		 * these are events are aren't handled by any dispatcher
		 */
		void processEvent(InputEvent* e);

		/**
		 * Adds a mouse motion listener to the cursor object.
		 * This keeps the Cursor object hidden.
		 */
		void addCursorMoveListener(MouseMotionListener* c);

		/**
		 * Removes a mouse motion listener to the cursor object.
		 * This keeps the Cursor object hidden.
		 */
		void removeCursorMoveListener(MouseMotionListener* c);

		/**
		 * Creates a dispatcher object that dispatches to the targetManager.
		 * Adds the new dispatcher object to the dispatcher list.
		 */

		void addTargetManager(TargetManager* targetManager);

		/**
		 * Creates a dispatcher object that dispatches to the targetManager.
		 * Adds the new dispatcher object to the dispatcher list.
		 */

		void addEventTarget(EventTarget* eventTarget);

		/**
		 * Processes all events on the queue.
		 * sends each event to each dispatcher.
		 * deletes the event objects
		 */
		bool frameStarted(const FrameEvent& evt);

		// PositionTarget methods
		/**
		 * returns 0, since this is a default event target, default events have a top of 0 
		 */
		Real getTop() const;

		/**
		 * returns 0, since this is a default event target, default events have a left of 0 
		 */
		Real getLeft() const;

		/**
		 * returns NULL, There is no parent of the default event target.
		 */
		PositionTarget* getPositionTargetParent() const;



		bool isKeyEnabled() const
		{ return true; }

		inline InputReader* getInputReader()
		{ return mInputDevice; }
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
        static EventProcessor& getSingleton(void);
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
        static EventProcessor* getSingletonPtr(void);
    };



}


#endif //__EventProcessor_H__
