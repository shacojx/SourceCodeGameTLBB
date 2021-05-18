#include "stdafx.h"
#include "CommandThread.h"
#include "SMULogicManager.h"
#include "Log.h"

CommandThread g_CommandThread;

VOID CommandThread::run()
{ 
	__ENTER_FUNCTION


		while( IsActive() )
		{
			BOOL ret = FALSE ;
			ret = CmdParser::HandleInput();
			Assert(ret);
		}


	__LEAVE_FUNCTION
	

}

#define SM_CMD_SIZE 255

BOOL	 CmdParser::HandleInput()
{
	__ENTER_FUNCTION
	
	CHAR InBuffer[SM_CMD_SIZE];
	memset(InBuffer,0,SM_CMD_SIZE);
	
	INT CharCount	= 0;
	INT	CharIdentiy;	
	
	while(TRUE)
	{
		CharIdentiy=getchar();

		if(CharIdentiy=='\n')
		{
			break;
		}
		if(CharCount>=SM_CMD_SIZE-1)
			break;

		InBuffer[CharCount]=CharIdentiy;
		CharCount++;
	}
	
	
	SM_COMMANDS_STATE cmdState =	ParserCommandType(InBuffer)	;
	
	switch(cmdState.cmdType) 
	{
	case CMD_SAVE_ALL:
		{
			g_CommandThread.m_SMConfig.Commands = cmdState;
			Log::SaveLog("./Log/ShareMemory.log","Start SaveAll!\r\n");
		}
		break;
	case CMD_CLEAR_ALL:
		{
			g_CommandThread.m_SMConfig.Commands = cmdState;
			Log::SaveLog("./Log/ShareMemory.log","Start ClearAll!\r\n");
		}
		break;

	default:
		{
			Log::SaveLog("./Log/ShareMemory.log","Unknow Commands!\r\n");
			

		}
	}
	
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

SM_COMMANDS_STATE	CmdParser::ParserCommandType(CHAR* pBuff)
{
	__ENTER_FUNCTION
		
		CHAR szCmd[SM_CMD_SIZE];
		memset(szCmd,0,SM_CMD_SIZE);
		
		INT i;
		for( i =0;i<SM_CMD_SIZE;i++)
		{
			if(pBuff[i]==' '||pBuff[i]==0||pBuff[i]=='\t')
				break;
			szCmd[i] = pBuff[i];
		}

		SM_COMMANDS_STATE CMDState;
		if(strcmp(szCmd,"saveall") == 0)
		{
			CMDState.cmdType =  CMD_SAVE_ALL;
		}
		else if(strcmp(szCmd,"clearall") == 0)
		{
			CMDState.cmdType =  CMD_CLEAR_ALL;
		}
		else
		{
			CMDState.cmdType = CMD_UNKNOW;
		}
		
		return CMDState;

	__LEAVE_FUNCTION
}