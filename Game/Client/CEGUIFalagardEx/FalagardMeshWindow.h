#ifndef _FalMeshWindow_h_
#define _FalMeshWindow_h_

#include "FalModule.h"
#include "CEGUIWindowFactory.h"
#include "FalagardMeshWindowProperties.h"
#include "IFalagardMeshWindow.h"
#include "CEGUIRenderableImage.h"
#include "elements\CEGUIStaticImage.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	//================================================================================
	//
	// FalagardMeshWindow
	//
	//================================================================================
	class FALAGARDBASE_API FalagardMeshWindow : public StaticImage, public IFalagardMeshWindow
	{
	public:
		static const utf8   WidgetTypeName[];	//!< type name for this widget.


        static const String EventNamespace;		//!< Namespace for global events

        static const String EventMeshHook;		//!< Name of the event fired when the window is set to show a fake obj.
        static const String EventMeshShown;		//!< Name of the event fired when the window is shown.
        static const String EventMeshHidden;	//!< Name of the event fired when the window is hidden.

		/*!
        \brief
            Constructor
        */
		FalagardMeshWindow(const String& type, const String& name);

		/*!
        \brief
            Destructor
        */
		~FalagardMeshWindow(void);


		/*************************************************************************
			Accessors
		*************************************************************************/

		/*!
		\brief
			Return the current fake object name set for this widget.
		*/
		String			getFakeObject(void) const { return d_fakeObjectName; }
		/*!
		\brief
			Set the fake object name required for this widget.
		*/
		void			setFakeObject(const String& fakeObject);

		/*!
		\brief
			Return the window name that control show or hide.
		*/
		String			getShowControl(void) const { return d_showControlWindowName; }
		/*!
		\brief
			Set the window name that control show or hide.
		*/
		void			setShowControl(const String& windowName);

        /*************************************************************************
        		Implementation called by game logic
        *************************************************************************/
		/*!
		\brief
			Subscribes MeshShown Event.
		*/
		virtual void	subscribeShownEvent(Event::Subscriber subscriber);
		/*!
		\brief
			Subscribes MeshHiden Event.
		*/
		virtual void	subscribeHidenEvent(Event::Subscriber subscriber);
		/*!
		\brief
			Subscribes MeshHook Event.
		*/
		virtual void	subscribeHookEvent(Event::Subscriber subscriber);

	protected:
        /*************************************************************************
        	Implementation of abstract methods in Window
        *************************************************************************/
		virtual void			populateRenderCache();
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
			if (class_name==(const utf8*)"FalagardMeshWindow")	return true;
			return StaticImage::testClassName_impl(class_name);
		}

		/*************************************************************************
			Implementation Data
		*************************************************************************/
		String				d_fakeObjectName;			//!< The fake object to display name.
		String				d_showControlWindowName;	//!< The window name that control show/hide(Top level parent window.)

		/*************************************************************************
			The function handle show control window's event
		*************************************************************************/
		bool			handleControlShown(const CEGUI::EventArgs& e);
		bool			handleControlHidden(const CEGUI::EventArgs& e);

	private:
        /*************************************************************************
            Static properties for the MeshWindow widget
        *************************************************************************/
		static FalagardMeshWindowProperties::FakeObject		d_fakeObjectProperty;
		static FalagardMeshWindowProperties::ShowControl	d_showControlProperty;

        /*************************************************************************
        	Implementation methods
        *************************************************************************/

        /*!
        \brief
            Adds properties specific to the MeshWindow base class.
        
        \return
            Nothing.
        */
        void addMeshWindowProperties(void);
	};

    /*!
    \brief
        WindowFactory for FalagardMeshWindow type Window objects.
    */
	class FALAGARDBASE_API FalagardMeshWindowFactory : public WindowFactory
    {
    public:
        FalagardMeshWindowFactory(void) : WindowFactory(FalagardMeshWindow::WidgetTypeName) { }
        ~FalagardMeshWindowFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
};

#endif