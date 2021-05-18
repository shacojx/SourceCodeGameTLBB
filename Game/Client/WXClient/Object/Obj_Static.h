
#pragma once

#include "Obj_Map.h"


/**
	注册到固定网格内的物体不会移动的物体，例如石块，树木，建筑等
*/

class CObject_Static : public CObject_Map
{
public:

public:
	CObject_Static();
	virtual ~CObject_Static();

protected:

protected:
	WX_DECLARE_DYNAMIC(CObject_Static);
};
