
#include "CEGUIAnimatemanager.h"
#include "CEGUIExceptions.h"
#include "CEGUISystem.h"
#include "CEGUILogger.h"
#include "CEGUIXMLAttributes.h"
#include "CEGUIXmlParser.h"
#include "CEGUIAnimate_xmlHandler.h"
#include "CEGUIImageset.h"
#include "CEGUIImage.h"
#include "CEGUITexture.h"

namespace CEGUI
{
	template<> AnimateManager* Singleton<AnimateManager>::ms_Singleton	= NULL;
	AnimateManager::AnimateManager(const String& animateDefFilename)
	{
		if (animateDefFilename.empty() || (animateDefFilename == (utf8*)""))
		{
			throw InvalidRequestException((utf8*)"AnimateManager::AnimateManager - Filename supplied for Animate loading must be valid");
		}

		Animate_xmlHandler handler;
		System::getSingleton().getXMLParser()->parseXMLFile(handler, animateDefFilename, "", "");
	}

	AnimateManager::~AnimateManager(void)
	{
		destroyAllAnimates();
	}

	AnimateManager& AnimateManager::getSingleton(void)
	{
		return Singleton<AnimateManager>::getSingleton();
	}

	AnimateManager* AnimateManager::getSingletonPtr(void)
	{
		return Singleton<AnimateManager>::getSingletonPtr();
	}

	/*************************************************************************
		Create a new animate.
	*************************************************************************/
	Animate* AnimateManager::createAnimate(const String& name, int id, bool loop, int totaltime, bool alpha, int loopType )
	{
		// first ensure name uniqueness
		if (isAnimatePresent(name))
		{
			throw AlreadyExistsException((utf8*)"AnimateManager::createAnimate - A animate named '" + name + "' already exists.");
		}
		
		Animate* newAnimate = new Animate(name, id, loop, totaltime, alpha, loopType );

		d_animateNameMap[name] = newAnimate;

		//Specifies a id
		if(/*id >= 0 &&*/ d_animateIDMap.find(id) == d_animateIDMap.end())
		{
			d_animateIDMap[id] = newAnimate;
		}

		return newAnimate;
	}

	/*************************************************************************
		Check to see if a animate is available.
	*************************************************************************/
	bool AnimateManager::isAnimatePresent(const String& name) const
	{
		return (d_animateNameMap.find(name) != d_animateNameMap.end());
	}

	bool AnimateManager::isAnimatePresent(int id) const
	{
		return (d_animateIDMap.find(id) != d_animateIDMap.end());
	}

	/*************************************************************************
		Returns a pointer to the animate object with the specified name.
	*************************************************************************/
	const Animate* AnimateManager::getAnimate(const String& name) const
	{
		AnimateNameRegistry::const_iterator pos = d_animateNameMap.find(name);
		if (pos == d_animateNameMap.end())
		{
			throw UnknownObjectException("Aniamtemanager::getAniamte- No Animatenamed '" + name + "' is present in the system.");
		}

		return pos->second;
	}

	const Animate* AnimateManager::getAnimate(int id) const
	{
		AnimateIDRegistry::const_iterator pos = d_animateIDMap.find(id);
		if (pos == d_animateIDMap.end())
		{
			char szTemp[256];
			_snprintf(szTemp, 255, 
				"Aniamtemanager::getAniamte- No Animate's id '%d' is present in the system.", id);
			throw UnknownObjectException(szTemp);
		}

		return pos->second;
	}

	/*************************************************************************
		Destroys the Animate with the specified name
	*************************************************************************/
	void AnimateManager::destroyAnimate(const String& name)
	{
		AnimateNameRegistry::iterator pos = d_animateNameMap.find(name);

		if (pos != d_animateNameMap.end())
		{
			//remove from id map first
			if(pos->second->getID() > 0)
			{
				d_animateIDMap.erase(pos->second->getID());
			}

			delete pos->second;
			d_animateNameMap.erase(pos);
		}
	}

	/*************************************************************************
		Destroys the given Animate object
	*************************************************************************/
	void AnimateManager::destroyAnimate(Animate* animate)
	{
		if (animate != NULL)
		{
			destroyAnimate(animate->getName());
		}
	}

	/*************************************************************************
		Destroys all Animate objects registered in the system
	*************************************************************************/
	void AnimateManager::destroyAllAnimates(void)
	{
		while (!d_animateNameMap.empty())
		{
			destroyAnimate(d_animateNameMap.begin()->first);
		}
	}

	/*************************************************************************
		Clears all runtime animate.
	*************************************************************************/
	void AnimateManager::clearRuntimeAnimateList(void)
	{
		d_runtimeAnimate.clear();
	}

	/*************************************************************************
		Queue the animate to be drawn and create a runtime animate in runtime animate list. 
	*************************************************************************/
	void AnimateManager::draw(const Animate* animate, int time_start, int time_now, int time_total, const Vector3& position, const Size& size, const Rect& clip_rect, const colour& colours)
	{
		if(!animate)
		{
			throw NullObjectException((utf8*)"AnimateManager::draw - Animate pointer must not be null.");
		}

		const Image* image_current = animate->getFrame(time_now-time_start, time_total);

		//already stop.
		if(!image_current) return;

		unsigned long idQuad = image_current->draw(position, size, clip_rect, ColourRect(colours, colours, colours, colours));

		AnimateRuntime animateRuntime;
		animateRuntime.d_animate = animate;
		animateRuntime.d_timeStart = time_start;
		animateRuntime.d_timeNow = time_now;
		animateRuntime.d_timeTotal = time_total;
		animateRuntime.d_quadID = idQuad;
		animateRuntime.d_destRect = Rect(position.d_x, position.d_y, position.d_x + size.d_width, position.d_y + size.d_height);
		animateRuntime.d_clipRect = clip_rect;
		animateRuntime.d_colours = colours;


		d_runtimeAnimate.push_back(animateRuntime);
	}

	/*************************************************************************
		Cause all runtime animate to update itself.
	*************************************************************************/
	void AnimateManager::updateRuntimeAnimateList(void)
	{
		AnimateRuntimeRegistry::iterator it;
		for(it=d_runtimeAnimate.begin(); it!=d_runtimeAnimate.end(); it++)
		{
			AnimateRuntime& animate = *it;

			//setp time
			animate.d_timeNow = (int)(System::getSingleton().getCurTimeElapsed()*1000);

			const Image* image_current = animate.d_animate->getFrame(animate.d_timeNow-animate.d_timeStart, animate.d_timeTotal);
			if(image_current)
			{
				Size sz = image_current->getSize();
				Point pt = image_current->getOffsets();

				Rect	source_rect = image_current->getSourceTextureArea();
				Rect&	dest_rect = animate.d_destRect;
				Rect&	clip_rect = animate.d_clipRect;

				// get the rect area that we will actually draw to (i.e. perform clipping)
				Rect final_rect(dest_rect.getIntersection(clip_rect));

				Texture* pTex = image_current->getImageset()->getTexture();

				float x_scale = 1.0f / (float)pTex->getWidth();
				float y_scale = 1.0f / (float)pTex->getHeight();

				float tex_per_pix_x = source_rect.getWidth() / dest_rect.getWidth();
				float tex_per_pix_y = source_rect.getHeight() / dest_rect.getHeight();

				// calculate final, clipped, texture co-ordinates
				Rect  tex_rect((source_rect.d_left + ((final_rect.d_left - dest_rect.d_left) * tex_per_pix_x)) * x_scale,
					(source_rect.d_top + ((final_rect.d_top - dest_rect.d_top) * tex_per_pix_y)) * y_scale,
					(source_rect.d_right + ((final_rect.d_right - dest_rect.d_right) * tex_per_pix_x)) * x_scale,
					(source_rect.d_bottom + ((final_rect.d_bottom - dest_rect.d_bottom) * tex_per_pix_y)) * y_scale);
				
				System::getSingleton().getRenderer()->adjustQuad(
					animate.d_quadID, image_current->getImageset()->getTexture(), tex_rect, animate.d_colours );
			}
		}
	}

	/*************************************************************************
		Add a imageset to animatemanager.
	*************************************************************************/
	void AnimateManager::addAnimateImageset(const String& name)
	{
		d_animateImageset.insert(name);
	}

	/*************************************************************************
		Checks is a imageset in animatemanager.
	*************************************************************************/
	bool AnimateManager::isImagesetInAnimate(const String& name) const
	{
		AnimateImagesetRegistry::const_iterator it;
		it = d_animateImageset.find(name);
		return (it != d_animateImageset.end());
	}

}