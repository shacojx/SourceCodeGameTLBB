#pragma once

#include "LogicDataStructDef.h"
#include "Core/WXPrerequisites.h"
#include <map>
#include <OgreMaterial.h>
#include <OgreVector3.h>
#include "CustomShape.h"

namespace WX 
{
	class SceneManipulator;
	
	class CBaseRenderShape
	{

	public:
		CBaseRenderShape(void);
		~CBaseRenderShape(void);

	public:

			Ogre::SceneNode*	m_pParentSceneNode;		// 用来显示区域编辑的总的节点.
			Ogre::SceneNode*	m_pSceneNode;			// 点的场景节点.
			
			CCustomShape*		m_pRectRenderShape;		// 显示信息.
			SceneManipulator*   m_pSceneManipulator;	// 用来对ogre操作的句柄。

			
	public:
			
			/////////////////////////////////////////////////////////////////////////////////
			//
			// 区域管理操作
			//

			// 设置父场景节点.
			void SetParentSceneNode(Ogre::SceneNode* pSceneNode);

			// 设置SceneManipulator句柄
			void SetSceneManipulator(SceneManipulator*  SceneManipulator);

			// 创建场景节点模型.
			unsigned long CreateSceneNode();

			// 清空渲染数据
			void ReleaseRender();

			// 添加一个点
			void AddNewPoint(Ogre::Vector3 point);

			// 创建线实体
			unsigned long CreateRenderShape();

			// 清空显示的形状.
			void ClearRenderShape();

			void ReleaseEntity();

			void ReleaseSceneNode();

			void ReleaseRenderData();
			
	};


}
