#include "BaseRenderShape.h"
#include "SceneManipulator.h"

#include "Core/WXUtils.h"
#include "Core/TerrainData.h"

#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreViewport.h>

namespace WX 
{

	using namespace Ogre;
	/////////////////////////////////////////////////////////////////////////////////
	//
	// 区域管理操作
	//

	// 设置父场景节点.
	void CBaseRenderShape::SetParentSceneNode(Ogre::SceneNode* pSceneNode)
	{
		m_pParentSceneNode = pSceneNode;
	}

	// 设置SceneManipulator句柄
	void CBaseRenderShape::SetSceneManipulator(SceneManipulator*  SceneManipulator)
	{
		m_pSceneManipulator = SceneManipulator;
	}

	// 创建场景节点模型.
	unsigned long CBaseRenderShape::CreateSceneNode()
	{
		if(m_pSceneNode)
		{
			return 0;
		}

		if(m_pParentSceneNode)
		{
			 m_pSceneNode = (SceneNode*)m_pParentSceneNode->createChildSceneNode();
			 if(NULL == m_pSceneNode)
			 {
				 // 创建失败.
				 return 0xffffffff;
			 }
		}

		// 创建成功.
		return 0;
	}


	CBaseRenderShape::CBaseRenderShape(void)
	{
		m_pParentSceneNode = NULL;
		m_pSceneNode       = NULL;		
		m_pRectRenderShape = NULL;
		m_pSceneManipulator= NULL;
	}

	CBaseRenderShape::~CBaseRenderShape(void)
	{
		ReleaseRenderData();

        if (m_pRectRenderShape)
        {
            delete m_pRectRenderShape;
            m_pRectRenderShape = NULL;
        }
	}

	// 清空渲染数据
	void CBaseRenderShape::ReleaseRender()
	{
		//m_pParentSceneNode  = NULL;		// 用来显示区域编辑的总的节点.
		//m_pSceneNode        = NULL;		// 点的场景节点.
		//m_pRectRenderShape  = NULL;		// 显示信息.
		//m_pSceneManipulator = NULL;		// 用来对ogre操作的句柄。

	}

	// 添加一个点
	void CBaseRenderShape::AddNewPoint(Ogre::Vector3 point)
	{
		if(m_pRectRenderShape)
		{
			m_pRectRenderShape->AddNewPoint(point, false);
		}
	}

	// 创建线实体
	unsigned long CBaseRenderShape::CreateRenderShape()
	{
		if(m_pRectRenderShape)
		{
			delete m_pRectRenderShape;
			m_pRectRenderShape = NULL;
		}

		m_pRectRenderShape = new CCustomShape;
		if(m_pRectRenderShape)
		{
			m_pRectRenderShape->setQueryFlags(0);
			if(m_pSceneNode)
			{
				m_pSceneNode->attachObject(m_pRectRenderShape);
			}

			//m_pRectRenderShape->FillPointData();
		}

		//m_pLineShape->setMaterial(_createIntersectGridMaterial()->getName()); 
		//m_AreaShape.setMaterial(_createIntersectGridMaterial()->getName()); 
		//m_AreaShape.setMaterial("Region/AreaRegionSel");

		return 0;
	}

	// 清空显示的形状.
	void CBaseRenderShape::ClearRenderShape()
	{
		if(m_pRectRenderShape)
		{
			m_pRectRenderShape->m_pointArray.clear();
		}
	}

	void CBaseRenderShape::ReleaseEntity()
	{
		if((m_pRectRenderShape)&&(m_pSceneNode))
		{
			m_pSceneNode->detachObject(m_pRectRenderShape->getName());
	
		}
	}

	void CBaseRenderShape::ReleaseSceneNode()
	{
		if((m_pParentSceneNode)&&(m_pSceneNode))
		{
			m_pParentSceneNode->removeAndDestroyChild(m_pSceneNode->getName());
		}
	}

	void CBaseRenderShape::ReleaseRenderData()
	{

		ClearRenderShape();

		ReleaseEntity();
		ReleaseSceneNode();
		if(m_pRectRenderShape)
		{
			delete m_pRectRenderShape;
			m_pRectRenderShape = NULL;
		}
	}

			

}
