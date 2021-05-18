#include "FalagardImageListitem.h"
#include "ceguistring.h"

namespace CEGUI
{
	const utf8  FalagardImageListBox::WidgetTypeName[] = "Falagard/ImageListbox";
	
	FalagardImageListBoxProperties::ImageNormal    FalagardImageListBox::d_ImageNormalProperty;
	FalagardImageListBoxProperties::ImageHorver    FalagardImageListBox::d_ImageHorverProperty;
	FalagardImageListBoxProperties::ImageChecked   FalagardImageListBox::d_ImageCheckedProperty;
	FalagardImageListBoxProperties::ImageDisable   FalagardImageListBox::d_ImageDisableProperty;
	FalagardImageListBoxProperties::AddItem			FalagardImageListBox::d_AddItemProperty;

	FalagardImageListBox::FalagardImageListBox(const String& type, const String& name):
		FalagardListbox(type, name)
	{
		addProperty( &d_ImageNormalProperty );
		addProperty( &d_ImageHorverProperty );
		addProperty( &d_ImageCheckedProperty );
		addProperty( &d_ImageDisableProperty );
		addProperty( &d_AddItemProperty );

		d_LastItem = NULL;
	}

	FalagardImageListBox::~FalagardImageListBox(void)
	{
	}

	void FalagardImageListBox::onMouseMove( MouseEventArgs& e )
	{
		Point localPos(screenToWindow(e.position));

		if (getMetricsMode() == Relative)
		{
			localPos = relativeToAbsolute(localPos);
		}

		FalagardImageListBoxItem* item = (FalagardImageListBoxItem*)getItemAtPoint(localPos);
		if (item != NULL && d_LastItem != item )
		{
			d_LastItem->SetState( LIST_ITEM_NORMAL );
			item->SetState( LIST_ITEM_HORVER );
			d_LastItem = item;
			requestRedraw();
		}
		Listbox::onMouseMove( e );
		
	}

	const Image* FalagardImageListBox::getNormalImage() const
	{	
		return d_normalImage;
	}
	const Image*  FalagardImageListBox::getHorverImage() const 
	{
		return d_hoverImage;
	}
	const Image*  FalagardImageListBox::getCheckedImage() const
	{
		return d_pushedImage;
	}
	const Image*  FalagardImageListBox::getDisabledImage() const
	{
		return d_disabledImage;
	}
	void FalagardImageListBox::setNormalImage(const Image* image )
	{
		d_normalImage = ( Image* )image;
	}
	void FalagardImageListBox::setHorverImage(const Image* image )
	{
		d_hoverImage = ( Image* )image;
	}
	void FalagardImageListBox::setCheckedImage(const Image* image )
	{
		d_pushedImage = ( Image* )image;
	}
	void FalagardImageListBox::setDisabledImage(const Image* image )
	{
		d_disabledImage = ( Image* )image;
	}

	//////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardImageListBoxFactory::createWindow(const String& name)
    {
        return new FalagardImageListBox(d_type, name);
    }

    void FalagardImageListBoxFactory::destroyWindow(Window* window)
    {
		delete window;
    }

    //////////////////////////////////////////////////////////////////////////
	FalagardImageListBoxItem::FalagardImageListBoxItem(const String& text, uint item_id, void* item_data, bool disabled, bool auto_delete) :
		ListboxTextItem(text, item_id, item_data, disabled, auto_delete)
	{
		d_State = LIST_ITEM_NORMAL;
	}
	
	void FalagardImageListBoxItem::SetNormalImage( Image* pImage )
	{
		d_Normal = pImage;
	}
	void FalagardImageListBoxItem::SetHorverImage( Image* pImage )
	{
		d_Horver = pImage;
	}
	void FalagardImageListBoxItem::SetCheckedImage( Image* pImage )
	{
		d_Clicked = pImage;
	}
	void FalagardImageListBoxItem::SetDisableImage( Image* pImage )
	{
		d_Disable = pImage;
	}
	void FalagardImageListBoxItem::SetState( int nState )
	{
		d_State = nState;
	}
	void FalagardImageListBoxItem::draw(RenderCache& cache,const Rect& targetRect, float zBase,  float alpha, const Rect* clipper) const
	{

		if (d_selected && d_selectBrush != 0)
		{
			cache.cacheImage(*d_selectBrush, targetRect, zBase, getModulateAlphaColourRect(d_selectCols, alpha), clipper);
		}

		const Font* font = getFont();

		if (font)
		{
			Rect finalPos(targetRect);
			finalPos.d_top -= (font->getLineSpacing() - font->getBaseline()) * 0.5f;
			finalPos.d_left += targetRect.getHeight();// (font->getLineSpacing() - font->getBaseline()) * 0.5f;
			cache.cacheText(d_itemText, font, LeftAligned, finalPos, zBase, getModulateAlphaColourRect(d_textCols, alpha), clipper);
			
			Rect rect( targetRect );
			rect.d_right = rect.d_left + rect.getHeight();
			rect.d_bottom = rect.d_top + rect.getHeight();

			ColourRect  color( colour( 0xffffffff ) );
			
			if( isSelected() )
			{
				cache.cacheImage( *d_Clicked, rect, zBase, getModulateAlphaColourRect(color, alpha), clipper );
				return;
			}
			else
			{
				if( d_State == LIST_ITEM_HORVER )
					cache.cacheImage( *d_Horver, rect, zBase, getModulateAlphaColourRect(color, alpha), clipper );
				else
					cache.cacheImage( *d_Normal, rect, zBase, getModulateAlphaColourRect(color, alpha), clipper );
			}
		}
	}


}