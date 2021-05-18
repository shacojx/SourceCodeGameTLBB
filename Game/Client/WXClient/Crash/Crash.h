/************************************************************\
*          程序崩溃时保存信息并启动CrachReport               *
*                                                            *
*                                                            *
*   ref from http://www.codeproject.com/tools/minidump.asp   *
*       By Vladimir Sedach                                   *
*                                                            *
*                                              by jinchao    *
\************************************************************/

#pragma optimize("y", off)		//generate stack frame pointers for all functions - same as /Oy- in the project

void WINAPI TDProcessDump(PEXCEPTION_POINTERS pException);
//
//#define _TDTRY_				__try {
//
//#define _TDEXCEPT_			} __except (TDProcessDump(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER) {}
//
//#define _TDCATCH			\
//							}catch(const tAnyException& e) \
//						