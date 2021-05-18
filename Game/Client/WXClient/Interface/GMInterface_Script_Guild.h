/***********************************\
*									*
*		帮会相关接口				*
*		2006-03-24					*
*									*
\***********************************/

#include "Type.h"

namespace LuaPlus
{
	class LuaState;
	class LuaObject;
};

namespace SCRIPT_SANDBOX
{
	class Guild
	{

	//LuaPlus界面相关接口
	public:
		//创建帮会
		INT	CreateGuild(LuaPlus::LuaState* state);

		//创建帮会确认
		INT CreateGuildConfirm(LuaPlus::LuaState* state);

		//向World申请帮会详细信息
		INT AskGuildDetailInfo(LuaPlus::LuaState* state);

		//向World申请帮会成员信息
		INT AskGuildMembersInfo(LuaPlus::LuaState* state);

		//向World申请帮会职位信息
		INT AskGuildAppointPosInfo(LuaPlus::LuaState* state);

		//获得现有帮会的总数
		INT	GetGuildNum(LuaPlus::LuaState* state);

		//获得其他帮会的信息
		INT GetGuildInfo(LuaPlus::LuaState* state);

		//加入帮会
		INT JoinGuild(LuaPlus::LuaState* state);

		//退出帮会
		INT	QuitGuild(LuaPlus::LuaState* state);

		//踢出帮会,拒绝申请
		INT KickGuild(LuaPlus::LuaState* state);

		//接纳会员
		INT RecruitGuild(LuaPlus::LuaState* state);

		//获得自己的帮众信息
		INT GetMembersNum(LuaPlus::LuaState* state);

		//Lua显示里list控件的位置[4/16/2006]
		INT GetShowMembersIdx(LuaPlus::LuaState* state);
		INT GetShowTraineesIdx(LuaPlus::LuaState* state);

		//获得自己的帮众详细信息
		INT	GetMembersInfo(LuaPlus::LuaState* state);

		//获得自己的帮派信息
		INT GetMyGuildInfo(LuaPlus::LuaState* state);

		//获得自己帮派的详细信息
		INT GetMyGuildDetailInfo(LuaPlus::LuaState* state);

		//获得自己的帮派权利
		INT GetMyGuildPower(LuaPlus::LuaState* state);

        //修改现有帮众职位
		INT	AdjustMemberAuth(LuaPlus::LuaState* state);

		//帮会让位给别人
		INT ChangeGuildLeader(LuaPlus::LuaState* state);

		//帮会删除
		INT	DestoryGuild(LuaPlus::LuaState* state);

		//修改帮会信息
		INT FixGuildInfo(LuaPlus::LuaState* state);

		//帮会禅让
		INT DemisGuild(LuaPlus::LuaState* state);

		//准备帮会成员数据[4/16/2006]
		INT PrepareMembersInfomation(LuaPlus::LuaState* state);

	//共用函数
	public:
		//显示提示信息
		// msgType		消息号，用来在字典里获取相应的文字
		// bTalk		需要显示在聊天窗口
		// bTip			需要显示在屏幕中间的提示
		VOID ShowMsg(INT msgType, BOOL bTalk = FALSE, BOOL bTip = TRUE);

		struct Name_Idx
		{
			STRING	m_MemberName;		//对应DataPool中GuildMemberInfo_t结构里的m_szName
			INT		m_MemberIdx;		//对应DataPool中GuildMemberInfo_t的索引值
			INT		m_Position;			//在帮中的职位

			Name_Idx()
			{
				m_MemberName.erase();
				m_MemberIdx = -1;
				m_Position = GUILD_POSITION_INVALID;
			}

			VOID	CleanUp()
			{
				m_MemberName.erase();
				m_MemberIdx = -1;
				m_Position = GUILD_POSITION_INVALID;
			}
		};

		struct ShowColor
		{
			STRING	m_OnlineLeaderColor;		//在线领导的显示颜色
			STRING	m_LeaveLeaderColor;			//离线领导的显示颜色
			STRING	m_OnlineMemberColor;		//在线帮众显示颜色
			STRING	m_LeaveMemberColor;			//离线帮众显示颜色
			STRING	m_OnlineTraineeColor;		//在线申请人显示颜色
			STRING	m_LeaveTraineeColor;		//离线申请人显示颜色

			ShowColor()
			{
				m_OnlineLeaderColor = "#B";			//蓝色
				m_OnlineMemberColor = "#W";			//白色
				m_OnlineTraineeColor = "#W";

				m_LeaveLeaderColor = "#c808080";	//灰色
				m_LeaveMemberColor = "#c808080";
				m_LeaveTraineeColor = "#c808080";
			}
		};
	private:
		//帮会成立时间转换
		VOID ConvertServerTime(INT dTime, STRING& strTime);

		//更新帮会相应的显示信息
		VOID PerpareMembersBeforeShow();

		//选择显示颜色
		STRING GetShowColor_For_Lua(INT idx);

		std::vector<STRING>		m_MsgArray;			//配合ShowMsg函数使用
		INT						m_iMemberBak;		//改变职位时的备份是要修改哪个玩家的职位信息

		//成员列表
		typedef		std::vector<Name_Idx>	MEMBER_TABLE;
		typedef		std::vector<INT>		MEMBER_INDEX_TABLE;

		MEMBER_INDEX_TABLE						m_AllMembers;			//所有成员
		MEMBER_INDEX_TABLE						m_AllTrainees;			//所有预备成员
		ShowColor								m_LuaShowColors;		//Lua里显示的颜色
	public:
		Guild();
		virtual ~Guild();

		static Guild s_Guild;
		static LuaPlus::LuaObject* s_pMetaTable;
	};

	class SimpleCmpFunc
	{
	public:
		static BOOL less_strcmp(const Guild::Name_Idx& s1, const Guild::Name_Idx& s2);
		static BOOL less_strcoll(const Guild::Name_Idx& s1, const Guild::Name_Idx& s2);

		static BOOL less_position(const Guild::Name_Idx&s1, const Guild::Name_Idx& s2);
	};
};