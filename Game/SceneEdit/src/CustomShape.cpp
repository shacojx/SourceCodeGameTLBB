#include "CustomShape.h"
#include <OgreHardwareVertexBuffer.h>		// 定点缓冲区
#include <OgreHardwareBufferManager.h>		// 缓冲区管理
#include <OgreCamera.h>						// 摄像机
#include <OgreRoot.h>
#include "RegionEditAction.h"
#include "UnreachRegion.h"

namespace WX 
{

	CCustomShape::CCustomShape(void)
	{
		
		SetShapeType();

		m_fRadius = 100;
		setPolygonModeOverrideable(false);

		m_defaultColor = Ogre::ColourValue(0, 1.0, 0, 0.5);

		m_pUnreachRegion = NULL;
		m_Weights = NULL;
		//m_Dist = NULL;
	}

	CCustomShape::~CCustomShape(void)
	{
		ClearVertexBuffer();
		ClearVertexBuffer1();

		m_pUnreachRegion = NULL;
	}

	void CCustomShape::SetRegion(UNREACHREGIONMAP* pRegion)
	{
		m_pUnreachRegion = pRegion;
	}


	// 填充点的信息
	void CCustomShape::FillPointData()
	{
		size_t posCount = m_pointArray.size();

		if ( posCount <= 0 )
		{
			return;
		}

		// 创建定点缓冲区.
		CreateVertexBuffer();
	
		// 更新包围盒以便能够显示线段
		Ogre::Vector3 vmax(Ogre::Vector3::ZERO);
        Ogre::Vector3 vmin(Ogre::Vector3::ZERO);

     	for (size_t ii = 0; ii < posCount; ++ii)
		{
			if(vmin.x > m_pointArray[ii].x)
			{
				vmin.x = m_pointArray[ii].x;
			}

			if(vmin.y > m_pointArray[ii].y)
			{
				vmin.y = m_pointArray[ii].y;
			}

			if(vmin.z > m_pointArray[ii].z)
			{
				vmin.z = m_pointArray[ii].z;
			}
			

			if(vmax.x < m_pointArray[ii].x)
			{
				vmax.x = m_pointArray[ii].x;
			}

			if(vmax.y < m_pointArray[ii].y)
			{
				vmax.y = m_pointArray[ii].y;
			}

			if(vmax.z < m_pointArray[ii].z)
			{
				vmax.z = m_pointArray[ii].z;
			}

		}

		vmin.x -= 100.0f;
        vmax.x += 100.0f;

		vmin.y -= 100.0f;
        vmax.y += 100.0f;

		vmin.z -= 100.0f;
        vmax.z += 100.0f;

		// 求半径
        Ogre::Real sqLen = std::max(vmax.squaredLength(), vmin.squaredLength());
        m_fRadius = Ogre::Math::Sqrt(sqLen);//

		// 更新包围盒
        mBox.setExtents(vmin,vmax);//
        getParentNode()->needUpdate();//

		// 锁定缓冲区， 填充数据.
		Ogre::RGBA *pCol;
		Ogre::ColourValue color;
		bool bSetColor = false;
		float *vertexPos = static_cast<float*>(m_vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
		for (size_t i = 0; i < posCount; ++i)
		{
			*vertexPos++ = m_pointArray[i].x;
			*vertexPos++ = m_pointArray[i].y+10;
			*vertexPos++ = m_pointArray[i].z;
			
			if( !bSetColor)
			{
				// 填充定点颜色
				color = m_defaultColor;
			}

			if(m_LineColroMap.count(i))
			{
				color = m_LineColroMap[i];
				bSetColor = true;
			}
			else
			{
				bSetColor = false;
			}

            pCol = static_cast<Ogre::RGBA*>(static_cast<void*>(vertexPos));
       		// 用diffuse来做为alpha来源
            Ogre::Root::getSingleton().convertColourValue(m_defaultColor, pCol++ ) ;
            vertexPos = static_cast<float*>(static_cast<void*>(pCol));
		}

		m_vbuf->unlock();     

		SetShapeType();

	}
	void CCustomShape::FillPointData1()
	{
		size_t posCount = m_pointArray.size();

		if ( posCount <= 1 )
		{
			return;
		}

		// 创建定点缓冲区.
		CreateVertexBuffer1();

		if( mRenderOp.vertexData->vertexCount == 0 )
		{
			return;
		}

		// 更新包围盒以便能够显示线段
		Ogre::Vector3 vmax(Ogre::Vector3::ZERO);
		Ogre::Vector3 vmin(Ogre::Vector3::ZERO);

		for (size_t ii = 0; ii < posCount; ++ii)
		{
			if(vmin.x > m_pointArray[ii].x)
			{
				vmin.x = m_pointArray[ii].x;
			}

			if(vmin.y > m_pointArray[ii].y)
			{
				vmin.y = m_pointArray[ii].y;
			}

			if(vmin.z > m_pointArray[ii].z)
			{
				vmin.z = m_pointArray[ii].z;
			}


			if(vmax.x < m_pointArray[ii].x)
			{
				vmax.x = m_pointArray[ii].x;
			}

			if(vmax.y < m_pointArray[ii].y)
			{
				vmax.y = m_pointArray[ii].y;
			}

			if(vmax.z < m_pointArray[ii].z)
			{
				vmax.z = m_pointArray[ii].z;
			}

		}

		vmin.x -= 100.0f;
		vmax.x += 100.0f;

		vmin.y -= 100.0f;
		vmax.y += 100.0f;

		vmin.z -= 100.0f;
		vmax.z += 100.0f;

		// 求半径
		Ogre::Real sqLen = std::max(vmax.squaredLength(), vmin.squaredLength());
		m_fRadius = Ogre::Math::Sqrt(sqLen);//

		// 更新包围盒
		mBox.setExtents(vmin,vmax);//
		getParentNode()->needUpdate();//

		// 锁定缓冲区， 填充数据.
		Ogre::RGBA *pCol;
		Ogre::ColourValue color;
		bool bSetColor = false;
		float *vertexPos = static_cast<float*>(m_vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
		for (size_t i = 0; i < posCount-1; ++i)
		{
			for( size_t j = i+1; j < posCount; j ++)
			{
				float w = m_Weights[i + j*posCount];
				if( w != FLT_MAX )
				{
					*vertexPos++ = m_pointArray[i].x;
					*vertexPos++ = m_pointArray[i].y+10;
					*vertexPos++ = m_pointArray[i].z;

					if( !bSetColor)
						color = m_defaultColor;

					if(m_LineColroMap.count(i))
					{
						color = m_LineColroMap[i];
						bSetColor = true;
					}
					else
					{
						bSetColor = false;
					}

					pCol = static_cast<Ogre::RGBA*>(static_cast<void*>(vertexPos));
					Ogre::Root::getSingleton().convertColourValue(m_defaultColor, pCol++ ) ;
					vertexPos = static_cast<float*>(static_cast<void*>(pCol));


					*vertexPos++ = m_pointArray[j].x;
					*vertexPos++ = m_pointArray[j].y+10;
					*vertexPos++ = m_pointArray[j].z;

					if( !bSetColor)
						color = m_defaultColor;

					if(m_LineColroMap.count(i))
					{
						color = m_LineColroMap[j];
						bSetColor = true;
					}
					else
					{
						bSetColor = false;
					}

					pCol = static_cast<Ogre::RGBA*>(static_cast<void*>(vertexPos));
					Ogre::Root::getSingleton().convertColourValue(m_defaultColor, pCol++ ) ;
					vertexPos = static_cast<float*>(static_cast<void*>(pCol));
				}
			}
		}

		m_vbuf->unlock();     

		SetShapeType( Ogre::RenderOperation::OT_LINE_LIST );

	}

	
	// 释放定点缓冲区
	void CCustomShape::ClearVertexBuffer()
	{
		 // 先把上次创建的缓冲区删掉
        if ( mRenderOp.vertexData )
        {
            delete mRenderOp.vertexData;
            mRenderOp.vertexData = 0;
        }
	}

	void CCustomShape::ClearVertexBuffer1()
	{
		// 先把上次创建的缓冲区删掉
		if ( mRenderOp.vertexData )
		{
			delete mRenderOp.vertexData;
			mRenderOp.vertexData = 0;
		}

		delete [] m_Weights; m_Weights = NULL;
		//delete [] m_Dist; m_Dist = NULL;
	}

	// 创建一个顶点缓冲区
	void CCustomShape::CreateVertexBuffer()
    {
		if(0 == m_pointArray.size())
		{
			return;
		}

        // 先把上次创建的缓冲区删掉
        ClearVertexBuffer();

        mRenderOp.vertexData = new Ogre::VertexData;

        mRenderOp.vertexData->vertexCount = m_pointArray.size();
        mRenderOp.vertexData->vertexStart = 0;
      
		Ogre::VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;

		// 声明定点格式
        size_t offset = 0;
      	decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
        decl->addElement(0, offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);

	     m_vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            decl->getVertexSize(0),
            mRenderOp.vertexData->vertexCount,
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

         mRenderOp.vertexData->vertexBufferBinding->setBinding(0, m_vbuf);
    }

	size_t CCustomShape::CalcVertexCount1(size_t nPointsCount)
	{
		if( nPointsCount <= 0 ) return 0;

		return ( nPointsCount + CalcVertexCount1(nPointsCount-1) );
	}

	void CCustomShape::CreateVertexBuffer1()
	{
		if(0 == m_pointArray.size())
		{
			return;
		}

		// 先把上次创建的缓冲区删掉
		ClearVertexBuffer1();

		size_t nVertexCount = CalcVertexCount1( m_pointArray.size()-1 )*2;

		if( m_pUnreachRegion )
		{
			nVertexCount = 0;

			m_Weights = new float[m_pointArray.size()*m_pointArray.size()];
			memset( m_Weights, 0, m_pointArray.size()*m_pointArray.size()*sizeof(float) );
			size_t s;
			for( s = 0; s < m_pointArray.size()-1; s ++ )
			{
				for( size_t t = s+1; t < m_pointArray.size(); t ++ )
				{
					Ogre::Vector3& v1 = m_pointArray[s];
					Ogre::Vector3& v2 = m_pointArray[t];
					UNREACHREGIONMAP::iterator it;
					for( it = m_pUnreachRegion->begin(); it != m_pUnreachRegion->end();it ++ )
					{
						CUnreachRegion* pRe = it->second;
						if( pRe->CheckCross( v1, v2 ) )
							break;
					}
					float w = FLT_MAX;
					if( it == m_pUnreachRegion->end() )
					{
						w = (v1.x-v2.x)*(v1.x-v2.x) + (v1.z-v2.z)*(v1.z-v2.z);
						nVertexCount ++;
					}

					m_Weights[(s) + (t)*(m_pointArray.size())] = w;
					m_Weights[(t) + (s)*(m_pointArray.size())] = w;
				}
				m_Weights[(s) + (s)*(m_pointArray.size())] = 0.0f;
			}

			m_Weights[(s) + (s)*(m_pointArray.size())] = 0.0f;
			m_Weights[0] = 0.0f;

			nVertexCount *= 2;
		}

		mRenderOp.vertexData = new Ogre::VertexData;

		mRenderOp.vertexData->vertexCount = nVertexCount;
		mRenderOp.vertexData->vertexStart = 0;

		if( mRenderOp.vertexData->vertexCount == 0 ) return;

		Ogre::VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;

		// 声明定点格式
		size_t offset = 0;
		decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
		offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
		decl->addElement(0, offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);

		m_vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
			decl->getVertexSize(0),
			mRenderOp.vertexData->vertexCount,
			Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

		mRenderOp.vertexData->vertexBufferBinding->setBinding(0, m_vbuf);

	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 自定义形状的属性操作
	//
	
	//设置画的类型
	void CCustomShape::SetShapeType(Ogre::RenderOperation::OperationType type)
	{
		mRenderOp.operationType = type; 
        mRenderOp.useIndexes    = false; 
	}

	// 添加一个点
	void CCustomShape::AddNewPoint(Ogre::Vector3 point, bool bRecreateBuf)
	{
		m_pointArray.push_back(point);

		if(bRecreateBuf)
		{
			FillPointData();
		}
	}

	// 添加一个点
	void CCustomShape::AddNewPoint1(Ogre::Vector3 point, bool bRecreateBuf)
	{
		m_pointArray.push_back(point);

		if(bRecreateBuf)
		{
			FillPointData1();
		}
	}

	// 设置所有图形的默认颜色.
	void CCustomShape::SetDefaultColor(Ogre::ColourValue color)
	{
		m_defaultColor = color;
	}

	// 设置指定边的颜色
	void CCustomShape::SetLineColor(int iIdex, Ogre::ColourValue color)
	{
		m_LineColroMap[iIdex] = color;
	}



	///////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 作为自定义的形状类， 需要实现的虚函数
	//
	//
	Ogre::Real CCustomShape::getBoundingRadius() const
	{
		return m_fRadius;
	}//

	Ogre::Real CCustomShape::getSquaredViewDepth(const Ogre::Camera* cam) const
	{
		Ogre::Vector3 min, max, mid, dist;

        min = mBox.getMinimum();
        max = mBox.getMaximum();
        mid = ((min - max) * 0.5) + min;
        dist = cam->getDerivedPosition() - mid;

			
        return dist.squaredLength(); 
	}





	// 创建显示
	void CCustomShape::buildPlanePoint()
	{
		POSITION_ARRAY tempPointVector;
		POSITION_ARRAY remainPointVector;

		std::vector<int> remainPointIndexVector;
		std::vector<int> storePointIndexVector;

		// 清空平面数组
		m_planePointArray.clear();

		int iPointCount = m_pointArray.size();
		for(int i = 0; i < iPointCount; i++)
		{
			remainPointIndexVector.push_back(i);
		}

		float fx = 0;
		float fy = 0;
		int	  iReduceCount = 0;

		int iRemainPointCount = remainPointIndexVector.size();

		int iFindCount = 0;
		while(iRemainPointCount > 3)
		{
			iFindCount++;
			iReduceCount = 0;
			storePointIndexVector.clear();
			for(int iIndex = 0; iIndex < iRemainPointCount; iIndex++)
			{
				
				if((iIndex + 2) >= iRemainPointCount)
				{
					// 需要把剩余的点加入到下一次查找的数组中。
					// 计算还剩下多少个点。
					if(iIndex < iRemainPointCount)
					{
						storePointIndexVector.push_back(remainPointIndexVector[iIndex]);
					}
					if(iIndex+1 < iRemainPointCount)
					{
						storePointIndexVector.push_back(remainPointIndexVector[iIndex+1]);
					}

					//iRemainPointCount = iRemainPointCount - iReduceCount;
					remainPointIndexVector.clear();
					remainPointIndexVector.assign(storePointIndexVector.begin(), storePointIndexVector.end());
					iRemainPointCount = remainPointIndexVector.size();
					break;
				}
			
				fx = m_pointArray[remainPointIndexVector[iIndex]].x +  m_pointArray[remainPointIndexVector[iIndex+2]].x;
				fx = fx / 2;

				fy = m_pointArray[remainPointIndexVector[iIndex]].z +  m_pointArray[remainPointIndexVector[iIndex+2]].z;
				fy = fy / 2;

				if(IsPointInRegion(fx, fy))
				{
					//  找到一个三角行
					m_planePointArray.push_back(m_pointArray[remainPointIndexVector[iIndex]]);
					m_planePointArray.push_back(m_pointArray[remainPointIndexVector[iIndex+1]]);
					m_planePointArray.push_back(m_pointArray[remainPointIndexVector[iIndex+2]]);
					storePointIndexVector.push_back(remainPointIndexVector[iIndex]);
					iReduceCount++;
					iIndex++;
				}
				else
				{
					storePointIndexVector.push_back(remainPointIndexVector[iIndex]);
				}
			}

			// 计算还剩下多少个点。
			
			remainPointIndexVector.clear();
			remainPointIndexVector.assign(storePointIndexVector.begin(), storePointIndexVector.end());
			//iRemainPointCount = iRemainPointCount - iReduceCount;
			iRemainPointCount = remainPointIndexVector.size();

			if(10000 < iFindCount)
			{
				break;
			}
		}


		if(3 == iRemainPointCount)
		{
			m_planePointArray.push_back(m_pointArray[remainPointIndexVector[0]]);
			m_planePointArray.push_back(m_pointArray[remainPointIndexVector[1]]);
			m_planePointArray.push_back(m_pointArray[remainPointIndexVector[2]]);
		}
		else
		{
			m_planePointArray.clear();
		}

	}




	
	//////////////////////////////////////////////////////////////////////////////////////////////
	//
	//  点线碰撞函数。
	// 

	// 判断一个点是否在一个区域内。
	bool CCustomShape::IsPointInRegion(float fx, float fy)
	{
		//float fsx = 0;
		//float fex = 0;
		//float fsy = 0;
		//float fey = 0;

		//float fd1 = 0;
		//float fd2 = 0;
		//float fd3 = 0;

		//int   iInterSectCount = 0;

		//int iPointCount = m_pointArray.size();
		//for(int i = 0; i < iPointCount; i++)
		//{
		//	fsx = m_pointArray[i].x;
		//	fsy = m_pointArray[i].z;

		//	if(i == iPointCount - 1)
		//	{
		//		fex = m_pointArray[0].x;
		//		fey = m_pointArray[0].z;
		//	}
		//	else
		//	{
		//		fex = m_pointArray[i+1].x;
		//		fey = m_pointArray[i+1].z;
		//	}

		//	fd1 = abs(fx - fsx);
		//	fd2 = abs(fex - fx);
		//	fd3 = abs(fex - fsx);

		//	if((fd1 + fd2) <= fd3)
		//	{
		//		iInterSectCount++;
		//	}

		//}

		//if(iInterSectCount % 2)
		//{
		//	return true;
		//}

		//return false;//




		float fsx = 0;
		float fex = 0;
		float fsy = 0;
		float fey = 0;

		float t = 0;
		float y = 0;

		int   iInterSectCount = 0;
		int iPointCount = m_pointArray.size();

		for(int i = 0; i < iPointCount; i++)
		{
			t = 0;
			fsx = m_pointArray[i].x;
			fsy = m_pointArray[i].z;

			if(i == iPointCount - 1)
			{
				fex = m_pointArray[0].x;
				fey = m_pointArray[0].z;
			}
			else
			{
				fex = m_pointArray[i+1].x;
				fey = m_pointArray[i+1].z;
			}

			if((abs(fsx - fex) < 0.00001)&&(abs(fsx - fx) < 0.00001))
			{
				iInterSectCount++;
				continue;
			}

			t = (fx - fsx)/(fex - fsx);

			if((t >= 0 )&&(t <=1))
			{
				y = fsy + t*(fey - fsy);

				if((fy - y) < 0)
				{
					iInterSectCount++;
				}
			}
			

		}

		if(iInterSectCount % 2)
		{
			return true;
		}

		return false;//

	}

	
	// 填充数据区
	void CCustomShape::FillAreaPointData()
	{

		size_t posCount = m_planePointArray.size();

		if ( posCount <= 0 )
		{
			 //ClearVertexBuffer();
			return;
		}

		// 创建定点缓冲区.
		CreateAreaVertexBuffer();
	
		// 更新包围盒以便能够显示线段
		Ogre::Vector3 vmax(Ogre::Vector3::ZERO);
        Ogre::Vector3 vmin(Ogre::Vector3::ZERO);

     	for (size_t ii = 0; ii < posCount; ++ii)
		{
			if(vmin.x > m_planePointArray[ii].x)
			{
				vmin.x = m_planePointArray[ii].x;
			}

			if(vmin.y > m_planePointArray[ii].y)
			{
				vmin.y = m_planePointArray[ii].y;
			}

			if(vmin.z > m_planePointArray[ii].z)
			{
				vmin.z = m_planePointArray[ii].z;
			}
			

			if(vmax.x < m_planePointArray[ii].x)
			{
				vmax.x = m_planePointArray[ii].x;
			}

			if(vmax.y < m_planePointArray[ii].y)
			{
				vmax.y = m_planePointArray[ii].y;
			}

			if(vmax.z < m_planePointArray[ii].z)
			{
				vmax.z = m_planePointArray[ii].z;
			}

		}

		vmin.x -= 100.0f;
        vmax.x += 100.0f;

		vmin.y -= 100.0f;
        vmax.y += 100.0f;

		vmin.z -= 100.0f;
        vmax.z += 100.0f;

		// 求半径
        Ogre::Real sqLen = std::max(vmax.squaredLength(), vmin.squaredLength());
        m_fRadius = Ogre::Math::Sqrt(sqLen);//

		// 更新包围盒
        mBox.setExtents(vmin,vmax);//
        getParentNode()->needUpdate();//

		// 锁定缓冲区， 填充数据.
		Ogre::RGBA *pCol;
		Ogre::ColourValue color;
		bool bSetColor = false;
		float *vertexPos = static_cast<float*>(m_vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

		Ogre::ColourValue Areacolor = Ogre::ColourValue(5.0, 0, 1.0, 0.5);
		for (size_t i = 0; i < posCount; ++i)
		{
			*vertexPos = m_planePointArray[i].x;
			vertexPos++;
			*vertexPos = m_planePointArray[i].y+20;
			vertexPos++;
			*vertexPos = m_planePointArray[i].z;
			vertexPos++;
			
			pCol = static_cast<Ogre::RGBA*>(static_cast<void*>(vertexPos));
       		// 用diffuse来做为alpha来源
            Ogre::Root::getSingleton().convertColourValue(Areacolor, pCol++ ) ;
            vertexPos = static_cast<float*>(static_cast<void*>(pCol));
		}

		m_vbuf->unlock();    


		SetShapeType(Ogre::RenderOperation::OT_TRIANGLE_LIST);

	}

	// 创建定点缓冲区
	void CCustomShape::CreateAreaVertexBuffer()
	{
		
		if(0 == m_planePointArray.size())
		{
			return;
		}

        // 先把上次创建的缓冲区删掉
        ClearVertexBuffer();

        mRenderOp.vertexData = new Ogre::VertexData;

        mRenderOp.vertexData->vertexCount = m_planePointArray.size();
        mRenderOp.vertexData->vertexStart = 0;
      
		Ogre::VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;

		// 声明定点格式
        size_t offset = 0;
      	decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
        offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
        decl->addElement(0, offset, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);

	     m_vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            decl->getVertexSize(0),
            mRenderOp.vertexData->vertexCount,
            Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

         mRenderOp.vertexData->vertexBufferBinding->setBinding(0, m_vbuf);
	}

	// 添加一个点
	void CCustomShape::AddAreaNewPoint(Ogre::Vector3 point)
	{
		m_pointArray.push_back(point);
		buildPlanePoint();
		FillAreaPointData();
	}

	void CCustomShape::_updateRenderQueue  (  Ogre::RenderQueue *  queue   )
	{
		if(queue)
		{
			if(mRenderOp.operationType == Ogre::RenderOperation::OT_TRIANGLE_LIST)
			{
				queue->addRenderable(this, Ogre::RENDER_QUEUE_8);
			}
			else
			{
				queue->addRenderable(this, Ogre::RENDER_QUEUE_3);
			}
		}
	}//


}// namespace WX 
