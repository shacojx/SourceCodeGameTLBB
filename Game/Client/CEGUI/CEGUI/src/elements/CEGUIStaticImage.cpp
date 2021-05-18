/************************************************************************
	filename: 	CEGUIStaticImage.cpp
	created:	4/6/2004
	author:		Paul D Turner
	
	purpose:	Implementation of the static image widget class.
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "elements/CEGUIStaticImage.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIWindowManager.h"

// Start of CEGUI namespace section
namespace CEGUI
{
const String StaticImage::EventNamespace("StaticImage");

/*************************************************************************
	Definitions of Properties for this class
*************************************************************************/
StaticImageProperties::Image			StaticImage::d_imageProperty;
StaticImageProperties::ImageColours		StaticImage::d_imageColoursProperty;
StaticImageProperties::VertFormatting	StaticImage::d_vertFormattingProperty;
StaticImageProperties::HorzFormatting	StaticImage::d_horzFormattingProperty;
StaticImageProperties::RightAnchor		StaticImage::d_rightAnchorProperty;
StaticImageProperties::BottomAnchor		StaticImage::d_bottomAnchorProperty;


/*************************************************************************
	Constructor for static image widgets.	
*************************************************************************/
StaticImage::StaticImage(const String& type, const String& name) :
	Static(type, name),
	d_imageCols(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF)
{
	addStaticImageProperties();

	// default to stretched image
	d_image.setHorzFormatting(RenderableImage::HorzStretched);
	d_image.setVertFormatting(RenderableImage::VertStretched);
}


/*************************************************************************
	Destructor for static image widgets.
*************************************************************************/
StaticImage::~StaticImage(void)
{
}


/*************************************************************************
	Set the Image object to be drawn by this widget.	
*************************************************************************/
void StaticImage::setImage(const Image* image)
{
	d_image.setImage(image);
	
	requestRedraw();
}
void StaticImage::setSize(MetricsMode mode, const Size& size)
{
	Window::setSize( mode, size );
	d_image.setSize(getUnclippedInnerRect().getSize());
}

/*************************************************************************
	Set the Image object to be drawn by this widget.	
*************************************************************************/
void StaticImage::setImage(const String& imageset, const String& image)
{
	setImage(&ImagesetManager::getSingleton().getImageset(imageset)->getImage(image));
}


/*************************************************************************
	Sets the colours to be applied when rendering the image.
*************************************************************************/
void StaticImage::setImageColours(const ColourRect& colours)
{
	d_imageCols = colours;
	updateRenderableImageColours();

	requestRedraw();
}


/*************************************************************************
	Sets the colours to be applied when rendering the image.	
*************************************************************************/
void StaticImage::setImageColours(const colour& top_left_colour, const colour& top_right_colour, const colour& bottom_left_colour, const colour& bottom_right_colour)
{
	d_imageCols.d_top_left		= top_left_colour;
	d_imageCols.d_top_right		= top_right_colour;
	d_imageCols.d_bottom_left	= bottom_left_colour;
	d_imageCols.d_bottom_right	= bottom_right_colour;
	updateRenderableImageColours();

	requestRedraw();
}


/*************************************************************************
	Set the formatting required for the image.	
*************************************************************************/
void StaticImage::setFormatting(HorzFormatting h_fmt, VertFormatting v_fmt)
{
	d_image.setHorzFormatting((RenderableImage::HorzFormatting)h_fmt);
	d_image.setVertFormatting((RenderableImage::VertFormatting)v_fmt);

	requestRedraw();
}


/*************************************************************************
	Set the formatting required for the image.	
*************************************************************************/
void StaticImage::setVerticalFormatting(VertFormatting v_fmt)
{
	d_image.setVertFormatting((RenderableImage::VertFormatting)v_fmt);
	
	requestRedraw();
}


/*************************************************************************
	Set the formatting required for the image.
*************************************************************************/
void StaticImage::setHorizontalFormatting(HorzFormatting h_fmt)
{
	d_image.setHorzFormatting((RenderableImage::HorzFormatting)h_fmt);

	requestRedraw();
}


/*************************************************************************
	update the internal RenderableImage with currently set colours and
	alpha settings.
*************************************************************************/
void StaticImage::updateRenderableImageColours(void)
{
	float alpha = getEffectiveAlpha();

	d_image.setColours(
		calculateModulatedAlphaColour(d_imageCols.d_top_left, alpha),
		calculateModulatedAlphaColour(d_imageCols.d_top_right, alpha),
		calculateModulatedAlphaColour(d_imageCols.d_bottom_left, alpha),
		calculateModulatedAlphaColour(d_imageCols.d_bottom_right, alpha)
	);

}


/*************************************************************************
	Perform the actual rendering for this Window.
*************************************************************************/
void StaticImage::populateRenderCache()
{
	// get whatever base class needs to render.
	Static::populateRenderCache();

	//
	// Render image.
	// TODO: Probably needs custom clipping to be added to RenderableImage.
	//
	updateRenderableImageColours();
	d_image.draw(d_renderCache);
}


/*************************************************************************
	Handler for when window is sized.
*************************************************************************/
void StaticImage::onParentSized(WindowEventArgs& e)
{
	// base class handling.
	Static::onParentSized( e );
	d_image.setSize(getUnclippedInnerRect().getSize());

	e.handled = true;
}

void StaticImage::onSized(WindowEventArgs& e)
{
	Static::onSized(e);
	d_image.setSize(getUnclippedInnerRect().getSize());
	e.handled = true;
}
/*************************************************************************
	Handler for when window alpha changes
*************************************************************************/
void StaticImage::onAlphaChanged(WindowEventArgs& e)
{
	// base class handling
	Static::onAlphaChanged(e);

	// Update image alpha values.
	updateRenderableImageColours();

	e.handled = true;
}


/*************************************************************************
	Handler for when frame state changes.
*************************************************************************/
void StaticImage::onStaticFrameChanged(WindowEventArgs& e)
{
	// base class processing
	Static::onStaticFrameChanged(e);

	// update the size and position of the image to reflect changes made
	// to the frame in the base class
	Rect absRect(getUnclippedPixelRect());
	Rect innerRect(getUnclippedInnerRect());
	d_image.setSize(innerRect.getSize());
	d_image.setPosition(Point(innerRect.d_left - absRect.d_left, innerRect.d_top - absRect.d_top));

	e.handled = true;
}

/*************************************************************************
	Called after size changed,to adjust right and bottom.
*************************************************************************/
void StaticImage::postSizeChanged(void)
{
	bool bRightAnchor = false;
	bool bBottomAnchor = false;

	float fAnchorWidth = 0.0f;
	float fAnchorHeight = 0.0f;

	if(!d_rightAnchor.empty())
	{
		Window* rightAnchor = WindowManager::getSingleton().getWindow(d_rightAnchor);
		if(rightAnchor)
		{
			bRightAnchor = true;
			fAnchorWidth = rightAnchor->getPixelRect().d_left-getPixelRect().d_left;
		}
	}

	if(!d_bottomAnchor.empty())
	{
		Window* bottomAnchor = WindowManager::getSingleton().getWindow(d_bottomAnchor);
		if(bottomAnchor)
		{
			bBottomAnchor = true;
			fAnchorHeight = bottomAnchor->getPixelRect().d_top-getPixelRect().d_top;
		}
	}

	if(bRightAnchor || bBottomAnchor)
	{
		float f1 = fAnchorWidth/getParentWidth();
		float f2 = f1*getParentWidth();

		if(fAnchorWidth != f2)
		{
			float fDif = fAnchorWidth-f2;
		}

		UVector2 usz(
			(bRightAnchor  ? UDim(0.0, fAnchorWidth)  : UDim(getRelativeWidth(), 0.0)),
			(bBottomAnchor ? UDim(0.0, fAnchorHeight) : UDim(getRelativeHeight(), 0.0)));

		setWindowSize(usz);
		d_image.setSize(getUnclippedInnerRect().getSize());
		requestRedraw();
	}

	Window::postSizeChanged();
}

/*************************************************************************
	Add properties for static image
*************************************************************************/
void StaticImage::addStaticImageProperties(void)
{
	CEGUI_START_ADD_STATICPROPERTY( StaticImage );
	   CEGUI_ADD_STATICPROPERTY( &d_imageProperty );
	   CEGUI_ADD_STATICPROPERTY( &d_imageColoursProperty );
	   CEGUI_ADD_STATICPROPERTY( &d_vertFormattingProperty );
	   CEGUI_ADD_STATICPROPERTY( &d_horzFormattingProperty );
	   CEGUI_ADD_STATICPROPERTY( &d_rightAnchorProperty );
	   CEGUI_ADD_STATICPROPERTY( &d_bottomAnchorProperty );
	CEGUI_END_ADD_STATICPROPERTY
}


} // End of  CEGUI namespace section
