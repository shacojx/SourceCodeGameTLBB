#include ".\FalagardBeHitBoard.h"
#include "CEGUIPropertyHelper.h"
namespace CEGUI
{
	const utf8 FalagardBeHitBoard::WidgetTypeName[] = "Falagard/BeHidBoard";


	FalagardBeHitBoardProperties::XPos  			FalagardBeHitBoard::d_XPosProperty;
    FalagardBeHitBoardProperties::YPos				FalagardBeHitBoard::d_YPosProperty;
	FalagardBeHitBoardProperties::XSpeed			FalagardBeHitBoard::d_XSpeedProperty;
    FalagardBeHitBoardProperties::YSpeed			FalagardBeHitBoard::d_YSpeedProperty;
	FalagardBeHitBoardProperties::XAcceleration		FalagardBeHitBoard::d_XAccelerationproperty;
	FalagardBeHitBoardProperties::YAcceleration		FalagardBeHitBoard::d_YAccelerationProperty;
    FalagardBeHitBoardProperties::MaxLife			FalagardBeHitBoard::d_MaxLifeProperty;
	FalagardBeHitBoardProperties::AlphaMode			FalagardBeHitBoard::d_AlphaModeProperty;
	FalagardBeHitBoardProperties::ColorType			FalagardBeHitBoard::d_ColorTypeProperty;
	FalagardBeHitBoardProperties::BackColorType		FalagardBeHitBoard::d_BackColorTypeProperty;
	FalagardBeHitBoardProperties::MoveMode			FalagardBeHitBoard::d_MoveModeProperty;


	FalagardBeHitBoardProperties::DoubleEndSize		FalagardBeHitBoard::d_DoubleEndSizeProperty;
	FalagardBeHitBoardProperties::NormalSize		FalagardBeHitBoard::d_NormalSizeProperty;
	FalagardBeHitBoardProperties::DoubleSize		FalagardBeHitBoard::d_DoubleSizeProperty;
	FalagardBeHitBoardProperties::DoubleTime		FalagardBeHitBoard::d_DoubleTimeProperty;

	FalagardBeHitBoardProperties::TempPoint			FalagardBeHitBoard::d_TempPointProperty;
	FalagardBeHitBoardProperties::UseTempPos		FalagardBeHitBoard::d_UseTempPosProperty;

	FalagardBeHitBoard::FalagardBeHitBoard(const String& type, const String& name) :
		FalagardSelfFitWindow(type, name)
	{
		CEGUI_START_ADD_STATICPROPERTY( FalagardBeHitBoard )
			CEGUI_ADD_STATICPROPERTY( &d_XPosProperty  );
			CEGUI_ADD_STATICPROPERTY( &d_YPosProperty  );
			CEGUI_ADD_STATICPROPERTY( &d_XSpeedProperty  );
			CEGUI_ADD_STATICPROPERTY( &d_YSpeedProperty  );
			CEGUI_ADD_STATICPROPERTY( &d_XAccelerationproperty  );
			CEGUI_ADD_STATICPROPERTY( &d_YAccelerationProperty  );
			CEGUI_ADD_STATICPROPERTY( &d_MaxLifeProperty  );
			CEGUI_ADD_STATICPROPERTY( &d_AlphaModeProperty  );
			CEGUI_ADD_STATICPROPERTY( &d_ColorTypeProperty  );
			CEGUI_ADD_STATICPROPERTY( &d_BackColorTypeProperty  );
			CEGUI_ADD_STATICPROPERTY( &d_NormalSizeProperty  );
			CEGUI_ADD_STATICPROPERTY( &d_DoubleSizeProperty  );
			CEGUI_ADD_STATICPROPERTY( &d_DoubleTimeProperty  );
			CEGUI_ADD_STATICPROPERTY( &d_DoubleEndSizeProperty  );
			CEGUI_ADD_STATICPROPERTY( &d_MoveModeProperty  );
			CEGUI_ADD_STATICPROPERTY( &d_TempPointProperty  );
			CEGUI_ADD_STATICPROPERTY( &d_UseTempPosProperty  );
		CEGUI_END_ADD_STATICPROPERTY

		for( int i = 0; i < MAX_COLOR_TYPE_NUMBER; i ++ )
		{
			m_ColorType[ i ] = colour( 1,1,1,1 );
			m_BackColorType[ i ] = colour( 1,1,1,1 );
		}
		m_fDoubleSize = 1;
		m_fNormalSize = 1;
		m_fDoubleTime = 1;
		m_fDoubleEndSize = 1;
		for( int i = 0; i < MAX_TEMP_TYPE_NUMBER; i ++ )
			m_UseTempPos[ i ] = false;
		for( int i = 0; i < MAX_MOVE_TYPE_NUMBER; i ++ )
			m_nMoveMode[ i ] = 0;
	}
	
	FalagardBeHitBoard::~FalagardBeHitBoard(void)
	{
	}
	void FalagardBeHitBoard::setTextScale( float fScaleX, float fScaleY)
	{
		FalagardSimpleText::setTextScale( Size( fScaleX, fScaleY ) );
	}
	float FalagardBeHitBoard::getTextScaleX()
	{
		return getTextScale().d_width;
	}
	void  FalagardBeHitBoard::SetTextColor( const colour& col )
	{
		setTextColor( col );
	}
	float FalagardBeHitBoard::getTextScaleY()
	{
		return getTextScale().d_height;
	}
	void FalagardBeHitBoard::setDisplayCenter( float fX, float fY )
	{
		setCenter( Point( fX, fY ) );
		updateSelfPosition();
	}
	namespace FalagardBeHitBoardProperties
	{

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
		String UseTempPos::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void UseTempPos::set(PropertyReceiver* receiver, const String& value)
		{
			int   nType = 0;
			int   nUse	= 0;
			sscanf( value.c_str(), "type:%d value:%d", &nType, &nUse);
			static_cast<FalagardBeHitBoard*>(receiver)->setTempPosUse( nType, nUse );
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
		String TempPoint::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void TempPoint::set(PropertyReceiver* receiver, const String& value)
		{
			int   nType = 0;
			float fX	= 0;
			float fY	= 0;
			sscanf( value.c_str(), "type:%d x:%f y:%f", &nType, &fX, &fY);
			static_cast<FalagardBeHitBoard*>(receiver)->setTempPos( nType, Point( fX, fY) );
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
		String DoubleTime::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void DoubleTime::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardBeHitBoard*>(receiver)->setDoubleTime( PropertyHelper::stringToFloat( value ) );//PropertyHelper::stringToInt(value));
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
		String DoubleEndSize::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void DoubleEndSize::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardBeHitBoard*>(receiver)->setDoubleEndSize( PropertyHelper::stringToFloat( value ) );//PropertyHelper::stringToInt(value));
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
		String DoubleSize::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void DoubleSize::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardBeHitBoard*>(receiver)->setDoubleSize( PropertyHelper::stringToFloat( value ) );//PropertyHelper::stringToInt(value));
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
		String NormalSize::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void NormalSize::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardBeHitBoard*>(receiver)->setNormalSize( PropertyHelper::stringToFloat( value ) );//PropertyHelper::stringToInt(value));
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
		String XPos::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void XPos::set(PropertyReceiver* receiver, const String& value)
		{
			int  nType = 0;
			int  nValue = 0;
			sscanf( value.c_str(), "type:%d value:%d", &nType, &nValue);
			static_cast<FalagardBeHitBoard*>(receiver)->setXPos( nType, nValue );//PropertyHelper::stringToInt(value));
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
		String MoveMode::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void MoveMode::set(PropertyReceiver* receiver, const String& value)
		{
			int  nType = 0;
			int  nValue = 0;
			sscanf( value.c_str(), "type:%d value:%d", &nType, &nValue);
			static_cast<FalagardBeHitBoard*>(receiver)->setMoveMode( nType, nValue );//PropertyHelper::stringToInt(value));
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
		String YPos::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void YPos::set(PropertyReceiver* receiver, const String& value)
		{
			int  nType = 0;
			int  nValue = 0;
			sscanf( value.c_str(), "type:%d value:%d", &nType, &nValue);
			static_cast<FalagardBeHitBoard*>(receiver)->setYPos( nType, nValue );//PropertyHelper::stringToInt(value));
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
		String XSpeed::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void XSpeed::set(PropertyReceiver* receiver, const String& value)
		{
			int  nType = 0;
			int  nValue = 0;
			sscanf( value.c_str(), "type:%d value:%d", &nType, &nValue);
			static_cast<FalagardBeHitBoard*>(receiver)->setXSpeed( nType, nValue );//PropertyHelper::stringToInt(value));
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
		String YSpeed::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void YSpeed::set(PropertyReceiver* receiver, const String& value)
		{
			int  nType = 0;
			int  nValue = 0;
			sscanf( value.c_str(), "type:%d value:%d", &nType, &nValue);
			static_cast<FalagardBeHitBoard*>(receiver)->setYSpeed(nType, nValue);
		}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
		String XAcceleration::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void XAcceleration::set(PropertyReceiver* receiver, const String& value)
		{
			int  nType = 0;
			int  nValue = 0;
			sscanf( value.c_str(), "type:%d value:%d", &nType, &nValue);
			static_cast<FalagardBeHitBoard*>(receiver)->setXAcceleration(nType, nValue);
		}
	//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
		String YAcceleration::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void YAcceleration::set(PropertyReceiver* receiver, const String& value)
		{
			int  nType = 0;
			int  nValue = 0;
			sscanf( value.c_str(), "type:%d value:%d", &nType, &nValue);
			static_cast<FalagardBeHitBoard*>(receiver)->setYAcceleration(nType, nValue);
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
		String MaxLife::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void MaxLife::set(PropertyReceiver* receiver, const String& value)
		{
			int  nType = 0;
			int  nValue = 0;
			sscanf( value.c_str(), "type:%d value:%d", &nType, &nValue);
			static_cast<FalagardBeHitBoard*>(receiver)->setLife(nType, nValue);
		}
	
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
		String AlphaMode::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void AlphaMode::set(PropertyReceiver* receiver, const String& value)
		{
			int  nType = 0;
			int  nValue = 0;
			sscanf( value.c_str(), "type:%d value:%d", &nType, &nValue);
			static_cast<FalagardBeHitBoard*>(receiver)->setAlpha(nType,  nValue!=0 );
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
		String ColorType::get(const PropertyReceiver* receiver) const
		{
			return "";//sPropertyHelper::colourToString(static_cast<const FalagardBeHitBoard*>(receiver)->getColorType( 0 ));
		}

		void ColorType::set(PropertyReceiver* receiver, const String& value)
		{
			argb_t val = 0xFF000000;
			int nType = 0;
			sscanf( value.c_str(), "type:%d color:%8X", &nType, &val );
			static_cast<FalagardBeHitBoard*>(receiver)->setColorType( nType, colour(val) );
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
		String BackColorType::get(const PropertyReceiver* receiver) const
		{
			return "";//sPropertyHelper::colourToString(static_cast<const FalagardBeHitBoard*>(receiver)->getColorType( 0 ));
		}

		void BackColorType::set(PropertyReceiver* receiver, const String& value)
		{
			argb_t val = 0xFF000000;
			int nType = 0;
			sscanf( value.c_str(), "type:%d color:%8X", &nType, &val );
			static_cast<FalagardBeHitBoard*>(receiver)->setBackColorType( nType, colour(val) );
		}
	}

//	    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardBeHitFactory::createWindow(const String& name)
    {
        return new FalagardBeHitBoard(d_type, name);
    }

    void FalagardBeHitFactory::destroyWindow(Window* window)
    {
		delete window;
    }
};