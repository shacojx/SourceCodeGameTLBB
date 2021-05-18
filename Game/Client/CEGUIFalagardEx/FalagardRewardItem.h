#ifndef _FalRewardItemWindow_h_
#define _FalRewardItemWindow_h_

#include "FalModule.h"
#include "FalButton.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	//================================================================================
	//
	// FalagardRewardItem
	//
	//================================================================================
	class FALAGARDBASE_API FalagardRewardItem : public FalagardButton
	{
	public:
        static const utf8   WidgetTypeName[];       //!< type name for this widget.

        /*!
        \brief
            Constructor
        */
		FalagardRewardItem(const String& type, const String& name);

		/*!
        \brief
            Destructor
        */
        ~FalagardRewardItem();

		/*!
		\brief
			Initialise the Window based object ready for use.

		\note
			This must be called for every window created.  Normally this is handled automatically by the WindowFactory for each Window type.

		\return
			Nothing
		*/
		virtual void	initialise(void);

		//Checked state
		bool isChecked() const { return d_Checked; }
		void setChecked(bool checked) { d_Checked = checked; }

		//Helper 
		void* getHelper(void) const { return d_OwnerItem; }
		void  setHelper(void* pHelper) { d_OwnerItem = pHelper; }

	protected:
		//Recaculate
		void performWindowLayout(void);

		/*************************************************************************
			Overridden events
		*************************************************************************/
		virtual void	onSized(WindowEventArgs& e);

		virtual void	drawSelf(float z);

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
			if (class_name==(const utf8*)"FalagardRewardItem")	return true;
			return FalagardButton::testClassName_impl(class_name);
		}


		/*************************************************************************
			Implementation Data
		*************************************************************************/
		Rect	d_rectImage;			// Rect for Image
		bool	d_Checked;				// Checked state
		void*	d_OwnerItem;			// Helper
	};


    /*!
    \brief
        WindowFactory for FalagardRewardItem type Window objects.
    */
	class FALAGARDBASE_API FalagardRewardItemWindowFactory : public WindowFactory
	{
	public:
		FalagardRewardItemWindowFactory(void) : WindowFactory(FalagardRewardItem::WidgetTypeName) { }
		~FalagardRewardItemWindowFactory(void){}
		Window* createWindow(const String& name);
		void destroyWindow(Window* window);
	};

}

#endif
