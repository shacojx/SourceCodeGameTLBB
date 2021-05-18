--注意：

--物品技能的逻辑只能使用基础技能和脚本来实现


--脚本:

--以下是脚本样例:


--4801.lua
------------------------------------------------------------------------------------------
--马鞍的默认脚本

--脚本号
g_scriptId = 334801 --临时写这个,真正用的时候一定要改.

--需要的等级
--需要的门派
g_MenPai = -1 --各门派都可以用的。

--效果的ID
g_Impact1 = 4801 --临时写这个
g_Impact2 = -1 --不用

--**********************************
--事件交互入口
--**********************************
function OnDefaultEvent( sceneId, selfId, bagIndex )
-- 骑乘不需要这个接口，但要保留空函数
end

--**********************************
--这个物品的使用过程是否类似于技能：
--系统会在执行开始时检测这个函数的返回值，如果返回失败则忽略后面的类似技能的执行。
--返回1：技能类似的物品，可以继续类似技能的执行；返回0：忽略后面的操作。
--**********************************
function IsSkillLikeScript( sceneId, selfId)
	return 1; --这个脚本需要动作支持
end

--**********************************
--直接取消效果：
--系统会直接调用这个接口，并根据这个函数的返回值确定以后的流程是否执行。
--返回1：已经取消对应效果，不再执行后续操作；返回0：没有检测到相关效果，继续执行。
--**********************************
function CancelImpacts( sceneId, selfId )
	nRet = 0;
	nRet = LuaFnCancelSpecificImpact(sceneId, selfId, g_Impact1)
	if(0<nRet) then
		return 1;
	end
	return 0;
end

--**********************************
--条件检测入口：
--系统会在技能检测的时间点调用这个接口，并根据这个函数的返回值确定以后的流程是否执行。
--返回1：条件检测通过，可以继续执行；返回0：条件检测失败，中断后续执行。
--**********************************
function OnConditionCheck( sceneId, selfId )
	--校验使用的物品
	if(1~=LuaFnVerifyUsedItem(sceneId, selfId)) then
		return 0
	end
	if(-1~=g_MenPai) then
		if(LuaFnGetMenPai(sceneId, selfId)==g_MenPai) then --门派专用座骑检查
			return 1;
		else
			Msg2Player( sceneId,selfId,"您不能骑这种座骑。",MSG2PLAYER_PARA) --通知玩家
		end
	else
		return 1;
	end
	return 0; --骑乘不需要任何条件
end

--**********************************
--消耗检测及处理入口：
--系统会在技能消耗的时间点调用这个接口，并根据这个函数的返回值确定以后的流程是否执行。
--返回1：消耗处理通过，可以继续执行；返回0：消耗检测失败，中断后续执行。
--注意：这不光负责消耗的检测也负责消耗的执行。
--**********************************
function OnDeplete( sceneId, selfId )
	return 1; --骑乘不消耗
end

--**********************************
--只会执行一次入口：
--聚气和瞬发技能会在消耗完成后调用这个接口（聚气结束并且各种条件都满足的时候），而引导
--技能也会在消耗完成后调用这个接口（技能的一开始，消耗成功执行之后）。
--返回1：处理成功；返回0：处理失败。
--注：这里是技能生效一次的入口
--**********************************
function OnActivateOnce( sceneId, selfId )
	if(-1~=g_Impact1) then
		LuaFnSendSpecificImpactToUnit(sceneId, selfId, selfId, selfId, g_Impact1, 0);
	end
	Msg2Player( sceneId,selfId,"您已经骑上座骑了。",MSG2PLAYER_PARA) --通知玩家
	return 1;
end

--**********************************
--引导心跳处理入口：
--引导技能会在每次心跳结束时调用这个接口。
--返回：1继续下次心跳；0：中断引导。
--注：这里是技能生效一次的入口
--**********************************
function OnActivateEachTick( sceneId, selfId)
	return 1; --骑乘不是引导性脚本, 只保留空函数.
end