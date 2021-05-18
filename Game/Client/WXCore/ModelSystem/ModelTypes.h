#ifndef _Model_Types_h_
#define _Model_Types_h_



// 名字空间定义

#define Begin_Model namespace ModelSystem  \
{

#define End_Model	};


// API 定义

#ifdef MODELSYSTEM_STATIC_LINK
#define MODEL_API
#else
#ifndef MODELSYSTEM_EXPORTS
#define MODEL_API		__declspec(dllimport)
#else
#define MODEL_API		__declspec(dllexport)
#endif 
#endif

//Ogre头文件引用



#endif