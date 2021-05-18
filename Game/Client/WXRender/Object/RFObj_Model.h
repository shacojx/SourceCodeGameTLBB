/****************************************\
*										*
*	   CRFObj_Model						*
*										*
\****************************************/

#pragma once
#include "RFObj.h"

class CRFObj_Model : public CWXObj_Object
{
public:
	//得到类型
	virtual EntityType	GetType(VOID) const	{ return ETYPE_MODEL; }
	//设置是否可见
	virtual VOID		SetVisible(BOOL bVisible);

	//设置MeshFile
	virtual VOID		StaObj_SetMeshFile(LPCTSTR szMeshFile);

public:
	CRFObj_Model();
	virtual ~CRFObj_Model();
};
