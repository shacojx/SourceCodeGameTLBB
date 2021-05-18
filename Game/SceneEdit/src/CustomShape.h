#ifndef __CUSTOMSHAPE_H__
#define __CUSTOMSHAPE_H__

#include <OgreSimpleRenderable.h>
#include <map>

namespace WX 
{
	typedef std::vector<Ogre::Vector3> POSITION_ARRAY;
	typedef std::map<int, Ogre::ColourValue> LINE_COLOR_MAP;

	class CUnreachRegion;
	typedef std::map<unsigned long, CUnreachRegion*> UNREACHREGIONMAP;

	class CCustomShape : public Ogre::SimpleRenderable
	{
		public:
			CCustomShape(void);
			~CCustomShape(void);

		public:

			/**************/
			///////////////////////////////////////////////////////////////////////////////////////////////
			//
			// 作为自定义的形状类， 需要实现的虚函数
			//
			//

			///////////////////////////////////////////////////////////////
			// 属性变量
			//

			// 自定义形状的半径大小
			float m_fRadius;

			// 得到绑定的半径
			Ogre::Real getBoundingRadius() const;

			Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const;//

			virtual void _updateRenderQueue  (  Ogre::RenderQueue *  queue   );
 


		public:

			/**************/
			//////////////////////////////////////////////////////////////////////////////////////////////
			//
			// 数据缓冲区操作。
			//

			/////////////////////////////////////////////////
			// 属性数据
			//

			//  定点数组
			Ogre::HardwareVertexBufferSharedPtr m_vbuf;

			// 线的点列表
			POSITION_ARRAY	m_pointArray;
			UNREACHREGIONMAP* m_pUnreachRegion;
			float*						m_Weights;
			//float*						m_Dist;

			void SetRegion(UNREACHREGIONMAP* pRegion);

			POSITION_ARRAY& GetPointArray() { return m_pointArray; }

			// 填充数据区
			void FillPointData();
			void FillPointData1();

			// 创建定点缓冲区
			void CreateVertexBuffer();
			void CreateVertexBuffer1();

			size_t CalcVertexCount1(size_t nPointsCount);

			// 释放定点缓冲区
			void ClearVertexBuffer();
			void ClearVertexBuffer1();



			/***************/
			///////////////////////////////////////////////////////////////////////////////////////////////////
			//
			// 自定义形状的属性操作
			//

			///////////////////////////////////////
			//属性变量

			// 形状的默认颜色.
			Ogre::ColourValue m_defaultColor;

			// 边颜色列表
			LINE_COLOR_MAP m_LineColroMap;


			// 设置显示的类型.
			void SetShapeType(Ogre::RenderOperation::OperationType type = Ogre::RenderOperation::OT_LINE_STRIP);

			// 设置图形的默认颜色。
			void SetDefaultColor(Ogre::ColourValue color);

			// 设置指定边的颜色
			void SetLineColor(int iIdex, Ogre::ColourValue color);



			/**************/
			////////////////////////////////////////////////////////////////////////////////////////////////
			//
			// 逻辑数据操作
			//
					
			// 添加一个点
			void AddNewPoint(Ogre::Vector3 point, bool bRecreateBuf = true);
			void AddNewPoint1(Ogre::Vector3 point, bool bRecreateBuf = true);



			///////////////////////////////////////////////////////////////////////////////////////////////
			//
			// 显示平面操作
			//

			// 平面的点列表
			POSITION_ARRAY	m_planePointArray;
			
			// 创建显示
			void buildPlanePoint();

			// 填充数据区
			void FillAreaPointData();

			// 创建定点缓冲区
			void CreateAreaVertexBuffer();

			void AddAreaNewPoint(Ogre::Vector3 point);



			//////////////////////////////////////////////////////////////////////////////////////////////
			//
			//  点线碰撞函数。
			// 
			bool IsPointInRegion(float fx, float fy);


			
			




		
	};


}

#endif //__CUSTOMSHAPE_H__