#pragma once

namespace CEGUI
{
	class IFalagardSceneMapWindow
	{
	public:
		virtual const Point GetMouseScenePos() const = 0;
		virtual void setMyselfDir( const int nDirect ) = 0;
		virtual void setMyselfPos( const Point& pos ) = 0;
		virtual void setMAXJuLi( const float Maxjuli ) = 0;
		virtual void addPlayerPos( const Point& pos, const String& name ) = 0;
		virtual void Clear() = 0;

		virtual void SetSceneSize( Size size ) = 0;
		virtual void SetSceneFileName( const String& str, const int nUserSharp ) = 0;
		virtual void SetZoomMode( int nMode ) = 0;

		virtual void setMinimapData( int nType, float fX, float fY, String& szName  ) = 0;

		virtual void setSceneMapCenter() = 0;

		
	};
};