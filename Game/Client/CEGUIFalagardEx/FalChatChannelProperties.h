#ifndef _FalChatChannelProperties_h_
#define _FalChatChannelProperties_h_

#include "CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	namespace FalagardChatChannelProperties
	{
		/*!
		\brief
			Property to access the item size.

			\par Usage:
				- Name: ItemSize
				- Format: "[Size]".
		*/
		class ItemSize : public Property
		{
		public:
			ItemSize() : Property(
				"ItemSize",
				"",
				"w:0.0 h:0.0")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};

		/*!
		\brief
			Property to access the AnchorPosition.

			\par Usage:
				- Name: AnchorPosition
				- Format: "[Position]".
		*/
		class AnchorPosition : public Property
		{
		public:
			AnchorPosition() : Property(
				"AnchorPosition",
				"",
				"x:0.0 y:0.0")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};

		/*!
		\brief
			Property to access the HoverChannelBkg.

			\par Usage:
				- Name: HoverChannelBkg
				- Format: "[Image]".
		*/
		class HoverChannelBkg : public Property
		{
		public:
			HoverChannelBkg() : Property(
				"HoverChannelBkg",
				"",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};

		/*!
		\brief
			Property to access the HoverChannel.

			\par Usage:
				- Name: HoverChannel
				- Format: "[Int]".
		*/
		class HoverChannel : public Property
		{
		public:
			HoverChannel() : Property(
				"HoverChannel",
				"",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};

		/*!
		\brief
			Property to access the HoverChannelName.

			\par Usage:
				- Name: HoverChannelName
				- Format: "[String]".
		*/
		class HoverChannelName : public Property
		{
		public:
			HoverChannelName() : Property(
				"HoverChannelName",
				"",
				"")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};
	}
}


#endif