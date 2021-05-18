#include "StdAfx.h"
#include "TDCrashReport.h"
#include <tlhelp32.h>
#include <time.h>
#include <dbghelp.h>
#include "TDException.h"
#include <stdio.h>
#include "..\WXClient\Global.h"
#include "..\WXClient\Resource.h"


#define	DUMP_SIZE_MAX	8000	//max size of our dump
#define	CALL_TRACE_MAX	((DUMP_SIZE_MAX - 2000) / (MAX_PATH + 40))	//max number of traced calls
#define	NL				_T("\n")	//new line


//寻找脚本错误模式
extern BOOL g_IsFindLuaError = FALSE;

//--------------------------------------------------------------
//自动加载dbghelp.dll
typedef	BOOL (WINAPI * MINIDUMP_WRITE_DUMP)(
	IN HANDLE			hProcess,
	IN UINT			ProcessId,
	IN HANDLE			hFile,
	IN MINIDUMP_TYPE	DumpType,
	IN CONST PMINIDUMP_EXCEPTION_INFORMATION	ExceptionParam, OPTIONAL
	IN PMINIDUMP_USER_STREAM_INFORMATION		UserStreamParam, OPTIONAL
	IN PMINIDUMP_CALLBACK_INFORMATION			CallbackParam OPTIONAL
	);

struct tAUTO_LOAD_DBGHELP
{
	tAUTO_LOAD_DBGHELP();
	~tAUTO_LOAD_DBGHELP();
	
	HMODULE					m_hDbgHelp;
	MINIDUMP_WRITE_DUMP		m_pfnMiniDumpWriteDump;
} theDbgHelper;

tAUTO_LOAD_DBGHELP::tAUTO_LOAD_DBGHELP()
{
	m_hDbgHelp = ::LoadLibrary(_T("DBGHELP.DLL"));
	if(NULL == theDbgHelper.m_hDbgHelp) return; //Load failed!
	m_pfnMiniDumpWriteDump = (MINIDUMP_WRITE_DUMP)GetProcAddress(m_hDbgHelp, (LPCSTR)("MiniDumpWriteDump"));
	if(NULL == m_pfnMiniDumpWriteDump) 
	{
		::FreeLibrary(m_hDbgHelp);
		m_hDbgHelp = NULL;
	}
}

tAUTO_LOAD_DBGHELP::~tAUTO_LOAD_DBGHELP()
{
	if(m_hDbgHelp)
	{
		::FreeLibrary(m_hDbgHelp);
		m_hDbgHelp= NULL;
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
} //Get_Module_By_Ret_Addr

//******************************************************************
VOID Get_Call_Stack(PEXCEPTION_POINTERS pException, FILE* fp)
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

} //Get_Call_Stack

//***********************************
VOID WINAPI Get_Version_Str(FILE* fp)
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
			const STRING* pCPPException = (const STRING*)E.ExceptionInformation[0];
			if(!pCPPException) return;

			_ftprintf(fp,
				_T("C++ Exception\n")
				_T("\n")
				_T("Expr:      %s\n"),

				pCPPException->c_str());
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
VOID Get_Exception_Info(PEXCEPTION_POINTERS pException, FILE* fp, UINT dwLastError)
//*************************************************************
// Allocate Str[DUMP_SIZE_MAX] and return Str with dump, if !pException - just return call stack in Str.
{
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
		
		//_ftprintf(fp, _T("------------------------------------------------------------------------------")NL);

		// Save instruction that caused exception.
		//if(E.ExceptionAddress)
		//{
		//	_ftprintf(fp, _T("Instruction: ")NL);
		//	for (i = 0; i < 16; i++)
		//		_ftprintf(fp, _T(" %02X"), PBYTE(E.ExceptionAddress)[i]);
		//}
		// Save registers at exception.
		//_ftprintf(fp, NL _T("Registers:") NL);
		//_ftprintf(fp, _T("EAX: %08X  EBX: %08X  ECX: %08X  EDX: %08X") NL, C.Eax, C.Ebx, C.Ecx, C.Edx);
		//_ftprintf(fp, _T("ESI: %08X  EDI: %08X  ESP: %08X  EBP: %08X")NL, C.Esi, C.Edi, C.Esp, C.Ebp);
		//_ftprintf(fp, _T("EIP: %08X  EFlags: %08X")NL, C.Eip, C.EFlags);
	} //if (pException)
	
	_ftprintf(fp, _T("------------------------------------------------------------------------------")NL);

	// Save call stack info.
	_ftprintf(fp, _T("Call Stack:")NL);
	Get_Call_Stack(pException, fp);
} //Get_Exception_Info


BOOL SaveCurrentlyLog(VOID* TheLog, size_t LogSize)
{
	TCHAR szTempFile[MAX_PATH] = {0};
	if (NULL == ::GetModuleFileName(g_hInstance,szTempFile,MAX_PATH))
	{
		memset(szTempFile,0,MAX_PATH);
		memcpy(szTempFile, "C:\\WXC_SML.TXT", strlen("C:\\WXC_SML.TXT"));
	}
	else
	{
		if(TRUE == ::PathRemoveFileSpec(szTempFile))
		{
			strcat(szTempFile, "\\WXC_SML.TXT");
		}
		else
		{
			memset(szTempFile,0,MAX_PATH);
			memcpy(szTempFile, "C:\\WXC_SML.TXT", strlen("C:\\WXC_SML.TXT"));
		}
	}

	FILE* fp = _tfopen(szTempFile, _T("at"));
	if(!fp) return FALSE;

	fwrite("\n操作描述为:", strlen("\n操作描述为:"), 1, fp);
	fwrite(TheLog, LogSize, 1, fp);
	SYSTEMTIME TNow;
	memset(&TNow,0,sizeof(TNow));
	::GetLocalTime(&TNow);
	CHAR strtime[100]={0};
	memset(strtime, 0, 100);
	sprintf(strtime,"\nTime:%d年%d月%d日%d时%d分%d秒\n",TNow.wYear,TNow.wMonth,TNow.wDay,TNow.wHour,TNow.wMinute,TNow.wSecond);
	fwrite(strtime, strlen(strtime), 1, fp);
	fwrite("\n\n\n------------------------------------------------------\n", strlen("\n\n\n------------------------------------------------------\n"), 1, fp);

	fclose(fp); fp = NULL;
	return TRUE;
}

INT_PTR CALLBACK SML_DialogProc(
								HWND hwndDlg,
								UINT uMsg,
								WPARAM wParam,
								LPARAM lParam
								)
{
	switch(uMsg)
	{
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDOK:
				{
					CHAR t_strErrorInfo[MAX_ERROR_INFO];
					memset(t_strErrorInfo, 0, MAX_ERROR_INFO);
					GetDlgItemText(hwndDlg, IDC_EDIT_WXC_SML, t_strErrorInfo, MAX_ERROR_INFO);
					SaveCurrentlyLog(t_strErrorInfo, strlen(t_strErrorInfo));
					::DestroyWindow(hwndDlg);
				}
				break;
			case IDCANCEL:
				{
					::DestroyWindow(hwndDlg);
				}
			}
		}
		break;
	}
	return 0;
}
BOOL CreateSmallDumpInfo(PEXCEPTION_POINTERS pException, PTCHAR szSmallFile, UINT dwLastError)
{
	if (!pException) return FALSE;

	//创建基本描述文件
	TCHAR szTempFile[MAX_PATH] = {0};
	if (NULL == ::GetModuleFileName(g_hInstance, szTempFile,MAX_PATH))
	{
		memset(szTempFile,0,MAX_PATH);
		memcpy(szTempFile, "C:\\WXC_SML.TXT", strlen("C:\\WXC_SML.TXT"));
	}
	else
	{
		if(TRUE == ::PathRemoveFileSpec(szTempFile))
		{
			strcat(szTempFile, "\\WXC_SML.TXT");
		}
		else
		{
			memset(szTempFile,0,MAX_PATH);
			memcpy(szTempFile, "C:\\WXC_SML.TXT", strlen("C:\\WXC_SML.TXT"));
		}
	}

	//::GetTempPath(MAX_PATH, szTempDir);

	//TCHAR szTempFile[MAX_PATH] = {0};
	//::GetTempFileName(szTempDir, _T("sml"), MAX_PATH, szTempFile);

	FILE* fp = _tfopen(szTempFile, _T("a+"));
	if(!fp) return FALSE;

	CreateExceptionDesc( pException, fp, dwLastError);

	fclose(fp); fp = NULL;

	if(g_IsFindLuaError)//如果是找异常模式
	{
		HWND HDialog = ::CreateDialog(g_hInstance, MAKEINTRESOURCE(IDD_DIALOG_WXC_SML), g_hMainWnd, SML_DialogProc);
		::ShowWindow(HDialog, SW_SHOW);
	}
	::GetShortPathName(szTempFile, szSmallFile, MAX_PATH);
	if(szSmallFile[0] == 0) return FALSE;

	return TRUE;
}

BOOL CreateBigInfoFile(PEXCEPTION_POINTERS pException, PTCHAR szBigFile, UINT dwLastError)
{
	if (!pException) return FALSE;

	TCHAR szTempDir[MAX_PATH] = {0};
	::GetTempPath(MAX_PATH, szTempDir);

	TCHAR szTempFile[MAX_PATH] = {0};
	::GetTempFileName(szTempDir, _T("dtl"), MAX_PATH, szTempFile);

	FILE* fp = _tfopen(szTempFile, _T("w"));
	if(!fp) return FALSE;

	Get_Exception_Info(pException, fp, dwLastError);

	fclose(fp); fp = NULL;

	::GetShortPathName(szTempFile, szBigFile, MAX_PATH);
	if(szBigFile[0] == 0) return FALSE;

	return TRUE;
}
//创建DBGHLEP所需要的dmp信息
BOOL CreateDumpHelpFile(PEXCEPTION_POINTERS pException, PTCHAR szDumpFile)
{
	szDumpFile[0] = ' ';
	szDumpFile[1] = 0;

	// If MiniDumpWriteDump() of DbgHelp.dll available.
	if (!theDbgHelper.m_pfnMiniDumpWriteDump) return FALSE;

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

	theDbgHelper.m_pfnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDump_File,
		MiniDumpNormal, (pException) ? &M : NULL, NULL, NULL);

	CloseHandle(hDump_File);

	::GetShortPathName(szTempFile, szDumpFile, MAX_PATH);
	if(szDumpFile[0] == 0) 
	{
		return FALSE;
	}

	return TRUE;
}

