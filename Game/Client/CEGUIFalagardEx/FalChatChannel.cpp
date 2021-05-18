#include "FalChatChannel.h"
#include "CEGUIPropertyHelper.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"


// Start of CEGUI namespace section
namespace CEGUI
{
	//===================================================================================
	//
	// FalagardChatChannel
	//
	//===================================================================================
    const utf8  FalagardChatChannel::WidgetTypeName[] = "Falagard/ChatChannel";

	FalagardChatChannelProperties::ItemSize			FalagardChatChannel::d_itemSizeProperty;
	FalagardChatChannelProperties::AnchorPosition	FalagardChatChannel::d_anchorPositionProperty;
	FalagardChatChannelProperties::HoverChannelBkg	FalagardChatChannel::d_hoverChannelBkgProperty;
	FalagardChatChannelProperties::HoverChannel		FalagardChatChannel::d_hoverChannelProperty;
	FalagardChatChannelProperties::HoverChannelName	FalagardChatChannel::d_hoverChannelNameProperty;

	FalagardChatChannel::FalagardChatChannel(const String& type, const String& name) :
        Window(type, name),
		d_itemSize(0.0, 0.0),
		d_anchorPosition(0.0, 0.0),
		d_hoverChannelBkg(0),
		d_hoverChannel(-1),
		d_lastHoverChannel(-1),
		d_pushed(false)
    {
        addChatChannelProperties();

		setAlwaysOnTop(true);
    }

    FalagardChatChannel::~FalagardChatChannel()
    {
    }

	void FalagardChatChannel::addChatChannelProperties(void)
    {
		CEGUI_START_ADD_STATICPROPERTY( FalagardChatChannel )
		   CEGUI_ADD_STATICPROPERTY( &d_itemSizeProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_anchorPositionProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_hoverChannelBkgProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_hoverChannelProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_hoverChannelNameProperty );
		CEGUI_END_ADD_STATICPROPERTY
	}

	void FalagardChatChannel::onMouseMove(MouseEventArgs& e)
	{
		// base class processing
		Window::onMouseMove(e);

		updateInternalState(e.position);
		e.handled = true;
	}

	void FalagardChatChannel::updateInternalState(const Point& ptMouse)
	{
		Point pt(ptMouse);

		pt.d_x -= windowToScreenX(0);
		pt.d_y -= windowToScreenY(0);

		Rect absarea(getPixelRect());
		absarea.offset(Point(-absarea.d_left, -absarea.d_top));

		if(absarea.isPointInRect(pt))
		{
			float fItemHeight = absarea.getHeight()/(int)d_allChannel.size();

			d_hoverChannel = (int)(pt.d_y/fItemHeight);
			if(d_lastHoverChannel != d_hoverChannel)
			{
				requestRedraw();
				d_lastHoverChannel = d_hoverChannel;
			}
		}
	}

	void FalagardChatChannel::onMouseButtonDown(MouseEventArgs& e)
	{
		// default processing
		Window::onMouseButtonDown(e);

		if (e.button == LeftButton)
		{
			if (captureInput())
			{
				d_pushed = true;
				updateInternalState(e.position);
				requestRedraw();
			}

			// event was handled by us.
			e.handled = true;
		}
	}

	void FalagardChatChannel::onMouseButtonUp(MouseEventArgs& e)
	{
		// default processing
		Window::onMouseButtonUp(e);

		if (e.button == LeftButton)
		{
			releaseInput();

			// event was handled by us.
			e.handled = true;
		}
	}

	void FalagardChatChannel::onMouseLeaves(MouseEventArgs& e)
	{
		// base class processing
		Window::onMouseLeaves(e);

		d_hoverChannel = -1;
		d_lastHoverChannel = -1;
		requestRedraw();
	}

	void FalagardChatChannel::populateRenderCache(void)
	{
        // get WidgetLookFeel for the assigned look.
		const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
        // try and get imagery for our current state
		const StateImagery* imagery = &wlf.getStateImagery("Frame");
        // peform the rendering operation.
		imagery->render(*this);

		const Rect rectIconSize(wlf.getNamedArea("IconSize").getArea().getPixelRect(*this));

		const FontBase* font = getFont();

		Rect absarea(getPixelRect());
		absarea.offset(Point(-absarea.d_left, -absarea.d_top));
		ColourRect final_cols(colour(1.0f, 1.0f, 1.0f));

		float fItemHeight = absarea.getHeight()/(int)d_allChannel.size();

		for(int i=0; i<(int)d_allChannel.size(); i++)
		{
			ChannelItem& theChannel = d_allChannel[i];

			Rect rectIcon(rectIconSize);
			rectIcon.offset(Point(0, (fItemHeight-rectIconSize.getHeight())/2 + fItemHeight*i));
			d_renderCache.cacheImage(
				*theChannel.d_headIcon, rectIcon, 0.0f, final_cols);

			Rect rectText(absarea.d_left, fItemHeight*i,
				absarea.d_right, fItemHeight*(i+1));
			rectText.d_left += rectIcon.getWidth()+2;
			
			if(d_hoverChannelBkg && d_hoverChannel == i)
			{
				d_renderCache.cacheImage(
					*d_hoverChannelBkg, rectText, 0.0f, ColourRect(0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000));
			}

			d_renderCache.cacheText(this, 
				theChannel.d_strName, font, (TextFormatting)WordWrapLeftAligned, rectText, 0.0f, final_cols);
		}
	}

	void FalagardChatChannel::resizeSelf(void)
	{
		float fTotalHeight = d_itemSize.d_height * (int)d_allChannel.size();
		setSize(Absolute,Size(d_itemSize.d_width, fTotalHeight));

		float fParentHeight = d_parent->getPixelRect().getHeight();
		if(d_parent->getPixelRect().getWidth() < 1024) fParentHeight += 51;

		setPosition(Absolute,Point(d_anchorPosition.d_x, fParentHeight - d_anchorPosition.d_y - fTotalHeight));
	}

	void FalagardChatChannel::clearAllChannel(void)
	{
		d_allChannel.clear();
		resizeSelf();
	}

	void FalagardChatChannel::addChannel(const String& strType, const String& strIconName, const String& strName)
	{
		ChannelItem newChannel;

		newChannel.d_strType = strType;
		newChannel.d_headIcon = PropertyHelper::stringToImage(strIconName);
		if(!newChannel.d_headIcon) return;
		newChannel.d_strName = strName;

		d_allChannel.push_back(newChannel);
		resizeSelf();
	}

	String FalagardChatChannel::getHoverChannel(void) const
	{
		if(d_hoverChannel >= 0 && d_hoverChannel < (int)d_allChannel.size())
		{
			return d_allChannel[d_hoverChannel].d_strType;
		}
		return String("");
	}

	String FalagardChatChannel::getHoverChannelName(void) const
	{
		if(d_hoverChannel >= 0 && d_hoverChannel < (int)d_allChannel.size())
		{
			return d_allChannel[d_hoverChannel].d_strName;
		}
		return String("");
	}
    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
	Window* FalagardChatChannelFactory::createWindow(const String& name)
    {
        return new FalagardChatChannel(d_type, name);
    }

    void FalagardChatChannelFactory::destroyWindow(Window* window)
    {
		delete window;
    }
}