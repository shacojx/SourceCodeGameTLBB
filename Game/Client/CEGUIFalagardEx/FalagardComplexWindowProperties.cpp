
#include "FalagardComplexWindowProperties.h"
#include "falagardComplexWindow.h"
#include "CEGUIPropertyHelper.h"
#include <stdio.h>


namespace CEGUI
{
	namespace FalagardComplexWindowProperties
	{

		String ActionButtonSize::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::sizeToString( static_cast<const FalagardComplexWindow*>(receiver)->getActionButtonSize() );
		}

		void ActionButtonSize::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardComplexWindow*>(receiver)->setActionButtonSize( PropertyHelper::stringToSize(value) );
		}

		String ActionButtonDistance::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::sizeToString( static_cast<const FalagardComplexWindow*>(receiver)->getActionButtonDistance() );
		}

		void ActionButtonDistance::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardComplexWindow*>(receiver)->setActionButtonDistance( PropertyHelper::stringToSize(value) );
		}

	};
}
