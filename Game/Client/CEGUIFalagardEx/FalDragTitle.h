#ifndef _FalDragTitle_h_
#define _FalDragTitle_h_

#include "CEGUIWindow.h"
#include "FalModule.h"
#include "CEGUIWindowFactory.h"
#include "FalDragTitleProperties.h"

// Start of CEGUI namespace section
namespace CEGUI
{

	class FalagardBoundWindow;
	//================================================================================
	//
	// FalagardDragTitle
	//
	//================================================================================
	class FALAGARDBASE_API FalagardDragTitle : public Window
	{
	public:
        static const utf8   WidgetTypeName[];       //!< type name for this widget.

        /*!
        \brief
            Constructor
        */
		FalagardDragTitle(const String& type, const String& name);

		/*!
        \brief
            Destructor
        */
        ~FalagardDragTitle();

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
			if (class_name==(const utf8*)"FalagardDragTitle")	return true;
			return Window::testClassName_impl(class_name);
		}


		/*************************************************************************
			Accessors
		*************************************************************************/
		String			getDragTarget(void) const;	
		void			setDragTarget(const String& strName);

	protected:
		/*************************************************************************
			Overridden event handlers
		*************************************************************************/
		virtual void	onMouseMove(MouseEventArgs& e);
		virtual void	onMouseButtonDown(MouseEventArgs& e);
		virtual void	onMouseButtonUp(MouseEventArgs& e);
		virtual void	onCaptureLost(WindowEventArgs& e);

        /*************************************************************************
        	Data
        *************************************************************************/
		FalagardBoundWindow*	d_dragTarget;			//!< The drag target window.
		bool					d_dragging;				//!< set to true when the window is being dragged.
		Point					d_dragPoint;			//!< Point at which we are being dragged.

	private:
        /*************************************************************************
            Static properties for the FalagardDragTitle widget
        *************************************************************************/
		static FalagardDragTitleProperties::DragTarget		d_dragTargetProperty;

        /*************************************************************************
        	Implementation methods
        *************************************************************************/

        /*!
        \brief
            Adds properties specific to the DragTitle base class.
        
        \return
            Nothing.
        */
        void addDragTitleProperties(void);


	};

    /*!
    \brief
        WindowFactory for FalagardDragTitle type Window objects.
    */
    class FALAGARDBASE_API FalagardDragTitleFactory : public WindowFactory
    {
    public:
        FalagardDragTitleFactory(void) : WindowFactory(FalagardDragTitle::WidgetTypeName) { }
        ~FalagardDragTitleFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };

}

#endif