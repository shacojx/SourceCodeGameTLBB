#ifndef _FalChatHistoryBox_h_
#define _FalChatHistoryBox_h_

#include "FalModule.h"
#include "CEGUIWindowFactory.h"
#include "FalChatHistoryProperties.h"
#include "IFalChatHistory.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	class FalagardChatBoard;

    class FALAGARDBASE_API FalagardChatHistory : public Window, public IFalagardChatHistory
    {
    public:
        static const utf8   WidgetTypeName[];       //!< type name for this widget.

       /*!
        \brief
            Constructor
        */
        FalagardChatHistory(const String& type, const String& name);

        /*!
        \brief
            Destructor
        */
        ~FalagardChatHistory();

		/*************************************************************************
			Accessors
		*************************************************************************/
		//Set/Get ChatBoardNumber property
		int				getChatBoardNumber() const { return d_chatBoardNum; }
		void			setChatBoardNumber(const int BoardNum) { d_chatBoardNum = BoardNum;}

		/*************************************************************************
        		Implementation called by game logic
        *************************************************************************/
		/*!
		\brief
			Insert a chat string to chat board list.
		*/
		virtual void insertChatString(const String& strChat);

		/*!
		\brief
			Remove all chat boards.
		*/
		virtual void removeAllChatStrings();

		/*!
		\brief
			Extend a Clear Region in history window.
		*/
		virtual void extendClearRegion();

		/*!
		\brief
			Scroll to ParentWindows's end
		*/
		virtual void scrollToEnd();

		/*!
		\brief
			Recaculate all sub window size.
		*/
		void performWindowLayout(void);

	public:
		class ParentWindow : public Window
		{
		public:
	        static const utf8   WidgetTypeName[];       //!< type name for this widget.
			ParentWindow(const String& type, const String& name);
	        ~ParentWindow();
			
			void setChatHistoryWindow(FalagardChatHistory* chathistoryWindow)
			{
				d_chatHistoryWindow = chathistoryWindow;
			}

		protected:
			FalagardChatHistory* d_chatHistoryWindow;
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
        /*************************************************************************
        	Implementation of abstract methods in Window
        *************************************************************************/
		virtual void	initialise(void);
		virtual void	onSized(WindowEventArgs& e);

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
			if (class_name==(const utf8*)"FalagardChatHistory")	return true;
			return Window::testClassName_impl(class_name);
		}

		/*!
		\brief
			Return a Rect object describing, in un-clipped pixels, the window relative area
			that the text should be rendered in to.

		\return
			Rect object describing the area of the Window to be used for rendering text.
		*/
		Rect getTextRenderArea(void) const;

		/*!
		\brief
			display required integrated scroll bars according to current state of the edit box and update their values.
		*/
		void configureScrollbars(void);

		/*************************************************************************
			Event subscribers
		*************************************************************************/
		/*!
		\brief
			handler function for when the scroll bar positions change.
		*/
		bool handleScrollbarChange(const EventArgs& e);

		/*************************************************************************
		Event subscribers
		*************************************************************************/
		/*!
		\brief
		handler function for when the toEnd button clicked.
		*/
		bool handleToEndBtnClick(const EventArgs& e);

		/*************************************************************************
			Implementation Data
		*************************************************************************/
		ParentWindow*						d_parentWindow;		//!< Parent of all child element
		std::list< FalagardChatBoard* >		m_listChatBoard;	//!< The list of chat history.
		Scrollbar*							d_vertScrollbar;	//!< Widget used as vertical scrollbar;
		float								d_totalHeight;		//!< Total Height of all sub elements.
		int									d_chatBoardNum;		//!< chat element num
		bool								d_first;			//!< is history msg show first.
		PushButton*							d_toEnd;			//!< Widget used for the toEnd button.
		float								d_extendHeight;		//!< Total extend Height for user's show clear.

	private:
        /*************************************************************************
            Static properties for the ChatHistory widget
        *************************************************************************/
		static FalagardChatHistoryProperties::ChatBoardNumber		d_chatBoardNumProperty;

       /*************************************************************************
        	Implementation methods
        *************************************************************************/

        /*!
        \brief
            Adds properties specific to the ChatHistory base class.
        
        \return
            Nothing.
        */
        void addChatHistoryProperties(void);
	};

    /*!
    \brief
        WindowFactory for FalagardChatHistory type Window objects.
    */
    class FALAGARDBASE_API FalagardChatHistoryFactory : public WindowFactory
    {
    public:
        FalagardChatHistoryFactory(void) : WindowFactory(FalagardChatHistory::WidgetTypeName) { }
        ~FalagardChatHistoryFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
}

#endif