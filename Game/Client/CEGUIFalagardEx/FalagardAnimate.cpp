#include "falagardanimate.h"
#include "ceguianimate.h"
#include "ceguianimatemanager.h"
#include "ogreroot.h"
#include "ogretimer.h"
namespace CEGUI
{
	const utf8 FalagardAnimate::WidgetTypeName[] = "Falagard/Animate";
	FalagardAnimateProperties::Animate FalagardAnimate::d_AnimateNameProperty;

	FalagardAnimate::FalagardAnimate(const String& type, const String& name) :
        FalagardButton(type, name)
	{
		CEGUI_START_ADD_STATICPROPERTY( FalagardAnimate )
		   CEGUI_ADD_STATICPROPERTY( &d_AnimateNameProperty  );
		CEGUI_END_ADD_STATICPROPERTY

		m_pAnimate = 0;
		m_bPlay = true;
		d_normalImage.setHorzFormatting( RenderableImage::HorzStretched );
		d_normalImage.setVertFormatting( RenderableImage::VertStretched );
	}

	FalagardAnimate::~FalagardAnimate(void)
	{
	}
	void FalagardAnimate::SetAnimate( const String& strAnimate )
	{
		if(AnimateManager::getSingleton().isAnimatePresent(strAnimate))
		{
			m_pAnimate = ( Animate*)AnimateManager::getSingleton().getAnimate( strAnimate );
			m_nAnimateStart = int( System::getSingleton().getCurTimeElapsed() * 1000.0f );
		}
	}
	void FalagardAnimate::drawNormal(float z)
	{
		if( m_bPlay )
		{
			ColourRect colours(d_normalImage.getColours());
			colours.setAlpha(getEffectiveAlpha() * m_fAlpha );
			d_normalImage.setColours(colours);
			d_normalImage.draw(d_renderCache);
		}
	}
	void	FalagardAnimate::drawSelf(float z)
	{
		if( d_needsRedraw )
		{
			d_renderCache.clearCachedImagery();
			d_needsRedraw = false;
			// normal 状态是作为永远显示模式出现的
			drawNormal( z );
			if (isDisabled(true) )
			{
				drawDisabled(z);
			}
			else
			{
				//If pushed or checked draw "Pushed" Layer
				if (isPushed() ) 
				{
					drawPushed(z);
				}
				//Draw "Hover" Layer
				if (isHovering() )
				{
					drawHover(z);
				}
			}
		}
		Window::drawSelf(z);
	}
	void	FalagardAnimate::Play( bool bPlay )
	{
		m_bPlay = bPlay;
		m_nAnimateStart = int( System::getSingleton().getCurTimeElapsed() * 1000.0f );
	}
	void	FalagardAnimate::updateSelf(float elapsed)
	{
		// 得到相应得播放了得时间
		if( m_pAnimate  )
		{
			if( m_bPlay )
			{
				int time_now = (int)(System::getSingleton().getCurTimeElapsed()*1000.0f);
				const Image* pFrame = m_pAnimate->getFrame( time_now - m_nAnimateStart, -1 );
				m_fAlpha = m_pAnimate->getFrameAlpha( time_now - m_nAnimateStart, -1 );
				d_normalImage.setImage( pFrame ) ;
			}
			else
			{
				d_normalImage.setImage( NULL ) ;
			}
			
			requestRedraw();
		}
		
	}

    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardAnimateFactory::createWindow(const String& name)
    {
        return new FalagardAnimate(d_type, name);
    }

    void FalagardAnimateFactory::destroyWindow(Window* window)
    {
//        if (window->getType() == d_type)
            delete window;
    }
}