///////////////////////////////////////////////////////////////////////////////
// 文件名：EventCore.h
// 功能说明：事件缓存逻辑内核
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef EVENTCORE_H
#define EVENTCORE_H

#include "Type.h"
#include "EventStruct.h"
#include "GameDefine.h"
#include "GameDefine2.h"
#include "GameStruct.h"
#include "GameUtil.h"
#include "GameStruct_Skill.h"
#include "GameStruct_Impact.h"

class Scene;
namespace Event_Module
{
	class EventCore_T
	{
		public:
			typedef ResizableVector_T<Event_T, 4096> EventVector_T;
			typedef ResizableVector_T<OWN_IMPACT, 2048> ImpactVector_T;
			enum
			{
				EVENT_QUEUE_SIZE = 2048,
				IMPACT_LIST_SIZE = 1024
			};
			EventCore_T() : 
				m_pScene(0),
				m_nCountOfEvents(0),
				m_nCountOfImpacts(0),
				m_nMaxCountOfEvent(0),
				m_nMaxCountOfImpacts(0)
			{}
			~EventCore_T(){}
			BOOL Init(Scene* pScene);
			VOID CleanUp(VOID);
			VOID Reset(VOID) {};
			VOID HeartBeat(Time_t nDeltaTime);
			BOOL RegisterImpactEvent(ObjID_t nReceiver, ObjID_t nSender, OWN_IMPACT& rImp, Time_t nDelayTime, SkillID_t nSkillID = INVALID_ID);
			BOOL RegisterBeSkillEvent(ObjID_t nReceiver, ObjID_t nSender, INT nBehaviorType, Time_t nDelayTime);
			BOOL RegisterSkillMissEvent(ObjID_t nReceiver, ObjID_t nSender, SkillID_t nSkill, Time_t nDelayTime);
			BOOL RegisterActiveSpecialObjEvent(ObjID_t nObj, Time_t nDelayTime);
			BOOL RegisterSkillHitEvent(ObjID_t nReceiver, ObjID_t nSender, SkillID_t nSkill, Time_t nDelayTime);

		protected:
			INT GetIndexOfFirstAvailableEventSlot(VOID);
			INT GetIndexOfFirstAvailableImpactSlot(VOID);
			BOOL ResizeAllVector(VOID);
		private:
			EventVector_T m_EventQueue;
			ImpactVector_T m_ImpactQueue;
			INT m_nCountOfEvents;
			INT m_nCountOfImpacts;
			INT m_nMaxCountOfEvent;
			INT m_nMaxCountOfImpacts;
			Scene* m_pScene;
	};
};

#endif //EVENTCORE_H
