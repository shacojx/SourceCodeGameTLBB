/********************************************************************************
 *	文件名：	AbilityGeneric.h
 *	全路径：	d:\Prj\Server\Server\Ability\AbilityGeneric.h
 *	创建时间：	2006 年 3 月 23 日	14:50
 *
 *	功能说明：	1、通用生活技能，适用于一些嫁接在生活系统上实现的一些没有复杂逻辑的功能
 *				2、通用合成技能和通用采集技能，
 *				适用于可以只通过生活技能表数据定义来确定逻辑规则的生活技能使用
 *	修改记录：
*********************************************************************************/

#ifndef __ABILITYGENERIC_H__
#define __ABILITYGENERIC_H__

#include "Ability.h"
#include "AbilityCompound.h"
#include "AbilityGather.h"

class AbilityGeneric : public Ability
{
public:
	AbilityGeneric(const Ability& a) : Ability(a) {}

private:
};

class AbilityGenericCompound : public AbilityCompound
{

public:
	AbilityGenericCompound(const Ability& a) : AbilityCompound(a) {}

private:
};

class AbilityGenericGather:public AbilityGather
{

public:
	AbilityGenericGather(Ability& a):AbilityGather(a) {}

};

#endif // __ABILITYGENERIC_H__
