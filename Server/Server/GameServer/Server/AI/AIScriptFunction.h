/*************************************************************************
 文件名	: 	AIScriptFunction.h
 版本号 :	0.0.1
 功  能	:	AIScript的专用函数
 修改记录:	
 *************************************************************************/


#ifndef _AISCRIPTFUNCTION_H_
#define _AISCRIPTFUNCTION_H_

class Obj_Character;

typedef INT (*PLAISFunc)( const Obj_Character *pChar, INT param0, INT param1, INT param2 );
#define REGISTERAISFUNC(funcname) {#funcname,PLAISFunc(funcname)},

struct AISFuncMap
{
	CHAR* funcname;
	PLAISFunc Func;
};

struct AISMacroMap
{
	CHAR* Macroname;
	INT	  MacroID;
};

const PLAISFunc GetFuncPtrbyName( const CHAR* funcName );
const CHAR*		GetFuncNamebyPtr( const PLAISFunc FuncPtr );
const INT		GetMacrobyName( const CHAR* MacroName );
// 接口部分
//////////////////////////////////////////////////////////////////////////
INT	 AIS_GetBuffState( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT	 AIS_GetAIState( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_GetHP( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_GetMP( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_Rand( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_GetEBuffState( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_GetEMP( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_GetEHP( const Obj_Character *pChar, INT param0, INT param1, INT param2 );	

INT  AIS_GetEMenPai( const Obj_Character *pChar, INT param0, INT param1, INT param2 );	

INT  AIS_GetELevel( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_IsCanSpeak( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_IsHasEnemy( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_GetESex( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_GetCamp(const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_GetPK( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_GetETeamNum( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_IsEHasSpecialItem( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_GetSingleDamage( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_GetEnemyNum( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_IsBoss( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_BeSkill( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_ToFlee( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_IsCanSkill( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_ToSkill( const Obj_Character *pChar, INT param0, INT param1, INT param2 );	

INT  AIS_ToSummon( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_ToLongAttack( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_ToFollow( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_ToSpeak( const Obj_Character *pChar, INT param0, INT param1, INT param2 );	

INT  AIS_SetTimes( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_SetPRI( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

INT  AIS_ToAssistTeammate( const Obj_Character *pChar, INT param0, INT param1, INT param2 );

//INT  AIS_PetToAttack( const Obj_Character *pChar, INT param0, INT param1, INT param2 );
	   

#endif