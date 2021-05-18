#include "StdAfx.h"
#include "TDClass.h"
#include "TDNode.h"

tNode*	tClass::CreateObject(LPCTSTR szName)	//!< 用于生成一个实例的函数
{
	if(pfnCreateObject == NULL) return NULL;
	tNode *pObject = (*pfnCreateObject)();
	pObject->m_strName = szName;
	return pObject;
}