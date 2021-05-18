
#pragma once

#include "..\Obj_Phy.h"

/**
	不属于地表层的短期物体，例如闪电，魔法，射出的箭
	这类物体不受场景管理
*/
class CObject_Surface : public CObject_Phy
{
public:

public:
	CObject_Surface();
	virtual ~CObject_Surface();

protected:
	WX_DECLARE_DYNAMIC(CObject_Surface);
};
