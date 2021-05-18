#include "StdAfx.h"
#include "TDNode.h"

WX_IMPLEMENT_DYNAMIC(tNode, NULL);
tNode::tNode(VOID)
	:m_pParentNode(NULL)
{
}

tNode::tNode(LPCTSTR szName)
{ 
	m_strName = szName;	
}

//查找子节点,如果没有找到,返回NULL;
tNode*	tNode::LookUpChild(LPCTSTR szNodeName)
{
	register std::list< tNode * >::iterator it;
	for(it = m_ChildBuf.begin(); it!=m_ChildBuf.end(); it++)
	{
		if((*it)->GetNodeName() == szNodeName) return (tNode*)*it;
	}
	return NULL;
}

//添加子节点到该节点上
VOID tNode::AddChild(tNode *pNewNode)
{
	if(!pNewNode) return;
	(pNewNode->m_pParentNode) = this;
	m_ChildBuf.push_back(pNewNode);
}

//删除某个子节点,当该节点存在时返回TRUE,否则返回FALSE
BOOL tNode::EraseChild(tNode* pNode)
{
	if(GetChildBuf().empty()) return FALSE;
	register std::list< tNode * >::iterator it;
	for(it = m_ChildBuf.begin(); it!=m_ChildBuf.end(); it++)
	{
		if(pNode == *it)
		{
			m_ChildBuf.erase(it);
			return TRUE;
		}
	}
	return FALSE;
}

//删除所有子节点
VOID tNode::EraseAllChild( VOID )
{
	if(GetChildBuf().empty()) return ;
	m_ChildBuf.clear( );
}

//释放自己所所拥有的资源
VOID tNode::Release(VOID)
{
	if(GetChildBuf().empty()) return;
	//释放所有子节点
	register std::list< tNode * >::iterator it;
	for(it = m_ChildBuf.begin(); it!=m_ChildBuf.end(); it++)
	{
		(*it)->Release();
		delete (*it);
		*it = NULL;
	}
	m_ChildBuf.clear();
}

VOID tNode::Tick(VOID)
{
	if(GetChildBuf().empty()) return;

	register std::list< tNode* >::iterator it=GetChildBuf().end(), itNext;
	it--;
	
	do
	{
		itNext = it;
		if(itNext == GetChildBuf().begin()) itNext = GetChildBuf().end();
		else itNext--;

		((tNode*)(*it))->Tick();

		if(itNext == GetChildBuf().end()) break;

		it = itNext;
	}while(true);
}