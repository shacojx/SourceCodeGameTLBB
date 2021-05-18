#include ".\falagardcheckbutton.h"
#include "ceguicolourrect.h"
#include "CEGUIPropertyHelper.h"
namespace CEGUI
{
	const utf8  FalagardCheckButton::WidgetTypeName[] = "Falagard/CheckBox";

    FalagardCheckButtonProperties::SelectImage FalagardCheckButton::d_selectImageProperty;
	FalagardCheckButtonProperties::Selected FalagardCheckButton::d_selectedProperty;
	FalagardCheckButtonProperties::CheckMode FalagardCheckButton::d_checkModeProperty;
	FalagardCheckButton::FalagardCheckButton( const String& type, const String& name ):
		FalagardButton(type, name)
	{
		CEGUI_START_ADD_STATICPROPERTY( FalagardCheckButton )
		   CEGUI_ADD_STATICPROPERTY( &d_selectImageProperty  );
		   CEGUI_ADD_STATICPROPERTY( &d_selectedProperty  );
		   CEGUI_ADD_STATICPROPERTY( &d_checkModeProperty  );
		CEGUI_END_ADD_STATICPROPERTY

		d_nCheckMode = 0;
	}

	FalagardCheckButton::~FalagardCheckButton(void)
	{
	}
	void	FalagardCheckButton::drawSelf(float z)
	{
		if( d_needsRedraw )
		{
			if( isCheck() && !isPushed())
			{
				d_renderCache.clearCachedImagery();
            // signal that we'll no loger need a redraw.
				d_needsRedraw = false;
				drawPushed( z );
			}
			else
				return FalagardButton::drawSelf( z );
		}
		Window::drawSelf(z);
	}
	void FalagardCheckButton::drawCheck()
	{
        ColourRect colours(d_selectImage.getColours());
        colours.setAlpha(getEffectiveAlpha());
        d_selectImage.setColours(colours);
        d_selectImage.draw(d_renderCache);
	}
	void	FalagardCheckButton::setSelectImage(const RenderableImage* image)
	{
		if (image)
		{
			d_selectImage = *image;
			d_selectImage.setRect(Rect(0, 0, getAbsoluteWidth(), getAbsoluteHeight()));
		}
		requestRedraw();
	}
	void FalagardCheckButton::setCustomImageryAutoSized(bool setting)
	{
		// if we are enabling auto-sizing, scale images for current size
		if (setting && setting != d_autoscaleImages)
		{
			Rect area(0, 0, getAbsoluteWidth(), getAbsoluteHeight());
			d_selectImage.setRect(area);
			requestRedraw();
		}

		PushButton::setCustomImageryAutoSized( setting );
	}
	void	FalagardCheckButton::onMouseButtonDown(MouseEventArgs& e)
	{
		PushButton::onMouseButtonDown(e);

		if (e.button == LeftButton)
		{
			if (captureInput())
			{
				if( d_nCheckMode == 1 )
				{
					if( d_bIsSelected ) // 如果已经被选中了
						return;
				}
				d_pushed = true;
				d_bIsSelected = !d_bIsSelected;
				updateInternalState(e.position);
				requestRedraw();
				
			}

			// event was handled by us.
			e.handled = true;
		}
	}
	void	FalagardCheckButton::setCheck( bool bCheck )
	{
		d_bIsSelected = bCheck;
		if( d_nCheckMode == 1 && bCheck ) // 如果是radio,就把别的同级的按钮全都弹起来
		{
			if( d_bIsSelected )
			{
				Window* pParent = getParent();
				uint child_count = pParent->getChildCount();
				for (uint i = 0; i < child_count; ++i)
				{
					Window* pChildWindow = pParent->getChildAtIdx( i );
					if(!(pChildWindow->testClassName("FalagardCheckButton"))) continue;

					FalagardCheckButton* pButton = (FalagardCheckButton*)pChildWindow;

					if(pButton != this )
						pButton->setCheck( false );
				}
			}
		}
		requestRedraw();
	}
	void FalagardCheckButton::setCheckMode( int nMode )
	{
		d_nCheckMode = nMode;
	}
	const int FalagardCheckButton::getCheckMode() const
	{
		return d_nCheckMode;
	}
	void	FalagardCheckButton::onMouseButtonUp(MouseEventArgs& e)
	{
		FalagardButton::onMouseButtonUp(e);
		if( d_nCheckMode == 1 ) // 如果是radio,就把别的同级的按钮全都弹起来
		{
			if( d_bIsSelected )
			{
				Window* pParent = getParent();
				uint child_count = pParent->getChildCount();
				for (uint i = 0; i < child_count; ++i)
				{
					Window* pChildWindow = pParent->getChildAtIdx( i );
					if(!(pChildWindow->testClassName("FalagardCheckButton"))) continue;

					FalagardCheckButton* pButton = (FalagardCheckButton*)pChildWindow;

					if(pButton != this)
						pButton->setCheck( false );
				}
			}
		}

	}

	//////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardCheckButtonFactory::createWindow(const String& name)
    {
        return new FalagardCheckButton(d_type, name);
    }

    void FalagardCheckButtonFactory::destroyWindow(Window* window)
    {
		delete window;
    }

	//////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Property 

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
	namespace FalagardCheckButtonProperties
	{
		String SelectImage::get(const PropertyReceiver* receiver) const
		{
			return "";//PropertyHelper::imageToString(static_cast<const FalagardCheckButton*>(receiver)->getSelectImage());
		}

		void SelectImage::set(PropertyReceiver* receiver, const String& value)
		{
			RenderableImage image;
			image.setImage(PropertyHelper::stringToImage(value));
			image.setHorzFormatting(RenderableImage::HorzStretched);
			image.setVertFormatting(RenderableImage::VertStretched);
			static_cast<FalagardCheckButton*>(receiver)->setSelectImage(&image);
		}
		String Selected::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::boolToString(static_cast<const FalagardCheckButton*>(receiver)->isCheck());
		}

		void Selected::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardCheckButton*>(receiver)->setCheck(PropertyHelper::stringToBool(value));
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// CheckMode 是radio 还是 check
		String CheckMode::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::intToString(static_cast<const FalagardCheckButton*>(receiver)->getCheckMode());
		}

		void CheckMode::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardCheckButton*>(receiver)->setCheckMode(PropertyHelper::stringToInt(value));
		}
	}
};