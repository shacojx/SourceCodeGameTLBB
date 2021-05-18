#ifndef _CEGUIAnimateManager_h_
#define _CEGUIAnimateManager_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUISingleton.h"
#include "CEGUIAnimate.h"

#include <vector>
#include <map>
#include <set>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4275)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
	class CEGUIEXPORT AnimateManager :	public Singleton< AnimateManager >
	{
	public:
		/*!
		\brief
			Constructor for FontManager objects
		*/
		AnimateManager(const String& name);

		/*!
		\brief
			Destructor for FontManager objects
		*/
		~AnimateManager(void);

		/*!
		\brief
			Return singleton FontManager object

		\return
			Singleton FontManager object
		*/
		static	AnimateManager&	getSingleton(void);

		/*!
		\brief
			Return pointer to singleton FontManager object

		\return
			Pointer to singleton FontManager object
		*/
		static	AnimateManager*	getSingletonPtr(void);

		/*!
		\brief
			Creates a new animate, and returns a pointer to the new Font object.

		\param name
			String object containing a unique name for the new animate.

		\param id
			Specifies the face motion id for the new animate.

		\param loop
			Specifies the loop mode for the new animate.

		\return
			Pointer to the newly created Animate object.

		\exception	AlreadyExistsException		thrown if a Animate already exists with the name specified.
		*/
		Animate* createAnimate(const String& name, int id, bool loop, int totaltime, bool bAlphaMode , int loopType );

		/*!
		\brief
			Destroys the Animate with the specified name

		\param name
			String object containing the name of the Animate to be destroyed.  If no such Animate exists, nothing happens.

		\return
			Nothing.
		*/
		void destroyAnimate(const String& name);

		/*!
		\brief
			Destroys the given Animate object

		\param animate
			Pointer to the Animate to be destroyed.  If no such Animate exists, nothing happens.

		\return
			Nothing.
		*/
		void destroyAnimate(Animate* animate);

		/*!
		\brief
			Destroys all Animate objects registered in the system

		\return
			Nothing
		*/
		void destroyAllAnimates(void);

		/*!
		\brief
			Add a imageset to animatemanager

		\param name
			String object holding the name of the imageset.

		\return
			None
		*/
		void addAnimateImageset(const String& name);

		/*!
		\brief
			Checks is a imageset in animatemanager

		\param name
			String object holding the name of the imageset.

		\return
			true if a Imageset object named \a name exists in the system, false if no such imageset exists.
		*/
		bool isImagesetInAnimate(const String& name) const;

		/*!
		\brief
			Checks the existence of a given animate.

		\param name
			String object holding the name of the Animate object to look for.

		\return
			true if a Animate object named \a name exists in the system, false if no such animate exists.
		*/
		bool isAnimatePresent(const String& name) const;

		/*!
		\brief
			Checks the existence of a given animate.

		\param id
			The id of the Animate object to look for.

		\return
			true if a Animate object's id exists in the system, false if no such animate exists.
		*/
		bool isAnimatePresent(int id) const;

		/*!
		\brief
			Returns a pointer to the animate object with the specified name.

		\param name
			String object containing the name of the Animate object to be returned

		\return
			Pointer to the requested Animate object

		\exception UnknownObjectException	Thrown if no animate with the given name exists.
		*/
		const Animate* getAnimate(const String& name) const;

		/*!
		\brief
			Returns a pointer to the animate object with the specified id.

		\param id
			The id of the Animate object to be returned

		\return
			Pointer to the requested Animate object

		\exception UnknownObjectException	Thrown if no animate with the given name exists.
		*/
		const Animate* getAnimate(int id) const;

		/*!
		\brief
			Queue the animate to be drawn and create a runtime animate in runtime animate list. 
			
		\param anme
			Specifies the name of the animate to be drawn.

		\param time_start
			Specifies the time start(ms).

		\param time_now
			Specifies the time now(ms).

		\param time_total
			Specifies the total time length(ms).

		\param position
			Vector3 object containing the location where the Image is to be drawn

		\param size
			Size object describing the size that the Image is to be drawn at.

		\param clip_rect
			Rect object that defines an on-screen area that the Image will be clipped to when drawing.

		\return
			Nothing
		*/
		void draw(const String& name, int time_start, int time_now, int time_total, const Vector3& position, const Size& size, const Rect& clip_rect, const colour& colours)
		{
			if(isAnimatePresent(name))
			{
				draw(getAnimate(name), time_start, time_now, time_total, position, size, clip_rect, colours);
			}
		}

		/*!
		\brief
			Queue the animate to be drawn and create a runtime animate in runtime animate list. 
			
		\param anme
			Specifies the name of the animate to be drawn.

		\param time_start
			Specifies the time start(ms).

		\param time_now
			Specifies the time now(ms).

		\param time_total
			Specifies the total time length(ms).

		\param position
			Vector3 object containing the location where the Image is to be drawn

		\param size
			Size object describing the size that the Image is to be drawn at.

		\param clip_rect
			Rect object that defines an on-screen area that the Image will be clipped to when drawing.

		\return
			Nothing
		*/
		void draw(int id, int time_start, int time_now, int time_total, const Vector3& position, const Size& size, const Rect& clip_rect, const colour& colours)
		{
			if(isAnimatePresent(id))
			{
				draw(getAnimate(id), time_start, time_now, time_total, position, size, clip_rect, colours);
			}
		}

		/*!
		\brief
			Queue the animate to be drawn and create a runtime animate in runtime animate list. 
			
		\param animate
			Specifies the animate to e drawn.

		\param time_now
			Specifies the time start.

		\param time_total
			Specifies the total time length.

		\param position
			Vector3 object containing the location where the Image is to be drawn

		\param size
			Size object describing the size that the Image is to be drawn at.

		\param clip_rect
			Rect object that defines an on-screen area that the Image will be clipped to when drawing.

		\return
			Nothing
		*/
		void draw(const Animate* animate, int time_start, int time_now, int time_total, const Vector3& position, const Size& size, const Rect& clip_rect, const colour& colours);

		/*!
		\brief
			Clears all runtime animate.
		
		\return
			Nothing
		*/
		void clearRuntimeAnimateList(void);

		/*!
		\brief 
			Cause all runtime animate in render queue to update itself.

		\return
			Nothing
		*/
		void updateRuntimeAnimateList(void);

	private:
		/*************************************************************************
			Implementation Data
		*************************************************************************/
		typedef std::map< String, Animate* > AnimateNameRegistry;
		//All animate
		AnimateNameRegistry d_animateNameMap;

		typedef std::map< int, Animate* > AnimateIDRegistry;
		//Animate use id as index.
		AnimateIDRegistry d_animateIDMap;

		//All runtime animate.
		typedef std::vector< AnimateRuntime > AnimateRuntimeRegistry;
		AnimateRuntimeRegistry d_runtimeAnimate;

		//All imageset that include animate
		typedef std::set< String > AnimateImagesetRegistry;
		AnimateImagesetRegistry d_animateImageset;
	};
};

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif