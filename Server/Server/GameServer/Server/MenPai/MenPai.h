///////////////////////////////////////////////////////////////////////////////
// 文件名：MenPai.h
// 功能说明：所有门派的接口声明
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef MENPAI_H
#define MENPAI_H

#include "Type.h"
#include "Obj_Human.h"

namespace MenPai_Module
{
	class MenPai_T
{
	public:
		typedef enum
		{
			ID_INVALID = -1, //无效门派ID, 初始化用和判断非法用
			ID_SHAOLIN 	= MATTRIBUTE_SHAOLIN,	//少林
			ID_MINGJIAO = MATTRIBUTE_MINGJIAO,	//明教
			ID_GAIBANG = MATTRIBUTE_GAIBANG,	//丐帮
			ID_WUDANG = MATTRIBUTE_WUDANG,	//武当
			ID_EMEI = MATTRIBUTE_EMEI,	//峨嵋
			ID_XINGXIU = MATTRIBUTE_XINGXIU,	//星宿
			ID_DALI = MATTRIBUTE_DALI,	//大理
			ID_TIANSHAN = MATTRIBUTE_TIANSHAN,	//天山
			ID_XIAOYAO = MATTRIBUTE_XIAOYAO,	//逍遥
			ID_WUMENPAI = MATTRIBUTE_WUMENPAI, //无门派，如新手。。等
			NUMBER_OF_MENPAI,	//门派数目

		} MenPaiID_T;
		enum
		{
			ID= INVALID_ID,
		};
		
			MenPai_T(){};
			~MenPai_T(){};
			virtual MenPaiID_T GetID(VOID) const{return ID_INVALID;};
			//MaxHP
			SHORT GetInitMaxHP(VOID) const;
			SHORT GetMaxHPConRefix(VOID) const;
			SHORT GetMaxHPLevelRefix(VOID) const;
			//HP Regenerate
			SHORT GetInitHPRegenerate(VOID) const;
			SHORT GetHPRegenerateConRefix(VOID) const;
			SHORT GetHPRegenerateLevelRefix(VOID) const;
			//MaxMP
			SHORT GetInitMaxMP(VOID) const;
			SHORT GetMaxMPIntRefix(VOID) const;
			SHORT GetMaxMPLevelRefix(VOID) const;
			//MP Regenerate
			SHORT GetInitMPRegenerate(VOID) const;
			SHORT GetMPRegenerateIntRefix(VOID) const;
			SHORT GetMPRegenerateLevelRefix(VOID) const;
			//Attr Level 1 Sstr
			SHORT GetInitStr(VOID) const;
			SHORT GetStrLevelupRefix(SHORT const nLevel) const;
			//Attr Level 1 Spr
			SHORT	GetInitSpr(VOID) const;
			SHORT GetSprLevelupRefix(SHORT const nLevel) const;
			//Attr Level 1 Con
			SHORT GetInitCon(VOID) const;
			SHORT GetConLevelupRefix(SHORT const nLevel) const;
			//Attr Level 1 Int
			SHORT GetInitInt(VOID) const;
			SHORT GetIntLevelupRefix(SHORT const nLevel) const;
			//Attr Level 1 Dex
			SHORT GetInitDex(VOID) const;
			SHORT GetDexLevelupRefix(SHORT const nLevel) const;
			//Attr Level 2 Attack Physics
			SHORT GetInitAttackPhysics(VOID) const;
			SHORT GetAttackPhysicsStrRefix(VOID) const;
			SHORT GetAttackPhysicsLevelRefix(VOID) const;
			//Attr Level 2 Defence Physics
			SHORT GetInitDefencePhysics(VOID) const;
			SHORT GetDefencePhysicsConRefix(VOID) const;
			SHORT GetDefencePhysicsLevelRefix(VOID) const;
			//Attr Level 2 Attack Magic
			SHORT GetInitAttackMagic(VOID) const;
			SHORT GetAttackMagicSprRefix(VOID) const;
			SHORT GetAttackMagicLevelRefix(VOID) const;
			//Attr Level 2 Defence Magic
			SHORT GetInitDefenceMagic(VOID) const;
			SHORT GetDefenceMagicIntRefix(VOID) const;
			SHORT GetDefenceMagicLevelRefix(VOID) const;
			//Attr Level 2 Hit
			SHORT GetInitHit(VOID) const;
			SHORT GetHitDexRefix(VOID) const;
			SHORT GetHitLevelRefix(VOID) const;
			//Attr Level 2 Critical
			SHORT GetInitCritical(VOID) const;
			SHORT GetCriticalDexRefix(VOID) const;
			SHORT GetCriticalLevelRefix(VOID) const;
			//Attr Level 2 Miss
			SHORT GetInitMiss(VOID) const;
			SHORT GetMissDexRefix(VOID) const;
			SHORT GetMissLevelRefix(VOID) const;
			//Attr Attack Fluctuation
			SHORT GetAttackFluctuation(VOID) const;
			// Need more menpai special Refix functions
			//event handler
			virtual VOID OnDamage(Obj_Human& rMe, INT nDamage) const;
			virtual VOID OnDamageTarget(Obj_Human& rMe,INT nDamage) const;
			virtual VOID OnMyTrapActivated(Obj_Human& rMe, Obj_Special& rTrap) const {};
			virtual VOID OnHealTarget(Obj_Human& rMe, INT nHealedHp) const {};
			virtual VOID OnDepleteStrikePoints(Obj_Human& rMe, INT nStrikePoint) const {};
	protected:
	
	private:
			//Rage
			virtual INT TransferValueToRage(INT nDamage) const {return 0;};
			//Strike 
	};
};
#endif //MENPAI_H
