
#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "WXEditorFrame.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include <wx/pen.h>
    #include <wx/dcscreen.h>
    #include <wx/dcmemory.h>
    #include <wx/scrolwin.h>
    #include <wx/toolbar.h>
    #include <wx/statusbr.h>
    #include <wx/stattext.h>
    #include <wx/button.h>
#endif

#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/spinctrl.h>
#include <wx/busyinfo.h>
#include <wx/config.h>
#include <wx/stockitem.h>
#include <wx/filename.h>

// copy paste
#include <wx/dataobj.h>
#include <wx/clipbrd.h>

#include "WXEditorFrame.h"
#include "WXSceneCanvas.h"
#include "BrushSelector.h"
#include "ModelSelector.h"
#include "ActorSelector.h"
#include "RegionEditDlgSelector.h"					//不可行走区域编辑对话框.
#include "ParticleSystemSelector.h"
#include "SceneBrowser.h"
#include "ObjectPropertyEditor.h"
#include "TerrainSelections.h"
#include "LiquidCreatorDialog.h"
#include "SceneManipulator.h"
#include "WXOperatorManager.h"
#include "Action.h"
#include "NewSceneDialog.h"
#include "SceneInfoMapDialog.h"
#include "SettingDialogs.h"

#include "RegionEditAction.h"

#include "Core/WXObject.h"
#include "Core/WXSceneInfo.h"
#include "Core/FrameStatsListener.h"
#include "Core/WXObjectFactoryManager.h"
#include "Core/WXStaticEntityObject.h"
#include "Core/WXModelObject.H"

#include <OgreMatrix4.h>
#include <OgreString.h>


// ----------------------------------------------------------------------------
// wxDockIt headers
// ----------------------------------------------------------------------------

#include <wx/layoutmanager.h>
#include <wx/dockwindow.h>
#include <wx/dockhost.h>
#include <wx/accel.h>

//-----------------------------------------------------------------------------
// File header
//-----------------------------------------------------------------------------
#include <wx/file.h>

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------
#include "OgreStringConverter.h"
#include "OgreCamera.h"

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

#define OFFEET 0.9f

///////////////////////////////////////////////////////////////////////////////////
//
// 建筑物可行走面数据
//


//
// 得到建筑物可行走面的信息库
// 
// return 0 -- 成功。非 0 -- 失败 具体错误信息待定.
//
DWORD FairyEditorFrame::GetBuildingCollisionInfo()
{

	FILE* pFile = NULL;
	std::string strBuildingCollisionName = "";


	pFile = ::fopen("..\\building_collision\\data.ini", "r+");
	if(NULL == pFile)
	{
		return 1;
	}

	m_buildingCollisionInfoLibMap.clear();

	char bufInfo[512];
	TRI_INFO_VECTOR triVector;
	while(!feof(pFile))
	{
		memset(bufInfo, 0, sizeof(bufInfo));
		::fgets(bufInfo, sizeof(bufInfo), pFile);
		strBuildingCollisionName = bufInfo;
		//m_buildingCollisionInfoLibMap[strBuildingCollisionName] = 1;
		m_buildingCollisionInfoLibMap[bufInfo] = triVector;
	}
	::fclose(pFile);
	return 0;
}


//
// 把注册到地图中的行走面信息存储到文件。
// 文件的名字与场景文件的名字相同， 扩展名字为bcol.
//
// 参数：文件名字， 不带扩展名， 带路径信息。
void FairyEditorFrame::SaveBuildingCollisionInfo(wxString strCollisionFile)
{
	FILE* pFile = NULL;

	std::string strStlFileName = strCollisionFile;
	
	pFile = ::fopen(strStlFileName.c_str(), "wb");
	if(NULL == pFile)
	{
		return;
	}

	// 先写版本号
	DWORD dwVersion = 0xff000001;
	::fwrite(&dwVersion, sizeof(dwVersion), 1, pFile);


	// 写三角形列表的个数。
	int iCount = m_TriInMapInfoMap.m_triInfoInMap.size();
	::fwrite(&iCount, sizeof(iCount), 1, pFile);


	TRI_INFO_MAP::iterator itEnd;
	TRI_INFO_MAP::iterator it;


	POINT_2D	pos;				// 三角形列表注册的位置。
	int			iTriCountInVector;	// 三角形列表中三角形的个数。
	TRI_INFO	triInfo;			// 三角形的结构。
	
	itEnd  = m_TriInMapInfoMap.m_triInfoInMap.end();
	for(it = m_TriInMapInfoMap.m_triInfoInMap.begin(); it != itEnd; it++)
	{
		// 写三角形注册的位置。
		pos = it->first;
		::fwrite(&pos, sizeof(pos), 1, pFile);


		// 写注册在这个位置上的三角形的个数。
		TRI_INFO_VECTOR &triVector = it->second;
		iTriCountInVector = triVector.size();
		::fwrite(&iTriCountInVector, sizeof(iTriCountInVector), 1, pFile);

		for(int i = 0; i < iTriCountInVector; i++)
		{
			// 写每一个三角形的信息。
			triInfo = triVector[i];
			::fwrite(&triInfo, sizeof(triInfo), 1, pFile);
		}
		
	}

	::fclose(pFile);

}

// 通过物体的指针得到物体模型的名字。
//
// 参数：静态物体模型的名字.
// 
// return 物体的mesh文件名字.
//std::string FairyEditorFrame::GetMeshNameByObjectPtr(const WX::ObjectPtr& object)
//{
//	wxString strMeshName = "";
//
//	return strMeshName;
//}//


//
// 创建建筑物的行走面数据。
//
void FairyEditorFrame::CreateBuildingCollisionData()
{

   std::string			strTemp;			// 
   Ogre::String			meshName;			// 静态物体对应的mesh模型文件.
   Ogre::String			meshFile;			// 静态物体对应的mesh模型文件.
   Ogre::String			meshPath;			// 静态物体对应的mesh模型文件.
  
   Ogre::Vector3		position;			// 静态物体摆放的位置.
   Ogre::Quaternion		rotateQuaternion;	// 旋转数据.
   Ogre::Vector3		scale;				// 缩放数据.

   int	iStrLen = 0;	// mesh 文件名字的长度
   int  iPos	= 0;	// mesh 文件名字中 ‘ / ’的位置.

   // 清空以前的数据。
   m_TriInMapInfoMap.m_triInfoInMap.clear();


   // 查找每一个带有行走面数据的物体
   const WX::Scene::Objects& mObjects = GetSceneManipulator()->getSceneInfo()->getObjects();
   for (WX::Scene::Objects::const_iterator it = mObjects.begin(); it != mObjects.end(); ++it)
   {
       const WX::ObjectPtr& object = *it;
       //if ( ("StaticEntity" != object->getType())&&("Model" != object->getType()))
       //{
       //   continue;      
       //}//
	   //if ( (WX::StaticEntityObject::msType != object->getType())
		  // &&(WX::ModelObject::msType !=  object->getType()))
    //   {
    //      continue;      
    //   }//

	   if(WX::StaticEntityObject::msType != object->getType())
	   {
          continue;      
       }//

		meshName = WX::VariantCast<Ogre::String>(object->getProperty("mesh name"));
	    Ogre::StringUtil::splitFilename(meshName, meshFile, meshPath);
		iPos     = meshFile.find_last_of(".");
   		
		std::string strName;
		strName.empty();
		if(iPos> 0)
		{
			strName  = meshFile.substr(0, iPos);
		}
		else
		{
			strName = meshFile;
		}

		strName  = strName + ".cll";

		//FairyEditorFrame::BUILDING_COLLISION_MAP::iterator it1;
		//FairyEditorFrame::BUILDING_COLLISION_MAP::iterator itEnd1;

		//itEnd1 = m_buildingCollisionInfoLibMap.end();
		//for(it1 = m_buildingCollisionInfoLibMap.begin(); it1 != itEnd1; it1++)
		//{
		//	std::string strCllFileName = it1->first;
		//	if(strName == strCllFileName)
		//	{
		//		break;
		//	}
		//}

		//if(it1 == itEnd1)
		//{
		//	continue;
		//}
		////if(0 == m_buildingCollisionInfoLibMap.count(strName))
		////{
		////	continue;
		////}//

		// 得到模型的平移， 旋转和缩放数据。
		position         = WX::VariantCast<Ogre::Vector3>(object->getProperty("position"));
		rotateQuaternion = WX::VariantCast<Ogre::Quaternion>(object->getProperty("orientation"));
		scale			 = WX::VariantCast<Ogre::Vector3>(object->getProperty("scale"));

		// 创建变换矩阵。
		Ogre::Matrix4 TransformMatrix;
		BuildTransformMatrix(TransformMatrix, position, rotateQuaternion, scale);

		// 先把行走面数据注册到地图中去。
		RegisterCollisionToMap(strName, TransformMatrix);

    }
}


//
// 根据输入的平移， 旋转， 缩放分量创建出位置变换矩阵.
//
void FairyEditorFrame::BuildTransformMatrix(Ogre::Matrix4& Matrix,  const Ogre::Vector3& position, const Ogre::Quaternion rotate, const Ogre::Vector3 scale)
{

	Ogre::Matrix4 posMatrix;
	Ogre::Matrix4 scaleMatrix;
	Ogre::Matrix4 rotateMatrix(rotate);

	posMatrix = Ogre::Matrix4::IDENTITY;
	posMatrix.setTrans(position);

	scaleMatrix = Ogre::Matrix4::IDENTITY;
	scaleMatrix.setScale(scale);

	// 最终的变换矩阵.	
	Matrix = posMatrix * rotateMatrix * scaleMatrix;


}


//
// 根据建筑物的名字把建筑物的行走面注册到地图中.
//
void FairyEditorFrame::RegisterCollisionToMap(const std::string& strMeshName, Ogre::Matrix4& TransformMatrix)
{
	std::string strCollisionName = strMeshName;

	//if(0 == m_buildingCollisionInfoLibMap.count(strCollisionName))
	//{
	//	// 这个物体没有行走面。
	//	return;
	//}//

	std::string strFileName = "..\\building_collision\\" + strCollisionName;
	FILE* pFile = NULL;
	pFile = ::fopen(strFileName.c_str(), "rb");
	if(NULL == pFile)
	{
		return;
	}
	::fclose(pFile);


	// 得到行走面的三角形数据。
	TRI_INFO_VECTOR &TriInfoVector = m_buildingCollisionInfoLibMap[strCollisionName];
	if(0 == TriInfoVector.size())
	{
		//std::string strFileName = "..\\building_collision\\" + strCollisionName;
		if(0 != LoadCollisionDataFromFile(strFileName, TriInfoVector))
		{
			return;
		}
	}

	// 计算建筑物行走面的预处理数据
	//WriteCollisionDataToTriMap(TriInfoVector, TransformMatrix);
	WriteCollisionDataToTriMap_Rectangle(TriInfoVector, TransformMatrix);

}



//
// 从建筑物的行走面数据文件中读取三角形的信息。
//
// 参数： std::string strFileName 建筑物行走面的数据文件。
//
// return： 0 - 成功。 非0 失败。具体含义根据需要待定。
DWORD FairyEditorFrame::LoadCollisionDataFromFile(std::string strFileName, TRI_INFO_VECTOR &TriInfoVector)
{
	
	FILE* pFile = NULL;
	pFile = ::fopen(strFileName.c_str(), "rb");
	if(NULL == pFile)
	{
		return 1;
	}

	DWORD dwVersion = 0;
	int   iTriCount = 0;

	TRI_INFO triInfo;
	TriInfoVector.clear();

	::fread(&dwVersion, sizeof(dwVersion), 1, pFile);
	::fread(&iTriCount, sizeof(iTriCount), 1, pFile);

	for(int i = 0; i < iTriCount; i++)
	{
		::fread(&triInfo, sizeof(triInfo), 1, pFile);
		TriInfoVector.push_back(triInfo);
	}

	::fclose(pFile);

	return 0;
}



//
// 
// 把场景坐标转换成游戏逻辑坐标。
//
void FairyEditorFrame::TranslateSceneToGameLogic(float &x, float &y, float &z, Ogre::Matrix4& TransformMatrix)
{

	Ogre::Vector3 pos;

	pos.x = x;
	pos.y = y;
	pos.z = z;
	pos = TransformMatrix * pos;

	if(GetSceneManipulator())
	{
		pos.x = (pos.x - GetSceneManipulator()->getTerrainData()->mPosition.x) / GetSceneManipulator()->getTerrainData()->mScale.x;
		pos.y = (pos.y - GetSceneManipulator()->getTerrainData()->mPosition.y) / GetSceneManipulator()->getTerrainData()->mScale.y;
		pos.z = (pos.z - GetSceneManipulator()->getTerrainData()->mPosition.z) / GetSceneManipulator()->getTerrainData()->mScale.z;
	}
	
	
	x = pos.x;
	y = pos.y;
	z = pos.z;
}

//
// 把三角形信息写到地图的三角形与位置的映射表中。
//
// 参数1： 三角形列表。
// 
// 参数2： 三角形的变换矩阵。
//
void FairyEditorFrame::WriteCollisionDataToTriMap(TRI_INFO_VECTOR &TriInfoVector, Ogre::Matrix4& TransformMatrix)
{

	typedef std::vector<SCANLINE_INFO> SCAN_LINE_VECTOR;

	SCANLINE_INFO    Scanline_info;			// 扫描信息.
	SCAN_LINE_VECTOR ScanLineVector;		// 每一个三角形的扫描信息.
	
	int iCount = 0;			//每一个物体的行走面三角形个数
	
	POINT_3D  Info1;		// 行走面的第一个点。
	POINT_3D  Info2;        // 行走面的第二个点。
    POINT_3D  Info3;		// 行走面的第三个点.

	float       fV1x;		// 用来求三角形三条边的参数方程。 
	float       fV1z;		//
	float       fV2x;		// fV1x = v1.x - v2.x  fV1z = v1.z - v2.z
	float       fV2z;		// fV2x = v2.x - v3.x  fV2y = v2.z - v3.z
	float       fV3x;		// fV3x = v3.x - v1.x  fV3y = v3.z - v1.z
	float       fV3z;		//

	float       ft;			// 参数方程比率t

	int iScanStart = 0;		// 扫描线的开始点。
	int iScanEnd   = 0;		// 扫描线的结束点。

	float fFindStart;		// 每次扫描找到的开始点。
	float fFindEnd;			// 每次扫描找到的结束点。

	float fMaxX = 0;		// 三角形x的取值范围.
	float fMinX = 0;		//

	float fValue = 0;		// 找到的值。

	POINT_2D point2DKey;

	// 需要扫描的三角形。
	TRI_INFO tri;

	int iFaceCount = TriInfoVector.size();

	TRI_INFO Tri;
	for(int i = 0; i < iFaceCount; i++)
	{
		ScanLineVector.clear();
		
		Tri = TriInfoVector[i];
		Info1.fx = TriInfoVector[i].f1x;
		Info1.fy = TriInfoVector[i].f1y;
		Info1.fz = TriInfoVector[i].f1z;
		TranslateSceneToGameLogic(Info1.fx, Info1.fy, Info1.fz, TransformMatrix);
		fMinX = Info1.fx;
		fMaxX = Info1.fx;
			
		Info2.fx = TriInfoVector[i].f2x;
		Info2.fy = TriInfoVector[i].f2y;
		Info2.fz = TriInfoVector[i].f2z;
		TranslateSceneToGameLogic(Info2.fx, Info2.fy, Info2.fz, TransformMatrix);
		//fMinX = min(fMinX, Info2.fx);
		//fMaxX = max(fMaxX, Info2.fx);
		if(fMinX > Info2.fx)
		{
			fMinX = Info2.fx;
		}
		if(fMaxX < Info2.fx)
		{
			fMaxX = Info2.fx;
		}
				

		Info3.fx = TriInfoVector[i].f3x;
		Info3.fy = TriInfoVector[i].f3y;
		Info3.fz = TriInfoVector[i].f3z;
		TranslateSceneToGameLogic(Info3.fx, Info3.fy, Info3.fz, TransformMatrix);
		//fMinX = min(fMinX, Info3.fx);
		//fMaxX = max(fMaxX, Info3.fx);
		if(fMinX > Info3.fx)
		{
			fMinX = Info3.fx;
		}
		if(fMaxX < Info3.fx)
		{
			fMaxX = Info3.fx;
		}
			
		
		// 计算要扫描的三角形。
		tri.f1x = Info1.fx;
		tri.f1y = Info1.fy;
		tri.f1z = Info1.fz;
		
		tri.f2x = Info2.fx;
		tri.f2y = Info2.fy;
		tri.f2z = Info2.fz;

		tri.f3x = Info3.fx;
		tri.f3y = Info3.fy;
		tri.f3z = Info3.fz;


		//////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//  找到所有的扫描线.
		//
		//

		// 扫描第一条边。
		fV1x = Info2.fx - Info1.fx;
		fV1z = Info2.fz - Info1.fz;
		if(fabs((float)fV1x) < 0.0001)
		{

			Scanline_info.iStart    = (int)((Info1.fz < Info2.fy) ? Info1.fz : Info2.fz ) - 1;
			Scanline_info.iEnd      = (int)((Info1.fz > Info2.fy) ? Info1.fz : Info2.fz ) + 1;
			Scanline_info.iScanLine = (int)Info1.fx;
			ScanLineVector.push_back(Scanline_info);

		}

			
		// 扫描第二条边。
		fV2x = Info3.fx - Info2.fx;
		fV2z = Info3.fz - Info2.fz;
		if(fabs((float)fV2x) < 0.0001)
		{

			Scanline_info.iStart    = (int)((Info3.fz < Info2.fz) ? Info3.fz : Info2.fz) - 1;
			Scanline_info.iEnd      = (int)((Info3.fz > Info2.fz) ? Info3.fz : Info2.fz) + 1;
			Scanline_info.iScanLine = (int)Info2.fx;
			ScanLineVector.push_back(Scanline_info);

		}

		// 扫描第三条边。
		fV3x = Info1.fx - Info3.fx;
		fV3z = Info1.fz - Info3.fz;
		if(fabs((float)fV3x) < 0.0001)
		{

			Scanline_info.iStart    = (int)((Info1.fz < Info3.fz) ? Info1.fz : Info3.fz) - 1;
			Scanline_info.iEnd      = (int)((Info1.fz > Info3.fz) ? Info1.fz : Info3.fz) + 1;
			Scanline_info.iScanLine = (int)Info3.fx;
			ScanLineVector.push_back(Scanline_info);

		}

		iScanStart = (int)fMinX - 1;			// 扫描线的开始点。
		iScanEnd   = (int)fMaxX + 1;			// 扫描线的结束点。
		
		for(int k = iScanStart; k <= iScanEnd; k++)
		{// 循环每一个扫描线.
			
			///////////////////////////////////////////////////////
			//
			// 初始化这三个值的意义是第一条直线段可能不在扫描线中
			//
			//
			fFindStart = 10000.0f;		// 这个取值要大于直线方程中可能取到的最大值。
			fFindEnd   = 0;	
			fValue     = -1;

			if(fabs((float)fV1x) > 0.0001)
			{
				ft = (k * 1.0f - Info1.fx) / fV1x;
				if((ft >= 0)&&(ft <= 1.0))
				{
					fValue     = Info1.fz + ft * fV1z; 
					fFindStart = Info1.fz + ft * fV1z; 
					fFindEnd   = Info1.fz + ft * fV1z; 
				}
			}

			if(fabs((float)fV2x) > 0.0001)
			{
				ft = (k * 1.0f - Info2.fx) / fV2x;
				if((ft >= 0)&&(ft <= 1.0))
				{
					fValue = Info2.fz + ft * fV2z;
					if(fValue < fFindStart)
					{
						fFindStart = fValue;
					}

					if(fValue > fFindEnd)
					{
						fFindEnd = fValue;
					}
				}

			}

			if(fabs((float)fV3x) > 0.0001)
			{
				ft = (k * 1.0f - Info3.fx) / fV3x;
				if((ft >= 0)&&(ft <= 1.0))
				{
					fValue = Info3.fz + ft * fV3z;
					if(fValue < fFindStart)
					{
						fFindStart = fValue;
					}

					if(fValue > fFindEnd)
					{
						fFindEnd = fValue;
					}
				}

			}

			if(fValue >= 0)
			{
				Scanline_info.iStart    = (int)(fFindStart) - 1;
				Scanline_info.iEnd      = (int)(fFindEnd) + 1;
				Scanline_info.iScanLine = (int)k;
				ScanLineVector.push_back(Scanline_info);
			}
		
			// 循环每一个扫描线结束.
		}// for(int k = iScanStart; k <= iScanEnd; k++)

		// 把三角形的三个顶点， 添加到数组
		Scanline_info.iStart    = (int)(Info1.fz);
		Scanline_info.iEnd      = (int)(Info1.fz);
		Scanline_info.iScanLine = (int)(Info1.fx);
		ScanLineVector.push_back(Scanline_info);

		Scanline_info.iStart    = (int)(Info2.fz);
		Scanline_info.iEnd      = (int)(Info2.fz);
		Scanline_info.iScanLine = (int)(Info2.fx);
		ScanLineVector.push_back(Scanline_info);

		Scanline_info.iStart    = (int)(Info3.fz);
		Scanline_info.iEnd      = (int)(Info3.fz);
		Scanline_info.iScanLine = (int)(Info3.fx);
		ScanLineVector.push_back(Scanline_info);
	
		//
		//  找到所有的扫描线.
		//
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////
	
		// 根据找到的扫描线把三角形注册到地图中。
		TRI_INFO_VECTOR newTriInfoVector;
		for(int j = 0; j < (int)ScanLineVector.size(); j++)
		{
			for(int iY = ScanLineVector[j].iStart; iY <= ScanLineVector[j].iEnd; iY++)
			{
				point2DKey.iX = ScanLineVector[j].iScanLine;
				point2DKey.iY = iY;
				if(m_TriInMapInfoMap.m_triInfoInMap.count(point2DKey))
				{
					m_TriInMapInfoMap.m_triInfoInMap[point2DKey].push_back(tri);
					
				}
				else
				{
					newTriInfoVector.clear();
					newTriInfoVector.push_back(tri);
					m_TriInMapInfoMap.m_triInfoInMap[point2DKey] = newTriInfoVector;
				}
			}

		}

	}// for(int i = 0; i < iFaceCount; i++)

}



//
// 把三角形信息写到地图的三角形与位置的映射表中。
//
// 参数1： 三角形列表。
// 
// 参数2： 三角形的变换矩阵。
//
void FairyEditorFrame::WriteCollisionDataToTriMap_Rectangle(TRI_INFO_VECTOR &TriInfoVector, Ogre::Matrix4& TransformMatrix)
{

	
	int iCount = 0;			//每一个物体的行走面三角形个数
	
	POINT_3D  Info1;		// 行走面的第一个点。
	POINT_3D  Info2;        // 行走面的第二个点。
    POINT_3D  Info3;		// 行走面的第三个点.

	POINT_3D  recP1;		// 矩形第一个点。
	POINT_3D  recP2;        // 矩形第二个点。
    POINT_3D  recP3;		// 矩形第三个点.
	POINT_3D  recP4;		// 矩形第四个点.

	float       fV1x = 0;		// 用来求三角形三条边的参数方程。 
	float       fV1z = 0;		//
	float       fV2x = 0;		// fV1x = v1.x - v2.x  fV1z = v1.z - v2.z
	float       fV2z = 0;		// fV2x = v2.x - v3.x  fV2y = v2.z - v3.z
	float       fV3x = 0;		// fV3x = v3.x - v1.x  fV3y = v3.z - v1.z
	float       fV3z = 0;		//


	float       fMinX = 0;		// 用来求三角形三条边的参数方程。 
	float       fMinZ = 0;		//
	float       fMaxX = 0;		// fV1x = v1.x - v2.x  fV1z = v1.z - v2.z
	float       fMaxZ = 0;		// fV2x = v2.x - v3.x  fV2y = v2.z - v3.z
	


	float       ft = 0;			// 参数方程比率t

	int iScanStart = 0;		// 扫描线的开始点。
	int iScanEnd   = 0;		// 扫描线的结束点。

	float fFindStart = 0;		// 每次扫描找到的开始点。
	float fFindEnd   = 0;		// 每次扫描找到的结束点。

	float fValue = 0;		// 找到的值。

	POINT_2D point2DKey;

	int iFaceCount = TriInfoVector.size();

	TRI_INFO Tri;

	Info1.fx = TriInfoVector[0].f1x;
	Info1.fy = TriInfoVector[0].f1y;
	Info1.fz = TriInfoVector[0].f1z;

	TranslateSceneToGameLogic(Info1.fx, Info1.fy, Info1.fz, TransformMatrix);

	fMinX = Info1.fx;
	fMinZ = Info1.fz;
	fMaxX = Info1.fx;
	fMaxZ = Info1.fz;

	for(int i = 0; i < iFaceCount; i++)
	{

		// 计算点一
		Info1.fx = TriInfoVector[i].f1x;
		Info1.fy = TriInfoVector[i].f1y;
		Info1.fz = TriInfoVector[i].f1z;
		TranslateSceneToGameLogic(Info1.fx, Info1.fy, Info1.fz, TransformMatrix);

		if(fMinX > Info1.fx)
		{
			fMinX = Info1.fx;
		}

		if(fMinZ > Info1.fz)
		{
			fMinZ = Info1.fz;
		}

		if(fMaxX < Info1.fx)
		{
			fMaxX = Info1.fx;
		}

		if(fMaxZ < Info1.fz)
		{
			fMaxZ = Info1.fz;
		}

		// 计算点二		
		Info1.fx = TriInfoVector[i].f2x;
		Info1.fy = TriInfoVector[i].f2y;
		Info1.fz = TriInfoVector[i].f2z;
		TranslateSceneToGameLogic(Info1.fx, Info1.fy, Info1.fz, TransformMatrix);

		if(fMinX > Info1.fx)
		{
			fMinX = Info1.fx;
		}

		if(fMinZ > Info1.fz)
		{
			fMinZ = Info1.fz;
		}

		if(fMaxX < Info1.fx)
		{
			fMaxX = Info1.fx;
		}

		if(fMaxZ < Info1.fz)
		{
			fMaxZ = Info1.fz;
		}


		// 计算点三
		Info1.fx = TriInfoVector[i].f3x;
		Info1.fy = TriInfoVector[i].f3y;
		Info1.fz = TriInfoVector[i].f3z;
		TranslateSceneToGameLogic(Info1.fx, Info1.fy, Info1.fz, TransformMatrix);

		if(fMinX > Info1.fx)
		{
			fMinX = Info1.fx;
		}

		if(fMinZ > Info1.fz)
		{
			fMinZ = Info1.fz;
		}

		if(fMaxX < Info1.fx)
		{
			fMaxX = Info1.fx;
		}

		if(fMaxZ < Info1.fz)
		{
			fMaxZ = Info1.fz;
		}

	}// for(int i = 0; i < iFaceCount; i++)


	int iMinX = 0;
	int iMinZ = 0;
	int iMaxX = 0;
	int iMaxZ = 0;

	iMinX = fMinX - 1;
	iMinZ =	fMinZ - 1;
	iMaxX =	fMaxX + 1;
	iMaxZ =	fMaxZ + 1;

	TRI_INFO_VECTOR newTriInfoVector;
	for(int x = iMinX; x <= iMaxX; x++)
		for(int z = iMinZ; z <= iMaxZ; z++)
		{

			recP1.fx = x;
			recP1.fz = z;
			recP2.fx = x + 1;
			recP2.fz = z;
            recP3.fx = x + 1;
			recP3.fz = z + 1;
			recP4.fx = x;
			recP4.fz = z + 1;

			for(int iFace = 0; iFace < iFaceCount; iFace++)
			{

				// 计算点一
				Info1.fx = TriInfoVector[iFace].f1x;
				Info1.fy = TriInfoVector[iFace].f1y;
				Info1.fz = TriInfoVector[iFace].f1z;
				TranslateSceneToGameLogic(Info1.fx, Info1.fy, Info1.fz, TransformMatrix);

				// 计算点二		
				Info2.fx = TriInfoVector[iFace].f2x;
				Info2.fy = TriInfoVector[iFace].f2y;
				Info2.fz = TriInfoVector[iFace].f2z;
				TranslateSceneToGameLogic(Info2.fx, Info2.fy, Info2.fz, TransformMatrix);

				// 计算点三
				Info3.fx = TriInfoVector[iFace].f3x;
				Info3.fy = TriInfoVector[iFace].f3y;
				Info3.fz = TriInfoVector[iFace].f3z;
				TranslateSceneToGameLogic(Info3.fx, Info3.fy, Info3.fz, TransformMatrix);

				if(Is_Rectangle_Triangle_Intersect(Info1, Info2, Info3, recP1, recP2, recP3, recP4))
				{
					// 把三角形注册到地图中去
					Tri.f1x = Info1.fx;
					Tri.f1y = Info1.fy;
					Tri.f1z = Info1.fz;
					Tri.f2x = Info2.fx;
					Tri.f2y = Info2.fy;
					Tri.f2z = Info2.fz;
					Tri.f3x = Info3.fx;
					Tri.f3y = Info3.fy; 
					Tri.f3z = Info3.fz;

					point2DKey.iX = x;
					point2DKey.iY = z;
					if(m_TriInMapInfoMap.m_triInfoInMap.count(point2DKey))
					{
						m_TriInMapInfoMap.m_triInfoInMap[point2DKey].push_back(Tri);
						
					}
					else
					{
						newTriInfoVector.clear();
						newTriInfoVector.push_back(Tri);
						m_TriInMapInfoMap.m_triInfoInMap[point2DKey] = newTriInfoVector;
					}

				}// if(Is_Rectangle_Triangle_Intersect(Info1, Info2, Info3, recP1, recP2, recP3, recP4))

			}// for(int iFace = 0; iFace < iFaceCount; iFace++)

		}	//for(int x = iMinX; x < iMaxX; x++)
		//for(int z = fMinZ; z < fMaxZ; z++)


}



// 
// 矩形与三角形是否相交。
//
// 参数1： 三角形点1。
// 
// 参数2： 三角形点2。
//
// 参数3： 三角形点3。
//
// 参数1： 矩形点1。
// 
// 参数2： 矩形点2。
//
// 参数3： 矩形点3。
//
// 参数4： 矩形点4。
//
// 返回值：1 相交， 0 不相交。
// 
bool FairyEditorFrame::Is_Rectangle_Triangle_Intersect(
										  POINT_3D triP1, 
										  POINT_3D triP2,
										  POINT_3D triP3,
										  POINT_3D recP1,
										  POINT_3D recP2,
										  POINT_3D recP3,
										  POINT_3D recP4
										  )
{

	// 先判断三角形是否在矩形内部。

	// 判断点1
	if((triP1.fx >= recP1.fx - OFFEET )&&(triP1.fx <= recP2.fx + OFFEET)&&(triP1.fz >= recP1.fz - OFFEET)&&(triP1.fz <= recP3.fz + OFFEET))
	{
		return true;
	}

	// 判断点2
    if((triP2.fx >= recP1.fx - OFFEET )&&(triP2.fx <= recP2.fx + OFFEET)&&(triP2.fz >= recP1.fz - OFFEET)&&(triP2.fz <= recP3.fz + OFFEET))
	{
		return true;
	}

	// 判断点3
	if((triP3.fx >= recP1.fx - OFFEET )&&(triP3.fx <= recP2.fx + OFFEET)&&(triP3.fz >= recP1.fz - OFFEET)&&(triP3.fz <= recP3.fz + OFFEET))
	{
		return true;
	}

	// 判断四方形的点是否在三角形内

	// 判断点1
	if(IsPointInTriangle(recP1, triP1, triP2, triP3))
	{
		return true;
	}

	// 判断点2
	if(IsPointInTriangle(recP2, triP1, triP2, triP3))
	{
		return true;
	}

	// 判断点3
	if(IsPointInTriangle(recP3, triP1, triP2, triP3))
	{
		return true;
	}

	// 判断点4
	if(IsPointInTriangle(recP4, triP1, triP2, triP3))
	{
		return true;
	}

	// 判断直线是否相交。
	if(CheckCross(recP1, recP2, triP1, triP2))
	{
		return true;
	}
	// 判断直线是否相交。
	if(CheckCross(recP1, recP2, triP2, triP3))
	{
		return true;
	}
	// 判断直线是否相交。
	if(CheckCross(recP1, recP2, triP3, triP1))
	{
		return true;
	}



	// 判断直线是否相交。
	if(CheckCross(recP2, recP3, triP1, triP2))
	{
		return true;
	}
	// 判断直线是否相交。
	if(CheckCross(recP2, recP3, triP2, triP3))
	{
		return true;
	}
	// 判断直线是否相交。
	if(CheckCross(recP2, recP3, triP3, triP1))
	{
		return true;
	}


	// 判断直线是否相交。
	if(CheckCross(recP3, recP4, triP1, triP2))
	{
		return true;
	}
	// 判断直线是否相交。
	if(CheckCross(recP3, recP4, triP2, triP3))
	{
		return true;
	}
	// 判断直线是否相交。
	if(CheckCross(recP3, recP4, triP3, triP1))
	{
		return true;
	}


	// 判断直线是否相交。
	if(CheckCross(recP4, recP1, triP1, triP2))
	{
		return true;
	}
	// 判断直线是否相交。
	if(CheckCross(recP4, recP1, triP2, triP3))
	{
		return true;
	}
	// 判断直线是否相交。
	if(CheckCross(recP4, recP1, triP3, triP1))
	{
		return true;
	}


	return false;
}


// 
// 判断点是否在三角形内部
//
// 参数1： 点的坐标
//
// 参数1： 三角形点1。
// 
// 参数2： 三角形点2。
//
// 参数3： 三角形点3。
//
// 返回值：1 在内部， 0 不在内部。
// 
bool FairyEditorFrame::IsPointInTriangle(POINT_3D point, POINT_3D triP1, POINT_3D triP2, POINT_3D triP3)
{
	float a1 = 0;
	float a2 = 0;
	float a3 = 0;

	float b1 = 0;
	float b2 = 0;
	float b3 = 0;

	float fu = 0;
	float fv = 0;

	float fBase = 0;
	
	a1 = point.fx - triP1.fx;
	b1 = point.fz - triP1.fz;

	a2 = triP2.fx - triP1.fx;
	b2 = triP2.fz - triP1.fz;

	a3 = triP3.fx - triP1.fx;
	b3 = triP3.fz - triP1.fz;

	fBase = a2*b3 - a3*b2;
	if(abs(fBase) > 0.0001)
	{
		fu = (a2*b1 - a1*b2) / fBase;
		fv = (a1*b3 - a3*b1) / fBase;

		if((fu > -0.8)&&(fv > -0.8)&&((fu + fv) < 1.8))
		{
			return true;
		}
	}
	else 
	{
		return true;
	}

	return false;
}


// 判断两条直线是否相交.
//
// 参数1： 第1条直线的开始点。
// 参数2： 第1条直线的结束点。
// 参数3： 第2条直线的开始点。
// 参数4： 第2条直线的结束点。
// 
bool FairyEditorFrame::CheckCross(POINT_3D fvMS, POINT_3D fvME, POINT_3D fvNS, POINT_3D fvNE)
{

	POINT_2D_FLOAT	fvMS2;
	POINT_2D_FLOAT	fvME2;
	POINT_2D_FLOAT	fvNS2;
	POINT_2D_FLOAT	fvNE2;
	
	fvMS2.fx = fvMS.fx;
	fvMS2.fy = fvMS.fz;

	fvME2.fx = fvME.fx;
	fvME2.fy = fvME.fz;

	fvNS2.fx = fvNS.fx;
	fvNS2.fy = fvNS.fz;

	fvNE2.fx = fvNE.fx;
	fvNE2.fy = fvNE.fz;

	if ( std::max(fvMS2.fx,fvME2.fx) <= std::min(fvNS2.fx,fvNE2.fx) )
	{
		return false;
	}
	if ( std::min(fvMS2.fx,fvME2.fx) >= std::max(fvNS2.fx,fvNE2.fx) )
	{
		return false;
	}
	if ( std::max(fvMS2.fy,fvME2.fy) <= std::min(fvNS2.fy,fvNE2.fy) )
	{
		return false;
	}
	if ( std::min(fvMS2.fy,fvME2.fy) >= std::max(fvNS2.fy,fvNE2.fy) )
	{
		return false;
	}

	float temp1 = fvMulti(fvSub(fvMS2,fvNS2),fvSub(fvNE2,fvNS2)) * fvMulti( fvSub(fvNE2,fvNS2),fvSub(fvME2,fvNS2));
	float temp2 = fvMulti(fvSub(fvNS2,fvMS2),fvSub(fvME2,fvMS2)) * fvMulti( fvSub(fvME2,fvMS2),fvSub(fvNE2,fvMS2));

	if((temp1>=0)&&(temp2>=0))
	{
		return true;
	}

	return false;
		
}

// 求两个向量的行列式的值
float FairyEditorFrame::fvMulti(POINT_2D_FLOAT fv1, POINT_2D_FLOAT fv2)
{
	return (fv1.fx*fv2.fy - fv1.fy*fv2.fx);
}


POINT_2D_FLOAT FairyEditorFrame::fvSub(POINT_2D_FLOAT fv1, POINT_2D_FLOAT fv2)
{
	POINT_2D_FLOAT fvResult;
	fvResult.fx = fv1.fx-fv2.fx;
	fvResult.fy = fv1.fy-fv2.fy;
	return fvResult;
}


//
// 建筑物可行走面数据
//
///////////////////////////////////////////////////////////////////////////////////

