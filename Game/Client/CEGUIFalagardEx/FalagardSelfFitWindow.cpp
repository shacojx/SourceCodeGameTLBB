#include ".\falagardselffitwindow.h"
#include "ceguipropertyhelper.h"
#include "windows.h"
#include "ceguifontbase.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
namespace CEGUI
{
	const utf8 FalagardSelfFitWindow::WidgetTypeName[] = "Falagard/SelfFitWindow";

	FalagardSelfFitProperties::FitType			FalagardSelfFitWindow::d_FitTypeProperty;
	FalagardSelfFitProperties::FitPosition		FalagardSelfFitWindow::d_FitPosProperty;
	FalagardSelfFitProperties::MaxWidth			FalagardSelfFitWindow::d_MaxWidthProperty;
	FalagardSelfFitProperties::Life				FalagardSelfFitWindow::d_LifeProperty;
	FalagardSelfFitProperties::FadeOutTime		FalagardSelfFitWindow::d_FadeOutProperty;
	FalagardSelfFitProperties::FadeInTime		FalagardSelfFitWindow::d_FadeInProperty;
	FalagardSelfFitWindow::FalagardSelfFitWindow(const String& type, const String& name):
		FalagardSimpleText(type, name)
	{
		
		d_maxWidth = 200;
		m_bFadeMode = false;
		m_nFitType = SELF_FIT_CENTER;
		m_fFadeInTime = 1;
		m_fFadeOutTime = 2;
		m_fLife = 1;
		CEGUI_START_ADD_STATICPROPERTY( FalagardSelfFitWindow )
		   CEGUI_ADD_STATICPROPERTY( &d_FitTypeProperty );
           CEGUI_ADD_STATICPROPERTY( &d_FitPosProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_MaxWidthProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_LifeProperty );
           CEGUI_ADD_STATICPROPERTY( &d_FadeOutProperty );
		   CEGUI_ADD_STATICPROPERTY( &d_FadeInProperty );

		CEGUI_END_ADD_STATICPROPERTY
	}

	FalagardSelfFitWindow::~FalagardSelfFitWindow(void)
	{
	}

	void FalagardSelfFitWindow::updateSelf(float elapsed)
	{
		if( m_bFadeMode && m_fCurLife < m_fMaxLife && isVisible() )
		{
			m_fCurLife += elapsed;
			if( m_fCurLife > m_fMaxLife )
			{
				hide();
				return;
			}
			if( m_fCurLife < m_fFadeInTime )  
				setAlpha( m_fCurLife / m_fFadeInTime );
			else if( m_fCurLife < m_fFadeInTime + m_fLife )
				setAlpha( 1 );
			else
				setAlpha( ( m_fMaxLife - m_fCurLife ) / m_fFadeOutTime );
			requestRedraw();
			
		}
	}

	void FalagardSelfFitWindow::updateSelfSize(void)
	{
        const FontBase* fnt = getFont();

        if (fnt && !d_text.empty())
        {
            Rect area(System::getSingleton().getRenderer()->getRect());
			if(area.getWidth() > (float)d_maxWidth) area.d_right = area.d_left + d_maxWidth;

            // get required size of the tool tip according to the text extents.
			Size sz = fnt->getFormattedSize(d_text, area, CEGUI::WordWrapLeftAligned, d_fScaleX, d_fScaleY);
			sz.d_height += 2; //exten boarder
			sz.d_width += 2;
            setSize(Absolute, sz);
        }
        else
        {
            setSize(Absolute, Size(0,0));
        }
		requestRedraw();
	}

	void FalagardSelfFitWindow::updateSelfPosition()
	{
		// 根据绑定类型移动位置
		updateSelfSize();
		CEGUI::Point pos = m_ptCenter;
		switch( m_nFitType )
		{
		case SELF_FIT_LEFT:
			pos.d_y -= getAbsoluteHeight()/2.f;
			break;
		case SELF_FIT_RIGHT:
			pos.d_x -= getAbsoluteWidth();
			pos.d_y -= getAbsoluteHeight()/2;
			break;
		case SELF_FIT_TOP:
			pos.d_x -= getAbsoluteWidth() / 2;
			break;
		case SELF_FIT_BOTTOM:
			pos.d_x -= getAbsoluteWidth() / 2;
			pos.d_y -= getAbsoluteHeight();
			break;
		case SELF_FIT_LEFTTOP:
			break;
		case SELF_FIT_LEFTBOTTOM:
			pos.d_y -= getAbsoluteHeight();
			break;
		case SELF_FIT_RIGHTTOP:
			pos.d_x -= getAbsoluteWidth();
			break;
		case SELF_FIT_RIGHTBOTOM:
			pos.d_x -= getAbsoluteWidth();
			pos.d_y -= getAbsoluteHeight();
			break;
		case SELF_FIT_CENTER:
			pos.d_x -= getAbsoluteWidth() / 2;
			pos.d_y -= getAbsoluteHeight() / 2;
			break;
		default:
			break;
		}

		setPosition(Absolute, pos);
	}
	void FalagardSelfFitWindow::onParentSized(WindowEventArgs& e)
	{
		FalagardSimpleText::onParentSized(e);
		updateSelfSize();
	}

	// 重新计算边框尺寸
	void FalagardSelfFitWindow::setText( const String& text )
	{
		Window::setText(text);

		updateSelfSize();
		m_fCurLife = 0;
		m_fMaxLife = m_fLife + m_fFadeInTime + m_fFadeOutTime;
		if( text.empty() )	hide();
	}

	namespace FalagardSelfFitProperties
	{
		//////////////////////////////////////////////////////////////////////////
		String	MaxWidth::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::intToString(static_cast<const FalagardSelfFitWindow*>(receiver)->getMaxWidth());
		}
		void MaxWidth::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSelfFitWindow*>(receiver)->setMaxWidth( PropertyHelper::stringToInt( value ) );
		}

		//////////////////////////////////////////////////////////////////////////
		String	FitType::get(const PropertyReceiver* receiver) const
		{
			return "";
		}
		void	FitType::set(PropertyReceiver* receiver, const String& value)
		{
			int nType = 0;
			if( value == (utf8*)"Center" )
			{
				nType = FalagardSelfFitWindow::SELF_FIT_CENTER;
			}
			else if( value == (utf8*)"Left" )
			{
				nType = FalagardSelfFitWindow::SELF_FIT_LEFT;
			}
			else if( value == (utf8*)"Right" )
			{
				nType = FalagardSelfFitWindow::SELF_FIT_RIGHT;
			}
			else if( value == (utf8*)"Top" )
			{
				nType = FalagardSelfFitWindow::SELF_FIT_TOP;
			}
			else if( value == (utf8*)"Bottom" )
			{
				nType = FalagardSelfFitWindow::SELF_FIT_BOTTOM;
			}
			else if( value == (utf8*)"LeftTop" )
			{
				nType = FalagardSelfFitWindow::SELF_FIT_LEFTTOP;
			}
			else if( value == (utf8*)"RightBottm" )
			{
				nType = FalagardSelfFitWindow::SELF_FIT_RIGHTBOTOM;
			}
			else if( value == (utf8*)"LeftBottom" )
			{
				nType = FalagardSelfFitWindow::SELF_FIT_LEFTBOTTOM;
			}
			else if( value == (utf8*)"RightTop" )
			{
				nType = FalagardSelfFitWindow::SELF_FIT_RIGHTBOTOM;
			}

			static_cast<FalagardSelfFitWindow*>(receiver)->setFitType( nType );
		}

		/////////	/////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
		String	FitPosition::get(const PropertyReceiver* receiver) const
		{
			Point nPos = static_cast<const FalagardSelfFitWindow*>(receiver)->getCenter();
			return PropertyHelper::pointToString( nPos );
		}
		void	FitPosition::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSelfFitWindow*>(receiver)->setCenter( PropertyHelper::stringToPoint( value ) );
		}

/////////	/////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
		String	Life::get(const PropertyReceiver* receiver) const
		{
			return "";
		}
		void	Life::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSelfFitWindow*>(receiver)->setLife( PropertyHelper::stringToFloat( value ) );
		}

/////////	/////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
		String	FadeInTime::get(const PropertyReceiver* receiver) const
		{
			return "";
		}
		void	FadeInTime::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSelfFitWindow*>(receiver)->setFadeInTime( PropertyHelper::stringToFloat( value ) );
		}

/////////	/////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
		String	FadeOutTime::get(const PropertyReceiver* receiver) const
		{
			return "";
		}
		void	FadeOutTime::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardSelfFitWindow*>(receiver)->setFadeOutTime( PropertyHelper::stringToFloat( value ) );
		}

	}

	 //*************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardSelfFitFactory::createWindow(const String& name)
    {
        return new FalagardSelfFitWindow(d_type, name);
    }

    void FalagardSelfFitFactory::destroyWindow(Window* window)
    {
		delete window;
    }
};