
#ifndef __SHAREMEMORY_H__
#define __SHAREMEMORY_H__

#include "Type.h"
#include "SMUManager.h"
#include "Config.h"





class ShareMemory
{
	

	struct SMUPool_T 
	{
		_SHAREMEM_DATA	m_Data;
		void*			m_Pool;

		SMUPool_T()
		{
			m_Pool	=	NULL;
		}
	};


	struct SMUManager_T
	{
		void*				m_SMULogicManager;
		SHAREMEM_TYPE		m_Type;
		SMUManager_T()
		{
			m_Type = ST_INVAILD;
			m_SMULogicManager	=	0;
		}
	};
public :
	ShareMemory( ) ;
	~ShareMemory( ) ;



	BOOL		Init( ) ;
	BOOL		Loop( ) ;
	BOOL		Exit( ) ;

	BOOL		DoWork();

	VOID		ConsoleCommand();


protected :
	BOOL					NewStaticManager( ) ;
	BOOL					InitStaticManager( ) ;
	BOOL					DelStaticManager( ) ;


private:

	SMUPool_T				m_SMUPool[MAX_SM_OBJ_NUM];
	SMUManager_T			m_SMULogicManager[MAX_SM_OBJ_NUM];

	BOOL					m_bExited;

};

extern ShareMemory g_ShareMemory ;
extern INT		   g_CmdArgv;
extern INT		   g_CmdInput;


class ShmExceptionHandler
{
public:
	ShmExceptionHandler();
	//VOID INTHandler(INT);
	//VOID TERMHandler(INT);
	//VOID ABORTHandler(INT);
	//VOID ILLHandler(INT);
	//VOID FPEHandler(INT);
	//VOID SEGHandler(INT);
	//VOID XFSZHandler(INT);
};

extern ShmExceptionHandler g_ShmExceptionHandler;

#endif
