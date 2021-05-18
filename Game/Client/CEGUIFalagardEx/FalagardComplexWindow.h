#ifndef _FalComplexWindow_h_
#define _FalComplexWindow_h_

#include "CEGUIWindow.h"
#include "FalModule.h"
#include "CEGUIWindowFactory.h"
#include "IFalagardComplexWindow.h"
#include "FalagardComplexWindowProperties.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	class FalagardButton;
	class FalagardRewardItem;
	class FalagardActionButton;
	class FalagardMoney;

	//================================================================================
	//
	// FalagardComplexWindow
	//
	//================================================================================
	class FALAGARDBASE_API FalagardComplexWindow : public Window, public IFalagardComplexWindow
	{
    public:
        static const utf8   WidgetTypeName[];       //!< type name for this widget.

		/*************************************************************************
			Event name constants
		*************************************************************************/
		static const String EventNamespace;				//!< Namespace for global events
		static const String EventOptionClicked;
		static const String EventLinkClicked;
		static const String EventRewardItemClicked;

		static FalagardComplexWindowProperties::ActionButtonSize		d_ActionButtonSizeProperty;
		static FalagardComplexWindowProperties::ActionButtonDistance	d_ActionButtonDistanceProperty;
	   /*!
        \brief
            Constructor
        */
		FalagardComplexWindow(const String& type, const String& name);

		/*!
        \brief
            Destructor
        */
        ~FalagardComplexWindow();

		/*!
		\brief
			Initialise the Window based object ready for use.

		\note
			This must be called for every window created.  Normally this is handled automatically by the WindowFactory for each Window type.

		\return
			Nothing
		*/
		virtual void	initialise(void);

		//设置window内的action button的size
		void		setActionButtonSize( const Size& rect );
		//取得window内的action button的size
		const Size	getActionButtonSize() const;

		//设置window内的action button的distance
		void		setActionButtonDistance( const Size& rect );
		//取得window内的action button的distance
		const Size	getActionButtonDistance() const;

	public:
		//Add ChildElement (Text)
		virtual void			AddChildElement_Text(String strText, FontBase* font, TextFormatting typeset=WordWrapLeftAligned);
		//Add ChildElement (Option)
		virtual void			AddChildElement_Option(String strText,String strExtra1,String strExtra2, String strText3, FontBase* font=NULL);

		//Add ChildElement (Link)
		virtual void			AddChildElement_HyperLink( String strText, int nLinkID, bool bReArrange, FontBase* font=NULL);

		//Add ChildElement (Item)
		virtual void			AddChildElement_Item(String strIconImage, String strText,  bool bRadio, bool bReArrange, FontBase* font=NULL);

		//Add ChildElement (Action)
		virtual Window*			AddChildElement_Action(String strIconImage, int ActionID, bool bRadio, bool bReArrange, FontBase* font=NULL);

		//Add ChildElement (Money)
		virtual void			AddChildElement_Money(int money, FontBase* font=NULL);


		//Clear
		virtual void			ClearAllElement(void);
		//Recaculate
		void					performWindowLayout(void);

		virtual void			PageEnd();
	public:
		class ParentWindow : public Window
		{
		public:
	        static const utf8   WidgetTypeName[];       //!< type name for this widget.
			ParentWindow(const String& type, const String& name);
	        ~ParentWindow();
			
			void	SetComplexWindow(FalagardComplexWindow* pComplex)
			{
				d_ComplexWindow = pComplex;
			}

		protected:
			virtual void	populateRenderCache();

			FalagardComplexWindow* d_ComplexWindow;
		};

		class ParentWindowFactory : public WindowFactory
		{
		public:
			ParentWindowFactory(void) : WindowFactory(ParentWindow::WidgetTypeName) { }
			~ParentWindowFactory(void){}
			Window* createWindow(const String& name);
			void destroyWindow(Window* window);
		};

	protected:
		//----------------------------------------------------------------
		//Child Window Type
		struct ChildElement
		{
			String			d_Name;
			ChildType		d_Type;
			Rect			d_rectClient;
			const FontBase*	d_Font;
			String          d_exInfo1;
			String          d_exInfo2;
			TextFormatting	d_Typeset;
		};

		//Text
		struct ChildElement_Text : public ChildElement
		{
			String						d_Text;
		};

		//Option
		struct ChildElement_Option : public ChildElement
		{
			FalagardButton*				d_Button;
			String						d_Text;
			int							d_State;
		};

		// RewardItem
		struct ChildElement_Item : public ChildElement
		{
			FalagardRewardItem*			d_Button;
//			FalagardActionButton*		d_ActionButton;
			String						d_Text;
			bool						d_ReArrange;
			bool						d_Radio;
		};

		// 只有Action
		struct ChildElement_Action : public ChildElement
		{
//			FalagardRewardItem*			d_Button;
			FalagardActionButton*		d_ActionButton;
			String						d_Text;
			bool						d_ReArrange;
			bool						d_Radio;
		};

		// 只有HyperLink这种button
		struct ChildElement_HyperLink : public ChildElement
		{
			FalagardButton*				d_Button;
			int							d_State;
			String						d_Text;
			bool						d_ReArrange;
		};

		// Money
		struct ChildElement_Money : public ChildElement
		{
			FalagardMoney*				d_Money;
		};

		Size							d_ActionButtonSize;			// Action Button的size
		Size							d_ActionButtonDistance;		// Action Button的distance

		typedef std::vector< ChildElement* >	ChildVector;
		typedef std::vector< ChildElement_Item* >	RadioItemChild;

	protected:
		/*************************************************************************
			Implementation functions
		*************************************************************************/

		/*************************************************************************
			Overridden events
		*************************************************************************/
		virtual void	onSized(WindowEventArgs& e);
		virtual void	prepareBigchar(void);

		/*!
		\brief
			Return a Rect object describing, in un-clipped pixels, the window relative area
			that the text should be rendered in to.

		\return
			Rect object describing the area of the Window to be used for rendering text.
		*/
		Rect		getTextRenderArea(void) const;

		/*!
		\brief
			display required integrated scroll bars according to current state of the edit box and update their values.
		*/
		void		configureScrollbars(void);
		/*!
		\brief
			Destroy all child elements
		*/
		void		_destroyAllElements(void);


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
			if (class_name==(const utf8*)"FalagardComplexWindow")	return true;
			return Window::testClassName_impl(class_name);
		}

		/*************************************************************************
			Event subscribers
		*************************************************************************/
		/*!
		\brief
			handler function for when the scroll bar positions change.
		*/
		bool	handleScrollbarChange(const EventArgs& e);
		/*!
		\brief
			handler function for when option button clicked.
		*/
		bool	handleOptionClicked(const EventArgs& e);
		/*!
		\brief
			handler function for when Link button clicked.
		*/
		bool	handleLinkClicked(const EventArgs& e);
		/*!
		\brief
			handler function for when reward item icon clicked.
		*/
		bool	handleRewardItemClicked(const EventArgs& e);

		/*************************************************************************
		Implementation Data
		*************************************************************************/
		ParentWindow*	d_ParentWindow;			//!< Parent of all child element
		Scrollbar*		d_vertScrollbar;		//!< Widget used as vertical scrollbar;
		bool			d_enableVertScrollbar;	//!< true if vertical scroll bar is enabled.
		ChildVector		d_ChildVector;			//!< Child element vector 
		RadioItemChild	d_RadioChild;			//!< Item that enable radio selected
		float			d_totalHeight;			//!< Total Height 
	};


    /*!
    \brief
        WindowFactory for FalagardComplexWindow type Window objects.
    */
    class FALAGARDBASE_API FalagardComplexWindowFactory : public WindowFactory
    {
    public:
        FalagardComplexWindowFactory(void) : WindowFactory(FalagardComplexWindow::WidgetTypeName) { }
        ~FalagardComplexWindowFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
}

#endif