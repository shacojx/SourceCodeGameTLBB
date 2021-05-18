/****************************************\
*										*
*			  资源提供系统				*
*										*
\****************************************/
#pragma once
#include "TDNode.h"

//
//资源提供系统
//
class tResourceProvider : public tNode
{
public:
	virtual DWORD loadResource(LPCTSTR szFileName, LPSTR& szMemAddr, LPCTSTR szResourceGroup = NULL) = 0;
	virtual BOOL unloadResource(LPSTR szMemAddr, DWORD dwSize) = 0;
	WX_DECLARE_DYNAMIC(tResourceProvider);
};
