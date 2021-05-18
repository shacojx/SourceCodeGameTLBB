#include "FalagardImageListex.h"
#include "ceguistring.h"

namespace CEGUI
{
	const utf8  FalagardImageListBoxEx::WidgetTypeName[] = "Falagard/ImageListboxEx";
	
	FalagardImageListBoxExProperties::SetStateImage FalagardImageListBoxEx::d_SetStateImageProperty;
	FalagardImageListBoxExProperties::AddItem		FalagardImageListBoxEx::d_AddItemProperty;
	FalagardImageListBoxExProperties::SelectImage	FalagardImageListBoxEx::d_SelectImageProperty;
	FalagardImageListBoxExProperties::HorverImage	FalagardImageListBoxEx::d_HorverImageProperty;
	FalagardImageListBoxExProperties::NormalImage	FalagardImageListBoxEx::d_NormalImageProperty;

	FalagardImageListBoxEx::FalagardImageListBoxEx(const String& type, const String& name):
		FalagardListbox(type, name)
	{
		addProperty( &d_SetStateImageProperty );
		addProperty( &d_AddItemProperty );
		addProperty( &d_SelectImageProperty );
		addProperty( &d_HorverImageProperty );
		addProperty( &d_NormalImageProperty );
		d_SelectImage = NULL;
		d_pLastHorver = NULL;
	}

	FalagardImageListBoxEx::~FalagardImageListBoxEx(void)
	{
	}

	void FalagardImageListBoxEx::onMouseMove( MouseEventArgs& e )
	{
		
		Point localPos(screenToWindow(e.position));

		if (getMetricsMode() == Relative)
		{
			localPos = relativeToAbsolute(localPos);
		}

		FalagardImageListBoxItemEx* item = (FalagardImageListBoxItemEx*)getItemAtPoint(localPos);
		if (item != NULL && d_pLastHorver != item )
		{
			if( d_pLastHorver )
				d_pLastHorver->SetHorver( false );
			item->SetHorver();
			d_pLastHorver = item;
			requestRedraw();
		}
		Listbox::onMouseMove( e );
	}
	void FalagardImageListBoxEx::onMouseButtonDown(MouseEventArgs& e)
	{
		// base class processing
		Window::onMouseButtonDown(e);

		if (e.button == LeftButton)
		{
			bool modified = false;

			// clear old selections if no control key is pressed or if multi-select is off
			if (!(e.sysKeys & Control) || !d_multiselect)
			{
				modified = clearAllSelections_impl();
			}

			Point localPos(screenToWindow(e.position));

			if (getMetricsMode() == Relative)
			{
				localPos = relativeToAbsolute(localPos);
			}

			ListboxItem* item = getItemAtPoint(localPos);

			if (item != NULL)
			{
				modified = true;

				// select range or item, depending upon keys and last selected item
				if (((e.sysKeys & Shift) && (d_lastSelected != NULL)) && d_multiselect)
				{
					selectRange(getItemIndex(item), getItemIndex(d_lastSelected));
				}
				else
				{
					item->setSelected(item->isSelected() ^ true);
				}

				// update last selected item
				d_lastSelected = item->isSelected() ? item : NULL;
			}

			// fire event if needed
			WindowEventArgs args(this);
			onSelectionChanged(args);
			
			e.handled = true;
		}

	}


	void FalagardImageListBoxEx::setMaxStateNumber( int nIndex )
	{
		d_nStateNumber = nIndex;
	}
	
	void FalagardImageListBoxEx::setHorverImage( const Image* image )
	{
		d_HorverImage = image;
	}
	const Image* FalagardImageListBoxEx::getHorverImage() const
	{		
		return d_HorverImage;
	}	
	void FalagardImageListBoxEx::SetNormalImage( const Image* pImage )
	{
		d_NormalImage = pImage;
	}

	const Image* FalagardImageListBoxEx::getNormalImage() const
	{
		return d_NormalImage;
	}
	void FalagardImageListBoxEx::setSelectImage( const Image* image )
	{
		d_SelectImage = image;
	}
	const Image* FalagardImageListBoxEx::getSelectImage() const
	{
		return d_SelectImage;
	}


	const Image* FalagardImageListBoxEx::getStateImage( int nIndex ) const
	{	
		StateImage::const_iterator it;// =	d_stateImage.find( nIndex );
		int nCount = 0;
		for(  it = d_stateImage.begin(); it != d_stateImage.end(); it ++, nCount ++ )
		{
			if( nCount == nIndex )
			{
				return (*it);
			}
		}
		return NULL;
	}
	
	void FalagardImageListBoxEx::setStateImage(const Image* image, int nIndex )
	{
		if( nIndex > 0 )
		{
			StateImage::iterator it;
			int nCount = 0;
			for( it = d_stateImage.begin(); it != d_stateImage.end(); it ++, nCount ++ )
			{
				if( nCount == nIndex )
				{
					(*it) = (Image*) image;
					return;
				}
			}
		}
		d_stateImage.push_back( ( Image* )image );
	}

	//////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardImageListBoxExFactory::createWindow(const String& name)
    {
        return new FalagardImageListBoxEx(d_type, name);
    }

    void FalagardImageListBoxExFactory::destroyWindow(Window* window)
    {
		delete window;
    }

    //////////////////////////////////////////////////////////////////////////
	FalagardImageListBoxItemEx::FalagardImageListBoxItemEx(const String& text, uint item_id, void* item_data, bool disabled, bool auto_delete) :
		ListboxTextItem(text, item_id, item_data, disabled, auto_delete)
	{
		d_State = 0;
	}
	void FalagardImageListBoxItemEx::SetIconImage( const Image* pImage )
	{
		d_IconImage = pImage;
	}
	void FalagardImageListBoxItemEx::SetNormalImage( const Image* pImage )
	{
		d_Normal = pImage;
	}
	void FalagardImageListBoxItemEx::SetHorverImage( const Image* pImage )
	{
		d_HorverImage = pImage;
	}

	void FalagardImageListBoxItemEx::SetState( int nState )
	{
		d_State = nState;
	}

	void FalagardImageListBoxItemEx::draw(RenderCache& cache,const Rect& targetRect, float zBase,  float alpha, const Rect* clipper) const
	{
		

		if (d_selected && d_selectBrush != 0)
		{
			cache.cacheImage(*d_selectBrush, targetRect, zBase, getModulateAlphaColourRect(d_selectCols, alpha), clipper);
		}
		if (d_bHorver && d_HorverImage != 0 ) // 如果有高亮图片
		{
			cache.cacheImage(*d_HorverImage, targetRect, zBase, getModulateAlphaColourRect( colour( 0xffffffff ), alpha), clipper);
		}
		if( d_Normal )
			cache.cacheImage(*d_Normal, targetRect, zBase, getModulateAlphaColourRect( colour( 0xffffffff ), alpha), clipper);

		const Font* font = getFont();

		if (font)
		{
			Rect finalPos(targetRect);
			float fIconWidth = targetRect.getHeight() / d_IconImage->getHeight() * d_IconImage->getWidth();
			finalPos.d_top -= (font->getLineSpacing() - font->getBaseline()) * 0.5f;
			finalPos.d_left += fIconWidth;// (font->getLineSpacing() - font->getBaseline()) * 0.5f;
			cache.cacheText(d_itemText, font, LeftAligned, finalPos, zBase, getModulateAlphaColourRect(d_textCols, alpha), clipper);
			
			Rect rect( targetRect );
			rect.d_right = rect.d_left + fIconWidth;
			rect.d_bottom = rect.d_top + rect.getHeight();

			ColourRect  color( colour( 0xffffffff ) );
			
			cache.cacheImage( *d_IconImage, rect, zBase, getModulateAlphaColourRect(color, alpha), clipper );
			
		}
	}


}