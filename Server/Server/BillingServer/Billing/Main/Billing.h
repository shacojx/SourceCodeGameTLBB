#ifndef __BILLING_H__
#define __BILLING_H__

#include "Type.h"


class Billing
{
public :
	Billing( ) ;
	~Billing( ) ;



	BOOL		Init( ) ;
	BOOL		Loop( ) ;
	BOOL		Exit( ) ;	


protected :
	BOOL		NewStaticManager( ) ;
	BOOL		InitStaticManager( ) ;
	BOOL		DelStaticManager( ) ;




};

extern Billing g_Billing ;


#endif
