/****************************************\
*										*
*			    资源提供系统			*
*										*
\****************************************/
#pragma once

#include "TDNode.h"


namespace Ogre
{
	class PakFileSystemArchive;
	class ResourceGroupManager;
	class OgreStringVector;
};

class CResourceProvider : public tNode
{
public:
	CResourceProvider(VOID);
	virtual ~CResourceProvider(VOID);
	virtual DWORD loadResource(LPCTSTR szFileName, LPSTR& szMemAddr, LPCTSTR szResourceGroup = NULL);
	virtual BOOL unloadResource(LPSTR szMemAddr, DWORD dwSize);
protected:
	WX_DECLARE_DYNAMIC(CResourceProvider);
};

