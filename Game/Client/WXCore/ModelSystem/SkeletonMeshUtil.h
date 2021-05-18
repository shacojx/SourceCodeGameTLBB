#ifndef _SkeletonMeshUtil_h_
#define _SkeletonMeshUtil_h_

#include "ModelTypes.h"
#include <OgrePrerequisites.h>



typedef std::vector<Ogre::String>			StringList;


class DObject_NT;

class  MODEL_API	SkeletonMeshUtil
{

public:
	SkeletonMeshUtil(){};
	~SkeletonMeshUtil(){};

//资源管理方法
public: 
		//-----------------------------
		//Save 接口,文件类型	*.obj  
		//-----------------------------
		static	bool	Save(DObject_NT* pMesh,Ogre::String& StrFileName,bool isBinary = false);

		//----------------------------//
		//Load 接口
		//-----------------------------

		static  bool    Load(DObject_NT*			pMesh,
							const Ogre::String		strObjGroup,
							const Ogre::String&		strXmlFile,
							const StringList&		attribNames,
							const StringList&		attribValues);

		static  bool	LoadChild(DObject_NT*		pMesh,
							const Ogre::String		strObjGroup,
							const Ogre::String&		strXmlFile,
							const StringList&		attribNames,
							const StringList&		attribValues,
							Ogre::SkeletonInstance*		pParentSkeleton,
							Ogre::Bone*					pParentBone
							);
//内部 实现方法
private:
		
		//xml	加载处理
		static bool	LoadFromXml(DObject_NT* pMesh);
		//binary 加载处理
		static bool	LoadFrameBinary(DObject_NT* pMesh);
		//xml   存盘处理
		static bool	SaveToXml(DObject_NT* pMesh,Ogre::String StrFileName);
		//binary 存盘处理
		static bool	SaveToBinary(DObject_NT* pMesh);

private:
		static void	_LoadEntity(DObject_NT* pMesh);
		static void	_LoadChildEntity(DObject_NT* pMesh);
};
	

#endif