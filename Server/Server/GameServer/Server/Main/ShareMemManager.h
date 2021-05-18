#ifndef _SHAREMEM_POOL_H_
#define _SHAREMEM_POOL_H_
#include "PlayerShopDefine.h"

struct  HumanSMU;

using namespace PLAYER_SHOP;

template<typename T> 
class SMUPool;

template<typename T>
class SMUManager;


extern SMUPool<HumanSMU>				g_HumanSMUPool;

extern SMUManager<HumanSMU>				g_HumanSMUManager;				

extern SMUPool<PlayerShopSM>			g_PlayerShopSMUPool;

extern SMUManager<PlayerShopSM>			g_PlayerShopSMUManager;		

extern SMUPool<ItemSerialKeySMU>				g_ItemSerialSMUPool;
extern SMUManager<ItemSerialKeySMU>		g_ItemSerialSMUManager;

#endif