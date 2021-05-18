#ifndef __LOGICDATASTRUCTDEF_H__
#define __LOGICDATASTRUCTDEF_H__
// #include "LogicDataStructDef.h"


#include <map>
#include <list>
#include <string>
#include <OgreVector2.h>

namespace WX 
{
	typedef struct _POINT_2D
	{
		int iX;
		int iY;

	}POINT_2D, *PPOINT_2D;


	typedef struct _AREARECT
	{
		Ogre::Vector2 m_LeftTop;
		Ogre::Vector2 m_RightBottom;

		void operator= (_AREARECT rect)
		{
			m_LeftTop     = rect.m_LeftTop;
			m_RightBottom = rect.m_RightBottom;
		}

	}AREARECT, *PAREARECT;

	class Point_2D_less 
	{
	public:
		bool operator() ( const POINT_2D& point1, const POINT_2D& point2 ) const
		{
			if(point1.iX < point2.iX)
			{
				return true;
			}
			else if(point1.iX == point2.iX)
			{
				if(point1.iY < point2.iY)
				{
					return true;	
				}
			}

			return false;
		}
	};

		
	// 实例列表
	typedef std::list<unsigned long> INSTANCE_ID_LIST;
		
	// 注册在地图中的信息
	typedef std::map<POINT_2D, INSTANCE_ID_LIST, Point_2D_less> POS_ID_MAP;

	// id和名字映射表
	typedef std::map<unsigned long, std::string>	ID_NAME_MAP;

	// id与id对应的map
	typedef std::map<int, int> ID_ID_MAP;
}

#endif //__LOGICDATASTRUCTDEF_H__