
#ifndef _CEGUIAnimate_h_
#define _CEGUIAnimate_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUIRect.h"
#include <vector>
#include "ceguicolour.h"
#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4275)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
	class CEGUIEXPORT Animate
	{
		friend class Animate_xmlHandler;
		friend class AnimateManager;

	public:
		/*!
		\brief
			Return the name of this animate.

		\return
			String object holding the name of this font.
		*/
		const String& getName(void) const {return	d_name;}

		/*!
		\brief
			Return the id of this animate.

		\return
			the id of this animate.
		*/
		int getID(void) const {return	d_id;}

		/*!
		\brief
			Define a new animate key for this animate.

		\param image
			Image point holding the image that will be assigned to the new Animate, which must be unique within the Imageset.
		*/
		void defineAnimateKey(const Image* image);

		/*!
		\brief
			Get the frame use species time and time total.

		\param time_now
			Specifies the time current.

		\param time_total
			Specifies the total time length.

		\return 
			The frame image that at the time. If the animate should stop or not start yet, return 0.
		*/
		const Image* getFrame(int time_elapsed, int time_total) const;

		/*!
		\brief
			Get the species frame.

		\param index
			Specifies the frame index.

		\return 
			The frame image that at the time. If the animate index not exist, return 0.
		*/
		const Image* getFrame(int index) const;

		/*!
		\brief
			Get the total frame number

		\return 
			The frame number.
		*/
		int getFrameNumber(void) const { return (int)d_animateList.size(); }

		/*!
		\brief
			Return a Size object containing the dimensions of the Animate.

		\return
			Size object holding the width and height of the Image.
		*/
		Size	getSize(void) const;

		float getFrameAlpha( int time_elapsed, int time_total );

	protected:
		/*************************************************************************
			Implementation Data
		*************************************************************************/
		struct AnimateKey
		{
			const Image* d_image;
		};
		typedef std::vector< AnimateKey >  AnimateKeyRegistry;

		String				d_name;			//!< Holds the name of this animate.
		int					d_id;			//!< Holds the id of this animate
		bool				d_loopMode;		//!< True when animate play in loop mode.
		int					d_loopType;		// 0-255, 0-255-0, 255-0	
		int					d_totalTime;
		AnimateKeyRegistry	d_animateList;	//!< All animate frame key.

		bool				d_bModeAlpha;  // 启用alpha混合模式
	private:
		/*************************************************************************
			Construction & Destruction
		*************************************************************************/
		/*!
		\brief
			Constructs a new Animate object

		\param name
			The unique name that will be used to identify this Animate.

		\param id
			Specifies the face motion id for the new Animate.

		\param loop
			Specifies the loop mode for the new animate.

		*/
		Animate(const String& name, int id, bool loop, int totaltime, bool alpha, int alphaType );

		/*!
		\brief
			Destroys a Animate object
		*/
		~Animate(void);

	private:
		/*************************************************************************
			Implementation Methods
		*************************************************************************/

	};

	//struct for runtime animate
	struct CEGUIEXPORT AnimateRuntime
	{
		const Animate*	d_animate;			//!< The animate.
		int				d_timeStart;		//!< The start system time(ms).
		int				d_timeNow;			//!< The current system time(ms).
		int				d_timeTotal;		//!< The total time that play whole frame(ms).
		Rect			d_destRect;			//!< Rect object defining the area on-screen where the Image is to be drawn.  The Image will be scaled to fit the area as required.
		Rect			d_clipRect;			//!< Rect object that defines an on-screen area that the Image will be clipped to when drawing.
		unsigned long	d_quadID;			//!< The id of render quad in render queue.
		colour			d_colours;
	};
}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif