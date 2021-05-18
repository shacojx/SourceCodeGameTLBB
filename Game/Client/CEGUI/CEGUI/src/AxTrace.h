//////////////////////////////////////////////////////
//
//	AIXA|Trace2 (C) Copyright 開元. 2004
//
//	使用方法:
//		1. 包含 AxTrace.h AxTrace.cpp到工程中,建议将AxTrace.h包含到预编译头StdAfx.h中
//
//		2. 使用 AxTrace 输出调试信息
//
//			void AxTrace(unsigned char nWnd, unsigned char nType, const char *pszFmt, ...);
//					nWnd		-- 接收消息的子窗口编号
//					nType		-- 消息显示类型，可在接收窗口设置显示该消息所用字体
//					pszFmt,...	-- 消息内容
//			例如:
//					AxTrace(0, 1, "nIndex=%d", nIndex);
//
//		3. 使用 AxVar 输出变量监控信息
//			void AxVar(unsigned char nWnd, unsigned char nType, AXVALUE nVariableType, const char *szValueName, void* pValue);
//					nWnd			-- 接收变量监控的子窗口编号
//					nType			-- 消息显示类型，可在接收窗口设置显示该消息所用字体
//					nVariableType	-- 变量类型
//					szValueName		-- 变量类型
//					pValue			-- 指向变量指针
//			例如:
//					AxVar(0, 1, AX_INT32, "nVar", &nVar);
//
//		4. 定义 _AXTRACE_DISABLE_ 可以屏蔽所有的信息输出功能
//		
//////////////////////////////////////////////////////



#ifndef _AXTRACE_INCLUDE_
#define _AXTRACE_INCLUDE_

#pragma once
#define INVALID_TRACE_WND	((unsigned char)-1)

#ifdef _AXTRACE_DISABLE_
	#define AxTrace		__noop
	#define AxVar		__noop
#else
	enum AXVALUE
	{
		AX_INT8,
		AX_UINT8,
		AX_INT16,
		AX_UINT16,
		AX_INT32,
		AX_UINT32,
		AX_INT64,
		AX_UINT64,
		AX_FLOAT32,
		AX_DOUBLE64,
		AX_STRING,
	};

	void AxTrace(unsigned char nWnd, unsigned char nType, const char *pszFmt, ...);
	void AxVar(unsigned char nWnd, unsigned char nType, AXVALUE nVariableType, const char *szValueName, void* pValue);

#endif


#endif