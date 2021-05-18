#ifndef _FalMoneyProperties_h_
#define _FalMoneyProperties_h_

#include "CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	/*!
	\brief
		Properties for the Falagard Money
	*/
	namespace FalagardMoneyProperties
	{
		/*!
		\brief
			Property to access the horizontal formatting mode setting.

			\par Usage:
				- Name: HorzFormatting
				- Format: "[text]".

			\par Where [text] is one of:
				- "LeftAligned"
				- "RightAligned"
		*/
		class HorzFormatting : public Property
		{
		public:
			HorzFormatting() : Property(
				"HorzFormatting",
				"Property to get/set the horizontal formatting mode.  Value is one of the HorzFormatting strings.",
				"LeftAligned")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};

		/*!
		\brief
		Property to get/set money number

		\par Usage:
			- Name: MoneyNumber
			- Format: "[int]".

		*/
		class MoneyNumber : public Property
		{
		public:
			MoneyNumber() : Property(
				"MoneyNumber",
				"Property to get/set Money Number.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};


		/*!
		\brief
		Property to get/set icons

		\par Usage:
			- Name: GoldIcon/SilverIcon/CopperIcon
			- Format: "[image]".

		*/
		class GoldIcon : public Property
		{
		public:
			GoldIcon() : Property(
				"GoldIcon",
				"Property to get/set Gold Icon.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		class SilverIcon : public Property
		{
		public:
			SilverIcon() : Property(
				"SilverIcon",
				"Property to get/set Silver Icon.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		class CopperIcon : public Property
		{
		public:
			CopperIcon() : Property(
				"CopperIcon",
				"Property to get/set Copper Icon.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

	}
}

#endif