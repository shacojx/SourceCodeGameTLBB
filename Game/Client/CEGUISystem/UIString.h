
/*
|
|	文本资源管理器
|
*/

#pragma once
#include "TDBasicType.h"
#include "TDUISystem.h"
#include <hash_map>
#include <set>
#include <CEGUIWindowManager.h>
#include "ceguiwindow.h"
#include <CEGUISystem.h>


struct UI_FACECODE
{
	const CHAR*		szCode;
	CEGUI::utf32	utfCode;
};

class CUIStringSystem
{
public:
	//转化字符串
	VOID				ParserString_Prebuild(const STRING& strSource, STRING& strOut);
	VOID				ParserString_Runtime(const STRING& strSource, CEGUI::String& strOut);
	STRING				ParserString_VarParam(const STRING& strID, va_list ptr);
	VOID				ParserString_NoColor(const STRING& strSrc, STRING&strOut, BOOL bControl = FALSE);
	STRING				ParserString_NoVarParam(const STRING& strID);

	//聊天模板字串的取得
	STRING				getTalkTemplate(const STRING& strKey, UINT index);
	STRING				ModifyChatTemplate(const STRING& strSrc, const STRING& strTalker, const STRING& strTarget);

	STRING				getTalkRandHelpMsg();

	//utf<->mbcs
	static VOID		mbcs_to_utf8(const std::string& strIn, std::string& strOut);
	static VOID		utf8_to_mbcs(const std::string& strIn, std::string& strOut);

public:
	//初始化
	VOID				Init(LPCTSTR szStringDBC = "../Config/Dictionary.txt");

	//过滤字符串中的非法字符
	BOOL	_CheckStringFilter(const STRING& strIn);
	//完全匹配过滤
	BOOL	_CheckStringFullCompare(const STRING& strIn, const STRING& strFilterType = "all", BOOL bUseAllTable = TRUE);
	//检查字符串的合法性
	BOOL	_CheckStringCode(const STRING& strIn, STRING& strOut);

public:
#ifdef __SGI_STL_PORT
	typedef std::hash_map< STRING, STRING >			DIRECTIONARY;
#else
	typedef stdext::hash_map< STRING, STRING >		DIRECTIONARY;
#endif

	typedef std::map< INT, CEGUI::utf32 >		CODE_CONVERT;
	typedef	std::vector< STRING >				CODE_FILTER;

	typedef std::vector< STRING >					UI_TALK_TEMPLATE;
	typedef std::map< STRING, UI_TALK_TEMPLATE >	TALK_TEMPLATE_MAP;
	typedef std::set<STRING>						UI_FULLCMP_TEMPLATE;
	typedef	std::map< STRING, UI_FULLCMP_TEMPLATE >	FULLCMP_MAP;

protected:
	//------------------------------------
	//字符串字典
	DIRECTIONARY		m_theDirectionary;
	CODE_CONVERT		m_mapCodeConvertTable;
	CODE_FILTER			m_vecFilterTable;
	TALK_TEMPLATE_MAP	m_talkTemplateTable;
	FULLCMP_MAP			m_fullCompareTable;

protected:
	std::pair< BOOL, DIRECTIONARY::iterator >	_IsValidDictionKey(const STRING& strKey);
	//检查字符串的合法性
	//提到public中
//	BOOL	_CheckStringCode(const STRING& strIn, STRING& strOut);
	
public:
	CUIStringSystem();
	virtual ~CUIStringSystem();

	static CUIStringSystem*	GetMe(VOID) { return s_pMe; }
private:
	VOID				ParserString_RuntimeNew(const STRING& strSource, CEGUI::String& strOut);
	VOID				GenerateTalkTemplateTable();
	VOID				GenerateFullCompareTable();
protected:
	static CUIStringSystem*	s_pMe;
};
