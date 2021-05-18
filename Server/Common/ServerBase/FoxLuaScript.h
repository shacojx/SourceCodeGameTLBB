//---------------------------------------------------------------------------
// Script Engine by FireFox
// Desc:	
//---------------------------------------------------------------------------
#ifndef FoxLuaScript_H
#define FoxLuaScript_H
//---------------------------------------------------------------------------
#include "FoxScript.h"
#ifndef __linux
	#include "LuaLib.h"
#else
	#include "LuaLib.h"
	#include <string.h>
#endif

#define MAINFUNCTIONNAME "main"
#define LUA_CREATE_ERROR 1
#define LUA_SCRIPT_LEN_ERROR 2
#define LUA_SCRIPT_COMPILE_ERROR 3
#define LUA_SCRIPT_EXECUTE_ERROR 4
#define LUA_SCRIPT_NOT_NUMBER_ERROR 10
#define LUA_SCRIPT_NOT_STRING_ERROR 11
#define LUA_SCRIPT_NOT_TABLE_ERROR 12
#define LUA_SCRIPT_STATES_IS_NULL 20

typedef struct  {
	const char *name;
	lua_CFunction func;
	
}TLua_Funcs;

//---------------------------------------------------------------------------
class ENGINE_API FoxLuaScript : public FoxScript
{
public:
	FoxLuaScript();
	virtual ~FoxLuaScript();
	//FoxLuaScript( int StackSize);
	
	int		Activate(){Execute(); return 1;};
	virtual BOOL Init();
	virtual void Exit();
	virtual BOOL Load(char* FileName);
	virtual	BOOL Compile(char* FileName);
	virtual	BOOL Execute();
	virtual	BOOL CallFunction(LPSTR cFuncName, int nResults, LPSTR cFormat, ...);
	virtual BOOL RegisterFunction(LPSTR FuncName, void* Func);
	virtual void SetScriptName(LPSTR scpname){strcpy(m_szScriptName, scpname);};

	int	 GetUserTag() { return m_UserTag; };
	BOOL RegisterFunctions( TLua_Funcs Funcs[], int n = 0 );
	BOOL LoadBuffer(PBYTE pBuffer, DWORD dwLen );
	void SafeCallBegin(int * pIndex);//记录当前堆栈栈顶位置，并保存到pIndex指向的数据中
	void SafeCallEnd (int nIndex);//恢复至调用之前栈顶位置。
	BOOL GetValuesFromStack(char * cFormat , ...);			
	
	BOOL Stop();
	BOOL Resume();
	DWORD CreateTable();
	DWORD ModifyTable(LPSTR szTableName); 
	void SetGlobalName(LPSTR szName);
	inline void SetTableMember(int nIndex, int Id, const char * szString){Lua_SetTable_StringFromId(m_LuaState,  nIndex, Id, szString);};
	inline void SetTableMember(int nIndex, int Id, double nNumber){Lua_SetTable_DoubleFromId(m_LuaState,  nIndex, Id, nNumber);};
	inline void SetTableMember(int nIndex, int Id, int nNumber){Lua_SetTable_IntFromId(m_LuaState,  nIndex, Id, nNumber);};
	inline void SetTableMember(int nIndex, const char * szMemberName, Lua_CFunction CFun){Lua_SetTable_CFunFromName(m_LuaState,  nIndex, szMemberName, CFun);};
	inline void SetTableMember(int nIndex, int nId, Lua_CFunction CFun){Lua_SetTable_CFunFromId(m_LuaState,  nIndex, nId, CFun);};
	inline void SetTableMember(int nIndex, const char * szMemberName, char * szString){Lua_SetTable_StringFromName(m_LuaState,  nIndex, szMemberName, szString);};
	inline void SetTableMember(int nIndex, const char * szMemberName, int Number){Lua_SetTable_IntFromName(m_LuaState,  nIndex, szMemberName, Number);};
	inline void SetTableMember(int nIndex, const char * szMemberName, double Number){Lua_SetTable_DoubleFromName(m_LuaState,  nIndex, szMemberName, Number);};
	void ScriptError(int Error);
	void ScriptError(int , int);
	BOOL ExecuteCode();

//private:
	void RegisterStandardFunctions();
	BOOL m_IsRuning;//是否该脚本有效
	char m_szScriptName[100];
	int	 m_UserTag;
	Lua_State * m_LuaState;
private:
	BOOL CallFunction(LPSTR cFuncName, int nResults, LPSTR cFormat, va_list vlist);

};
//---------------------------------------------------------------------------
#endif //FoxLuaScript_H
