#ifndef _FalChatBoard_h_
#define _FalChatBoard_h_

#include "FalModule.h"
#include "CEGUIWindowFactory.h"
#include "FalChatBoardProperties.h"

// Start of CEGUI namespace section
namespace CEGUI
{

    class FALAGARDBASE_API FalagardChatBoard : public Window
    {
    public:
        static const utf8   WidgetTypeName[];       //!< type name for this widget.

        /*!
        \brief
            Constructor
        */
        FalagardChatBoard(const String& type, const String& name);

        /*!
        \brief
            Destructor
        */
        ~FalagardChatBoard();

		/*!
		\brief
			Recaculate the window size.
		*/
		virtual float			resizeSelf(float maxWidth);

    protected:
        /*************************************************************************
        	Implementation of abstract methods in Window
        *************************************************************************/
		virtual void			populateRenderCache();

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
			if (class_name==(const utf8*)"FalagardChatBoard")	return true;
			return Window::testClassName_impl(class_name);
		}

		/*************************************************************************
			Implementation Data
		*************************************************************************/

	private:
        /*************************************************************************
            Static properties for the ChatBoard widget
        *************************************************************************/

        /*************************************************************************
        	Implementation methods
        *************************************************************************/

        /*!
        \brief
            Adds properties specific to the ChatBoard base class.
        
        \return
            Nothing.
        */
        void addChatBoardProperties(void);
    };

    /*!
    \brief
        WindowFactory for FalagardChatBoard type Window objects.
    */
    class FALAGARDBASE_API FalagardChatBoardFactory : public WindowFactory
    {
    public:
        FalagardChatBoardFactory(void) : WindowFactory(FalagardChatBoard::WidgetTypeName) { }
        ~FalagardChatBoardFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };

}

#endif