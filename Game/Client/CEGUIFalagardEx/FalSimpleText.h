#ifndef _FalSimpleText_h_
#define _FalSimpleText_h_


#include "FalModule.h"
#include "CEGUIWindowFactory.h"
#include "FalSimpleTextProperties.h"

// Start of CEGUI namespace section
namespace CEGUI
{

    class FALAGARDBASE_API FalagardSimpleText : public Window
    {
    public:
        static const utf8   WidgetTypeName[];       //!< type name for this widget.

		/*************************************************************************
			Formatting Enumerations
		*************************************************************************/
		/*!
		\brief
			Enumeration of horizontal formatting options for static text widgets
		*/
		enum HorzFormatting
		{
			LeftAligned,			//!< Text is output as a single line of text with the first character aligned with the left edge of the widget.
			RightAligned,			//!< Text is output as a single line of text with the last character aligned with the right edge of the widget.
			HorzCentred,			//!< Text is output as a single line of text horizontally centred within the widget.
			HorzJustified,			//!< Text is output as a single line of text with the first and last characters aligned with the edges of the widget.
			WordWrapLeftAligned,	//!< Text is output as multiple word-wrapped lines of text with the first character of each line aligned with the left edge of the widget.
			WordWrapRightAligned,	//!< Text is output as multiple word-wrapped lines of text with the last character of each line aligned with the right edge of the widget.
			WordWrapCentred,		//!< Text is output as multiple word-wrapped lines of text with each line horizontally centered within the widget.
			WordWrapJustified		//!< Text is output as multiple word-wrapped lines of text with the first and last characters of each line aligned with the edges of the widget.
		};


		/*!
		\brief
			Enumeration of vertical formatting options for a static text widgets
		*/
		enum VertFormatting
		{
			TopAligned,		//!< Text is output with the top of first line of text aligned with the top edge of the widget.
			BottomAligned,	//!< Text is output with the bottom of last line of text aligned with the bottom edge of the widget.
			VertCentred     //!< Text is output vertically centred within the widget.
		};

        /*!
        \brief
            Constructor
        */
        FalagardSimpleText(const String& type, const String& name);

        /*!
        \brief
            Destructor
        */
        ~FalagardSimpleText();

		/*************************************************************************
			Accessors
		*************************************************************************/

		/*!
		\brief
			Return the current horizontal formatting option set for this widget.

		\return
			One of the FalagardSimpleText::HorzFormatting enumerated values specifying the horizontal formatting currently set.
		*/
		HorzFormatting	getHorizontalFormatting(void) const		{return	d_horzFormatting;}


		/*!
		\brief
			Return the current vertical formatting option set for this widget.

		\return
			One of the FalagardSimpleText::VertFormatting enumerated values specifying the vertical formatting currently set.
		*/
		VertFormatting	getVerticalFormatting(void) const		{return	d_vertFormatting;}

		/*!
		\brief
			Set the formatting required for the text.

		\param v_fmt
			One of the VertFormatting enumerated values specifying the formatting required.

		\return
			Nothing
		*/
		void	setVerticalFormatting(VertFormatting v_fmt);


		/*!
		\brief
			Set the formatting required for the text.

		\param h_fmt
			One of the HorzFormatting enumerated values specifying the formatting required.

		\return
			Nothing
		*/
		void	setHorizontalFormatting(HorzFormatting h_fmt);


    protected:
        /*************************************************************************
        	Implementation of abstract methods in Window
        *************************************************************************/
		virtual void			populateRenderCache();
		virtual Rect			getTextRenderArea(void) const { return Rect(Point(0, 0), getAbsoluteSize()); }

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
			if (class_name==(const utf8*)"FalagardSimpleText")	return true;
			return Window::testClassName_impl(class_name);
		}

		/*************************************************************************
			Implementation Data
		*************************************************************************/
		HorzFormatting	d_horzFormatting;		//!< Horizontal formatting to be applied to the text.
		VertFormatting	d_vertFormatting;		//!< Vertical formatting to be applied to the text.


		float			d_fScaleX;
		float			d_fScaleY;
		colour			d_TextColour;
	public:
		void	setTextScale( Size  scale );
		Size    getTextScale() { return Size( d_fScaleX, d_fScaleY ); };
		void    setTextColor( const colour& col ) { d_TextColour = col; };
	private:
        /*************************************************************************
            Static properties for the SimpleText widget
        *************************************************************************/
		static FalagardSimpleTextProperties::VertFormatting	d_vertFormattingProperty;
		static FalagardSimpleTextProperties::HorzFormatting	d_horzFormattingProperty;
		static FalagardSimpleTextProperties::TextScale		d_textScaleProperty;
		static FalagardSimpleTextProperties::TextColor		d_textColorProperty;

        /*************************************************************************
        	Implementation methods
        *************************************************************************/

        /*!
        \brief
            Adds properties specific to the SimpleText base class.
        
        \return
            Nothing.
        */
        void addSimpleTextProperties(void);

    };

    /*!
    \brief
        WindowFactory for FalagardSimpleText type Window objects.
    */
    class FALAGARDBASE_API FalagardSimpleTextFactory : public WindowFactory
    {
    public:
        FalagardSimpleTextFactory(void) : WindowFactory(FalagardSimpleText::WidgetTypeName) { }
        ~FalagardSimpleTextFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
}



#endif