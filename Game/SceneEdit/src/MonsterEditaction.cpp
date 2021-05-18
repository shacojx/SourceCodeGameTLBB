#include "MonsterEditAction.h"
#include "SceneManipulator.h"
#include "HitIndicator.h"
#include "IntersectPoint.h"
#include "CustomShape.h"
#include "WXOperatorManager.h"       // redo undo 管理器.

#include "OgreTimer.h"
#include "TriFunc.h"
#include "MonsterOperator.h"
#include "Core/TerrainData.h"
#include "Core/WXResources.h"
#define MODELSYSTEM_STATIC_LINK
#include "Windows.h"
#include "TLBB_DBC.h"
#include ".\editdobject_nt.h"
#include "ModelSystem\SkeletonMeshUtil.h"
#include "ModelSystem\SkeletonMesh.h"
#include <OgreCamera.h>
#include <windows.h>


#define _2PI		6.28
#define DIR_STEP	36
namespace WX 
{
	using namespace Ogre;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CMonsterInfo
//
//
	CMonsterInfo::CMonsterInfo()
		: m_pDobject(NULL)
	{
		
		//m_pDobject	   = NULL;
		m_iRefreshTime = REFRESH_TIME;
		m_ActiveType   = AREA;
		m_fAreaRadius  = AREA_RADIUS;

		m_dwObjectId		= 0xffffffff;			
       	m_iBaseAIId			= -1;					
		m_strAdvanceAIFile	= "";			
		m_strEvent			= "";					
		m_dwMonsterGUID		= 0xffffffff;		
		m_iGroupId			= -1;					
		m_iTeamId			= -1;					
		m_fMonstDir			= 0;
		m_iReputationID		= -1;
		m_iLevel			= -1;

		for(int i = 0; i < SHOP_COUNT; i++)
		{
			m_iShop[i] = -1;
		}

		

	}

	CMonsterInfo::~CMonsterInfo()
	{
		
	}

	void CMonsterInfo::DeleteDObject()
	{
		delete m_pDobject;
		m_pDobject = NULL;

		for(int i = 0; i < SHOP_COUNT; i++)
		{
			m_iShop[i] = -1;
		}

	}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// CMonsterEditaction
//

	CMonsterEditaction::CMonsterEditaction(SceneManipulator* sceneManipulator)
		  : Action(sceneManipulator)
		  , m_tempObject(NULL)
		  , m_dwCurSelMonsterInstanceId(0xffffffff)
		  , m_bIsAddOperator(false)
		  , m_pSceneNode(NULL)
	{
		InitEditData();
		CreateSceneNode();
		ClearCounter();
		m_ulAddtime = getRoot()->getTimer()->getMilliseconds();
	}

	CMonsterEditaction::~CMonsterEditaction(void)
	{
		
		
		if(m_tempObject)
		{
			delete m_tempObject;
			m_tempObject = NULL;
		}
		
		ReleaseAllMonster();
	}


	// 创建区域编辑的场景节点
	void CMonsterEditaction::CreateSceneNode()
	{
		m_pSceneNode = getSceneManipulator()->getSceneManager()->getRootSceneNode()->createChildSceneNode();
		if(m_pSceneNode)
		{
			m_pSceneNode->setVisible(false);

		}
		
	}



	// 鼠标移动的操作
    void CMonsterEditaction::_onMove(const Point& pt)
	{
		// 进行选中的临时模型的操作.
		Ogre::Vector3 position;
	    bool hit = getSceneManipulator()->getTerrainIntersects(pt.x, pt.y, position);
		if(hit)
		{
			m_curPoint = position;
			if(m_tempObject)
			{
				if(m_tempObject->getSceneNode())
				{
					m_tempObject->getSceneNode()->setPosition(position);
				}
			}
		}

		// 清空功能键
		ClearFuncKey();
	
	}



	// 鼠标按下的操作.
	void CMonsterEditaction::_onBegin(const Point& pt)
	{
		
	}



    void CMonsterEditaction::_onDrag(const Point& pt)
	{
		if(m_FuncKeyState & FUNC_KEY_SHIFT)
		{
		}
		else//
		{
			
		}

		// 每次执行结束清空功能键
		ClearFuncKey();
			
	}



	// 鼠标抬起的操作
    void CMonsterEditaction::_onEnd(const Point& pt, bool canceled)
	{
		
		if(!getActive())
		{
			return;
		}

		//if(m_FuncKeyState & FUNC_KEY_SHIFT)
		//{
		//		
		//}
		//else if(m_FuncKeyState & FUNC_KEY_CTRL)
		//{
		//	DoSelectObject(pt);
		//}
		//else
		//{
		//	
		//	DoAddNewMonster(pt);
		//}//

		if(m_bIsAddOperator)
		{
			
			// 控制每秒钟只能添加一个怪
			if(m_ulAddtime + 1000 < getRoot()->getTimer()->getMilliseconds())
			{
				DoAddNewMonster(pt);
				m_ulAddtime = getRoot()->getTimer()->getMilliseconds();

			}
		}
		else
		{
			DoSelectObject(pt);
		}
	
		// 清空功能键
		ClearFuncKey();

	}



    void CMonsterEditaction::_onActive(bool active)
	{
		
		if (!active)
        {
          	
        }
		else
		{
	
		}
		
		//HideScene(active);
	}



	const String& CMonsterEditaction::getName(void) const
	{
		//返回一个字符串名字， 用来判断当前操作的类型.
		static const String name = "MonsterEdit";
		return name;
	}



	// 隐藏节点。
	void CMonsterEditaction::HideScene(bool bHide)
	{
		if(m_pSceneNode)
		{
			m_pSceneNode->setVisible(bHide);
			m_bIsShow = bHide;
		}
	}



	// 更换模型。
	void CMonsterEditaction::ChangeModel(String& name)
	{
		if("" == name)
		{
			return;
		}

		if(m_pSceneNode)
		{
			m_bIsAddOperator = true;

			if(!m_tempObject)
			{
				// 创建一个临时选中的铜人。		
				DObject_NT::SetSceneMgr(m_pSceneNode->getCreator());
				String resourceName, groupName;
				splitResourceName(name, resourceName, groupName);
				m_tempObject = new CEditDobject_NT(mSceneManipulator);
				SkeletonMeshUtil::Load(m_tempObject, groupName, resourceName, DStringList(), DStringList());
				m_tempObject->SetParentNode(m_pSceneNode);
				//m_tempObject->SetTransparence(0.8f);

				Ogre::ColourValue color(0, 0, 1.0f, 0.5f);
				m_tempObject->SetSelectLook(color);

				// 设置位置
				m_tempObject->getSceneNode()->setPosition(m_curPoint);
			
			}
			else
			{
				delete m_tempObject;
				DObject_NT::SetSceneMgr(m_pSceneNode->getCreator());
				String resourceName, groupName;
				splitResourceName(name, resourceName, groupName);
				m_tempObject = new CEditDobject_NT(mSceneManipulator);
				SkeletonMeshUtil::Load(m_tempObject, groupName, resourceName, DStringList(), DStringList());
				m_tempObject->SetParentNode(m_pSceneNode);
				//m_tempObject->SetTransparence(0.8f);

				Ogre::ColourValue color(0, 0, 1.0f, 0.5f);
				m_tempObject->SetSelectLook(color);

				// 设置位置
				m_tempObject->getSceneNode()->setPosition(m_curPoint);
			}
		}

	}



	// 更换模型。
	void CMonsterEditaction::ChangeModel(const char* pName)
	{
		String strName = pName;
		ChangeModel(strName);
	}


	
	// 添加一个怪物
	bool CMonsterEditaction::AddNewMonster(const char* pName,  Ogre::Vector3& pos, unsigned long dwObjectId)
	{
		
		String strName = pName;
		return AddNewMonster(strName, pos, dwObjectId);
	}



	// 添加一个怪物
	bool CMonsterEditaction::AddNewMonster(String& name,  Ogre::Vector3& pos, unsigned long dwObjectId)
	{

		if("" == name)
		{
			return false;
		}

		CEditDobject_NT* pObject = NULL;
		DObject_NT::SetSceneMgr(m_pSceneNode->getCreator());
		String resourceName, groupName;
		splitResourceName(name, resourceName, groupName);
		pObject = new CEditDobject_NT(mSceneManipulator);
		if(SkeletonMeshUtil::Load(pObject, groupName, resourceName, DStringList(), DStringList()))
		{
			
			pObject->SetParentNode(m_pSceneNode);
			pObject->getSceneNode()->setPosition(pos);

			CMonsterInfo monsterInfo;
			monsterInfo.m_dwInstanceId = m_dwMonsterInstanceId;
			monsterInfo.m_dwObjectId = dwObjectId;
			monsterInfo.m_pDobject = pObject;
			monsterInfo.m_position = pos;
			monsterInfo.m_dwMonsterGUID = m_dwGUID;
			m_dwGUID++;

			m_MonsterInfoMap[m_dwMonsterInstanceId] = monsterInfo;

			POINT_2D _2dPos;

			_2dPos.iX = (int)(pos.x / 100);
			_2dPos.iY = (int)(pos.z / 100);

			if(m_MonsterPosMap.count(_2dPos))
			{
				INSTANCE_ID_LIST& list = m_MonsterPosMap[_2dPos];
				list.push_back(m_dwMonsterInstanceId);
			}
			else
			{
				INSTANCE_ID_LIST list;
				list.push_back(m_dwMonsterInstanceId);
				m_MonsterPosMap[_2dPos] = list;
			}

			getSceneManipulator()->_fireAddNewMonster(dwObjectId, m_dwMonsterInstanceId);
			m_dwMonsterInstanceId++;
			return true;
		}
		else
		{
			delete pObject;
			return false;
		}

	}



	// 初始化编辑的数据
	void CMonsterEditaction::InitEditData()
	{
		m_strCurEditMonsterName = "";
		m_dwCurEditMonsterId = 0xffffffff;
		ReleaseAllMonster();
		m_MonsterInfoMap.clear();

		// 读取外形文件
		GetMonsterIdLookTable();
	}



	//设置新的编辑模型。
	void CMonsterEditaction::SetNewEditObject(String& name, unsigned long dwObjectId)
	{
		// 当前编辑的怪物的名称。
		m_strCurEditMonsterName = name;

		// 当前编辑的怪物外形的id
		m_dwCurEditMonsterId = dwObjectId;

		// 添加映射表.
		m_IdNameMap[dwObjectId] = name;

		// 改变名字
		ChangeModel(name);

	}



	//设置新的编辑模型。
	void CMonsterEditaction::SetNewEditObject(const char* pName, unsigned long dwObjectId)
	{
		String strName = pName;
		m_IdNameMap[dwObjectId] = strName;
		SetNewEditObject(strName, dwObjectId);
	}



	// 释放所有的怪物。
	void CMonsterEditaction::ReleaseAllMonster()
	{
		MONSTERINFO_MAP::iterator it;
		MONSTERINFO_MAP::iterator itEnd;

		itEnd = m_MonsterInfoMap.end();

		for(it = m_MonsterInfoMap.begin(); it != itEnd; it++)
		{
			CMonsterInfo& monsterInfo = it->second;
			monsterInfo.DeleteDObject();
		}

		m_MonsterInfoMap.clear();

		ClearCounter();
	}



	// 得到选中的怪物
	unsigned long  CMonsterEditaction::SelectObject(const Point& pt)
	{

		float fMinDis = 100000.0f;
		float ft      = 0;
		bool  bSel    = false;
		unsigned long dwId = 0xffffffff;

		if(NULL == getSceneManipulator())
		{
			return 0xffffffff;
		}

		Ogre::Vector3 position;
		bool hit = getSceneManipulator()->getTerrainIntersects(pt.x, pt.y, position);

		if(false == hit)
		{
			return 0xffffffff;
		}

	


		Ogre::Ray ray = getSceneManipulator()->getWindowToViewportRay(pt.x, pt.y);
		int iCenterX = (int)(position.x / 100);
		int iCenterY = (int)(position.z / 100);
		
		POINT_2D _2dPos;
		INSTANCE_ID_LIST::iterator it;
		INSTANCE_ID_LIST::iterator itEnd;

		for(int i = iCenterX - 15; i < iCenterX + 15; i++)
		{
			for(int j = iCenterY - 15; j < iCenterY + 15; j++)
			{
				
				_2dPos.iX = i;
				_2dPos.iY = j;
				if(0 == m_MonsterPosMap.count(_2dPos))
				{
					continue;
				}

				INSTANCE_ID_LIST& list = m_MonsterPosMap[_2dPos];

				itEnd = list.end();
				
				for(it = list.begin(); it != itEnd; it++)
				{
					CMonsterInfo& monsterInfo = m_MonsterInfoMap[(*it)];
					if(IsSelectObject(ray, monsterInfo, ft))
					{
						bSel = true;

						if(fMinDis > ft)
						{
							fMinDis = ft;
							dwId = monsterInfo.m_dwInstanceId;
						}

					}
				}
			}
				
		}


		return dwId;
	}


	// 是否选中怪物
	bool CMonsterEditaction::IsSelectObject(Ogre::Ray& ray, CMonsterInfo& monsterInfo, float& fdis)
	{
		
		float fu = 0;
		float fv = 0;
		float fd = 0;
		bool  bRet = false;

		if(NULL == monsterInfo.m_pDobject)
		{
			return false;
		}
		const AxisAlignedBox& BindBox = monsterInfo.m_pDobject->getBoundingBox();
		
		Ogre::Vector3 v1; 
		Ogre::Vector3 v2; 
		Ogre::Vector3 v3;

		const Ogre::Vector3 dir  = ray.getDirection();
		const Ogre::Vector3 orig = ray.getOrigin();

		const Ogre::Vector3 vMax = BindBox.getMaximum(); 
		const Ogre::Vector3 vMin = BindBox.getMinimum() ;
		 
		float fDisTance = 100000.0f;

		////////////////////////////////////////////////////////////////////////////////////////
		// 取maxZ
		v1.x = vMin.x + monsterInfo.m_position.x;
		v1.y = vMin.y + monsterInfo.m_position.y;
		v1.z = vMax.z + monsterInfo.m_position.z;

		v2.x = vMin.x + monsterInfo.m_position.x;
		v2.y = vMax.y + monsterInfo.m_position.y;
		v2.z = vMax.z + monsterInfo.m_position.z;

		v3.x = vMax.x + monsterInfo.m_position.x;
		v3.y = vMin.y + monsterInfo.m_position.y;
		v3.z = vMax.z + monsterInfo.m_position.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}

		// 取maxZ
		v1.x = vMax.x + monsterInfo.m_position.x;
		v1.y = vMax.y + monsterInfo.m_position.y;
		v1.z = vMax.z + monsterInfo.m_position.z;

		v2.x = vMin.x + monsterInfo.m_position.x;
		v2.y = vMax.y + monsterInfo.m_position.y;
		v2.z = vMax.z + monsterInfo.m_position.z;

		v3.x = vMax.x + monsterInfo.m_position.x;
		v3.y = vMin.y + monsterInfo.m_position.y;
		v3.z = vMax.z + monsterInfo.m_position.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}


		///////////////////////////////////////////////////////////////////////////////////////////////////
		// 取minZ
		v1.x = vMax.x + monsterInfo.m_position.x;
		v1.y = vMax.y + monsterInfo.m_position.y;
		v1.z = vMin.z + monsterInfo.m_position.z;

		v2.x = vMin.x + monsterInfo.m_position.x;
		v2.y = vMax.y + monsterInfo.m_position.y;
		v2.z = vMin.z + monsterInfo.m_position.z;

		v3.x = vMax.x + monsterInfo.m_position.x;
		v3.y = vMin.y + monsterInfo.m_position.y;
		v3.z = vMin.z + monsterInfo.m_position.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}

		// 取minZ
		v1.x = vMax.x + monsterInfo.m_position.x;
		v1.y = vMax.y + monsterInfo.m_position.y;
		v1.z = vMin.z + monsterInfo.m_position.z;

		v2.x = vMin.x + monsterInfo.m_position.x;
		v2.y = vMax.y + monsterInfo.m_position.y;
		v2.z = vMin.z + monsterInfo.m_position.z;

		v3.x = vMax.x + monsterInfo.m_position.x;
		v3.y = vMin.y + monsterInfo.m_position.y;
		v3.z = vMin.z + monsterInfo.m_position.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}


		///////////////////////////////////////////////////////////////////////////////////////////////////
		// 取MaxX
		v1.x = vMax.x + monsterInfo.m_position.x;
		v1.y = vMin.y + monsterInfo.m_position.y;
		v1.z = vMin.z + monsterInfo.m_position.z;

		v2.x = vMax.x + monsterInfo.m_position.x;
		v2.y = vMax.y + monsterInfo.m_position.y;
		v2.z = vMin.z + monsterInfo.m_position.z;

		v3.x = vMax.x + monsterInfo.m_position.x;
		v3.y = vMin.y + monsterInfo.m_position.y;
		v3.z = vMax.z + monsterInfo.m_position.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}

		//  取MaxX
		v1.x = vMax.x + monsterInfo.m_position.x;
		v1.y = vMax.y + monsterInfo.m_position.y;
		v1.z = vMax.z + monsterInfo.m_position.z;

		v2.x = vMax.x + monsterInfo.m_position.x;
		v2.y = vMax.y + monsterInfo.m_position.y;
		v2.z = vMin.z + monsterInfo.m_position.z;

		v3.x = vMax.x + monsterInfo.m_position.x;
		v3.y = vMin.y + monsterInfo.m_position.y;
		v3.z = vMax.z + monsterInfo.m_position.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}



		///////////////////////////////////////////////////////////////////////////////////////////////////
		// 取MinX
		v1.x = vMin.x + monsterInfo.m_position.x;
		v1.y = vMin.y + monsterInfo.m_position.y;
		v1.z = vMin.z + monsterInfo.m_position.z;

		v2.x = vMin.x + monsterInfo.m_position.x;
		v2.y = vMax.y + monsterInfo.m_position.y;
		v2.z = vMin.z + monsterInfo.m_position.z;

		v3.x = vMin.x + monsterInfo.m_position.x;
		v3.y = vMin.y + monsterInfo.m_position.y;
		v3.z = vMax.z + monsterInfo.m_position.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}

		//  取MinX
		v1.x = vMin.x + monsterInfo.m_position.x;
		v1.y = vMax.y + monsterInfo.m_position.y;
		v1.z = vMax.z + monsterInfo.m_position.z;

		v2.x = vMin.x + monsterInfo.m_position.x;
		v2.y = vMax.y + monsterInfo.m_position.y;
		v2.z = vMin.z + monsterInfo.m_position.z;

		v3.x = vMin.x + monsterInfo.m_position.x;
		v3.y = vMin.y + monsterInfo.m_position.y;
		v3.z = vMax.z + monsterInfo.m_position.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}



		///////////////////////////////////////////////////////////////////////////////////////////////////
		// 取MaxY
		v1.x = vMin.x + monsterInfo.m_position.x;
		v1.y = vMax.y + monsterInfo.m_position.y;
		v1.z = vMin.z + monsterInfo.m_position.z;

		v2.x = vMin.x + monsterInfo.m_position.x;
		v2.y = vMax.y + monsterInfo.m_position.y;
		v2.z = vMax.z + monsterInfo.m_position.z;

		v3.x = vMax.x + monsterInfo.m_position.x;
		v3.y = vMax.y + monsterInfo.m_position.y;
		v3.z = vMin.z + monsterInfo.m_position.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}

		//  取MaxY
		v1.x = vMax.x + monsterInfo.m_position.x;
		v1.y = vMax.y + monsterInfo.m_position.y;
		v1.z = vMax.z + monsterInfo.m_position.z;

		v2.x = vMin.x + monsterInfo.m_position.x;
		v2.y = vMax.y + monsterInfo.m_position.y;
		v2.z = vMax.z + monsterInfo.m_position.z;

		v3.x = vMax.x + monsterInfo.m_position.x;
		v3.y = vMax.y + monsterInfo.m_position.y;
		v3.z = vMin.z + monsterInfo.m_position.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}



		
		///////////////////////////////////////////////////////////////////////////////////////////////////
		// 取MinY
		v1.x = vMin.x + monsterInfo.m_position.x;
		v1.y = vMin.y + monsterInfo.m_position.y;
		v1.z = vMin.z + monsterInfo.m_position.z;

		v2.x = vMin.x + monsterInfo.m_position.x;
		v2.y = vMin.y + monsterInfo.m_position.y;
		v2.z = vMax.z + monsterInfo.m_position.z;

		v3.x = vMax.x + monsterInfo.m_position.x;
		v3.y = vMin.y + monsterInfo.m_position.y;
		v3.z = vMin.z + monsterInfo.m_position.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}

		//  取MinY
		v1.x = vMax.x + monsterInfo.m_position.x;
		v1.y = vMin.y + monsterInfo.m_position.y;
		v1.z = vMax.z + monsterInfo.m_position.z;

		v2.x = vMin.x + monsterInfo.m_position.x;
		v2.y = vMin.y + monsterInfo.m_position.y;
		v2.z = vMax.z + monsterInfo.m_position.z;

		v3.x = vMax.x + monsterInfo.m_position.x;
		v3.y = vMin.y + monsterInfo.m_position.y;
		v3.z = vMin.z + monsterInfo.m_position.z;

		if(IsSelTri(dir, orig, v1, v2, v3, fd, fu, fv))
		{
			if(fd < fDisTance)
			{
				fDisTance = fd;
				bRet = true;
			}
		}

		fdis = fDisTance;
		return bRet;
	}

	
	void CMonsterEditaction::setParameter(const String& name, const String& value)
    {
        if (name == "FUNC_KEY")
        {
           if("SHIFT" == value)
		   {
			   SetFuncKey(FUNC_KEY_SHIFT);
		   }
		   else if("CTRL" == value)
		   {
			   SetFuncKey(FUNC_KEY_CTRL);
		   }
		   else if("ATL" == value)
		   {
			   SetFuncKey(FUNC_KEY_ALT);
		   }
		   else if("ESC" == value)
		   {
			   m_bIsAddOperator = false;
			   if(m_tempObject)
			   {

				   if(m_tempObject->getSceneNode())
				   {
					   m_tempObject->getSceneNode()->setVisible(false);
				   }
			   }
		   }
        }
		else if("DELETE_MONSTER" == name)
		{
			DelSelectMonster();
		}
    
    }

	// 设置按下的功能键
	void CMonsterEditaction::SetFuncKey(unsigned long ulFuncKey)
	{
		m_FuncKeyState |= ulFuncKey;
	}

	//  清空功能键.
	void CMonsterEditaction::ClearFuncKey()
	{
		m_FuncKeyState = FUNCTION_KEY;
	}

	// 进行选择物体的操作
	void CMonsterEditaction::DoSelectObject(const Point& pt)
	{
		// 没有选中
		unsigned long  dwId = SelectObject(pt);
		if(0xffffffff != dwId)
		{
			if( 0 == m_MonsterInfoMap.count(dwId))
			{
				return;
			}

			if(m_dwCurSelMonsterInstanceId == dwId)
			{
				return;
			}
				
			
			CMonsterInfo monsterInfo = m_MonsterInfoMap[dwId];
			Ogre::ColourValue color(0, 0, 1.0f, 0.5f);
			monsterInfo.m_pDobject->SetSelectLook(color);
			
			if(m_MonsterInfoMap.count(m_dwCurSelMonsterInstanceId))
			{
				m_MonsterInfoMap[m_dwCurSelMonsterInstanceId].m_pDobject->SetUnSelLook();
			}

			// 通知界面选择了一个怪物.
			getSceneManipulator()->_fireSelMonster(monsterInfo.m_dwObjectId, dwId);
			
			m_dwCurSelMonsterInstanceId = dwId;
		}
	}

	// 进行添加物体的操作
	void CMonsterEditaction::DoAddNewMonster(const Point& pt)
	{
		Ogre::Vector3 position;
		bool hit = getSceneManipulator()->getTerrainIntersects(pt.x, pt.y, position);

		if(hit)
		{
			if(AddNewMonster(m_strCurEditMonsterName, position, m_dwCurEditMonsterId))
			{
				if(m_MonsterInfoMap.count(m_dwCurSelMonsterInstanceId))
				{
					m_MonsterInfoMap[m_dwCurSelMonsterInstanceId].m_pDobject->SetUnSelLook();
				}

				m_dwCurSelMonsterInstanceId = m_dwMonsterInstanceId - 1;
				if(m_MonsterInfoMap.count(m_dwCurSelMonsterInstanceId))
				{
					Ogre::ColourValue color(0.4f, 0.1f, 0.9f, 0.1f);
					m_MonsterInfoMap[m_dwCurSelMonsterInstanceId].m_pDobject->SetSelectLook(color);
				}


				// 增加redo undo 操作.
				CMonsterEditAddNewMonsterOperator* pAddNewMonsterOperator = NULL;
				pAddNewMonsterOperator = new CMonsterEditAddNewMonsterOperator;

				pAddNewMonsterOperator->SetMonsterAction(this);
				pAddNewMonsterOperator->SetMonsterName(m_strCurEditMonsterName);
				pAddNewMonsterOperator->SetMonsterInstanceId(m_dwMonsterInstanceId - 1);
				pAddNewMonsterOperator->SetMonsterId(m_dwCurEditMonsterId);
				pAddNewMonsterOperator->SetPosition(position, position);
				getOperatorManager()->addOperator(pAddNewMonsterOperator);
	  			
			}
		}
	}

	// 删除物体
	void CMonsterEditaction::DelSelectMonster()
	{
		DelMonster(m_dwCurSelMonsterInstanceId);
  	}

	
	// 进行删除物体的操作
	void CMonsterEditaction::DoDelMonster(const Point& pt)
	{

	}


	// 删除物体
	void CMonsterEditaction::DelMonster(unsigned long dwInstanceId)
	{
		if(0xffffffff == dwInstanceId)
		{
			return;
		}

		MONSTERINFO_MAP::iterator it;
		it = m_MonsterInfoMap.find(dwInstanceId);
		if(it == m_MonsterInfoMap.end())
		{
			return;
		}

		CMonsterInfo&  MonsterInfo = it->second;
		getSceneManipulator()->_fireDelMonster(MonsterInfo.m_dwObjectId, MonsterInfo.m_dwInstanceId);
		
		delete MonsterInfo.m_pDobject;
		m_MonsterInfoMap.erase(it);

		// 当前选中的怪物id
		if(dwInstanceId == m_dwCurSelMonsterInstanceId)
		{
			m_dwCurSelMonsterInstanceId = 0xffffffff;
		}
	}


	
// 得到怪物id， 与名字的映射表.
void CMonsterEditaction::LoadMonsterIdNameTable()
{

	FILE* pFile = NULL;
	pFile = ::fopen("..\\LogicEditData\\monster\\CreatureModel.txt", "r");
		
	if(NULL == pFile)
	{
		return;
	}

	char buf[2048];
	::fgets(buf, sizeof(buf), pFile);
	::fgets(buf, sizeof(buf), pFile);

	// 怪物id
	char bufId[64];

	// 怪物外形文件名字
	char bufName[128];

	m_IdNameMap.clear();

	int iId = -1;
	while(!feof(pFile))
	{
		memset(buf, 0, sizeof(buf));
		memset(bufId, 0, sizeof(bufId));
		memset(bufName, 0, sizeof(bufName));

		// 得到一个怪物
		::fgets(buf, sizeof(buf), pFile);
		sscanf(buf, "%s %s ", bufId, bufName);

		if(0 == strcmp("", bufId))
		{
			continue;
		}
		iId = atoi(bufId);
		m_IdNameMap[iId] = bufName;

	}

	::fclose(pFile);

}

// 得到怪物类型表
void CMonsterEditaction::LoadMonsterTypeTable()
{

	FILE* pFile = NULL;
	pFile = ::fopen("..\\LogicEditData\\monster\\CreatureInfo.txt", "r");
	
	if(NULL == pFile)
	{
		return;
	}

	char buf[2048];
	::fgets(buf, sizeof(buf), pFile);
	::fgets(buf, sizeof(buf), pFile);

	// 怪物类型id
	char bufId[64];

	// 怪物外型id
	char bufLookId[64];

	int iId     = -1;
	int iLookId = -1;
	
	m_Id_LookId_Map.clear();

	while(!feof(pFile))
	{
		memset(buf, 0, sizeof(buf));
		memset(bufId, 0, sizeof(bufId));
		memset(bufLookId, 0, sizeof(bufLookId));

		// 得到一个怪物
		::fgets(buf, sizeof(buf), pFile);
		sscanf(buf, "%s %s ", bufId, bufLookId);
		if(0 == strcmp("", bufId))
		{
			continue;
		}

		iId     = atoi(bufId);
		iLookId = atoi(bufLookId);
		m_Id_LookId_Map[iId] = iLookId;

	}

	::fclose(pFile);

}



// 读取文件的时候使用, 添加一个怪物
bool CMonsterEditaction::AddNewMonster(Ogre::Vector3& pos, unsigned long dwObjectId, bool bTransPos)
{
	if(0 == m_IdNameMap.count(dwObjectId))
	{
		return false;
	}

	// 得到名字
	std::string strName = m_IdNameMap[dwObjectId];

	if(bTransPos)
	{
		// 缩放系数
		float fScale = 1.0;
		float fTerrainDataPosX = 0;
		float fTerrainDataPosZ = 0;
		float fTerrainDataPosY = 0;

		fTerrainDataPosX = getTerrainData()->mPosition.x;
		fTerrainDataPosZ = getTerrainData()->mPosition.z;
		fTerrainDataPosY = getTerrainData()->mPosition.y;

		if(getSceneManipulator())
		{
			fScale = getSceneManipulator()->getBaseScale();
			if(abs((float)fScale) < 0.00001)
			{
				fScale = 1.0;
			}
		}

		// 逻辑坐标和场景坐标转换.
		pos.x = pos.x * fScale + fTerrainDataPosX;
		pos.z =	pos.z * fScale + fTerrainDataPosZ;
		pos.y = getTerrainData()->getHeightAt(pos.x, pos.z);


	}

	return AddNewMonster(strName, pos, dwObjectId);
}

// 根据怪物实例id， 得到怪物的信息指针
CMonsterInfo* CMonsterEditaction::GetMonster(unsigned long dwInstanceId)
{

	if(0 == m_MonsterInfoMap.count(dwInstanceId))
	{
		return NULL;
	}

	CMonsterInfo& monsterInfo = m_MonsterInfoMap[dwInstanceId];

	return &monsterInfo;

}


// 改变所有的怪物的刷新时间。
void CMonsterEditaction::ChangeAllMonsterRefreshTime(int iTime)
{
	MONSTERINFO_MAP::iterator it;
	MONSTERINFO_MAP::iterator itEnd;

	itEnd = m_MonsterInfoMap.end();
	for(it = m_MonsterInfoMap.begin(); it != itEnd; it++)
	{
		CMonsterInfo& monsterInfo = it->second;
		monsterInfo.m_iRefreshTime = iTime;

	}

}

// 改变所有怪物的活动半径。
void CMonsterEditaction::ChangeAllMonsterRadius(float fRadius)
{
	MONSTERINFO_MAP::iterator it;
	MONSTERINFO_MAP::iterator itEnd;

	itEnd = m_MonsterInfoMap.end();
	for(it = m_MonsterInfoMap.begin(); it != itEnd; it++)
	{
		CMonsterInfo& monsterInfo = it->second;
		monsterInfo.m_fAreaRadius  = fRadius;

	}

}

// 改变刷新时间和活动半径。
void CMonsterEditaction::ChangeAllMonsterRefreshTimeRadius(int iTime, float fRadius)
{
	MONSTERINFO_MAP::iterator it;
	MONSTERINFO_MAP::iterator itEnd;

	itEnd = m_MonsterInfoMap.end();
	for(it = m_MonsterInfoMap.begin(); it != itEnd; it++)
	{
		CMonsterInfo& monsterInfo = it->second;
		monsterInfo.m_iRefreshTime = iTime;
		monsterInfo.m_fAreaRadius  = fRadius;

	}
}

// 改变某一类怪的刷新时间。
void CMonsterEditaction::ChangeMonsterRefreshTime_Type(unsigned long dwObjectId, int iTime)
{
	MONSTERINFO_MAP::iterator it;
	MONSTERINFO_MAP::iterator itEnd;

	itEnd = m_MonsterInfoMap.end();
	for(it = m_MonsterInfoMap.begin(); it != itEnd; it++)
	{
		CMonsterInfo& monsterInfo = it->second;
		if(monsterInfo.m_dwObjectId == dwObjectId)
		{
			monsterInfo.m_iRefreshTime = iTime;

		}
	}

}

// 改变某一类怪的活动半径。
void CMonsterEditaction::ChangeMonsterRefreshRadius_Type(unsigned long dwObjectId, float fRadius)
{
	MONSTERINFO_MAP::iterator it;
	MONSTERINFO_MAP::iterator itEnd;

	itEnd = m_MonsterInfoMap.end();
	for(it = m_MonsterInfoMap.begin(); it != itEnd; it++)
	{
		CMonsterInfo& monsterInfo = it->second;
		if(monsterInfo.m_dwObjectId == dwObjectId)
		{
			monsterInfo.m_fAreaRadius  = fRadius;

		}
	}
}

// 改变某一类怪的刷新时间和活动半径。
void CMonsterEditaction::ChangeMonsterRefreshTimeRadius_Type(unsigned long dwObjectId, int iTime, float fRadius)
{
	MONSTERINFO_MAP::iterator it;
	MONSTERINFO_MAP::iterator itEnd;

	itEnd = m_MonsterInfoMap.end();
	for(it = m_MonsterInfoMap.begin(); it != itEnd; it++)
	{
		CMonsterInfo& monsterInfo = it->second;
		if(monsterInfo.m_dwObjectId == dwObjectId)
		{
			monsterInfo.m_iRefreshTime = iTime;
			monsterInfo.m_fAreaRadius  = fRadius;

		}
	}
	
}

	
// 改变怪物的刷新时间。
void CMonsterEditaction::ChangeMonsterRefreshTime(unsigned long dwInstanceId, int iTime)
{
	if(0 == m_MonsterInfoMap.count(dwInstanceId))
	{
		return;
	}

	CMonsterInfo& monsterInfo = m_MonsterInfoMap[dwInstanceId];

	monsterInfo.m_iRefreshTime = iTime;
}

// 改变怪物的活动半径。
void CMonsterEditaction::ChangeMonsterRefreshRadius(unsigned long dwInstanceId, float fRadius)
{
	if(0 == m_MonsterInfoMap.count(dwInstanceId))
	{
		return;
	}

	CMonsterInfo& monsterInfo = m_MonsterInfoMap[dwInstanceId];

	monsterInfo.m_fAreaRadius = fRadius;
}

// 改变怪物的刷新时间和半径。
void CMonsterEditaction::ChangeMonsterRefreshTimeRadius(unsigned long dwInstanceId, int iTime, float fRadius)
{

	if(0 == m_MonsterInfoMap.count(dwInstanceId))
	{
		return;
	}

	CMonsterInfo& monsterInfo = m_MonsterInfoMap[dwInstanceId];

	monsterInfo.m_iRefreshTime = iTime;
	monsterInfo.m_fAreaRadius  = fRadius;
}

// 改变怪物的方向
void CMonsterEditaction::ChangeMonsterDir(unsigned long dwInstanceId, float radian)
{

	if(0 == m_MonsterInfoMap.count(dwInstanceId))
	{
		return;
	}

	CMonsterInfo& monsterInfo = m_MonsterInfoMap[dwInstanceId];

	if(	monsterInfo.m_pDobject )
	{
		if(monsterInfo.m_pDobject->getSceneNode())
		{
			float fw = 0;
			float fx = 0;
			float fy = 0;
			float fz = 0;

			monsterInfo.m_fMonstDir = radian;

			if(monsterInfo.m_fMonstDir >= _2PI)
			{
				monsterInfo.m_fMonstDir -= _2PI;
			}

			if(monsterInfo.m_fMonstDir <= -_2PI)
			{
				monsterInfo.m_fMonstDir += _2PI;
			}

			fw = cos(radian/2);
			fx = 0;
			fy = sin(radian/2);
			fz = 0;
			monsterInfo.m_pDobject->getSceneNode()->setOrientation(fw, fx, fy, fz);
		}
	}

	SetModifyed();


}


// 从区域读取文件
unsigned long CMonsterEditaction::ReadEditMonsterFromFile(std::string strFileName)
{
	char bufMonsterIndex[64];

	int iMonsterCount = ::GetPrivateProfileInt("info", "monstercount", 0, strFileName.c_str());
	int iMonsterType  = 0;
	unsigned long   dwGUID = 0xffffffff;
	
	int  iMonsterDir     = 0;
	int  iMonsterGroupId = 0;
	int  iMonsterTeamId  = 0;
	int  iBaseAIId       = 0;

	char bufX[64];
	char bufY[64];
	char bufZ[64];

	char bufAI[1024];
	char bufEven[1024];


	// 缩放系数
	float fScale = 1.0;
	float fTerrainDataPosX = 0;
	float fTerrainDataPosZ = 0;
	float fTerrainDataPosY = 0;

	fTerrainDataPosX = getTerrainData()->mPosition.x;
	fTerrainDataPosZ = getTerrainData()->mPosition.z;
	fTerrainDataPosY = getTerrainData()->mPosition.y;

	if(getSceneManipulator())
	{
		fScale = getSceneManipulator()->getBaseScale();
		if(abs((float)fScale) < 0.00001)
		{
			fScale = 1.0;
		}
	}


	String  MonsterTypename;
	Ogre::Vector3 pos;

	float fDir = 0;

	CMonsterInfo* pMonsterInfo = NULL;

	int iLookId = -1;
	for(int i = 0; i < iMonsterCount; i++)
	{

		memset(bufMonsterIndex, 0, sizeof(bufMonsterIndex));
		memset(bufX, 0, sizeof(bufX));
		memset(bufY, 0, sizeof(bufY));
		memset(bufZ, 0, sizeof(bufZ));
		sprintf(bufMonsterIndex, "monster%d", i);

		// 读取怪物类型
		iMonsterType = ::GetPrivateProfileInt(bufMonsterIndex, "monster_type", -1, strFileName.c_str());
		if(m_Id_LookId_Map.count(iMonsterType))
		{
			continue;
		}

		iLookId = m_Id_LookId_Map[iMonsterType];
		if(m_IdNameMap.count(iLookId))
		{
			
			MonsterTypename = m_IdNameMap[iLookId];
		
			// 读取怪物guid
			dwGUID = ::GetPrivateProfileInt(bufMonsterIndex, "monster_guid", 0xffffffff, strFileName.c_str());

			// 读取位置
			::GetPrivateProfileString(bufMonsterIndex, "position_X",  "none", bufX, sizeof(bufX), strFileName.c_str());
			::GetPrivateProfileString(bufMonsterIndex, "position_Y",  "none", bufY, sizeof(bufY),strFileName.c_str());
			::GetPrivateProfileString(bufMonsterIndex, "position_Z",  "none", bufZ, sizeof(bufZ),strFileName.c_str());

			// 读取方向。
			iMonsterDir = ::GetPrivateProfileInt(bufMonsterIndex, "monster_dir",  -1, strFileName.c_str());

			// 读取groupid
			iMonsterGroupId = ::GetPrivateProfileInt(bufMonsterIndex, "monster_groupid", -1, strFileName.c_str());

			// 读取teamid
			iMonsterTeamId = ::GetPrivateProfileInt(bufMonsterIndex, "monster_teamid", -1, strFileName.c_str());

			// 读怪物的basseAiId
			iBaseAIId = ::GetPrivateProfileInt(bufMonsterIndex, "monster_BaseAI",  -1, strFileName.c_str());

			// 读怪物的高级ai文件名字
			memset(bufAI, 0, sizeof(bufAI));
			::GetPrivateProfileString(bufMonsterIndex, "monster_AIFile",  "none", bufAI, sizeof(bufAI), strFileName.c_str());

			// 读怪物的事件文件名字
			memset(bufEven, 0, sizeof(bufEven));
			::GetPrivateProfileString(bufMonsterIndex, "monster_EvenFile", "none", bufEven, sizeof(bufEven), strFileName.c_str());

			// 保证当前地图的guid是最大的.
			if(0xffffffff != dwGUID)
			{
				if(m_dwGUID < dwGUID)
				{
					m_dwGUID = dwGUID;
				}
			}

			iMonsterDir += 9;
			iMonsterDir = iMonsterDir % DIR_STEP;

			fDir = (iMonsterDir*1.0 / DIR_STEP) * _2PI;

			pos.x = atof(bufX);
			pos.y = atof(bufY);
			pos.z = atof(bufZ);

			// 逻辑坐标和场景坐标转换.
			pos.x = pos.x * fScale + fTerrainDataPosX;
			pos.y = pos.y * fScale + fTerrainDataPosY;
			pos.z =	pos.z * fScale + fTerrainDataPosZ;

			if(AddNewMonster(MonsterTypename,  pos, iMonsterType))
			{
				ChangeMonsterDir(m_dwMonsterInstanceId - 1, fDir);

				CMonsterInfo& monsterInfo   = m_MonsterInfoMap[m_dwMonsterInstanceId - 1];
				monsterInfo.m_dwMonsterGUID = m_dwGUID;
				monsterInfo.m_iGroupId  = iMonsterGroupId;
				monsterInfo.m_iTeamId   = iMonsterTeamId;
				monsterInfo.m_iBaseAIId = iBaseAIId;
				monsterInfo.m_strAdvanceAIFile = bufAI;
				monsterInfo.m_strEvent = bufEven;
			}

		}

	}

	return 0;
}
	
// 把区域存储到文件
unsigned long CMonsterEditaction::SaveEditMonsterToFile(std::string strFileName)
{
	char bufWrite[128];
	char bufMonsterIndex[128];
	int iMonsterType  = 0;
	int  iMonsterDir;
	char bufX[64];
	char bufY[64];
	char bufZ[64];

	float fx = 0;
	float fy = 0;
	float fz = 0;


	// 缩放系数
	float fScale = 1.0;
	float fTerrainDataPosX = 0;
	float fTerrainDataPosZ = 0;
	float fTerrainDataPosY = 0;

	fTerrainDataPosX = getTerrainData()->mPosition.x;
	fTerrainDataPosZ = getTerrainData()->mPosition.z;
	fTerrainDataPosY = getTerrainData()->mPosition.y;

	if(getSceneManipulator())
	{
		fScale = getSceneManipulator()->getBaseScale();
		if(abs((float)fScale) < 0.00001)
		{
			fScale = 1.0;
		}
	}


	// 删除旧的文件。
	::DeleteFile(strFileName.c_str());

    String  MonsterTypename;
	Ogre::Vector3 pos;

	int iMonsterCount = 0;
	iMonsterCount = m_MonsterInfoMap.size();

	memset(bufWrite, 0, sizeof(bufWrite));
	sprintf(bufWrite, "%d", iMonsterCount);

	// 填写怪物的个数
	WritePrivateProfileString("info", "monstercount", bufWrite, strFileName.c_str());
	
	MONSTERINFO_MAP::iterator it;
	MONSTERINFO_MAP::iterator itEnd;
	
	itEnd = m_MonsterInfoMap.end();
	int i = 0;
	
	for(it = m_MonsterInfoMap.begin(); it != itEnd; it++)
	{
		CMonsterInfo& MonsterInfo = it->second;

		memset(bufMonsterIndex, 0, sizeof(bufMonsterIndex));
		sprintf(bufMonsterIndex, "monster%d", i);

		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_dwObjectId);
		
		
		// 写怪物的类型.
		::WritePrivateProfileString(bufMonsterIndex, "monster_type", bufWrite, strFileName.c_str());

		// 写怪物的位置
		memset(bufX, 0, sizeof(bufX));

		// 进行场景坐标和逻辑坐标转换
        fx = (MonsterInfo.m_position.x - fTerrainDataPosX) / fScale;
		fy = (MonsterInfo.m_position.y - fTerrainDataPosY) / fScale;
		fz = (MonsterInfo.m_position.z - fTerrainDataPosZ) / fScale;

		sprintf(bufX, "%.4f", fx);
		::WritePrivateProfileString(bufMonsterIndex, "position_X",  bufX, strFileName.c_str());

		memset(bufY, 0, sizeof(bufY));
		sprintf(bufY, "%.4f", fy);
		::WritePrivateProfileString(bufMonsterIndex, "position_Y",  bufY, strFileName.c_str());

		memset(bufZ, 0, sizeof(bufZ));
		sprintf(bufZ, "%.4f", fz);
		::WritePrivateProfileString(bufMonsterIndex, "position_Z",  bufZ, strFileName.c_str());

		// 写怪物的方向.
		iMonsterDir = (MonsterInfo.m_fMonstDir / _2PI) * DIR_STEP;
		iMonsterDir = iMonsterDir + 27;
		iMonsterDir = iMonsterDir % DIR_STEP;

		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", iMonsterDir);
		::WritePrivateProfileString(bufMonsterIndex, "monster_dir",  bufWrite, strFileName.c_str());

		// 写怪物的guid
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_dwMonsterGUID);
		::WritePrivateProfileString(bufMonsterIndex, "monster_guid",  bufWrite, strFileName.c_str());

		// 写怪物的groupid
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iGroupId);
		::WritePrivateProfileString(bufMonsterIndex, "monster_groupid",  bufWrite, strFileName.c_str());

		// 写怪物的teamid
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iTeamId);
		::WritePrivateProfileString(bufMonsterIndex, "monster_teamid",  bufWrite, strFileName.c_str());

		// 写怪物的basseAiId
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iBaseAIId);
		::WritePrivateProfileString(bufMonsterIndex, "monster_BaseAI",  bufWrite, strFileName.c_str());

		// 写怪物的高级ai文件名字
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%s", MonsterInfo.m_strAdvanceAIFile.c_str());
		::WritePrivateProfileString(bufMonsterIndex, "monster_AIFile",  bufWrite, strFileName.c_str());

		// 写怪物的事件文件名字
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%s", MonsterInfo.m_strEvent.c_str());
		::WritePrivateProfileString(bufMonsterIndex, "monster_EvenId",  bufWrite, strFileName.c_str());


        // 怪物编号
		i++;
	
	}

	return 0;
}

void CMonsterEditaction::ClearCounter()
{
	m_dwGUID = 0;
	m_dwMonsterInstanceId = 0;
	m_dwCurEditMonsterId  = 0xffffffff;
	m_dwCurSelMonsterInstanceId = 0xffffffff;
}



	// 设置已经修改
void CMonsterEditaction::SetModifyed(bool bModifyed)
{

	CMonsterOperator* pOperator = new CMonsterOperator;
	getOperatorManager()->addOperator(pOperator);

}

void CMonsterEditaction::GetMonsterIdLookTable()
{

	
	DBC::DBCFile excelDBFileModelName(1);
	std::map<int, std::string> Id_Model_Map;
	

	const DBC::DBCFile::FIELD* pInfo = NULL;	
	//char bufInfo[512];
	
	DBC::DBCFile excelDBFile(1);
	if(!excelDBFile.OpenFromTXT("..\\..\\public\\config\\MonsterAttrExTable.txt"))
	{
		return;
	}
	
	if(!excelDBFileModelName.OpenFromTXT("..\\config\\CharModel.txt"))
	{
		return;
	}

	int iModelCount = excelDBFileModelName.GetRecordsNum();
  	int iRecordCount = excelDBFile.GetRecordsNum(); 
	int iId     = -1;
	for(int iIndex = 0; iIndex < iModelCount; iIndex++)
	{
		pInfo = excelDBFileModelName.Search_Posistion(iIndex, 0);
		iId = pInfo->iValue;
		
		// 外形名字
		pInfo = excelDBFileModelName.Search_Posistion(iIndex, 1);
		Id_Model_Map[iId] = pInfo->pString;
	}
		
	iId     = -1;
	int iLastColum   = excelDBFile.GetFieldsNum();
	for(int i = 0; i < iRecordCount; i++)
	{
		// 读取服务器端
		pInfo = excelDBFile.Search_Posistion(i, 0);
		if(pInfo)
		{
			iId = pInfo->iValue;
		}
		
		// 外形名字
		pInfo = excelDBFile.Search_Posistion(i, 39);
		m_IdNameMap[iId] = Id_Model_Map[pInfo->iValue];//
	}

}


// 得到怪物的位置
Ogre::Vector3   CMonsterEditaction::GetMonsterPos(unsigned long ulInstance)
{
	Ogre::Vector3 pos(0 ,0, 0);

	float fx = 0;
	float fz = 0;

	if(m_MonsterInfoMap.count(ulInstance))
	{
		CMonsterInfo& monsterInfo = m_MonsterInfoMap[ulInstance];
		// 缩放系数
		float fScale = 1.0;
		float fTerrainDataPosX = 0;
		float fTerrainDataPosZ = 0;
		float fTerrainDataPosY = 0;

		fTerrainDataPosX = getTerrainData()->mPosition.x;
		fTerrainDataPosZ = getTerrainData()->mPosition.z;
		fTerrainDataPosY = getTerrainData()->mPosition.y;

		if(getSceneManipulator())
		{
			fScale = getSceneManipulator()->getBaseScale();
			if(abs((float)fScale) < 0.00001)
			{
				fScale = 1.0;
			}
		}

		// 进行场景坐标和逻辑坐标转换
        fx = (monsterInfo.m_position.x - fTerrainDataPosX) / fScale;
		fz = (monsterInfo.m_position.z - fTerrainDataPosZ) / fScale;

		pos.x = fx;
		pos.z = fz;
	}


	return pos;
}



} 
