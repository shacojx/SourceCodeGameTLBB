#ifndef _FalBoundWindow_h_
#define _FalBoundWindow_h_

#include "FalModule.h"
#include "CEGUIWindowFactory.h"
#include "FalBoundWindowProperties.h"

// Start of CEGUI namespace section
namespace CEGUI
{

    class FALAGARDBASE_API FalagardBoundWindow : public Window
    {
    public:
        static const utf8   WidgetTypeName[];       //!< type name for this widget.

        /*!
        \brief
            Constructor
        */
        FalagardBoundWindow(const String& type, const String& name);

        /*!
        \brief
            Destructor
        */
        ~FalagardBoundWindow();


		/*!
		\brief
			Move the window by the pixel offsets specified in 'offset'.	
		*/
		void		offsetPixelPosition(const Vector2& offset);

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
			if (class_name==(const utf8*)"FalagardBoundWindow")	return true;
			return Window::testClassName_impl(class_name);
		}

	private:
        /*************************************************************************
            Static properties for the BoundWindow widget
        *************************************************************************/

        /*************************************************************************
        	Implementation methods
        *************************************************************************/
        /*!
        \brief
            Adds properties specific to the BoundWindow base class.
        
        \return
            Nothing.
        */
        void addBoundWindowProperties(void);
    };

    /*!
    \brief
        WindowFactory for FalagardBoundWindow type Window objects.
    */
    class FALAGARDBASE_API FalagardBoundWindowFactory : public WindowFactory
    {
    public:
        FalagardBoundWindowFactory(void) : WindowFactory(FalagardBoundWindow::WidgetTypeName) { }
        ~FalagardBoundWindowFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };

}

#endif
