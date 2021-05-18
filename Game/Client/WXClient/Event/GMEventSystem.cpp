#include "StdAfx.h"
#include "GMEventSystem.h"
#include "TDException.h"
#include "../Procedure/GameProcedure.h"
#include "TDTimeSystem.h"

//==============================================================
EVENT_DEFINE g_GlobalEvent[] = 
{
	{ GE_APPLICATION_INITED,		"APPLICATION_INITED",		},
	{ GE_ON_SCENE_TRANS,			"ON_SCENE_TRANS",			},
	{ GE_ON_SERVER_TRANS,			"ON_SERVER_TRANS",			},
	{ GE_ON_SCENE_UPDATED,			"ON_SCENE_UPDATED",			},
	{ GE_SCENE_TRANSED,				"SCENE_TRANSED",			},
	{ GE_PLAYER_ENTER_WORLD,		"PLAYER_ENTERING_WORLD",	},
	{ GE_PLAYER_LEAVE_WORLD,		"PLAYER_LEAVE_WORLD",		},
	{ GE_TEAM_CHANG_WORLD,			"TEAM_CHANGE_WORLD",			},	// 队友改变场景了
	{ GE_SKILL_CHANGED,				"SKILL_CHANGED",			},
	{ GE_MAINTARGET_CHANGED,		"MAINTARGET_CHANGED",		},
	{ GE_MAINTARGET_OPEND,			"MAINTARGET_OPEN",			},		//打开主窗口
	{ GE_PACKAGE_ITEM_CHANGED,		"PACKAGE_ITEM_CHANGED",		},
	{ GE_TOGLE_SKILL_BOOK,			"TOGLE_SKILL_BOOK",			},
	{ GE_CLOSE_SKILL_BOOK,			"CLOSE_SKILL_BOOK",			},
	{ GE_SKILL_UPDATE,				"SKILL_UPDATE",				},
	{ GE_TOGLE_PET_PAGE,			"TOGLE_PET_PAGE",			},
	{ GE_TOGLE_LIFE_PAGE,			"TOGLE_LIFE_PAGE",			},
	{ GE_TOGLE_COMMONSKILL_PAGE,	"TOGLE_COMMONSKILL_PAGE"	},
	{ GE_TOGLE_CONTAINER,			"TOGLE_CONTAINER",			},
//---------------------------------------------------------------------------	
	//------------------------------
	//角色基本属性
	{ GE_UNIT_HP,					"UNIT_HP",					  FALSE},
	{ GE_UNIT_MP,					"UNIT_MP",					  FALSE},
	{ GE_UNIT_RAGE,					"UNIT_RAGE",				  TRUE},
	{ GE_UNIT_XINFA,				"UNIT_XINFA",				  TRUE},
	{ GE_UNIT_EXP,					"UNIT_EXP",					  FALSE},
	{ GE_UNIT_MONEY,				"UNIT_MONEY",				  TRUE},
	{ GE_UNIT_RACE_ID,				"UNIT_RACE_ID",				  TRUE},
	{ GE_UNIT_NAME,					"UNIT_NAME",				  TRUE},
	{ GE_UNIT_CAMP_ID,				"UNIT_CAMP_ID",				  TRUE},
	{ GE_UNIT_LEVEL,				"UNIT_LEVEL",				  FALSE},
	{ GE_UNIT_MOVE_SPEED,			"UNIT_MOVE_SPEED",			  TRUE},
	{ GE_UNIT_FIGHT_STATE,			"UNIT_FIGHT_STATE",			  TRUE},
	{ GE_UNIT_MAX_EXP,				"UNIT_MAX_EXP",				  FALSE},
	//------------------------------"
	//一级战斗属性					"
	{ GE_UNIT_STR,					"UNIT_STR",					  TRUE},
	{ GE_UNIT_SPR,					"UNIT_SPR",					  TRUE},
	{ GE_UNIT_CON,					"UNIT_CON",					  TRUE},
	{ GE_UNIT_INT,					"UNIT_INT",					  TRUE},
	{ GE_UNIT_DEX,					"UNIT_DEX",					  TRUE},
	{ GE_UNIT_POINT_REMAIN,			"UNIT_POINT_REMAIN",		  TRUE},
	//------------------------------"
	//二级战斗属性					"
	{ GE_UNIT_ATT_PHYSICS,			"UNIT_ATT_PHYSICS",			  TRUE},
	{ GE_UNIT_ATT_MAGIC,			"UNIT_ATT_MAGIC",			  TRUE},
	{ GE_UNIT_DEF_PHYSICS,			"UNIT_DEF_PHYSICS",			  TRUE},
	{ GE_UNIT_DEF_MAGIC,			"UNIT_DEF_MAGIC",			  TRUE},
	{ GE_UNIT_MAX_HP,				"UNIT_MAX_HP",				  TRUE},
	{ GE_UNIT_MAX_MP,				"UNIT_MAX_MP",				  TRUE},
	{ GE_UNIT_HP_RE_SPEED,			"UNIT_HP_RE_SPEED",			  TRUE},
	{ GE_UNIT_MP_RE_SPEED,			"UNIT_MP_RE_SPEED",			  TRUE},
	{ GE_UNIT_HIT,					"UNIT_HIT",					  TRUE},
	{ GE_UNIT_MISS,					"UNIT_MISS",				  TRUE},
	{ GE_UNIT_CRIT_RATE,			"UNIT_CRIT_RATE",			  TRUE},
	{ GE_UNIT_ATT_SPEED,			"UNIT_ATT_SPEED",			  TRUE},
	{ GE_UNIT_ATT_COLD,				"UNIT_ATT_COLD",			  TRUE},
	{ GE_UNIT_DEF_COLD,				"UNIT_DEF_COLD",			  TRUE},
	{ GE_UNIT_ATT_FIRE,				"UNIT_ATT_FIRE",			  TRUE},
	{ GE_UNIT_DEF_FIRE,				"UNIT_DEF_FIRE",			  TRUE},
	{ GE_UNIT_ATT_LIGHT,			"UNIT_ATT_LIGHT",			  TRUE},
	{ GE_UNIT_DEF_LIGHT,			"UNIT_DEF_LIGHT",			  TRUE},
	{ GE_UNIT_ATT_POSION,			"UNIT_ATT_POSION",			  TRUE},
	{ GE_UNIT_DEF_POSION,			"UNIT_DEF_POSION",			  TRUE},
	{ GE_UNIT_MENPAI,				"UNIT_MENPAI",				  TRUE},
	{ GE_UNIT_HAIR_MESH,			"UNIT_HAIR_MESH",			  TRUE},
	{ GE_UNIT_HAIR_COLOR,			"UNIT_HAIR_COLOR",			  TRUE},
	{ GE_UNIT_FACE_IMAGE,			"UNIT_FACE_IMAGE",			  TRUE},// 头像信息改变 2006-3-9
	{ GE_UNIT_EQUIP_VER,			"UNIT_EQUIP_VER",			  TRUE},
	{ GE_UNIT_EQUIP,				"UNIT_EQUIP",				  TRUE},
	{ GE_UNIT_EQUIP_WEAPON,			"UNIT_EQUIP_WEAPON",		  TRUE},
	{ GE_UNIT_TITLE,				"UNIT_TITLE",				  TRUE},
	{ GE_UNIT_STRIKEPOINT,			"UNIT_STRIKEPOINT",			  TRUE},
	{ GE_UNIT_RELATIVE,				"UNIT_RELATIVE",			  TRUE},
	{ GE_UNIT_RMB,					"UNIT_RMB",					  TRUE},
	//{ GE_UNIT_MAX_VIGOR,			"UNIT_MAX_VIGOR",			  TRUE},
	//{ GE_UNIT_ENERGY,				"UNIT_ENERGY",				  TRUE},
	//{ GE_UNIT_MAX_ENERGY,			"UNIT_MAX_ENERGY",			  TRUE},
//
//---------------------------------------------------------------------------
	{ GE_SHOW_CONTEXMENU,			"SHOW_CONTEXMENU",			},
	{ GE_TOGLE_COMPOSE_WINDOW,		"TOGLE_COMPOSE_WINDOW",		},
	{ GE_TOGLE_CONSOLE,				"TOGLE_CONSOLE",			},
	{ GE_ON_SKILL_ACTIVE,			"ON_SKILL_ACTIVE",			},
	{ GE_POSITION_MODIFY,			"POSITION_MODIFY",			},
	{ GE_CHAT_MESSAGE,				"CHAT_MESSAGE",				},
	{ GE_CHAT_CHANGE_PRIVATENAME,	"CHAT_CHANGE_PRIVATENAME",	},
	{ GE_CHAT_CHANNEL_CHANGED,		"CHAT_CHANNEL_CHANGED",		},

	{ GE_CHAT_TAB_CREATE,			"CHAT_TAB_CREATE",			},
	{ GE_CHAT_TAB_CONFIG,			"CHAT_TAB_CONFIG",			},
	{ GE_CHAT_TAB_CREATE_FINISH,	"CHAT_TAB_CREATE_FINISH",	},
	{ GE_CHAT_TAB_CONFIG_FINISH,	"CHAT_TAB_CONFIG_FINISH",	},
	{ GE_CHAT_FACEMOTION_SELECT,	"CHAT_FACEMOTION_SELECT",	},
	{ GE_CHAT_TEXTCOLOR_SELECT,		"CHAT_TEXTCOLOR_SELECT",	},
	{ GE_CHAT_CONTEX_MENU,			"CHAT_CONTEX_MENU",			},
	{ GE_CHAT_ACTSET,				"CHAT_ACTSET",				},
	{ GE_CHAT_ACT_SELECT,			"CHAT_ACT_SELECT",			},
	{ GE_CHAT_ACT_HIDE,				"CHAT_ACT_HIDE",			},
	{ GE_CHAT_PINGBI_LIST,			"CHAT_PINGBI_LIST",			},
	{ GE_CHAT_PINGBI_UPDATE,		"CHAT_PINGBI_UPDATE",		},
	{ GE_ACCELERATE_KEYSEND,		"ACCELERATE_KEYSEND",		},
	{ GE_CHAT_ADJUST_MOVE_CTL,		"CHAT_ADJUST_MOVE_CTL",		},
	{ GE_CHAT_INPUTLANGUAGE_CHANGE,	"CHAT_INPUTLANGUAGE_CHANGE",	TRUE},
	{ GE_CHAT_SHOWUSERINFO,			"CHAT_SHOWUSERINFO",		},
	{ GE_CHAT_LOAD_TAB_CONFIG,		"CHAT_LOAD_TAB_CONFIG",		},

	{ GE_LOOT_OPENED,				"LOOT_OPENED",				},
	{ GE_LOOT_SLOT_CLEARED,			"LOOT_SLOT_CLEARED",		},
	{ GE_LOOT_CLOSED,				"LOOT_CLOSED",				},
	{ GE_PROGRESSBAR_SHOW,			"PROGRESSBAR_SHOW",			},
	{ GE_PROGRESSBAR_HIDE,			"PROGRESSBAR_HIDE",			},
	{ GE_PROGRESSBAR_WIDTH,			"PROGRESSBAR_WIDTH",		},
	{ GE_CHANGE_BAR,				"CHANGE_BAR",				},
	{ GE_TOGLE_MISSION,				"TOGLE_MISSION",			},
	{ GE_UPDATE_MISSION,			"UPDATE_MISSION",			},
	{ GE_REPLY_MISSION,				"REPLY_MISSION",			},
	{ GE_UPDATE_REPLY_MISSION,		"UPDATE_REPLY_MISSION",		},
	{ GE_TOGLE_LARGEMAP,			"TOGLE_LARGEMAP",			},
	{ GE_TOGLE_SCENEMAP,			"TOGLE_SCENE_MAP",			},
	{ GE_OPEN_MINIMAP,				"OPEN_MINIMAP",				},
	{ GE_OPEN_MINIMAPEXP,			"OPEN_MINIMAPEXP",			},

	{ GE_QUEST_EVENTLIST,			"QUEST_EVENTLIST",			},
	{ GE_QUEST_INFO,				"QUEST_INFO",				},
	{ GE_QUEST_REGIE,				"QUEST_REGIE",				},
	{ GE_QUEST_CONTINUE_DONE,		"QUEST_CONTINUE_DONE",		},
	{ GE_QUEST_CONTINUE_NOTDONE,	"QUEST_CONTINUE_NOTDONE",	},
	{ GE_QUEST_AFTER_CONTINUE,		"QUEST_AFTER_CONTINUE",		},
	{ GE_QUEST_TIPS,				"QUEST_TIPS",				},
	{ GE_TOGLE_COMPOSEITEM,			"TOGLE_COMPOSEITEM",		},
	{ GE_TOGLE_COMPOSEGEM,			"TOGLE_COMPOSEGEM",			},
	{ GE_ON_LIFEABILITY_ACTIVE,		"ON_LIFEABILITY_ACTIVE",	},
	{ GE_NEW_DEBUGMESSAGE,			"NEW_DEBUGMESSAGE",			},
	{ GE_OPEN_BOOTH,				"OPEN_BOOTH",				},
	{ GE_CLOSE_BOOTH,				"CLOSE_BOOTH",				},

	//------------------------------------------------------------------------------------------------------------
	// 装备属性界面消息
	{ GE_UPDATE_EQUIP,				"REFRESH_EQUIP",				},	// 更新装备, 无参数.
	{ GE_MANUAL_ATTR_SUCCESS_EQUIP,	"MANUAL_ATTR_SUCCESS_EQUIP",	},	// 手动调节属性成功.
	{ GE_CUR_TITLE_CHANGEED,		"CUR_TITLE_CHANGED",			},	// 当前标题改变了.


	{ GE_UPDATE_BOOTH,				"UPDATE_BOOTH",				},
	{ GE_UPDATE_TEAM_MEMBER,		"ON_UPDATE_TEAM_MEMBER",	},

	{ GE_OPEN_CHARACTOR,			"OPEN_CHARACTOR",			},
	{ GE_OPEN_EQUIP,				"OPEN_EQUIP",				},
	{ GE_TOGLE_JOINSCHOOL,			"TOGLE_JOINSCHOOL"			},
	{ GE_UPDATE_CONTAINER,			"UPDATE_CONTAINER",			},
	{ GE_IMPACT_SELF_UPDATE,		"IMPACT_SELF_UPDATE",		},
	{ GE_IMPACT_SELF_UPDATE_TIME,	"IMPACT_SELF_UPDATE_TIME",	},
	{ GE_OPEN_AGNAME,				"OPEN_AGNAME",				},
	{ GE_CLOSE_AGNAME,				"CLOSE_AGNAME",				},
	{ GE_TOGLE_BANK,				"TOGLE_BANK",				},
	{ GE_UPDATE_BANK,				"UPDATE_BANK",				},
	{ GE_TOGLE_INPUT_MONEY,			"TOGLE_INPUT_MONEY",		},
	{ GE_CLOSE_INPUT_MONEY,			"CLOSE_INPUT_MONEY",		},

	{ GE_RECEIVE_EXCHANGE_APPLY,	"RECEIVE_EXCHANGE_APPLY",	},
	{ GE_STOP_EXCHANGE_TIP,			"STOP_EXCHANGE_TIP",		},
	{ GE_OPEN_EXCHANGE_FRAME,		"OPEN_EXCHANGE_FRAME",		},
	{ GE_UPDATE_EXCHANGE,			"UPDATE_EXCHANGE",			},

	{ GE_CANCEL_EXCHANGE,			"CANCEL_EXCHANGE",			},
	{ GE_SUCCEED_EXCHANGE_CLOSE,	"SUCCEED_EXCHANGE_CLOSE",	},
	{ GE_UPDATE_PET_PAGE,			"UPDATE_PET_PAGE",			},
	{ GE_UPDATE_LIFESKILL_PAGE,		"UPDATE_LIFESKILL_PAGE",	},
	{ GE_OPEN_COMPOSE_ITEM,			"OPEN_COMPOSE_ITEM",		},
	{ GE_UPDATE_COMPOSE_ITEM,		"UPDATE_COMPOSE_ITEM",		},
	{ GE_OPEN_COMPOSE_GEM,			"OPEN_COMPOSE_GEM",			},
	{ GE_UPDATE_COMPOSE_GEM,		"UPDATE_COMPOSE_GEM",		},
	{ GE_AFFIRM_SHOW,				"AFFIRM_SHOW",				},

	// 摆摊相关
	{ GE_OPEN_STALL_SALE,			"OPEN_STALL_SALE",			},
	{ GE_OPEN_STALL_BUY,			"OPEN_STALL_BUY",			},
	{ GE_UPDATE_STALL_BUY,			"UPDATE_STALL_BUY",			},
	{ GE_UPDATE_STALL_SALE,			"UPDATE_STALL_SALE",		},
	{ GE_OPEN_STALL_RENT_FRAME,		"OPEN_STALL_RENT_FRAME",	},
	{ GE_STALL_SALE_SELECT,			"STALL_SALE_SELECT",		},
	{ GE_STALL_BUY_SELECT,			"STALL_BUY_SELECT",			},
	{ GE_OPEN_STALL_MESSAGE,		"OPEN_STALL_MESSAGE",		},
	{ GE_CLOSE_STALL_MESSAGE,		"CLOSE_STALL_MESSAGE",		},

	// 销毁物品
	{ GE_OPEN_DISCARD_ITEM_FRAME,	"OPEN_DISCARD_ITEM_FRAME",	},
	// 不能销毁物品
	{ GE_OPEN_CANNT_DISCARD_ITEM,	"OPEN_CANNT_DISCARD_ITEM",	},

	{ GE_SHOW_SPLIT_ITEM,			"SHOW_SPLIT_ITEM",			},
	{ GE_HIDE_SPLIT_ITEM,			"HIDE_SPLIT_ITEM",			},
	{ GE_TOGLE_FRIEND_INFO,			"TOGLE_FRIEND_INFO",		},
	{ GE_TOGLE_FRIEND,				"TOGLE_FRIEND",				},
	{ GE_OPEN_EMAIL,				"OPEN_EMAIL",				},
	{ GE_OPEN_EMAIL_WRITE,			"OPEN_EMAIL_WRITE",			},
	{ GE_HAVE_MAIL,					"HAVE_MAIL",				},
	{ GE_SEND_MAIL,					"SEND_MAIL",				},
	{ GE_CHAT_OPEN,					"CHAT_OPEN",				},
	{ GE_CHAT_CLOSED,				"CHAT_CLOSED",				},
	{ GE_UPDATE_EMAIL,				"UPDATE_EMAIL",				},
	{ GE_MOOD_SET,					"MOOD_SET",					},
	{ GE_MOOD_CHANGE,				"MOOD_CHANGE",				},
	{ GE_OPEN_HISTROY,				"OPEN_HISTROY",				},

	//系统设置相关
	{ GE_TOGLE_SYSTEMFRAME,			"TOGLE_SYSTEMFRAME",		},
	{ GE_TOGLE_VIEWSETUP,			"TOGLE_VIEWSETUP",			},
	{ GE_TOGLE_SOUNDSETUP,			"TOGLE_SOUNDSETUP",			},
	{ GE_TOGLE_UISETUP,				"TOGLE_UISETUP",			},
	{ GE_TOGLE_INPUTSETUP,			"TOGLE_INPUTSETUP",			},
	{ GE_TOGLE_GAMESETUP,			"TOGLE_GAMESETUP",			},

	//玩家商店
	{ GE_PS_OPEN_OTHER_SHOP,		"PS_OPEN_OTHER_SHOP",		},
	{ GE_PS_OPEN_MY_SHOP,			"PS_OPEN_MY_SHOP",			},
	{ GE_PS_OPEN_CREATESHOP,		"PS_OPEN_CREATESHOP",		},
	{ GE_PS_CLOSE_CREATESHOP,		"PS_CLOSE_CREATESHOP",		},
	{ GE_PS_OPEN_SHOPLIST,			"PS_OPEN_SHOPLIST",			},
	{ GE_PS_SELF_ITEM_CHANGED,		"PS_SELF_ITEM_CHANGED",		},
	{ GE_PS_OTHER_SELECT,			"PS_OTHER_SELECT",			},
	{ GE_PS_SELF_SELECT,			"PS_SELF_SELECT",			},

	{ GE_PS_UPDATE_MY_SHOP,			"PS_UPDATE_MY_SHOP",		},
	{ GE_PS_UPDATE_OTHER_SHOP,		"PS_UPDATE_OTHER_SHOP",		},
									
	{ GE_PS_OPEN_OTHER_TRANS,		"PS_OPEN_OTHER_TRANS",		},
	{ GE_PS_UPDATE_OTHER_TRANS,		"PS_UPDATE_OTHER_TRANS",	},
	{ GE_PS_OTHER_TRANS_SELECT,		"PS_OTHER_TRANS_SELECT",	},

	{ GE_OPEN_PS_MESSAGE_FRAME,		"OPEN_PS_MESSAGE_FRAME",	},

	{ GE_PS_OPEN_MESSAGE,			"PS_OPEN_MESSAGE",			},
	{ GE_PS_UPDATE_MESSAGE,			"PS_UPDATE_MESSAGE",		},

	{ GE_OPEN_PET_LIST,				"OPEN_PET_LIST",			},
	{ GE_VIEW_EXCHANGE_PET,			"VIEW_EXCHANGE_PET",		},
	{ GE_CLOSE_PET_LIST,			"CLOSE_PET_LIST",			},
	{ GE_UPDATE_PET_LIST,			"UPDATE_PET_LIST",			},

	{ GE_OPEN_PRIVATE_INFO,			"OPEN_PRIVATE_INFO",		},

	{ GE_OPEN_CALLOF_PLAYER,		"OPEN_CALLOF_PLAYER",		},
	{ GE_NET_CLOSE,					"NET_CLOSE",				},

	{ GE_OPEN_ITEM_COFFER,			"OPEN_ITEM_COFFER",			},
	{ GE_PS_INPUT_MONEY,			"PS_INPUT_MONEY",			},
	//------------------------------------------------------------------------------------------------------------
	//
	// 组队相关事件
	
	{ GE_TEAM_OPEN_TEAMINFO_DLG,	"TEAM_OPEN_TEAMINFO_DLG",	},		// 打开队伍信息对话框.
																		// 
																		// arg0 - 正整数, 从0开始
																		// 0 : 打开队伍信息
																		// 1 : 有人申请加入队伍
																		// 2 : 有人邀请你加入队伍

	{ GE_TEAM_NOTIFY_APPLY,			"TEAM_NOTIFY_APPLY",		},		// mainmenubar, 闪烁按钮. 
																		// arg0 - 正整数, 从0开始
																		// 0 : 表示已经加入队伍, 不需要闪烁按钮, 打开界面为队伍信息
																		// 1 : 有人申请加入队伍
																		// 2 : 有人邀请你加入队伍
																		// 申请的人的具体信息从申请的信息队列中获得.

	{ GE_TEAM_APPLY,				"TEAM_APPLY",				},		// 有人申请你加入队伍.
																		// 
																		// arg0 - 字符串
																		// 申请的人.

	{ GE_TEAM_INVITE,				"TEAM_INVITE",				},		// 有人邀请你加入队伍.
																		// 
																		// arg0 - 字符串
																		// 邀请你的人.
	
	{ GE_TEAM_CLEAR_UI,				"TEAM_HIDE_ALL_PLAYER",		},		// 清空界面
										
	
	{ GE_TEAM_REFRESH_UI,			"TEAM_REFRESH_DATA",		},		// 刷新界面
	
	
	{ GE_TEAM_MEMBER_ENTER,			"TEAM_ENTER_MEMBER",		},		// 有新的队员进入
																		//
																		// arg0 - 正整数, 从1 开始
																		// 在ui界面中的显示位置
	
	{ GE_TEAM_UPTEDATA_MEMBER_INFO,	"TEAM_UPDATE_MEMBER",		},		// 更新队员信息
																		// arg0 - 正整数, 从1 开始
																		// 在ui界面中的显示位置
	
	{ GE_TEAM_MESSAGE,				"TEAM_TEAM_MESSAGE",		},		// 队伍信息提示信息
																		// arg0 - 字符串
																		// 需要提示的信息.
	
	{ GE_TEAM_MEMBER_INVITE,		"TEAM_MEMBER_INVITE",      },		// 队员邀请某一个人加入队伍请求队长同意
																		//
																		// arg0 - 队员名字
																		// arg1 - 被邀请人的名字

	{ GE_TEAM_FOLLOW_INVITE,		"TEAM_FOLLOW_INVITE",		},		// 队长邀请队员进入组队跟随模式
																		//
																		// arg0 - 队长名字

	{ GE_TEAM_REFRESH_MEMBER,		"TEAM_REFRESH_MEMBER",		},		// 刷新队员信息
																		//
																		// arg0 - 队员的位置索引

    /*****************************************************************************************************************************
	**
	** 以下以后需要删除.
	**
	**
	******************************************************************************************************************************/
	//{ GE_ON_TEAM_ENTER_MEMBER,		"TEAM_ENTER_MEMBER",		},// 有新的队员进入
	{ GE_UPDATE_TEAM_MEMBER,		"ON_UPDATE_TEAM_MEMBER",	},// 更新队员信息
	{ GE_SHOW_TEAM_YES_NO,				"SHOW_TEAM_YES_NO",		},// 有人邀请你加入队伍.
	{ GE_SHOW_TEAM_MEMBER_INFO,			"SHOW_PLAYER_INFO",		},// 显示队员的详细信息
	{ GE_SHOW_TEAM_MEMBER_NAME,			"SHOW_PLAYER_NAME",		},// 在左边的队友列表框中显示队友的名字
	{ GE_HIDE_ALL_PLAYER,				"HIDE_ALL_PLAYER",		},// 自己离开队伍后, 清空左边的队伍列表.
	{ GE_SHOW_TEAM_MEMBER_NICK,			"SHOW_TEAM_NICK",		},// 昵称
	{ GE_SHOW_TEAM_MEMBER_FAMILY,		"SHOW_TEAM_FAMILY",		},// 门派
	{ GE_SHOW_TEAM_MEMBER_LEVEL,		"SHOW_TEAM_LEVEL",		},// 等级
	{ GE_SHOW_TEAM_MEMBER_POS,			"SHOW_TEAM_POS",		},// 位置
	{ GE_SHOW_TEAM_MEMBER_HP,			"SHOW_TEAM_HP",			},// hp
	{ GE_SHOW_TEAM_MEMBER_MP,			"SHOW_TEAM_MP",			},// mp
	{ GE_SHOW_TEAM_MEMBER_ANGER,		"SHOW_TEAM_ANGER",		},// 怒气
	{ GE_SHOW_TEAM_MEMBER_DEAD_LINK,	"SHOW_TEAM_DEAD_LINK",	},// 连线信息
	{ GE_SHOW_TEAM_MEMBER_DEAD,			"SHOW_TEAM_DEAD",		},// 死亡信息.
	{ GE_SHOW_TEAM_FUNC_MENU_MEMBER,	"SHOW_TEAM_CONTEXMENU_MEMBER", },// 显示队员的功能菜单
	{ GE_SHOW_TEAM_FUNC_MENU_LEADER,	"SHOW_TEAM_CONTEXMENU_LEADER", },// 显示队长的功能菜单

	// 组队相关事件
	//---------------------------------------------------------------------------------------------------------------------------------------


	//------------------------------------------------------------------------------------------------------------------------
	//
	// 登录流程相关
	//
	
	{ GE_GAMELOGIN_SHOW_SYSTEM_INFO,		"GAMELOGIN_SHOW_SYSTEM_INFO",	},		// 显示系统信息
																					//
																					// arg0 - 字符串 : 需要提示的系统信息.


	{ GE_GAMELOGIN_SHOW_SYSTEM_INFO_CLOSE_NET,		"GAMELOGIN_SHOW_SYSTEM_INFO_CLOSE_NET",	},		// 显示系统信息
																									//
																									// arg0 - 字符串 : 需要提示的系统信息.
																									//
																									// 点击确认按钮后断开网络。


	{ GE_GAMELOGIN_CLOSE_SYSTEM_INFO,		"GAMELOGIN_CLOSE_SYSTEM_INFO",	},		// 显示队长的功能菜单

	
	{ GE_GAMELOGIN_OPEN_SELECT_SERVER,		"GAMELOGIN_OPEN_SELECT_SERVER",	},		// 关闭选择服务器界面
																					

	{ GE_GAMELOGIN_CLOSE_SELECT_SERVER,		"GAMELOGIN_CLOSE_SELECT_SERVER",},		// 关闭选择服务器界面


	{ GE_GAMELOGIN_OPEN_COUNT_INPUT,		"GAMELOGIN_OPEN_COUNT_INPUT",	},		// 打开帐号输入界面
																					

	{ GE_GAMELOGIN_CLOSE_COUNT_INPUT,		"GAMELOGIN_CLOSE_COUNT_INPUT",},		// 关闭帐号输入界面

	{ GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON,		"GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON",	},	// 显示系统信息, 不显示按钮
																								//
																								// arg0 - 字符串 : 需要提示的系统信息.

	{ GE_GAMELOGIN_OPEN_SELECT_CHARACTOR,		"GAMELOGIN_OPEN_SELECT_CHARACTOR",	},			// 显示人物选择界面

	{ GE_GAMELOGIN_CLOSE_SELECT_CHARACTOR,		"GAMELOGIN_CLOSE_SELECT_CHARACTOR",	},			// 关闭人物选择界面


	{ GE_GAMELOGIN_OPEN_CREATE_CHARACTOR,		"GAMELOGIN_OPEN_CREATE_CHARACTOR", },			// 显示人物创建界面

	{ GE_GAMELOGIN_CLOSE_CREATE_CHARACTOR,		"GAMELOGIN_CLOSE_CREATE_CHARACTOR",},			// 关闭人物关闭界面

	{ GE_GAMELOGIN_REFRESH_ROLE_SELECT_CHARACTOR,"GAMELOGIN_REFRESH_ROLE_SELECT_CHARACTOR",},	// 刷新人物信息


	{ GE_GAMELOGIN_CLOSE_BACK_GROUND,			"GAMELOGIN_CLOSE_BACK_GROUND",},				// 关闭背景界面


	{ GE_GAMELOGIN_SYSTEM_INFO_YESNO,			"GAMELOGIN_SYSTEM_INFO_YESNO",},				// 系统信息提示 yes_no 界面.
																								//
																								// 参数0,	提示的字符串
																								// 参数1,	对话框的类型
																								//			0 -- 是否退出游戏
																								//			1 -- 是否删除角色
																								//			2 -- 是否更换帐号



	{ GE_GAMELOGIN_SELECT_LOGIN_SERVER,			"GAMELOGIN_SELECT_LOGIN_SERVER",},				// 选择一个login server
																								//
																								// 参数0, iAreaIndex   区域索引
																								// 参数1, iLoginServer 区域索引

	{ GE_GAMELOGIN_CLEAR_ACCOUNT,				"GAMELOGIN_CLEAR_ACCOUNT"},						// 清空帐号显示.

	{ GE_GAMELOGIN_SELECT_AREA,					"GAMELOGIN_SELECT_AREA"},						// 选择区域
												
	{ GE_GAMELOGIN_SELECT_LOGINSERVER,			"GAMELOGIN_SELECT_LOGINSERVER"},				// 选择Login Server

	{ GE_GAMELOGIN_CREATE_CLEAR_NAME,			"GAMELOGIN_CREATE_CLEAR_NAME"},				// 清空创建角色名字。

	{ GE_GAMELOGIN_CREATE_ROLE_OK,				"GAMELOGIN_CREATE_ROLE_OK"},				// 创建角色成功。2006－4－17

	
												

	
	//
	// 登录流程相关
	//
	//------------------------------------------------------------------------------------------------------------------------


	{ GE_TOGLE_SKILLSTUDY,				"TOGLE_SKILLSTUDY",		},		// 门派技能学习界面
	{ GE_SKILLSTUDY_SUCCEED,			"SKILLSTUDY_SUCCEED",	},		// 门派技能学习界面
	{ GE_TOGLE_ABILITY_STUDY,			"TOGLE_ABILITY_STUDY"	},		// 生活技能学习界面


	{ GE_SUPERTOOLTIP,					"SHOW_SUPERTOOLTIP" }, //显示超级tooltip
	{ GE_UPDATE_SUPERTOOLTIP,			"UPDATE_SUPERTOOLTIP" }, //刷新超级tooltip


	//
	// 复活界面相关
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{ GE_RELIVE_SHOW,					"RELIVE_SHOW",			},		// 显示复活界面
	{ GE_RELIVE_HIDE,					"RELIVE_HIDE",			},		// 隐藏复活界面
	{ GE_RELIVE_REFESH_TIME,			"RELIVE_REFESH_TIME",	},		// 刷新灵魂出窍的时间

	{ GE_OBJECT_CARED_EVENT,			"OBJECT_CARED_EVENT",	},

	{ GE_UPDATE_MAP,					"UPDATE_MAP",	},

	{ GE_UI_COMMAND,					"UI_COMMAND",	},
	{ GE_OTHERPLAYER_UPDATE_EQUIP,		"OTHERPLAYER_UPDATE_EQUIP", },
	{ GE_VARIABLE_CHANGED,				"VARIABLE_CHANGED" , TRUE }, //全局变量发生改变
	{ GE_TIME_UPDATE,					"TIME_UPDATE", },

	{ GE_UPDATE_FRIEND,					"UPDATE_FRIEND",	},	// 好友数据更新了
	{ GE_UPDATE_FRIEND_INFO,			"UPDATE_FRIEND_INFO", },
	

	{ GE_UPDATE_TARGETPET_PAGE,			"UPDATE_TARGETPET_PAGE", },
	{ GE_UPDATE_PETSKILLSTUDY,			"UPDATE_PETSKILLSTUDY", },
	{ GE_UPDATE_PETINVITEFRIEND,		"UPDATE_PETINVITEFRIEND", },
	{ GE_REPLY_MISSION_PET,				"REPLY_MISSION_PET", },
	
	//------------------------------------------------------------------------------------------------------------------------
	//
	// 是否设置二级保护密码
	//

	{ GE_MINORPASSWORD_OPEN_SET_PASSWORD_DLG,		"MINORPASSWORD_OPEN_SET_PASSWORD_DLG", },		// 打开设置二级密码界面
	{ GE_MINORPASSWORD_OPEN_UNLOCK_PASSWORD_DLG,	"MINORPASSWORD_OPEN_UNLOCK_PASSWORD_DLG", },	// 打开unlock密码界面。
	{ GE_MINORPASSWORD_OPEN_CHANGE_PASSWORD_DLG,	"MINORPASSWORD_OPEN_CHANGE_PASSWORD_DLG", },	// 打开更改密码界面。

	{ GE_OPEN_SYSTEM_TIP_INFO_DLG,	"OPEN_SYSTEM_TIP_INFO_DLG", },									// 提示系统信息对话框。

	{ GE_GUILD_CREATE,					"GUILD_CREATE", },
	{ GE_GUILD_CREATE_CONFIRM,			"GUILD_CREATE_CONFIRM", },
	{ GE_GUILD_SHOW_LIST,				"GUILD_SHOW_LIST", },
	{ GE_GUILD_SHOW_MEMBERINFO,			"GUILD_SHOW_MEMBERINFO", },
	{ GE_GUILD_SHOW_DETAILINFO,			"GUILD_SHOW_DETAILINFO", },
	{ GE_GUILD_SHOW_APPOINTPOS,			"GUILD_SHOW_APPOINTPOS", },
	{ GE_GUILD_DESTORY_CONFIRM,			"GUILD_DESTORY_CONFIRM", },
	{ GE_GUILD_QUIT_CONFIRM,			"GUILD_QUIT_CONFIRM", },
	{ GE_GUILD_UPDATE_MEMBERINFO,		"GUILD_UPDATE_MEMBERINFO", },
	{ GE_GUILD_FORCE_CLOSE,				"GUILD_FORCE_CLOSE", },

	{ GE_CLOSE_MISSION_REPLY,			"CLOSE_MISSION_REPLY", },
	{ GE_CLOSE_TARGET_EQUIP,			"CLOSE_TARGET_EQUIP",},
	{ GE_VIEW_RESOLUTION_CHANGED,		"VIEW_RESOLUTION_CHANGED", },
	{ GE_CLOSE_SYNTHESIZE_ENCHASE,		"CLOSE_SYNTHESIZE_ENCHASE", },
	{ GE_TOGGLE_PETLIST,				"TOGGLE_PETLIST", },
	{ GE_PET_FREE_CONFIRM,				"PET_FREE_CONFIRM", },

	{ GE_TOGLE_CONTAINER1,				"TOGLE_CONTAINER1",			},//fujia 2007.10.24
	{ GE_TOGLE_CONTAINER2,				"TOGLE_CONTAINER2",			},
	{ GE_NEW_GONGGAOMESSAGE,			"NEW_GONGGAOMESSAGE",		},
};

//==============================================================
WX_IMPLEMENT_DYNAMIC(CEventSystem, GETCLASS(tEventSystem));
CEventSystem* CEventSystem::s_pMe = NULL;
CEventSystem::CEventSystem()
{
	s_pMe = this;
	m_dwLastTickCount = 0;
}

CEventSystem::~CEventSystem()
{
}

VOID CEventSystem::Initial(VOID*)
{
	INT nEventNum = sizeof(g_GlobalEvent) / sizeof(EVENT_DEFINE);

	for(INT i=0; i<nEventNum; i++)
	{
		//生成名称检索表
		m_mapEventIndex_AsName[g_GlobalEvent[i].szEvent] = &(g_GlobalEvent[i]);
		//生成id检索表
		m_mapEventIndex_AsID[g_GlobalEvent[i].idEvent] = &(g_GlobalEvent[i]);
	}
}

VOID CEventSystem::Release(VOID)
{
}

VOID CEventSystem::Tick(VOID)
{
}

void CEventSystem::_PushEvent(const EVENT& event)
{
	if(!event.pEventDef) return;

	//if(event.pEventDef->idEvent == GE_GAMELOGIN_SHOW_SYSTEM_INFO_NO_BUTTON)
	//{
	//	__asm int 3;
	//}

	//如果是慢速处理的事件
	if(event.pEventDef->delayProcess)
	{
		m_delayQueueEvent.push_back(event);
	}
	else
	{
		m_queueEvent.push_back(event);
	}
}

VOID CEventSystem::PushEvent(GAME_EVENT_ID id, const std::vector< STRING >& vParam)
{
	if(m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsID[id];

	event.vArg = vParam;

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(GAME_EVENT_ID id)
{
	if(m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsID[id];

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(GAME_EVENT_ID id, FLOAT fArg0)
{
	if(m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsID[id]; 

	CHAR szTemp[32];
	_snprintf(szTemp, 32, "%f", fArg0);
	event.vArg.push_back(szTemp);

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1)
{
	if(m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsID[id]; 

	event.vArg.push_back(szArg0);
	event.vArg.push_back(szArg1);

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(GAME_EVENT_ID id, INT iArg0)
{
	if(m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsID[id]; 

	CHAR szTemp[32];
	_snprintf(szTemp, 32, "%d", iArg0);
	event.vArg.push_back(szTemp);

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0)
{
	if(m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsID[id]; 

	event.vArg.push_back(szArg0);

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1, INT nArg2 )
{
	if(m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsID[id]; 

	event.vArg.push_back(szArg0);
	event.vArg.push_back(szArg1);
	CHAR szTemp[32];
	_snprintf(szTemp, 32, "%d", nArg2);
	event.vArg.push_back( szTemp );

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(GAME_EVENT_ID id, INT iArg0, INT iArg1)
{
	if(m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsID[id]; 

	CHAR szTemp[32];

	_snprintf(szTemp, 32, "%d", iArg0);
	event.vArg.push_back(szTemp);
	_snprintf(szTemp, 32, "%d", iArg1);
	event.vArg.push_back(szTemp);

	_PushEvent(event);
}

VOID CEventSystem::PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1, INT iArg2, INT iArg3)
{
	if(m_mapEventIndex_AsID.find(id) == m_mapEventIndex_AsID.end()) return;

	EVENT event;
	event.pEventDef=m_mapEventIndex_AsID[id]; 

	event.vArg.push_back(szArg0);
	event.vArg.push_back(szArg1);

	CHAR szTemp[32];
	_snprintf(szTemp, 32, "%d", iArg2);
	event.vArg.push_back(szTemp);
	_snprintf(szTemp, 32, "%d", iArg3);
	event.vArg.push_back(szTemp);

	_PushEvent(event);
}

//注册事件处理函数
VOID CEventSystem::RegisterEventHandle(const STRING& nameEvent, FUNC_EVENT_HANDLE funHandle, UINT uOwnerData)
{
	if(!funHandle) return;

	EVENT_DEFINE* pEvent = m_mapEventIndex_AsName[nameEvent];
	if(!pEvent) return;

	pEvent->listFuncNotify.push_back( std::make_pair(funHandle, uOwnerData) );
}

VOID CEventSystem::ProcessAllEvent(VOID)
{
	//处理慢速队列
	if(!(m_delayQueueEvent.empty()))
	{
		const UINT WORK_STEP = 2;
		UINT nTickCountNow = CGameProcedure::s_pTimeSystem->GetTickCount();
		UINT nTickCountStep = CGameProcedure::s_pTimeSystem->CalSubTime(m_dwLastTickCount, nTickCountNow);
		if(nTickCountStep >= WORK_STEP) 
		{
			m_dwLastTickCount = nTickCountNow;

			const EVENT& event = *(m_delayQueueEvent.begin());

			_ProcessEvent(event);

			m_delayQueueEvent.erase(m_delayQueueEvent.begin());
		}
	}

	register std::list< EVENT >::iterator it;
	for(it=m_queueEvent.begin(); it!=m_queueEvent.end(); it++)
	{
		const EVENT& event = *it;

		//检测是否有同样的Event已经被处理
		bool bMultiPushed = false;
		for(register std::list< EVENT >::iterator itPrev=m_queueEvent.begin(); itPrev !=it; itPrev++)
		{
			if(*itPrev == *it) 
			{
				bMultiPushed = true;
				break;
			}
		}

		if(bMultiPushed) continue;

		_ProcessEvent(event);
	}
	m_queueEvent.clear();
}

void CEventSystem::_ProcessEvent(const EVENT& event)
{
	//查找事件定义
	EVENT_DEFINE* pEventDef = event.pEventDef;
	if(!pEventDef) return;

	//调用处理函数
	if(!(pEventDef->listFuncNotify.empty()))
	{
		EVENT_DEFINE::REGISTER_STRUCT::iterator it;
		for(it=pEventDef->listFuncNotify.begin(); it!=pEventDef->listFuncNotify.end(); it++)
		{
			if((*it).first) ((*it).first)(&event, (*it).second);
		}
	}
}
