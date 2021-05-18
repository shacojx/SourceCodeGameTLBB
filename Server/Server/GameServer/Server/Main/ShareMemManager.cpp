#include "stdafx.h"
#include "ShareMemManager.h"
#include "SMUManager.h"

using namespace PLAYER_SHOP;

SMUPool<HumanSMU>			g_HumanSMUPool;
SMUManager<HumanSMU>		g_HumanSMUManager;
SMUPool<PlayerShopSM>			g_PlayerShopSMUPool;
SMUManager<PlayerShopSM>		g_PlayerShopSMUManager;

SMUPool<ItemSerialKeySMU>			g_ItemSerialSMUPool;
SMUManager<ItemSerialKeySMU>		g_ItemSerialSMUManager;