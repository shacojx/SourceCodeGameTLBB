
#include "FalStopWatchProperties.h"
#include "FalStopWatch.h"
#include "CEGUIPropertyHelper.h"


// Start of CEGUI namespace section
namespace CEGUI
{

	// Start of StaticTextProperties namespace section
	namespace FalagardStopWatchProperties
	{
		///////////////////////////////////////////////////////////////////////////

		String Step::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::intToString( static_cast<const FalagardStopWatch*>(receiver)->getStep() );
		}

		void Step::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardStopWatch*>(receiver)->setStep( PropertyHelper::stringToInt(value) );
		}

		///////////////////////////////////////////////////////////////////////////

		String Timer::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::intToString( static_cast<const FalagardStopWatch*>(receiver)->getTimer() );
		}

		void Timer::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardStopWatch*>(receiver)->setTimer( PropertyHelper::stringToInt(value) );
		}

	}
}