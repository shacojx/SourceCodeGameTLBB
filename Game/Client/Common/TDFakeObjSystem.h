/****************************************\
*										*
* 	  用于在ui上显示的物体管理器接口	*
*										*
\****************************************/

#pragma once
#include "TDNode.h"
/*
	1. 创建/销毁在ui上显示的obj对象
	2. 对于需要显示的FakeObj，每桢调用渲染层渲染到Texture上，并更新到ui上
	3. 这些Obj自身的逻辑操作，例如更换服装、武器等操作，需要更上层逻辑处理
*/

#define FAKE_RENDERTEXTURE		("_RenderTexture")

class tObject;
class tFakeObjSystem : public tNode
{
public:
	//创建/销毁在ui上显示的obj对象
	virtual tObject*		NewFakeObject(LPCTSTR szClassName, LPCTSTR szObjName, LPCTSTR szCameraName)	= 0;
	virtual VOID			DestroyObject(LPCTSTR szObjName) = 0;

	//UI需要显示某物体
	virtual VOID			OnUIHook(LPCTSTR szWindowName, LPCTSTR szObjName)	= 0;

	//UI显示/隐藏
	virtual	VOID			OnUIShown(LPCTSTR szWindowName, float fAspectRatio)	= 0;
	virtual VOID			OnUIHiden(LPCTSTR szWindowName)	= 0;

	//旋转模型
	virtual VOID			RotateObj_Begin(LPCTSTR szObjName, FLOAT fAngle) = 0;
	virtual VOID			RotateObj_End(LPCTSTR szObjName) = 0;
	virtual VOID			Obj_ChangeAction(LPCTSTR szObjName, INT SkillID,INT SkillLoop) = 0;
protected:
	WX_DECLARE_DYNAMIC(tFakeObjSystem);
};
