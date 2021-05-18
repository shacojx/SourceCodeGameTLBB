#ifndef _EditDObject_NT_H_
#define _EditDObject_NT_H_

#define MODELSYSTEM_STATIC_LINK
#include "Modelsystem\SkeletonMeshUtil.h"
#include "Modelsystem\SkeletonMesh.h"


////////////////////////////////////////////////////////////////////////////////////
//
// 编辑器使用.
//

// object 选中时使用的材质。
typedef			std::vector<Ogre::MaterialPtr> MATERIAL_SELECT_VECTOR;

// object 原来的材质.
typedef			std::vector<Ogre::MaterialPtr> MATERIAL_OLD_VECTOR;


namespace WX	{
	class System;
};

class CEditDobject_NT : public DObject_NT
{
public:
	CEditDobject_NT( WX::System *system );
	~CEditDobject_NT(void);

	////////////////////////////////////////////////////////////////////////////////////
	//
	// 编辑器使用.
	//
	

	// 
	public:

	// 设置模型的透明度
	// 0 -- 完全透明。
	// 1 -- 不透明。
	void SetTransparence(float Transparence = 1.0);

	// 设置模型的混合颜色
	void SetBlendColor(float fRed = 0, float fGreen = 0, float fBlue = 0);

	// object 选中时使用的材质。
	MATERIAL_SELECT_VECTOR m_materialSelVector;

	// object 原来的材质.
	MATERIAL_OLD_VECTOR    m_materilaOldVector;

	// 设置选中的外观颜色.
	void SetSelectLook(Ogre::ColourValue color);

	// 设置正常的外观颜色.
	void SetUnSelLook();

	// 清空材质 
	void ClearMaterial();

	//
	// 编辑器使用.
	//
	////////////////////////////////////////////////////////////////////////////////////

};

#endif
