脚本系统C导出函数文档	LuaFnTbl_Ability.h
	 */
	该函数注册为 GetHumanGUID
	INT	LuaFnGetHumanGUID(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetGemEmbededCount
	*     参数说明 : 
	*     功能说明 :
	*/
	该函数注册为 GetGemEmbededCount
	INT LuaFnGetGemEmbededCount(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2006-1-17
	*     函数名称 : LuaFnGetBagItemParam
	*     参数说明 : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				BagPos
	*				UINT				StartPos
	*				INT					Type
	*     功能说明 :
	*		获取背包中物品的参数
	*/
	该函数注册为 GetBagItemParam
	INT LuaFnGetBagItemParam(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2006-1-17
	*     函数名称 : LuaFnSetBagItemParam
	*     参数说明 : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				BagPos
	*				UINT				StartPos
	*				INT					Type
	*				INT					Value
	*     功能说明 :
	*		设置背包中物品的参数
	*/
	该函数注册为 SetBagItemParam
	INT LuaFnSetBagItemParam(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2006-1-17
	*     函数名称 : LuaFnGetBagItemDur
	*     参数说明 : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				BagPos
	*     功能说明 :
	*		获取背包中物品的耐久度
	*/
	该函数注册为 GetBagItemDur
	INT LuaFnGetBagItemDur(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2006-1-17
	*     函数名称 : LuaFnSetBagItemDur
	*     参数说明 : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				BagPos
	*				INT					Dur
	*     功能说明 :
	*		设置背包中物品的耐久度
	*/
	该函数注册为 SetBagItemDur
	INT LuaFnSetBagItemDur(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2006-3-27
	*     函数名称 : LuaFnGetBagItemIdent
	*     参数说明 : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				BagPos
	*     功能说明 :
	*		获取背包中物品的鉴定属性
	*/
	该函数注册为 GetBagItemIdent
	INT LuaFnGetBagItemIdent(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2006-3-27
	*     函数名称 : LuaFnSetBagItemDur
	*     参数说明 : 
	*				SceneID_t			SceneID
	*				ObjID_t				HumanID
	*				UINT				BagPos
	*     功能说明 :
	*		设置背包中物品的鉴定属性
	*/
	该函数注册为 SetBagItemIdent
	INT LuaFnSetBagItemIdent(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetItemQuality
	*     参数说明 : 
	*     功能说明 :
	*/
	该函数注册为 GetItemQuality
	该函数注册为 LuaFnGetItemQuality
	INT LuaFnGetItemQuality(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-4-6	15:04
	 *	函数名称：	LuaFnGetItemType
	 *	参数说明：
	 *				uSerialNumber
	 *					物品序列号
	 *
	 *	功能说明：	根据物品序列号得到物品的 Type
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnGetItemType
	INT LuaFnGetItemType(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-4-6	14:00
	 *	函数名称：	LuaFnGetBagEquipType
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					角色编号
	 *				nBagPos
	 *					背包格子
	 *
	 *	功能说明：	通过背包格子查到该物品是哪种装备（如果不是装备，则返回 -1）
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnGetBagEquipType
	INT LuaFnGetBagEquipType(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetItemIndex
	*     参数说明 : 
	*     功能说明 :
	*/
	该函数注册为 GetItemIndex
	INT LuaFnGetItemIndex(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetItemBoxRequireAbilityID
	*     参数说明 : 
	*     功能说明 :
	*				获得Obj_ItemBox需要的生活技能编号
	*		SceneID
	*				场景编号
	*		ItemBoxID
	*				Obj_ItemBox编号
	*
	*		返回
	*			AbilityID
	*				需要生活技能的编号
	*/
	该函数注册为 GetItemBoxRequireAbilityID
	INT	LuaFnGetItemBoxRequireAbilityID(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddItemToBox
	*     参数说明 : 
	*     功能说明 :
	*				添加物品到Obj_ItemBox
	*
	*
	*	  SceneID
	*			场景ID
	*	  ItemBoxID
	*			Obj_ItemBox编号
	*	  ItemCount
	*			放入Obj_ItemBox的物品个数,总共最大10个
	*	  Item0
	*			0号物品
	*	  ...
	*
	*     Item9 
	*			9号物品
	*     
	*/
	该函数注册为 AddItemToBox
	INT LuaFnAddItemToBox(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnItemBoxEnterScene
	*     参数说明 : 
	*     功能说明 :
	*				Obj_ItemBox进入场景
	*  
	*	  fx
	*				Obj_ItemBox进入场景X坐标
	*	  fz	
	*				Obj_ItemBox进入场景Z坐标
	*	  iBoxType  
	*				Obj_ItemBox类型
	*	  SceneID	
	*				场景编号
	*	  
	*	  ItemCount  
	*				放入的物品个数 最大10个
	*	  Item0    
	*				放入的0号物品
	*	  。。。
	*	  Item9		放入的9号物品
	*
	*	  ItemBoxID 
	*               返回的ItemBoxID
	*/
	该函数注册为 ItemBoxEnterScene
	INT LuaFnItemBoxEnterScene(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnQueryHumanAbility
	*     参数说明 : 
	*     功能说明 :
	*				查询human Ability是否学习
	*	
	*		sceneId
	*					场景编号
	*		selfId
	*					角色编号
	*		abilityID
	*					生活技能ID
	*
	*/
	该函数注册为 QueryHumanAbility
	INT LuaFnQueryHumanAbility(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnQueryHumanAbilityLevel
	*     参数说明 : 
	*     功能说明 :
	*				查询Obj_Human生活技能等级 SceneID,HumanID,AbilityID
	*/
	该函数注册为 QueryHumanAbilityLevel
	INT LuaFnQueryHumanAbilityLevel(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetHumanAbilityLevel
	*     参数说明 : 
	*     功能说明 :
	*				设置人物生活技能级别,SceneID,HumanID,AbilityID,AbilityLevel
	*/
	该函数注册为 SetHumanAbilityLevel
	INT LuaFnSetHumanAbilityLevel(Lua_State* L)


	/** ----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-10-28
	*     函数名称 : LuaFnSetAbilityOperaTime
	*     参数说明 : 
	*     功能说明 :
	*				设置生活技能操作时间
	*
	*	  SceneID
	*				场景编号
	*	  HumanID
	*				角色编号
	*	  OperaTime
	*				操作时间
	*
	*
	*
	*/
	该函数注册为 SetAbilityOperaTime
	INT LuaFnSetAbilityOperaTime(Lua_State* L)


	/** ----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-10-28
	*     函数名称 : LuaFnGetAbilityOperaRobotTime
	*     参数说明 : 
	*     功能说明 :
	*				获得生活技能挂机器时间
	*
	*	  SceneID
	*				场景编号
	*	  HumanID
	*				角色编号
	*    返回
	*	  RobotTime
	*				操作时间
	*
	*
	*
	*/
	该函数注册为 GetAbilityOperaRobotTime
	INT LuaFnGetAbilityOperaRobotTime(Lua_State* L)


	/** ----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-10-28
	*     函数名称 : LuaFnSetAbilityOperaRobotTime
	*     参数说明 : 
	*     功能说明 :
	*				设置生活技能挂机器时间
	*
	*	  SceneID
	*				场景编号
	*	  HumanID
	*				角色编号
	*	  
	*	  RobotTime
	*				操作时间
	*
	*
	*
	*/
	该函数注册为 SetAbilityOperaRobotTime
	INT LuaFnSetAbilityOperaRobotTime(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-29	15:54
	 *	函数名称：	LuaFnSetAbilityExp
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				abilityId
	 *					生活技能编号
	 *				abilityExp
	 *					生活技能经验
	 *
	 *	功能说明：	设置生活技能经验，发送的时候 / 100 是为了整型运算方便
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 SetAbilityExp
	INT LuaFnSetAbilityExp(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetAbilityExpPlus
	*     参数说明 : 		
	*		SceneID
	*				场景编号
	*		HumanID
	*				角色编号
	*		AbilityID
	*				生活技能编号
	*     功能说明 :
	*				获得生活技能经验
	*     计算公式
	*	            1如果PlayerAbilityLevel <= C  玩家获得技能经验 =B
	*				2如果 B-(PlayerAbilityLevel-C)*D <0 则玩家获得技能经验=0
	*				3否则 玩家获得技能经验= B-(PlayerAbilityLevel-C)*D 之后取整
	*/
	该函数注册为 GetAbilityExpPlus
	INT	LuaFnGetAbilityExpPlus(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetAbilityExp
	*     参数说明 : 		
	*		SceneID
	*				场景编号
	*		HumanID
	*				角色编号
	*		AbilityID
	*				生活技能编号
	*     功能说明 :
	*				获得生活技能经验
	*/
	该函数注册为 GetAbilityExp
	INT	LuaFnGetAbilityExp(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-10-17
	*     函数名称 : LuaFnSetPrescription
	*     参数说明 : 
	*     功能说明 :
	*				学会某项合成配方（只能操作学会，不能操作放弃具体某项配方，具体
	*				的配方放弃功能由技能放弃功能提供）
	*/
	该函数注册为 SetPrescription
	INT LuaFnSetPrescription(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-10-17
	*     函数名称 : LuaFnIsPrescrLearned
	*     参数说明 : 
	*     功能说明 :
	*				判断是否已经学会某项配方
	*/
	该函数注册为 IsPrescrLearned
	该函数注册为 LuaFnIsPrescrLearned
	INT LuaFnIsPrescrLearned(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetAbilityObjIndex
	*     参数说明 : 
	*     功能说明 :
	*
	*/
	该函数注册为 GetAbilityObjIndex
	INT LuaFnGetAbilityObjIndex(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetItemBoxRecycleTimes
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*				调用该函数场景ID,short
	*				ItemBoxID
	*					Obj_ItemBox 编号
	*				RecycleTimes
	*					回收次数
	*				
	*				获得itemBox回收次数
	*/
	该函数注册为 SetItemBoxRecycleTimes
	INT		LuaFnSetItemBoxRecycleTimes(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetItemBoxRecycleTimes
	*     参数说明 : 
	*     功能说明 :
	*			   SceneID	
	*			   调用该函数场景ID
	*				ItemBoxID
	*					Obj_ItemBox 编号
	*			   返回
	*				RecycleTimes
	*					回收次数
	*
	*				设置Obj_ItemBox回收次数
	*/
	该函数注册为 GetItemBoxRecycleTimes
	INT		LuaFnGetItemBoxRecycleTimes(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetItemBoxOwner
	*     参数说明 : 
	*     功能说明 :
	*			   SceneID	
	*			       调用该函数场景ID
	*				ItemBoxID
	*					Obj_ItemBox 编号
	*				OwnerID
	*					所有者ID			
	*				设置Obj_ItemBox所有者
	*/
	该函数注册为 SetItemBoxOwner
	INT		LuaFnSetItemBoxOwner(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetItemBoxOpenFlag
	*     参数说明 : 
	*     功能说明 :
	*			   SceneID	
	*			       调用该函数场景ID
	*				ItemBoxID
	*					Obj_ItemBox 编号
	*/
	该函数注册为 GetItemBoxOpenFlag
	INT		LuaFnGetItemBoxOpenFlag(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetItemBoxOwner
	*     参数说明 : 
	*     功能说明 :
	*			   SceneID	
	*			       调用该函数场景ID
	*				ItemBoxID
	*					Obj_ItemBox 编号
	*			返回
	*				OwnerID
	*					所有者ID	
	*
	*				获得Obj_ItemBox所有者
	*/
	该函数注册为 GetItemBoxOwner
	INT		LuaFnGetItemBoxOwner(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2006-1-20
	*     函数名称 : LuaFnAddGPOwner
	*     参数说明 : 
	*     功能说明 :
	*			   SceneID	
	*			       调用该函数场景ID
	*			   OwnerID
	*					所有者ID	
	*
	*				
	*/
	该函数注册为 AddGPOwner
	INT		LuaFnAddGPOwner(Lua_State* L) 


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-5-17	10:56
	 *	函数名称：	LuaFnSetItemCreator
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				nBagPos
	 *					物品在背包中的位置
	 *				szCreatorName
	 *					创建者名称
	 *
	 *	功能说明：	给背包里的某个物品增加创建者名称
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnSetItemCreator
	INT LuaFnSetItemCreator(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2006-1-20
	*     函数名称 : LuaFnDelGPOwner
	*     参数说明 : 
	*     功能说明 :
	*			   SceneID	
	*			       调用该函数场景ID
	*			   OwnerID
	*					所有者ID	
	*
	*				
	*/
	该函数注册为 DelGPOwner
	INT		LuaFnDelGPOwner(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2006-1-20
	*     函数名称 : LuaFnGetGPCountByOwner
	*     参数说明 : 
	*     功能说明 :
	*			   SceneID	
	*			       调用该函数场景ID
	*			   OwnerID
	*					所有者ID	
	*			  返回对应OwnerID 所有的gpCount
	*				
	*/
	该函数注册为 GetGPCountByOwner
	INT		LuaFnGetGPCountByOwner(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetItemBoxWorldPosX
	*     参数说明 : 
	*     功能说明 :
	*			   SceneID	
	*			       调用该函数场景ID
	*				ItemBoxID
	*					Obj_ItemBox 编号
	*			返回
	*				fX
	*					Obj_ItemBox世界坐标X
	*
	*				获得Obj_ItemBox世界坐标X
	*/
	该函数注册为 GetItemBoxWorldPosX
	INT		LuaFnGetItemBoxWorldPosX(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetItemBoxWorldPosZ
	*     参数说明 : 
	*     功能说明 :
	*			   SceneID	
	*			       调用该函数场景ID
	*				ItemBoxID
	*					Obj_ItemBox 编号
	*			返回
	*				fZ
	*					Obj_ItemBox世界坐标Z
	*				获得Obj_ItemBox世界坐标Z
	*/
	该函数注册为 GetItemBoxWorldPosZ
	INT		LuaFnGetItemBoxWorldPosZ(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetItemBoxMaxGrowTime
	*     参数说明 : 
	*     功能说明 :
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				ItemBoxID
	*					Obj_ItemBox编号
	*			返回
	*				maxGrowTime
	*					Obj_ItemBox最大生长时间
	*
	*					获得Obj_ItemBox最大生长时间
	*/
	该函数注册为 GetItemBoxMaxGrowTime
	INT		LuaFnGetItemBoxMaxGrowTime(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetItemBoxMaxGrowTime
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				ItemBoxID
	*					Obj_ItemBox编号
	*				maxGrowTime
	*					Obj_ItemBox最大生长时间
	*				设置Obj_ItemBox最大生长时间
	*/
	该函数注册为 SetItemBoxMaxGrowTime
	INT		LuaFnSetItemBoxMaxGrowTime(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetItemBoxPickOwnerTime
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				ItemBoxID
	*					Obj_ItemBox编号
	*				PickProtTime
	*					设置保护时间长度
	*				设置Obj_ItemBox所有者保护时间
	*/
	该函数注册为 SetItemBoxPickOwnerTime
	INT		LuaFnSetItemBoxPickOwnerTime(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnEnableItemBoxPickOwnerTime
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				ItemBoxID
	*					Obj_ItemBox编号
	*				保护时间开始计时
	*/
	该函数注册为 EnableItemBoxPickOwnerTime
	INT		LuaFnEnableItemBoxPickOwnerTime(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnCreateMonsterOnScene
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				MonsterType
	*					怪物类型
	*				fx
	*					位置x
	*				fz
	*					位置z
	*				AIType
	*					怪物AI类型
	*				AIScriptID
	*					怪物AIScriptID
	*
	*				在场景创建怪物 SceneID,MonsterType,x,z,AIType
	*/
	该函数注册为 CreateMonsterOnScene
	INT	  LuaFnCreateMonsterOnScene(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-24
	*     函数名称 : LuaFnCreateMonster
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				MonsterType
	*					怪物类型
	*				fx
	*					位置x
	*				fz
	*					位置z
	*				AIType
	*					怪物AI类型
	*				AIScriptID
	*					怪物AIScriptID
	*				ScriptID
	*					事件脚本ID
	*
	*				在场景创建怪物 SceneID,MonsterType,x,z,AIType
	*/
	该函数注册为 LuaFnCreateMonster
	INT	  LuaFnCreateMonster(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-24
	*     函数名称 : LuaFnDeleteMonster
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				ObjID
	*					怪物号
	*
	*				删除动态创建出来的怪(或NPC)，注意：场景里摆的怪不能被删除
	*/
	该函数注册为 LuaFnDeleteMonster
	INT	  LuaFnDeleteMonster(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddStorePointOnScene
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*						场景编号
	*				StorePointType
	*						藏宝点类型
	*				fx
	*						藏宝点x
	*				fz		
	*						藏宝点z
	*				
	*				在场景里添加一个藏宝点
	*/
	该函数注册为 AddStorePointOnScene
	INT	LuaFnAddStorePointOnScene(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : DelStorePointOnScene
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*						场景编号
	*				StorePointType
	*						藏宝点类型
	*				fx
	*						藏宝点x
	*				fz		
	*						藏宝点z
	*				
	*				在场景里删除一个藏宝点
	*/
	该函数注册为 DelStorePointOnScene
	INT	LuaFnDelStorePointOnScene(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnFindStorePointOnScene
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*						场景编号
	*				StorePointType
	*						藏宝点类型
	*				fx
	*						藏宝点x
	*				fz		
	*						藏宝点z
	*				
	*				在场景里查找一个藏宝点
	*/
	该函数注册为 FindStorePointOnScene
	INT	LuaFnFindStorePointOnScene(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetStorePointType
	*     参数说明 : 
	*				SceneID
	*						场景编号
	*				StorePointType
	*						藏宝点类型
	*				fx
	*						藏宝点x
	*				fz		
	*						藏宝点z
	*	
	*     功能说明 :				
	*				在场景里获得一个藏宝点类型
	*/
	该函数注册为 GetStorePointType
	INT	LuaFnGetStorePointType(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnTryRecieveItem
	*     参数说明 : 
	*				SceneID
	*						场景编号
	*				HumanID
	*						人物编号
    *				ItemType
	*						物品9位标示号
	*				Quality
	*						蓝色装备品质段,其他装备随便添一个正数
	*					
	*	
	*     功能说明 :
	*				尝试接受物品
	*/
	该函数注册为 TryRecieveItem
	该函数注册为 LuaFnTryRecieveItem
	INT	LuaFnTryRecieveItem(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetHumanWorldX
	*     参数说明 : 
	*     功能说明 :
	*				获得Obj_Human的世界坐标X 参数SceneID,HumanID,返回WorldX
	*/
	该函数注册为 GetHumanWorldX
	INT	LuaFnGetHumanWorldX(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetHumanWorldZ
	*     参数说明 : 
	*     功能说明 :
	*				获得Obj_Human的世界坐标Z 参数SceneID,HumanID,返回WorldZ
	*/
	该函数注册为 GetHumanWorldZ
	INT	LuaFnGetHumanWorldZ(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetStoreMapWorldX
	*     参数说明 : 

	*				SceneID
	*					场景编号
	*				HumanID
	*					人物编号
	*				BagIndex
	*					背包位置
	*				fx
	*					藏宝图x
	*     功能说明 :
	*			获得藏宝图位置X
	*/
	该函数注册为 GetStoreMapX
	INT	LuaFnGetStoreMapX(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetStoreMapWorldZ
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				HumanID
	*					人物编号
	*				BagIndex
	*					背包位置
	*				fz
	*					藏宝图z
	*			获得藏宝图位置Z
	*/
	该函数注册为 GetStoreMapZ
	INT	LuaFnGetStoreMapZ(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetStoreMapSceneID
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				HumanID
	*					人物编号
	*				BagIndex
	*					背包位置
	*				MapSceneID
	*					藏宝图场景
	*     功能说明 :
	*			获得藏宝图场景ID
	*/
	该函数注册为 GetStoreMapSceneID
	INT	LuaFnGetStoreMapSceneID(Lua_State* L) 


	/****************************************************************************
	*	开发人员：	
	*	编写时间：	2005-11-1	10:15
	*	函数名称：	LuaFnQueryAreaStandingTime
	*	参数说明：
	*				sceneId
	*					场景编号
	*				selfId
	*					调用者 ID，无符号整型
	*
	*	功能说明：	查询玩家在事件区域停留的时间
	*	修改记录：
	*****************************************************************************/
	该函数注册为 QueryAreaStandingTime
	INT LuaFnQueryAreaStandingTime(Lua_State* L)


	/****************************************************************************
	*	开发人员：	
	*	编写时间：	2005-11-1	11:02
	*	函数名称：	LuaFnResetAreaStandingTime
	*	参数说明：
	*				sceneId
	*					场景编号
	*				selfId
	*					调用者 ID，无符号整型
	*				nTimeStampOffset
	*					新的时间戳偏移量（毫秒）
	*
	*	功能说明：	重新设置玩家进入事件区域的时间（作为下一次触发事件的时间戳）
	*	修改记录：
	*****************************************************************************/
	该函数注册为 ResetAreaStandingTime
	INT LuaFnResetAreaStandingTime(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-4-10	15:06
	 *	函数名称：	LuaFnSendAbilitySuccessMsg
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				AbilityId
	 *					生活技能 ID 号
	 *				RecipeId
	 *					配方 ID 号
	 *				ItemSerialNum
	 *					物品序列号
	 *
	 *	功能说明：	向客户端发送物品制造成功的消息
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnSendAbilitySuccessMsg
	INT LuaFnSendAbilitySuccessMsg(Lua_State* L)


脚本系统C导出函数文档	LuaFnTbl_Attr.h
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetPos
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
    *					调用该函数场景ID,short类型
	*				selfId
	*					调用者ID,无符号整型
	*				x
	*					需要warp到的新位置的X坐标值
	*				z
	*					需要warp到的新位置的Z坐标值
	*     功能说明 :
	*				角色的同场景转移
	*				设置角色(玩家和怪物)跳转到当前地图上的某点
	*/
	该函数注册为 SetPos
	INT LuaFnSetPos(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetRevPos
	*     参数说明 : 
	*				L
	*				LuaState执行环境指针
	*				sceneId
	*				调用该函数场景ID,short
	*				selfId
	*				调用者ID,无符号整型
	*				x
	*					位置x坐标
	*				z
	*					位置z坐标
	*     功能说明 :
	*				设置玩家的重生点（再次登入当前世界的位置）
	*				这个函数有待细究
	*/
	该函数注册为 SetRevPos
	INT LuaFnSetRevPos(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnNewWorld
	*     参数说明 : 
	*				L
	*				LuaState执行环境指针
	*				sceneId
	*				调用该函数场景ID,short
	*				selfId
	*				调用者ID,无符号整型
	*				targetSceneId
	*				将要跳转场景ID,short
	*				x
	*					位置x坐标
	*				z
	*					位置z坐标
	*     功能说明 :
	*				用于切换场景
	*				把selfId的玩家从sceneId标志的场景跳转到targetSceneId标志的场景的(x,z)位置
	*				示例:NewWorld(sceneId,selfId,4,100,102)
	*/
	该函数注册为 NewWorld
	INT LuaFnNewWorld(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetWorldPos
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
    *				得到角色(character)当前坐标
	*				返回x和z的FLOAT类型值
	*				示例: x,z = GetWorldPos(sceneId,selfId) 
	*/
	该函数注册为 GetWorldPos
	该函数注册为 LuaFnGetWorldPos
	INT LuaFnGetWorldPos(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetPatrolId
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				设置Obj_Monster的巡逻路径
	*				示例:SetPatrolId(sceneId,selfId,0)
	*				patrolId的有效值是 
	*				0 到 该sceneId所指场景_patrolpoint.ini文件PATROLNUMBER(不包括)之间的值
	*/
	该函数注册为 SetPatrolId
	INT LuaFnSetPatrolId(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnIsCaptain
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				判断当前玩家是否为队长，返回值为0：不是队长；返回值为1：队长。
	*				没有测试
	*/
	该函数注册为 IsCaptain
	INT LuaFnIsCaptain(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddXinFa
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				xinfaId
	*					心法编号
	*     功能说明 :
	*				给当前玩家加某个心法
	*				没有测试
	*/
	该函数注册为 AddXinFa
	INT LuaFnAddXinFa(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddSkill
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				skillId
	*					技能编号
	*     功能说明 :
	*				给当前玩家加某个技能
	*				没有测试
	*/
	该函数注册为 AddSkill
	INT LuaFnAddSkill(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDelXinFa
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				xinfaId
	*					心法编号
	*     功能说明 :
	*				删除当前玩家的某个心法
	*				没有测试
	*/
	该函数注册为 DelXinFa
	INT LuaFnDelXinFa(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDelSkill
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				skillId
	*					技能编号				
	*     功能说明 :
	*				删除当前玩家的某个技能
	*				没有测试
	*/
	该函数注册为 DelSkill
	INT LuaFnDelSkill(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnHaveXinFa
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				xinfaId
	*					心法编号				
	*     功能说明 :
	*				判断当前玩家是否有某个心法。未学过该技能返回-1，否则返回心法等级
	*				没有测试
	*/
	该函数注册为 HaveXinFa
	INT LuaFnHaveXinFa(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnHaveSkill
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				skillId
	*					技能编号				
	*     功能说明 :
	*				判断当前玩家是否有某个技能。未学过该技能返回-1，否则返回技能等级。//? 技能没有等级
	*/
	该函数注册为 HaveSkill
	INT LuaFnHaveSkill(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnUseSkill
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				skillId
	*					技能编号				
	*     功能说明 :
	*				有待与策划商榷
	*/
	该函数注册为 UseSkill
	INT LuaFnUseSkill(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetCurCamp
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得玩家的当前阵营
	*				没有测试
	*/
	该函数注册为 GetCurCamp
	INT LuaFnGetCurCamp(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetCurCamp
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				campIndex
	*					阵营编号,INT类型
	*     功能说明 :
	*				修改玩家阵营,函数体尚未填写
	*/
	该函数注册为 SetCurCamp
	INT LuaFnSetCurCamp (Lua_State* L)	


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-10	19:42
	 *	函数名称：	LuaFnGetGUID
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	得到调用者的 GUID
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnGetGUID
	INT LuaFnGetGUID(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetExp
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得玩家的当前战斗经验值
	*/
	该函数注册为 GetExp
	INT LuaFnGetExp(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddExp
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				exp
	*					经验值增加,INT
	*     功能说明 :
	*				增加玩家的战斗经验值
	*/
	该函数注册为 AddExp
	该函数注册为 LuaFnAddExp
	INT LuaFnAddExp(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetHp
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得玩家的生命值
	*				返回玩家的HP值
	*/
	该函数注册为 GetHp
	INT LuaFnGetHp(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnRestoreHp
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				恢复角色的生命全满
	*				把角色的生命值设为MaxHP值
	*/
	该函数注册为 RestoreHp
	INT LuaFnRestoreHp(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMp
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得角色的内力值
	*				返回角色的MP值
	*/
	该函数注册为 GetMp
	INT LuaFnGetMp(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnRestoreMp
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				恢复玩家的内力全满
	*/
	该函数注册为 RestoreMp
	INT LuaFnRestoreMp(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetCon
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得玩家的体力值
	*/
	该函数注册为 GetCon
	INT LuaFnGetCon(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetSex
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得玩家的性别
	*				返回值为0：女
	*				返回值为1：男
	*/
	该函数注册为 GetSex
	该函数注册为 LuaFnGetSex
	INT LuaFnGetSex(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetName
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得角色(Obj_Character)名字，返回一个字符串
	*				角色包括玩家和Obj_Monster
	*				示例: npcName = GetName(sceneId,npcId)
	*/
	该函数注册为 GetName
	该函数注册为 LuaFnGetName
	INT LuaFnGetName(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMoney
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得玩家的现金
	*				返回玩家拥有的金钱数
	*/
	该函数注册为 GetMoney
	该函数注册为 LuaFnGetMoney
	INT LuaFnGetMoney(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnCostMoney
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				payment
	*					金钱消耗
	*     功能说明 :
	*				扣除玩家金钱
	*				成功返回1
	*				以后是否考虑返回修改后的玩家金钱数?
	*				目前payment应该为正值
	*				失败返回-1
	*/
	该函数注册为 CostMoney
	该函数注册为 LuaFnCostMoney
	INT LuaFnCostMoney(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddMoney
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				profit
	*					金钱增加数目
	*     功能说明 :
	*				增加玩家金钱
	*				返回1,成功
	*				以后是否考虑返回修改后的玩家金钱数?
	*				返回-1,失败
	*/
	该函数注册为 AddMoney
	该函数注册为 LuaFnAddMoney
	INT LuaFnAddMoney(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetLevel
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				取得玩家当前等级
	*				Level的合法范围在?-?
	*				返回-1,失败
	*/
	该函数注册为 GetLevel
	该函数注册为 LuaFnGetLevel
	INT LuaFnGetLevel(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 　
	*     编写时间 : 2005-10-20
	*     函数名称 : LuaFnSetLevel
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				level
	*					角色级别,INT
	*     功能说明 :
	*				设置玩家玩家等级
	*				Level的合法范围在1-60
	*				返回-1,失败
	*/
	该函数注册为 SetLevel
	INT LuaFnSetLevel(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-7	16:36
	 *	函数名称：	LuaFnCanLevelUp
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	判断该玩家是否可以升级
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnCanLevelUp
	INT LuaFnCanLevelUp(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-28
	*     函数名称 : LuaFnGetItemTableIndexByIndex
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				itemIndex
	*					物品在身上的位置索引
	*     功能说明 :
	*				取当前位置的物品种类
	*				
	*/
	该函数注册为 GetItemTableIndexByIndex
	该函数注册为 LuaFnGetItemTableIndexByIndex
	INT LuaFnGetItemTableIndexByIndex(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-30	14:47
	 *	函数名称：	LuaFnGetMaterialStartBagPos
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	得到背包中材料的起始位置
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnGetMaterialStartBagPos
	INT LuaFnGetMaterialStartBagPos(Lua_State* L)	


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-30	14:47
	 *	函数名称：	LuaFnGetMaterialEndBagPos
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	得到背包中材料的结束位置
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnGetMaterialEndBagPos
	INT LuaFnGetMaterialEndBagPos(Lua_State* L)	


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-30	19:30
	 *	函数名称：	LuaFnGetItemCountInBagPos
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				uBagPos
	 *					背包中位置
	 *
	 *	功能说明：	得到背包某格中物品的数量
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnGetItemCountInBagPos
	INT LuaFnGetItemCountInBagPos(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetItemCount
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				itemTypeSn
	*					ItemType的9位数号(实际为8位)
	*     功能说明 :
	*				判断当前玩家有多少个道具
	*				只从玩家的装备和物品包中查找
	*/
	该函数注册为 GetItemCount
	该函数注册为 LuaFnGetItemCount
	INT LuaFnGetItemCount(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-1-22
	*     函数名称 : LuaFnHaveItemInBag
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				itemTypeSn
	*					ItemType的9位数号(实际为8位)
	*     功能说明 :
	*				判断当前玩家包中是否拥有某个道具
	*				如果成功,返回大于0
	*				如果失败,返回-1
	*/
	该函数注册为 HaveItemInBag
	INT LuaFnHaveItemInBag (Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnHaveItem
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				itemTypeSn
	*					ItemType的9位数号(实际为8位)
	*     功能说明 :
	*				判断当前玩家是否拥有某个道具
	*				如果成功,返回大于0
	*				如果失败,返回-1
	*/
	该函数注册为 HaveItem
	INT LuaFnHaveItem (Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDelItem
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				itemTypeSn
	*					ItemType的9位数号(实际为8位)
	*				delCount
	*					删除数目
	*     功能说明 :
	*				删除当前玩家的某个道具	
	*				不仅是指任务道具	
	*				不对已装备的道具删除
	*				删除成功,返回1
	*				删除失败,返回-1
	*/
	该函数注册为 DelItem
	INT LuaFnDelItem (Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnEraseItem
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				bagIndex
	*					背包的格子
	*     功能说明 :
	*				删除当前玩家背包某一格的道具
	*				删除成功,返回1
	*				删除失败,返回-1
	*/
	该函数注册为 EraseItem
	该函数注册为 LuaFnEraseItem
	INT LuaFnEraseItem (Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMenPai
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获取玩家当前所属门派ID
	*/
	该函数注册为 GetMenPai
	该函数注册为 LuaFnGetMenPai
	INT LuaFnGetMenPai(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-13
	*     函数名称 : LuaFnSetMenPai
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				menpaiId
	*					门派ID,INT
	*     功能说明 :
	*				设置玩家当前所属门派为menpaiId
	*/
	该函数注册为 SetMenPai
	INT LuaFnSetMenPai(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : yangei
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMenPaiMaster
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获取玩家所在帮会的帮主名称（字符串）
	*				函数体没有实现
	*/
	该函数注册为 GetMenPaiMaster
	INT LuaFnGetMenPaiMaster(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMenPaiFigure
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				返回玩家在帮会中的阶级：0帮众，1队长，2长老，4帮主
	*				函数体没有实现
	*/
	该函数注册为 GetMenPaiFigure
	INT LuaFnGetMenPaiFigure(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMenPaiTitle
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				返回玩家在帮会中的头衔，返回头衔中不包括帮会名
	*				函数体没有实现
	*/
	该函数注册为 GetMenPaiTitle
	INT LuaFnGetMenPaiTitle(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDestroyMenPai
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				menpaiId
	*					帮会编号
	*     功能说明 :
	*				解散帮会
	*				函数体没有实现
	*/
	该函数注册为 DestroyMenPai
	INT LuaFnDestroyMenPai(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetNpcCurCamp
	*     参数说明 : 
	*     功能说明 :
	*				改变NPC的阵营,函数体尚未填写
	*/
	该函数注册为 SetNpcCurCamp
	INT LuaFnSetNpcCurCamp(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnRestoreNpcCamp
	*     参数说明 : 
	*     功能说明 :
	*				恢复NPC原有的阵营,函数体尚未填写
	*/
	该函数注册为 RestoreNpcCamp
	INT LuaFnRestoreNpcCamp(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-1-23
	*     函数名称 : LuaFnSetDamage
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					自己的ObjID
	*				targetId
	*					目标的ObjID
	*				damage
	*					需要设置的伤害
	*     功能说明 :
	*				设置某个obj的伤害
	*/
	该函数注册为 LuaFnSetDamage
	INT LuaFnSetDamage(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnGmKillObj
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					自己的ObjID
	*				targetId
	*					目标的ObjID
	*     功能说明 :
	*				直接杀死指定的obj;
	*/
	该函数注册为 LuaFnGmKillObj
	INT LuaFnGmKillObj(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-1-23
	*     函数名称 : LuaFnFindMonsterByGUID
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				guid
	*					需要寻找的怪物GUID
	*     功能说明 :
	*				寻找场景中的某个怪物,根据GUID
	*/
	该函数注册为 LuaFnFindMonsterByGUID
	INT LuaFnFindMonsterByGUID(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnGetHumanMaxVigor
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*     功能说明 :
	*				得到玩家活力最大值
	*/
	该函数注册为 GetHumanMaxVigor
	INT LuaFnGetHumanMaxVigor(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnGetHumanMaxEnergy
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*     功能说明 :
	*				得到玩家精力最大值
	*/
	该函数注册为 GetHumanMaxEnergy
	INT LuaFnGetHumanMaxEnergy(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnGetHumanVigor
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*     功能说明 :
	*				得到玩家活力值
	*/
	该函数注册为 GetHumanVigor
	INT LuaFnGetHumanVigor(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnGetHumanEnergy
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*     功能说明 :
	*				得到玩家精力值
	*/
	该函数注册为 GetHumanEnergy
	INT LuaFnGetHumanEnergy(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnSetHumanMaxVigor
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家活力最大值
	*/
	该函数注册为 SetHumanMaxVigor
	INT LuaFnSetHumanMaxVigor(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnSetHumanMaxEnergy
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家精力最大值
	*/
	该函数注册为 SetHumanMaxEnergy
	INT LuaFnSetHumanMaxEnergy(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnSetHumanVigor
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家活力
	*/
	该函数注册为 SetHumanVigor
	INT LuaFnSetHumanVigor(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnSetHumanEnergy
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家精力
	*/
	该函数注册为 SetHumanEnergy
	INT LuaFnSetHumanEnergy(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnGetHumanVigorRegeneRate
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*     功能说明 :
	*				得到玩家活力恢复速度
	*/
	该函数注册为 GetHumanVigorRegeneRate
	INT LuaFnGetHumanVigorRegeneRate(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnGetHumanEnergyRegeneRate
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*     功能说明 :
	*				得到玩家精力恢复速度
	*/
	该函数注册为 GetHumanEnergyRegeneRate
	INT LuaFnGetHumanEnergyRegeneRate(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnSetHumanVigorRegeneRate
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家活力恢复速度
	*/
	该函数注册为 SetHumanVigorRegeneRate
	INT LuaFnSetHumanVigorRegeneRate(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnSetHumanEnergyRegeneRate
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家精力恢复速度
	*/
	该函数注册为 SetHumanEnergyRegeneRate
	INT LuaFnSetHumanEnergyRegeneRate(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnSetHumanHairColor
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家头发颜色
	*/
	该函数注册为 SetHumanHairColor
	INT LuaFnSetHumanHairColor(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnGetHumanHairColor
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家头发颜色
	*/
	该函数注册为 GetHumanHairColor
	INT LuaFnGetHumanHairColor(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnSetHumanHairModel
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				uHairStyle
	*					设值的值
	*     功能说明 :
	*				设置玩家头发模型
	*/
	该函数注册为 SetHumanHairModel
	INT LuaFnSetHumanHairModel(Lua_State* L)	


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-5-15	14:36
	 *	函数名称：	LuaFnChangeHumanHairModel
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				uHairStyle
	 *					发型值
	 *
	 *	功能说明：	将玩家的发型换成 uHairStyle
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnChangeHumanHairModel
	INT LuaFnChangeHumanHairModel(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-02-20
	*     函数名称 : LuaFnGetHumanHairModel
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					玩家ID
	*				value
	*					设值的值
	*     功能说明 :
	*				设置玩家头发模型
	*/
	该函数注册为 GetHumanHairModel
	INT LuaFnGetHumanHairModel(Lua_State* L)	


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-5-15	14:26
	 *	函数名称：	LuaFnGetHumanGoodBadValue
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	得到 selfId 的善恶值
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnGetHumanGoodBadValue
	INT LuaFnGetHumanGoodBadValue(Lua_State* L)	


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-5-15	14:26
	 *	函数名称：	LuaFnSetHumanGoodBadValue
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				value
	 *					善恶值
	 *
	 *	功能说明：	设置 selfId 的善恶值
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnSetHumanGoodBadValue
	INT LuaFnSetHumanGoodBadValue(Lua_State* L)	


脚本系统C导出函数文档	LuaFnTbl_Battle.h
	 */
	该函数注册为 LuaFnGetPetCount
	INT	LuaFnGetPetCount(Lua_State* L)


	 */
	该函数注册为 LuaFnGetPet_DataID
	INT	LuaFnGetPet_DataID(Lua_State* L)


	 */
	该函数注册为 LuaFnGetPet_Level
	INT	LuaFnGetPet_Level(Lua_State* L)


	 */
	该函数注册为 LuaFnGetPet_TakeLevel
	INT	LuaFnGetPet_TakeLevel(Lua_State* L)


	 */
	该函数注册为 LuaFnGetPet_HP
	INT	LuaFnGetPet_HP(Lua_State* L)


	 */
	该函数注册为 LuaFnGetPet_MaxHP
	INT	LuaFnGetPet_MaxHP(Lua_State* L)


	 */
	该函数注册为 LuaFnGetPet_Life
	INT	LuaFnGetPet_Life(Lua_State* L)


	 */
	该函数注册为 LuaFnGetPet_Type
	INT	LuaFnGetPet_Type(Lua_State* L)


	 */
	该函数注册为 LuaFnGetPet_Generation
	INT	LuaFnGetPet_Generation(Lua_State* L)


	 */
	该函数注册为 LuaFnGetPet_Happyness
	INT	LuaFnGetPet_Happyness(Lua_State* L)


	 */
	该函数注册为 LuaFnGetPet_StrPerception
	INT	LuaFnGetPet_StrPerception(Lua_State* L)


	 */
	该函数注册为 LuaFnGetPet_ConPerception
	INT	LuaFnGetPet_ConPerception(Lua_State* L)


	 */
	该函数注册为 LuaFnGetPet_DexPerception
	INT	LuaFnGetPet_DexPerception(Lua_State* L)


	 */
	该函数注册为 LuaFnGetPet_SprPerception
	INT	LuaFnGetPet_SprPerception(Lua_State* L)


	 */
	该函数注册为 LuaFnGetPet_IntPerception
	INT	LuaFnGetPet_IntPerception(Lua_State* L)


	 */
	该函数注册为 LuaFnGetPet_GenGu
	INT	LuaFnGetPet_GenGu(Lua_State* L)


	 */
	该函数注册为 LuaFnGetPet_RemainPoint
	INT	LuaFnGetPet_RemainPoint(Lua_State* L)


	 */
	该函数注册为 LuaFnGetPet_Exp
	INT	LuaFnGetPet_Exp(Lua_State* L)


	 */
	该函数注册为 LuaFnGetPet_AttrLevel1
	INT	LuaFnGetPet_AttrLevel1(Lua_State* L)


	 */
	该函数注册为 LuaFnGetPet_Skill
	INT	LuaFnGetPet_Skill(Lua_State* L)


	 */
	该函数注册为 LuaFnCreatePet
	INT	LuaFnCreatePet(Lua_State* L)


	 */
	该函数注册为 LuaFnDeletePet
	INT	LuaFnDeletePet(Lua_State* L)


脚本系统C导出函数文档	LuaFnTbl_DoAction.h
	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-5-17	11:29
	 *	函数名称：	LuaFnDoAction
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *				nActionID
	 *					动作编号
	 *				TargetID
	 *					目标编号（没有目标填 -1 就可以了）
	 *	功能说明：	对目标做动作
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnDoAction
	INT	LuaFnDoAction(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-1-10
	*     函数名称 : LuaFnUnitDoEmoteAction
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*				nSkillID
	*					技能编号
	*				nTargetID
	*					目标编号
	*     功能说明 :
	*/
	该函数注册为 LuaFnUnitDoEmoteAction
	INT	LuaFnUnitDoEmoteAction(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-1-10
	*     函数名称 : LuaFnUnitUseSkill
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*				nSkillID
	*					技能编号
	*				nTargetID
	*					目标编号
	*				fPos_X
	*					目标位置的X坐标
	*				fPos_Z
	*					目标位置的Z坐标
	*				fDir
	*					目标方向
	*				bPassSkillCheck
	*					是否忽略技能的条件检测和物品消耗
	*     功能说明 :
	*/
	该函数注册为 LuaFnUnitUseSkill
	INT	LuaFnUnitUseSkill(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnSendSpecificImpactToUnit
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*				nSenderID
	*					效果创造者ID,无符号整型
	*				nTargetID
	*					目标角色ID,无符号整型
	*				nImpactDataIndex
	*					效果数据索引,无符号整型
	*				nDelayTime
	*					延迟时间
	*     功能说明 :
	*				
	*/
	该函数注册为 LuaFnSendSpecificImpactToUnit
	INT	LuaFnSendSpecificImpactToUnit(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnCancelSpecificImpact
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*				nImpactDataIndex
	*					效果数据索引, 无符号整型；在标准效果表里查询
	*     功能说明 :
	*				
	*/
	该函数注册为 LuaFnCancelSpecificImpact
	INT	LuaFnCancelSpecificImpact(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnVerifyUsedItem
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*     功能说明 :
	*				
	*/
	该函数注册为 LuaFnVerifyUsedItem
	INT	LuaFnVerifyUsedItem(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnDepletingUsedItem
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*     功能说明 :
	*				
	*/
	该函数注册为 LuaFnDepletingUsedItem
	INT	LuaFnDepletingUsedItem(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnGetTargetObjID
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*     功能说明 :
	*				
	*/
	该函数注册为 LuaFnGetTargetObjID
	INT	LuaFnGetTargetObjID(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnGetTargetPosition_X
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*     功能说明 :
	*				
	*/
	该函数注册为 LuaFnGetTargetPosition_X
	INT	LuaFnGetTargetPosition_X(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnGetTargetPosition_Z
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*     功能说明 :
	*				
	*/
	该函数注册为 LuaFnGetTargetPosition_Z
	INT	LuaFnGetTargetPosition_Z(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnGetTargetBagSlot
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*     功能说明 :
	*				
	*/
	该函数注册为 LuaFnGetTargetBagSlot
	INT	LuaFnGetTargetBagSlot(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-4-11	19:29
	 *	函数名称：	LuaFnGetBagIndexOfUsedItem
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	?
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnGetBagIndexOfUsedItem
	INT LuaFnGetBagIndexOfUsedItem(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnGetLowSectionOfTargetPetGuid
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*     功能说明 :
	*				
	*/
	该函数注册为 LuaFnGetLowSectionOfTargetPetGuid
	INT	LuaFnGetLowSectionOfTargetPetGuid(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnGetHighSectionOfTargetPetGuid
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*     功能说明 :
	*				
	*/
	该函数注册为 LuaFnGetHighSectionOfTargetPetGuid
	INT	LuaFnGetHighSectionOfTargetPetGuid(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnIsScriptDynamicParamIndexLegal
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*				nIndex
	*					参数索引
	*     功能说明 :
	*				
	*/
	该函数注册为 LuaFnIsScriptDynamicParamIndexLegal
	INT	LuaFnIsScriptDynamicParamIndexLegal(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnGetScriptDynamicParamByIndex
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*				nIndex
	*					参数索引
	*     功能说明 :
	*				
	*/
	该函数注册为 LuaFnGetScriptDynamicParamByIndex
	INT	LuaFnGetScriptDynamicParamByIndex(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-7
	*     函数名称 : LuaFnSetScriptDynamicParamByIndex
	*     参数说明 : 
	*				sceneID
	*					场景编号
	*				nSelfID
	*					调用者ID,无符号整型
	*				nIndex
	*					参数索引
	*				nValue
	*					参数值
	*     功能说明 :
	*				
	*/
	该函数注册为 LuaFnSetScriptDynamicParamByIndex
	INT	LuaFnSetScriptDynamicParamByIndex(Lua_State* L)


脚本系统C导出函数文档	LuaFnTbl_Guild.h
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-3-21
	*     函数名称 : LuaFnGuildList
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				
	*/
	该函数注册为 GuildList
	INT	LuaFnGuildList(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-3-21
	*     函数名称 : LuaFnGuildCreate
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				
	*/
	该函数注册为 GuildCreate
	INT	LuaFnGuildCreate(Lua_State* L) 


脚本系统C导出函数文档	LuaFnTbl_Misc.h
/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetTimer
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 selfId
	*					调用者ID,无符号整型
	*				 scriptId
	*					执行脚本ID,short类型
	*				 funcName
	*					计时器回调调函数名,字符串类型
	*				 tickTime
	*					计时器调用间隔时间
	*     功能说明 :
	*				 给玩家打开计时器,时间到时将自动调用OnTimer函数	
	*				 玩家本身身上并没有计时器，脚本程序需要计时器时向系统申请“租用”，由系统分配
	*                返回－1,表示错误
	*/
	该函数注册为 SetTimer
	INT LuaFnSetTimer(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnStopTimer
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				sceneTimerIndex
	*					计时器在场景计时器池中的数组下标,有SetTimer时返回		
	*     功能说明 :
    *				关闭当前玩家的计时器	//每个玩家身上最多可以绑定三个计时器
	*/
	该函数注册为 StopTimer
	INT LuaFnStopTimer(Lua_State* L)	


/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-24
	*     函数名称 : LuaFnCheckTimer
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 TimeIndex
	*					计时器索引
	*     功能说明 : 判断TimeIndex号计时器是否已经在使用中
	*                返回1,表示在使用；返回0，表示没有使用; 返回-1，表示异常
	*/
	该函数注册为 CheckTimer
	INT LuaFnCheckTimer(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetGameTime
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
    *				获取当前玩家在线时间，单位为秒
	*				用当前时间减去上线时间
	*				没有测试
	*/
	该函数注册为 GetGameTime
	INT LuaFnGetGameTime(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-1-10
	*     函数名称 : LuaFnGetCurrentTime
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*     功能说明 :
	*				获取当前时间，单位为秒
	*/
	该函数注册为 LuaFnGetCurrentTime
	INT LuaFnGetCurrentTime(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddGlobalCountNews
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				newstr
	*					公告内容,字符串指针类型
	*				count
	*					发送次数
	*     功能说明 :
    *				向全组服务器发送新闻公告，公告发送指定次数
	*				没有测试
	*/
	该函数注册为 AddGlobalCountNews
	该函数注册为 LuaFnAddGlobalCountNews
	INT LuaFnAddGlobalCountNews(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnPrintNum
	*     参数说明 : 
	*				num
	*				消息框的提示数字
	*     功能说明 :
	*				弹出消息框,测试脚本用
	*/
	该函数注册为 PrintNum
	INT LuaFnPrintNum(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnPrintStr
	*     参数说明 : 
	*				strText
	*				消息框的提示字符串
	*     功能说明 :
	*				弹出消息框,测试脚本用
	*/
	该函数注册为 PrintStr
	INT LuaFnPrintStr(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnCallScriptFunction
	*     参数说明 : 
	*     功能说明 :
	*					调用可变参数的任意脚本函数
	*/
	该函数注册为 CallScriptFunction
	INT LuaFnCallScriptFunction(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-19
	*     函数名称 : LuaFnIsInDist
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					对方的ID编号,ObjID
	*				dist
	*					self与target距离
	*     功能说明 :
	*				判断selfId与targetId之间的距离是否在dist内(包括)
	*				目前用于护送任务,判断玩家和被护送者之间的距离
	*/
	该函数注册为 IsInDist
	INT LuaFnIsInDist(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-15
	*     函数名称 : LuaFnGetDist
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					对方的ID编号,ObjID
	*     功能说明 :
	*				得到selfId和targetId之间的距离
	*/
	该函数注册为 GetDist
	INT LuaFnGetDist(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-20
	*     函数名称 : LuaFnGuid2ObjId
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				guid
	*					GUID,GUID_t类型，仅适用于玩家的GUID转换
	*     功能说明 :
	*				把Guid转换为objId
	*/
	该函数注册为 LuaFnGuid2ObjId
	INT LuaFnGuid2ObjId(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-16
	*     函数名称 : LuaFnObjId2Guid
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 : 返回selfId的GUID
	*/
	该函数注册为 LuaFnObjId2Guid
	INT LuaFnObjId2Guid(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-21
	*     函数名称 : LuaFnRemoveMonster
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				删除Obj_Monster,把Obj_Monster从地图删除
	*/
	该函数注册为 RemoveMonster
	INT LuaFnRemoveMonster(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-2
	*     函数名称 : LuaFnAwardTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				TitleIndex
	*					称号ID
	*				TitleId
	*					称号ID
	*     功能说明 :
	*				为selfId授予一个称号
	*/
	该函数注册为 AwardTitle
	该函数注册为 LuaFnAwardTitle
	INT	LuaFnAwardTitle(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-2
	*     函数名称 : LuaFnDeleteTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				TitleIndex
	*					称号ID
	*     功能说明 :
	*				删除某个称号
	*/
	该函数注册为 DeleteTitle
	INT	LuaFnDeleteTitle(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-3
	*     函数名称 : LuaFnDispatchAllTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				更新此人的所有称号
	*/
	该函数注册为 DispatchAllTitle
	该函数注册为 LuaFnDispatchAllTitle
	INT	LuaFnDispatchAllTitle(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-3
	*     函数名称 : LuaFnGetTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				TitleIndex
	*					称号ID
	*     功能说明 :
	*				获得此人的某个称号
	*/
	该函数注册为 GetTitle
	INT	LuaFnGetTitle(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-3
	*     函数名称 : LuaFnGetDyTitlesNum
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得此人的动态称号个数
	*/
	该函数注册为 GetDyTitlesNum
	INT	LuaFnGetDyTitlesNum(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-3
	*     函数名称 : LuaFnAddDyTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				TitleId
	*					称号ID
	*     功能说明 :
	*				加入新的动态称号，如果满了放在第一个
	*/
	该函数注册为 AddDyTitle
	INT	LuaFnAddDyTitle(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-4
	*     函数名称 : LuaFnAwardNickTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				strText
	*					签名档性质的昵称
	*     功能说明 :
	*				设置此人的签名档性质的昵称
	*/
	该函数注册为 AwardNickTitle
	INT LuaFnAwardNickTitle(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-4
	*     函数名称 : LuaFnAwardSpouseTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				strText
	*					配偶名
	*     功能说明 :
	*				设置配偶名
	*/
	该函数注册为 AwardSpouseTitle
	该函数注册为 LuaFnAwardSpouseTitle
	INT LuaFnAwardSpouseTitle(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-4
	*     函数名称 : LuaFnAwardJieBaiTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				strText
	*					结拜称号
	*     功能说明 :
	*				设置结拜称号
	*/
	该函数注册为 AwardJieBaiTitle
	INT LuaFnAwardJieBaiTitle(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-4
	*     函数名称 : LuaFnAwardBangPaiTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				strText
	*					帮派名
	*     功能说明 :
	*				设置此人的帮派称号
	*/
	该函数注册为 AwardBangpaiTitle
	INT LuaFnAwardBangPaiTitle(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-4
	*     函数名称 : LuaFnAwardShiTuTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				strText
	*					帮派名
	*     功能说明 :
	*				设置此人的帮派称号
	*/
	该函数注册为 AwardShiTuTitle
	INT LuaFnAwardShiTuTitle(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-4
	*     函数名称 : LuaFnAwardShangDianTitle
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				strText
	*					帮派名
	*     功能说明 :
	*				设置此人的帮派称号
	*/
	该函数注册为 AwardShangDianTitle
	INT LuaFnAwardShangDianTitle(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-8
	*     函数名称 : LuaFnEnableBankRentIndex
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				RentIndex
	*					打开的租赁箱
	*     功能说明 :
	*				打开此人的租赁箱
	*/
	该函数注册为 EnableBankRentIndex
	INT LuaFnEnableBankRentIndex(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-1-12
	*     函数名称 : LuaFnGetBankRentIndex
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				获得当前租赁箱已打开的索引
	*/
	该函数注册为 GetBankRentIndex
	INT LuaFnGetBankRentIndex(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-8
	*     函数名称 : LuaFnBankBegin
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				通知客户端开始进行银行交易
	*/
	该函数注册为 BankBegin
	INT LuaFnBankBegin(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-14
	*     函数名称 : LuaSetPlayerDefaultReliveInfo
	*     参数说明 : 
	*				sceneId
	*					场景编号
	*				playerId
	*					进入场景的玩家ID
	*				pszHP
	*					HP的数据(%10=HP恢复10/100，10=HP恢复10点)
	*				pszMP
	*					MP的数据(%10=HP恢复10/100，10=HP恢复10点)
	*				pszSP
	*					SP的数据(%10=HP恢复10/100，10=HP恢复10点)
	*				defaultSceneID
	*					复活的场景ID
	*				defaultPosX
	*					坐标X
	*				defaultPosZ
	*					坐标Z
	*     功能说明 :
	*				玩家的缺省复活相关的数据设置
	*/
	该函数注册为 SetPlayerDefaultReliveInfo
	INT LuaSetPlayerDefaultReliveInfo(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-20
	*     函数名称 : LuaGetDayTime
	*     参数说明 : 
	*					无
	*     功能说明 :
	*				取得当前时间的天数，例如：1140 表示2001年的第140天
	*										  6099 表示2006年的第99天
	*/
	该函数注册为 GetDayTime
	INT LuaGetDayTime(Lua_State* L )


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-23
	*     函数名称 : LuaGetObjCreateTime
	*     参数说明 : 
	*				sceneId
    *					调用该函数场景ID,short类型
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				取得ObjID 为 selfId的obj的创建时间
	*					
	*/
	该函数注册为 GetObjCreateTime
	INT LuaGetObjCreateTime(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-28
	*     函数名称 : LuaFnUpdateAttr
	*     参数说明 : 
	*				sceneId
	*					调用该函数场景ID,short类型
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				通过邮件系统更新人物属性
	*					
	*/
	该函数注册为 LuaFnUpdateAttr
	INT LuaFnUpdateAttr(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-4-26	17:08
	 *	函数名称：	LuaFnIsPasswordSetup
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				flag
	 *					标记，仅当玩家未设置二级密码有效，如果 1，则让调用者设置二级密码，0 则不需要设置
	 *
	 *	功能说明：	判断玩家是否设置二级密码，0 为未设置，1 为已设置
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnIsPasswordSetup
	INT LuaFnIsPasswordSetup(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-4-26	17:21
	 *	函数名称：	LuaFnIsPasswordUnlocked
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				flag
	 *					标记，仅当玩家未解锁二级密码有效，如果 1，则让调用者解锁二级密码，0 则不强制解锁
	 *
	 *	功能说明：	判断玩家是否解锁二级密码，0 为未解锁，1 为已解锁
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnIsPasswordUnlocked
	INT LuaFnIsPasswordUnlocked(Lua_State* L)


脚本系统C导出函数文档	LuaFnTbl_Mission.h
	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnBeginEvent
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*				和EndEvent配合使用
	*				为下一步的相关操作做准备
	*/
	该函数注册为 BeginEvent
	INT LuaFnBeginEvent(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddText
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				msg
	*					提示内容
	*     功能说明 :
	*				添加字符串提示
	*/
	该函数注册为 AddText
	INT LuaFnAddText(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddNumber
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				id
	*					数目,INT
	*     功能说明 :
	*				添加数目提示
	*/
	该函数注册为 AddNumber
	INT LuaFnAddNumber(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddNumText
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				id
	*					第几个提示,INT
	*				msg
	*					提示内容
	*				state 
	*					是否可以交任务和接任务
	*				index
	*					策划用整形数
	*     功能说明 :
	*				添加数目和字符串提示
	*				state和index可以不添
	*/
	该函数注册为 AddEventList
	该函数注册为 AddNumText
	INT LuaFnAddNumText(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-12-9
	*     函数名称 : LuaFnSetNumText
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				number
	*					待设置的值,short
	*     功能说明 :
	*				设置脚本交换用数据
	*/
	该函数注册为 LuaFnSetNumText
	INT LuaFnSetNumText(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-12-9
	*     函数名称 : LuaFnGetNumText
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				number
	*					待设置的值,short
	*     功能说明 :
	*				取出脚本交换用数据
	*/
	该函数注册为 GetNumText
	该函数注册为 GetEventList
	INT LuaFnGetNumText(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddMoneyBonus
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				count
	*					奖励金钱数目
	*     功能说明 :
	*				添加Money
	*/
	该函数注册为 AddMoneyBonus
	INT LuaFnAddMoneyBonus(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddItemBonus
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				itemTypeSn
	*					奖励物品的ItemType的9位编号
	*				count
	*					奖励数目
	*     功能说明 :
	*				添加count个编号为itemType道具
	*/
	该函数注册为 AddItemBonus
	INT LuaFnAddItemBonus(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddItemDemand
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				itemTypeSn
	*					需求物品的ItemType的9位编号
	*				count
	*					需求数目

	*     功能说明 :
	*				需求count个编号为itemType道具
	*/
	该函数注册为 AddItemDemand
	INT LuaFnAddItemDemand(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddRandItemBonus
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*					提示奖励随机道具
	*/
	该函数注册为 AddRandItemBonus
	INT LuaFnAddRandItemBonus(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddRadioItemBonus
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				itemTypeSn
	*					物品的ItemType9位编号
	*				count
	*					奖励多选一物品的数目
	*     功能说明 :
	*				添加多选一道具
	*/
	该函数注册为 AddRadioItemBonus
	INT LuaFnAddRadioItemBonus(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnEndEvent
	*     参数说明 : 
	*     功能说明 :
	*				和BeginEvent配合使用
	*				之后需调用某个Dispatch***函数
	*/
	该函数注册为 EndEvent
	INT LuaFnEndEvent(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDispatchEventList
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*     功能说明 :
	*				向客户端返回该Npc可触发事件列表
	*				可触发事件由该npc的挂的事件OnEnumerate函数判断
	*				如果玩家满足任务条件,通过AddNumText添加
	*				BeginEvent
	*				AddText
	*				可能: AddNumText
	*				EndEvent
	*/
	该函数注册为 DispatchEventList
	INT LuaFnDispatchEventList(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDispatchRegieEventList
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*     功能说明 :
	*				向客户端返回该Npc可触发事件列表
	*				可触发事件由该npc的挂的事件OnEnumerate函数判断
	*				如果玩家满足任务条件,通过AddNumText添加
	*				BeginEvent
	*				AddText
	*				可能: AddNumText
	*				EndEvent
	*/
	该函数注册为 DispatchRegieEventList
	INT LuaFnDispatchRegieEventList(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDispatchMissionInfo
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				scriptId
	*					脚本编号
	*				misId
	*					任务编号
	*     功能说明 :
	*				向客户端返回任务信息处理
	*				
	*/
	该函数注册为 DispatchMissionInfo
	INT LuaFnDispatchMissionInfo(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDispatchMissionDemandInfo
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				scriptId
	*					脚本编号
	*				misId
	*					任务编号
	*				done
	*					是否完成
	*     功能说明 :
	*				向客户端返回任务需求处理
	*				done如果为TRUE,客户端将激活任务界面的完成按钮
	*				之前需使用的函数依次为
	*				BeginEvent
	*				AddText,描述性字符串
	*				AddItemDemand,需求物品
	*				EndEvent
	*/
	该函数注册为 DispatchMissionDemandInfo
	INT LuaFnDispatchMissionDemandInfo(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDispatchMissionContinueInfo
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				scriptId
	*					脚本编号
	*				misId
	*					任务编号
	*     功能说明 :
	*				向客户端返回任务继续处理
	*/
	该函数注册为 DispatchMissionContinueInfo
	INT LuaFnDispatchMissionContinueInfo(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDispatchMissionTips
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				向客户端返回任务相关提示信息
	*				
	*/
	该函数注册为 DispatchMissionTips
	INT LuaFnDispatchMissionTips(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-28
	*     函数名称 : LuaFnDispatchMissionResult
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				向客户端返回任务验证结果
	*				
	*/
	该函数注册为 DispatchMissionResult
	INT LuaFnDispatchMissionResult(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnMissionCom
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*     功能说明 :
	*				任务完成
	*				之后需DelMission
	*/
	该函数注册为 MissionCom
	INT LuaFnMissionCom(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMission
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*     功能说明 :
	*				获得当前玩家某一任务号的值	//做过,正在做,未做
	*/
	该函数注册为 GetMission
	INT LuaFnGetMission(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMissionIndexByID
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*     功能说明 :
	*				设置当前玩家某一任务号的值
	*/
	该函数注册为 GetMissionIndexByID
	INT LuaFnGetMissionIndexByID(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetMissionParam
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misIndex
	*					任务位置索引
	*				paramIndex
	*					该任务字段编号
	*     功能说明 :
	*				 取任务参数
	*/
	该函数注册为 GetMissionParam
	INT LuaFnGetMissionParam(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnSetMissionByIndex
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*				paramIndex
	*					该任务字段编号
	*				value
	*					设paramIndex字段的值
	*     功能说明 :
	*				 设置任务参数
	*/
	该函数注册为 SetMissionByIndex
	INT LuaFnSetMissionByIndex(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2006-1-18
	*     函数名称 : LuaFnGetMissionCount
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				 得到已接任务的数量
	*				 没有任务时返回0
	*/
	该函数注册为 GetMissionCount
	INT LuaFnGetMissionCount(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddMission
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*				scriptId
	*					调用脚本编号
	*				killObjEvent
	*					是否回调OnKillObject
	*				enterAreaEvent
	*					是否回调OnEnterArea
	*				itemChangeEvent
	*					是否回调OnItemChange
	*     功能说明 :
	*				在玩家身上添加misId编号的任务
	*				成功返回1
	*/
	该函数注册为 AddMission
	INT LuaFnAddMission(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-13
	*     函数名称 : LuaFnAddMissionEx
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*				scriptId
	*					调用脚本编号
	*     功能说明 :
	*				在玩家身上添加misId编号的任务
	*				成功返回1
	*/
	该函数注册为 AddMissionEx
	INT LuaFnAddMissionEx(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-13
	*     函数名称 : LuaFnSetMissionEvent
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*				eventId
	*					调用脚本编号，0 KillObj, 1 EnterArea, 2 ItemChanged, 3 PetChanged
	*     功能说明 :
	*				修改玩家身上misId编号的任务相应事件
	*				成功返回1
	*/
	该函数注册为 SetMissionEvent
	INT LuaFnSetMissionEvent(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnDelMission
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*     功能说明 :
	*				删除任务
	*/
	该函数注册为 DelMission
	INT LuaFnDelMission(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnIsMissionHaveDone
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*     功能说明 :
	*				是否做过此任务
	*				返回值1为成功
	*				返回值0为失败
	*/
	该函数注册为 IsMissionHaveDone
	INT LuaFnIsMissionHaveDone(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : IsHaveMission
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				misId
	*					任务编号
	*     功能说明 :
	*				玩家是否正在做此任务
	*				返回值大于0为成功
	*				
	*/
	该函数注册为 IsHaveMission
	INT LuaFnIsHaveMission(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnBeginAddItem
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*				开始添加道具操作
	*				与EndAddItem配对使用
	*/
	该函数注册为 BeginAddItem
	该函数注册为 LuaFnBeginAddItem
	INT LuaFnBeginAddItem(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddItem
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				itemTypeSn
	*					ItemType的9位数号(实际为8位)	
	*				count
	*					添加数目
	*     功能说明 :
	*				在玩家物品栏中增加count个itemType编号的道具
	*/
	该函数注册为 AddItem
	该函数注册为 LuaFnAddItem
	INT LuaFnAddItem(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnEndAddItem
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				结束添加道具操作
	*				与BeginAddItem配对使用
	*				返回1表示有空间放置添加物品
	*/
	该函数注册为 EndAddItem
	该函数注册为 LuaFnEndAddItem
	INT LuaFnEndAddItem(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnAddItemListToHuman
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				把物品列表中的所有物品放置到玩家包中
	*				在调用BeginAddItem和EndAddItem之后使用
	*
	*	  备注:
	*				此函数会回调响应OnItemChanged事件的脚本
	*/
	该函数注册为 AddItemListToHuman
	该函数注册为 LuaFnAddItemListToHuman
	INT LuaFnAddItemListToHuman(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-11-16
	*     函数名称 : LuaFnGetMissionData
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				nIndex
	*					数据块索引
	*     功能说明 :
	*				得到Human全局数据块中的nIndex个数据值
	*				GetMissionData(L,18,12,3)
	*/
	该函数注册为 GetMissionData
	INT LuaFnGetMissionData(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-11-16
	*     函数名称 : LuaFnSetMissionData
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				nIndex
	*					数据块索引
	*				nData
	*					数据值
	*     功能说明 :
	*				设置Human的nIndex个数据块的值为nData
	*				在调用BeginAddItem和EndAddItem之后使用
	*/
	该函数注册为 SetMissionData
	INT LuaFnSetMissionData(Lua_State* L)


	/** ---------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-11-16
	*     函数名称 : LuaFnAddMonsterDropItem
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				MonsterID
	*					怪物编号
	*				HumanID
	*					角色编号
	*				ItemIndex
	*					掉落的物品类型
	*				
	*     功能说明 :
	*			在怪物身上添加一个掉落任务物品,并表明所有者
	*/
	该函数注册为 AddMonsterDropItem
	INT LuaFnAddMonsterDropItem(Lua_State* L)


	/** ---------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-11-16
	*     函数名称 : LuaFnGetMonsterOwnerCount
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				MonsterID
	*					怪物编号
	*				
	*     功能说明 :
	*			获取怪物所有者数量
	*/
	该函数注册为 GetMonsterOwnerCount
	INT LuaFnGetMonsterOwnerCount(Lua_State* L)


	/** ---------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-11-16
	*     函数名称 : LuaFnGetMonsterOwnerID
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				MonsterID
	*					怪物编号
	*				OwnerOffSet
	*					所有者偏移量
	*     功能说明 :
	*			获取怪物所有者编号
	*			返回所有者id
	*/
	该函数注册为 GetMonsterOwnerID
	INT LuaFnGetMonsterOwnerID(Lua_State* L)


	/** ---------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-2-8
	*     函数名称 : LuaFnBeginUICommand
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*			开始构造UICommand数据体
	*			无返回
	*/
	该函数注册为 BeginUICommand
	INT LuaFnBeginUICommand(Lua_State* L)


	/** ---------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-2-8
	*     函数名称 : LuaFnEndUICommand
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*			结束构造UICommand数据体
	*			无返回
	*/
	该函数注册为 EndUICommand
	INT LuaFnEndUICommand(Lua_State* L)


	/** ---------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-2-8
	*     函数名称 : LuaFnDispatchUICommand
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				nUIIndex
	*					窗口索引值
	*     功能说明 :
	*			结束构造UICommand数据体
	*			无返回
	*/
	该函数注册为 DispatchUICommand
	INT LuaFnDispatchUICommand(Lua_State* L)


	/** ---------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-2-8
	*     函数名称 : LuaFnUICommand_AddInt
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				nValue
	*					向数据体添加的数据,整数
	*     功能说明 :
	*			结束构造UICommand数据体
	*			无返回
	*/
	该函数注册为 UICommand_AddInt
	INT LuaFnUICommand_AddInt(Lua_State* L)


	/** ---------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-2-8
	*     函数名称 : LuaFnUICommand_AddString
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				szValue
	*					向数据体添加的数据,字符串
	*     功能说明 :
	*			结束构造UICommand数据体
	*			无返回
	*/
	该函数注册为 UICommand_AddString
	INT LuaFnUICommand_AddString(Lua_State* L)


	/** ---------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-2-10
	*     函数名称 : LuaFnGetQuestionsRecord
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				nIndex
	*					问题号，整数
	*     功能说明 :
	*			读取题库信息
	*			返回值：问题内容(字符串),
	*					选项0(字符串),
	*					选项1(字符串),
	*					选项2(字符串),
	*					选项3(字符串),
	*					选项4(字符串),
	*					选项5(字符串),
	*					答案0(整数),
	*					答案1(整数),
	*					答案2(整数),
	*					答案3(整数),
	*					答案4(整数),
	*					答案5(整数),
	*/
	该函数注册为 GetQuestionsRecord
	INT LuaFnGetQuestionsRecord(Lua_State* L)


	/** ---------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-4-19
	*     函数名称 : LuaFnSetMissionDataBit
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				nIndex
	*					MissionData的编号索引
	*				nBits
	*					偏移的位数（0－31）
	*				nValue
	*					数据, 0或非0
	*     功能说明 :
	*			无返回
	*/
	该函数注册为 SetMissionDataBit
	INT LuaFnSetMissionDataBit(Lua_State* L)


	/** ---------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-4-19
	*     函数名称 : LuaFnGetMissionDataBit
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				nIndex
	*					MissionData的编号索引
	*				nBits
	*					偏移的位数（0－31）
	*     功能说明 :
	*			0 或者 1
	*/
	该函数注册为 GetMissionDataBit
	INT LuaFnGetMissionDataBit(Lua_State* L)


	该函数注册为 MissionNotComplateInfoNM
	INT LuaFnMissionNotComplateInfoNM(Lua_State* L)


	该函数注册为 MissionComplateInfoNM
	INT LuaFnMissionComplateInfoNM(Lua_State* L)


	该函数注册为 IsHaveMissionNM
	INT LuaFnIsHaveMissionNM(Lua_State* L)


	该函数注册为 IsMissionHaveDoneNM
	INT LuaFnIsMissionHaveDoneNM(Lua_State* L)


	该函数注册为 MissionCheckAcceptNM
	INT LuaFnMissionCheckAcceptNM( Lua_State* L )


	该函数注册为 MissionCheckSubmitNM
	INT LuaFnMissionCheckSubmitNM( Lua_State* L )


	该函数注册为 AddMissionTextNM
	INT LuaFnAddMissionTextNM( Lua_State* L )


	该函数注册为 AddMissionNM
	INT LuaFnAddMissionNM( Lua_State* L )


	该函数注册为 MisMsg2PlayerNM
	INT LuaFnMisMsg2PlayerNM( Lua_State* L )


	该函数注册为 GetMonsterWayInfoNM
	INT LuaFnGetMonsterWayInfoNM( Lua_State* L )


	该函数注册为 ListMissionsNM
	INT LuaFnListMissionsNM( Lua_State* L )


	该函数注册为 RequestMissionNM
	INT LuaFnRequestMissionNM( Lua_State* L )


	该函数注册为 AddDispatchMissionInfoNM
	INT LuaFnAddDispatchMissionInfoNM(Lua_State* L)


	该函数注册为 OnMissionAcceptNM
	INT LuaFnOnMissionAcceptNM( Lua_State* L )


	该函数注册为 SubmitMissionNM
	INT LuaFnSubmitMissionNM( Lua_State* L )


	该函数注册为 MissionComplateNM
	INT LuaFnMissionComplateNM(Lua_State* L)


	该函数注册为 GetNeedItemNumNM
	INT LuaFnGetNeedItemNumNM(Lua_State* L)


	该函数注册为 GetMissionIndexByIDNM
	INT LuaFnGetMissionIndexByIDNM(Lua_State* L)	


	该函数注册为 DelMissionNM
	INT LuaFnDelMissionNM(Lua_State* L)


	该函数注册为 GetNeedKillObjNumNM
	INT LuaFnGetNeedKillObjNumNM(Lua_State* L)


	该函数注册为 IfFinishdKillObjNM
	INT LuaFnIfFinishdKillObjNM(Lua_State* L)


	该函数注册为 GetMissionTypeNM
	INT LuaFnGetMissionTypeNM(Lua_State* L)


	该函数注册为 GetMissionItemNM
	INT LuaFnGetMissionItemNM(Lua_State* L)


	该函数注册为 FinishKillObjGetItem
	INT LuaFnFinishKillObjGetItem(Lua_State* L)


脚本系统C导出函数文档	LuaFnTbl_Pet.h
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-021-20
	*     函数名称 : LuaFnPetStudySkill
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					完家ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*				skillId
	*					技能ID
	*     功能说明 :
	*				宠物学习技能
	*/
	该函数注册为 PetStudySkill
	INT LuaFnPetStudySkill(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-021-20
	*     函数名称 : LuaFnPetStudySkill
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					完家ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*				skillId
	*					技能ID
	*     功能说明 :
	*				宠物学习门派技能
	*/
	该函数注册为 PetStudySkill_MenPai
	INT LuaFnPetStudySkill_MenPai(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-021-20
	*     函数名称 : LuaFnCalcPetDomesticationMoney
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					宠物ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*     功能说明 :
	*				得到训养宠物所需费用
	*/
	该函数注册为 CalcPetDomesticationMoney
	INT LuaFnCalcPetDomesticationMoney(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-021-20
	*     函数名称 : LuaFnPetDomestication
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				humanId
	*					宠物ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*     功能说明 :
	*				训养宠物
	*/
	该函数注册为 PetDomestication
	INT LuaFnPetDomestication(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-03-17
	*     函数名称 : LuaFnGetPetHP
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					玩家ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*     功能说明 :
	*				设置宠物HP
	*/
	该函数注册为 LuaFnGetPetHP
	INT LuaFnGetPetHP(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-03-17
	*     函数名称 : LuaFnGetPetMaxHP
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					玩家ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*     功能说明 :
	*				设置宠物HP最大值
	*/
	该函数注册为 LuaFnGetPetMaxHP
	INT LuaFnGetPetMaxHP(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-03-17
	*     函数名称 : LuaFnGetPetLife
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					玩家ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*     功能说明 :
	*				设置宠物寿命
	*/
	该函数注册为 LuaFnGetPetLife
	INT LuaFnGetPetLife(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-03-17
	*     函数名称 : LuaFnGetPetHappiness
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					玩家ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*     功能说明 :
	*				设置宠物快乐度
	*/
	该函数注册为 LuaFnGetPetHappiness
	INT LuaFnGetPetHappiness(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-03-17
	*     函数名称 : LuaFnSetPetHP
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					玩家ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*				hp
	*					要设置的HP值	
	*     功能说明 :
	*				设置宠物HP
	*/
	该函数注册为 LuaFnSetPetHP
	INT LuaFnSetPetHP(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-03-17
	*     函数名称 : LuaFnSetPetMaxHP
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					玩家ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*				maxHP
	*					要设置的HP最大值	
	*     功能说明 :
	*				设置宠物HP最大值
	*/
	该函数注册为 LuaFnSetPetMaxHP
	INT LuaFnSetPetMaxHP(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-03-17
	*     函数名称 : LuaFnSetPetLife
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					玩家ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*				life
	*					要设置的寿命值	
	*     功能说明 :
	*				设置宠物寿命值	
	*/
	该函数注册为 LuaFnSetPetLife
	INT LuaFnSetPetLife(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-03-17
	*     函数名称 : LuaFnSetPetHappiness
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					玩家ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*				happiness
	*					要设置的快乐度
	*     功能说明 :
	*				设置宠物快乐度
	*/
	该函数注册为 LuaFnSetPetHappiness
	INT LuaFnSetPetHappiness(Lua_State* L)	


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-4-11	16:31
	 *	函数名称：	LuaFnPetCanUseFood
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				petGUID_H
	 *					宠物GUID高位
	 *				petGUID_L
	 *					宠物GUID低位
	 *				nIndex
	 *					物品背包位置
	 *
	 *	功能说明：	判断物品是否适合宠物食用
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnPetCanUseFood
	INT LuaFnPetCanUseFood(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-04-19
	*     函数名称 : LuaFnPetReturnToChild
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					玩家ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*     功能说明 :
	*				宠物还童Lua接口
	*/
	该函数注册为 LuaFnPetReturnToChild
	INT LuaFnPetReturnToChild(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-04-19
	*     函数名称 : LuaFnPetReturnToChild
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					玩家ID
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*     功能说明 :
	*				宠物还童Lua接口
	*/
	该函数注册为 LuaFnPetCanReturnToChild
	INT LuaFnPetCanReturnToChild(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-05-12
	*     函数名称 : LuaFnGetPetGUID
	*     参数说明 :
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					场景号
	*				selfId
	*					玩家ID
	*				petIndex
	*					宠物索引
	*     功能说明 :
	*				取得宠物的GUID
	*/
	该函数注册为 LuaFnGetPetGUID
	INT LuaFnGetPetGUID(Lua_State* L)	


脚本系统C导出函数文档	LuaFnTbl_PetPlacard.h
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-03-07
	*     函数名称 : LuaFnIssuePetPlacardByIndex
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*				pszMessage
	*					宠主的留言
	*				
	*     功能说明 :
	*				发布宠物公告
	*				
	*/
	该函数注册为 IssuePetPlacardByIndex
	INT	LuaFnIssuePetPlacardByIndex(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-03-07
	*     函数名称 : LuaFnIssuePetPlacard
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				petGUID_H
	*					宠物GUID高位
	*				petGUID_L
	*					宠物GUID低位
	*				pszMessage
	*					宠主的留言
	*				
	*     功能说明 :
	*				发布宠物公告
	*				
	*/
	该函数注册为 IssuePetPlacard
	INT	LuaFnIssuePetPlacard(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-03-07
	*     函数名称 : LuaFnIssuePetPlacard
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				
	*     功能说明 :
	*				取消宠物公告
	*				
	*/
	该函数注册为 CancelPetPlacard
	INT	LuaFnCancelPetPlacard(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-03-07
	*     函数名称 : LuaFnDispatchPetPlacardList
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				firstCheckId
	*					用于找到这次应该从哪一条信息开始往客气端刷新
	*					顺序=（上次刷新的倒数第一条ID）
	*					反序=（上次刷新的第一条ID）
	*				secondCheckId
	*					顺序=（上次刷新的倒数第二个ID)
	*					反序=（上次刷新的第二条ID）
	*				nextPageOrPrevPage
	*					!=0为顺序
	*				
	*     功能说明 :
	*				请求宠物的公告列表
	*				
	*/
	该函数注册为 DispatchPetPlacardList
	INT	LuaFnDispatchPetPlacardList(Lua_State* L) 


脚本系统C导出函数文档	LuaFnTbl_Relation.h
	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-7	16:13
	 *	函数名称：	LuaFnIsFriend
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				friendID
	 *					好友的 ID
	 *
	 *	功能说明：	判断 selfId 和 friendID 是否为好友
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnIsFriend
	INT LuaFnIsFriend(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-6	13:48
	 *	函数名称：	LuaFnGetFriendName
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				friendGUID
	 *					好友的 GUID
	 *
	 *	功能说明：	得到 GUID 为 friendGUID 的好友的名字
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnGetFriendName
	INT LuaFnGetFriendName(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-2-28	14:42
	 *	函数名称：	LuaFnGetFriendPoint
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				friendID
	 *					好友的 ID
	 *
	 *	功能说明：	得到和 ID 为 friendID 的好友的友好度
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnGetFriendPoint
	INT LuaFnGetFriendPoint(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-2-28	20:07
	 *	函数名称：	LuaFnSetFriendPoint
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				friendID
	 *					好友的 ID
	 *				friendPoint
	 *					友好度值
	 *
	 *	功能说明：	设置 selfId 与 friendID 的友好度为 friendPoint
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnSetFriendPoint
	INT LuaFnSetFriendPoint(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-6	12:14
	 *	函数名称：	LuaFnSetFriendPointByGUID
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				friendGUID
	 *					好友的 GUID
	 *				friendPoint
	 *					友好度值
	 *
	 *	功能说明：	设置 selfId 与 friendGUID 的友好度为 friendPoint
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnSetFriendPointByGUID
	INT LuaFnSetFriendPointByGUID(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-2-28	15:04
	 *	函数名称：	LuaFnIsMarried
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	判断调用者是否已婚
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnIsMarried
	INT LuaFnIsMarried(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-6	12:17
	 *	函数名称：	LuaFnGetSpouseGUID
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	得到配偶的 GUID
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnGetSpouseGUID
	INT LuaFnGetSpouseGUID(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-2-28	16:11
	 *	函数名称：	LuaFnMarry
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				spouseId
	 *					配偶 ID
	 *
	 *	功能说明：	sceneId 和 spouseId 结为夫妻
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnMarry
	INT LuaFnMarry(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-2-28	19:24
	 *	函数名称：	LuaFnIsSpouses
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				targetId
	 *					对方 ID
	 *
	 *	功能说明：	判断 selfId 和 targetId 是否是夫妻
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnIsSpouses
	INT LuaFnIsSpouses(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-6	11:26
	 *	函数名称：	LuaFnUnMarry
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				spouseId
	 *					配偶 ID
	 *
	 *	功能说明：	sceneId 和 spouseId 离婚
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnUnMarry
	INT LuaFnUnMarry(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-6	13:11
	 *	函数名称：	LuaFnDivorce
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	单方面离婚
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnDivorce
	INT LuaFnDivorce(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-6	17:07
	 *	函数名称：	LuaFnIsMaster
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				targetId
	 *					对方 ID
	 *
	 *	功能说明：	判断 targetId 是否是 selfId 的师傅
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnIsMaster
	INT LuaFnIsMaster(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-7	10:03
	 *	函数名称：	LuaFnIsPrentice
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				targetId
	 *					对方 ID
	 *
	 *	功能说明：	判断 targetId 是否是 selfId 的徒弟
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnIsPrentice
	INT LuaFnIsPrentice(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-10	20:17
	 *	函数名称：	LuaFnGetPrenticeGUID
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				nIndex
	 *					第几个徒弟
	 *
	 *	功能说明：	?
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnGetPrenticeGUID
	INT LuaFnGetPrenticeGUID(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-7	10:04
	 *	函数名称：	LuaFnHaveMaster
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	判断 selfId 是否有师傅
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnHaveMaster
	INT LuaFnHaveMaster(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-10	19:57
	 *	函数名称：	LuaFnGetMasterGUID
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	得到师傅的 GUID
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnGetMasterGUID
	INT LuaFnGetMasterGUID(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-7	19:21
	 *	函数名称：	LuaFnGetMasterMoralPoint
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	取得 selfId 的师德点
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnGetMasterMoralPoint
	INT LuaFnGetMasterMoralPoint(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-7	19:22
	 *	函数名称：	LuaFnSetMasterMoralPoint
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				uMoralPoint
	 *					师德点
	 *
	 *	功能说明：	设置师德点
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnSetMasterMoralPoint
	INT LuaFnSetMasterMoralPoint(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-7	10:07
	 *	函数名称：	LuaFnGetPrenticeCount
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	得要徒弟数量
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnGetPrenticeCount
	INT LuaFnGetPrenticeCount(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-7	10:10
	 *	函数名称：	LuaFnGetPrenticeBetrayTime
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	得到最后一次徒弟叛师的时间
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnGetPrenticeBetrayTime
	INT LuaFnGetPrenticeBetrayTime(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-7	10:11
	 *	函数名称：	LuaFnAprentice
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				targetId
	 *					对方 ID
	 *
	 *	功能说明：	selfId 拜 targetId 为师
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnAprentice
	INT LuaFnAprentice(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-7	10:18
	 *	函数名称：	LuaFnFinishAprentice
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				targetId
	 *					对方 ID
	 *
	 *	功能说明：	selfId 离开 targetId 师们，正式出师
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnFinishAprentice
	INT LuaFnFinishAprentice(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-10	19:47
	 *	函数名称：	LuaFnBetrayMaster
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	叛师
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnBetrayMaster
	INT LuaFnBetrayMaster(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-10	19:47
	 *	函数名称：	LuaFnExpelPrentice
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				PrenticeGUID
	 *					徒弟的 GUID
	 *
	 *	功能说明：	开除某个徒弟
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnExpelPrentice
	INT LuaFnExpelPrentice(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-7	10:40
	 *	函数名称：	LuaFnIsBrother
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				targetId
	 *					对方 ID
	 *
	 *	功能说明：	判断 targetId 是否是 selfId 的结拜兄弟
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnIsBrother
	INT LuaFnIsBrother(Lua_State* L)


脚本系统C导出函数文档	LuaFnTbl_Scene.h
/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnGetSceneType
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*				 返回当前场景的类型，详见：Config/SceneInfo.ini 里Type字段描述
	*/
	该函数注册为 LuaFnGetSceneType
	INT LuaFnGetSceneType(Lua_State* L)	


/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnCreateCopyScene
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*				 返回值：如果返回-1，表示创建失败，否则返回创建成功的场景号
	*				 根据已经设置好的数据创建副本, 可以选择的设置参数：
	*				 见“*SetSceneLoad_*”或“*SetCopySceneData_*”类型的函数
	*/
	该函数注册为 LuaFnCreateCopyScene
	INT LuaFnCreateCopyScene(Lua_State* L)	


/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnSetSceneLoad_Map
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 mapName
	*					导航图文件名,字符串类型
	*     功能说明 :
	*				 设置副本创建选项里的导航图文件，类似"*.nav", 不需要带路径
	*				 注意：在创建副本时候，必须设置地图文件名，而且，此地图已经
	*					   配置的SceneInfo.ini里
	*/
	该函数注册为 LuaFnSetSceneLoad_Map
	INT LuaFnSetSceneLoad_Map(Lua_State* L)	


/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnSetSceneLoad_Monster
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 monsterName
	*					怪物分布文件名,字符串类型
	*     功能说明 :
	*				 设置副本创建选项里的怪物分布文件,类似"*_monster.ini",
	*				 不需要带路径
	*/
	该函数注册为 LuaFnSetSceneLoad_Monster
	INT LuaFnSetSceneLoad_Monster(Lua_State* L)	


/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnSetSceneLoad_Platform
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 platformName
	*					操作台分布文件名,字符串类型
	*     功能说明 :
	*				 设置副本创建选项里的操作台分布文件,类似"*_platform.ini",
	*				 不需要带路径
	*/
	该函数注册为 LuaFnSetSceneLoad_Platform
	INT LuaFnSetSceneLoad_Platform(Lua_State* L)	


/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnSetSceneLoad_GrowPointData
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 growpointdataName
	*					生长点分布文件名,字符串类型
	*     功能说明 :
	*				 设置副本创建选项里的生长点分布文件,类似"*_growpoint.ini",
	*				 不需要带路径
	*/
	该函数注册为 LuaFnSetSceneLoad_GrowPointData
	INT LuaFnSetSceneLoad_GrowPointData(Lua_State* L)	


/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnSetSceneLoad_GrowPointSetup
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 growpointsetupName
	*					从生长点里能生成的物品分布文件名,字符串类型
	*     功能说明 :
	*				 设置副本创建选项里的从生长点里能生成的物品分布文件,类似"*_growpointsetup.ini",
	*				 不需要带路径
	*/
	该函数注册为 LuaFnSetSceneLoad_GrowPointSetup
	INT LuaFnSetSceneLoad_GrowPointSetup(Lua_State* L)	


/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnSetSceneLoad_Area
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 areaName
	*					事件区分布文件名,字符串类型
	*     功能说明 :
	*				 设置副本创建选项里的事件区分布文件,类似"*_area.ini",
	*				 不需要带路径
	*/
	该函数注册为 LuaFnSetSceneLoad_Area
	INT LuaFnSetSceneLoad_Area(Lua_State* L)	


/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnSetSceneLoad_Pet
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 petName
	*					宠物分布文件名,字符串类型
	*     功能说明 :
	*				 设置副本创建选项里的宠物分布文件,类似"*_pet.ini",
	*				 不需要带路径
	*/
	该函数注册为 LuaFnSetSceneLoad_Pet
	INT LuaFnSetSceneLoad_Pet(Lua_State* L)	


/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnSetSceneLoad_PatrolPointData
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 patrolpointdataName
	*					巡逻路径分布文件名,字符串类型
	*     功能说明 :
	*				 设置副本创建选项里的巡逻路径分布文件,类似"*_patrolpoint.ini",
	*				 不需要带路径
	*/
	该函数注册为 LuaFnSetSceneLoad_PatrolPointData
	INT LuaFnSetSceneLoad_PatrolPointData(Lua_State* L)	


/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnSetCopySceneData_TeamLeader
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 guidLeader
	*					队长的GUID,uint
	*     功能说明 :
	*				 设置副本创建数据里的队长GUID
	*/
	该函数注册为 LuaFnSetCopySceneData_TeamLeader
	INT LuaFnSetCopySceneData_TeamLeader(Lua_State* L)	


/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnSetCopySceneData_NoUserCloseTime
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 closeTime
	*					时间（毫秒）,uint
	*     功能说明 :
	*				 设置副本创建数据里的：没有人后副本场景自动关闭清除的时间。
	*/
	该函数注册为 LuaFnSetCopySceneData_NoUserCloseTime
	INT LuaFnSetCopySceneData_NoUserCloseTime(Lua_State* L)	


/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnSetCopySceneData_Timer
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 uTime
	*					时间（毫秒）,uint
	*     功能说明 :
	*				 设置副本创建数据里的：场景定时器的时间间隔，如果为0表示没有设置。
	*/
	该函数注册为 LuaFnSetCopySceneData_Timer
	INT LuaFnSetCopySceneData_Timer(Lua_State* L)	


/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnSetCopySceneData_Param
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 uIndex
	*					索引,uint 最大可以到32
	*				 nValue
	*					值,uint
	*     功能说明 :
	*				 设置副本创建数据里的：场景参数信息
	*/
	该函数注册为 LuaFnSetCopySceneData_Param
	INT LuaFnSetCopySceneData_Param(Lua_State* L)	


/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnGetCopySceneData_TeamLeader
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*				 返回副本创建数据里的队长GUID，失败－1 
	*/
	该函数注册为 LuaFnGetCopySceneData_TeamLeader
	INT LuaFnGetCopySceneData_TeamLeader(Lua_State* L)	


/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnGetCopySceneData_NoUserCloseTime
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*				 返回没有人后副本场景自动关闭清除的时间（毫秒）。
	*/
	该函数注册为 LuaFnGetCopySceneData_NoUserCloseTime
	INT LuaFnGetCopySceneData_NoUserCloseTime(Lua_State* L)	


/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnGetCopySceneData_Timer
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*				 读取场景定时器的时间间隔，如果为0表示没有设置。
	*/
	该函数注册为 LuaFnGetCopySceneData_Timer
	INT LuaFnGetCopySceneData_Timer(Lua_State* L)	


/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-16
	*     函数名称 : LuaFnGetCopySceneData_Param
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 uIndex
	*					索引,uint 最大可以到32
	*     功能说明 :
	*				 读取场景参数信息
	*/
	该函数注册为 LuaFnGetCopySceneData_Param
	INT LuaFnGetCopySceneData_Param(Lua_State* L)	


/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-18
	*     函数名称 : LuaFnGetCopyScene_HumanCount
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*				 返回当前场景里面的玩家数量
	*/
	该函数注册为 LuaFnGetCopyScene_HumanCount
	INT LuaFnGetCopyScene_HumanCount(Lua_State* L)	


/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-18
	*     函数名称 : LuaFnGetCopyScene_HumanObjId
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 uIndex
	*					索引,uint 最大可以到32
	*     功能说明 :
	*				 读取场景参数信息
	*/
	该函数注册为 LuaFnGetCopyScene_HumanObjId
	INT LuaFnGetCopyScene_HumanObjId(Lua_State* L)	


/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-24
	*     函数名称 : LuaFnIsObjValid
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 objId
	*					objId 号
	*     功能说明 :
	*				 判断是否拥有objId号的Obj
	*/
	该函数注册为 LuaFnIsObjValid
	INT LuaFnIsObjValid(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-11-24
	*     函数名称 : LuaFnIsCharacterLiving
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 objId
	*					objId 号
	*     功能说明 :
	*				 判断objId号的Obj是否是活的
	*/
	该函数注册为 LuaFnIsCharacterLiving
	INT LuaFnIsCharacterLiving(Lua_State* L)	


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-2-28	10:28
	 *	函数名称：	LuaFnSendNormalMail
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				objId
	 *					调用者 ID，无符号整型
	 *				szName
	 *					收件人姓名
	 *				szMail
	 *					邮件内容
	 *
	 *	功能说明：	发送普通邮件
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnSendNormalMail
	INT LuaFnSendNormalMail(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-2-28	13:30
	 *	函数名称：	LuaFnSendScriptMail
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				szName
	 *					收件人姓名
	 *				uParam0 ~ uParam3
	 *					可执行邮件自带的 4 个参数
	 *
	 *	功能说明：	发送可执行邮件
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnSendScriptMail
	INT LuaFnSendScriptMail(Lua_State* L)


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-3-4	17:46
	 *	函数名称：	LuaFnSendMailToAllFriend
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *				szMail
	 *					邮件内容
	 *
	 *	功能说明：	给所有好友发送普通邮件
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 LuaFnSendMailToAllFriend
	INT LuaFnSendMailToAllFriend(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-23
	*     函数名称 : LuaFnGetMonsterCount
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*     功能说明 :
	*				 取得此场景中的怪物数量
	*/
	该函数注册为 GetMonsterCount
	INT LuaFnGetMonsterCount(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-23
	*     函数名称 : LuaFnGetMonsterObjID
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 nIndex
	*					怪物索引号
	*     功能说明 :
	*				 根据索引号取得怪物的ObjID
	*/
	该函数注册为 GetMonsterObjID
	INT LuaFnGetMonsterObjID(Lua_State* L)	


/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-05-15
	*     函数名称 : LuaFnIsCanEnterCopyScene
	*     参数说明 : 
	*				 L
	*					LuaState执行环境指针
	*				 sceneId
	*					调用该函数场景ID,short
	*				 guid
	*					玩家GUID
	*     功能说明 :
	*				 判断玩家是否可以进入场景sceneId, 
	*					返回1 表示能够进，返回0表示不能进, -1表示出错
	*/
	该函数注册为 IsCanEnterCopyScene
	INT LuaFnIsCanEnterCopyScene(Lua_State* L)	


脚本系统C导出函数文档	LuaFnTbl_Shop.h
	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-10-12
	*     函数名称 : LuaFnDispatchShopItem
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				ShopId
	*					商店编号
	*				
	*/
	该函数注册为 DispatchShopItem
	INT	LuaFnDispatchShopItem(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-2-23
	*     函数名称 : LuaFnApplyPlayerShop
	*     功能说明 : 通知客户端弹开申请开店
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				
	*/
	该函数注册为 ApplyPlayerShop
	INT	LuaFnApplyPlayerShop(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-2-23
	*     函数名称 : LuaFnDispatchPlayerShopList
	*     功能说明 : 发送此场景中的商店列表
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				
	*/
	该函数注册为 DispatchPlayerShopList
	INT	LuaFnDispatchPlayerShopList(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-3-18
	*     函数名称 : LuaFnDispatchPlayerShopSaleOutList
	*     功能说明 : 发送此场景中的盘出的商店列表
	*     参数说明 : 
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				
	*/
	该函数注册为 DispatchPlayerShopSaleOutList
	INT	LuaFnDispatchPlayerShopSaleOutList(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-20
	*     函数名称 : LuaFnDispatchXinfaInfo
	*     参数说明 : 
	*     功能说明 : 门派心法导师使用
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				MenpaiId
	*					这个老师的ID（门派ID）
	*/
	该函数注册为 DispatchXinfaLevelInfo
	INT LuaFnDispatchXinfaLevelInfo(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-20
	*     函数名称 : LuaFnDispatchMenpaiInfo
	*     参数说明 : 
	*     功能说明 : 门派导师使用
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				MenpaiId
	*					这个老师的ID（门派ID）
	*/				
	该函数注册为 DispatchMenpaiInfo
	INT LuaFnDispatchMenpaiInfo(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-10-20
	*     函数名称 : LuaFnDispatchAbilityInfo
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				AbilityId
	*					这个老师所教的生活技能的ID
	*/
	该函数注册为 DispatchAbilityInfo
	INT LuaFnDispatchAbilityInfo(Lua_State* L) 


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-01-09
	*     函数名称 : LuaFnJoinMenpai
	*     参数说明 : 
	*     功能说明 :
	*				SceneID
	*					场景编号
	*				selfId
	*					调用者ID,无符号整型
	*				targetId
	*					Npc编号
	*				AbilityId
	*					这个老师所教的生活技能的ID
	*/
	该函数注册为 LuaFnJoinMenpai
	INT LuaFnJoinMenpai(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-4-26
	*     函数名称 : LuaFnGetShopName
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				iShopType
	*					商店类型
	*     功能说明 :
	*				获得指定人身上的玩家商店名
	*/
	该函数注册为 LuaFnGetShopName
	INT LuaFnGetShopName(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-4-26
	*     函数名称 : LuaFnOpenPlayerShop
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				iShopType
	*					商店类型
	*     功能说明 :
	*				获得指定人身上的玩家商店名
	*/
	该函数注册为 LuaFnOpenPlayerShop
	INT LuaFnOpenPlayerShop(Lua_State* L)	


脚本系统C导出函数文档	LuaFnTbl_Team.h
	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetTeamId
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型			
	*     功能说明 :
	*				返回玩家队伍ID
	*				没有测试
	*/
	该函数注册为 GetTeamId
	INT LuaFnGetTeamId(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnGetTeamSize
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				返回玩家所在队伍人数
	*				没有测试
	*/
	该函数注册为 GetTeamSize
	该函数注册为 LuaFnGetTeamSize
	INT LuaFnGetTeamSize(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnLeaveTeam
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*     功能说明 :
	*				让玩家离开自身队伍
	*				没有测试
	*/
	该函数注册为 LeaveTeam
	INT LuaFnLeaveTeam(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnMsg2Player
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				msg
	*					聊天内容,字符串指针类型
	*				type
	*					CHAT_TYPE_NORMAL =0 ,	//普通说话消息
	*					CHAT_TYPE_TEAM = 1,		//队聊消息
	*					CHAT_TYPE_SCENE = 2,		//场景消息
	*					CHAT_TYPE_TELL = 3,		//私聊消息
	*					CHAT_TYPE_SYSTEM = 4,		//系统消息
	*					CHAT_TYPE_CHANNEL = 5,		//自建聊天频道消息
	*					CHAT_TYPE_GUILD = 6,		//帮派消息
	*					CHAT_TYPE_MENPAI = 7 ,		//门派消息
	*					CHAT_TYPE_SELF = 8,			//仅客户端使用的消息
	*					
	*     功能说明 :
	*				将消息发送给玩家
	*				消息显示在左下角的聊天框内
	*				type默认值为CHAT_TYPE_NORMAL,即0
	*
	*				金钱提示信息，需要写成#{_MONEY%d}方式
	*				例如：你获得了#{_MONEY208934}。 
	*				这句话在客户端提示的时候会自动计算出金、银、铜的数量，并用对应的图标表示出来。
	*				
	*/
	该函数注册为 Msg2Player
	该函数注册为 LuaFnMsg2Player
	INT LuaFnMsg2Player(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 :
	*     编写时间 : 2005-9-28
	*     函数名称 : LuaFnMsg2Team
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				msg
	*					聊天内容,字符串指针类型
	*     功能说明 :
	*				将消息发送给玩家所在组的所有成员
	*				函数体尚未填写
	*/
	该函数注册为 Msg2Team
	INT LuaFnMsg2Team(Lua_State* L)	


	/****************************************************************************
	 *	开发人员：	
	 *	编写时间：	2006-1-17	17:53
	 *	函数名称：	LuaFnIsTeamFollow
	 *	参数说明：
	 *				sceneId
	 *					场景编号
	 *				selfId
	 *					调用者 ID，无符号整型
	 *
	 *	功能说明：	得到 selfId 对应的玩家是否处于组队跟随状态
	 *	修改记录：
	*****************************************************************************/
	该函数注册为 IsTeamFollow
	INT LuaFnIsTeamFollow(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-12-14
	*     函数名称 : LuaFnGetFollowedMembersCount
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型			
	*     功能说明 :
	*				得到跟随状态的队友数量
	*				没有测试
	*/
	该函数注册为 GetFollowedMembersCount
	该函数注册为 LuaFnGetFollowedMembersCount
	INT LuaFnGetFollowedMembersCount(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     函数名称 : LuaFnGetFollowedMember
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型
	*				memberIndex
	*					在跟随队伍中的下标索引,从0起始,整型			

	*     功能说明 :
	*				得到跟随状态的memberIndex下标的队友objId
	*				没有测试
	*/
	该函数注册为 GetFollowedMember
	INT LuaFnGetFollowedMember(Lua_State* L)	


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-12-14
	*     函数名称 : LuaFnStopTeamFollow
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型			
	*     功能说明 :
	*				停止跟随,针对selfId标志的玩家
	*				没有测试
	*/
	该函数注册为 StopTeamFollow
	INT LuaFnStopTeamFollow(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-12-14
	*     函数名称 : LuaFnClearFollowedMembers
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型			
	*     功能说明 :
	*				清除所有的跟随队友列表
	*				没有测试
	*/
	该函数注册为 ClearFollowedMembers
	INT LuaFnClearFollowedMembers(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-12-14
	*     函数名称 : LuaFnGetTeamSceneMemberCount
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型			
	*     功能说明 :
	*				队伍中同场景成员数量 human
	*				没有测试
	*/
	该函数注册为 GetTeamSceneMemberCount
	该函数注册为 LuaFnGetTeamSceneMemberCount
	INT LuaFnGetTeamSceneMemberCount(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-12-14
	*     函数名称 : LuaFnGetTeamSceneMember
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型	
	*				memberIndex
	*					在队伍中的下标索引,从0起始,整型			
	*     功能说明 :
	*				取得队伍中某同场景成员的 OBJID
	*				没有测试
	*/
	该函数注册为 LuaFnGetTeamSceneMember
	INT LuaFnGetTeamSceneMember(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-12-14
	*     函数名称 : LuaFnSetTeamFollowSpeed
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型			
	*     功能说明 :
	*				设置组队跟随的速度
	*				没有测试
	*/
	该函数注册为 SetTeamFollowSpeed
	INT LuaFnSetTeamFollowSpeed(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-12-14
	*     函数名称 : LuaFnIsTeamLeader
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型			
	*     功能说明 :
	*				selfId玩家是否为队长
	*				返回1,是
	*				返回0,否
	*				没有测试
	*/
	该函数注册为 LuaFnIsTeamLeader
	INT LuaFnIsTeamLeader(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2005-12-14
	*     函数名称 : LuaFnGetTeamMemberGuid
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型	
	*				memberIndex
	*					在队伍中的下标索引,从0起始,整型			
	*     功能说明 :
	*				获得selfId所在Team中第memberIndex个下标玩家的Guid
	*				没有测试
	*/
	该函数注册为 GetTeamMemberGuid
	INT LuaFnGetTeamMemberGuid(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-1-10
	*     函数名称 : LuaFnHasTeam
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型	
	*     功能说明 :
	*				判断selfId玩家是否在组队状态
	*				没有测试
	*/
	该函数注册为 LuaFnHasTeam
	INT LuaFnHasTeam(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-17
	*     函数名称 : LuaFnGetNearTeamCount
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型	
	*     功能说明 :
	*				取得selfId玩家附近的队友数量(包括自己)
	*				如果玩家不属于某个队伍，则返回-1
	*/
	该函数注册为 GetNearTeamCount
	INT LuaFnGetNearTeamCount(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-1-10
	*     函数名称 : LuaFnGetNearTeamMember
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					调用者ID,无符号整型	
	*				index
	*					附近队友的索引号
	*     功能说明 :
	*				取得selfId玩家附近队友的objid(包括自己)
	*/
	该函数注册为 GetNearTeamMember
	INT LuaFnGetNearTeamMember(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-33
	*     函数名称 : LuaFnGetMonsterGroupID
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					需要取得数据的怪物的objid
	*     功能说明 :
	*				取得selfId怪物的GroupID, monster.ini里面配置
	*/
	该函数注册为 GetMonsterGroupID
	INT LuaFnGetMonsterGroupID(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-33
	*     函数名称 : LuaFnGetMonsterTeamID
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					需要取得数据的怪物的objid
	*     功能说明 :
	*				取得selfId怪物的TeamID, monster.ini里面配置
	*/
	该函数注册为 GetMonsterTeamID
	INT LuaFnGetMonsterTeamID(Lua_State* L)


	/** -----------------------------------------------------------------------
	*     开发人员 : 
	*     编写时间 : 2006-3-23
	*     函数名称 : LuaFnGetMonsterDataID
	*     参数说明 : 
	*				L
	*					LuaState执行环境指针
	*				sceneId
	*					调用该函数场景ID,short
	*				selfId
	*					需要取得数据的怪物的objid
	*     功能说明 :
	*				取得selfId怪物的DataID, monster.ini里面配置
	*/
	该函数注册为 GetMonsterDataID
	INT LuaFnGetMonsterDataID(Lua_State* L)


脚本系统C导出函数文档	LuaFnRegister.cpp
	/**/
	/**/
文档生成日期:2007年9月16日12时