/********************************************************************
	创建日期:	2005年11月2日
	创建时间:	16:55
	文件名称:	ItemTypes.h
	文件路径:	d:\Prj\Server\Server\Item\ItemTypes.h
	
	文件功能： 物品常量\宏定义等	类型数据
	修改纪录：
	
*********************************************************************/

#ifndef _ITEM_TYPES_H_
#define _ITEM_TYPES_H_

#include "Type.h"
//物品系统常量定义


//ItemBox 池最大值
#define		MAX_ITEM_BOX_POOL					5000

//最大绿色装备附加属性个数
#define		MAX_GREEN_ITEM_EXT_ATTR				4



//最大掉落包容量
#define		MAX_DROPBOX_CARRAGE					100

//最大怪物掉落
#define		MAX_MONSTER_DROPBOX					20

//ItemBox 掉落位置随机范围
#define		ITEMBOX_DROP_RANGE					10

//最大ItemQuality 表 项目个数(内部使用)
#define		MAX_ITEM_TABLE_QUALITYS				10

//最大ItemValue 表 项目个数
#define		MAX_ITEM_TABLE_VALUES				10



//怪物掉落结构
struct MONSTER_DROPBOXS 
{
	MONSTER_DROPBOXS()
	{
		memset(m_DropBox,0,sizeof(INT)*MAX_MONSTER_DROPBOX);
	}
	INT		m_DropBox[MAX_MONSTER_DROPBOX];
};




#endif