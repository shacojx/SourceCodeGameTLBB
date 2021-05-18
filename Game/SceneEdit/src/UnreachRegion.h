#ifndef __UNREACHREGION_H__
#define __UNREACHREGION_H__

#include "Core/WXPrerequisites.h"
#include <OgreMaterial.h>
#include <OgreVector3.h>
#include "CustomShape.h"

namespace WX 
{
	class SceneManipulator;
	class CCustomShape;

	class CRegionPoint
	{

	public:
		CRegionPoint()
		{
			x = 0;
			y = 0;
		}

		CRegionPoint(float x, float y) : x(x), y(y)
		{
		}

		~CRegionPoint()
		{

		}

	public:
		float x;
		float y;
	};


	//////////////////////////////////////////////////////////////////////////////////////
	//
	// 行走点类。
	//
	class CPointInfo
	{

	public:

		CPointInfo() : m_pSceneNode(NULL)
					 , m_pEntity(NULL)
					 , m_Position(Ogre::Vector3::ZERO)
					 , m_pParentSceneNode(NULL)
					 , m_pSceneManipulator(NULL)
					 , m_ulPointId(0)
					 , m_ulParentId(0)
					 , m_bIfYValued(true)
					
		{
		
		}

		CPointInfo(
					Ogre::Vector3		position, 
					Ogre::SceneNode*	pSceneNode,
					SceneManipulator*	pSceneManipulator,
					unsigned long		ulPointId,
					unsigned long       ulParentId

					) 
					: m_pSceneNode(NULL)
					, m_pEntity(NULL)
					, m_Position(position)
					, m_pParentSceneNode(pSceneNode)
					, m_pSceneManipulator(pSceneManipulator)
					, m_ulPointId(ulPointId)
					, m_ulParentId(ulParentId)
					, m_bIfYValued(true)
					
		{
			CreatePointShape();
		}


		~CPointInfo()
		{

		}

	public:

		SceneManipulator*  m_pSceneManipulator;	    // 用来对ogre操作的句柄。

		Ogre::SceneNode*	m_pParentSceneNode;		// 用来显示区域编辑的总的节点.
		Ogre::SceneNode*	m_pSceneNode;			// 点的场景节点.
		Ogre::Vector3		m_Position;				// 位置。
		Ogre::Entity*		m_pEntity;				// 与场景关联的属性节点

		Ogre::MaterialPtr	m_pMaterialLine;		// 正常的点的材质	


		unsigned long       m_ulPointId;			// 点的id
		unsigned long       m_ulParentId;			// 点的父区域id
		bool				m_bIfYValued;			// Y值是否是有效的

		///////////////////////////////////////////////////////////////////////////////
		//
		// 基本操作函数 
		//

		// 设置位置
		void SetPos(Ogre::Vector3 position);

		// 设置Y是否是有效的
		void SetValuedY( bool b ) { m_bIfYValued = b; }

		// 得到Y是否是有效点
		bool GetValuedY( void ) { return m_bIfYValued; }

		// 得到位置
		Ogre::Vector3 GetPosition();

		// 设置材质
		void SetMaterial(String strMaterialName);

		// 设置父场景节点.
		void SetParentSceneNode(Ogre::SceneNode* pSceneNode);

		// 设置SceneManipulator句柄
		void SetSceneManipulator(SceneManipulator*  SceneManipulator);

		// 设置父区域的id
		void SetParentRegionId(unsigned long ulParentId);

		// 创建点模型.
		unsigned long CreatePointShape();
	
		// 创建场景节点模型.
		unsigned long CreateSceneNode();
	
		// 创建实体模型.
		unsigned long CreateEntity();

		// 创建线实体
		unsigned long CreateLineShape();

		// 释放线实体资源
		void ClearlineShape();

		// 在释放场景之前，释放所有实体
		void DetachEntity();

		// 在释放场景之前，释放场景节点
		void DelSceneNode();

		// 设置选中的材质
		void SetSelMaterial();

	};
	
	

	//////////////////////////////////////////////////////////////////////////////////////////
	//
	// 不可行走区域类
	//
	//
	typedef std::map<unsigned long, CPointInfo*> POINT_MAP;

	class TerrainData;
	class CUnreachRegion
	{

	public:
		unsigned long		m_ulId;					// 这个区域的id.
		POINT_MAP			m_pointMap;				// 点列表.
		POINT_MAP			m_pointMap1;			// 点列表.
													
													// 主要用来变换材质使用
													// 当新选择一个点或者线， 要把前一个选中的材质设置成
													// 正常状态的材质.
		unsigned long		m_ulCurEditPointId;		// 这个区域前一个点的id.
		unsigned long		m_ulCurEditLineId;		// 这个区域前一个线的id.

		unsigned long		m_ulCurMaxPointId;		// 这个区域的点的最大编号id.
		unsigned long		m_ulCurMaxLineId;		// 这个区域的线的最大编号id.

		int				m_bFlyEnable;			// 是否可以通过，用飞飞的哦

		Ogre::SceneNode*	m_pParentSceneNode;		// 用来显示区域编辑的总的节点.
		Ogre::SceneNode*	m_pSceneNode;			// 点的场景节点.
		
		Ogre::SceneNode*	m_pLineSceneNode;		// 线的场景节点.
		Ogre::SceneNode*	m_pAreaSceneNode;		// 面积场景节点.
		Ogre::SceneNode*	m_pLineSceneNode1;		// 线的场景节点.
		Ogre::SceneNode*	m_pAreaSceneNode1;		// 面积场景节点.
		//Ogre::Entity*		m_pLineEntity;			// 与场景关联的属性节点
		CCustomShape*		m_pLineShape;  
		CCustomShape*		m_pLineShape1;
		CCustomShape		m_AreaShape;  
		CCustomShape		m_AreaShape1;  

		SceneManipulator*   m_pSceneManipulator;	// 用来对ogre操作的句柄。


		/////////////////////////////////////////////////////////////////////////////////
		//
		// 区域管理操作
		//
	public:
		// 是否可以用轻功通过
		void SetFlyable( int bFlag ) { m_bFlyEnable = bFlag; }

		// 是否可以用轻功通过
		int GetFlyable( void ) { return m_bFlyEnable; }

		// 设置父场景节点.
		void SetParentSceneNode(Ogre::SceneNode* pSceneNode);

		// 设置SceneManipulator句柄
		void SetSceneManipulator(SceneManipulator*  SceneManipulator);

		// 创建场景节点模型.
		unsigned long CreateSceneNode();

		// 添加新的点
		unsigned long AddNewPoint(Ogre::Vector3 position);

		// 添加新的点
		unsigned long AddNewPoint1(Ogre::Vector3 position);

		// 通过下标， 得到一个点的信息.
		CPointInfo* GetPointInfoByIndex(int iIndex);
		CPointInfo* GetPointInfoByIndex1(int iIndex);

		// 通过id， 得到一个点的信息.
		CPointInfo* GetPointInfoById(unsigned long ulId);
		CPointInfo* GetPointInfoById1(unsigned long ulId);

		// 得到点的个数
		int GetPointCount();
		int GetPointCount1();

		// 设置前一个小球的材质.
		void  SetPrePointMaterial();

		// 创建线形的材质
		Ogre::MaterialPtr	_createIntersectGridMaterial(void);

		// 创建线形数据
		unsigned long CreateLineShape();
		unsigned long CreateLineShape1();

		// 清空线形数据
		void ClearlineShape();
	
		// 清空点的数据
		void ClearPointInfo();

		// 设置区域材质
		void SetRegionMaterial(String strMatrerial);

		// 设置区域正常状态材质材质.
		void SetRegionNorMaterial();
		void SetRegionNorMaterial1();

		// 设置区域选中状态材质.
		void SetRegionSelMaterial();
		void SetRegionSelMaterial1();

		// 设置线的颜色
		void SetLineColor( Ogre::ColourValue color);
		void SetLineColor1( Ogre::ColourValue color);

		// 设置线选中的颜色
		void SetLineSelColor();

		// 设置线的正常的颜色
		void SetLineNorColor();

		// 根据地形改变所有点的高度
		void SetPointHeight( TerrainData* pTerrainData);
		void SetPointHeight1( TerrainData* pTerrainData);

		// 根据地形修改所有线的高度位置
		void SetLineHeight( TerrainData* pTerrainData);
		void SetLineHeight1( TerrainData* pTerrainData);

		// 在释放场景之前，释放所有实体
		void DetachAllEntity();

		// 删除所有子节点
		void DestroyAllChildSceneNode();


		// 判断两条线是否相交.
		bool CheckCross(CRegionPoint &fvMS, CRegionPoint &fvME, CRegionPoint &fvNS, CRegionPoint &fvNE);

		// 判断线段跟本区域是否相交（忽略Y）
		bool CheckCross(Ogre::Vector3 fvmStart, Ogre::Vector3 fvmEnd);

		// 判断两直线是否相交（忽略Y）
		bool CheckCross(Ogre::Vector3 fvmStart, Ogre::Vector3 fvmEnd, Ogre::Vector3 fvmStart1, Ogre::Vector3 fvmEnd1);

		// 两个点是否相等（忽略Y）
		bool fvEuqual(const Ogre::Vector3& fv1, const Ogre::Vector3& fv2);

		// 是否可以添加一个新点.
		bool CanAddNewPoint(Ogre::Vector3 position);
		
		// 求一个二维向量
		CRegionPoint fvSub(CRegionPoint fv1, CRegionPoint fv2);

		Ogre::Vector3 fvSub(Ogre::Vector3& fv1, Ogre::Vector3& fv2);

		// 求二维向量的行列式.
		float fvMulti(CRegionPoint fv1, CRegionPoint fv2);

		float fvMulti(Ogre::Vector3& fv1, Ogre::Vector3& fv2);

		// 求最大值。
		float max(float f1, float f2);

		// 求最小值
		float min(float f1, float f2);

		// 判断是否可以改变一个小球的位置。
		bool CanChangePointPos(int iIndex, Ogre::Vector3 position);


		// 指定id指定位置添加一个小球
		void AddNewPoint(unsigned long ulId, Ogre::Vector3 position);

		void AddNewPoint1(unsigned long ulId, Ogre::Vector3 position);

		// 通过id 删除一个小球
		void DelPointById(unsigned long ulId);
		void DelPointById1(unsigned long ulId);

		// 从尾部 删除一个小球
		void DelPointFromTail(unsigned long ulId);
		void DelPointFromTail1(unsigned long ulId);

		// 鼠标能否选中这个区域
		bool IsMouseSel(float x, float y);

		// 是否选中一个三角型
		bool IsSelTri(Ogre::Vector3 dir, Ogre::Vector3 orig, Ogre::Vector3 v1, Ogre::Vector3 v2, Ogre::Vector3 v3);


		// 更新面积区域
		void ReShapeArea();
		void ReShapeArea1();

		// 区域是否合法
		bool IsRegionValidate();

		// 隐藏区域节点。
		void HideAreaNode(bool bHide);
		void HideAreaNode1(bool bHide);

		// 隐藏线节点。
		void HideLineNode(bool bHide);

		// 改变没有深度缓冲区的材质
		void ChangeNoDeepMaterial(bool bOpenDeepBuf = false);
		



	public:
		CUnreachRegion(void);

		CUnreachRegion(unsigned long ulId, Ogre::SceneNode* pParentSceneNode, SceneManipulator* pSceneManipulator);
		
		~CUnreachRegion(void);
	};
}

#endif //__UNREACHREGION_H__