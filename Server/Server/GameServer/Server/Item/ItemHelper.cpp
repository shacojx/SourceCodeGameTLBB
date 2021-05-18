#include "stdafx.h"
#include "ItemHelper.h"
#include "Rand.h"
//最大物品随机分辨率
#define			MAX_ITEM_RAND_QUOATE	100000000


double	ItemRander::DoubleRand()
{
	
	return g_RandGen.RandDouble();
}

INT		ItemRander::IntRand()
{
	return (INT)(MAX_ITEM_RAND_QUOATE*DoubleRand());
}