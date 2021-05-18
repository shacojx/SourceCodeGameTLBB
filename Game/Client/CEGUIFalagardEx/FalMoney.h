#ifndef _FalMoney_h_
#define _FalMoney_h_

#include "CEGUIWindow.h"
#include "FalModule.h"
#include "CEGUIWindowFactory.h"
#include "FalMoneyProperties.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	
	//================================================================================
	//
	// FalagardMoney
	//
	//================================================================================
    class FALAGARDBASE_API FalagardMoney : public Window
    {
    public:
        static const utf8   WidgetTypeName[];       //!< type name for this widget.

		/*************************************************************************
			Formatting Enumerations
		*************************************************************************/
		/*!
		\brief
			Enumeration of horizontal formatting options for text widgets
		*/
		enum HorzFormatting
		{
			LeftAligned,			//!< Text and icon is output as a single line of text with the first character aligned with the left edge of the widget.
			RightAligned,			//!< Text and icon is output as a single line of text with the last character aligned with the right edge of the widget.
		};

        /*!
        \brief
            Constructor
        */
		FalagardMoney(const String& type, const String& name);

		/*!
        \brief
            Destructor
        */
        ~FalagardMoney();

		/*!
		\brief
			Initialise the Window based object ready for use.

		\note
			This must be called for every window created.  Normally this is handled automatically by the WindowFactory for each Window type.

		\return
			Nothing
		*/
		virtual void	initialise(void);

		/*************************************************************************
			Accessors
		*************************************************************************/

		/*!
		\brief
			Return the current horizontal formatting option set for this widget.

		\return
			One of the FalagardMoney::HorzFormatting enumerated values specifying the horizontal formatting currently set.
		*/
		HorzFormatting	getHorizontalFormatting(void) const		{return	d_horzFormatting;}
		/*!
		\brief
			Set the formatting required for the text.

		\param h_fmt
			One of the HorzFormatting enumerated values specifying the formatting required.

		\return
			Nothing
		*/
		void	setHorizontalFormatting(HorzFormatting h_fmt);


		//Set/Get MoneyNumber property
		int				getMoneyNumber() const { return d_MoneyNum; }
		void			setMoneyNumber(int moneyNumber);

		//Set icons
		const Image*	getGoldIcon(void) const	{ return d_imageGoldIcon; }
		void			setGoldIcon(const Image* pImage)	{ d_imageGoldIcon = pImage; }

		const Image*	getSilverIcon(void) const	{ return d_imageSilverIcon; }
		void			setSilverIcon(const Image* pImage)	{ d_imageSilverIcon = pImage; }
		
		const Image*	getCopperIcon(void) const	{ return d_imageCopperIcon; }
		void			setCopperIcon(const Image* pImage)	{ d_imageCopperIcon = pImage; }

	protected:
        /*************************************************************************
        	Implementation of abstract methods in Window
        *************************************************************************/

		void			populateRenderCache();

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
			if (class_name==(const utf8*)"FalagardMoney")	return true;
			return Window::testClassName_impl(class_name);
		}

	protected:
        /*************************************************************************
        	Data
        *************************************************************************/
		int				d_MoneyNum;				//!< The Money Number.
		HorzFormatting	d_horzFormatting;		//!< Horizontal formatting to be applied to the text.

		const Image*	d_imageGoldIcon;		//!< The Image of gold image.
		const Image*	d_imageSilverIcon;		//!< The Image of gold image.
		const Image*	d_imageCopperIcon;		//!< The Image of gold image.

	private:
        /*************************************************************************
            Static properties for the Spinner widget
        *************************************************************************/
		static FalagardMoneyProperties::HorzFormatting	d_horzFormattingProperty;
        static FalagardMoneyProperties::MoneyNumber		d_moneyNumProperty;
        static FalagardMoneyProperties::GoldIcon		d_goldIconProperty;
        static FalagardMoneyProperties::SilverIcon		d_silverIconProperty;
        static FalagardMoneyProperties::CopperIcon		d_copperIconProperty;

        /*************************************************************************
        	Implementation methods
        *************************************************************************/

        /*!
        \brief
            Adds properties specific to the Money base class.
        
        \return
            Nothing.
        */
        void addMoneyProperties(void);

	};

    /*!
    \brief
        WindowFactory for FalagardMoney type Window objects.
    */
    class FALAGARDBASE_API FalagardMoneyFactory : public WindowFactory
    {
    public:
        FalagardMoneyFactory(void) : WindowFactory(FalagardMoney::WidgetTypeName) { }
        ~FalagardMoneyFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
}
#endif