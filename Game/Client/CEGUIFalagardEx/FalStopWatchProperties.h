#ifndef _FalStopWatchProperties_h_
#define _FalStopWatchProperties_h_

#include "CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	/*!
	\brief
		Properties for the Falagard StopWatch
	*/
	namespace FalagardStopWatchProperties
	{
		
		class Step: public Property	
		{
			public:
			Step() : Property(
				"Step",
				"Property to get/set StopWatch status.",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		class Timer: public Property	
		{
			public:
			Timer() : Property(
				"Timer",
				"Property to get/set StopWatch status.",
				"0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
	}
}

#endif