#include "ResourceProvider.h"
#include "ResourceSystem/WXPakFileSystem.h"
#include <OgreResourceGroupManager.h>
#include <OgreStringVector.h>
#include <OgreException.h>

WX_IMPLEMENT_DYNAMIC(CResourceProvider, GETCLASS(tNode));

CResourceProvider::CResourceProvider(VOID)
{
}

CResourceProvider::~CResourceProvider(VOID)
{
}

DWORD CResourceProvider::loadResource(LPCTSTR szFileName, LPSTR& szMemAddr, LPCTSTR szResourceGroup)
{
	if( szFileName == NULL || strlen( szFileName ) == 0 ) return 0;

	Ogre::ResourceGroupManager& resManager = Ogre::ResourceGroupManager::getSingleton();
	const Ogre::StringVector& strGroupVec = resManager.getResourceGroups();
	for(int i=0; i<(int)strGroupVec.size(); i++)
	{
		if( szResourceGroup && strGroupVec[i] != szResourceGroup ) continue;
		try
		{
			Ogre::String grp = strGroupVec[i];
			if( resManager.resourceExists( grp, szFileName ) )
			{
				Ogre::DataStreamPtr stream = resManager.openResource( szFileName, grp, false );
				if( stream.get() && stream.get()->size() > 0 )
				{
					szMemAddr = new char[stream.get()->size()+1];
					stream.get()->read( szMemAddr, stream.get()->size() );
					szMemAddr[stream.get()->size()] = 0;
					return (DWORD)stream.get()->size();
				}
			}
		}
		catch( const Ogre::Exception& e )
		{
			return 0;
		}
	}
	return 0;
}

BOOL CResourceProvider::unloadResource( LPSTR szMemAddr, DWORD dwSize )
{
	if( szMemAddr ) delete [] szMemAddr;
	szMemAddr = NULL;
	return TRUE;
}
