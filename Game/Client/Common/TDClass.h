#pragma once

#include "TDBasicType.h"
/****************************************\
*										*
*			  节点类定义类				*
*										*
\****************************************/
class tNode;
struct /*TDENGDA_API*/ tClass
{
	LPCTSTR							szClassName;					//!< Node类名称
	INT								nObjectSize;					//!< Node类的大小
	tNode* (__stdcall*				pfnCreateObject)();				//!< 函数指针，用于生成一个INode类实例
	tClass*							pBaseClass;						//!< 所继承的基类
	tClass*							pNextClass;						//!< 下一个类
	tNode*							CreateObject(LPCTSTR szName);	//!< 用于生成一个实例的函数
};

//根据类名取得定义类
#define GETCLASS(className)  (&className::m_class##className)

//类定义声明宏
#define WX_DECLARE_DYNAMIC(className) \
public: \
	static tClass				m_class##className; \
	virtual const tClass*		GetClass(VOID) const; \
	static tNode* __stdcall	CreateObject();

//类定义实现宏
#define WX_IMPLEMENT_DYNAMIC(className, baseClass) \
	static TCHAR sz##className[] = _T(#className); \
	tClass className::m_class##className = \
	{ sz##className, sizeof(className), className::CreateObject, baseClass, NULL }; \
	const tClass* className::GetClass() const \
		{ return &className::m_class##className; } \
	tNode* className::CreateObject() \
		{ return new className; } 


//纯虚类类定义实现宏
#define WX_IMPLEMENT_DYNAMIC_VIRTUAL(className, baseClass) \
	static TCHAR sz##className[] = _T(#className); \
	tClass className::m_class##className = \
	{ sz##className, sizeof(className), className::CreateObject, baseClass, NULL }; \
	const tClass* className::GetClass() const \
		{ return &className::m_class##className; } \
	tNode* className::CreateObject() \
		{ TDThrow(_T("Pure virtual class")); return NULL; } 
