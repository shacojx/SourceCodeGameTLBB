#include "stdafx.h"
/*************************************************************************
文件名	: 	AIScript.cpp
版本号 :	0.0.2
功  能	:	针对特有的扩展AI脚本编写的解析及执行类
修改记录:	修改了前版接口函数扩展性不强的缺点
*************************************************************************/


#include "Obj.h"
#include "Obj_Character.h"
#include "Obj_Monster.h"
#include "Obj_Human.h"
#include "Obj_Pet.h"
#include "AI_Monster.h"
#include "AI_Character.h"
#include "AIScript.h"
#include "AIScriptDef.h"
#include <vector>

AIScript::AIScript()
{
	m_pcdtList = NULL;
	m_pcdtList = NULL;
    m_poptorNodeList = NULL;
	m_pcdtNodeList = NULL;
	m_ptodoNodeList = NULL;
	m_pcdtTree = NULL;
	m_pcdtNode = NULL;
	m_nID = -1;
	m_fX = 0;
	m_fZ = 0;
	//m_SkillID = 0;
	//m_nSummonCount = 3;
	//m_nSpeakID = INVALID_ID;
	//m_SkillErrorCode = OR_OK;

	memset(m_vStateLists, 0, sizeof(ConditionList*) * (ONDEADSECTION+1));
}

AIScript::~AIScript()
{
	ClearCdtList();
}

BOOL AIScript::ParseScriptForPet(const CHAR* filename)
{
	__ENTER_FUNCTION
	m_file.Open(filename);
	INT		line = m_file.GetLines();
	CHAR*	szScript;

	//////////////////////////////////////////////////////////////////////////
	//解析follow部分
	szScript = NULL;
	INT start = m_file.ReturnLineNum("follow");
	INT end = m_file.ReturnLineNum("followend");
	for (INT i=start+1 ; i<end; i++)
	{
		szScript = m_file.ReadOneLine(i+1);
		if (szScript == NULL)
		{
			continue;
		}

		ParseOneScript(szScript);
	}
	InitList(SFOLLOW);
	//////////////////////////////////////////////////////////////////////////
	//解析attack部分
	szScript = NULL;
	start = m_file.ReturnLineNum("attack");
	end = m_file.ReturnLineNum("attackend");
	for (INT i=start+1 ; i<end; i++)
	{
		szScript = m_file.ReadOneLine(i+1);
		if (szScript == NULL)
		{
			continue;
		}

		ParseOneScript(szScript);
	}
	InitList(SATTACK);
	//////////////////////////////////////////////////////////////////////////
	//解析beskill部分
	szScript = NULL;
	start = m_file.ReturnLineNum("beskill");
	end = m_file.ReturnLineNum("beskillend");
	for(INT  i = start+1 ; i < end; i++)
	{
		szScript = m_file.ReadOneLine(i+1);
		if (szScript == NULL)
		{
			continue;
		}

		ParseOneScript(szScript);
	}
	InitList(ONBESKILLSECTION);

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

BOOL AIScript::ParseScript(const CHAR* filename)
{
	__ENTER_FUNCTION
	m_file.Open(filename);
	INT		line = m_file.GetLines();
	CHAR*	szScript;

	m_nID = -1;
	//////////////////////////////////////////////////////////////////////////
	//解析common部分
	szScript = NULL;
	INT start = m_file.ReturnLineNum("common");
	INT end = m_file.ReturnLineNum("commonend");
	for (INT i=start+1 ; i<end; i++)
	{
		szScript = m_file.ReadOneLine(i+1);
		if (szScript == NULL)
		{
			continue;
		}

		ParseOneScript(szScript);
	}
	//处理m_pcdtList，将其分解到各个状态List中，如m_pIdleList,m_pAttackList...
	InitAllStateList();
	//////////////////////////////////////////////////////////////////////////
	//解析skill部分
	szScript = NULL;
	start = m_file.ReturnLineNum("skill");
	end = m_file.ReturnLineNum("skillend");
	for(INT i = start+1 ; i < end; i++)
	{
		szScript = m_file.ReadOneLine(i+1);
		if(szScript == NULL)
		{
			continue;
		}

		ParseOneScript(szScript);
	}
	InitList(SKILLSECTION);
	//////////////////////////////////////////////////////////////////////////
	//解析beskill部分
	szScript = NULL;
	start = m_file.ReturnLineNum("beskill");
	end = m_file.ReturnLineNum("beskillend");
	for(INT i = start+1 ; i < end; i++)
	{
		szScript = m_file.ReadOneLine(i+1);
		if (szScript == NULL)
		{
			continue;
		}

		ParseOneScript(szScript);
	}
	InitList(ONBESKILLSECTION);
	//////////////////////////////////////////////////////////////////////////
	//解析damage部分
	szScript = NULL;
	start = m_file.ReturnLineNum("damage");
	end = m_file.ReturnLineNum("damageend");
	for (INT i = start+1 ; i < end; i++)
	{
		szScript = m_file.ReadOneLine(i+1);
		if(szScript == NULL)
		{
			continue;
		}

		ParseOneScript(szScript);
	}
	InitList(ONDAMAGESECTION);
	//////////////////////////////////////////////////////////////////////////
	//解析dead部分
	szScript = NULL;
	start = m_file.ReturnLineNum("dead");
	end = m_file.ReturnLineNum("deadend");
	for(INT i = start+1 ; i < end; i++)
	{
		szScript = m_file.ReadOneLine(i+1);
		if(szScript == NULL)
		{
			continue;
		}

		ParseOneScript(szScript);
	}
	InitList(ONDEADSECTION);

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

BOOL AIScript::ParseOneScript(const CHAR* szScript)
{
	__ENTER_FUNCTION
	//对每行字符串进行子字符串查找
	CHAR		szCmd[32];
	const CHAR*		pDest;		
	INT j = 0;
	INT prev = 0;
	INT	index = 0;
	INT len = (INT)strlen(szScript);
	memset(szCmd,0, sizeof(szCmd));

	BOOL bToDoFlag = FALSE;

	pDest = strstr(szScript, ":");
	if (pDest && (INT)(pDest-szScript+1) > 0)
	{
		j = (INT)(pDest-szScript+1);
		CHAR szID[8];
		memset(szID, 0, sizeof(szID));
		memcpy(szID, szScript, j-1);
		//m_nID = atol(szID);
		
		if (m_nID++ >= AISCRIPT_NUM)
		{
			Assert(NULL && "id > AISCRIPT_NUM...array overflow...");
			return FALSE;
		}

	}
	else
	{
		//MessageBox(NULL,"Script Error!!!","Error",MB_OK);
		return FALSE;
	}

	for (; j < len; j++)
	{
		switch(ParseCHAR(&szScript[j])) 
		{
		case ANDOROP:
			{//如果是'&','|','(',')'
				Atom atom;
				memcpy(atom.szCHAR, &szScript[j], 1);
				atom.pos = j;
				if (szScript[j] == '|')
				{
					atom.PRI = 0;

					ProcessCtdOrToDoQ(szCmd, bToDoFlag);

					memset(szCmd,0, sizeof(szCmd));
					index = 0;
				}
				else if (szScript[j] == '&')
				{
					atom.PRI = 0;

					ProcessCtdOrToDoQ(szCmd, bToDoFlag);

					memset(szCmd,0,sizeof(szCmd));
					index = 0;
				}
				else if (szScript[j] == '(' && !bToDoFlag)
				{
					m_array.array[m_array.count++] = static_cast<INT>(m_operatorQ.size());
				}
				else if(szScript[j] == ')')
				{
					if (!bToDoFlag)
					{
						for(INT m = m_array.array[m_array.count-1]+1; m < static_cast<INT>(m_operatorQ.size()); m++)
						{//使被()括住的'&','|'的优先级增加
							if(m_operatorQ[m].szCHAR[0]=='&'
								||m_operatorQ[m].szCHAR[0]=='|')
							{
								m_operatorQ[m].PRI += 2;
							}
						}
						m_array.count--;
					}

					CHAR* pParamString = "";
					if (!FindParam(szScript, j, pParamString))
					{
						ProcessCtdOrToDoQ(szCmd, bToDoFlag);
						memset(szCmd,0, sizeof(szCmd));
						index = 0;
					}
					else
					{
						if (!bToDoFlag)
						{
							sprintf(m_cdtExpQ[m_cdtExpQ.size()-1].szCHAR, "%s%c%s%c",
								m_cdtExpQ[m_cdtExpQ.size()-1].szCHAR,
								'(',
								szCmd,
								')');

							CHAR* pString = m_cdtExpQ[m_cdtExpQ.size()-1].szCHAR;
							memset(szCmd,0,sizeof(szCmd));
							index = 0;
						}
						else
						{
							sprintf(m_todoExpQ[m_todoExpQ.size()-1].szCHAR, "%s%c%s%c",
								m_todoExpQ[m_todoExpQ.size()-1].szCHAR,
								'(',
								szCmd,
								')');

							CHAR* pString = m_todoExpQ[m_todoExpQ.size()-1].szCHAR;
							memset(szCmd,0, sizeof(szCmd));
							index = 0;
						}
					}
				}

				//向操作符队列添加
				if (!bToDoFlag)
				{
					m_operatorQ.push_back(atom);
				}
				
				//向条件表达式队列添加
				if (strcmp(szCmd, "") != 0)
				{
                    sprintf(szCmd, "%s", const_cast<CHAR*>(szCmd));
					
					if (strcmp(szCmd,"if") == 0)
						;
					else if (!bToDoFlag) 
					{
						memcpy(atom.szCHAR, szCmd, sizeof(szCmd));
						atom.pos = j-1;
						atom.PRI = -1;
						m_cdtExpQ.push_back(atom);
					}
					else
					{
						memcpy(atom.szCHAR, szCmd, sizeof(szCmd));
						m_todoExpQ.push_back(atom);
					}

					memset(szCmd,0, sizeof(szCmd));
					index = 0;
				}

			}
			break;
		case TODOOP:
			{
				bToDoFlag = TRUE;
				Atom atom;
				//向行为表达式队列添加
				if (strcmp(szCmd, "") != 0)
				{
					//if(VerifyExpress(szCmd)==FALSE)
					//	break;
					memcpy(atom.szCHAR, szCmd, sizeof(szCmd));
					m_todoExpQ.push_back(atom);
					memset(szCmd,0, sizeof(szCmd));
					index = 0;
				}
				if (szScript[j] == '}')
				{//为了能在条件脚本的后面添加注释
					goto Next;
				}
			}
			break;
		case OTHEROP:
		case RELATIONOP:
		case PARAMOP:
			{
				memcpy(&szCmd[index++], &szScript[j], 1);
			}
			break;
		default:
			break;
		}

	}

Next:

	if(m_array.count)
	{
		//MessageBox(NULL,"Script Error!!!","Error",MB_OK);
		return FALSE;
	}

	//PrINTQ();
	ProcessOptorQ();
	ProcessCdtQ();
	ProcessToDoQ();
	ClearQ();

	//PrINTList();
	//生成条件树
	CreateCdtTree();
	//生成条件节点
	CreateCdtNode();
	//加入到条件节点链表中
	InsertCdtNodeToCdtList();

	ClearList();

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;

}

BOOL AIScript::ProcessScript(INT nState, AI_Character* pAI)
{
	__ENTER_FUNCTION
	if (!pAI)
	{
		Assert(NULL && "AIScript::ProcessScript...pAI=NULL...");
		return FALSE;
	}
	Obj_Character* pChar = pAI->GetCharacter();
	if (pChar && pChar->GetObjType() != Obj::OBJ_TYPE_MONSTER)
	{
		Assert(NULL && "pAI->GetCharacter() != OBJ_TYPE_MONSTEr...");
		return FALSE;
	}

	((AI_Monster*)pAI)->ReSetNeedGoDist();
	//将m_pcdtList中的条件节点依次取出
	ConditionList* pCur = NULL;
	pCur = m_vStateLists[nState];

	while(pCur)
	{
		BOOL ret = FALSE;
		//判断条件是否成功，执行条件语句
		//取出m_pcdtList中的ConditionTree,解析树结构
		ConditionTree* pTree = NULL;
		pTree = pCur->pNode->pRootCondition;
		
		//将AI_xxx中的times的数据复制给相应的Node的times
		INT id = pCur->pNode->id;
		if (id >= AISCRIPT_NUM)
		{
			Assert(NULL && "id > AISCRIPT_NUM...array overflow...");
			return FALSE;
		}
		pCur->pNode->times = ((AI_Monster*)pAI)->m_aAIScriptTimes[id];

		if(pCur->pNode->times > 0 || pCur->pNode->times == -1)
		{
			ret = ExcuteCdtScript(pTree, pAI);
		}
        else
		{
			pCur = pCur->pNext;
			continue;
		}

		//如果条件成立，则执行行为语句
		//取出m_pcdtList中的pToDo,解析toDo结构
		if (TRUE == ret)
		{
			ExcuteToDoScript(pCur->pNode->pToDo, pAI);
			pCur->pNode->SetDownFlag(1);//设置该操作执行过

			if (pCur->pNode->times > 0)
			{// 只是不想改变原来的结构和逻辑 故这样做:)...
				pCur->pNode->times--;
				if (pCur->pNode->times < 0)
					pCur->pNode->times = 0;
				((AI_Monster*)pAI)->m_aAIScriptTimes[id]--;
				if (((AI_Monster*)pAI)->m_aAIScriptTimes[id] < 0)
					((AI_Monster*)pAI)->m_aAIScriptTimes[id] = 0;
			}
			if (pCur->pNode->times == 0)
			{
				pCur->pNode->SetDownFlag(0);//设置该操作执行完
			}
			//if (pAI->GetCharacter()->GetObjType() == Obj::OBJ_TYPE_MONSTER) 
			//{
			//	((AI_Monster*)pAI)->SetSkillID(m_SkillID);
			//}
			return TRUE;

		}
		else
		{
			if (SKILLSECTION == nState) 
			{
				if (pAI->GetCharacter()->GetObjType() == Obj::OBJ_TYPE_MONSTER) 
				{
					ObjID_t idSkill = ((AI_Monster*)pAI)->GetSkillID();
					FLOAT fTmpDist = ((AI_Monster*)pAI)->GetNeedGoDist(idSkill);
					FLOAT fNeedGoDist = ((AI_Monster*)pAI)->GetNeedGoDist();
					if (fTmpDist > 0 && fNeedGoDist > fTmpDist)
					{
						((AI_Monster*)pAI)->SetNeedGoDist(fTmpDist);
						((AI_Monster*)pAI)->SetSkillID(idSkill);
					}
				}
			}
		}
		pCur = pCur->pNext;
	}
	if (NULL == pCur) 
	{/** 一个也没有执行成功 */
		return FALSE;
	}

	return TRUE;
	__LEAVE_FUNCTION
	return TRUE;
}
//pReturn为上一次执行的结果
BOOL AIScript::ExcuteCdtScript(ConditionTree* p, const AI_Character* pAI)
{
	__ENTER_FUNCTION
	BOOL ret = FALSE;
	BOOL bContinue = FALSE;
	if (NULL == p)
	{
		return FALSE;
	}
	// 确保stack的正确性, 每次使用前先重置一次
	m_stack.Reset();
	do
	{
		while(p != NULL)
		{
			m_stack.push((Elem*)p);
			p = p->pLeft;
		}

		do
		{
			p = (ConditionTree*)m_stack.pop();

			if (!p)
			{
				//Assert(NULL && "AIScript::ExcuteCdtScript...p = (ConditionTree*)m_stack.pop() = NULL!!");
				return FALSE;
			}
			if (p->Node.op != OPTAND && p->Node.op != OPTOR && p->Node.op != OPTROOT) 
			{
				//执行条件表达式
				ret = ExcuteExpress(&p->Node, pAI);

				if (ret==TRUE && p->pParent->Node.op==OPTOR
					 || ret==FALSE && p->pParent->Node.op==OPTAND)
				{
					bContinue = TRUE;
				}
				else
				{
					bContinue = FALSE;
				}

			}
			else if (ret == TRUE && p->Node.op == OPTAND)
			{
				bContinue = FALSE;
			}
			else if (ret == TRUE && p->Node.op == OPTOR)
			{
				bContinue = TRUE;
			}
			else if (ret == FALSE && p->Node.op == OPTOR)
			{
				bContinue = FALSE;
			}
			else if (ret == FALSE && p->Node.op == OPTAND)
			{
				bContinue = TRUE;
			}
			else
			{
				bContinue = FALSE;
			}


		}while (bContinue==TRUE);

		p = p->pRight;

	}while (!(m_stack.isEmpty() && p==NULL));

	return ret;
	__LEAVE_FUNCTION
	return FALSE;
}

BOOL AIScript::ExcuteToDoScript(TreeNodeList* p, const AI_Character* pAI)
{
	__ENTER_FUNCTION
	BOOL bRet = FALSE;
	while(p != NULL)
	{
		bRet = ExcuteToDoExpress(&p->Node, pAI);
		p = p->pNext;
	}

	return bRet;
	__LEAVE_FUNCTION
	return FALSE;
}

BOOL AIScript::ExcuteExpress(const TreeNode* p, const AI_Character* pAI)
{
	__ENTER_FUNCTION
	if(p==NULL)
	{
		Assert(FALSE);
		return FALSE;
	}

	INT op = p->op;
	INT rRet = p->value;
	if (!pAI)
	{
		Assert(NULL && "AIScript::ExcuteExpress...pAI=NULL...");
		return FALSE;
	}
	INT lRet = p->FuncPtr(((Obj_Character*)pAI->GetCharacter()), p->param[0], p->param[1], p->param[2]);

	switch(op) 
	{
	case OPTEQUAL:
		return lRet==rRet?TRUE:FALSE;
	case OPTLESSTHAN:
		return lRet<rRet?TRUE:FALSE;
	case OPTLESSEQUAL:
		return lRet<=rRet?TRUE:FALSE;
	case OPTLARGETHAN:
		return lRet>rRet?TRUE:FALSE;
	case OPTLARGEEQUAL:
		return lRet>=rRet?TRUE:FALSE;
	case OPTUNEQUAL:
		return lRet!=rRet?TRUE:FALSE;
	default:
		Assert(FALSE);
		break;
	}

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

BOOL AIScript::ExcuteToDoExpress(const TreeNode * p, const AI_Character* pAI)
{
	__ENTER_FUNCTION
	if(p==NULL)
	{
		Assert(FALSE);
		return FALSE;
	}

	if(0 < p->FuncPtr(((Obj_Character*)pAI->GetCharacter()), p->param[0], p->param[1], p->param[2]))
		return TRUE;
	else
		return FALSE;

	return FALSE;
	__LEAVE_FUNCTION
	return FALSE;

}

VOID AIScript::ReSetTimes(VOID)
{
	__ENTER_FUNCTION
	ConditionList* pCur = NULL;
	pCur = m_pcdtList;
	while(pCur)
	{
		pCur->pNode->times = pCur->pNode->times2;
		pCur = pCur->pNext;
	} 
	__LEAVE_FUNCTION

}

INT	AIScript::ParseCHAR(const CHAR* szch)
{
	__ENTER_FUNCTION
	switch(*szch)
	{
	case '=':
	case '>':
	case '<':
	case '!':
		return RELATIONOP;
	case '&':
	case '|':
	case '(':
	case ')':
		return ANDOROP;
	case ';':
	case '{':
	case '}':
		return TODOOP;
	default:
		return OTHEROP;
	}	

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

BOOL AIScript::ProcessOptorQ(VOID)
{
	__ENTER_FUNCTION
	for(INT i=0; i < static_cast<INT>(m_operatorQ.size()); i++)
	{
		if (m_operatorQ[i].szCHAR[0]=='&'
			 ||m_operatorQ[i].szCHAR[0]=='|')
		{
			InsertToNodeList(m_operatorQ[i], OPERATOR);
		}
	}

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

BOOL AIScript::ProcessCdtQ()
{
	__ENTER_FUNCTION
	for (INT i=0; i < static_cast<INT>(m_cdtExpQ.size()); i++)
	{
		InsertToNodeList(m_cdtExpQ[i], CDTEXPRESSION);
	}

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

BOOL AIScript::ProcessToDoQ(VOID)
{
	__ENTER_FUNCTION
	for (INT i = 0; i < static_cast<INT>(m_todoExpQ.size()); i++)
	{
		InsertToNodeList(m_todoExpQ[i], TODOEXPRESSION);
	}

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

BOOL AIScript::CreateCdtTree(VOID)
{
	__ENTER_FUNCTION
	m_pcdtTree = NULL;
	if (m_poptorNodeList == NULL && m_pcdtNodeList == NULL)
	{
		return FALSE;
	}

	//生成新的条件树的根节点
	ConditionTree* pRoot = NULL;
	pRoot = new ConditionTree;
	if(pRoot == NULL)
	{
		Assert(FALSE);
		return FALSE;
	}

	//第一步，根据操作符list生成树框架
	TreeNodeList* pList = m_poptorNodeList;
	while(pList)
	{
		//new出一个新的树结点
		ConditionTree* pNewTree = NULL;
		pNewTree = new ConditionTree;
		if (pNewTree == NULL)
		{
			Assert(FALSE);
			return FALSE;
		}
		//进行插入操作-->是第一次插入
		if(m_pcdtTree == NULL)
		{
			//根节点不进行赋值
			m_pcdtTree = pRoot; 
			//将list表中的Node复制一份到pNewTree;
			pNewTree->Node = pList->Node;
			m_pcdtTree->pLeft = pNewTree;
			pNewTree->pParent = pRoot;

		}
		else
		{
			//从根节点的左子节点寻找位置
			ConditionTree* pCur = pRoot->pLeft;
			ConditionTree* pParent = NULL;
			while(pCur)
			{
				pParent = pCur;
				if (pList->Node.pos > pCur->Node.pos)
				{
					pCur = pCur->pRight;
				}
				else
				{
					pCur = pCur->pLeft;
				}
			}
			pNewTree->Node = pList->Node;
			if (pNewTree->Node.pos>pParent->Node.pos)
			{
				pParent->pRight = pNewTree;
			}
			else
			{
				pParent->pLeft = pNewTree;
			}

			pNewTree->pParent = pParent;

		}


		pList = pList->pNext;
	}

	//第二步，根据条件表达式list生成完整的条件树
	pList = m_pcdtNodeList;
	while (pList)
	{
		//new出一个新的树结点
		ConditionTree* pNewTree = NULL;
		pNewTree = new ConditionTree;
		if(pNewTree == NULL)
		{
			Assert(FALSE);
			return FALSE;
		}
		//进行插入操作-->是第一次插入
		//该情况只有当条件表达式为if(HP<10)这样的时候才能进入
		if (m_pcdtTree==NULL)
		{
			//根节点不进行赋值
			m_pcdtTree = pRoot; 
			//将list表中的Node复制一份到pNew;
			pNewTree->Node = pList->Node;
			m_pcdtTree->pLeft = pNewTree;
			pNewTree->pParent = pRoot;

		}
		else
		{
			//从根节点的左子节点寻找位置
			ConditionTree* pCur = pRoot->pLeft;
			ConditionTree* pParent=NULL;
			while (pCur)
			{
				pParent = pCur;
				if (pList->Node.pos>pCur->Node.pos)
				{
					pCur = pCur->pRight;
				}
				else
				{
					pCur = pCur->pLeft;
				}
			}

			pNewTree->Node = pList->Node;
			if (pNewTree->Node.pos>pParent->Node.pos)
			{
				pParent->pRight = pNewTree;
			}
			else
			{
				pParent->pLeft = pNewTree;
			}
			pNewTree->pParent = pParent;

		}


		pList = pList->pNext;
	}
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

BOOL AIScript::CreateCdtNode(VOID)
{
	__ENTER_FUNCTION
	m_pcdtNode = NULL;
	//判断条件树是否存在
	if (m_pcdtTree == NULL)
	{
		return FALSE;
	}

	//生成新的条件节点
	m_pcdtNode = new ConditionNode;
	if (m_pcdtNode == NULL)
	{
		Assert(FALSE);
		return FALSE;
	}

	//将解析的id赋值给m_pcdtNode->id
	m_pcdtNode->id = m_nID;
	//使条件节点中的树指针指向生成的条件树结点
	m_pcdtNode->pRootCondition = m_pcdtTree;

	//根据行为表达式list生成条件节点的toDo项
	TreeNodeList* pCur = NULL;
	//-------------------------------------------------------------------------
	pCur = m_ptodoNodeList;
	TreeNodeList* pParent = NULL;
	INT i=0, count = 0;
	while (pCur)
	{
		//把TIMES的表达式提取到Node结构体中
		const CHAR* FuncName = GetFuncNamebyPtr(pCur->Node.FuncPtr);
		if(FuncName && strcmp(FuncName, "AIS_SetTimes") == 0)
		{
			TreeNodeList* pTemp = pCur;
			m_pcdtNode->times   = pCur->Node.param[0];
			m_pcdtNode->times2  = pCur->Node.param[0];
			if (pParent == NULL)
			{
				pTemp = pCur;
				pCur = pCur->pNext;
				pTemp->pNext = NULL ;
			}
			else
			{
				pTemp = pCur;
				pParent->pNext = pCur->pNext;
				pCur = pCur->pNext ;
				pTemp->pNext = NULL;
			}
			SAFE_DELETE(pTemp) ; // 删除该节点

			continue;
		}
		else if(FuncName && strcmp(FuncName, "AIS_SetPRI") == 0)
		{
			TreeNodeList* pTemp = pCur;
			m_pcdtNode->PRI  = pCur->Node.param[0];
			if (pParent == NULL)
			{
				pTemp = pCur;
				pCur = pCur->pNext;
				pTemp->pNext = NULL ;
			}
			else
			{
				pTemp = pCur;
				pParent->pNext = pCur->pNext;
				pCur = pCur->pNext ;
				pTemp->pNext = NULL;
			}
			SAFE_DELETE(pTemp) ; // 删除该节点

			continue;
		}

		count++;
		pParent = pCur;
		pCur = pCur->pNext;
	}

	pCur = NULL;
	pCur = m_ptodoNodeList;
	while (pCur && i<count)
	{
		TreeNodeList* pToDoList = NULL;
		pToDoList = new TreeNodeList;
		if (pToDoList == NULL)
		{
			Assert(FALSE);
			return FALSE;
		}

		pToDoList->Node = pCur->Node;
		TreeNodeList* pTmp = NULL;
		pTmp = m_pcdtNode->pToDo;

		if (pTmp == NULL)
		{
			m_pcdtNode->pToDo = pToDoList;
		}
		else
		{
			while (pTmp->pNext)
			{
				pTmp = pTmp->pNext;
			}
			pTmp->pNext = pToDoList;
		}
		pCur = pCur->pNext;
		i++;

	}

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

BOOL AIScript::InsertCdtNodeToCdtList(VOID)
{
	__ENTER_FUNCTION
	//判断条件节点是否存在
	if (m_pcdtNode == NULL)
	{
		return FALSE;
	}

	//生成新的条件list节点
	ConditionList* pNewListNode = NULL;
	pNewListNode = new ConditionList;
	if (pNewListNode == NULL)
	{
		Assert(FALSE);
		return FALSE;
	}

	pNewListNode->pNode = m_pcdtNode;
	//将条件list节点中的条件节点指针指向生成的条件节点
	if (m_pcdtList == NULL)
	{
		m_pcdtList = pNewListNode;
	}
	else
	{//按照条件节点的优先级插入条件list，最终生成的条件list是按
		//条件节点优先级降序的List.
		ConditionList* pCur = NULL;
		ConditionList* pParent = NULL;
		pCur = m_pcdtList;
		if (pNewListNode->pNode->PRI > pCur->pNode->PRI)
		{
			pNewListNode->pNext = pCur;
			m_pcdtList = pNewListNode;
		}
		else
		{
			while (pCur != NULL && pNewListNode->pNode->PRI <= pCur->pNode->PRI)
			{
				pParent = pCur;
				pCur = pCur->pNext;
			}
			if(pCur == NULL)
			{
				pParent->pNext = pNewListNode;
			}
			else
			{
				pParent->pNext = pNewListNode;
				pNewListNode->pNext = pCur;
			}

		}
	}
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

BOOL AIScript::InsertToNodeList(Atom atom, INT flag)
{
	__ENTER_FUNCTION
	TreeNode	  treeNode;
	TreeNodeList* ptreeNodeList = NULL;

	ptreeNodeList = new TreeNodeList;
	if (ptreeNodeList == NULL)
	{
		Assert(FALSE);
		return FALSE;
	}
	switch(flag) 
	{
	case OPERATOR:
		{
			if ('&' == atom.szCHAR[0]) 
			{
				treeNode.op = OPTAND;
			}
			else if ('|' == atom.szCHAR[0])  	
			{
				treeNode.op = OPTOR;
			}

			treeNode.PRI = atom.PRI;
			treeNode.pos = atom.pos;
			ptreeNodeList->Node = treeNode;

			if (m_poptorNodeList == NULL)
			{
				m_poptorNodeList = ptreeNodeList;
			}
			else
			{//对'&','|'按各自的优先级排序（升序）
				TreeNodeList* pParent;
				TreeNodeList* pCur;
				pCur = m_poptorNodeList;

				if (ptreeNodeList->Node.PRI < pCur->Node.PRI)
				{//新插入的节点的优先级比第一个节点的小时，应替换首节点
					ptreeNodeList->pNext = pCur;
					m_poptorNodeList = ptreeNodeList;
					pCur = ptreeNodeList;
				}
				else
				{//找到适合的位置,然后进行插入
					while (pCur != NULL && ptreeNodeList->Node.PRI >= pCur->Node.PRI)
					{
						pParent = pCur;
						pCur = pCur->pNext;
					}
					if(pCur == NULL)
					{
						pParent->pNext = ptreeNodeList;
					}
					else
					{
						pParent->pNext = ptreeNodeList;
						ptreeNodeList->pNext = pCur;
					}

				}
			}
		}
		break;
	case CDTEXPRESSION:
		{
			treeNode = ParseExpress(atom);
			ptreeNodeList->Node = treeNode;
			if (m_pcdtNodeList == NULL)
			{
				m_pcdtNodeList = ptreeNodeList;
			}
			else
			{
				TreeNodeList* pParent;
				TreeNodeList* pCur;
				pCur = m_pcdtNodeList;
				while (pCur != NULL)
				{
					pParent = pCur;
					pCur = pCur->pNext;
				}
				pParent->pNext = ptreeNodeList;

			}
		}
		break;
	case TODOEXPRESSION:
		{
			treeNode = ParseExpress(atom);
			ptreeNodeList->Node = treeNode;
			if (m_ptodoNodeList == NULL)
			{
				m_ptodoNodeList = ptreeNodeList;
			}
			else
			{
				TreeNodeList* pParent;
				TreeNodeList* pCur;
				pCur = m_ptodoNodeList;
				while (pCur != NULL)
				{
					pParent = pCur;
					pCur = pCur->pNext;
				}
				pParent->pNext = ptreeNodeList;


			}
		}
		break;
	default:
		break;
	}
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;

}

TreeNode AIScript::ParseExpress(Atom atom)
{
	TreeNode treeNode;
	__ENTER_FUNCTION
	CHAR		szCmd[32];
	static INT	index = 0;
	INT			prev = 0;
	INT len = (INT)strlen(atom.szCHAR);
	memset(szCmd,0, sizeof(szCmd));

	

	for (INT i = 0; i < len; i++)
	{
		switch(ParseCHAR(&atom.szCHAR[i])) 
		{
		case ANDOROP:
			{
				if (atom.szCHAR[i] == '(')
				{
					if (strcmp(szCmd, "") != 0)
					{//是命令,如MP<5 or 5>MP
						PLAISFunc FuncPtr = GetFuncPtrbyName(szCmd);
						if (FuncPtr)
						{
							treeNode.FuncPtr = FuncPtr;
						}
						else
						{
							Assert(FALSE);
						}

						memset(szCmd,0, sizeof(szCmd));
						index = 0;
					}
				}
				else if (atom.szCHAR[i] == ')') 
				{
					SetParam(szCmd, atom);
					memset(szCmd,0, sizeof(szCmd));
					memcpy(treeNode.param, atom.param, sizeof(atom.param));
					index = 0;
				}
			}
			break;
		case RELATIONOP:
			{
				if (RELATIONOP == ParseCHAR(&atom.szCHAR[i+1])) 
				{
					if ('<' == atom.szCHAR[i] && '=' == atom.szCHAR[i+1]) 
					{ 
						treeNode.op = OPTLESSEQUAL;
					}
					else if ('>' == atom.szCHAR[i] && '=' == atom.szCHAR[i+1])  	
					{
						treeNode.op = OPTLARGEEQUAL;
					}
					else if ('!' == atom.szCHAR[i] && '=' == atom.szCHAR[i+1])
					{
						treeNode.op = OPTUNEQUAL;
					}
					++i;
				}
				else
				{
					if ('=' == atom.szCHAR[i]) 
					{
						treeNode.op = OPTEQUAL;
					}
					else if ('>' == atom.szCHAR[i])
					{
						treeNode.op = OPTLARGETHAN;
					}
					else if ('<' == atom.szCHAR[i])
					{
						treeNode.op = OPTLESSTHAN;
					}

				}
				treeNode.pos = atom.pos;
			}
			break;
		case OTHEROP:
			{
				memcpy(&szCmd[index++], &atom.szCHAR[i], 1);
				if (i == len-1)//这时的szCmd必然是右边的value值,如MP<HP
				{
					if (strcmp(szCmd,"") != 0)
					{
						INT ret = GetMacrobyName(szCmd);
						if (-1 != ret) 
						{
							treeNode.value = ret;
						}
						else
						{
							INT len = (INT)strlen(szCmd);
							for (INT j = 0; j < len; ++j)
							{
								 if (!isdigit(szCmd[j]))
								 {
									 Assert(FALSE);
								 }
							}
							treeNode.value = atoi(szCmd);
						}

						memset(szCmd,0, sizeof(szCmd));
						index = 0;
					}
				}

			}
			break;
		default:
			Assert(FALSE);
			break;
		}
	}
	return treeNode;
	__LEAVE_FUNCTION
	return treeNode;
}

VOID AIScript::ClearQ(VOID)
{
	__ENTER_FUNCTION
	m_operatorQ.clear();
	m_cdtExpQ.clear();
	m_todoExpQ.clear();
	__LEAVE_FUNCTION

}

VOID AIScript::ClearList(VOID)
{
	__ENTER_FUNCTION
	TreeNodeList* pTmp = NULL;
	while(m_poptorNodeList)
	{
		pTmp = m_poptorNodeList->pNext;
		SAFE_DELETE(m_poptorNodeList);
		m_poptorNodeList = pTmp;
	}

	pTmp = NULL;
	while(m_pcdtNodeList)
	{
		pTmp = m_pcdtNodeList->pNext;
		SAFE_DELETE(m_pcdtNodeList);
		m_pcdtNodeList = pTmp;
	}

	pTmp = NULL;
	while(m_ptodoNodeList)
	{
		pTmp = m_ptodoNodeList->pNext;
		SAFE_DELETE(m_ptodoNodeList);
		m_ptodoNodeList = pTmp;
	}
	m_pcdtList;

	__LEAVE_FUNCTION
}

VOID AIScript::ClearTree(ConditionTree* p)
{
	__ENTER_FUNCTION
	if(p==NULL)
	{
		return ;
	}

	ClearTree(p->pLeft);
	ClearTree(p->pRight);
	SAFE_DELETE(p);

	__LEAVE_FUNCTION

}

VOID AIScript::ClearNode(ConditionNode* p)
{
	__ENTER_FUNCTION
	if (p)
	{
		ClearTree(p->pRootCondition);
		TreeNodeList* pToDo = NULL;
		TreeNodeList* pTmp  = NULL;
		pToDo = p->pToDo;
		while (pToDo)
		{
			pTmp = pToDo->pNext;
			SAFE_DELETE(pToDo);
			pToDo = pTmp;
		}
		SAFE_DELETE(p);
	}
	__LEAVE_FUNCTION
}

VOID AIScript::ClearCdtList(VOID)
{
	__ENTER_FUNCTION
	ConditionList* pTmp = NULL; 
	/*	
	while(m_pcdtList2)
	{
	pTmp = m_pcdtList2->pNext;
	//此处已经将实际的Node(包括TreeNode,ToDoNode...)节点delete,故下面的各个状态链表只要负责
	//delete本身的链表节点就可以了.
	ClearNode(m_pcdtList2->pNode);
	SAFE_DELETE(m_pcdtList2);
	m_pcdtList2 = pTmp;
	}
	*/
	//////////////////////////////////////////////////////////////////////////
	for (INT i = 0; ONDEADSECTION+1 > i; ++i)
	{
		while(m_vStateLists[i])
		{
			pTmp = m_vStateLists[i]->pNext;
			ClearNode(m_vStateLists[i]->pNode);
			SAFE_DELETE(m_vStateLists[i]);
			m_vStateLists[i] = pTmp;
		}
	}
	__LEAVE_FUNCTION
}

BOOL AIScript::VerifyExpress(const CHAR* szch)
{

	return TRUE;
}

VOID AIScript::ReSetFlags(VOID)
{
	__ENTER_FUNCTION
	ConditionList* pCur = NULL;
	pCur = m_pcdtList;
	while (pCur)
	{
		pCur->pNode->ReSetDownFlag(pCur->pNode->id);
		pCur = pCur->pNext;
	}
	__LEAVE_FUNCTION
}

VOID AIScript::ResetStateList(const AI_Character* pAI)
{
	__ENTER_FUNCTION
#define MAX_RETRY_COUNT (64)
	
	INT id = 0;
	INT retryCount = 0;
	if (!pAI)
	{
		Assert(NULL && "AIScript::ResetStateList...pAI=NULL...");
		return ;
	}
	Obj_Character* pChar = pAI->GetCharacter();
	if (pChar && pChar->GetObjType() != Obj::OBJ_TYPE_MONSTER)
	{
		return ;
	}
	for (INT i = 0; ONDEADSECTION+1 > i; ++i)
	{
		if(NULL == m_vStateLists[i])
		{
			continue;
		}
		ConditionList* pCur = m_vStateLists[i];
		while(pCur)
		{
			if (retryCount++ > MAX_RETRY_COUNT)
				break;
			pCur->pNode->ResetTimes();
			/////////////////////////////////////////////////////
			// 重置AI_Monster中的Script的次数Times
			id = pCur->pNode->id;
			((AI_Monster*)pAI)->m_aAIScriptTimes[id] = pCur->pNode->times2;
			////////////////////////////////////////////////////
			pCur->pNode->ReSetAllDownFlags();
			pCur = pCur->pNext;

		};
	}
	__LEAVE_FUNCTION
}

VOID AIScript::InitAllStateList(VOID)
{
	__ENTER_FUNCTION
	ConditionList* pCur = m_pcdtList;
	//m_pcdtList2 = m_pcdtList;
	while(pCur)
	{
		INT ret = 0;
		VerdictState(pCur->pNode->pRootCondition, pCur->pNode, ret);
		if(!ret)
		{// 没有加入任何状态List中，向所有的List添加该条件语句
			for (INT i = 0; SRETURN > i; ++i)
			{
				ConditionList* p = new ConditionList;
				if (NULL == p) break;

				p->pNode = pCur->pNode;
				if (NULL == m_vStateLists[i])
				{
					m_vStateLists[i] = p;
				}
				else
				{
					ConditionList* ptmp = m_vStateLists[i];
					while (ptmp->pNext)
					{
						ptmp = ptmp->pNext;
					}
					ptmp->pNext = p;
				}
			}
		}
		pCur = pCur->pNext;
	}

	ConditionList* pTmp = NULL; 
	while(m_pcdtList)
	{
		pTmp = m_pcdtList->pNext;
		SAFE_DELETE(m_pcdtList);
		m_pcdtList = pTmp;
	}
	m_pcdtList = NULL;

	__LEAVE_FUNCTION
}

VOID AIScript::VerdictState(ConditionTree* t, ConditionNode* p, INT& ret)
{
	__ENTER_FUNCTION
	Assert(p!=NULL);

	if (t == NULL)
	{
		return ;
	}

	VerdictState(t->pLeft, p, ret);
	//Verdict and process...
	const CHAR* FuncName = GetFuncNamebyPtr(t->Node.FuncPtr);
	
	if(FuncName && strcmp(FuncName, "AIS_GetAIState") == 0)
	{
		INT nState;
		ConditionList* pRet = NULL;
		ConditionList* pp = NULL;

		pp = new ConditionList;
		if(NULL == pp) 
		{
			return ;
		}

		pp->pNode = p;
		nState = t->Node.value;
		if (m_vStateLists[nState] == NULL)
		{
			m_vStateLists[nState] = pp;
		}
		else
		{
			ConditionList* pCur = m_vStateLists[nState];
			while (pCur->pNext)
			{
				pCur = pCur->pNext;
			}
			pCur->pNext = pp;
		}
		ret = 1;//标记已经加入过了
	}

	VerdictState(t->pRight, p, ret);

	__LEAVE_FUNCTION
}

VOID AIScript::SetParam(const CHAR* szCmd, Atom& atom)
{
	__ENTER_FUNCTION
	INT i = 0;
	INT pos1 = 0, pos2 = 0; 
	string sParam(szCmd);
	pos2 = (INT)sParam.find(',', pos1);

	if (pos2 < 0)
	{
		if (strcmp(szCmd, "") == 0)
		{
			atom.param[0] = -1;
			return ;
		}
	}

	while (i < sizeof(atom.param)/sizeof(INT))
	{
		atom.param[i] = atoi(sParam.substr(pos1, pos2).c_str());
		pos1 = pos2 + 1;
		pos2 = (INT)sParam.find(',', pos1);
		++i;
		if (pos2 < 0)
		{// 最后一个参数
			atom.param[i] = atoi(sParam.substr(pos1).c_str());
			return ;
		}
	}

	{
		atom.param[0] = atoi(szCmd);
	}

	__LEAVE_FUNCTION
}

BOOL AIScript::IsParam(const CHAR* szCmd)
{
	__ENTER_FUNCTION
	INT len = (INT)strlen(szCmd);
	
	for (INT i = 0; i < len; ++i)
	{
		if (!isdigit(szCmd[i]))
		{
			if (szCmd[i] == '-')
			{
				return TRUE;
			}
			else if (szCmd[i] == ',')  
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}

		}
	}

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

VOID AIScript::ProcessCtdOrToDoQ(const CHAR* szCmd, BOOL bToDoFlag)
{
	__ENTER_FUNCTION
	if (!bToDoFlag)
	{
		if (m_cdtExpQ.size() > 0 && m_cdtExpQ[m_cdtExpQ.size()-1].szCHAR)
		{
			sprintf(m_cdtExpQ[m_cdtExpQ.size()-1].szCHAR, "%s%s",
				m_cdtExpQ[m_cdtExpQ.size()-1].szCHAR,
				szCmd);
		}
	}
	else
	{
		if (m_cdtExpQ.size() > 0 && m_cdtExpQ[m_cdtExpQ.size()-1].szCHAR)
		{
			sprintf(m_todoExpQ[m_todoExpQ.size()-1].szCHAR, "%s%s",
				m_todoExpQ[m_todoExpQ.size()-1].szCHAR,
				szCmd);
		}
	}
	__LEAVE_FUNCTION
}

BOOL AIScript::FindParam(const CHAR* szCmd, INT index, CHAR* pString)
{
	__ENTER_FUNCTION
	for (INT i = index-1; i >= 0; --i)
	{
		if (szCmd[i] == '(')
		{
			CHAR szParam[32];
			memset(szParam, 0, sizeof(szParam));
			Assert(index-i-1<=32);
			memcpy(szParam, &szCmd[i+1], index-i-1);
			if (IsParam(szParam))
			{
				pString = szParam;
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	}

	return FALSE;
	__LEAVE_FUNCTION
	return FALSE;
}
