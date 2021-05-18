//-----------------------------------------------------------------------------
// 文件名 : LuaFnMacro.h
// 模块	:	Script
// 功能	 :  定义脚本系统需要的宏
// 修改历史:
//-----------------------------------------------------------------------------
#ifndef __LUAFNMACRO_H__
#define __LUAFNMACRO_H__


namespace LuaFnTbl
{
#ifdef  __WINDOWS__
	#define BEGINSCENEDEFINE(FUNCNAME) 	 { \
		Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));	AssertEx(pScene,"场景ID出错 "##FUNCNAME);	if(pScene) {	\
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;\
		}
	#define ENDSCENEDEFINE } 

	#define BEGINCHARCTERDEFINE(FUNCNAME) 	 { \
		Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));	AssertEx(pScene,"场景ID出错 "##FUNCNAME);	if(pScene) { \
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;\
		Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(selfId));	AssertEx(pObj,"selfId出错 "##FUNCNAME);		if(pObj && IsCharacterObj( pObj->GetObjType() )) { \
		Obj_Character* pCharacter = (Obj_Character*)pObj;			AssertEx(pCharacter,"Obj到Obj_Character指针转换出错 "##FUNCNAME);			if(pCharacter) {\
		}
	#define ENDCHARCTERDEFINE 		}	} }

	#define  BEGINHUMANDEFINE(FUNCNAME) 	 { \
		Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));		AssertEx(pScene,"场景ID出错 "##FUNCNAME);   if(pScene) { \
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;\
		Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(selfId));		AssertEx(pObj,"selfId出错 "##FUNCNAME);		if(pObj && IsCharacterObj( pObj->GetObjType() )) { \
		Obj_Character* pCharacter = (Obj_Character*)pObj;			AssertEx(pCharacter,"Obj到Obj_Character指针转换出错 "##FUNCNAME);			if(pCharacter && pCharacter->GetObjType() == Obj::OBJ_TYPE_HUMAN) { \
		Obj_Human* pHuman = (Obj_Human*)pCharacter; AssertEx(pHuman,"Obj_Character到Obj_Human指针转换出错 "##FUNCNAME);if(pHuman) {\
		}
	#define  ENDHUMANDEFINE }	}	}	} 

	#define  BEGINMONSTERDEFINE(FUNCNAME) 	 {\
		Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));	AssertEx(pScene,"场景ID出错 "##FUNCNAME);	if(pScene) {\
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;\
		Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(selfId));    	AssertEx(pObj,"selfId出错 "##FUNCNAME);		if(pObj && IsCharacterObj( pObj->GetObjType() )) {\
		Obj_Character* pCharacter = (Obj_Character*)pObj;			AssertEx(pCharacter,"Obj到Obj_Character指针转换出错 "##FUNCNAME);			if(pCharacter && pCharacter->GetObjType() == Obj::OBJ_TYPE_MONSTER) { \
		Obj_Monster* pMonster = (Obj_Monster*)pCharacter;	AssertEx(pMonster,"Obj_Character到Obj_Monster指针转换出错 "##FUNCNAME);	if(pMonster) {\
		}
	#define  ENDMONSTERDEFINE }	}	}	} 
#elif __LINUX__
	#define BEGINSCENEDEFINE(FUNCNAME) 	 { \
		Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));	AssertEx(pScene,"场景ID出错 "FUNCNAME);	if(pScene) {	\
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;\
		}
	#define ENDSCENEDEFINE } 

	#define BEGINCHARCTERDEFINE(FUNCNAME) 	 { \
		Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));	AssertEx(pScene,"场景ID出错 "FUNCNAME);	if(pScene) { \
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;\
		Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(selfId));	AssertEx(pObj,"selfId出错 "FUNCNAME);		if(pObj && IsCharacterObj( pObj->GetObjType() )) { \
		Obj_Character* pCharacter = (Obj_Character*)pObj;			AssertEx(pCharacter,"Obj到Obj_Character指针转换出错 "FUNCNAME);			if(pCharacter) {\
		}
	#define ENDCHARCTERDEFINE 		}	} }

	#define  BEGINHUMANDEFINE(FUNCNAME) 	 { \
		Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));		AssertEx(pScene,"场景ID出错 "FUNCNAME);   if(pScene) { \
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;\
		Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(selfId));		AssertEx(pObj,"selfId出错 "FUNCNAME);		if(pObj && IsCharacterObj( pObj->GetObjType() )) { \
		Obj_Character* pCharacter = (Obj_Character*)pObj;			AssertEx(pCharacter,"Obj到Obj_Character指针转换出错 "FUNCNAME);			if(pCharacter && pCharacter->GetObjType() == Obj::OBJ_TYPE_HUMAN) { \
		Obj_Human* pHuman = (Obj_Human*)pCharacter; AssertEx(pHuman,"Obj_Character到Obj_Human指针转换出错 "FUNCNAME);if(pHuman) {\
		}
	#define  ENDHUMANDEFINE }	}	}	} 

	#define  BEGINMONSTERDEFINE(FUNCNAME) 	 {\
		Scene* pScene = (Scene*)(g_pSceneManager->GetScene(sceneId));	AssertEx(pScene,"场景ID出错 "FUNCNAME);	if(pScene) {\
		Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;\
		Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(selfId));    	AssertEx(pObj,"selfId出错 "FUNCNAME);		if(pObj && IsCharacterObj( pObj->GetObjType() )) {\
		Obj_Character* pCharacter = (Obj_Character*)pObj;			AssertEx(pCharacter,"Obj到Obj_Character指针转换出错 "FUNCNAME);			if(pCharacter && pCharacter->GetObjType() == Obj::OBJ_TYPE_MONSTER) { \
		Obj_Monster* pMonster = (Obj_Monster*)pCharacter;	AssertEx(pMonster,"Obj_Character到Obj_Monster指针转换出错 "FUNCNAME);	if(pMonster) {\
		}
	#define  ENDMONSTERDEFINE }	}	}	} 

#endif
	

#define RETURNFALSE {	Lua_PushNumber(L,-1);	return 1;}
}

#pragma  warning(disable : 4244)



//采用Quake3的strncpy函数
VOID Q_strncpyz( CHAR *dest, const CHAR *src, INT destsize ) 
{
	__ENTER_FUNCTION

	if ( !src ) {
		AssertEx(src,"null pointer in Q_strncpyz");
		return;
	}
	if ( destsize < 1 ) {
		AssertEx(destsize>0,"dest buffer is null in Q_strncpyz");
		return;
	}

	strncpy( dest, src, destsize-1 );
	dest[destsize-1] = 0;

	__LEAVE_FUNCTION
}

#endif
