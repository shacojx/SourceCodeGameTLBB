

#include "CEGUIAnimate.h"
#include "CEGUIImageset.h"
#include "CEGUIExceptions.h"
#include "AxTrace.h"
#include "windows.h"
#include "math.h"
// Start of CEGUI namespace section
namespace CEGUI
{
	/*************************************************************************
		Constructs a new Animate object.
	*************************************************************************/
	Animate::Animate(const String& name, int id, bool loop, int totaltime, bool alpha, int loopType) :
		d_name(name),
		d_id(id),
		d_loopMode(loop),
		d_totalTime( totaltime ),
		d_bModeAlpha( alpha ),
		d_loopType( loopType )

	{
	}

	/*************************************************************************
		Destroys a Animate object
	*************************************************************************/
	Animate::~Animate(void)
	{
	}

	/*************************************************************************
		Define a new animate key for this animate.
	*************************************************************************/
	void Animate::defineAnimateKey(const Image* image)
	{
		if(!image)
		{
			throw NullObjectException((utf8*)"Animate::defineAnimateKey - Image pointer must not be null.");
		}

		AnimateKey newKey;
		newKey.d_image = image;

		d_animateList.push_back(newKey);
	}

	/*************************************************************************
		Get the frame use species time and time total.
	*************************************************************************/
	const Image* Animate::getFrame(int time_elapsed, int time_total) const
	{
		if( time_total == -1 )
			time_total = d_totalTime;
		if(time_elapsed < 0) return 0;

		//play not in loop mode.
		if(!d_loopMode && time_elapsed >= time_total) return 0;

		float fFrameScale = (float)(time_elapsed % time_total)/(float)(time_total);
		int nFrameIndex = (int)(fFrameScale*(float)d_animateList.size());

		return d_animateList[nFrameIndex].d_image;
	}
	float Animate::getFrameAlpha( int time_elapsed, int time_total )
	{
		if( time_total == -1 ) 
			time_total = d_totalTime;
		if( d_bModeAlpha == false )
			return 1;
		int nTime = time_elapsed;
		if( d_loopMode )
			nTime = time_elapsed % time_total;
		
		float alpha	= float( nTime ) / float( time_total );
		switch( d_loopType )
		{
		case 0: // 0 -- 255
			break;
		case 1: // 0 -- 255 -- 0
			{
				if( alpha < 0.5 )
					alpha *= 2;
				else
					alpha = ( 1.0f - alpha ) * 2;
			}
			break;
		case 2: // 255 -- 0
			alpha = 1 - alpha;
			break;
		}
		return alpha;

	}
	const Image* Animate::getFrame(int index) const
	{
		if(index < 0 || index >= (int)d_animateList.size()) return 0;

		return d_animateList[index].d_image;
	}

	/*************************************************************************
		Return a Size object containing the dimensions of the Animate.
	*************************************************************************/
	Size Animate::getSize(void) const
	{
		if(d_animateList.empty()) return Size(0.0f, 0.0f);

		return d_animateList[0].d_image->getSize();
	}

}