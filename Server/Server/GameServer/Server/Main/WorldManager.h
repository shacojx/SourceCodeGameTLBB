

#ifndef __WORLDMANAGER_H__
#define __WORLDMANAGER_H__

#include "Type.h"


class WorldManager
{
public :
	WorldManager( ) ;
	~WorldManager( ) ;


	BOOL			Init( ) ;


	ID_t			GetWorldID( ) ;


private :


};

extern WorldManager* g_pWorldManager ;



#endif
