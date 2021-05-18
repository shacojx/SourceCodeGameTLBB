#ifndef _FalMultiIMEEditBox_h_
#define _FalMultiIMEEditBox_h_

#include "FalModule.h"
#include "CEGUIWindowFactory.h"
#include "FalMultiLineEditbox.h"
#include "FalMultiIMEEditBoxProperties.h"

// Start of CEGUI namespace section
namespace CEGUI
{

    class FALAGARDBASE_API FalagardMultiIMEEditBox : public FalagardMultiLineEditbox
    {
    public:
        static const utf8   WidgetTypeName[];       //!< type name for this widget.

        /*!
        \brief
            Constructor
        */
        FalagardMultiIMEEditBox(const String& type, const String& name);

        /*!
        \brief
            Destructor
        */
        ~FalagardMultiIMEEditBox();

		/*************************************************************************
			Accessors
		*************************************************************************/
		/*!
		\brief
			Reset the caret flash time.

		\return
			None.
		*/
		virtual void	resetCaretBlink(void);

		/*!
		\brief
			return true if the Editbox has input focus.

		\return
			true if the Editbox has keyboard input focus, false if the Editbox does not have keyboard input focus.
		*/
		virtual bool	hasInputFocus(void) const;

    protected:
        /*************************************************************************
        	Implementation of abstract methods in Window
        *************************************************************************/
		virtual void			onMouseButtonDown(MouseEventArgs& e);
		virtual void			onActivated(ActivationEventArgs& e);
		virtual void			onDeactivated(ActivationEventArgs& e);
		virtual void			cacheCaratImagery(const Rect& textArea);
		virtual void			updateSelf(float elapsed);
		virtual void			onParentHidden(void);
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
			if (class_name==(const utf8*)"FalagardMultiIMEEditBox")	return true;
			return FalagardMultiLineEditbox::testClassName_impl(class_name);
		}

	private:
        /*************************************************************************
            Static properties for the ChatEditBox widget
        *************************************************************************/
		static FalagardMultiIMEEditBoxProperties::DefaultEditBox	d_defaultEditBoxProperty;


        // rendering internal vars
        float	d_lastTextOffset;			//!< x rendering offset used last time we drew the widget.
		

		double	d_blinkTime;				//!< Caret blink time in milliseconds
		double	d_lastBlink;				//!< Last timestamp of caret blink
		bool	d_caretOn;					//!< Flag to indicate whether caret is currently visible

		int		d_lastTextLen;				//!< Store the text length.

       /*************************************************************************
        	Implementation methods
        *************************************************************************/

        /*!
        \brief
            Adds properties specific to the MultiIMEEditBox base class.
        
        \return
            Nothing.
        */
        void addMultiIMEEditBoxProperties(void);
	};

    /*!
    \brief
        WindowFactory for FalagardIMEEditBox type Window objects.
    */
    class FALAGARDBASE_API FalagardMultiIMEEditBoxFactory : public WindowFactory
    {
    public:
        FalagardMultiIMEEditBoxFactory(void) : WindowFactory(FalagardMultiIMEEditBox::WidgetTypeName) { }
        ~FalagardMultiIMEEditBoxFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };

}

#endif