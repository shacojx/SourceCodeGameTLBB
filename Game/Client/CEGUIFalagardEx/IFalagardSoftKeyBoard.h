#pragma once
namespace CEGUI
{
	class Window;
	class IFalagardSoftKeyBoard
	{
	public:
		virtual void SetActiveEditBox( String& windowName ) = 0;
	};

};