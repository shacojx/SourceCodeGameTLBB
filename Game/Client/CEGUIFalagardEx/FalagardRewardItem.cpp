#include "FalagardRewardItem.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"


// Start of CEGUI namespace section
namespace CEGUI
{

	//================================================================================
	//
	// FalagardRewardItem
	//
	//================================================================================
    const utf8 FalagardRewardItem::WidgetTypeName[] = "Falagard/RewardItem";

    FalagardRewardItem::FalagardRewardItem(const String& type, const String& name) :
        FalagardButton(type, name)
		, d_Checked(false)
    {
    }

    FalagardRewardItem::~FalagardRewardItem()
    {
    }

	void FalagardRewardItem::initialise(void)
	{
		performWindowLayout();
	}

	void FalagardRewardItem::drawSelf( float z )
	{
		if (d_needsRedraw  )
		{
			d_renderCache.clearCachedImagery();
			d_needsRedraw = false;

			drawNormal(z);
		}
		
		if (isChecked()) // 如果按下了，就画按下的蒙字
		{
			drawPushed(z);
		}

		Window::drawSelf(z);
	}

	/*************************************************************************
		Handler called when size is changed
	*************************************************************************/
	void FalagardRewardItem::onSized(WindowEventArgs& e)
	{
		FalagardButton::onSized(e);

		performWindowLayout();
		requestRedraw();
	}

	void FalagardRewardItem::performWindowLayout(void)
	{
	}

    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardRewardItemWindowFactory::createWindow(const String& name)
    {
        return new FalagardRewardItem(d_type, name);
    }

    void FalagardRewardItemWindowFactory::destroyWindow(Window* window)
    {
		delete window;
    }
}