
#ifndef _CEGUIAnimate_xmlHandler_h_
#define _CEGUIAnimate_xmlHandler_h_

#include "CEGUIXMLHandler.h"
#include "CEGUIAnimate.h"
#include "CEGUIString.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	/*************************************************************************
	Implementation Classes
	*************************************************************************/
	/*!
	\brief
	Handler class used to parse the Imageset XML files using SAX2
	*/
	class Animate_xmlHandler :	public XMLHandler
	{
	public:
		/*************************************************************************
		Construction & Destruction
		*************************************************************************/
		/*!
		\brief
		Constructor for Animate::xmlHandler objects
		*/
		Animate_xmlHandler() {};
		/*!
		\brief
		Destructor for Animate::xmlHandler objects
		*/
		virtual ~Animate_xmlHandler(){};

		/*************************************************************************
		SAX2 Handler overrides
		*************************************************************************/ 
		/*!
		\brief
		document processing (only care about elements, schema validates format)
		*/
		virtual void elementStart(const String& element, const XMLAttributes& attributes);
		virtual void elementEnd(const String& element);

		/*************************************************************************
		Functions used by our implementation
		*************************************************************************/
		Animate*	getAnimate(void) const				{return d_animate;}

	private:
		/*************************************************************************
		Implementation Constants
		*************************************************************************/
		static const String AnimateManagerElement;		//!< Tag name for Animate Manager elements.		
		static const String AnimateElement;				//!< Tag name for Animate elements.		
		static const String AniamteItemElement;			//!< Tag name for Image elements.		
		static const char	AnimateNameAttribute[];		//!< Attribute name that stores the name of the Animate.
		static const char	AnimatePlayLoop[];			//!< Attribute name that stores the loop mode of the new Animate.
		static const char	AnimateTime[];				//!< Attribute name that stores the default play time of the new Animate.
		static const char   AnimateID[];				//!< Attribute name that stores the face motion id of the new Animate.
		static const char   AniamteImage[];				//!< Attribute name that stores the image name of the new Animate element.
		static const char	AnimateAlphaMode[];
		static const char   AnimateAlphaType[];

		/*************************************************************************
		Implementation Data
		*************************************************************************/
		Animate*   d_animate;			//!< Holds a pointer to the animate that created the handler object
	};
};

#endif