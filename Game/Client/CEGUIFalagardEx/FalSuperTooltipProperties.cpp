
#include "FalSuperTooltipProperties.h"
#include "FalSuperTooltip.h"
#include "CEGUIPropertyHelper.h"


// Start of CEGUI namespace section
namespace CEGUI
{
	// Start of StaticTextProperties namespace section
	namespace FalagardResizeTextProperties
	{
		////////////////////////////////////////////////////////////////////////

		String	TopFrame::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::imageToString((static_cast<const FalagardSuperTooltip::ResizeText*>(receiver)->getTopFrame()));
		}

		void TopFrame::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSuperTooltip::ResizeText*>(receiver)->setTopFrame(PropertyHelper::stringToImage(value));
		}

		////////////////////////////////////////////////////////////////////////

		String	MiddleFrame::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::imageToString((static_cast<const FalagardSuperTooltip::ResizeText*>(receiver)->getMiddleFrame()));
		}

		void MiddleFrame::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSuperTooltip::ResizeText*>(receiver)->setMiddleFrame(PropertyHelper::stringToImage(value));
		}

		////////////////////////////////////////////////////////////////////////

		String	BottomFrame::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::imageToString((static_cast<const FalagardSuperTooltip::ResizeText*>(receiver)->getBottomFrame()));
		}

		void BottomFrame::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSuperTooltip::ResizeText*>(receiver)->setBottomFrame(PropertyHelper::stringToImage(value));
		}
		////////////////////////////////////////////////////////////////////////

		String	TextArea::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::urectToString((static_cast<const FalagardSuperTooltip::ResizeText*>(receiver)->getTextArea()));
		}

		void TextArea::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSuperTooltip::ResizeText*>(receiver)->setTextArea(PropertyHelper::stringToURect(value));
		}
	}

	// Start of StaticTextProperties namespace section
	namespace FalagardSuperTooltipProperties
	{
		////////////////////////////////////////////////////////////////////////

		String	PageElements::get(const PropertyReceiver* receiver) const
		{
			return (static_cast<const FalagardSuperTooltip*>(receiver)->getPageElementsName());
		}

		void PageElements::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSuperTooltip*>(receiver)->setPageElementsName(value);
		}

	}

}
