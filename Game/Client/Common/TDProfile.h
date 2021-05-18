/****************************************\
*										*
* 			效率检测器					*
*										*
\****************************************/

#pragma once
#include "TDBasicType.h"
#include <hash_set>
#include <hash_map>



/////////////////////////////////////////////////////////////
//使用宏
// 
//		PROFILE_PUSH(name1);
//			PROFILE_PUSH(name2)
//			PROFILE_POP();
//		PROFILE_POP();


//宏 在堆中Push节点
//#define PROFILE_PUSH(name) { \
//	TDProfile_PushNode(#name, NULL); }
//
//#define PROFILE_PUSHEX(name, nameEx) {\
//	TDProfile_PushNode(#name, nameEx); }

//宏 从堆中Pop节点
//#define PROFILE_POP(name) { \
//TDProfile_PopNode(#name); }

/////////////////////////////////////////////////////////////


struct RPOFILE_TREE_NODE;
//堆栈中的节点(用于Push/Pop)
struct /*TDENGDA_API*/ PROFILE_STACK_NODE
{
	//名称
	STRING				strName;
	//开始时间
	LARGE_INTEGER		nStartTime;
	//在树形节点上的指针
	RPOFILE_TREE_NODE*	pTreeNode;

	PROFILE_STACK_NODE(LPCTSTR szName=NULL, LPCTSTR szAttName=NULL);
};

//堆栈
struct PROFILE_STACK
{
	enum 
	{ 
		MAX_STATCK_NUM = 256,
	};

	//节点堆(MAX_STATCK_NUM)
	PROFILE_STACK_NODE 	vNodeStack[MAX_STATCK_NUM];
	//栈顶指针,初始0
	INT					nTopIndex;
	//当前栈中工作的节点名称的Hash集合

#ifdef __SGI_STL_PORT
	typedef std::hash_set< STRING > HASH_NAMESET;
#else
	typedef stdext::hash_set< STRING > HASH_NAMESET;
#endif

	HASH_NAMESET  vHashName;
};

//------------------------------

//树形节点(用于统计)
struct RPOFILE_TREE_NODE
{
#ifdef __SGI_STL_PORT
	typedef std::hash_map< STRING, RPOFILE_TREE_NODE* > HASH_TREENODE;
#else
	typedef stdext::hash_map< STRING, RPOFILE_TREE_NODE* > HASH_TREENODE;
#endif
	//名称
	STRING				strName;
	//运行次数
	UINT				nInvokeCount;
	//子节点
	HASH_TREENODE		mapChild;
	//父节点
	RPOFILE_TREE_NODE*	pParentNode;
	//占用的时间
	LARGE_INTEGER		nTotalTime;

	~RPOFILE_TREE_NODE();
};

//节点树
struct PROFILE_TREE
{
	//根节点
	RPOFILE_TREE_NODE	theRoot;
	//当前节点
	RPOFILE_TREE_NODE*	pCurrentNode;
};

//查询节点树函数指针
typedef VOID (__stdcall* FUNC_PROFILE_DUMP)(const PROFILE_TREE* pProfileNodeTree);

//extern "C"
//{
//	//在堆中Push节点
//	VOID	/*TDENGDA_API*/ TDProfile_PushNode(LPCTSTR name, LPCTSTR nameEx = NULL);
//	//从堆中Pop节点
//	VOID	/*TDENGDA_API*/ TDProfile_PopNode(LPCTSTR name=NULL);
//	//请求查询Profile节点信息(当堆栈空时会调用传入的函数指针)
//	VOID	/*TDENGDA_API*/ TDProfile_DumpSruct(FUNC_PROFILE_DUMP func);
//}

//由时间管理器调用!!!
VOID	TDProfile_Tick(VOID);

