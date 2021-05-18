#pragma once

#include "TDBasicType.h"
#include "TDClass.h"

class tNode
{
public:
	tNode(VOID);
	tNode(LPCTSTR szName);
	virtual ~tNode() { Release(); }

	//返回子节点数目
	virtual INT							GetChildNum(VOID) const	{ return (INT)m_ChildBuf.size(); }
	//返回节点名
	virtual const STRING&				GetNodeName(VOID) const	{ return m_strName; }
	//返回子节点列表
	virtual std::list< tNode* >&		GetChildBuf(VOID)		{ return m_ChildBuf; }
	//返回父节点
	virtual tNode*						GetParent(VOID)			{ return m_pParentNode; }
	//节点初始化
	virtual VOID						Initial(VOID*)			{ }
	//逻辑轮循函数
	virtual VOID						Tick(VOID);

	//查找子节点,如果没有找到,返回NULL;
	virtual tNode*	LookUpChild(LPCTSTR szNodeName);

	//添加子节点到该节点上
	virtual VOID AddChild(tNode *pNewNode);

	//删除某个子节点,当该节点存在时返回TRUE,否则返回FALSE
	virtual BOOL EraseChild(tNode* pNode);

	//删除所有子节点
	virtual VOID EraseAllChild( VOID );

	//释放自己所所拥有的资源
	virtual VOID Release(VOID);

protected:
	typedef std::list< tNode * > ChildList;

	//节点名
	STRING				m_strName;
	//子节点列表
	ChildList			m_ChildBuf;
	//父节点指针
	tNode*				m_pParentNode;

	friend struct tClass;
	WX_DECLARE_DYNAMIC(tNode);
};