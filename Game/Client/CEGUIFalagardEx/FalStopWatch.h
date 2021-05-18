#ifndef _FalStopWatch_h_
#define _FalStopWatch_h_


#include "FalModule.h"
#include "CEGUIWindowFactory.h"
#include "FalStopWatchProperties.h"
#include "FalSimpleText.h"

// Start of CEGUI namespace section
namespace CEGUI
{

    class FALAGARDBASE_API FalagardStopWatch :	public FalagardSimpleText//public Window
    {
    public:
        static const utf8   WidgetTypeName[];       //!< type name for this widget.

        /*!
        \brief
            Constructor
        */
        FalagardStopWatch(const String& type, const String& name);

        /*!
        \brief
            Destructor
        */
        ~FalagardStopWatch();

		
		void	setStep(int step);
		int		getStep(void) const { return d_timeStep; }

		void	setTimer(int time);
		int		getTimer(void) const { return d_currentTime; }

    protected:
        /*************************************************************************
        	Implementation of abstract methods in Window
        *************************************************************************/

		/*!
		\brief
		Return whether this window was inherited from the given class name at some point in the inheritance heirarchy.

		\param class_name
		The class name that is to be checked.

		\return
		true if this window was inherited from \a class_name. false if not.
		*/
		virtual bool	testClassName_impl(const String& class_name) const
		{
			if (class_name==(const utf8*)"FalagardStopWatch")	return true;
			return Window::testClassName_impl(class_name);
		}

		virtual void	updateSelf(float elapsed);

		/*************************************************************************
			Implementation Data
		*************************************************************************/
		float			f_lastSecond;				//!< Last timestamp of caret blink
		int				d_timeStep;
		int				d_currentTime;

	private:
        /*************************************************************************
            Static properties for the SimpleText widget
        *************************************************************************/
		static FalagardStopWatchProperties::Step			d_stepProperty;
		static FalagardStopWatchProperties::Timer			d_timeProperty;

        /*************************************************************************
        	Implementation methods
        *************************************************************************/

        /*!
        \brief
            Adds properties specific to the SimpleText base class.
        
        \return
            Nothing.
        */
        void addStopWatchProperties(void);

    };

    /*!
    \brief
        WindowFactory for FalagardStopWatch type Window objects.
    */
    class FALAGARDBASE_API FalagardStopWatchFactory : public WindowFactory
    {
    public:
        FalagardStopWatchFactory(void) : WindowFactory(FalagardStopWatch::WidgetTypeName) { }
        ~FalagardStopWatchFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
}



#endif