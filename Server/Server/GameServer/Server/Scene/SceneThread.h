

#ifndef __SCENETHREAD_H__
#define __SCENETHREAD_H__

#include "Type.h"
#include "Thread.h"
#include "Scene.h"

#define MAX_SCENE_PER_THREAD 128

//场景线程
//每个场景由一个独立的线程来驱动执行
class SceneThread : public Thread
{
public :
	SceneThread( ) ;
	~SceneThread( ) ;

	//线程执行主循环
	virtual VOID	run( ) ;

	//停止执行
	//调用此接口后不是马上线程就能停止，而且在下一个循环操作时退出
	virtual VOID	stop( ) { m_Active = FALSE ; } ;

	//判断当前线程是否有效，如果无效，则退出循环
	BOOL			IsActive( ){ return m_Active ; } ;

	VOID			Quit( ) ;

	BOOL			AddScene( Scene* pScene ) ;

private :
	Scene*		m_apScene[MAX_SCENE_PER_THREAD] ;
	INT			m_nSceneCount ;
	BOOL		m_Active ;



};




#endif
