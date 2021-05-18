//
// User是用来保存玩家数据的集合，此玩家有可能当前并不在线
//
//
#ifndef __USER_H__
#define __USER_H__


#include "Type.h"



//无效的用户ID句柄
#define INVALID_UID -1
//一个服务器Server程序内最多可以拥有的玩家数据个数
#define MAX_USER 2048






struct _USERINFO
{
	ID_t		m_UID ;


	_USERINFO(){
		m_UID = INVALID_UID ;
	};
};





class User
{
public :
	User() ;
	~User() ;



public :
	_USERINFO	m_Info ;

};





#endif
