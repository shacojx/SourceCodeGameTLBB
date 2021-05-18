#ifndef __STALLINFOACTION_H__
#define __STALLINFOACTION_H__
#pragma once


#include "Action.h"
#include "HitInfo.h"
#include "ScanLine.h"
#include <OgreMaterial.h>
#include <map>
#include ".\LogicDataStructDef.h"
#include "CustomShape.h"
#include ".\baserendershape.h"

namespace WX 
{
	class CStallInfoData
	{
	public:

		CStallInfoData();
		~CStallInfoData();

	public:

		unsigned char	m_bCanStall;	//是否可以摆摊
		unsigned char	m_iTradeTax;	//交易税
		unsigned long	m_dwPosTax;		//摊位费
		unsigned char	m_iExtraInfo;	//附加信息

		int     m_iPosX;				// 摊位位置x
		int     m_iPosY;				// 摊位位置y

	public:
		
		CBaseRenderShape	m_RectRenderShape;		// 显示信息.
		
		// 创建显示图形.
		void CreateShape();		

		// 赋值语句重载函数
		CStallInfoData& operator=(const CStallInfoData& stallInfo);

	};




	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// action
	//
	class CStallInfoAction : public Action
	{


	public:

		enum OPERATE_TYPE
		{
			SET_STALL_INFO = 0,
			DEL_STALL_INFO,
			VIEW_STALL_INFO,
		};

		int m_iOperateType;

	public:
		CStallInfoAction(SceneManipulator* sceneManipulator);
		~CStallInfoAction(void);



	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 键盘输入操作。
		//

		// 记录键盘的按键状态.
		unsigned long     m_FuncKeyState;

		// 是否显示场景.
		bool  m_bIsShow;

			
		// 设置按下的功能键
		void SetFuncKey(unsigned long ulFuncKey);

		//  清空功能键.
		void ClearFuncKey();


	public:
		////////////////////////////////////////////////////////////////////////////////////
		//
		// 存盘和读盘操作
		//

		// 版本号
		unsigned long m_dwVersion;

		// 从区域读取文件
		unsigned long ReadStallInfoFromFile(std::string strFileName);
		
		// 把区域存储到文件
		unsigned long SaveStallInfoToFile(std::string strFileName);

	public:

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// action 操作。
		//
		
		// 得到这个action的名字.
		const String& getName(void) const;

		// 设置操作参数.
		void setParameter(const String& name, const String& value);

	public:

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 响应相应的鼠标操作.
		//

		void OnMouseMove(const Point& pt);

		void OnMouseLBnDown(const Point& pt);

		void OnMouseLBnUp(const Point& pt);

		void OnMouseRBnDown(const Point& pt);

		void OnMouseRBnUp(const Point& pt);

		void OnMouseDrag(const Point& pt);

	public:
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//
		// 与渲染层相关的操作.
		//
		
		Ogre::SceneNode*	m_pSceneNode;			// 用来显示区域编辑的总的节点.

		// 创建区域编辑的场景节点
		void CreateSceneNode();

		// 隐藏区域编辑场景节点
		void HideScene(bool bHide);

		// 摊位大小
		unsigned long	m_dwStallWidth;
		unsigned long 	m_dwStallHeight;

		// 摊位信息.
		CStallInfoData*	m_pStallInfoData;

		// 当前摊位画刷.
		CStallInfoData  m_curStallInfoBrush;

		// 两次鼠标按下的点
		Ogre::Vector3	m_PrePoint;
		Ogre::Vector3   m_CurPoint;

		// 当前生成的矩形.
		AREARECT m_AreaRect;

		// 显示模型
		CBaseRenderShape m_RenderShape;

		// 把当前的点转换成规格矩形.
		void NormalRect();

		// 创建四方形
		void CreateShowRect();


		// 设置摊位大小
		void SetStallSize(unsigned long dwWidth, unsigned long dwHeight);

		// 创建摊位
		void CreateStallInfo();

		// 创建一个显示矩形
		void CreateOneStallInfo(int ix, int iy, CStallInfoData& stallInfo);
			


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 设置摊位信息.
		//

		// 做画矩形的鼠标按下操作
		void DoDrawLBnDown(const Point& pt);

		// 做画矩形的鼠标托拽操作
		void DoDrawLBnDrag(const Point& pt);

		// 做画矩形的鼠标抬起操作
		void DoDrawLBnUp(const Point& pt);


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 删除摊位信息.
		//

		// 做删除矩形的鼠标按下操作
		void DoDelLBnDown(const Point& pt);

		// 做删除矩形的鼠标托拽操作
		void DoDelLBnDrag(const Point& pt);

		// 做删除矩形的鼠标抬起操作
		void DoDelLBnUp(const Point& pt);

		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 查看摊位信息.
		//

		// 查看摊位信息
		void DoViewMouseMove(const Point& pt);


		// 得到地形上的坐标点
		bool  GetTerrainPos(const Point& pt, Ogre::Vector3& position);

		
    protected:
        
		// 鼠标操作的虚函数.
        void _onMove(const Point& pt);
        void _onBegin(const Point& pt);
        void _onDrag(const Point& pt);
        void _onEnd(const Point& pt, bool canceled);

		// 当前激活active操作.
        void _onActive(bool active);

	};

}

#endif //__STALLINFOACTION_H__