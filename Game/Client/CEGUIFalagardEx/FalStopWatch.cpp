
#include "FalStopWatch.h"


// Start of CEGUI namespace section
namespace CEGUI
{
	//===================================================================================
	//
	// FalagardStopWatch
	//
	//===================================================================================
    const utf8  FalagardStopWatch::WidgetTypeName[] = "Falagard/StopWatch";

	FalagardStopWatchProperties::Step			FalagardStopWatch::d_stepProperty;
	FalagardStopWatchProperties::Timer			FalagardStopWatch::d_timeProperty;

    FalagardStopWatch::FalagardStopWatch(const String& type, const String& name) :
        FalagardSimpleText(type, name),
		d_currentTime(0),
		d_timeStep(-1)
    {
        addStopWatchProperties();

		f_lastSecond = System::getSingleton().getCurTimeElapsed();
    }

    FalagardStopWatch::~FalagardStopWatch()
    {
    }

	void FalagardStopWatch::addStopWatchProperties(void)
    {
		CEGUI_START_ADD_STATICPROPERTY( FalagardStopWatch )
		   CEGUI_ADD_STATICPROPERTY( &d_stepProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_timeProperty );
		CEGUI_END_ADD_STATICPROPERTY
	}

	void FalagardStopWatch::setStep(int step)
	{
		d_timeStep = step;
		requestRedraw();
	}

	void FalagardStopWatch::setTimer(int time)
	{
		d_currentTime = time;
		requestRedraw();
	}

	void FalagardStopWatch::updateSelf(float elapsed)
	{
		if(d_currentTime == 0 || d_currentTime < -1) 
		{
			setText("");
			requestRedraw();		
			return;
		}
		if(d_currentTime == -1 )
		{
			setText("N/A");
			requestRedraw();
			return;
		}
		float time_now = System::getSingleton().getCurTimeElapsed();

		//
		// Calculate the time
		//
		if( time_now - f_lastSecond >= 1.0f )
		{
			f_lastSecond = time_now;
			d_currentTime += d_timeStep;
	
			char value[128];
			int hour;
			int minute;
			int second;
			hour =  d_currentTime/3600;
			minute = (d_currentTime%3600)/60;
			second = (d_currentTime%3600)%60 - d_timeStep; // - (60 * minute)//精确校正值 +1

			if( hour == 0 )
			{
				if(second < 10)
					_snprintf(value, 128, "%d:%d%d", minute,0,second);
				else
					_snprintf(value, 128, "%d:%d", minute,second);
			}
			else
			{
				_snprintf(value, 128, "%d:%d:%d", hour, minute,second);
			}

			setText(value);

			requestRedraw();
		}

		FalagardSimpleText::updateSelf(elapsed);
	}

    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardStopWatchFactory::createWindow(const String& name)
    {
        return new FalagardStopWatch(d_type, name);
    }

    void FalagardStopWatchFactory::destroyWindow(Window* window)
    {
		delete window;
    }

}