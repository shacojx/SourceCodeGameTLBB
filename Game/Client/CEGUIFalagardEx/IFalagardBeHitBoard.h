#pragma once
#include "ceguivector.h"
namespace CEGUI
{
	const int MAX_COLOR_TYPE_NUMBER = 30;
	const int MAX_MOVE_TYPE_NUMBER = 20;
	const int MAX_TEMP_TYPE_NUMBER = 10; // ±∏”√Œª÷√
	class IFalagardBeHitBoard
	{
	public:
		/*!
		\brief
			Recaculate all subwindow
		*/
		virtual colour getColorType( int nType ) = 0;
		virtual colour getBackColorType( int nType ) = 0;
		virtual int  getMoveMode( int nType ) const = 0;
		virtual int  getLife(int nType)const = 0;
		virtual int  getYAcceleration(int nType)const =0;
		virtual int  getXAcceleration(int nType)const = 0;
		virtual int  getYSpeed(int nType)const = 0;
		virtual int  getXSpeed(int nType) const = 0;
		virtual int  getYPos(int nType)const = 0;
		virtual int  getXPos(int nType) const = 0;
		virtual bool getAlpha(int nType) const = 0;
		virtual void setDisplayCenter( float fX, float fY ) = 0;
		virtual void setTextScale( float fScaleX, float fScaleY) = 0;
		virtual float getTextScaleX() = 0;
		virtual float getTextScaleY() = 0;
		virtual void  SetTextColor( const colour& col ) = 0;
		
		virtual float getDoubleSize()const = 0;
		virtual float getNormalSize()const = 0;
		virtual float getDoubleTime()const = 0;
		virtual float getDoubleEndSize()const = 0;

		virtual CEGUI::Point getTempPos( int nType ) const = 0;
		virtual int getTempPosUse( int nType ) const = 0;
	};

}