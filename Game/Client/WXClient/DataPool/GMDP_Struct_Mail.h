#pragma once

#include "Type.h"
#include "GameStruct.h"

#define TIME_STRING_LENGTH				24

struct SMail
{
	UINT								m_uFlag;								// 邮件标志 enum MAIL_TYPE
	GUID_t								m_SourGUID;								// 发信人的 GUID
	CHAR								m_szCreateTime[TIME_STRING_LENGTH];		// 邮件发送时间
	CHAR								m_szSourName[MAX_CHARACTER_NAME];		// 发信人
	INT									m_nPortrait;							// 发信人头像
	CHAR								m_szDestName[MAX_CHARACTER_NAME];		// 收信人
	CHAR								m_szContex[MAX_MAIL_CONTEX];			// 内容
	CampID_t							m_Camp;

	SMail()								{ CleanUp(); }
	VOID								CleanUp();
};
typedef std::vector< SMail* >			SMailList;

class CMailPool
{
public:
	CMailPool();
	~CMailPool();

	// 得到邮件池中的邮件数量，好像没有多大意义
	INT									GetMailCount();

	// 得到剩余未收的邮件数量
	INT									GetMailLeftCount();

	// 设置剩余未收的邮件数量
	VOID								SetMailLeftCount( INT nCount );

	// 加入一封邮件，并将加入结果的在数据池中的索引返回，如果加入失败，则返回 -1
	INT									AddMail( const MAIL* pMail );

	// 根据编号取得邮件
	const SMail*						GetMail( UINT uIndex );

	// 通知 UI 有未收邮件
	VOID								InformUI();

	// 让 UI 显示最后一封邮件
	VOID								ShowLastMail();

private:
	INT									m_nMailLeft;							// 还有多少封没有收下来
	SMailList							m_MailPool;								// 邮件池
};
