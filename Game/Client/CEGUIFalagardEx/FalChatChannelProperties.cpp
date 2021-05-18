
#include "FalChatChannelProperties.h"
#include "FalChatChannel.h"
#include "CEGUIPropertyHelper.h"


// Start of CEGUI namespace section
namespace CEGUI
{
	namespace FalagardChatChannelProperties
	{

		//////////////////////////////////////////////////////////////////////////
		String ItemSize::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::sizeToString(static_cast<const FalagardChatChannel*>(receiver)->getItemSize());
		}

		void ItemSize::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardChatChannel*>(receiver)->setItemSize(PropertyHelper::stringToSize(value));
		}

		//////////////////////////////////////////////////////////////////////////
		String AnchorPosition::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::pointToString(static_cast<const FalagardChatChannel*>(receiver)->getAnchorPosition());
		}

		void AnchorPosition::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardChatChannel*>(receiver)->setAnchorPosition(PropertyHelper::stringToPoint(value));
		}

		//////////////////////////////////////////////////////////////////////////
		String HoverChannelBkg::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::imageToString(static_cast<const FalagardChatChannel*>(receiver)->getHoverChannelBkg());
		}

		void HoverChannelBkg::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardChatChannel*>(receiver)->setHoverChannelBkg(PropertyHelper::stringToImage(value));
		}

		//////////////////////////////////////////////////////////////////////////
		String HoverChannel::get(const PropertyReceiver* receiver) const
		{
			return (static_cast<const FalagardChatChannel*>(receiver)->getHoverChannel());
		}

		void HoverChannel::set(PropertyReceiver* receiver, const String& value)
		{
			//NONE!!!			
		}
		//////////////////////////////////////////////////////////////////////////
		String HoverChannelName::get(const PropertyReceiver* receiver) const
		{
			return (static_cast<const FalagardChatChannel*>(receiver)->getHoverChannelName());
		}

		void HoverChannelName::set(PropertyReceiver* receiver, const String& value)
		{
			//NONE!!!			
		}
	}
}
