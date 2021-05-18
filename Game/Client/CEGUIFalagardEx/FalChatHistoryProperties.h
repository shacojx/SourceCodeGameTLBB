#ifndef _FalChatHistoryProperties_h_
#define _FalChatHistoryProperties_h_

#include "CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{

	namespace FalagardChatHistoryProperties
	{
		/*!
		\brief
		Property to get/set money number

		\par Usage:
			- Name: MoneyNumber
			- Format: "[int]".

		*/
		class ChatBoardNumber : public Property
		{
		public:
			ChatBoardNumber() : Property(
				"ChatBoardNumber",
				"Property to get/set ChatBoard Number.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
	}
}


#endif