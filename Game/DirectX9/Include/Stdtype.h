#if !defined(__STDTYPE_H__)
#define __STDTYPE_H__

#if !defined(__cplusplus)
#define __cplusplus
#endif//#if !defined(__cplusplus)
#include <windows.h>
#include <windowsx.h>
#include <crtdbg.h>

#if !defined(OMUTIL_API)
#if defined(OMUTIL_EXPORTS)
#define OMUTIL_API __declspec(dllexport)
#else
#define OMUTIL_API __declspec(dllimport)
#endif//#if defined(OMUTIL_API)
#endif//#if !defined(OMUTIL_API)

#ifdef OMUTIL_EXPORTS
#if defined(_DEBUG)
#if !defined(__AFX_H__)	// not use MFC
#if !defined(new)
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif //#if !defined(new)
#endif // #if !define(__AFX_H__)
#endif // #if defined(_DEBUG)
#endif // #ifndef OMUTIL_EXPORTS

#if defined(_WINDOWS_) && defined(_WIN32)
#include <assert.h>
#endif

#if !defined(ASSERT)
#if defined(_DEBUG)
#define ASSERT(x)	_ASSERT(x)
#else
#define ASSERT(x)
#endif//#if defined(_DEBUG)
#endif//#if !defined(ASSERT)

#if !defined(assert)
#if defined(_DEBUG)
#define assert(x)	_ASSERT(x)
#else
#define assert(x)
#endif//#if defined(_DEBUG)
#endif//#if !defined(assert)


#if !defined(VERIFY)
#if defined(_DEBUG)
#define VERIFY(f)          ASSERT(f)
#else
#define VERIFY(f)          ((void)(f))
#endif//#if defined(_DEBUG)
#endif//#if !defined(VERIFY)

#if !defined(OFF)
#define	OFF	0
#endif
#if !defined(ON)
#define	ON	1
#endif
#if !defined(NO)
#define	NO	0
#endif
#if !defined(YES)
#define	YES	1
#endif
#if !defined(NONE)
#define NONE    0
#endif//#if !defined(NONE)

#if !defined(MAX_PATH_DOUBLE)
#define MAX_PATH_DOUBLE (MAX_PATH+MAX_PATH)
#endif//#if !defined(MAX_PATH_DOUBLE)


//----- 来源于 EmTools ------------//
/////////////////////////////////////////////////
#ifndef	FALSE
#define	FALSE	0
#endif

#ifndef	TRUE
#define	TRUE	1
#endif

#ifndef NULL
#define NULL    0
#endif
 

typedef union {
        __int64 i64Val;
        struct {
            unsigned long lowerhalf;
            long upperhalf;
        } twoints;
} HUGEINT;

/////////////////////////////////////////////////


/////////////////////////////////////////////////
#if !defined(_WINDOWS_)

#ifndef	MAKEWORD
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define LOWORD(l)           ((WORD)(l))
#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)           ((BYTE)(w))
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))
#endif

typedef	int		BOOL;
typedef	unsigned char	BYTE;
typedef	unsigned short	WORD;
typedef	unsigned long	DWORD;
typedef	unsigned int	UINT;
typedef	char *			LPSTR;
typedef	const char *	LPCTSTR;

#endif//#if !defined(_WINDOWS_)
/////////////////////////////////////////////////


/////////////////////////////////////////////////
// 定义字符串的缺省大小
#define		_MAX_STRING		128
#define		_MAX_STRING256	256
/////////////////////////////////////////////////


/////////////////////////////////////////////////
typedef void* POSI;
/////////////////////////////////////////////////



//------- disable some waring in warning level 4 ----------//
#pragma warning(disable:4100)// 未引用的形参
#pragma warning(disable:4324)// 结构被填充
#pragma warning(disable:4251)//模板类的dll 接口



#endif//#if !defined(__STDTYPE_H__)
