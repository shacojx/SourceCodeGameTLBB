#ifndef _FalMultiIMEEditBoxProperties_h_
#define _FalMultiIMEEditBoxProperties_h_

#include "CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	/*!
	\brief
		Properties for the Falagard MultiIMEEditBox
	*/
	namespace FalagardMultiIMEEditBoxProperties
	{
		/*!
		\brief
			Property to access the default editbox property.

			\par Usage:
				- Name: DefaultEditBox
				- Format: "bool".
		*/
		class DefaultEditBox : public Property
		{
		public:
			DefaultEditBox() : Property(
				"DefaultEditBox",
				"Property to get/set the default editbox property.",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};
	}
}


#endif