#ifndef _FalSuperTooltipProperties_h_
#define _FalSuperTooltipProperties_h_

#include "CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{

	namespace FalagardResizeTextProperties
	{
		class TopFrame : public Property
		{
		public:
			TopFrame() : Property(
				"TopFrame",
				"",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};		
		class MiddleFrame : public Property
		{
		public:
			MiddleFrame() : Property(
				"MiddleFrame",
				"",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};	
		class BottomFrame : public Property
		{
		public:
			BottomFrame() : Property(
				"BottomFrame",
				"",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};	
		class TextArea : public Property
		{
		public:
			TextArea() : Property(
				"TextArea",
				"",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};	
	}

	namespace FalagardSuperTooltipProperties
	{
		/*!
		\brief
			Property to access the page head window name.

			\par Usage:
				- Name: PageHeader
				- Format: "[text]".
		*/
		class PageHeader : public Property
		{
		public:
			PageHeader() : Property(
				"PageHeader",
				"Property to get/set the PageHeader window name.",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};		

		/*!
		\brief
			Property to access the page footer window name.

			\par Usage:
				- Name: PageFooter
				- Format: "[text]".
		*/
		class PageFooter : public Property
		{
		public:
			PageFooter() : Property(
				"PageFooter",
				"Property to get/set the PageFooter window name.",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};	


		/*!
		\brief
			Property to access the page elements window name.

			\par Usage:
				- Name: PageFooter
				- Format: "[text]".
		*/
		class PageElements : public Property
		{
		public:
			PageElements() : Property(
				"PageElements",
				"Property to get/set the page elements window name.",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};	
	}
}


#endif