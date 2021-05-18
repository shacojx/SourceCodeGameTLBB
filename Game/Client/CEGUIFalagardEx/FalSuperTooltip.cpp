#include "FalSuperTooltip.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include <CEGUIWindowManager.h>
//#include "AxTrace.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	static std::vector< String > split_string( const String& str, const String& delims = "\t\n ", unsigned int maxSplits = 0);

	//===================================================================================
	//
	// FalagardSuperTooltip/ResizeText
	//
	//===================================================================================
	const utf8  FalagardSuperTooltip::ResizeText::WidgetTypeName[] = "Falagard/SuperTooltip/ResizeText";
	
	FalagardResizeTextProperties::TopFrame		FalagardSuperTooltip::ResizeText::d_topFrameProperty;
	FalagardResizeTextProperties::MiddleFrame	FalagardSuperTooltip::ResizeText::d_middleFrameProperty;
	FalagardResizeTextProperties::BottomFrame	FalagardSuperTooltip::ResizeText::d_bottomFrameProperty;
	FalagardResizeTextProperties::TextArea 		FalagardSuperTooltip::ResizeText::d_textAreaProperty;

	FalagardSuperTooltip::ResizeText::ResizeText(const String& type, const String& name) :
        FalagardSimpleText(type, name), 
		d_topFrame(0),
		d_middleFrame(0),
		d_bottomFrame(0)
    {
        addResizeTextProperties();
    }

	FalagardSuperTooltip::ResizeText::~ResizeText()
	{
	}

	float FalagardSuperTooltip::ResizeText::_resizeSelf(void)
	{
		const FontBase* font = getFont();
		// can't do anything without a font :)
		if (font == 0)
			return 0.0f;

		// get destination area for the text.
        Rect textArea(d_textArea.asAbsolute(getAbsoluteSize()));
		//AxTrace(0, 1, "Size=%.2f,%.2f", getPixelRect().getWidth(), getPixelRect().getHeight());

		float fTextHeight = font->getFormattedSize(d_text, textArea, (TextFormatting)d_horzFormatting).d_height 
			+ (FontBase::LineSpace);

		float fTopFrameHeight = d_topFrame ? d_topFrame->getSourceTextureArea().getHeight() : 0.f;
		float fMiddleFrameHeight = d_middleFrame ? d_middleFrame->getSourceTextureArea().getHeight() : 0.f;
		float fBottomFrameHeight = d_bottomFrame ? d_bottomFrame->getSourceTextureArea().getHeight() : 0.f;

        Rect wndArea(getAbsoluteRect());
        fTextHeight += wndArea.getHeight() - textArea.getHeight();

		return ceguimax(fTopFrameHeight+fMiddleFrameHeight+fBottomFrameHeight, fTextHeight);
	}
	
	void FalagardSuperTooltip::ResizeText::populateRenderCache()
	{
        Rect wndArea(getAbsoluteRect());

		float fTopFrameHeight = d_topFrame ? d_topFrame->getSourceTextureArea().getHeight() : 0.f;
		float fBottomFrameHeight = d_bottomFrame ? d_bottomFrame->getSourceTextureArea().getHeight() : 0.f;

		float fMiddleFrameHeight = wndArea.getHeight()-fTopFrameHeight-fBottomFrameHeight;

	    ColourRect final_cols(colour(1.0f, 1.0f, 1.0f));
		d_renderCache.cacheImage(*d_topFrame, Rect(0, 0, wndArea.getWidth(),fTopFrameHeight), 0.0f, final_cols);
		d_renderCache.cacheImage(*d_middleFrame, 
			Rect(0, fTopFrameHeight, wndArea.getWidth(), fTopFrameHeight+fMiddleFrameHeight), 0.0f, final_cols);
		d_renderCache.cacheImage(*d_bottomFrame, 
			Rect(0, fTopFrameHeight+fMiddleFrameHeight, wndArea.getWidth(), wndArea.getHeight()), 0.0f, final_cols);

		FalagardSimpleText::populateRenderCache();
		//d_topFrame->draw()
	}

	Rect FalagardSuperTooltip::ResizeText::getTextRenderArea(void) const
	{
        return Rect(d_textArea.asAbsolute(getAbsoluteSize()));
	}

	void FalagardSuperTooltip::ResizeText::addResizeTextProperties(void)
	{
		CEGUI_START_ADD_STATICPROPERTY( ResizeText )
		   CEGUI_ADD_STATICPROPERTY( &d_topFrameProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_middleFrameProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_bottomFrameProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_textAreaProperty );
		CEGUI_END_ADD_STATICPROPERTY
	}

    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************
        Factory Methods
    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
	Window* FalagardSuperTooltip::ResizeTextFactory::createWindow(const String& name)
    {
        return new ResizeText(d_type, name);
    }

	void FalagardSuperTooltip::ResizeTextFactory::destroyWindow(Window* window)
    {
		delete window;
    }

	//===================================================================================
	//
	// FalagardSuperTooltip
	//
	//===================================================================================
    const utf8  FalagardSuperTooltip::WidgetTypeName[] = "Falagard/SuperTooltip";

	FalagardSuperTooltipProperties::PageElements	FalagardSuperTooltip::d_pageElementsProperty;

	FalagardSuperTooltip::FalagardSuperTooltip(const String& type, const String& name) :
        Window(type, name)
    {
        addSuperTooltipProperties();

        setAlwaysOnTop(true);
	}

    FalagardSuperTooltip::~FalagardSuperTooltip()
    {
    }

	void FalagardSuperTooltip::addSuperTooltipProperties(void)
    {
		CEGUI_START_ADD_STATICPROPERTY( FalagardSuperTooltip )
		   CEGUI_ADD_STATICPROPERTY( &d_pageElementsProperty );
		CEGUI_END_ADD_STATICPROPERTY
	}

	String FalagardSuperTooltip::getPageElementsName(void) const
	{
		String strOut;

		ElementsVector::const_iterator it;
		for(it=d_VectorElements.begin(); it!=d_VectorElements.end(); it++)
		{
			strOut += it->pWindow->getName();
			strOut += (utf8*)";";
		}

		return strOut;
	}
	
	void FalagardSuperTooltip::setPageElementsName(const String& strName)
	{
		//Split string 
		std::vector< String > vElementNameVector = split_string(strName, (utf8*)";", getChildCount());

		//Hide all sub window
		int nChildSize = (int)getChildCount();
		for(int i=0; i<nChildSize; i++)
		{
			d_children[i]->hide();
		}

		//Clear old elements
		d_VectorElements.clear();
		int nSize = (int)vElementNameVector.size();
		for(int i=0; i<nSize; i++)
		{
			//Only accept child window
			if(!isChild(vElementNameVector[i])) continue;

			//Get Child window
			Window* pChild = getChild(vElementNameVector[i]);
			pChild->show();

			Elements newElement;
			newElement.pWindow = pChild;
			//Dynamic size window (Resize Text)
			if(pChild->testClassName("FalagardSuperTooltip/ResizeText"))
			{
				newElement.bDynamicSize = true;
			}
			//Static size window
			else
			{
				newElement.bDynamicSize = false;
			}

			d_VectorElements.push_back(newElement);
		}
	}

	void FalagardSuperTooltip::onSized(WindowEventArgs& e)
	{
		Window::onSized( e );

		//resizeSelf();
	}

    void FalagardSuperTooltip::populateRenderCache()
    {
		//Ignore when child elements empty
		if(d_VectorElements.empty()) return;

		//Draw frame border!

        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
        // try and get imagery for our current state
        const StateImagery* imagery = &wlf.getStateImagery("Frame");
        // peform the rendering operation.
        imagery->render(*this);
    }
	
	void FalagardSuperTooltip::resizeSelf(void)
	{
		// get child count
		int nElementNum = (int)d_VectorElements.size();
		if(nElementNum == 0) return;

		Rect  rcAbsoluteRect = getAbsoluteRect(); 

		// The part height(Absolute).
		std::vector< float >	vPartHeight(nElementNum);
				
		//AxTrace(0, 1, "--------------");

		float  fAbsHeight = 0;
		for(int i=0; i<nElementNum; ++i)
		{
			Elements& element = d_VectorElements[i];

			if(element.bDynamicSize)
			{
				//Ignore empty resize text control.
				if(element.pWindow->getText().empty())
				{
					element.pWindow->setText((utf8*)" ");
				}

				vPartHeight[i] = ((ResizeText*)element.pWindow)->_resizeSelf();
				fAbsHeight += vPartHeight[i];
				//AxTrace(0, 0, "D[%d] = %f", i, vPartHeight[i]);
			}
			//Static part
			else
			{
				vPartHeight[i] = element.pWindow->getAbsoluteHeight();
				fAbsHeight += element.pWindow->getAbsoluteRect().getHeight();

				//AxTrace(0, 0, "S[%d] = %f", i, vPartHeight[i]);
			}
		}

		// get named area"DynamicPartBorderSize"
		/*
		|		It's fake!!
		|	Left	- LeftBorder Width
		|	Top		- TopBorder Height
		|	Width	- RightBorder Width
		|	Height	- BottomBorder Height
		|
		*/
        //const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
		//const Rect rectBorder(wlf.getNamedArea("DynamicPartBorderSize").getArea().getPixelRect(*this));

		//Add header and footer size
		//fAbsHeight += rectBorder.d_top + rectBorder.getHeight();

		setHeight(Absolute, fAbsHeight);

		//Resize and reposition all sub window.
		float fHeightStep = 0.0f; //rectBorder.d_top;
		for(int i=0; i<nElementNum; ++i)
		{
			Elements& element = d_VectorElements[i];

			//Ignore empty resize text control.
			if(element.bDynamicSize && element.pWindow->getText().empty()) continue;

			element.pWindow->setHeight(Absolute, vPartHeight[i]);
			element.pWindow->setPosition(Absolute, 
				Point(element.pWindow->getAbsoluteXPosition(), fHeightStep));

			fHeightStep += element.pWindow->getAbsoluteHeight();
		}
	}


    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardSuperTooltipFactory::createWindow(const String& name)
    {
        return new FalagardSuperTooltip(d_type, name);
    }

    void FalagardSuperTooltipFactory::destroyWindow(Window* window)
    {
		delete window;
    }

    //////////////////////////////////////////////////////////////////////////
	// SplitString
	//		Copy from OGRE
    //////////////////////////////////////////////////////////////////////////
    std::vector<String> split_string( const String& str, const String& delims, unsigned int maxSplits)
    {
        // static unsigned dl;
        std::vector<String> ret;
        unsigned int numSplits = 0;

        // Use STL methods 
        size_t start, pos;
        start = 0;
        do 
        {
            pos = str.find_first_of(delims, start);
            if (pos == start)
            {
                // Do nothing
                start = pos + 1;
            }
            else if (pos == String::npos || (maxSplits && numSplits == maxSplits))
            {
                // Copy the rest of the string
                ret.push_back( str.substr(start) );
                break;
            }
            else
            {
                // Copy up to delimiter
                ret.push_back( str.substr(start, pos - start) );
                start = pos + 1;
            }
            // parse up to next real data
            start = str.find_first_not_of(delims, start);
            ++numSplits;

        } while (pos != String::npos);



        return ret;
    }
}