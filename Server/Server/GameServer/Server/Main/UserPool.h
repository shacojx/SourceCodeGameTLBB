

#ifndef __USERPOOL_H__
#define __USERPOOL_H__

#include "Type.h"
#include "User.h"


class UserPool
{
public :
	UserPool( ) ;
	~UserPool( ) ;

	BOOL			Init( ) ;



public :
	User			m_pUser[MAX_USER] ;



};




#endif
