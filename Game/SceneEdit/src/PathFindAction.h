#pragma once

#include "Action.h"
#include "HitInfo.h"
#include "ScanLine.h"
#include <OgreMaterial.h>


namespace WX 
{
#define		FUNCTION_KEY        0X00000000
#define		FUNC_KEY_SHIFT		0X00000001
#define		FUNC_KEY_CTRL		0X00000002
#define		FUNC_KEY_ALT		0X00000004



	class CUnreachRegion;

	typedef std::map<std::string, Ogre::MaterialPtr> OGRE_MATERIAL_MAP;
	typedef std::map<unsigned long, CUnreachRegion*> UNREACHREGIONMAP;


	class CPathFindAction : public Action
	{
	public:

		// 材质列表
		OGRE_MATERIAL_MAP m_materialMap;

		//npc巡逻域列表
		UNREACHREGIONMAP  m_UnreachRegionMap;

		// 记录键盘的按键状态.
		unsigned long     m_FuncKeyState;

		// 是否显示场景.
		bool  m_bIsShow;

	public:

		CPathFindAction(SceneManipulator* sceneManipulator);
		~CPathFindAction(void);

	public:

		HitInfo mTerrainHitInfo;

	public:
		////////////////////////////////////////////////////////////////////////////////////
		//
		// 存盘和读盘操作
		//

		// 从区域读取文件
		unsigned long ReadRegionFromFile(std::string& strFileName);
		unsigned long ReadRegionFromFileVer0(std::string& strFileName);
		// 把区域存储到文件
		unsigned long SaveRegionToFile(std::string strFileName);

		//只输出,并不读取
		int* mWorld;
		int mWidth,mHeight;
		struct NavMapHeader
		{
			unsigned short width;
			unsigned short height;
		};

	public:

		////////////////////////////////////////////////////////////////////////////////////
		//
		// 编辑不可行走区域的数据
		//
		Ogre::MaterialPtr	m_pMaterialPoint;		// 正常的点的材质			
		Ogre::MaterialPtr	m_pMaterialPointSel;	// 选中点的材质
		Ogre::MaterialPtr	m_pMaterialPointTemp;	// 临时材质
		Ogre::MaterialPtr	m_pMaterialLine;		// 线的材质
		Ogre::MaterialPtr	m_pMaterialLineSel;		// 选中线的材质
		Ogre::MaterialPtr	m_pMaterialRegion;		// 区域材质
		Ogre::MaterialPtr	m_pMaterialRegionSel;	// 选中区域的材质。

		Ogre::SceneNode*	m_pSceneNode;			// 用来显示区域编辑的总的节点.


		unsigned long m_ulCurMaxId;					// 当前最大的区域ID
		unsigned long m_ulCurEditRegionId;			// 当前正在编辑的区域的id
		unsigned long m_ulCurEditPointId;			// 当前正在编辑的点的id
		unsigned long m_ulCurEditLineId;			// 当前正在编辑的线的id

		// 得到当前正在编辑的区域的id
		int GetCurEditRegionId()
		{
			return m_ulCurEditRegionId;
		}

		//设置当前区域id
		void SetCurEditRegionId(unsigned long ulRegionId)
		{
			m_ulCurEditRegionId = ulRegionId;
		}

		// 得到当前正在编辑的点的id
		int GetCurEditPointId()
		{
			return m_ulCurEditPointId;
		}
		//设置当前点id
		void SetCurEditPointId(unsigned long ulPointId)
		{
			m_ulCurEditPointId = ulPointId;
		}

		// 得到当前正在编辑的线的id
		int GetCurEditLineId()
		{
			return m_ulCurEditLineId;
		}//

		//设置当前线id
		void SetCurEditLineId(unsigned long ulLineId)
		{
			m_ulCurEditLineId = ulLineId;
		}




	public:
		////////////////////////////////////////////////////////////////////////////////////////
		//
		// 区域操作
		//

		// 添加一个新的区域
		unsigned long AddNewRegion();

		void ShowUnreachRegion();

		// 通过指定的id， 删除一个区域
		unsigned long DelRegion(unsigned long ulId);

		// 清除所有区域数据.
		void ClearRegionData();

		// 根据指定的id， 得到区域
		CUnreachRegion* GetRegionById(unsigned long ulId);

		// 选择一个区域
		void SelRegion(unsigned long ulId, bool bIsChangeCamera = true);

		// 不选择一个区域.
		void UnSelRegion(unsigned long ulId);

		// 隐藏一个区域的面积
		void HideRegionArea(unsigned long ulRegionId, bool bHide);

		// 当前的区域是否合法
		bool IsRegionValidate(unsigned long ulRegionId);

		// 重新创建显示的面积区域
		void ReshapeArea(unsigned long ulRegionId);

		/////////////////////////////////////////////////////////////////////////////////////////
		//
		// 点操作
		//

		// 当前操作之前的点的位置.
		Ogre::Vector3 m_preDoPointPos;

		// 当前操作之前的点的位置.
		Ogre::Vector3 m_curPointPos;

		// 在当前编辑的区域中添加一个新的点
		unsigned long AddNewPoint(Ogre::Vector3 position);

		// 指定区域id， 指定点的id， 指定位置， 添加一个小球.
		void AddNewPoint(unsigned long ulPointId, Ogre::Vector3 position);

		// 指定区域id， 指定点的id， 指定位置， 删除一个小球.
		void DelPoint(unsigned long ulPointId);


		// 操作点
		bool EditPoint(const Point& pt);

		// 选择一个点.
		void SelPoint(unsigned long ulPointId);

		// 改变当前编辑的点的位置, 参数是鼠标坐标
		void ChangeCurEditPointPos(const Point& pt);

		// 改变指定区域中指定的点的位置
		void ChangeCurEditPointPos(unsigned long ulPointId, Ogre::Vector3 position);


		// 记录当前操作前一次点的位置.
		void RecordPreDoPointPos();



		/////////////////////////////////////////////////////////////////////////////////////////
		//
		// 线操作
		//

		// 判断两条线是否相交.
		//bool CheckCross(fVector2 fvMS, fVector2 fvME,fVector2 fvNS, fVector2 fvNE);



	public:

		/////////////////////////////////////////////////////////////////////////////////////////
		//
		// 场景的操作
		//

		// 前一次鼠标操作类型.

		int m_iPreOperatorType;


		// 初始化区域编辑需要的一些数据
		void InitPathFind();

		// 设置其他编辑的鼠标点小球的材质
		void SetOldHitPointMaterial();

		// 创建区域编辑的场景节点
		void CreateSceneNode();

		// 隐藏区域编辑场景节点
		void HideScene(bool bHide);

		// 根据地形的高度改变编辑的区域。
		void SetRegionHeight();

		// 设置按下的功能键
		void SetFuncKey(unsigned long ulFuncKey);

		//  清空功能键.
		void ClearFuncKey();



	public:

		// 得到这个区域的名字.
		const String& getName(void) const;

	public:

		// 摄像机的位置操作.
		void SetCamera(float fx, float fy);
		void SetCamera();

		// 设置操作参数.
		void setParameter(const String& name, const String& value);

	protected:

		// 鼠标操作的虚函数.
		void _onMove(const Point& pt);
		void _onBegin(const Point& pt);
		void _onDrag(const Point& pt);
		void _onEnd(const Point& pt, bool canceled);

		// 当前激活active操作.
		void _onActive(bool active);



	public:

		void Test_AddRegion();

	};

}