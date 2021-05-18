/************************************************************\
*          程序崩溃时保存信息并启动CrachReport               *
*                                                            *
*                                                            *
*   ref from http://www.codeproject.com/tools/minidump.asp   *
*       By Vladimir Sedach                                   *
*                                                            *
\************************************************************/

#pragma optimize("y", off)		//generate stack frame poINTers for all functions - same as /Oy- in the project

BOOL CreateSmallDumpInfo(PEXCEPTION_POINTERS pException, PTCHAR szSmallFile, UINT dwLastError);
BOOL CreateBigInfoFile(PEXCEPTION_POINTERS pException, PTCHAR szBigFile, UINT dwLastError);

BOOL CreateDumpHelpFile(PEXCEPTION_POINTERS pException, PTCHAR szDumpFile);
