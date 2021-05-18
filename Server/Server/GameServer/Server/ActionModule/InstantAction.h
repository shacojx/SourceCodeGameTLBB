///////////////////////////////////////////////////////////////////////////////
// 文件名：InstantAction.h
// 功能说明：瞬发动作类
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef INSTANTACTION_H
#define INSTANTACTION_H
#include "Type.h"
#include "ActionLogic.h"

namespace Action_Module
{
	class InstantActionLogic_T : public ActionLogic_T
	{
		public:
			enum
			{
				ID = ACTION_INSTANT,
			};
			InstantActionLogic_T(VOID) {};
			~InstantActionLogic_T(VOID) {};
			virtual INT GetLogicID(VOID) const {return ID;}
		protected:
		private:
	};
	
}
#endif //INSTANTACTION_H
