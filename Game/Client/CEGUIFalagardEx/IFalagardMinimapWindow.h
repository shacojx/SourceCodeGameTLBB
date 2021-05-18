#pragma once

namespace CEGUI
{
	class IFalagardMinimapWindow
	{
	public:
		virtual void setMinimapData( int nType, float fX, float fY, String& szName  ) = 0;
		virtual void clearFlagList() = 0;
		virtual void setMinimapCenter( const Point& pos ) = 0;
		virtual void setPlayerDirection( int nDir ) = 0; 
	};
};