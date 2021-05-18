#ifndef _FalMeshWindowProperties_h_
#define _FalMeshWindowProperties_h_

#include "CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	namespace FalagardMeshWindowProperties
	{
		/*!
		\brief
			Property to access the fake object name.

			\par Usage:
				- Name: FakeObject
				- Format: "[text]".
		*/
		class FakeObject: public Property
		{
		public:
			FakeObject() : Property(
				"FakeObject",
				"Property to get/set FakeObject name.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		/*!
		\brief
			Property to access the window name that control show/hide.

			\par Usage:
				- Name: ShowControl
				- Format: "[text]".
		*/
		class ShowControl: public Property
		{
		public:
			ShowControl() : Property(
				"ShowControl",
				"Property to get/set window name that control show or hide.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
	}

}

#endif