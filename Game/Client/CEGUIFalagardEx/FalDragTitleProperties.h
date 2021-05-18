#ifndef _FalDragTitleProperties_h_
#define _FalDragTitleProperties_h_

#include "CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	/*!
	\brief
		Properties for the Falagard DefaultSheet
	*/
	namespace FalagardDragTitleProperties
	{
		/*!
		\brief
			Property to access window drag target window.


			\par Usage:
				- Name: DragTarget
				- Format: [Window Name].
		*/
		class DragTarget : public Property
		{
		public:
			DragTarget() : Property(
				"DragTarget",
				"Property to get/set the drag target Window.",
				"[String]", false)
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};
	}
}

#endif