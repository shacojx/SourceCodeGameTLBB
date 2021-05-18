#include "stdafx.h"
////////////////////////////////////////////////////////////////////////////////
//
// Thread.cpp
//
//
// Last Updated : 2005.03.22
//
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////
#include "Thread.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

#if defined(__WINDOWS__)

#include <Tlhelp32.h>
#include <time.h>
#include <dbghelp.h>
#include <stdio.h>
#include <tchar.h>

#define	DUMP_SIZE_MAX	8000	//max size of our dump
#define	CALL_TRACE_MAX	((DUMP_SIZE_MAX - 2000) / (MAX_PATH + 40))	//max number of traced calls
#define	NL				_T("\n")	//new line

BOOL WINAPI Get_Module_By_Ret_Addr(PBYTE Ret_Addr, CHAR* Module_Name, PBYTE & Module_Addr)
{
	MODULEENTRY32	M = {sizeof(M)};
	HANDLE	hSnapshot;

	Module_Name[0] = 0;

	hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, 0);

	if ((hSnapshot != INVALID_HANDLE_VALUE) &&
		::Module32First(hSnapshot, &M))
	{
		do
		{
			if (UINT(Ret_Addr - M.modBaseAddr) < M.modBaseSize)
			{
				lstrcpyn(Module_Name, M.szExePath, MAX_PATH);
				Module_Addr = M.modBaseAddr;
				break;
			}
		} while (::Module32Next(hSnapshot, &M));
	}

	::CloseHandle(hSnapshot);

	return !!Module_Name[0];
} 

VOID CreateExceptionDesc(PEXCEPTION_POINTERS pException, FILE* fp, UINT dwLastError)
{
	if (!pException || !fp) return;

	EXCEPTION_RECORD &	E = *pException->ExceptionRecord;
	CONTEXT &			C = *pException->ContextRecord;

	//取得异常发生地
	TCHAR		szModeleInfo[MAX_PATH];
	TCHAR		Module_Name[MAX_PATH];
	PBYTE		Module_Addr;
	if (Get_Module_By_Ret_Addr((PBYTE)E.ExceptionAddress, Module_Name, Module_Addr))
	{
		_sntprintf(szModeleInfo, MAX_PATH, _T("%s"), Module_Name);
	}
	else
	{
		_sntprintf(szModeleInfo, MAX_PATH, _T("%08X"), (DWORD_PTR)(E.ExceptionAddress));
	}

	switch(E.ExceptionCode)
	{
		//转化后的c++异常
	case 0XE000C0DE:
		{
			_ftprintf(fp,
				_T("C++ Exception\n")
				_T("\n")
				_T("Expr:      %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n"),
				E.ExceptionInformation[0],
				E.ExceptionInformation[1],
				E.ExceptionInformation[2],
				E.ExceptionInformation[3],
				E.ExceptionInformation[4],
				E.ExceptionInformation[5],
				E.ExceptionInformation[6],
				E.ExceptionInformation[7],
				E.ExceptionInformation[8],
				E.ExceptionInformation[9],
				E.ExceptionInformation[10],
				E.ExceptionInformation[11],
				E.ExceptionInformation[12],
				E.ExceptionInformation[13],
				E.ExceptionInformation[14]);
		}
		break;

		//试图对一个虚地址进行读写
	case EXCEPTION_ACCESS_VIOLATION:
		{
			// Access violation type - Write/Read.
			_ftprintf(fp,
				_T("\t\tAccess violation\n")
				_T("\n")
				_T("@:         %s\n")
				_T("Operate:   %s\n")
				_T("Address:   0x%08X\n")
				_T("LastError: 0x%08X\n"),
				szModeleInfo,
				(E.ExceptionInformation[0]) ? _T("Write") : _T("Read"), 
				E.ExceptionInformation[1], dwLastError);
		}
		break;

	default:
		{
			_ftprintf(fp,
				_T("\t\tOTHER\n")
				_T("\n")
				_T("@:         %s\n")
				_T("Code:      0x%08X\n")
				_T("LastError: 0x%08X\n"),
				szModeleInfo,
				E.ExceptionCode, dwLastError);
		}
		break;
	}

}

VOID WINAPI Get_Version_Str(FILE* fp)
{
	OSVERSIONINFOEX	V = {sizeof(OSVERSIONINFOEX)};	//EX for NT 5.0 and later

	if (!GetVersionEx((POSVERSIONINFO)&V))
	{
		ZeroMemory(&V, sizeof(V));
		V.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx((POSVERSIONINFO)&V);
	}

	if (V.dwPlatformId != VER_PLATFORM_WIN32_NT)
		V.dwBuildNumber = LOWORD(V.dwBuildNumber);	//for 9x HIWORD(dwBuildNumber) = 0x04xx

	TCHAR dateBuf[32];
	_tstrdate(dateBuf);
	TCHAR timeBuf[32];
	_tstrtime(timeBuf);


	_ftprintf(fp,
		_T("==============================================================================\n") 
		_T("Windows:  %d.%d.%d, SP %d.%d, Product Type %d\n") 	//SP - service pack, Product Type - VER_NT_WORKSTATION,...
		_T("\n")
		_T("Time:     %s %s\n"),
		V.dwMajorVersion, V.dwMinorVersion, V.dwBuildNumber, V.wServicePackMajor, V.wServicePackMinor, V.wProductType, 
		dateBuf, timeBuf);
}

VOID Get_Call_Stack(PEXCEPTION_POINTERS pException, FILE* fp)
{
	TCHAR	Module_Name[MAX_PATH];
	PBYTE	Module_Addr = 0;
	PBYTE	Module_Addr_1;

#pragma warning(disable: 4200)	//nonstandard extension used : zero-sized array in struct/union
	typedef struct STACK
	{
		STACK *	Ebp;
		PBYTE	Ret_Addr;
		UINT	Param[0];
	} STACK, * PSTACK;
#pragma warning(default: 4200)

	STACK	Stack = {0, 0};
	PSTACK	Ebp;

	if (pException)		//fake frame for exception address
	{
		Stack.Ebp = (PSTACK)(DWORD_PTR)pException->ContextRecord->Ebp;
		Stack.Ret_Addr = (PBYTE)pException->ExceptionRecord->ExceptionAddress;
		Ebp = &Stack;
	}
	else
	{
		Ebp = (PSTACK)&pException - 1;	//frame addr of Get_Call_Stack()

		// Skip frame of Get_Call_Stack().
		if (!IsBadReadPtr(Ebp, sizeof(PSTACK)))
			Ebp = Ebp->Ebp;		//caller ebp
	}

	// Trace CALL_TRACE_MAX calls maximum - not to exceed DUMP_SIZE_MAX.
	// Break trace on wrong stack frame.
	for (INT Ret_Addr_I = 0;
		(Ret_Addr_I < CALL_TRACE_MAX) && !IsBadReadPtr(Ebp, sizeof(PSTACK)) && !IsBadCodePtr(FARPROC(Ebp->Ret_Addr));
		Ret_Addr_I++, Ebp = Ebp->Ebp)
	{
		// If module with Ebp->Ret_Addr found.
		if (Get_Module_By_Ret_Addr(Ebp->Ret_Addr, Module_Name, Module_Addr_1))
		{
			if (Module_Addr_1 != Module_Addr)	//new module
			{
				// Save module's address and full path.
				Module_Addr = Module_Addr_1;
				_ftprintf(fp, _T("%08X  %s")NL, (LONG_PTR)Module_Addr, Module_Name);
			}

			// Save call offset.
			_ftprintf(fp, _T("  +%08X"), Ebp->Ret_Addr - Module_Addr);

			// Save 5 params of the call. We don't know the real number of params.
			if (pException && !Ret_Addr_I)	//fake frame for exception address
				_ftprintf(fp, _T("  Exception Offset") NL);
			else if (!IsBadReadPtr(Ebp, sizeof(PSTACK) + 5 * sizeof(UINT)))
			{
				_ftprintf(fp, _T("  (%X, %X, %X, %X, %X)") NL,
					Ebp->Param[0], Ebp->Param[1], Ebp->Param[2], Ebp->Param[3], Ebp->Param[4]);
			}
		}
		else
			_ftprintf(fp, _T("%08X")NL, (LONG_PTR)(Ebp->Ret_Addr));
	}

}

VOID Get_Exception_Info(PEXCEPTION_POINTERS pException, FILE* fp, UINT dwLastError)
{
	CHAR		Module_Name[MAX_PATH];
	PBYTE		Module_Addr;
	HANDLE		hFile;
	FILETIME	Last_Write_Time;
	FILETIME	Local_File_Time;
	SYSTEMTIME	T;



	Get_Version_Str(fp);

	_ftprintf(fp, _T("------------------------------------------------------------------------------\n"));
	_ftprintf(fp, _T("Process:  ") );

	GetModuleFileName(NULL, Module_Name, MAX_PATH);
	_ftprintf(fp, _T("%s\n") , Module_Name);

	// If exception occurred.
	if (pException)
	{
		EXCEPTION_RECORD &	E = *pException->ExceptionRecord;
		CONTEXT &			C = *pException->ContextRecord;

		// If module with E.ExceptionAddress found - save its path and date.
		if (Get_Module_By_Ret_Addr((PBYTE)E.ExceptionAddress, Module_Name, Module_Addr))
		{
			_ftprintf(fp, _T("Module:   %s\n") , Module_Name);

			if ((hFile = CreateFile(Module_Name, GENERIC_READ, FILE_SHARE_READ, NULL, 0, FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
			{
				if (GetFileTime(hFile, NULL, NULL, &Last_Write_Time))
				{
					FileTimeToLocalFileTime(&Last_Write_Time, &Local_File_Time);
					FileTimeToSystemTime(&Local_File_Time, &T);

					_ftprintf(fp, _T("Date Modified:  %02d/%02d/%d\n") , 
						T.wMonth, T.wDay, T.wYear);
				}
				CloseHandle(hFile);
			}
		}
		else
		{
			_ftprintf(fp, _T("Exception Addr:  %08X\n")  , (LONG_PTR)(E.ExceptionAddress));
		}

		_ftprintf(fp, _T("------------------------------------------------------------------------------\n"));

		//加入具体异常解释信息
		CreateExceptionDesc(pException, fp, dwLastError);

	}

	_ftprintf(fp, _T("------------------------------------------------------------------------------\n"));

	// Save call stack info.
	_ftprintf(fp, _T("Call Stack:\n"));
	Get_Call_Stack(pException, fp);
}

BOOL CreateBigInfoFile(PEXCEPTION_POINTERS pException, CHAR* szBigFile, UINT dwLastError)
{
	if (!pException) return FALSE;


	TCHAR szTempFile[MAX_PATH] = {0};
	strcpy( szTempFile, "./Server_Dump.txt");

	FILE* fp = _tfopen(szTempFile, _T("w"));
	if(!fp) return FALSE;

	Get_Exception_Info(pException, fp, dwLastError);

	fclose(fp); fp = NULL;

	::GetShortPathName(szTempFile, szBigFile, MAX_PATH);
	if(szBigFile[0] == 0) return FALSE;

	return TRUE;
}


VOID tProcessException(PEXCEPTION_POINTERS pException) throw()
{
	//保存最后的错误代码
	UINT	dwLastError = ::GetLastError();
	if(!pException) return;

	//生成完整表述文件
	CHAR szBigInfoFile[MAX_PATH] = {0};
	if(!CreateBigInfoFile(pException, szBigInfoFile, dwLastError))
	{
		return;
	}

	//生成dump报告
	CHAR szDumpFile[MAX_PATH] = {0};
	//CreateDumpHelpFile(pException, szDumpFile);
}

#endif


uint g_QuitThreadCount = 0 ;
MyLock g_thread_lock ;

////////////////////////////////////////////////////////////////////////////////
//
// constructor
//
////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////
Thread::Thread ( ) 
{
__ENTER_FUNCTION

	m_TID		= 0 ;
	m_Status	= Thread::READY ;

#if defined(__WINDOWS__)
	m_hThread = NULL ;
#endif

__LEAVE_FUNCTION
}


////////////////////////////////////////////////////////////////////////////////
//
// destructor (virtual)
//
////////////////////////////////////////////////////////////////////////////////
Thread::~Thread () 
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}


////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////
VOID Thread::start () 
{ 
__ENTER_FUNCTION
		
	if ( m_Status != Thread::READY )
		return ;

#if defined(__LINUX__)
	pthread_create( &m_TID, NULL , MyThreadProcess , this );
#elif defined(__WINDOWS__)
	m_hThread = ::CreateThread( NULL, 0, MyThreadProcess , this, 0, &m_TID ) ;
#endif

__LEAVE_FUNCTION
}


////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////
VOID Thread::stop ()
{
	__ENTER_FUNCTION
		
		
	__LEAVE_FUNCTION
}


////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////
VOID Thread::exit( VOID * retval )
{
	_MY_TRY
	{
		#if defined(__LINUX__)
			pthread_exit( retval );
		#elif defined(__WINDOWS__)
			::CloseHandle( m_hThread ) ;
		#endif
	}
	_MY_CATCH
	{

	}
}


////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////
#if defined(__LINUX__)
VOID * MyThreadProcess ( VOID * derivedThread )
{
__ENTER_FUNCTION

	Thread * thread = (Thread *)derivedThread;
	if( thread==NULL )
		return NULL;
	
	// set thread's status to "RUNNING"
	thread->setStatus(Thread::RUNNING);

	// here - polymorphism used. (derived::run() called.)
	thread->run();
	
	// set thread's status to "EXIT"
	thread->setStatus(Thread::EXIT);
	
	//INT ret = 0;
	//thread->exit(&ret);

	g_thread_lock.Lock() ;
	g_QuitThreadCount++ ;
	g_thread_lock.Unlock() ;

__LEAVE_FUNCTION

	return NULL;	// avoid compiler's warning
}
#elif defined (__WINDOWS__)

DWORD WINAPI MyThreadProcess(  VOID* derivedThread )
{
//__ENTER_FUNCTION

	__try
	{
		Thread * thread = (Thread *)derivedThread;
		if( thread==NULL )
			return 0;
		
		// set thread's status to "RUNNING"
		thread->setStatus(Thread::RUNNING);

		// here - polymorphism used. (derived::run() called.)
		thread->run();
		
		// set thread's status to "EXIT"
		thread->setStatus(Thread::EXIT);

		thread->exit(NULL);

		g_thread_lock.Lock() ;
		g_QuitThreadCount++ ;
		g_thread_lock.Unlock() ;
	}
	__except (tProcessException(GetExceptionInformation()), EXCEPTION_EXECUTE_HANDLER)
	{}

//__LEAVE_FUNCTION

	return 0;	// avoid compiler's warning
}
#endif

VOID Thread::run( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

