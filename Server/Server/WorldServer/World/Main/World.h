
#ifndef __WORLD_H__
#define __WORLD_H__

#include "Type.h"


class World
{
public :
	World( ) ;
	~World( ) ;



	BOOL		Init( ) ;
	BOOL		Loop( ) ;
	BOOL		Exit( ) ;	


protected :
	BOOL		NewStaticManager( ) ;
	BOOL		InitStaticManager( ) ;
	BOOL		DelStaticManager( ) ;




};

extern World g_World ;

class WorldExceptionHandler
{
public:
	WorldExceptionHandler();
	//VOID INTHandler(INT);
	//VOID TERMHandler(INT);
	//VOID ABORTHandler(INT);
	//VOID ILLHandler(INT);
	//VOID FPEHandler(INT);
	//VOID SEGHandler(INT);
	//VOID XFSZHandler(INT);
};

extern WorldExceptionHandler g_WorldExceptionHandler;

#endif
