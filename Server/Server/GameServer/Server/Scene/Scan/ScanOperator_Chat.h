/////////////////////////////////////////////////////////////////////////////////
//文件名：ScanOperator_Chat.h
//功能描述：搜索周围的符合要求的obj，用户发送聊天信息
//修改记录：2005-12-08创建
/////////////////////////////////////////////////////////////////////////////////


#ifndef __SCANOPERATOR_CHAT_H__
#define __SCANOPERATOR_CHAT_H__

#include "Type.h"
#include "ScanOperator.h"

class Packet ;

struct SCANOPERATOR_CHAT_INIT : public SCANOPERATOR_INIT
{
	Packet*			m_pPacket ;
	INT				m_nChatType ;
	GuildID_t		m_GuildID ;
	INT				m_MenpaiID ;

	SCANOPERATOR_CHAT_INIT( )
	{
		m_pPacket = NULL ;
		m_nChatType = 0 ;
		m_GuildID = INVALID_ID ;
		m_MenpaiID = MATTRIBUTE_WUMENPAI ;
	};
};



class ScanOperator_Chat : public ScanOperator
{
public :
	ScanOperator_Chat( ) ;
	virtual ~ScanOperator_Chat( ) ;

	//初始化扫描控制器
	virtual BOOL				Init( SCANOPERATOR_CHAT_INIT* pInit ) ;
	//扫描操作开始前回调
	virtual BOOL				On_BeforeScan( ) ;
	//判断参数中的Zone是否需要扫描
//	virtual BOOL				On_IsNeedScan( ZoneID_t ZoneID ) ;
	//搜索到一个Obj, 返回值见 SCANRETURN
	virtual UINT				On_FindObj( Obj* pObj ) ;
	//扫描操作完成后回调
//	virtual VOID				On_AfterScan( ) ;


public :
	Packet*			m_pPacket ;
	INT				m_nChatType ;
	GuildID_t		m_GuildID ;
	INT				m_MenpaiID ;

};




#endif
