
#include "FalChatProperties.h"
#include "FalagardChatEditBox.h"
#include "CEGUIPropertyHelper.h"


namespace CEGUI
{
	namespace FalagardChatProperties
	{

		String MaxLineNumber::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::intToString(static_cast<const FalagardChatEditBox*>(receiver)->getMaxLineNumber());
		}

		void MaxLineNumber::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardChatEditBox*>(receiver)->setMaxLineNumber(PropertyHelper::stringToInt(value));
		}

		String AddText::get(const PropertyReceiver* receiver) const
		{
			return "";//PropertyHelper::intToString(static_cast<const FalagardChatEditBox*>(receiver)->getMaxLineNumber());
		}

		void AddText::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardChatEditBox*>(receiver)->AddNewText( value );
		}

		String TextClear::get(const PropertyReceiver* receiver) const
		{
			return "";//PropertyHelper::intToString(static_cast<const FalagardChatEditBox*>(receiver)->getMaxLineNumber());
		}

		void TextClear::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardChatEditBox*>(receiver)->Clear();
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////
		String LineDown::get(const PropertyReceiver* receiver) const
		{
			return "";//PropertyHelper::intToString(static_cast<const FalagardChatEditBox*>(receiver)->getMaxLineNumber());
		}

		void LineDown::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardChatEditBox*>(receiver)->Clear();
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////

		String LineUp::get(const PropertyReceiver* receiver) const
		{
			return "";//PropertyHelper::intToString(static_cast<const FalagardChatEditBox*>(receiver)->getMaxLineNumber());
		}

		void LineUp::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardChatEditBox*>(receiver)->Clear();
		}

	}
}