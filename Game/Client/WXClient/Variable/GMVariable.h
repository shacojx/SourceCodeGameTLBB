/****************************************\
*										*
* 			  配置参数系统				*
*										*
\****************************************/
#pragma once
#include "TDVariable.h"

namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}

class CVariableSystem : public tVariableSystem
{
public:
	//得到某个变量的值，如果不存在，则返回FALSE
	virtual BOOL	GetVariable(LPCTSTR szName, STRING& strValue)		;	
	//设置某个变量的值，如果不存在，则首先创建
	virtual VOID	SetVariable(LPCTSTR szName, LPCTSTR szValue, BOOL bTemp=TRUE, BOOL bFireEvent=TRUE)	;
	//设置某个变量的值，但并不马上生效, 下一次启动时才会生效
	virtual VOID	SetVariableDelay(LPCTSTR szName, LPCTSTR szValue)	;

	//-------------------------------------------------------------------
	//快速设置通道

	//整数
	virtual VOID	SetAs_Int(LPCTSTR szName, INT nValue, BOOL bTemp=TRUE)				;
	//浮点数
	virtual VOID	SetAs_Float(LPCTSTR szName, FLOAT fValue, BOOL bTemp=TRUE)			;
	//Vector2
	virtual VOID	SetAs_Vector2(LPCTSTR szName, FLOAT fX, FLOAT fY, BOOL bTemp=TRUE)	;

	//-------------------------------------------------------------------
	//快速获取通道

	//字符串, DEF=""
	virtual const STRING&	GetAs_String(LPCTSTR szName, BOOL* bHave = NULL)	;
	//整数, DEF=0
	virtual INT				GetAs_Int(LPCTSTR szName, BOOL* bHave = NULL)		;
	//浮点数, DEF=0.0f
	virtual FLOAT			GetAs_Float(LPCTSTR szName, BOOL* bHave = NULL)		;
	//Vector2, DEF=(0.0f, 0.0f)
	virtual fVector2		GetAs_Vector2(LPCTSTR szName, BOOL* bHave = NULL)	;

	//-------------------------------------------------------------------
	//设置尚未设置的变量,如果该变量已经定义，此函数无效
	virtual void			SetVariableDefault(LPCTSTR szName, LPCTSTR szValue);

public:
	//用于脚本调用的函数
	INT Lua_SetGlobalVariable(LuaPlus::LuaState* state);	//设置全局变量
	INT Lua_GetGlobalVariable(LuaPlus::LuaState* state);	//取得全局变量
	INT	Lua_SetGlobalVariableDelay(LuaPlus::LuaState* state);//设置全局变量重起后生效
protected:
	LuaPlus::LuaObject* m_pMetaTable;

protected:
	struct VARIABLE
	{
		STRING	vValue;	//变量值
		BOOL	bTemp;	//临时变量
	};
#ifdef __SGI_STL_PORT
	typedef std::hash_map< STRING, VARIABLE >	VARIABLE_MAP;
#else
	typedef stdext::hash_map< STRING, VARIABLE >	VARIABLE_MAP;
#endif

	//从文件中读取变量
	VOID	LoadVariable(LPCTSTR szFileName, VARIABLE_MAP& mapBuf);
	//将变量保存到文件中
	VOID	SaveVariable(LPCTSTR szFileName, VARIABLE_MAP& mapBuf);

public:
	virtual VOID	Initial(VOID*);
	virtual VOID	Release(VOID);

protected:
	//当前保存的变量
	VARIABLE_MAP		m_mapCurrent;
	//游戏退出时更改并保存的变量,以便下一次游戏启动生效
	VARIABLE_MAP		m_mapNeedSave;

public:
	CVariableSystem();
	virtual ~CVariableSystem();
	static CVariableSystem* GetMe(VOID) { return s_pMe; }

protected:
	static CVariableSystem*		s_pMe;

	WX_DECLARE_DYNAMIC(CVariableSystem);
};
