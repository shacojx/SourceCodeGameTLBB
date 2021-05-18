#include "FalMoneyProperties.h"
#include "FalMoney.h"
#include "CEGUIPropertyHelper.h"


namespace CEGUI
{
namespace FalagardMoneyProperties
{

	//////////////////////////////////////////////////////////////////////////
	String	HorzFormatting::get(const PropertyReceiver* receiver) const
	{
		switch(static_cast<const FalagardMoney*>(receiver)->getHorizontalFormatting())
		{
		case FalagardMoney::RightAligned:
			return String((utf8*)"RightAligned");
			break;

		default:
			return String((utf8*)"LeftAligned");
			break;
		}
	}


	void	HorzFormatting::set(PropertyReceiver* receiver, const String& value)
	{
		FalagardMoney::HorzFormatting fmt;

		if (value == (utf8*)"RightAligned")
		{
			fmt = FalagardMoney::RightAligned;
		}
		else
		{
			fmt = FalagardMoney::LeftAligned;
		}

		static_cast<FalagardMoney*>(receiver)->setHorizontalFormatting(fmt);
	}

	//////////////////////////////////////////////////////////////////////////

	String MoneyNumber::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper::intToString(static_cast<const FalagardMoney*>(receiver)->getMoneyNumber());
    }

    void MoneyNumber::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<FalagardMoney*>(receiver)->setMoneyNumber(PropertyHelper::stringToInt(value));
    }

	//////////////////////////////////////////////////////////////////////////

	String GoldIcon::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper::imageToString(static_cast<const FalagardMoney*>(receiver)->getGoldIcon());
    }

    void GoldIcon::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<FalagardMoney*>(receiver)->setGoldIcon(PropertyHelper::stringToImage(value));
    }


	//////////////////////////////////////////////////////////////////////////

	String SilverIcon::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper::imageToString(static_cast<const FalagardMoney*>(receiver)->getSilverIcon());
    }

    void SilverIcon::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<FalagardMoney*>(receiver)->setSilverIcon(PropertyHelper::stringToImage(value));
    }

	//////////////////////////////////////////////////////////////////////////

	String CopperIcon::get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper::imageToString(static_cast<const FalagardMoney*>(receiver)->getCopperIcon());
    }

    void CopperIcon::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<FalagardMoney*>(receiver)->setCopperIcon(PropertyHelper::stringToImage(value));
    }


}
}