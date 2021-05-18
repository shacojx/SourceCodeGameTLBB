
#include "CEGUIAnimate_xmlhandler.h"

#include "CEGUIExceptions.h"
#include "CEGUISystem.h"
#include "CEGUILogger.h"
#include "CEGUIXMLAttributes.h"
#include "CEGUIAnimate.h"
#include "CEGUIAnimatemanager.h"
#include "CEGUIPropertyHelper.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	/*************************************************************************
	Definition of constant data for Animate (and sub-classes)
	*************************************************************************/
	// Declared in Animate::xmlHandler
	const String Animate_xmlHandler::AnimateManagerElement( (utf8*)"AnimateManager" );
	const String Animate_xmlHandler::AnimateElement( (utf8*)"Animate" );
	const String Animate_xmlHandler::AniamteItemElement( (utf8*)"Image" );

	const char	Animate_xmlHandler::AnimateNameAttribute[]			= "Name";
	const char	Animate_xmlHandler::AnimatePlayLoop[]				= "Loop";
	const char  Animate_xmlHandler::AnimateTime[]					= "PlayTime";
	const char  Animate_xmlHandler::AnimateID[]						= "FaceMotionID";
	const char  Animate_xmlHandler::AniamteImage[]					= "ImageName";
	const char  Animate_xmlHandler::AnimateAlphaMode[]				= "AlphaMode";
	const char  Animate_xmlHandler::AnimateAlphaType[]				= "AlphaType";

	/*************************************************************************
	SAX2 Handler methods
	*************************************************************************/
	void Animate_xmlHandler::elementStart(const String& element, const XMLAttributes& attributes)
	{
		// handle an Animate element (extract all element attributes and use data to define an Animate)
		if(element == AnimateManagerElement)
		{
		}
		else if(element == AnimateElement)
		{
			String	name(attributes.getValueAsString(AnimateNameAttribute));

			bool loop = attributes.getValueAsBool(AnimatePlayLoop);
	
			int id = attributes.getValueAsInteger(AnimateID, -1);

			int totalTime = attributes.getValueAsInteger(AnimateTime, -1);

			bool alpha = attributes.getValueAsBool( AnimateAlphaMode );

			int  loopType = attributes.getValueAsInteger(AnimateAlphaType, 1 );

			d_animate = AnimateManager::getSingleton().createAnimate(name, id, loop, totalTime, alpha, loopType );
		}
		else if (element == AniamteItemElement )
		{
			if(d_animate == 0)
			{
				throw GenericException("Aniamte::xmlHandler::startElement - Invalid file struct.");
			}
			const Image* pImage = PropertyHelper::stringToImage(attributes.getValueAsString(AniamteImage));
			if(pImage)
			{
				d_animate->defineAnimateKey(pImage);
				AnimateManager::getSingleton().addAnimateImageset(pImage->getImagesetName());
			}
		}
		else
		{
			throw FileIOException("Aniamte::xmlHandler::startElement - Unexpected data was found while parsing the animatefile: '" + element + "' is unknown.");
		}
	}

	void Animate_xmlHandler::elementEnd(const String& element)
	{
		if (element == AnimateElement)
		{
			Logger::getSingleton().logEvent("Finished creation of Animate '" + d_animate->d_name + "' via XML file.", Informative);
		}
		
	}
};