#ifndef _FalSimpleTextProperties_h_
#define _FalSimpleTextProperties_h_

#include "CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	/*!
	\brief
		Properties for the Falagard SimpleText
	*/
	namespace FalagardSimpleTextProperties
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
				- "HorzCentred"
				- "HorzJustified"
				- "WordWrapLeftAligned"
				- "WordWrapRightAligned"
				- "WordWrapCentred"
				- "WordWrapJustified"
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
			Property to access the vertical formatting mode setting.

			\par Usage:
				- Name: VertFormatting
				- Format: "[text]".

			\par Where [text] is one of:
				- "TopAligned"
				- "BottomAligned"
				- "VertCentred"
		*/
		class VertFormatting : public Property
		{
		public:
			VertFormatting() : Property(
				"VertFormatting",
				"Property to get/set the vertical formatting mode.  Value is one of the VertFormatting strings.",
				"VertCentred")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};
		//设置字符得缩放比例
		class TextScale: public Property
		{
		public:
			TextScale() : Property(
				"TextScale",
				"Property to get/set the text colours for the StaticText widget.  Value is \"tl:[aarrggbb] tr:[aarrggbb] bl:[aarrggbb] br:[aarrggbb]\".",
				"x:1 y:1")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};
		//设置字符得颜色
		class TextColor: public Property
		{
		public:
			TextColor() : Property(
				"TextColor",
				"Property to get/set the text colours for the StaticText widget.  Value is \"tl:[aarrggbb] tr:[aarrggbb] bl:[aarrggbb] br:[aarrggbb]\".",
				"x:1 y:1")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};
	}
}

#endif