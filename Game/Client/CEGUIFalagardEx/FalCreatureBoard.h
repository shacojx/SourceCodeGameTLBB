#ifndef _FalCreatureBoard_h
#define _FalCreatureBoard_h

#include "FalModule.h"
#include "CEGUIWindowFactory.h"
#include "FalCreatureBoardProperties.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    class FALAGARDBASE_API FalagardCreatureBoard : public Window
    {
    public:
        static const utf8   WidgetTypeName[];       //!< type name for this widget.

        /*!
        \brief
            Constructor
        */
        FalagardCreatureBoard(const String& type, const String& name);

        /*!
        \brief
            Destructor
        */
        ~FalagardCreatureBoard();

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
			if (class_name==(const utf8*)"FalagardCreatureBoard")	return true;
			return Window::testClassName_impl(class_name);
		}

		/*************************************************************************
			Implementation Data
		*************************************************************************/

	private:
        /*************************************************************************
            Static properties for the CreatureBoard widget
        *************************************************************************/

        /*************************************************************************
        	Implementation methods
        *************************************************************************/

        /*!
        \brief
            Adds properties specific to the CreatureBoard base class.
        
        \return
            Nothing.
        */
        void addCreatureBoardProperties(void);
    };

    /*!
    \brief
        WindowFactory for FalagardCreatureBoard type Window objects.
    */
    class FALAGARDBASE_API FalagardCreatureBoardFactory : public WindowFactory
    {
    public:
        FalagardCreatureBoardFactory(void) : WindowFactory(FalagardCreatureBoard::WidgetTypeName) { }
        ~FalagardCreatureBoardFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };


}


#endif