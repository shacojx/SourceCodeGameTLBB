
#include "FalagardComplexWindowProperties.h"
#include "FalagardComplexWindow.h"
#include "CEGUIExceptions.h"
#include "CEGUIWindowManager.h"
#include "elements/CEGUIScrollbar.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "FalButton.h"
#include "FalagardRewardItem.h"
#include "FalMoney.h"
#include "FalActionButton.h"
#include "..\Common\AxTrace.h"

// Start of CEGUI namespace section
namespace CEGUI
{

	//===================================================================================
	//
	// FalagardComplexWindow/ParentWindow
	//
	//===================================================================================
	const utf8 FalagardComplexWindow::ParentWindow::WidgetTypeName[] = "Falagard/ComplexWindow/ParentWindow";

	FalagardComplexWindowProperties::ActionButtonSize		FalagardComplexWindow::d_ActionButtonSizeProperty;
	FalagardComplexWindowProperties::ActionButtonDistance	FalagardComplexWindow::d_ActionButtonDistanceProperty;

	Window* FalagardComplexWindow::ParentWindowFactory::createWindow(const String& name)
    {
		return new FalagardComplexWindow::ParentWindow(d_type, name);
    }

	void FalagardComplexWindow::ParentWindowFactory::destroyWindow(Window* window)
    {
		delete window;
    }

	FalagardComplexWindow::ParentWindow::ParentWindow(const String& type, const String& name) :
        Window(type, name)
	{

	}

	FalagardComplexWindow::ParentWindow::~ParentWindow()
    {
    }

	/*************************************************************************
		Perform the actual rendering for this Window.
	*************************************************************************/
	void FalagardComplexWindow::ParentWindow::populateRenderCache()
	{
		// get destination area for the text.
		Rect absarea(getPixelRect());
		Rect clipper(absarea);


//	    ColourRect final_cols(colour(255.0f, 255.0f, 255.0f));

	    ColourRect final_cols(colour(1.0f, 1.0f, 1.0f));


		for(int i=0; i<(int)d_ComplexWindow->d_ChildVector.size(); i++)
		{
			ChildElement* pElement = d_ComplexWindow->d_ChildVector[i];
			const FontBase* font = pElement->d_Font;

			switch(pElement->d_Type)
			{
			case CT_Text:
				{
					ChildElement_Text* pTextElement = (ChildElement_Text*)pElement;
					Rect textArea(pElement->d_rectClient);
					textArea.d_top -= d_ComplexWindow->d_vertScrollbar->getScrollPosition();

//					AxTrace(0, 1, "ComplexWindowDraw:%f, %f", textArea.getWidth(), textArea.getHeight());

					d_renderCache.cacheText(this, 
						pTextElement->d_Text, font, (TextFormatting)pTextElement->d_Typeset, textArea, 0.0f, final_cols);
				}
				break;

			case CT_Option:
				{
				}
				break;

			case CT_Item:
				{
				}
				break;
			}
		}
	}
	//===================================================================================
	//
	// FalagardComplexWindow
	//
	//===================================================================================
    const utf8 FalagardComplexWindow::WidgetTypeName[] = "Falagard/ComplexWindow";

	/*************************************************************************
		Event name constants
	*************************************************************************/
	const String FalagardComplexWindow::EventNamespace("FalagardComplexWindow");
	const String FalagardComplexWindow::EventOptionClicked( (utf8*)"OptionClicked" );
	const String FalagardComplexWindow::EventLinkClicked( (utf8*)"LinkClicked" );
	const String FalagardComplexWindow::EventRewardItemClicked( (utf8*)"RewardItemClicked" );

    FalagardComplexWindow::FalagardComplexWindow(const String& type, const String& name) :
        Window(type, name), 
		d_ParentWindow(0),
		d_vertScrollbar(0),
		d_enableVertScrollbar(false),
		d_totalHeight(0.0f)
    {
		CEGUI_START_ADD_STATICPROPERTY( FalagardComplexWindow )
		   CEGUI_ADD_STATICPROPERTY( &d_ActionButtonSizeProperty  );
		   CEGUI_ADD_STATICPROPERTY( &d_ActionButtonDistanceProperty  );
		CEGUI_END_ADD_STATICPROPERTY


		d_ActionButtonSize = Size( 0.129293f,0.1236715f );
		d_ActionButtonDistance = Size( 0.129293f,0.1236715f );

    }

    FalagardComplexWindow::~FalagardComplexWindow()
    {
		_destroyAllElements();
    }

	void FalagardComplexWindow::initialise(void)
	{
		// create the component sub-widgets
		d_ParentWindow = static_cast<ParentWindow*>(WindowManager::getSingleton().getWindow(getName() + "__auto_parent__"));
		d_vertScrollbar = static_cast<Scrollbar*>(WindowManager::getSingleton().getWindow(getName() + "__auto_vscrollbar__"));

		d_ParentWindow->SetComplexWindow(this);
		d_vertScrollbar->hide();

		addChildWindow(d_vertScrollbar);

		performWindowLayout();
		performChildWindowLayout();

		// event subscription
		d_vertScrollbar->subscribeEvent(Scrollbar::EventScrollPositionChanged, Event::Subscriber(&FalagardComplexWindow::handleScrollbarChange, this));
	}

	/*************************************************************************
		Add edit box specific events
	*************************************************************************/
/*	void FalagardComplexWindow::addComplexWindowEvents(void)
	{
		addEvent(EventOptionClicked);
		addEvent(EventLinkClicked);
		addEvent(EventRewardItemClicked);
	}
*/
	/*************************************************************************
		Handler called when the scroll bar positions change
	*************************************************************************/
	bool FalagardComplexWindow::handleScrollbarChange(const EventArgs& e)
	{
		performWindowLayout();
		requestRedraw();
		d_ParentWindow->requestRedraw();
		return true;
	}

	/*************************************************************************
		Handler function for when option button clicked.
	*************************************************************************/
	bool FalagardComplexWindow::handleOptionClicked(const EventArgs& e)
	{
		WindowEventArgs arg( ((const WindowEventArgs&) e).window);
		fireEvent(EventOptionClicked, arg, EventNamespace);

		return true;
	}

	
	/*************************************************************************
		Handler function for when Link button clicked.
	*************************************************************************/
	bool FalagardComplexWindow::handleLinkClicked(const EventArgs& e)
	{
		WindowEventArgs arg( ((const WindowEventArgs&) e).window);
		fireEvent(EventLinkClicked, arg, EventNamespace);

		return true;
	}

	/*************************************************************************
		Handler function for when reward item icon clicked.
	*************************************************************************/
	bool FalagardComplexWindow::handleRewardItemClicked(const EventArgs& e)
	{
		//----------------------------------------------
		//Process radio group
		FalagardRewardItem* pRewardItem =  static_cast<FalagardRewardItem*>(((const WindowEventArgs&) e).window);
		if(!pRewardItem) return true;
		
		ChildElement_Item* pItemElement = (ChildElement_Item*)pRewardItem->getHelper();
		if(!pItemElement) return true;

		if(!(pItemElement->d_Radio)) return true;

		for(int i=0; i<(int)d_RadioChild.size(); i++)
		{
			if(d_RadioChild[i] == pItemElement) 
				d_RadioChild[i]->d_Button->setChecked(true);
			else
				d_RadioChild[i]->d_Button->setChecked(false);
			d_RadioChild[i]->d_Button->requestRedraw();
		}

		//----------------------------------------------
		//FireEvent
		WindowEventArgs arg( ((const WindowEventArgs&) e).window);
		fireEvent(EventRewardItemClicked, arg, EventNamespace);

		return true;
	}

	/*************************************************************************
		Handler called when size is changed
	*************************************************************************/
	void FalagardComplexWindow::onSized(WindowEventArgs& e)
	{
		Window::onSized(e);

		performWindowLayout();
		requestRedraw();
	}

	Rect FalagardComplexWindow::getTextRenderArea(void) const
	{
		return d_ParentWindow->getPixelRect();
	}
	void FalagardComplexWindow::setActionButtonSize( const Size& rect )
	{
//		if( d_RenderSize == rect )
//			return;
		d_ActionButtonSize = rect;
//		d_fresh = true;
		requestRedraw();
		d_ParentWindow->requestRedraw();
	}

	const Size FalagardComplexWindow::getActionButtonSize() const 
	{
		return d_ActionButtonSize;
	}
	void FalagardComplexWindow::setActionButtonDistance( const Size& rect )
	{
//		if( d_RenderSize == rect )
//			return;
		d_ActionButtonDistance = rect;
//		d_fresh = true;
//		requestRedraw();
//		d_ParentWindow->requestRedraw();
	}

	const Size FalagardComplexWindow::getActionButtonDistance() const 
	{
		return d_ActionButtonDistance;
	}

	void FalagardComplexWindow::AddChildElement_Text(String strText, FontBase* font, TextFormatting typeset)
	{
		ChildElement_Text* pNewChild = new ChildElement_Text;

		pNewChild->d_Name = (utf8*)"text";
		pNewChild->d_Type = CT_Text;
		pNewChild->d_Text = strText;
		pNewChild->d_Typeset = typeset;
		if(font) pNewChild->d_Font = font;
		else pNewChild->d_Font = getFont();

		d_ChildVector.push_back(pNewChild);

		performWindowLayout();
	}

	void FalagardComplexWindow::AddChildElement_Option(String strText,String strExtra1,String strExtra2, String strText3, FontBase* font)
	{
		ChildElement_Option* pNewChild = new ChildElement_Option;

		pNewChild->d_Type = CT_Option;
		pNewChild->d_Text = strText;
		if(font) pNewChild->d_Font = font;
		else pNewChild->d_Font = getFont();

		char szTempName[32];
		int nIndex = 0;
		for(int i=0; i<(int)d_ChildVector.size(); i++)
		{
			if(d_ChildVector[i]->d_Name.substr(0, 10) == (utf8*)"__option__")
			{
				nIndex++;
			}
		}

		_snprintf(szTempName, 32, "__option__%02d#%d,%d", nIndex,atoi(strExtra1.c_str()),atoi(strExtra2.c_str()));

		pNewChild->d_Name = (utf8*)szTempName;
		pNewChild->d_State = atoi(strText3.c_str());
		switch( pNewChild->d_State)
		{
		case 1:
			pNewChild->d_Button = static_cast<FalagardButton*>(WindowManager::getSingleton().createWindow((utf8*)"WoWLook/QuestButton_1", getName() + szTempName));
			break;
		case 2:
			pNewChild->d_Button = static_cast<FalagardButton*>(WindowManager::getSingleton().createWindow((utf8*)"WoWLook/QuestButton_2", getName() + szTempName));
			break;
		case 3:
			pNewChild->d_Button = static_cast<FalagardButton*>(WindowManager::getSingleton().createWindow((utf8*)"WoWLook/QuestButton_3", getName() + szTempName));
			break;
		case 4:
			pNewChild->d_Button = static_cast<FalagardButton*>(WindowManager::getSingleton().createWindow((utf8*)"WoWLook/QuestButton_4", getName() + szTempName));
			break;
		default:
			pNewChild->d_Button = static_cast<FalagardButton*>(WindowManager::getSingleton().createWindow((utf8*)"WoWLook/QuestButton_1", getName() + szTempName));
			break;
		}
//		pNewChild->d_Button = static_cast<FalagardButton*>(WindowManager::getSingleton().createWindow((utf8*)"WoWLook/Button_1", getName() + szTempName));
		pNewChild->d_Button->setText("          "+strText);
		pNewChild->d_Button->show();
		pNewChild->d_Button->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&FalagardComplexWindow::handleOptionClicked, this));
		

		//设为子窗口
		d_ParentWindow->addChildWindow(pNewChild->d_Button);

		d_ChildVector.push_back(pNewChild);
		performWindowLayout();
	}


	void FalagardComplexWindow::AddChildElement_Item(String strIconImage, String strText,  bool bRadio, bool bReArrange, FontBase* font)
	{
		ChildElement_Item* pNewChild = new ChildElement_Item;

		pNewChild->d_Type		= CT_Item;
		pNewChild->d_Text		= strText;
		if(font) pNewChild->d_Font = font;
		else pNewChild->d_Font = getFont();

		char szTempName[32];
		int nIndex = 0;

		if(bRadio)
		{
			for(int i=0; i<(int)d_ChildVector.size(); i++)
			{
				if(d_ChildVector[i]->d_Name.substr(0, 16) == (utf8*)"__reward_radio__") nIndex++;
				_snprintf(szTempName, 32, "__reward_radio__%02d", nIndex);
			}
		}
		else
		{
			_snprintf(szTempName, 32, "__reward__%02d", (int)d_ChildVector.size());
		}

		pNewChild->d_Name = (utf8*)szTempName;
		pNewChild->d_ReArrange = bReArrange;
		pNewChild->d_Button = static_cast<FalagardRewardItem*>(WindowManager::getSingleton().createWindow((utf8*)"WoWLook/RewardItem", getName() + szTempName));
		pNewChild->d_Button->setProperty("NormalImage", strIconImage);
		pNewChild->d_Button->setProperty("Tooltip", strText);
		pNewChild->d_Button->show();
		pNewChild->d_Radio = bRadio;
		pNewChild->d_Button->setHelper((void*)pNewChild);
		pNewChild->d_Button->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&FalagardComplexWindow::handleRewardItemClicked, this));

		//设为子窗口
		d_ParentWindow->addChildWindow(pNewChild->d_Button);

		d_ChildVector.push_back(pNewChild);
		if(bRadio) d_RadioChild.push_back(pNewChild);
		performWindowLayout();
	}

	Window* FalagardComplexWindow::AddChildElement_Action(String strIconImage, int ActionID, bool bRadio, bool bReArrange, FontBase* font)
	{
		ChildElement_Action* pNewChild = new ChildElement_Action;

		pNewChild->d_Type		= CT_Action;
		pNewChild->d_Text		= (utf8*)"action";
		if(font) pNewChild->d_Font = font;
		else pNewChild->d_Font = getFont();

		char szTempName[32];
		int nIndex = 0;

		if(bRadio)
		{
			for(int i=0; i<(int)d_ChildVector.size(); i++)
			{
				if(d_ChildVector[i]->d_Name.substr(0, 16) == (utf8*)"__reward_radio__") nIndex++;
				_snprintf(szTempName, 32, "__reward_radio_%02d_%02d", (int)ActionID,nIndex);
			}
		}
		else
		{
//			_snprintf(szTempName, 32, "__reward__%02d", (int)d_ChildVector.size());
			_snprintf(szTempName, 32, "__reward__%02d", (int)ActionID);
		}

		pNewChild->d_Name = (utf8*)szTempName;
		pNewChild->d_ReArrange = bReArrange;
		pNewChild->d_Radio = bRadio;
		pNewChild->d_ActionButton = static_cast<FalagardActionButton*>(WindowManager::getSingleton().createWindow((utf8*)"WoWLook/ActionButton", getName() + szTempName));
		pNewChild->d_ActionButton->setLogicItemData((void*)(__w64 int)ActionID);
		pNewChild->d_ActionButton->show();
		pNewChild->d_ActionButton->setProperty("Empty", "False");
		if(!strIconImage.empty())
		{
			pNewChild->d_ActionButton->setProperty("NormalImage", strIconImage);
		}

		//设为子窗口
		d_ParentWindow->addChildWindow(pNewChild->d_ActionButton);

		d_ChildVector.push_back(pNewChild);
//		if(bRadio) d_RadioChild.push_back(pNewChild);
		performWindowLayout();
		return pNewChild->d_ActionButton;
	}


	void FalagardComplexWindow::AddChildElement_HyperLink( String strText,	int nLinkID, bool bReArrange, FontBase* font)
	{
		ChildElement_HyperLink* pNewChild = new ChildElement_HyperLink;

		pNewChild->d_Type		= CT_HyperLink;
		pNewChild->d_Text		= strText;//(utf8*)"hyperlink";
		if(font) pNewChild->d_Font = font;
		else pNewChild->d_Font = getFont();

		char szTempName[32];
		int nIndex = 0;
/*
		for(int i=0; i<(int)d_ChildVector.size(); i++)
		{
			if(d_ChildVector[i]->d_Name.substr(0, 10) == (utf8*)"__option__")
			{
				nIndex++;
			}
		}
*/
		_snprintf(szTempName, 32, "%d", nLinkID);

		pNewChild->d_Name = (utf8*)szTempName;
		pNewChild->d_ReArrange = bReArrange;
		pNewChild->d_Button = static_cast<FalagardButton*>(WindowManager::getSingleton().createWindow((utf8*)"WoWLook/Button_1", getName() + szTempName));
		pNewChild->d_Button->setText(strText);
		pNewChild->d_Button->show();
		pNewChild->d_Button->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&FalagardComplexWindow::handleLinkClicked, this));
//		pNewChild->d_State = atoi(strExtra2.c_str());


		//设为子窗口
		d_ParentWindow->addChildWindow(pNewChild->d_Button);

		d_ChildVector.push_back(pNewChild);
//		if(bRadio) d_RadioChild.push_back(pNewChild);
		performWindowLayout();
		return ;
	}


	void FalagardComplexWindow::AddChildElement_Money(int money, FontBase* font)

	{
		ChildElement_Money* pNewChild = new ChildElement_Money;

		pNewChild->d_Type = CT_Money;
		pNewChild->d_Name = (utf8*)"money";
		if(font) pNewChild->d_Font = font;
		else pNewChild->d_Font = getFont();

		pNewChild->d_Money = static_cast<FalagardMoney*>(WindowManager::getSingleton().createWindow((utf8*)"WoWLook/Money"));
		pNewChild->d_Money->setMoneyNumber(money);
		pNewChild->d_Money->show();

		//设为子窗口
		d_ParentWindow->addChildWindow(pNewChild->d_Money);

		d_ChildVector.push_back(pNewChild);
		performWindowLayout();

	}

	void FalagardComplexWindow::performWindowLayout(void)
	{
        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
        // get Icon area for Item
//        const Rect iconItemArea(wlf.getNamedArea("ItemIconSize").getArea().getPixelRect(*this));

		Rect absarea(getTextRenderArea());

		float fHeightNow = 0.0f;
		float fItemPosition = 0.0f;
		float fTextPosition = 0.0f;
		for(int i=0; i<(int)d_ChildVector.size(); i++)
		{
			ChildElement* pElement = d_ChildVector[i];
			const FontBase* font = pElement->d_Font;

			switch(pElement->d_Type)
			{
			case CT_Text:
				{
					fItemPosition = 0.0f;
					ChildElement_Text* pTextElement = (ChildElement_Text*)pElement;

					float textHeight = font->getFormattedSize(pTextElement->d_Text, absarea, (TextFormatting)pTextElement->d_Typeset).d_height;

//					AxTrace(0, 2, "ComplexWindowCalText:%f, %f[%f]", absarea.getWidth(), absarea.getHeight(), textHeight);

					pTextElement->d_rectClient.setPosition(Point(0.0f, fHeightNow));
					pTextElement->d_rectClient.setSize(Size(absarea.getWidth(), textHeight));

//					fTextPosition += absarea.getWidth();
					fHeightNow += textHeight * 1.2f;
				}
				break;

			case CT_Option:
				{
					fItemPosition = 0.0f;
					fTextPosition = 0.0f;
					ChildElement_Option* pOptionElement = (ChildElement_Option*)pElement;
					if(i > 0)
					{
						ChildElement* pElement_Before = d_ChildVector[i-1];

						if( pElement_Before && pElement_Before->d_Type != CT_Option )
							fHeightNow += font->getLineSpacing() * 2.0f;
					}

					float textHeight = font->getLineSpacing()*1.7f;

					pOptionElement->d_rectClient.setPosition(Point(0.0f, fHeightNow));
					pOptionElement->d_rectClient.setSize(Size(absarea.getWidth(), textHeight));

					pOptionElement->d_Button->setPosition(Absolute, Point(0.0f, fHeightNow-d_vertScrollbar->getScrollPosition()));
					pOptionElement->d_Button->setSize(Absolute, Size(absarea.getWidth(), textHeight));

					fHeightNow += (int)(textHeight * 1.08);
				}
				break;
			case CT_HyperLink:
				{
					/*
					fItemPosition = 0.0f;
					ChildElement_HyperLink*	pLinkElement = (ChildElement_HyperLink*)pElement;
					ChildElement* pElement_Before = d_ChildVector[i-1];
					
					float textHeight = font->getFormattedSize(pLinkElement->d_Text, absarea, (TextFormatting)WordWrapLeftAligned).d_height;

					float textWidth  = font->getFormattedSize(pLinkElement->d_Text, absarea, (TextFormatting)WordWrapLeftAligned).d_width;

//					if(pElement_Before->d_Type != CT_Item)
//						fHeightNow += d_ActionButtonDistance.d_height  * absarea.getHeight();

//					if( pElement_Before->d_Type != CT_Text )
//						fHeightNow += textHeight;

					if(pLinkElement->d_ReArrange)
					{
						pLinkElement->d_rectClient.setPosition(Point(0.0f, fHeightNow));
						fTextPosition = 0.0f;
					}
					else
					{

						float temp = fTextPosition + textWidth;

						if( temp > absarea.getWidth() )
							fTextPosition = 0.0f;
						else if(fTextPosition != 0.0f)
						{
							fHeightNow -= textHeight;
						}
					}
/*
					pLinkElement->d_rectClient.setPosition(Point(fTextPosition, fHeightNow));
//					pItemElement->d_rectClient.setSize(Size(absarea.getWidth(), iconItemArea.getHeight()));
//					pItemElement->d_rectClient.setSize(Size(absarea.getWidth(), d_ActionButtonSize.d_height * absarea.getHeight() ) );
					pLinkElement->d_rectClient.setSize(Size(d_ActionButtonSize.d_width * absarea.getWidth(), d_ActionButtonSize.d_height * absarea.getHeight() ) );

					pItemElement->d_Button->setPosition(Absolute, Point(fItemPosition, fHeightNow-d_vertScrollbar->getScrollPosition()));
//					pItemElement->d_Button->setSize(Absolute, Size(iconItemArea.getWidth(), iconItemArea.getHeight()));
					pItemElement->d_Button->setSize(Relative, Size(d_ActionButtonSize.d_width, d_ActionButtonSize.d_height) );

//					fHeightNow += iconItemArea.getHeight();
					fHeightNow += ( ( d_ActionButtonSize.d_height  + d_ActionButtonDistance.d_height ) * absarea.getHeight() );
					fItemPosition += ( ( d_ActionButtonSize.d_width  + d_ActionButtonDistance.d_width ) * absarea.getWidth() );
					//如果已经是最后一个元素了，下一行就别空那么多了。
					if(i == (int)d_ChildVector.size() )
						fHeightNow -= (d_ActionButtonDistance.d_height  * absarea.getHeight());
*/
				}
				break;
			case CT_Item:
				{
					fTextPosition = 0.0f;
					ChildElement_Item*	pItemElement = (ChildElement_Item*)pElement;
					if(i > 0)
					{
						ChildElement* pElement_Before = d_ChildVector[i-1];

	//					if(pElement_Before->d_Type != CT_Item)
	//						fHeightNow += d_ActionButtonDistance.d_height  * absarea.getHeight();
						if(pElement_Before && pElement_Before->d_Type != CT_Item && pElement_Before->d_Type != CT_Action)
							fHeightNow += d_ActionButtonDistance.d_height  * absarea.getHeight();
					}

					if(pItemElement->d_ReArrange)
					{
						pItemElement->d_rectClient.setPosition(Point(0.0f, fHeightNow));
						fItemPosition = 0.0f;
					}
					else
					{

						float temp = fItemPosition + ( ( d_ActionButtonSize.d_width  + d_ActionButtonDistance.d_width ) * absarea.getWidth() );

						if( temp > absarea.getWidth() )
							fItemPosition = 0.0f;
						else if(fItemPosition != 0.0f)
						{
							fHeightNow -= ( ( d_ActionButtonSize.d_height  + d_ActionButtonDistance.d_height ) * absarea.getHeight() );
						}
					}

					pItemElement->d_rectClient.setPosition(Point(fItemPosition, fHeightNow));
//					pItemElement->d_rectClient.setSize(Size(absarea.getWidth(), iconItemArea.getHeight()));
//					pItemElement->d_rectClient.setSize(Size(absarea.getWidth(), d_ActionButtonSize.d_height * absarea.getHeight() ) );
					pItemElement->d_rectClient.setSize(Size(d_ActionButtonSize.d_width * absarea.getWidth(), d_ActionButtonSize.d_height * absarea.getHeight() ) );

					pItemElement->d_Button->setPosition(Absolute, Point(fItemPosition, fHeightNow-d_vertScrollbar->getScrollPosition()));
//					pItemElement->d_Button->setSize(Absolute, Size(iconItemArea.getWidth(), iconItemArea.getHeight()));
					pItemElement->d_Button->setSize(Relative, Size(d_ActionButtonSize.d_width, d_ActionButtonSize.d_height) );

//					fHeightNow += iconItemArea.getHeight();
					fHeightNow += ( ( d_ActionButtonSize.d_height  + d_ActionButtonDistance.d_height ) * absarea.getHeight() );
					fItemPosition += ( ( d_ActionButtonSize.d_width  + d_ActionButtonDistance.d_width ) * absarea.getWidth() );
					//如果已经是最后一个元素了，下一行就别空那么多了。
					if(i == (int)d_ChildVector.size() )
						fHeightNow -= (d_ActionButtonDistance.d_height  * absarea.getHeight());
				}
				break;

			case CT_Action:
				{
					fTextPosition = 0.0f;
					ChildElement_Action*	pItemElement = (ChildElement_Action*)pElement;

					if(i > 0)
					{
						ChildElement* pElement_Before = d_ChildVector[i-1];

						if(pElement_Before->d_Type != CT_Item && pElement_Before->d_Type != CT_Action)
							fHeightNow += d_ActionButtonDistance.d_height  * absarea.getHeight();
					}

					if(pItemElement->d_ReArrange)
					{
						pItemElement->d_rectClient.setPosition(Point(0.0f, fHeightNow));
						fItemPosition = 0.0f;
					}
					else
					{

						float temp = fItemPosition + ( ( d_ActionButtonSize.d_width  + d_ActionButtonDistance.d_width ) * absarea.getWidth() );

						if( temp > absarea.getWidth() )
							fItemPosition = 0.0f;
						else if(fItemPosition != 0.0f)
						{
							fHeightNow -= ( ( d_ActionButtonSize.d_height  + d_ActionButtonDistance.d_height ) * absarea.getHeight() );
						}
					}

					pItemElement->d_rectClient.setPosition(Point(fItemPosition, fHeightNow));
//					pItemElement->d_rectClient.setSize(Size(absarea.getWidth(), iconItemArea.getHeight()));
//					pItemElement->d_rectClient.setSize(Size(absarea.getWidth(), d_ActionButtonSize.d_height * absarea.getHeight() ) );
					pItemElement->d_rectClient.setSize(Size(d_ActionButtonSize.d_width * absarea.getWidth(), d_ActionButtonSize.d_height * absarea.getHeight() ) );

					pItemElement->d_ActionButton->setPosition(Absolute, Point(fItemPosition, fHeightNow-d_vertScrollbar->getScrollPosition()));
//					pItemElement->d_Button->setSize(Absolute, Size(iconItemArea.getWidth(), iconItemArea.getHeight()));
					pItemElement->d_ActionButton->setSize(Relative, Size(d_ActionButtonSize.d_width, d_ActionButtonSize.d_height) );

//					fHeightNow += iconItemArea.getHeight();
					fHeightNow += ( ( d_ActionButtonSize.d_height  + d_ActionButtonDistance.d_height ) * absarea.getHeight() );
					fItemPosition += ( ( d_ActionButtonSize.d_width  + d_ActionButtonDistance.d_width ) * absarea.getWidth() );
					//如果已经是最后一个元素了，下一行就别空那么多了。
					if(i == (int)d_ChildVector.size() - 1 )
						fHeightNow -= (d_ActionButtonDistance.d_height  * absarea.getHeight());
				}
				break;
			case CT_Money:
				{
					fTextPosition = 0.0f;
					fItemPosition = 0.0f;
					ChildElement_Money* pMoneyElement = (ChildElement_Money*)pElement;

					float textHeight = font->getLineSpacing() * 1.4f;

					pMoneyElement->d_rectClient.setPosition(Point(0.0f, fHeightNow));
					pMoneyElement->d_rectClient.setSize(Size(absarea.getWidth(), textHeight));

					pMoneyElement->d_Money->setPosition(Absolute, Point(0.0f, fHeightNow-d_vertScrollbar->getScrollPosition()));
					pMoneyElement->d_Money->setSize(Absolute, Size(absarea.getWidth(), textHeight));

					fHeightNow += textHeight;
				}
				break;
			}
		}

		d_totalHeight = fHeightNow;

		configureScrollbars();
	}

	void FalagardComplexWindow::ClearAllElement(void)
	{
		_destroyAllElements();

		performWindowLayout();
		requestRedraw();
		d_ParentWindow->requestRedraw();
	}

	void FalagardComplexWindow::_destroyAllElements(void)
	{
		for(int i=0; i<(int)d_ChildVector.size(); i++)
		{
			ChildElement* pElement = d_ChildVector[i];
			switch(pElement->d_Type)
			{
			case CT_Text:
				break;

			case CT_Option:
				WindowManager::getSingleton().destroyWindow(((ChildElement_Option*)pElement)->d_Button);
				break;

			case CT_Item:
				WindowManager::getSingleton().destroyWindow(((ChildElement_Item*)pElement)->d_Button);
				break;

			case CT_Action:
				WindowManager::getSingleton().destroyWindow(((ChildElement_Action*)pElement)->d_ActionButton);
				break;

			case CT_Money:
				WindowManager::getSingleton().destroyWindow(((ChildElement_Money*)pElement)->d_Money);
				break;

			case CT_HyperLink:
				WindowManager::getSingleton().destroyWindow(((ChildElement_HyperLink*)pElement)->d_Button);
				break;
			}

			delete d_ChildVector[i];
			d_ChildVector[i] = NULL;
		}
		d_ChildVector.clear();
		d_RadioChild.clear();
		requestRedraw();
		d_ParentWindow->requestRedraw();
//		performWindowLayout();
	}

	void FalagardComplexWindow::configureScrollbars(void)
	{
		Scrollbar* vertScrollbar;

		try
		{
			vertScrollbar = static_cast<Scrollbar*>(WindowManager::getSingleton().getWindow(getName() + "__auto_vscrollbar__"));
		}
		catch (UnknownObjectException)
		{
			// no scrollbars?  Can't configure then!
			return;
		}

		Rect initialArea(getTextRenderArea());
		float totalHeight	= d_totalHeight;

		//
		// First show or hide the scroll bars as needed (or requested)
		//
		// show or hide vertical scroll bar as required (or as specified by option)
		if (totalHeight > getTextRenderArea().getHeight())
		{
			vertScrollbar->show();
		}
		else
		{
			vertScrollbar->hide();
		}

		//
		// Set up scroll bar values
		//
		Rect renderArea(getTextRenderArea());

		vertScrollbar->setDocumentSize(totalHeight);
		vertScrollbar->setPageSize(renderArea.getHeight());
		vertScrollbar->setStepSize(ceguimax(1.0f, renderArea.getHeight() / 10.0f));
		vertScrollbar->setScrollPosition(d_vertScrollbar->getScrollPosition());
		requestRedraw();
		d_ParentWindow->requestRedraw();
	}

	void FalagardComplexWindow::PageEnd()
	{
		Rect renderArea(getTextRenderArea());
		d_vertScrollbar->setScrollPosition( d_totalHeight - renderArea.getHeight() );
		configureScrollbars();
	}

	void FalagardComplexWindow::prepareBigchar(void)
	{
		Window::prepareBigchar();

		for(int i=0; i<(int)d_ChildVector.size(); i++)
		{
			ChildElement* pElement = d_ChildVector[i];
			if(CT_Text == pElement->d_Type && pElement->d_Font)
			{
				((FontBase*)(pElement->d_Font))->prepareString(((ChildElement_Text*)pElement)->d_Text);
			}
		}
	}

    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardComplexWindowFactory::createWindow(const String& name)
    {
        return new FalagardComplexWindow(d_type, name);
    }

    void FalagardComplexWindowFactory::destroyWindow(Window* window)
    {
		delete window;
    }
};