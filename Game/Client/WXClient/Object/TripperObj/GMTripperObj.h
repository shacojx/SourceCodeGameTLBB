#pragma once
#include "..\Obj_Static.h"

/**
	Tripper Object
*/

class CTripperObject :	public CObject_Static
{
public:
	// 左键指令的分析
	virtual BOOL FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );
	// 右键指令的分析
	virtual BOOL FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );
protected:
	WX_DECLARE_DYNAMIC(CTripperObject);
};