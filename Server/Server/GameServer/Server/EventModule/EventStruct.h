///////////////////////////////////////////////////////////////////////////////
// 文件名：EventStruct.h
// 功能说明：事件的缓存结构
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef EVENTSTRUCT_H
#define EVENTSTRUCT_H

#include "Type.h"
#include "GameStruct.h"
#include "GameStruct_Impact.h"

namespace Event_Module
{
	class Event_T
	{
		public:
			typedef enum
			{
				TYPE_INVALID = -1,
				TYPE_IMPACT = 0,
				TYPE_ACTIVE_SPECIAL_OBJ,
				TYPE_BESKILL,
			}ParamType_T;
			enum
			{
				NUM_OF_PARAMS = 2,
			};
			Event_T() : 
				m_nSender(INVALID_ID),
				m_nReceiver(INVALID_ID),
				m_nDelayTime(0),
				m_nType(TYPE_INVALID),
				m_nParam(INVALID_ID)
			{
			};
			~Event_T() {};
			VOID Reset(VOID)
			{
				m_nSender = INVALID_ID;
				m_nReceiver = INVALID_ID;
				m_nDelayTime = 0;
				m_nType = TYPE_INVALID;
				m_nParam = INVALID_ID;
			};
			BOOL IsUsed(VOID) const {return (TYPE_INVALID!=m_nType);}
			BOOL IsAvailable(VOID) const {return (TYPE_INVALID==m_nType);}
			ObjID_t	GetSender(VOID) const {return m_nSender;}
			VOID SetSender(ObjID_t nID) {m_nSender =nID;}
			ObjID_t GetReceiver(VOID) const {return m_nReceiver;}
			VOID SetReceiver(ObjID_t nID) {m_nReceiver = nID;}
			Time_t	GetDelayTime(VOID) const {return m_nDelayTime;}
			VOID SetDelayTime(Time_t nTime) {m_nDelayTime = nTime;}
			ParamType_T	GetType(VOID) const {return m_nType;}
			VOID SetType(ParamType_T nType) {m_nType=nType;}
			INT GetParam(VOID) const {return m_nParam;} 
			VOID SetParam(INT nIdx, INT nValue) {m_nParam = nValue;}
		protected:
		private:
			ObjID_t m_nSender;
			ObjID_t m_nReceiver;
			Time_t 	m_nDelayTime;
			ParamType_T m_nType;
			INT		m_nParam;
	};
};

#endif //EVENTSTRUCT_H
