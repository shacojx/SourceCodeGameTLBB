#include "StdAfx.h"
#include "Crash.h"
#include <tlhelp32.h>
#include <time.h>
#include "..\Global.h"
#include "TDException.h"

// In case you don't have dbghelp.h.
#ifndef _DBGHELP_

typedef struct _MINIDUMP_EXCEPTION_INFORMATION {
	DWORD	ThreadId;
	PEXCEPTION_POINTERS	ExceptionPointers;
	BOOL	ClientPointers;
} MINIDUMP_EXCEPTION_INFORMATION, *PMINIDUMP_EXCEPTION_INFORMATION;

typedef enum _MINIDUMP_TYPE {
	MiniDumpNormal =			0x00000000,
	MiniDumpWithDataSegs =		0x00000001,
} MINIDUMP_TYPE;

typedef	BOOL (WINAPI * MINIDUMP_WRITE_DUMP)(
	IN HANDLE			hProcess,
	IN DWORD			ProcessId,
	IN HANDLE			hFile,
	IN MINIDUMP_TYPE	DumpType,
	IN CONST PMINIDUMP_EXCEPTION_INFORMATION	ExceptionParam, OPTIONAL
	IN PVOID									UserStreamParam, OPTIONAL
	IN PVOID									CallbackParam OPTIONAL
	);

#else

typedef	BOOL (WINAPI * MINIDUMP_WRITE_DUMP)(
	IN HANDLE			hProcess,
	IN DWORD			ProcessId,
	IN HANDLE			hFile,
	IN MINIDUMP_TYPE	DumpType,
	IN CONST PMINIDUMP_EXCEPTION_INFORMATION	ExceptionParam, OPTIONAL
	IN PMINIDUMP_USER_STREAM_INFORMATION		UserStreamParam, OPTIONAL
	IN PMINIDUMP_CALLBACK_INFORMATION			CallbackParam OPTIONAL
	);
#endif

#define	DUMP_SIZE_MAX	8000	//max size of our dump
#define	CALL_TRACE_MAX	((DUMP_SIZE_MAX - 2000) / (MAX_PATH + 40))	//max number of traced calls
#define	NL				_T("\n")	//new line

//=========================================================
//
//异常调试自动加载
//
//=========================================================

struct DBG_HELP_AUTOLOAD
{
	DBG_HELP_AUTOLOAD();
	~DBG_HELP_AUTOLOAD();
	
	// for DBGHELP.DLL
	HMODULE					hDbgHelp;
	MINIDUMP_WRITE_DUMP		pfnMiniDumpWriteDump;
} theDbgHelper;

DBG_HELP_AUTOLOAD::DBG_HELP_AUTOLOAD()
{
	//--------------------------------------------------------
	//加载微软调试模块dbghelp.dll
	this->hDbgHelp = ::LoadLibrary(_T("DBGHELP.DLL"));
	if(NULL == theDbgHelper.hDbgHelp) return;
	pfnMiniDumpWriteDump = (MINIDUMP_WRITE_DUMP)GetProcAddress(this->hDbgHelp, (LPCSTR)("MiniDumpWriteDump"));
	if(NULL == pfnMiniDumpWriteDump) 
	{
		::FreeLibrary(this->hDbgHelp);
		this->hDbgHelp = NULL;
		return;
	}
}

DBG_HELP_AUTOLOAD::~DBG_HELP_AUTOLOAD()
{
	if(this->hDbgHelp)
	{
		::FreeLibrary(this->hDbgHelp);
		this->hDbgHelp = NULL;
	}
}

//****************************************************************************************
BOOL WINAPI Get_Module_By_Ret_Addr(PBYTE Ret_Addr, PTCHAR Module_Name, PBYTE & Module_Addr)
//****************************************************************************************
// Find module by Ret_Addr (address in the module).
// Return Module_Name (full path) and Module_Addr (start address).
// Return TRUE if found.
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
			if (DWORD(Ret_Addr - M.modBaseAddr) < M.modBaseSize)
			{
				lstrcpyn(Module_Name, M.szExePath, MAX_PATH);
				Module_Addr = M.modBaseAddr;
				break;
			}
		} while (::Module32Next(hSnapshot, &M));
	}

	::CloseHandle(hSnapshot);

	return !!Module_Name[0];
} //Get_Module_By_Ret_Addr

//******************************************************************
void Get_Call_Stack(PEXCEPTION_POINTERS pException, FILE* fp)
//******************************************************************
// Fill Str with call stack info.
// pException can be either GetExceptionInformation() or NULL.
// If pException = NULL - get current call stack.
{
	TCHAR	Module_Name[MAX_PATH];
	PBYTE	Module_Addr = 0;
	PBYTE	Module_Addr_1;
	
#pragma warning(disable: 4200)	//nonstandard extension used : zero-sized array in struct/union
	typedef struct STACK
	{
		STACK *	Ebp;
		PBYTE	Ret_Addr;
		DWORD	Param[0];
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
	for (int Ret_Addr_I = 0;
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
			else if (!IsBadReadPtr(Ebp, sizeof(PSTACK) + 5 * sizeof(DWORD)))
			{
				_ftprintf(fp, _T("  (%X, %X, %X, %X, %X)") NL,
					Ebp->Param[0], Ebp->Param[1], Ebp->Param[2], Ebp->Param[3], Ebp->Param[4]);
			}
		}
		else
			_ftprintf(fp, _T("%08X")NL, (LONG_PTR)(Ebp->Ret_Addr));
	}

} //Get_Call_Stack

//***********************************
void WINAPI Get_Version_Str(FILE* fp)
//***********************************
// Fill Str with Windows version.
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
		_T("==============================================================================") NL
		_T("Windows:  %d.%d.%d, SP %d.%d, Product Type %d") NL	//SP - service pack, Product Type - VER_NT_WORKSTATION,...
		NL
		_T("Time:     %s %s")NL,
		V.dwMajorVersion, V.dwMinorVersion, V.dwBuildNumber, V.wServicePackMajor, V.wServicePackMinor, V.wProductType, 
		dateBuf, timeBuf);
} //Get_Version_Str


void CreateExceptionDesc(PEXCEPTION_POINTERS pException, FILE* fp, DWORD dwLastError)
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
	//核心软异常
	case 0XE000C0DE:
		{
			tException* pException = (tException*)E.ExceptionInformation[0];
			if(!pException) return;

			//断言错误
			if(pException->m_bAssert) 
			{
				_ftprintf(fp,
					_T("\t\tAssert  \n")
					_T("\n")
					_T("File:      %s\n")
					_T("Line:      %d\n")
					_T("Expr:      %s\n")
					_T("LastError: 0X%08X\n"),

					pException->m_strFileName.c_str(), 
					pException->m_nLineNum, 
					pException->m_strMessage.c_str(),
					pException->m_dwLastError);
			}
			else
			{
				_ftprintf(fp,
					_T("\t\tGeneral  \n")
					_T("\n")
					_T("Expr:      %s\n")
					_T("LastError: 0x%08X\n"),

					pException->m_strMessage.c_str(),
					pException->m_dwLastError);
			}
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

//*************************************************************
void Get_Exception_Info(PEXCEPTION_POINTERS pException, FILE* fp, DWORD dwLastError)
//*************************************************************
// Allocate Str[DUMP_SIZE_MAX] and return Str with dump, if !pException - just return call stack in Str.
{
	int			i;
	TCHAR		Module_Name[MAX_PATH];
	PBYTE		Module_Addr;
	HANDLE		hFile;
	FILETIME	Last_Write_Time;
	FILETIME	Local_File_Time;
	SYSTEMTIME	T;
	
	Get_Version_Str(fp);

	_ftprintf(fp, _T("------------------------------------------------------------------------------")NL);
	_ftprintf(fp, _T("Process:  ") );

	GetModuleFileName(NULL, Module_Name, MAX_PATH);
	_ftprintf(fp, _T("%s") NL, Module_Name);

	// If exception occurred.
	if (pException)
	{
		EXCEPTION_RECORD &	E = *pException->ExceptionRecord;
		CONTEXT &			C = *pException->ContextRecord;

		// If module with E.ExceptionAddress found - save its path and date.
		if (Get_Module_By_Ret_Addr((PBYTE)E.ExceptionAddress, Module_Name, Module_Addr))
		{
			_ftprintf(fp, _T("Module:   %s") NL, Module_Name);

			if ((hFile = CreateFile(Module_Name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL, NULL)) != INVALID_HANDLE_VALUE)
			{
				if (GetFileTime(hFile, NULL, NULL, &Last_Write_Time))
				{
					FileTimeToLocalFileTime(&Last_Write_Time, &Local_File_Time);
					FileTimeToSystemTime(&Local_File_Time, &T);

					_ftprintf(fp, _T("Date Modified:  %02d/%02d/%d") NL, 
						T.wMonth, T.wDay, T.wYear);
				}
				CloseHandle(hFile);
			}
		}
		else
		{
			_ftprintf(fp, _T("Exception Addr:  %08X") NL , (LONG_PTR)(E.ExceptionAddress));
		}
		
		_ftprintf(fp, _T("------------------------------------------------------------------------------")NL);

		//加入具体异常解释信息
		CreateExceptionDesc(pException, fp, dwLastError);
		
		_ftprintf(fp, _T("------------------------------------------------------------------------------")NL);

		// Save instruction that caused exception.
		if(E.ExceptionAddress)
		{
			_ftprintf(fp, _T("Instruction: ")NL);
			for (i = 0; i < 16; i++)
				_ftprintf(fp, _T(" %02X"), PBYTE(E.ExceptionAddress)[i]);
		}
		// Save registers at exception.
		_ftprintf(fp, NL _T("Registers:") NL);
		_ftprintf(fp, _T("EAX: %08X  EBX: %08X  ECX: %08X  EDX: %08X") NL, C.Eax, C.Ebx, C.Ecx, C.Edx);
		_ftprintf(fp, _T("ESI: %08X  EDI: %08X  ESP: %08X  EBP: %08X")NL, C.Esi, C.Edi, C.Esp, C.Ebp);
		_ftprintf(fp, _T("EIP: %08X  EFlags: %08X")NL, C.Eip, C.EFlags);
	} //if (pException)
	
	_ftprintf(fp, _T("------------------------------------------------------------------------------")NL);

	// Save call stack info.
	_ftprintf(fp, _T("Call Stack:")NL);
	Get_Call_Stack(pException, fp);
} //Get_Exception_Info

bool CreateSmallDumpInfo(PEXCEPTION_POINTERS pException, PTCHAR szSmallFile, DWORD dwLastError)
{
	if (!pException) return false;

	//创建基本描述文件
	TCHAR szTempDir[MAX_PATH] = {0};
	::GetTempPath(MAX_PATH, szTempDir);

	TCHAR szTempFile[MAX_PATH] = {0};
	::GetTempFileName(szTempDir, _T("sml"), MAX_PATH, szTempFile);

	FILE* fp = _tfopen(szTempFile, _T("w"));
	if(!fp) return false;

	CreateExceptionDesc( pException, fp, dwLastError);

	fclose(fp); fp = NULL;

	::GetShortPathName(szTempFile, szSmallFile, MAX_PATH);
	if(szSmallFile[0] == 0) return false;

	return true;
}

bool CreateBigInfoFile(PEXCEPTION_POINTERS pException, PTCHAR szBigFile, DWORD dwLastError)
{
	if (!pException) return false;

	TCHAR szTempDir[MAX_PATH] = {0};
	::GetTempPath(MAX_PATH, szTempDir);

	TCHAR szTempFile[MAX_PATH] = {0};
	::GetTempFileName(szTempDir, _T("dtl"), MAX_PATH, szTempFile);

	FILE* fp = _tfopen(szTempFile, _T("w"));
	if(!fp) return false;

	Get_Exception_Info(pException, fp, dwLastError);

	fclose(fp); fp = NULL;

	::GetShortPathName(szTempFile, szBigFile, MAX_PATH);
	if(szBigFile[0] == 0) return false;

	return true;
}

bool CreateDumpFile(PEXCEPTION_POINTERS pException, PTCHAR szDumpFile)
{
	szDumpFile[0] = ' ';
	szDumpFile[1] = 0;

	// If MiniDumpWriteDump() of DbgHelp.dll available.
	if (!theDbgHelper.pfnMiniDumpWriteDump) return false;

	TCHAR szTempDir[MAX_PATH] = {0};
	::GetTempPath(MAX_PATH, szTempDir);

	TCHAR szTempFile[MAX_PATH] = {0};
	::GetTempFileName(szTempDir, _T("dmp"), MAX_PATH, szTempFile);

	MINIDUMP_EXCEPTION_INFORMATION	M;

	M.ThreadId = GetCurrentThreadId();
	M.ExceptionPointers = pException;
	M.ClientPointers = 0;

	HANDLE hDump_File = CreateFile(szTempFile,
		GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	theDbgHelper.pfnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDump_File,
		MiniDumpNormal, (pException) ? &M : NULL, NULL, NULL);

	CloseHandle(hDump_File);

	::GetShortPathName(szTempFile, szDumpFile, MAX_PATH);
	if(szDumpFile[0] == 0) 
	{
		return false;
	}

	return true;
}

//*************************************************************************************
void WINAPI TDProcessDump(PEXCEPTION_POINTERS pException)
{

} //Create_Dump


void Catch_SEH_Exception(DWORD nSeCode, PEXCEPTION_POINTERS pException)
{
	g_theKernel.SetExceptionMode(true);

	//保存最后的错误代码
	DWORD	dwLastError = ::GetLastError();
	if(!pException) return;

	//生成基本描述文件
	TCHAR szSmallInfoFile[MAX_PATH] = {0};
	if(!CreateSmallDumpInfo(pException, szSmallInfoFile, dwLastError))
	{
		return;
	}

	//生成完整表述文件
	TCHAR szBigInfoFile[MAX_PATH] = {0};
	if(!CreateBigInfoFile(pException, szBigInfoFile, dwLastError))
	{
		return;
	}

	TCHAR szDumpFile[MAX_PATH] = {0};
	CreateDumpFile(pException, szDumpFile);

	//寻找CrashReport
	TCHAR szCrashReportFile[MAX_PATH] = {0};
	::GetModuleFileName(NULL, szCrashReportFile, MAX_PATH);
	::PathRemoveFileSpec(szCrashReportFile);
	::PathAppend(szCrashReportFile, _T("CrashReport.exe"));

	TCHAR szCmdLine[1024] = {0};
	_sntprintf(szCmdLine, 1024, _T("%s \"%s\" \"%s\" \"%s\""), 
		szCrashReportFile, 
		szSmallInfoFile, szBigInfoFile, szDumpFile);

	//启动CrashReport
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si,sizeof(STARTUPINFO));
	ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;

	if (::CreateProcess( NULL, szCmdLine, NULL, NULL, FALSE,CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi) == FALSE)
	{
		return;
	}
}