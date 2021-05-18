/*************************************************************************
 文件名	: 	AIScriptDef.h
 版本号 :	0.0.1
 功  能	:	AIScript用到的数据结构定义
 修改记录:	
 *************************************************************************/


#ifndef _AISCRIPTDEF_H_
#define _AISCRIPTDEF_H_

#include "AIScriptFunction.h"
#include <vector>
#include "State.h"

using namespace std;

#define		SET_FLAG_DOWN(flag,index)	flag = ( flag | 1<<(31-index)) 
#define		RESET_FLAG_DOWN(flag,index)	flag = ( flag & ~(1<<(31-index))) 
#define		GET_FLAG_DOWN(flag,index)	( ((flag)>>(31-index)) & 0x00000001 )

#define		MAX_STACK_NUM  (1024)
#define		NORMAL_AI_PRI  (20)
#define		MAX_TIMES_NUM  (7)
#define		MAX_ID_NUM	   (10)

enum
{
	ANDOROP = 0, //&,|,(,)
	RELATIONOP,	 //=,>,<,>=,<=	
	TODOOP,		 //{,},;,','
	PARAMOP,	 //[,]
	OTHEROP,	 //其他字符
};

enum 
{
	OPERATOR = 0,
	CDTEXPRESSION,
	TODOEXPRESSION,

};

// 关系运算符定义
enum REL_OPERATOR
{
	OPTEQUAL = 0 ,
	OPTUNEQUAL ,
	OPTLESSTHAN ,
	OPTLARGETHAN ,
	OPTLESSEQUAL ,
	OPTLARGEEQUAL ,
	OPTAND ,
	OPTOR ,
	OPTROOT ,

};

enum ENUM_AISTATE
{
	// { 为了兼容原来的宏定义--与已经配置好的scriptxxx.ai文件相关
	SIDLE			= ESTATE_IDLE,			//空闲状态
	SAPPROACH		= ESTATE_APPROACH,		//靠近状态
	SATTACK			= ESTATE_COMBAT,		//攻击状态
	SFOLLOW			= ESTATE_TEAMFOLLOW,	//跟随状态
	SRETURN			= ESTATE_GOHOME,		//放弃追赶
	SPATROL			= ESTATE_PATROL,		//巡逻	
	SFLEE			= ESTATE_FLEE,			//逃跑状态
	// }

	SKILLSECTION	= ESTATE_NUMBER+1,		//技能段
	ONBESKILLSECTION,	//被攻击段
	ONDAMAGESECTION,	//受到伤害段
	ONDEADSECTION,		//死亡段

	// { section of disuse...
	//SCHANGEENEMY,		//改变敌人状态
	//SWAIT,			//等待状态
	//SFASTFLEE,		//快速逃跑状态
	//SARRIVE,			//已到达目的地状态
	//SUSESKILL,		//使用技能状态
	//怪物状态
	//YUN,				//怪物晕
	//ZHONGDU,			//怪物中毒
	//DEAD,				//怪物死亡
	//SLEEP,			//怪物睡眠
	//怪物说话类型
	//HAPPY,			//说话类型--喜
	//ANGRY,			//说话类型--怒
	//SORROW,			//说话类型--哀
	//ENJOY,			//说话类型--乐
	//宠物攻击的类型
	//PHYSICSKILL,		//物理技能
	//MAGICSKILL,		//魔法技能
	//ASSISTSKILL,		//辅助技能
	// }

	ERRORCMD,			//错误命令
};

struct BracketArray
{
	INT			array[10];
	INT			count;
	BracketArray()
	{
		memset(array,0,sizeof(array));
		count = 0;
	}
};

struct Atom
{
	CHAR		szCHAR[32];
	INT			param[3];
	INT			pos;
	INT			PRI;
	Atom()
	{
        memset(szCHAR,0,sizeof(szCHAR));
		memset( param, -1, sizeof(param) );
		pos = -1;
		PRI = -1;
	}
};

struct TreeNode
{
	PLAISFunc 	FuncPtr;
	INT			op;
	INT			param[3];
	INT			value; 
	INT			pos;
	INT			PRI;
	TreeNode()
		:FuncPtr(0),
		op(OPTROOT),
		value(-1),
		PRI(-1),
		pos(-1)
	{
		memset( param, -1, sizeof(param) );
	}
};

struct TreeNodeList 
{
	TreeNode			Node;
	TreeNodeList*		pNext;
	TreeNodeList()
		:pNext(0)
	{}
};

struct ConditionTree
{
	TreeNode			Node;
	ConditionTree* 		pParent;
	ConditionTree* 		pLeft;
	ConditionTree* 		pRight;	
	ConditionTree()
		:pLeft(0),
		pRight(0)
	{};
};

struct ConditionNode
{
	DWORD				flag;
	//flag的每位的含义如下：
	//	00000000,00000000,00000000,00000000
	//	||
	//	||_该条件节点执行过了 
	//	|__ 该条件节点执行完了
	//   

	INT 				id;
	INT 				PRI;
	INT					times;
	INT					times2;
	ConditionTree* 		pRootCondition;
	TreeNodeList*		pToDo;
	ConditionNode()
		:flag(0),
		id(-1),
		PRI(-1),
		times(0),
		times2(0),
		pRootCondition(0),
		pToDo(0)
	{}
	VOID ResetTimes(VOID)
	{
		times = times2;
	}
	VOID SetDownFlag(INT index)
	{
		SET_FLAG_DOWN(flag,index);
	}
	INT GetDownFlag(INT index)
	{
		return GET_FLAG_DOWN(flag,index);
	}
	VOID ReSetDownFlag(INT index)
	{
		RESET_FLAG_DOWN(flag,index);
	}
	VOID ReSetAllDownFlags(VOID)
	{
		for(INT i=0;32>i;++i)
			ReSetDownFlag(i);
	}
};

struct ConditionList
{
	ConditionNode*		pNode;
	ConditionList*		pNext;
	ConditionList()
		:pNode(0),
		pNext(0)
	{}
};

struct TODOTIMES
{
	INT current_times;
	INT total_times;
	TODOTIMES()
	{
		current_times = 0;
		total_times = 1;
	}
};

typedef ConditionTree* Elem;



typedef vector<Atom> OperatorQueue;
typedef vector<Atom> CdtExpressionQueue;
typedef vector<Atom> ToDoExpressionQueue;

class Stack
{
public:
	Stack()
	{
		memset(elem, 0, MAX_STACK_NUM);
		top = -1;
	}
	~Stack()
	{
	}
	VOID push(Elem* p)
	{
		if (++top < MAX_STACK_NUM)
		{
			elem[top] = p;
		}
		else
		{
			Assert(NULL && "++top<MAX_STACK_NUM");
		}
	}
	Elem* pop()
	{
		if (top != -1 && top >= 0)
		{
			return elem[top--];
		}

		return NULL;
	}
	bool isEmpty()
	{
		return top==-1 ? true : false;
	}
	void Reset()
	{
		memset(elem, 0, MAX_STACK_NUM);
		top = -1;
	}

private:
	Elem*	elem[MAX_STACK_NUM];
	INT		top;

};

#endif