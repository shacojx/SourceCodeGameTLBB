/*************************************************************************
 文件名	: 	AIScript.h
 版本号 :	0.0.2
 功  能	:	针对特有的扩展AI脚本编写的解析及执行类
 修改记录:	修改了前版接口函数扩展性不强的缺点
 *************************************************************************/

#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#include "Type.h"
#include "Ini.h"
#include "AIScriptDef.h"

class MonsterAI;
class AI_Character;
class AIScriptParser;
class AIScriptExecutor;

class AIScript
{
public:

	AIScript();
	~AIScript();

	/** Desc	: 解析Pet脚步文件
	 *  @param	: filename 脚本文件名
	 *  @return	: 解析结果
	 */
	BOOL			ParseScriptForPet(const CHAR* filename);
	/** Desc	: 解析由文件读取的脚本内容
	 *  @param	: filename 脚本文件名
	 *  @return	: 解析结果
	 */
	BOOL			ParseScript(const CHAR* filename);
	/** Desc	: 处理脚本，即执行脚本
	 *  @param	: nState 执行哪个Section的脚本,如：ATTACK,APPROACH...
	 *  @return	: 处理结果
	 */
	BOOL			ProcessScript(INT nState, AI_Character* pAI);
	/** Desc	: 设置自身的AI_Character数据
	 *  @param	: pAI 将要挂接的AI_Character对象指针
	 *  @return	: 空
	 */
	//VOID			SetAI(AI_Character* pAI) { /*m_pAI = pAI;*/ }
	/** Desc	: 重置位m_vStateLists,即所有脚本数据复位
	 *  @param	: 空
	 *  @return	: 空
	 */
	VOID			ResetStateList(const AI_Character* pAI);

	/// 技能错误码相关
	//VOID			SetSkillErrorCode(ORESULT oResult) { m_SkillErrorCode = oResult; }
	//ORESULT			GetSkillErrorCode(VOID) const { return m_SkillErrorCode; }

	/////////////////////////////////////////
	//VOID			SetSkillID(INT idSkill) { m_SkillID = idSkill; }
	//INT				GetSkillID(VOID) const { return m_SkillID; }

protected:
	/// 脚本执行函数
	BOOL			ExcuteCdtScript(ConditionTree* p, const AI_Character* pAI);
	BOOL			ExcuteToDoScript(TreeNodeList* p, const AI_Character* pAI);
	BOOL			ExcuteExpress(const TreeNode* p, const AI_Character* pAI);
	BOOL			ExcuteToDoExpress(const TreeNode* p, const AI_Character* pAI);
	/// 脚本解析函数
	BOOL			ParseOneScript(const CHAR* szScript);
	INT				ParseCHAR(const CHAR* szch);
	INT				ParseCommand(const CHAR* szch);
	TreeNode		ParseExpress(Atom atom);
	BOOL			VerifyExpress(const CHAR* szch);
	/// 由脚本到预处理的脚步数据结构的中间函数
	BOOL			ProcessOptorQ(VOID);
	BOOL			ProcessCdtQ(VOID);
	BOOL			ProcessToDoQ(VOID);
	BOOL			CreateCdtTree(VOID);
	BOOL			CreateCdtNode(VOID);
	BOOL			InsertCdtNodeToCdtList(VOID);
	BOOL			InsertToNodeList(Atom atom,INT flag);
	
	VOID			ClearQ(VOID);
	VOID			ClearList(VOID);
	VOID			ClearTree(ConditionTree* p);
	VOID			ClearNode(ConditionNode* p);
	VOID			ClearCdtList(VOID);
	/// 操作条件List的相关函数
	ConditionList*	GetCdtList(VOID) const { return m_pcdtList; }
	VOID			InitAllStateList(VOID);
	VOID			InitList(ENUM_AISTATE stateIndex) { m_vStateLists[stateIndex] = m_pcdtList; m_pcdtList = NULL; }
	/// 其他的辅助成员函数
	VOID			VerdictState(ConditionTree* t, ConditionNode* p, INT& ret);
	VOID			SetParam(const CHAR* szCmd, Atom& atom);
	BOOL			IsParam(const CHAR* szCmd);
	VOID			ProcessCtdOrToDoQ(const CHAR* szCmd, BOOL bToDoFlag);
	BOOL			FindParam(const CHAR* szCmd, INT index, CHAR* pString);
	VOID			ReSetTimes(VOID);
	VOID			ReSetFlags(VOID);


private:

	Ini					m_file;
	ConditionList*		m_pcdtList;		//为中间变量，为各种操作提供方便
	ConditionList*		m_pcdtList2;	//真正的Common部分List的存储指针
	ConditionList*		m_vStateLists[ONDEADSECTION+1];

	OperatorQueue		m_operatorQ;
	CdtExpressionQueue	m_cdtExpQ;
	ToDoExpressionQueue m_todoExpQ;

	TreeNodeList*		m_poptorNodeList;
	TreeNodeList*		m_pcdtNodeList;
	TreeNodeList*		m_ptodoNodeList;

	ConditionTree*		m_pcdtTree;
	ConditionNode*		m_pcdtNode;
	BracketArray		m_array;
	Stack				m_stack;
	INT					m_nID;
	INT					m_nPrev;
	INT					m_fX;
	INT					m_fZ;
	//INT					m_SkillID;
	//INT					m_nSummonCount;
	//INT					m_nSpeakID;
	//FLOAT					m_fNeedGoDist;
	// 技能错误码
	//ORESULT				m_SkillErrorCode;

};

#endif