/********************************************************************************
*	文件名：	GameDefine_City.h
*
*	功能说明：	城市的定义
*	修改记录：
*********************************************************************************/

#ifndef __GAMEDEFINE_CITY_H__
#define __GAMEDEFINE_CITY_H__


#define	MAX_CITY_PER_WORLD	255
#define MAX_CITY_PORT		12
#define MAX_CITY_BUILDING	2048
#define MAX_CITY_PORT_GROUP	54

#define	MAX_CITY_PORT_NUM		1024

//城市的最大级别
#define MAX_CITY_LEVEL	5

//城市名字长度
#define MAX_CITY_NAME_SIZE_DB 26

//城市名字长度
#define MAX_CITY_NAME_SIZE 24

//城市中建筑物的最大数量
#define MAX_CITY_BUILDING_NUM 32

//城市中建筑物的最大级别
#define MAX_BUILDING_LEVEL	 5

//研究时可能对应的不同级别建筑
#define MAX_BUILDING_LEVEL_RESEARCH	 5

//配方数
#define MAX_RESEARCH_ITEM_PER_BUILDING	 100

//商店一次刷新可以卖得商品总数
#define MAX_ITEM_PER_SHOP	 5

//一个城市最多可建立的商业路线(友好城市)
#define MAX_FRIEND_GUILD	 8

enum CITY_ERROR_TYPE
{
	CITY_ERROR_NOTHING				= 0,	// 没什么错误……
	CITY_ERROR_GUID_ON_WORKING,				//当前工会正在进行高危操作，不能进行城市操作
};

enum CITY_RATE_TYPE
{
	IND_RATE = 0,			//工业率0
	AGR_RATE,				//农业率1
	COM_RATE,				//商业率2
	DEF_RATE,				//防卫率3
	TECH_RATE,				//科技率4
	AMBI_RATE,				//扩张率5

	EXTRA_RATE,				//剩余点数

	MAX_RATE,
};
//!!32位以内
enum ENUM_UPDATE_CITY_ATT
{
	CITY_UPDATE_CITY_LEVEL = 0,
	CITY_UPDATE_CITY_NAME,
	CITY_UPDATE_BUILDING,
	CITY_UPDATE_OPT,
	CITY_UPDATE_PROGRESS,
	CITY_UPDATE_CITY_MAINTAIN_STATUS,
	CITY_UPDATE_CITY_6_RATE,
	CITY_UPDATE_CITY_6_LV,
	CITY_UPDATE_MONEY,
	CITY_OPT_BUY_RESEARCH,
	CITY_OPT_SELL_MERCHANDISE,
	CITY_UPDATE_CITY_FRIEND_GUILD_LIST,

	CITY_UPDATE_MAX,
};

enum ENUM_CITY_OPT
{
	CITY_OPT_NONE	=	0,
	CITY_OPT_LEVEL_UP,
	CITY_OPT_CANCEL_LEVEL_UP,
	CITY_OPT_SUCCESS_LEVEL_UP,
	CITY_OPT_BUILD,
	CITY_OPT_LEVEL_DOWN,
	CITY_OPT_DESTORY,
	CITY_OPT_RESEARCH,
	CITY_OPT_CANCEL_RESEARCH,
	CITY_OPT_SUCCESS_RESEARCH,
	CITY_OPT_BUY_RESEARCH_ITEM,
	CITY_OPT_SELL_TICKET_ITEM,
	CITY_OPT_ADD_FRIEND_GUILD,
	CITY_OPT_REMOVE_FRIEND_GUILD,

	CITY_OPT_MAX,
};

enum BUILDING_TYPE
{
	BUILDING_XIANYA = 0,					//县衙0
	BUILDING_XIANGFANG,						//厢房1
	BUILDING_JIUSI,							//酒肆2
	BUILDING_QIANZHUANG,					//钱庄3
	BUILDING_FANGJUFANG,					//防具4
	BUILDING_DUANTAI,						//锻台5
	BUILDING_WUJUFANG,						//武具6
	BUILDING_MICANG,						//米仓7
	BUILDING_CHENGQIANG,					//城墙8
	BUILDING_JIFANG,						//集仿9
	BUILDING_YISHE,							//医舍10
	BUILDING_WUFANG,						//武仿11
	BUILDING_JIANTA,						//箭塔12
	BUILDING_SHUFANG,						//书房13
	BUILDING_QIJI,							//奇迹14
	BUILDING_XIAOCHANG,						//校场15
	BUILDING_HUOBINGTA,						//火冰塔16
	BUILDING_DAQI,							//大旗17

	BUILDING_MAX,

};

enum PROGRESS_TYPE
{
	PROGRESS_BUILDING = 0,					//建筑
	PROGRESS_RESEARCH,						//研究

	PROGRESS_MAX,

};

enum	MAINTAIN_STATUS
{
	MAINTAIN_NONE =0, 
	MAINTAIN_LOW,							//底端维护状态
	MAINTAIN_NORMAL,						//普通维护状态
	MAINTAIN_HIGH,							//高端维护状态
};

//商区
enum	COMMERCE_SCOPE
{
	INVALID_COMMERCE = -1,
	MAIN_CITIES,							//3主城0
	YAN_MEN,								//雁门1
	HANG_ZHOU,								//杭州2
	DIAN_YU,								//滇域3
	LIAO_DI,								//辽地4
	LING_NAN,								//岭南5
	SHI_CHENG,								//石城6
	SAI_WAI,								//塞外7
	HAI_KOU,								//海口8
	MIAO_YU,								//苗域9
	MAX_COMMERCE_SCOPE,
};

enum ValueType
{	
	SALE_PRICE_VALUE = 0,
	BUY_PRICE_VALUE,
};

#define MAX_RPITEM_PER_WORLD				255

#endif // 
