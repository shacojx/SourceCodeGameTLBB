#include "FalChatHistoryProperties.h"
#include "FalChatHistory.h"
#include "CEGUIPropertyHelper.h"


// Start of CEGUI namespace section
namespace CEGUI
{

	// Start of ChatHistoryProperties namespace section
	namespace FalagardChatHistoryProperties
	{
		//////////////////////////////////////////////////////////////////////////

		String ChatBoardNumber::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::intToString(static_cast<const FalagardChatHistory*>(receiver)->getChatBoardNumber());
		}

		void ChatBoardNumber::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardChatHistory*>(receiver)->setChatBoardNumber(PropertyHelper::stringToInt(value));
		}

	}
}