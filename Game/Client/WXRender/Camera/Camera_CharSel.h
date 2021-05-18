#pragma once
#include "Camera.h"

/*
|	用于任务选择时的摄像机
|
*/

class CCamera_CharSel : public CCamera
{
public:
	CCamera_CharSel(Ogre::Camera* pOgreCamera);
	virtual ~CCamera_CharSel();

public:
	virtual VOID		Update(VOID);
};