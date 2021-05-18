#ifndef __SkeletonMeshEditable_H__
#define __SkeletonMeshEditable_H__

#include "SkeletonMesh.h"

class DObjectEditable : public DObject_NT
{
public:

	DObjectEditable( WX::System *system );

	~DObjectEditable();

	virtual bool Execute(Ogre::Real delta);

	unsigned short                  GetSkeletonAnimationCount(void);
	Ogre::Animation*				GetSkeletonAnimation( unsigned short index );

	bool                            IsAnimEditing(void)	{	return mAnimEditing;	}
	void                            SetAnimEditing(bool edit = true)
	{	
		mAnimEditing = edit;
	}

	void                            SetAnimTimePos(float sliderValue)	{	mAnimTimePos = sliderValue;	}
	float                           GetAnimTimePos(void)	{	return mAnimTimePos;	}

	float                           GetCurrentTimePos(void);	

	const DString&                  GetCurrentAnimationName(void);

	unsigned short                  GetBoneCount(void);
	const DString&                  GetBoneName( unsigned short index );
	Ogre::Bone*                     GetBone(unsigned short index);

	const Ogre::Vector3            GetBonePosition(unsigned short index);

protected:
	// 是否在编辑状态中
	bool                            mAnimEditing;

	// slider拖拉的数值
	float                           mAnimTimePos;
};

#endif