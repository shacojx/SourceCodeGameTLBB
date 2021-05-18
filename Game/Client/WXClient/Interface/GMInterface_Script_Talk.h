/************************************\
*									*
*				聊天接口			*
*									*
\************************************/

#include "CGChannelCreate.h"
#include "CGChannelDismiss.h"
#include "CGChannelInvite.h"
#include "CGChannelKick.h"
#include "GCChat.h"
#include "GCChannelResult.h"
#include "GCChannelError.h"
#include "CGModifySetting.h"
#include "GCRetSetting.h"
#include "Type.h"

namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}

enum ENUM_CHAT_TYPE;
enum CHAT_NEED_TYPE;
struct EVENT;

enum TALK_MAP_ICON
{	
	//0
	SELF_CMD = 0,
	//1
	SELF_TYPE,
	//2
	SELF_SCENEID,
	SELF_MONSTER_SERVER_ID = SELF_SCENEID,
	SELF_SERVERMSG_TEMPLAT = SELF_SCENEID,
	//3
	SELF_X,
	SELF_MONSTER_PAOPAO_ID = SELF_X,
	SELF_SERVERMSG_PARAM1 = SELF_X,
	//4
	SELF_Z,
	//5
	SELF_TIP,
	SELF_NPC_NAME = SELF_TIP,
};

//发送状态
enum CHAT_SEND_STATE
{
	SEND_CHAT_OK = 1,
	SEND_FAILED_TIME_SLICE = -1,
	SEND_FAILED_LV10 = -2,
	SEND_FAILED_TEMPLATE = -3,
	SEND_FAILED_NEED = -4,
	SEND_FAILED_NOREASON = -5,
	SEND_FAILED_PINGBI = -6,
};
//人物休闲动作相关
typedef		std::vector<STRING>				TALK_ACT_MSG;
enum
{
	TALK_ACT_NOBODY,
	TALK_ACT_MYSELF,
	TALK_ACT_OTHER,
	
	TALK_ACT_NUMBER,
};

struct TALK_ACT_STRUCT
{
	TALK_ACT_MSG	m_actMsg;				//聊天显示信息
	STRING			m_actOrder;				//人物动作命令串

	INT				m_actIdx;				//动作索引，为界面的actbutton显示用

	STRING			m_iconName;				//图标
	STRING			m_tip;					//提示信息
};

//字符串分析相关
enum TALKANALYZE_STR_ENUM
{
	ORGINAL_STR,
	SEND_STR,
	TALK_STR,
	PAOPAO_STR,
	HISTORY_STR,

	PRIVATE_STR,
};

enum TALKANALYZE_STRTYPE_ENUM
{
	STRINGTYPE_INVALID = -1,

	STRINGTYPE_NORMAL = 0,			//普通消息
	STRINGTYPE_GM,					//GM命令
	STRINGTYPE_TXTACT,				//文字表情
	STRINGTYPE_ACT,					//人物休闲动作
// ----------------------------------------------------------------- [4/3/2006] @*特殊命令请往下添加
	STRINGTYPE_FLAGADD,				//地图指示点填加	[4/13/2006]废弃
	STRINGTYPE_FLASHADD,			//地图闪烁点填加	[4/13/2006]废弃
	STRINGTYPE_FLAGDEL,				//地图指示点删除	[4/13/2006]废弃
	STRINGTYPE_FLASHDEL,			//地图闪烁点删除	[4/13/2006]废弃

	STRINGTYPE_FLAG_NPC_ADD,		//用NPC名字来添加指示点
	STRINGTYPE_FLAG_POS_ADD,		//按x,z来添加指示点
	STRINGTYPE_FLASH_NPC_ADD,		//用NPC名字来添加黄色指示点
	STRINGTYPE_FLASH_POS_ADD,		//用x,z来添加黄色指示点

	STRINGTYPE_MONSTER_PAOPAO,		//怪物泡泡
	STRINGTYPE_SERVERMSG_TALK,		//Server端发送过来的提示信息消息
// ----------------------------------------------------------------- [4/3/2006] @*特殊命令请往上添加
	STRINGTYPE_NOUSER,				//私聊目标不在线，服务器传回来的提示信息

	STRINGTYPE_NUMBERS,
};

//
//脚本函数沙箱，这些函数应该由脚本调用
//

//#define _MSG_POSITION_SHOW_			//调试消息调用位置的定义

//CHAT_TYPE_SELF类型的自建消息
#define ADDTALKMSG(szMsg)	SCRIPT_SANDBOX::Global_Msg_Func::addTalkMsg(szMsg, CHAT_TYPE_SELF, __FILE__, __LINE__);

//CHAT_TYPE_TEAM类型的自建消息
#define ADD_TEAM_MSG(szMsg)	SCRIPT_SANDBOX::Global_Msg_Func::addTalkMsg(szMsg, CHAT_TYPE_TEAM, __FILE__, __LINE__);

//CHAT_TYPE_GUILD类型的自建消息
#define ADD_GUILD_MSG(szMsg)	SCRIPT_SANDBOX::Global_Msg_Func::addTalkMsg(szMsg, CHAT_TYPE_GUILD, __FILE__, __LINE__);

//CHAT_TYPE_MENPAI类型的自建消息
#define ADD_MENPAI_MSG(szMsg)	SCRIPT_SANDBOX::Global_Msg_Func::addTalkMsg(szMsg, CHAT_TYPE_MENPAI, __FILE__, __LINE__);

//CHAT_TYPE_SYSTEM类型的自建消息
#define ADD_SYSTEM_MSG(szMsg)	SCRIPT_SANDBOX::Global_Msg_Func::addTalkMsg(szMsg, CHAT_TYPE_SYSTEM, __FILE__, __LINE__);

namespace SCRIPT_SANDBOX
{
	extern const INT		TALK_TO_USER_MAXNUM;
	extern const INT		CHANNEL_INVITE_USER_MAXNUM;
	extern const INT		CHAT_TALK_SLICE;
	
	//添加消息封装
	class Global_Msg_Func
	{
	public:
		static VOID addTalkMsg(LPCTSTR szMsg, ENUM_CHAT_TYPE chatType, CHAR* file, INT line);
		static VOID addTalkMsg(STRING szMsg, ENUM_CHAT_TYPE chatType, CHAR* file, INT line);
	};

	//字符串分析
	class TalkAnalyze
	{
	public:
		TalkAnalyze(INT ty, STRING str);
		virtual			~TalkAnalyze(){};

		virtual VOID	doAnalyze(STRING& strTalker);	//分析

		INT				getChannelType(){return m_ChannelType;}
		INT				getOrChannelType(){return m_OrChannelTye;}

		STRING			getStr(INT type);
		STRING			getErrStr(){return m_ErrStr;}
		BOOL			IsOk();
		INT				getStrType(){return m_StrType;}
	protected:
		VOID			JudgeChannelType();
		VOID			MoveHeader();
		VOID			JudgeStringType();
		VOID			GenAllStr();

		virtual VOID	GenSendStr(){}
		virtual	VOID	GenTalkStr(){}
		virtual	VOID	GenPaoPaoStr(){}
		virtual VOID	GenHistoryStr(){}

		VOID			RulerCheck();
		STRING			RemoveInvalidControlChar(const STRING& strIn);
		STRING			RemoveSpaceChar(const STRING& strIn);
	protected:
		friend class Talk;

		INT m_ChannelType;		//分析后的实际频道类型
		STRING	m_SendStr;		//网络传输用的字串
		STRING	m_TalkStr;		//界面上聊天窗口显示用的字串
		STRING	m_PaoPaoStr;	//泡泡上显示用的字串
		STRING	m_HistoryStr;	//历史记录应使用的字串
		STRING	m_TargetStr;	//私聊对象的名称
		STRING  m_HeaderStr;	//被剪切掉的文字头，用来构造发送历史字串使用

		STRING	m_OrInputStr;	//玩家输入的原始完整的的消息字串
		INT		m_OrChannelTye;	//默认的当前频道类型

		BOOL	m_Ok;			//是否分析成功
		INT		m_StrType;		//字符串的类型
		STRING	m_ErrStr;		//获得分析结果
	protected:
		STRING	m_PrevResult;
	};

	class SendTalkAnalyze : public TalkAnalyze
	{
	public:
		SendTalkAnalyze(INT ty, STRING str):TalkAnalyze(ty,str){}
		BOOL			IsOk();
	protected:
		virtual VOID	GenSendStr();
		virtual VOID	GenHistoryStr();
	};

	class RecvTalkAnalyze : public TalkAnalyze
	{
	public:
		RecvTalkAnalyze(INT ty, STRING str):TalkAnalyze(ty,str){}
		virtual VOID	doAnalyze(STRING& strTalker);	//分析
		STRING			getTalker(){return m_Talker;}
	protected:
		virtual	VOID	GenTalkStr();
		virtual	VOID	GenPaoPaoStr();
		virtual VOID	GenHistoryStr();
	private:
		STRING			m_Talker;	//说话的人
	};

	class LuaTalkAnalyze : public TalkAnalyze
	{
	public:
		LuaTalkAnalyze(INT ty, STRING str):TalkAnalyze(ty,str){}
		BOOL			IsOk();
		virtual VOID	doAnalyze(STRING& strTalker);	//分析
	protected:
		virtual	VOID	GenTalkStr();
	private:
		STRING			m_Talker;	//说话的人
	};

	//聊天相关接口
	class Talk
	{
	public:
		class HistoryMsg
		{
		public:
			HistoryMsg()
			{
				m_Name = "";
				m_ChannelType = CHAT_TYPE_INVALID;
				m_Data = "";
				m_Time = 0;
			};

			HistoryMsg(ENUM_CHAT_TYPE type);

			VOID					SetMsgName(STRING name){m_Name = name;}
			STRING					GetMsgName(){return m_Name;}

			VOID					SetChannelType(ENUM_CHAT_TYPE ctype){m_ChannelType = ctype;}
			ENUM_CHAT_TYPE			GetChannelType(){return m_ChannelType;}

			VOID					SetMsgData(STRING data, CHAR* file = NULL, INT line = 0);
			STRING					GetMsgData(){return m_Data;}

			VOID					SetMsgTime(UINT time){m_Time = time;}
			UINT					GetMsgTime(){return m_Time;}

			INT						SetByPacket(GCChat *packet);	// GCChat Packet	->	HistoryMsg Msg
		protected:
			STRING					m_Name;			//talker
			ENUM_CHAT_TYPE			m_ChannelType;	//channel
			STRING					m_Data;			//content

			UINT					m_Time;			//recvtime or sendtime
		};
	protected:
		//历史记录队列
		std::deque< HistoryMsg >	m_SendHisQue;	//送出的聊天消息
		std::deque< HistoryMsg >	m_RecvHisQue;	//收到的聊天消息（包括客户端自己创建的消息）
		std::deque< HistoryMsg >	m_GMCmdHisQue;	//GM命令历史消息
		
		struct Talk_Need_Struct
		{
			CHAT_NEED_TYPE		type;				//消耗类型
			INT					value;				//消耗数量

			Talk_Need_Struct()
			{
				type = CHAT_NEED_NONE;
				value = 0;
			}
		};
		
		typedef std::vector< Talk_Need_Struct >		TALK_NEED_ARRAY;

		class Channel
		{
		public:
			Channel()
			{
				m_channelType = CHAT_TYPE_INVALID;
				m_channelName = "";
				m_channelHeader = "";
				m_TimeSlice = 0;
				m_close = FALSE;
				m_need.resize(CHAT_NEED_NUMBER);
				m_lastSendTime = 0;
			};
			virtual ~Channel(){};
			
			VOID					SetType(ENUM_CHAT_TYPE ctype){m_channelType = ctype;}
			ENUM_CHAT_TYPE			GetType(){return m_channelType;}

			VOID					SetName(STRING name){m_channelName = name;}
			STRING					GetName(){return m_channelName;}
			STRING					GetHeader(){return m_channelHeader;}

			UINT					GetSliceTime(){return m_TimeSlice;}
			VOID					SetSliceTime(UINT dwTime){m_TimeSlice = dwTime;}
			
			VOID					Close(){m_close = TRUE;}
			VOID					Open(){m_close = FALSE;}
			BOOL					isClose(){return m_close;}
			BOOL					TalkNeedCheck();	//TRUE 检查通过 FALSE 检查不通过
			BOOL					TalkTimeCheck(UINT dwTimeNow);	//TRUE 检查通过 FALSE 检查不通过
			VOID					SetNeed(INT tp, INT value);

			VOID					LoadChannelConfig();
			virtual VOID			SetSendPacket(CGChat *packet, TalkAnalyze* pAna);	//设置本频道需要设置的相关内容
		protected:
			ENUM_CHAT_TYPE			m_channelType;	//频道类型
			STRING					m_channelName;	//频道名称
			STRING					m_channelHeader;	//频道的前导字符
			UINT					m_TimeSlice;	//频道的默认时间控制
			BOOL					m_close;		//频道是否关闭
			TALK_NEED_ARRAY			m_need;			//频道消耗

			UINT					m_lastSendTime;	//最后一次发送信息的时间
		};

		class PrivateChannel : public Channel
		{
		public:
			PrivateChannel()
			{
				m_channelType = CHAT_TYPE_CHANNEL;
				//m_channelName = "自建频道";
				m_GUID = INVALID_GUID;
			};
			virtual ~PrivateChannel(){};

			VOID					SetGUID(GUID_t id){m_GUID = id;}
			GUID_t					GetGUID(){return m_GUID;}
			
			VOID					Clean();	//删除自建频道

			INT						AddToQue(STRING user);
			INT						KickFromQue(STRING user);
			INT						IsInQue(STRING user);
			STRING					GetUserByIndex(UINT idx);
		protected:
			GUID_t					m_GUID;			//自建频道在服务器上的唯一编号
			std::deque< STRING >	m_InviteQue;
		};

		//自建频道
		PrivateChannel*				m_PrvCh;

		class UserChannel : public Channel
		{
		public:
			UserChannel()
			{
				m_channelType = CHAT_TYPE_TELL;
				//m_channelName = "密聊频道";

				m_curSelPos = -1;
			};
			virtual ~UserChannel(){};

			INT						AddUser(STRING user);
			INT						IsInQue(STRING user);
			VOID					Clean();
			STRING					GetUserByIndex(UINT idx);
			UINT					GetUserNum(){return (UINT)m_UserQue.size();};

			STRING					SelectFirstUser();
			STRING					SelectLastUser();
			STRING					SelectNextUser(BOOL dir); //TRUE 0->end, FALSE end->0
			BOOL					IsFirstUserSelect(){return (0 == m_curSelPos)?TRUE:FALSE;}
			BOOL					IsLastUserSelect(){return (m_curSelPos == (INT)m_UserQue.size()-1)?TRUE:FALSE;}
			BOOL					SetSelectByUser(STRING name);

			virtual VOID			SetSendPacket(CGChat *packet, TalkAnalyze* pAna);
		protected:
			std::deque< STRING >	m_UserQue;

			INT						m_curSelPos;	//当前选中位置
		};

		//密聊频道
		UserChannel*				m_UsrCh;

		class TeamChannel : public Channel
		{
		public:
			TeamChannel()
			{
				m_channelType = CHAT_TYPE_TEAM;
				//m_channelName = "组队频道";
			};
			
			virtual ~TeamChannel(){};

			VOID					SetTeamID(TeamID_t id){m_teamID = id;};
			TeamID_t				GetTeamID(){return m_teamID;};
			virtual VOID			SetSendPacket(CGChat *packet, TalkAnalyze* pAna);
		protected:
			TeamID_t	m_teamID;
		};

		//组队频道
		TeamChannel*				m_TeamCh;

		typedef Channel SystemChannel;
		typedef Channel	SelfChannel;
		
		//系统频道
		SystemChannel*				m_SysCh;
		//客户端自用频道
		SelfChannel*				m_SelfCh;

		class MenPaiChannel : public Channel
		{
		public:
			MenPaiChannel()
			{
				m_channelType = CHAT_TYPE_MENPAI;
				//m_channelName = "门派频道";
			};
			virtual ~MenPaiChannel(){};

			VOID					SetMenPaiID(MenPaiID_t id){m_menpaiID = id;};
			MenPaiID_t				GetMenPaiID(){return m_menpaiID;};
			virtual VOID			SetSendPacket(CGChat *packet, TalkAnalyze* pAna);
		protected:
			MenPaiID_t	m_menpaiID;
		};

		//门派频道
		MenPaiChannel*				m_MenPaiCh;

		class GuildChannel : public Channel
		{
		public:
			GuildChannel()
			{
				m_channelType = CHAT_TYPE_GUILD;
				//m_channelName = "帮派频道";
			};

			virtual ~GuildChannel(){};

			VOID					SetGuildID(GuildID_t id){m_guildID = id;};
			GuildID_t				GetGuildID(){return m_guildID;};
			virtual VOID			SetSendPacket(CGChat *packet, TalkAnalyze* pAna);
		protected:
			GuildID_t	m_guildID;
		};

		//帮派频道
		GuildChannel*				m_GuildCh;

		//所有频道(给界面选择默认频道显示使用)
		std::list<Talk::Channel *>	m_listChannel;

		//保存聊天信息的最大值
		INT							m_MaxSaveNum;

		//上次处理历史发送信息的位置
		INT							m_PrevSendPos;

		//上次处理GM命令发送信息的位置
		INT							m_PrevGMCmdPos;

		//上次处理菜单的link内容
		STRING						m_PrevMenuLink;

		//泡泡文字信息是否限制
		BOOL						m_bPaoPaoRule;

		//聊天动作查询表
		typedef		std::map<STRING, TALK_ACT_STRUCT>		TALK_ACTMAP;
		TALK_ACTMAP					m_TalkActMap;
	
		//屏蔽人物列表
		std::deque< STRING >		m_PingBiQue;	

	//事件回调函数来定时处理帮助消息
	public:
		static VOID WINAPI	_SendTalkHelpMsg(const EVENT* pEvent, UINT dwOwnerData);
		static VOID WINAPI	_HandleWindowSizeChange(const EVENT* pEvent, UINT dwOwnerData);
		VOID				Initial();
	//LuaPlus界面相关接口
	public:
		//得到频道数量
		/*
		|	返回参数2 [String]  - 频道个数
		*/
		INT	GetChannelNumber(LuaPlus::LuaState* state);

		//得到某个频道数据
		/*
		|	返回参数1 [String]  - 频道类型 
		|		"near"		- 附近玩家
		|		"scene"		- 场景
		|		"system"	- 系统
		|		"team"		- 队伍
		|		"guild"		- 帮会
		|		"user"		- 自建
		|		"private"	- 私聊
		|		"self"		- 客户端拣到道具等不由服务器发送过来的消息， 是个隐藏的频道
		|
		|	返回参数2 [String]  - 频道名称
		|
		|
		*/
		INT	GetChannel(LuaPlus::LuaState* state);

		//获得聊天前导字串
		INT	GetChannelHeader(LuaPlus::LuaState* state);

		//玩家发送聊天指令
		/*
		|	传入参数1 [String] - 当前所在频道
		|	传入参数2 [String] - 消息具体内容
		|
		*/
		INT	SendChatMessage(LuaPlus::LuaState* state);

		//发送创建频道的消息 C->G
		INT	CreateChannel(LuaPlus::LuaState* state);

		//邀请其他玩家加入自建频道 C->G
		/*
		|	传入参数1 [String] - 要加入的玩家姓名
		*/
		INT	JoinUserToChannel(LuaPlus::LuaState* state);

		//踢掉自建频道中的其他玩家 C->G
		/*
		|	传入参数1 [String] - 要踢出的玩家姓名
		*/
		INT KickUserFromChannel(LuaPlus::LuaState* state);

		//发送删除频道的消息 C->G
		INT	DestoryChannel(LuaPlus::LuaState* state);

		//填加历史消息信息到界面
		/*
		|	传入参数1 [int]		- tab页面的编号，从0开始
		|	传入参数2 [String]	- 此页面的配置信息字符串
		*/
		INT	InsertHistory(LuaPlus::LuaState* state);
		
		//设置聊天消息保留的最大数量
		/*
		|	传入参数1 [int] - 保存历史的最大值
		*/
		INT SetMaxSaveNumber(LuaPlus::LuaState* state);

		//创建聊天页面Tab
		INT	CreateTab(LuaPlus::LuaState* state);

		//配置聊天页面Tab
		INT	ConfigTab(LuaPlus::LuaState* state);

		//创建聊天页面完成
		INT	CreateTabFinish(LuaPlus::LuaState* state);

		//保存聊天页面的设置
		INT SaveTab(LuaPlus::LuaState* state);

		//清空聊天页面的设置
		INT ClearTab(LuaPlus::LuaState* state);

		//配置聊天页面完成
		INT	ConfigTabFinish(LuaPlus::LuaState* state);

		//由频道名称获取相应的频道号
		INT	GetChannelType(LuaPlus::LuaState* state);

		//获取表情窗体
		INT	SelectFaceMotion(LuaPlus::LuaState* state);

		//获取颜色窗体
		INT	SelectTextColor(LuaPlus::LuaState* state);

		//保存当前聊天EditBox里已经输入的信息
		INT SaveOldTalkMsg(LuaPlus::LuaState* state);

		//用键盘快捷键更换默认频道
		INT	ChangeCurrentChannel(LuaPlus::LuaState* state);

		//键盘快捷键导致的聊天EditBox的文字变更
		INT ModifyChatTxt(LuaPlus::LuaState* state);

		//显示聊天菜单
		INT	ShowContexMenu(LuaPlus::LuaState* state);

		//处理菜单命令
		INT	ContexMenuTalk(LuaPlus::LuaState* state);
		// 得到当前选择的人命
		INT GetCurrentSelectName( LuaPlus::LuaState* state );

		//处理菜单其他命令
		INT HandleMenuAction(LuaPlus::LuaState* state);

		//获得聊天表情字串
		INT	GetTalkTemplateString_Lua(LuaPlus::LuaState* state);

		//获得具体的聊天人物休闲动作
		INT	EnumChatMood(LuaPlus::LuaState* state);

		//显示聊天休闲动作窗口
		INT	ShowChatMood(LuaPlus::LuaState* state);

		//隐藏聊天休闲动作窗口
		INT HideChatMood(LuaPlus::LuaState* state);

		//显示聊天屏蔽列表窗口
		INT ShowPingBi(LuaPlus::LuaState* state);

		//获得屏蔽列表的人数
		INT GetPingBiNum(LuaPlus::LuaState* state);

		//根据人数来填加信息
		INT GetPingBiName(LuaPlus::LuaState* state);

        //取消屏蔽玩家
		INT DelPingBi(LuaPlus::LuaState* state);

		//获取当前的输入法性质
		INT GetCurInputLanguage(LuaPlus::LuaState* state);

	//网络处理相关接口
	public:
		//聊天信息包 G->C
		INT		HandleRecvTalkPacket(GCChat* pPacket);

		//聊天相关结果包 G->C
		INT		HandleChannelResultPacket(GCChannelResult* pPacket);

		//聊天相关错误包 G->C
		INT		HandleChannelErrorPacket(GCChannelError* pPacket);

	//公用函数接口
	public:
		//频道类型转换
		static STRING				ChannelType2String(ENUM_CHAT_TYPE type);
		static ENUM_CHAT_TYPE		String2ChannelType(STRING strType);	

		//添加客户端自用消息 only used with CHANNEL_SELF
		// SCRIPT_SANDBOX::Global_Msg_Func [4/29/2006]

		//私聊频道相关接口
		VOID						UserCreate();
		//组队频道相关接口
		VOID						TeamCreate(TeamID_t id);
		VOID						TeamDestory();

		//门派频道相关接口
		VOID						MenPaiJoin(MenPaiID_t id);
		VOID						MenPaiLeave();

		//帮派频道相关接口
		VOID						GuildJoin(GuildID_t id);
		VOID						GuildLeave();

		//聊天相关快捷键处理
		VOID						HandleKey_Shift_ArrowUp(INT type);		// 上箭头，回滚上一条输入的聊天信息 type:0聊天信息 1GM指令
		VOID						HandleKey_Shift_ArrowDown(INT type);	// 下箭头，回滚下一条输入的聊天信息 type:0聊天信息 1GM指令

		VOID						HandleKey_Alt_S();				//发送
		VOID						HandleKey_Ctrl_Enter();			//发送

		VOID						HandleKey_Alt_FaceMotion();		//Alt + '-'	打开表情选择窗口
		VOID						HandleKey_PageUp();				//切换上一个频道
		VOID						HandleKey_PageDown();			//切换下一个频道

		VOID						SendGMCommand(LPCTSTR msg);		//发送GM命令

		VOID						DoAct(LPCTSTR act, LPCTSTR user);	//根据返回的信息来做人物动作
		VOID						GenTalkActMap();					//构造人物动作表

		STRING						GetTalkActMsg(LPCTSTR act, INT idx);	//人物动作命令对应的文字信息
		STRING						GetTalkActOrder(LPCTSTR act);			//人物动作命令对应的命令
		STRING						FindTalkActKey(INT idx);				//依据id来查找人物动作的key
		TALK_ACT_STRUCT*			GetTalkActStructByID(INT idx);			//依据id来查找对应的TALK_ACT_STRUCT，为了建立ActionItem而准备的

		//特殊字符串解析
		BOOL						HandleSpecialSelfString(TalkAnalyze* pAna);
		
		VOID						HandleFlagAdd(LPCTSTR msg);		//[4/13/2006]废弃
		VOID						HandleFlagDel(LPCTSTR msg);		//[4/13/2006]废弃
		VOID						HandleFlashAdd(LPCTSTR msg);	//[4/13/2006]废弃
		VOID						HandleFlashDel(LPCTSTR msg);	//[4/13/2006]废弃

		VOID						HandleFlagNPCAdd(LPCTSTR msg);
		VOID						HandleFlagPOSAdd(LPCTSTR msg);
		VOID						HandleFlashNPCAdd(LPCTSTR msg);
		VOID						HandleFlashPOSAdd(LPCTSTR msg);

		VOID						HandleMonsterPaoPao(LPCTSTR msg);
		VOID						HandleServerMsgTalk(LPCTSTR msg, INT channelType);

		//怪物泡泡
		VOID						SetMonsterPaoPaoText(INT idServer, INT idMsg);

		//屏蔽列表
		VOID						AddPingBi(STRING& strUser);
		VOID						DelPingBi(STRING& strUser);
		BOOL						IsInPingBi(STRING& strUser);
		VOID						ClearPingBi();

	public:
		//根据消息头获得频道类型
		static ENUM_CHAT_TYPE		GetChatTypeFromMessage(STRING &strMsg);
		
		//根据聊天模板获得字串
		static STRING				GetTalkTemplateString(STRING& strMsg, STRING& strTalker);

		//根据聊天动作获得字串
		static STRING				GetTalkActString(STRING& strMsg, STRING& strTalker);

		//聊天泡泡字串显示
		VOID						SetPaoPaoTxt(STRING& strUser, STRING& strTxt, INT strType = STRINGTYPE_NORMAL, INT extLength = 0);

		//聊天帮助信息的填加
		VOID						ShowTalkHelpMsg();

		//屏幕大小变化
		VOID						TalkDisplaySizeChanged(INT width, INT height);

		//文字信息文字尾处理
		STRING						TalkFixTail(STRING& strTalk);
	public:
		//消息历史记录
		INT							AddToSendHistoryQue(HistoryMsg& msg);
		INT							AddToRecvHistoryQue(HistoryMsg& msg);
		INT							AddToGMCmdHistoryQue(HistoryMsg& msg);

		//密聊频道相关接口
		VOID						AddUserToUserChannel(STRING &strUser);

		//根据人物当前的情况设置聊天具体的限制信息
		VOID						SetTalkRule();
		//读取聊天页面的配置信息
		VOID						LoadTabSetting(GCRetSetting* pPacket);
	public:
		Talk();
		virtual ~Talk();

		static Talk s_Talk;
		static LuaPlus::LuaObject* s_pMetaTable;

		friend class TalkAnalyze;
	};

	extern const char*		NAMETYPE_TALK_NEAR;
	extern const char*		NAMETYPE_TALK_SCENE;
	extern const char*		NAMETYPE_TALK_SYSTEM;
	extern const char*		NAMETYPE_TALK_TEAM;
	extern const char*		NAMETYPE_TALK_GUILD;
	extern const char*		NAMETYPE_TALK_USER;
	extern const char*		NAMETYPE_TALK_PRIVATE;
	extern const char*		NAMETYPE_TALK_SELF;

	extern const char*		GM_TALK_HEADER;
	extern const char*		CHAT_ACT_HEADER;
}