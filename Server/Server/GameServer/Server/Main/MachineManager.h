

#ifndef __MACHINEMANAGER_H__
#define __MACHINEMANAGER_H__

#include "Type.h"
#include "Config.h"


class MachineManager
{
public :
	MachineManager( ) ;
	~MachineManager( ) ;

	BOOL			Init( ) ;

	ID_t			GetMachineID( ) ;
	ID_t			GetMachineID( ID_t ServerID ) ;

	_MACHINE_DATA*	GetMachineInfo( ) ;
	_MACHINE_DATA*	GetMachineInfo( ID_t MachineID ) ;


};

extern MachineManager* g_pMachineManager ;



#endif
