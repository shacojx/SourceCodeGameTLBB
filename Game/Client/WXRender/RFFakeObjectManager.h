#pragma once
#include "..\WxRender\RenderSystem.h"
#include <OgreVector3.h>
#include <OgreTexture.h>

namespace Ogre
{
	class Camera;
	class Viewport;
	class RenderTexture;
};

class tEntityNode;
//用于UI显示的模型在渲染层的管理器
class CFakeObjectManager
{
public:
	//用于UI显示的模型渲染环境
	struct FakeObject
	{
		STRING					strName;			//名字
		Ogre::Camera*			pCamera;			//Ogre摄像机
		Ogre::RenderTexture*	pRenderTexture;		//用于渲染的Texture
		Ogre::Viewport*			pViewPort;			//用于渲染的View
		tEntityNode*			pEntityNode;		//显示节点，只负责显示，不负责创建/删除
		Ogre::TexturePtr		pTexturePtr;		//渲染纹理引用
		float					fCameraHeight;		//摄像机高度
		float					fCameraDistance;	//摄像机距离
		float					fCameraPitch;		//摄像机角度
	};
	typedef std::map< STRING, FakeObject > FakeObjectMap;

public:
	//创建一个管理节点
	void	CreateObject(LPCTSTR szObjName, tEntityNode* pNode, LPCTSTR szCameraName);
	//销毁
	void	DestroyObject(LPCTSTR szObjName);
	//设置该模型是否正在UI上显示，用于打开和关闭ViewPort的刷新，缺省不显示
	void	SetVisible(LPCTSTR szObjName, BOOL bVisible, float fAspectRatio);
	//设置正在渲染得模型，将该模型设为可见，其他设为不可见
	void	SetActiveObject(FakeObject* pObj);

	//按照ViewPort检索Object
	FakeObject*		GetObjectAsViewPort(Ogre::Viewport* pViewPort);

	VOID Debug_SetFakeCamera(int nType, float fValue);

public:
	CFakeObjectManager(void);
	~CFakeObjectManager(void);

	static CFakeObjectManager*	GetMe(VOID) { return s_pMe; }

	//初始化
	VOID		Initial(VOID);
	//释放资源
	VOID		Release(VOID);

protected:
	//得到指定渲染环境节点，如果没有就创建出新的，并放入Map中
	FakeObjectMap::iterator		_GetFakeNode(LPCTSTR szNodeName,tEntityNode* pNode, LPCTSTR szCameraName);
	//更新摄像机
	void						_UpdateCamera(FakeObject& node);

	void						_DestroyFakeNode(FakeObject& obj);

protected:
	static CFakeObjectManager*	s_pMe;

	//-------------------------------------------
	//显示数据
	FakeObjectMap			m_mapObject;
	//在场景中的位置
	Ogre::Vector3 			m_fvPosition;			

	//按照ViewPort检索的数据表
	std::map< Ogre::Viewport*, FakeObject* >	m_mapIndexOfViewPort;
};
