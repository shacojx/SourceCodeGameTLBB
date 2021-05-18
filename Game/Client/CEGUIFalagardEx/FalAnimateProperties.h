#ifndef _FalAnimateProperties_h_
#define _FalAnimateProperties_h_

#include "CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Properties for the Falagard ListHeaderSegment
*/
	namespace FalagardAnimateProperties
	{
	/*
	\brief
	Property to get/set Animate name status

	\par Usage:
		- Name: Empty
		- Format: "aniamte name".

	*/
		class Animate : public Property
		{
		public:
			Animate() : Property(
				"Animate",
				"Property to get/set Animate name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
	}
}

#endif  // end of guard _FalAnimateProperties_h_
