#include "WXSceneListener.h"

namespace WX
{
    void SceneListener::onSceneReset(void)
    {
    }

    void SceneListener::onAddObject(const ObjectPtr& object)
    {
    }

    void SceneListener::onRemoveObject(const ObjectPtr& object)
    {
    }

    void SceneListener::onRenameObject(const ObjectPtr& object, const String& oldName)
    {
    }

    void SceneListener::onSelectObject(const ObjectPtr& object)
    {
    }

    void SceneListener::onDeselectObject(const ObjectPtr& object)
    {
    }

    void SceneListener::onDeselectAllObjects(void)
    {
    }

    void SceneListener::onObjectPropertyChanged(const ObjectPtr& object, const String& name)
    {
    }

	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 不可行走区域编辑回调函数
	//
	// 

	// 添加一个新点
	void SceneListener::onRegionAddNewPoint(unsigned long ulRegionId,unsigned long ulPointId, Ogre::Vector3 position)
	{

	}

	// 删除一个点的回调函数
	void SceneListener::onRegionDelPoint(unsigned long ulRegionId,unsigned long ulPointId)
	{

	}

	
	// 选中一个区域的回调函数。
	void SceneListener::onRegionSel(unsigned long ulRegionId)
	{

	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 寻路编译
	//

	void SceneListener::onAddDijkPoint(unsigned long ulPointId, Ogre::Vector3 position)
	{

	}

	void SceneListener::onDelDijkPoint(unsigned long ulPointId)
	{

	}

	void SceneListener::onSelDijkPoint(unsigned long ulPointId)
	{

	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 怪物编辑回调函数
	//
	// 

	// 添加一个怪物
	void SceneListener::onAddNewMonster(unsigned long ulMonsterId, unsigned long ulInstanceId)
	{

	}

	// 删除一个怪物
	void SceneListener::onDelMonster( unsigned long ulMonsterId, unsigned long ulInstanceId)
	{

	}

	// 选择一个怪物
	void SceneListener::onSelMonster( unsigned long ulMonsterId, unsigned long ulInstanceId)
	{

	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 生长点编辑回调函数
	//
	void SceneListener::onAddNewGrowPoint(unsigned long ulTypeId, unsigned long ulInstanceId)
	{

	}

	// 删除一个怪物
	void SceneListener::onDelGrowPoint( unsigned long ulTypeId, unsigned long ulInstanceId)
	{

	}

	// 选择一个怪物
	void SceneListener::onSelGrowPoint( unsigned long ulTypeId, unsigned long ulInstanceId)
	{

	}

	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 事件区域编辑回调函数
	//
	
	void SceneListener::onAddNewRect(unsigned long ulTypeId, unsigned long ulInstanceId)
	{

	}

	// 删除
	void SceneListener::onDelRect( unsigned long ulTypeId, unsigned long ulInstanceId)
	{

	}

	// 选择
	void SceneListener::onSelRect( unsigned long ulTypeId, unsigned long ulInstanceId)
	{

	}

	//
	// 事件区域编辑回调函数
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 摊位编辑， 通知回调函数。
	//



	// 调用所有的添加事件响应函数。查看摊位信息
	void SceneListener::onViewStallInfo(
										unsigned char	bCanStall,	//是否可以摆摊
										unsigned char	iTradeTax,	//交易税
										unsigned long	dwPosTax,	//摊位费
										unsigned char	iExtraInfo	//附加信息 
										)
	{

	}
	
	//
	// 摊位编辑， 通知回调函数。
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 声音编辑， 通知回调函数。
	//



	// 调用所有的添加事件响应函数。查看声音信息
	void SceneListener::onViewCurSoundItem(
											unsigned long	ulInstance,	//声音实例id
											unsigned long	ulSoundType,//类型
											unsigned long	dwRadius	//有效半径 
											)
	{

	}//
	
	//
	// 声音编辑， 通知回调函数。
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////








    void SceneListener::onTerrainHeightChanged(const std::vector<TerrainInfo>& terrainInfo)
    {
    }

    void SceneListener::onCameraPosChanged()
    {
    }


    void SceneListener::onUIChanged(void* data, UIChangeType type)
    {

    }






    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// listener 管理器
	//
    SceneListenerManager::SceneListenerManager(void)
        : mListeners()
    {
    }

    SceneListenerManager::~SceneListenerManager()
    {
    }

    void SceneListenerManager::addSceneListener(SceneListener* listener)
    {
        mListeners.push_back(listener);
    }

    void SceneListenerManager::removeSceneListener(SceneListener* listener)
    {
        mListeners.remove(listener);
    }

    void SceneListenerManager::_fireSceneReset(SceneListener* exclude)
    {
        for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
                listener->onSceneReset();
        }
    }

    void SceneListenerManager::_fireAddObject(const ObjectPtr& object, SceneListener* exclude)
    {
        for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
                listener->onAddObject(object);
        }
    }

    void SceneListenerManager::_fireRemoveObject(const ObjectPtr& object, SceneListener* exclude)
    {
        for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
                listener->onRemoveObject(object);
        }
    }

    void SceneListenerManager::_fireRenameObject(const ObjectPtr& object, const String& oldName, SceneListener* exclude)
    {
        for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
                listener->onRenameObject(object, oldName);
        }
    }

    void SceneListenerManager::_fireSelectObject(const ObjectPtr& object, SceneListener* exclude)
    {
        for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
                listener->onSelectObject(object);
        }
    }

    void SceneListenerManager::_fireDeselectObject(const ObjectPtr& object, SceneListener* exclude)
    {
        for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
                listener->onDeselectObject(object);
        }
    }

    void SceneListenerManager::_fireDeselectAllObjects(SceneListener* exclude)
    {
        for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
                listener->onDeselectAllObjects();
        }
    }

    void SceneListenerManager::_fireObjectPropertyChanged(const ObjectPtr& object, const String& name, SceneListener* exclude)
    {
        for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
                listener->onObjectPropertyChanged(object, name);
        }
    }


	//////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 执行不可行走区域的回掉函数
	//

	// 添加一个新点
	void SceneListenerManager::_fireRegionAddNewPoint(unsigned long ulRegionId, unsigned long ulPointId, Ogre::Vector3 position, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
                listener->onRegionAddNewPoint(ulRegionId, ulPointId, position);
			}
        }
	}

	void SceneListenerManager::_fireRegionDelPoint(unsigned long ulRegionId, unsigned long ulPointId, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
				listener->onRegionDelPoint(ulRegionId, ulPointId);
			}
        }
	}

	// 选中一个区域
	void SceneListenerManager::_fireRegionSel(unsigned long ulRegionId, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
				listener->onRegionSel(ulRegionId);
			}
        }
	}



	//
	// 执行不可行走区域的回掉函数
	//
	//////////////////////////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 执行区域编辑添加点的回调函数
	//

	void SceneListenerManager::_fireAddDijkPoint(unsigned long ulPointId, Ogre::Vector3 position, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
		{
			SceneListener* listener = *it;
			if (listener != exclude)
			{
				listener->onAddDijkPoint(ulPointId, position);
			}
		}
	}

	void SceneListenerManager::_fireDelDijkPoint(unsigned long ulPointId, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
		{
			SceneListener* listener = *it;
			if (listener != exclude)
			{
				listener->onDelDijkPoint(ulPointId);
			}
		}
	}

	// 选中一个区域
	void SceneListenerManager::_fireSelDijkRegion(unsigned long ulPointId, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
		{
			SceneListener* listener = *it;
			if (listener != exclude)
			{
				listener->onSelDijkPoint(ulPointId);
			}
		}
	}

	//
	// 执行区域编辑添加点的回调函数
	//
	//////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 怪物编辑， 通知回调函数。
	//

	// 调用所有的添加怪物的事件响应函数。
	void SceneListenerManager::_fireAddNewMonster(unsigned long ulMonsterId, unsigned long ulInstanceId, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
				listener->onAddNewMonster(ulMonsterId, ulInstanceId);
			}
        }

	}

	// 调用所有的删除怪物的事件响应函数。
	void SceneListenerManager::_fireDelMonster( unsigned long ulMonsterId, unsigned long ulInstanceId, SceneListener* exclude)
	{

		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
				listener->onDelMonster(ulMonsterId, ulInstanceId);
			}
        }
	}


	// 调用所有的删除怪物的事件响应函数。
	void SceneListenerManager::_fireSelMonster( unsigned long ulMonsterId, unsigned long ulInstanceId, SceneListener* exclude)
	{

		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
				listener->onSelMonster(ulMonsterId, ulInstanceId);
			}
        }
	}


	//
	// 怪物编辑， 通知回调函数。
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 生长点编辑， 通知回调函数。
	//

		// 调用所有的添加的事件响应函数。
	void SceneListenerManager::_fireAddNewGrowPoint(unsigned long ulTypeId, unsigned long ulInstanceId, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
				listener->onAddNewGrowPoint(ulTypeId, ulInstanceId);
			}
        }
	}

		// 调用所有的删除的事件响应函数。
	void SceneListenerManager::_fireDelGrowPoint( unsigned long ulTypeId, unsigned long ulInstanceId, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
				listener->onDelGrowPoint(ulTypeId, ulInstanceId);
			}
        }

	}

		// 调用所有选择的事件响应函数
	void SceneListenerManager::_fireSelGrowPoint( unsigned long ulTypeId, unsigned long ulInstanceId, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
				listener->onDelMonster(ulTypeId, ulInstanceId);
			}
        }

	}

	//
	// 生长点编辑， 通知回调函数。
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 事件区域编辑， 通知回调函数。
	//



	// 调用所有的添加事件响应函数。
	void SceneListenerManager::_fireAddNewRect(unsigned long ulTypeId, unsigned long ulInstanceId, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
				listener->onAddNewRect(ulTypeId, ulInstanceId);
			}
        }
	}

	// 调用所有的删除事件响应函数。
	void SceneListenerManager::_fireDelRect( unsigned long ulTypeId, unsigned long ulInstanceId, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
				listener->onDelRect(ulTypeId, ulInstanceId);
			}
        }
	}

	// 调用所有选择事件响应函数
	void SceneListenerManager::_fireSelRect( unsigned long ulTypeId, unsigned long ulInstanceId, SceneListener* exclude)
	{
		for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
				listener->onSelRect(ulTypeId, ulInstanceId);
			}
        }
	}


	//
	// 事件区域编辑， 通知回调函数。
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 摊位编辑， 通知回调函数。
	//



	// 调用所有的添加事件响应函数。查看摊位信息
	void SceneListenerManager::_fireViewStallInfo(
													unsigned char	m_bCanStall,	//是否可以摆摊
													unsigned char	m_iTradeTax,	//交易税
													unsigned long	m_dwPosTax,		//摊位费
													unsigned char	m_iExtraInfo,	//附加信息 
													SceneListener*  exclude
													)
	{
		 for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
			{
                listener->onViewStallInfo(m_bCanStall, m_iTradeTax, m_dwPosTax, m_iExtraInfo);
			}
        }
	}


	//
	// 摊位编辑， 通知回调函数。
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


			
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 声音编辑， 通知回调函数。
		//


		// 调用所有的添加事件响应函数。查看声音信息
		void SceneListenerManager::_fireViewSoundInfo(
														unsigned long	ulInstance,	//声音实例id
														unsigned long	ulSoundType,//类型
														unsigned long	dwRadius,	//有效半径 
														SceneListener*  exclude
														)
		{
			for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
			{
				SceneListener* listener = *it;
				if (listener != exclude)
				{
					listener->onViewCurSoundItem(ulInstance, ulSoundType, dwRadius);
				}
			}
		}
		
		//
		// 声音编辑， 通知回调函数。
		//
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////









    void SceneListenerManager::_fireTerrainHeightChanged(const std::vector<TerrainInfo>& terrainInfo,SceneListener* exclude)
    {
        for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
                listener->onTerrainHeightChanged(terrainInfo);
        }
    }

    void SceneListenerManager::_fireCameraPosChanged(SceneListener* exclude)
    {
        for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
                listener->onCameraPosChanged();
        }
    }

    void SceneListenerManager::_fireUIChanged(void* data, UIChangeType type, SceneListener* exclude)
    {
        for (Listeners::const_iterator it = mListeners.begin(); it != mListeners.end(); ++it)
        {
            SceneListener* listener = *it;
            if (listener != exclude)
                listener->onUIChanged(data, type);
        }
    }

}
