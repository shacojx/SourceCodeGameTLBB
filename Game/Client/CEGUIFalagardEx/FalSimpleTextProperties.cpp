
#include "FalSimpleTextProperties.h"
#include "FalSimpleText.h"
#include "CEGUIPropertyHelper.h"


// Start of CEGUI namespace section
namespace CEGUI
{

	// Start of StaticTextProperties namespace section
	namespace FalagardSimpleTextProperties
	{
		String	HorzFormatting::get(const PropertyReceiver* receiver) const
		{
			switch(static_cast<const FalagardSimpleText*>(receiver)->getHorizontalFormatting())
			{
			case FalagardSimpleText::RightAligned:
				return String((utf8*)"RightAligned");
				break;

			case FalagardSimpleText::HorzCentred:
				return String((utf8*)"HorzCentred");
				break;

			case FalagardSimpleText::HorzJustified:
				return String((utf8*)"HorzJustified");
				break;

			case FalagardSimpleText::WordWrapLeftAligned:
				return String((utf8*)"WordWrapLeftAligned");
				break;

			case FalagardSimpleText::WordWrapRightAligned:
				return String((utf8*)"WordWrapRightAligned");
				break;

			case FalagardSimpleText::WordWrapCentred:
				return String((utf8*)"WordWrapCentred");
				break;

			case FalagardSimpleText::WordWrapJustified:
				return String((utf8*)"WordWrapJustified");
				break;

			default:
				return String((utf8*)"LeftAligned");
				break;
			}
		}


		void	HorzFormatting::set(PropertyReceiver* receiver, const String& value)
		{
			FalagardSimpleText::HorzFormatting fmt;

			if (value == (utf8*)"RightAligned")
			{
				fmt = FalagardSimpleText::RightAligned;
			}
			else if (value == (utf8*)"HorzCentred")
			{
				fmt = FalagardSimpleText::HorzCentred;
			}
			else if (value == (utf8*)"HorzJustified")
			{
				fmt = FalagardSimpleText::HorzJustified;
			}
			else if (value == (utf8*)"WordWrapLeftAligned")
			{
				fmt = FalagardSimpleText::WordWrapLeftAligned;
			}
			else if (value == (utf8*)"WordWrapRightAligned")
			{
				fmt = FalagardSimpleText::WordWrapRightAligned;
			}
			else if (value == (utf8*)"WordWrapCentred")
			{
				fmt = FalagardSimpleText::WordWrapCentred;
			}
			else if (value == (utf8*)"WordWrapJustified")
			{
				fmt = FalagardSimpleText::WordWrapJustified;
			}
			else
			{
				fmt = FalagardSimpleText::LeftAligned;
			}

			static_cast<FalagardSimpleText*>(receiver)->setHorizontalFormatting(fmt);
		}


		String	VertFormatting::get(const PropertyReceiver* receiver) const
		{
			switch(static_cast<const FalagardSimpleText*>(receiver)->getVerticalFormatting())
			{
			case FalagardSimpleText::BottomAligned:
				return String((utf8*)"BottomAligned");
				break;

			case FalagardSimpleText::VertCentred:
				return String((utf8*)"VertCentred");
				break;

			default:
				return String((utf8*)"TopAligned");
				break;
			}
		}


		void	VertFormatting::set(PropertyReceiver* receiver, const String& value)
		{
			FalagardSimpleText::VertFormatting fmt;

			if (value == (utf8*)"BottomAligned")
			{
				fmt = FalagardSimpleText::BottomAligned;
			}
			else if (value == (utf8*)"VertCentred")
			{
				fmt = FalagardSimpleText::VertCentred;
			}
			else
			{
				fmt = FalagardSimpleText::TopAligned;
			}

			static_cast<FalagardSimpleText*>(receiver)->setVerticalFormatting(fmt);
		}

		
		String	TextScale::get(const PropertyReceiver* receiver) const
		{
			return "";
		}


		void	TextScale::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSimpleText*>(receiver)->setTextScale(PropertyHelper::stringToSize(value));
		}

		String	TextColor::get(const PropertyReceiver* receiver) const
		{
			return "";
		}


		void	TextColor::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSimpleText*>(receiver)->setTextColor( PropertyHelper::stringToColour(value));
		}
	}
}