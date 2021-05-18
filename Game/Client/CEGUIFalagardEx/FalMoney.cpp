
#include "FalMoney.h"
#include "CEGUIExceptions.h"
#include "CEGUIWindowManager.h"
#include "CEGUIPropertyHelper.h"

// Start of CEGUI namespace section
namespace CEGUI
{

	//===================================================================================
	//
	// FalagardMoney
	//
	//===================================================================================
    const utf8  FalagardMoney::WidgetTypeName[] = "Falagard/Money";

	FalagardMoneyProperties::HorzFormatting		FalagardMoney::d_horzFormattingProperty;
    FalagardMoneyProperties::MoneyNumber		FalagardMoney::d_moneyNumProperty;
    FalagardMoneyProperties::GoldIcon			FalagardMoney::d_goldIconProperty;
    FalagardMoneyProperties::SilverIcon			FalagardMoney::d_silverIconProperty;
    FalagardMoneyProperties::CopperIcon			FalagardMoney::d_copperIconProperty;

    FalagardMoney::FalagardMoney(const String& type, const String& name) :
        Window(type, name),
		d_horzFormatting(LeftAligned),
		d_MoneyNum(0),
		d_imageGoldIcon(0),
		d_imageSilverIcon(0),
		d_imageCopperIcon(0)
    {
        addMoneyProperties();
    }

    FalagardMoney::~FalagardMoney()
    {
    }

	void FalagardMoney::initialise(void)
	{

	}

	void FalagardMoney::addMoneyProperties(void)
    {
		CEGUI_START_ADD_STATICPROPERTY( FalagardMoney )
		   CEGUI_ADD_STATICPROPERTY( &d_horzFormattingProperty );
           CEGUI_ADD_STATICPROPERTY( &d_moneyNumProperty );
           CEGUI_ADD_STATICPROPERTY( &d_goldIconProperty );
           CEGUI_ADD_STATICPROPERTY( &d_silverIconProperty );
           CEGUI_ADD_STATICPROPERTY( &d_copperIconProperty );
		CEGUI_END_ADD_STATICPROPERTY
	}

	//Set MoneyNumber property
	void FalagardMoney::setMoneyNumber(int moneyNumber)
    {
		d_MoneyNum = moneyNumber;
		requestRedraw();
    }

	/*************************************************************************
		Set the formatting required for the text.	
	*************************************************************************/
	void FalagardMoney::setHorizontalFormatting(HorzFormatting h_fmt)
	{
		d_horzFormatting = h_fmt;
		requestRedraw();
	}

	/*************************************************************************
		Perform the actual rendering for this Window.
	*************************************************************************/
	void FalagardMoney::populateRenderCache()
	{
		const int GAP_TEXT_ICON = 2;

		//Calculate ...
		int nCopperNum	= d_MoneyNum%100;
		int nSilverNum	= (d_MoneyNum%10000)/100;
		int nGoldNum	= d_MoneyNum/10000;

		// get destination area for the text.
		Rect areaTotal(0, 0, getPixelRect().getWidth(), getPixelRect().getHeight());
		Rect areaDisplay( 0,0,0,0 );
		char szMoney[32];
		const FontBase* pFont = getFont();
	    ColourRect final_cols(colour(1.0f, 1.0f, 1.0f));
		Size moneySize[ 3 ];

		if( nGoldNum > 0 ) // 说明有金币
		{
			_snprintf(szMoney, 32, "%d", nGoldNum);
			moneySize[ 0 ] = pFont->getFormattedSize((utf8*)szMoney, areaTotal, (TextFormatting)d_horzFormatting, 1 );
			areaDisplay.d_right += moneySize[ 0 ].d_width;
			areaDisplay.d_right += d_imageGoldIcon->getWidth();
			if( moneySize[ 0 ].d_height > areaDisplay.getHeight() )
				areaDisplay.d_bottom = moneySize[ 0 ].d_height;
			if( d_imageGoldIcon->getHeight() > areaDisplay.getHeight() )
				areaDisplay.d_bottom = d_imageGoldIcon->getHeight();

			areaDisplay.d_right += GAP_TEXT_ICON;
		}
		if( nGoldNum > 0 || nSilverNum > 0 ) // 说明有银币
		{
			_snprintf(szMoney, 32, "%d", nSilverNum);
			moneySize[ 1 ] = pFont->getFormattedSize((utf8*)szMoney, areaTotal, (TextFormatting)d_horzFormatting, 1 );
			areaDisplay.d_right += moneySize[ 1 ].d_width;
			areaDisplay.d_right += d_imageSilverIcon->getWidth();
			if( moneySize[ 1 ].d_height > areaDisplay.getHeight() )
				areaDisplay.d_bottom = moneySize[ 1 ].d_height;
			if( d_imageSilverIcon->getHeight() > areaDisplay.getHeight() )
				areaDisplay.d_bottom = d_imageSilverIcon->getHeight();

			areaDisplay.d_right += GAP_TEXT_ICON;
		}
		{
			_snprintf(szMoney, 32, "%d", nCopperNum);
			moneySize[ 2 ] = pFont->getFormattedSize((utf8*)szMoney, areaTotal, (TextFormatting)d_horzFormatting, 1 );
			areaDisplay.d_right += moneySize[ 2 ].d_width;
			areaDisplay.d_right += d_imageCopperIcon->getWidth();
			if( moneySize[ 2 ].d_height > areaDisplay.getHeight() )
				areaDisplay.d_bottom = moneySize[ 2 ].d_height;
			if( d_imageCopperIcon->getHeight() > areaDisplay.getHeight() )
				areaDisplay.d_bottom = d_imageCopperIcon->getHeight();
		}

		// 走到这一步，已经得到了整个图片的范围 areaDisplay;
		Rect rectRender(areaDisplay);
		switch( d_horzFormatting )//== FalagardMoney::LeftAligned)
		{
		case FalagardMoney::LeftAligned:
			break;
		case FalagardMoney::RightAligned:
			rectRender.d_left += areaTotal.getWidth() - areaDisplay.getWidth();
			rectRender.d_right = rectRender.d_left + areaDisplay.getWidth(); 
			break;
		default:
			break;
		}
		float fLeft = rectRender.d_left;
		if(nGoldNum > 0)
		{
			_snprintf(szMoney, 32, "%d", nGoldNum);
	
			Rect areaText(fLeft, areaTotal.getHeight() - moneySize[ 0 ].d_height, fLeft + moneySize[ 0 ].d_width, areaTotal.getHeight());
			fLeft += moneySize[ 0 ].d_width;
			Rect aretIcon(fLeft, areaTotal.getHeight() - d_imageGoldIcon->getHeight(), areaText.d_right + d_imageGoldIcon->getWidth(),areaTotal.getHeight() );
			fLeft += d_imageGoldIcon->getWidth();
			d_renderCache.cacheText(this, 
				(utf8*)szMoney, pFont, (TextFormatting)RightAligned, areaText, 0.0f, final_cols);
			d_renderCache.cacheImage(*d_imageGoldIcon, aretIcon, 0.0f, final_cols);
		
			areaDisplay.d_right += GAP_TEXT_ICON;
		}
		if( nSilverNum > 0 || nGoldNum > 0 )
		{
			_snprintf(szMoney, 32, "%d", nSilverNum);
			Rect areaText(fLeft, areaTotal.getHeight() - moneySize[ 1 ].d_height, fLeft + moneySize[ 1 ].d_width, areaTotal.getHeight());
			fLeft += moneySize[ 1 ].d_width;
			Rect aretIcon(fLeft, areaTotal.getHeight() - d_imageGoldIcon->getHeight(), areaText.d_right + d_imageSilverIcon->getWidth(),areaTotal.getHeight() );
			fLeft += d_imageSilverIcon->getWidth();
			
			d_renderCache.cacheText(this, 
				(utf8*)szMoney, pFont, (TextFormatting)RightAligned, areaText, 0.0f, final_cols);
			d_renderCache.cacheImage(*d_imageSilverIcon, aretIcon, 0.0f, final_cols);
			fLeft = aretIcon.d_right;

			areaDisplay.d_right += GAP_TEXT_ICON;
		}
		{
			_snprintf(szMoney, 32, "%d", nCopperNum);
	
			Rect areaText(fLeft, areaTotal.getHeight() - moneySize[ 2 ].d_height, fLeft + moneySize[ 2 ].d_width, areaTotal.getHeight());
			fLeft += moneySize[ 2 ].d_width;
			Rect aretIcon(fLeft, areaTotal.getHeight() - d_imageGoldIcon->getHeight(), areaText.d_right + d_imageCopperIcon->getWidth(),areaTotal.getHeight() );
			fLeft += d_imageCopperIcon->getWidth();
			
			d_renderCache.cacheText(this, 
				(utf8*)szMoney, pFont, (TextFormatting)RightAligned, areaText, 0.0f, final_cols);
			d_renderCache.cacheImage(*d_imageCopperIcon, aretIcon, 0.0f, final_cols);
		}
	}

    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardMoneyFactory::createWindow(const String& name)
    {
        return new FalagardMoney(d_type, name);
    }

    void FalagardMoneyFactory::destroyWindow(Window* window)
    {
		delete window;
    }

}