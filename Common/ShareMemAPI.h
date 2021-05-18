#ifndef _SHAREMEM_API_H_
#define _SHAREMEM_API_H_
#include "Type.h"


#ifdef	__WINDOWS__
#define INVALID_SM_HANDLE	 ((VOID*)0)
#elif	 __LINUX__
#define INVALID_SM_HANDLE	 -1
#endif



namespace	ShareMemAPI
{

	/*创建ShareMem 内存区
	 *	
	 *	key   创建ShareMem 的关键值
	 *
	 *  Size  创建大小
	 *
	 *	返回 对应ShareMem保持值
	 */
	SMHandle		CreateShareMem(SM_KEY key,UINT Size);
	/*打开ShareMem 内存区
	 *	
	 * key   打开ShareMem 的关键值
	 * 
	 * Size  打开大小
	 *
	 * 返回  对应ShareMem 保持值
	 */
	SMHandle		OpenShareMem(SM_KEY key,UINT Size);
	
	/*映射ShareMem 内存区
	 *	
	 *	handle 映射ShareMem 的保持值
	 *
	 *  返回 ShareMem 的数据指针
	 */
	CHAR*			MapShareMem(SMHandle handle);
	
	/*关闭映射 ShareMem 内存区
	 *
	 *	MemoryPtr			ShareMem 的数据指针
	 *	
	 *  
	 */	
	VOID			UnMapShareMem(CHAR* MemoryPtr);
	
	/*	关闭ShareMem
	 * 	handle  需要关闭的ShareMem 保持值
	 */
	VOID			CloseShareMem(SMHandle handle);


}


#endif

