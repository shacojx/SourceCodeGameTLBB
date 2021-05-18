#include "FalChatHistory.h"
#include "CEGUIExceptions.h"
#include "CEGUIWindowManager.h"
#include "elements/CEGUIPushButton.h"
#include "elements/CEGUIScrollbar.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "FalChatBoard.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	//===================================================================================
	//
	// FalagardChatHistory/ParentWindow
	//
	//===================================================================================
	const utf8 FalagardChatHistory::ParentWindow::WidgetTypeName[] = "Falagard/ChatHistory/ParentWindow";
	FalagardChatHistory::ParentWindow::ParentWindow(const String& type, const String& name) :
        Window(type, name)
	{
	}

	FalagardChatHistory::ParentWindow::~ParentWindow()
    {
    }

	Window* FalagardChatHistory::ParentWindowFactory::createWindow(const String& name)
    {
		return new FalagardChatHistory::ParentWindow(d_type, name);
    }

	void FalagardChatHistory::ParentWindowFactory::destroyWindow(Window* window)
    {
		delete window;
    }	
	//===================================================================================
	//
	// FalagardChatHistory
	//
	//===================================================================================
    const utf8  FalagardChatHistory::WidgetTypeName[] = "Falagard/ChatHistory";

	FalagardChatHistoryProperties::ChatBoardNumber		FalagardChatHistory::d_chatBoardNumProperty;

    FalagardChatHistory::FalagardChatHistory(const String& type, const String& name) :
        Window(type, name), 
		d_parentWindow(0),
		d_vertScrollbar(0),
		d_totalHeight(0),
		d_chatBoardNum(3),
		d_first(true),
		d_extendHeight(0)
    {
        addChatHistoryProperties();
    }

    FalagardChatHistory::~FalagardChatHistory()
    {
    }

	void FalagardChatHistory::addChatHistoryProperties(void)
    {
		CEGUI_START_ADD_STATICPROPERTY( FalagardChatHistory )
		   CEGUI_ADD_STATICPROPERTY( &d_chatBoardNumProperty );
		CEGUI_END_ADD_STATICPROPERTY
	}

	void FalagardChatHistory::initialise(void)
	{
		// create the component sub-widgets
		d_parentWindow = static_cast<ParentWindow*>(WindowManager::getSingleton().getWindow(getName() + "__auto_parent__"));
		d_vertScrollbar = static_cast<Scrollbar*>(WindowManager::getSingleton().getWindow(getName() + "__auto_vscrollbar__"));
		d_toEnd = static_cast<PushButton*>(WindowManager::getSingleton().getWindow(getName() + "__auto_toend_btn__"));
		// alawys show.
		d_vertScrollbar->show();

		d_parentWindow->setChatHistoryWindow(this);
		d_parentWindow->setMouseLButtonHollow(true);

		addChildWindow(d_vertScrollbar);
		addChildWindow(d_toEnd);

		//Resize child window(VerscrollBar and ParentWindow)
		performChildWindowLayout();
		//Resize all child elements.
		performWindowLayout();

		// event subscription
		d_vertScrollbar->setScrollPosition(0);
		d_vertScrollbar->subscribeEvent(Scrollbar::EventScrollPositionChanged, Event::Subscriber(&FalagardChatHistory::handleScrollbarChange, this));
		d_toEnd->subscribeEvent(PushButton::EventMouseButtonDown, Event::Subscriber(&FalagardChatHistory::handleToEndBtnClick, this));
	}

	Rect FalagardChatHistory::getTextRenderArea(void) const
	{
		return d_parentWindow->getPixelRect();
	}

	void FalagardChatHistory::onSized(WindowEventArgs& e)
	{
		Window::onSized(e);

		performWindowLayout();
		requestRedraw();
	}

	/*************************************************************************
		Insert a chat string to chat board list.
	*************************************************************************/
	void FalagardChatHistory::insertChatString(const String& strChat)
	{
		FalagardChatBoard* pWindow = static_cast<FalagardChatBoard*>(WindowManager::getSingleton().createWindow((utf8*)"WoWLook/ChatBoard"));
		pWindow->setText(strChat);
		pWindow->show();
		//pWindow->setMouseLButtonHollow(true);

		if(m_listChatBoard.size() == d_chatBoardNum && m_listChatBoard.size() > 0)
		{
			d_parentWindow->removeChildWindow((uint)0);
			m_listChatBoard.erase(m_listChatBoard.begin());
		}

		d_parentWindow->addChildWindow(pWindow);

		m_listChatBoard.push_back(pWindow);

		float posEnd = d_vertScrollbar->getDocumentSize()-d_vertScrollbar->getPageSize();
		bool bPosEnd = (posEnd - d_vertScrollbar->getScrollPosition() < 10)?true:false;

		float fWinHeight = pWindow->resizeSelf(getTextRenderArea().getWidth());
		//use d_extendHeight first.
		if(d_extendHeight > 0.0f)
		{
			d_extendHeight = (d_extendHeight - fWinHeight > 0.0f)?(d_extendHeight - fWinHeight):0.0f;
		}

		performWindowLayout();

		if(bPosEnd || (d_vertScrollbar->getDocumentSize() > d_vertScrollbar->getPageSize() && d_first))
		{
			//scroll to end
			d_vertScrollbar->setScrollPosition(
				d_vertScrollbar->getDocumentSize()-d_vertScrollbar->getPageSize());
			d_first = false;
		}

		if(!bPosEnd && (m_listChatBoard.size() == d_chatBoardNum && d_vertScrollbar->getScrollPosition()))
		{
			Rect absarea(getTextRenderArea());
			//scroll up a new chatboard window's height
			d_vertScrollbar->setScrollPosition(
				d_vertScrollbar->getScrollPosition() - fWinHeight);
		}
	}

	/*************************************************************************
		Remove all chat boards.
	*************************************************************************/
	void FalagardChatHistory::removeAllChatStrings()
	{
		uint count = d_parentWindow->getChildCount();
		for(uint i = 0; i < count; ++i)
			d_parentWindow->removeChildWindow((uint)0);

		m_listChatBoard.clear();
		d_extendHeight = 0.0f;

		performWindowLayout();

		d_first = false;
	}

	/*************************************************************************
	Extend a Clear Region in history window.
	*************************************************************************/
	void FalagardChatHistory::extendClearRegion()
	{
		if(m_listChatBoard.empty())
		{
			d_extendHeight = 0.0f;
			return;
		}

		//scroll to end before extend
		scrollToEnd();

		Rect absarea(getTextRenderArea());
		d_extendHeight = absarea.getHeight();	

		performWindowLayout();
		//set new scroll pos after extend
		d_vertScrollbar->setScrollPosition(
			d_vertScrollbar->getDocumentSize()-d_vertScrollbar->getPageSize()+d_extendHeight);
	}

	/*************************************************************************
		Scroll to End.
	*************************************************************************/
	void FalagardChatHistory::scrollToEnd()
	{
		d_extendHeight = 0.0f;
		performWindowLayout();
		//scroll to end
		d_vertScrollbar->setScrollPosition(
			d_vertScrollbar->getDocumentSize()-d_vertScrollbar->getPageSize());
	}

	/*************************************************************************
		Recaculate all sub window size.
	*************************************************************************/
	void FalagardChatHistory::performWindowLayout(void)
	{
		Rect absarea(getTextRenderArea());

		std::list< FalagardChatBoard* >::iterator it;
		float fHeightNow = 0.0f;

		for(it=m_listChatBoard.begin(); it!=m_listChatBoard.end(); it++)
		{
			FalagardChatBoard& child = *(*it);

			float childHeight = child.resizeSelf(absarea.getWidth());

			child.setSize(Absolute, Size(absarea.getWidth(), childHeight));
			child.setPosition(Absolute, Point(0.0f, fHeightNow-d_vertScrollbar->getScrollPosition()));
			child.requestRedraw();

			fHeightNow += childHeight;
		}

		d_totalHeight = fHeightNow + d_extendHeight;

		configureScrollbars();
	}

	void FalagardChatHistory::configureScrollbars()
	{
		// no scrollbars?  Can't configure then!
		if(!d_vertScrollbar) return;

		Rect initialArea(getTextRenderArea());

		//
		// Set up scroll bar values
		//
		Rect renderArea(getTextRenderArea());

		d_vertScrollbar->setDocumentSize(d_totalHeight);
		d_vertScrollbar->setPageSize(renderArea.getHeight());
		d_vertScrollbar->setStepSize(ceguimax(1.0f, renderArea.getHeight() / 10.0f));
		d_vertScrollbar->setScrollPosition(d_vertScrollbar->getScrollPosition());

		requestRedraw();
		d_parentWindow->requestRedraw();
	}

	/*************************************************************************
		Handler called when the scroll bar positions change
	*************************************************************************/
	bool FalagardChatHistory::handleScrollbarChange(const EventArgs& e)
	{
		performWindowLayout();

		requestRedraw();
		d_parentWindow->requestRedraw();
		return true;
	}

	/*************************************************************************
	Handler called when when the toEnd button clicked
	*************************************************************************/
	bool FalagardChatHistory::handleToEndBtnClick(const EventArgs& e)
	{
		scrollToEnd();
		return true;
	}

    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardChatHistoryFactory::createWindow(const String& name)
    {
        return new FalagardChatHistory(d_type, name);
    }

    void FalagardChatHistoryFactory::destroyWindow(Window* window)
    {
		delete window;
    }
}