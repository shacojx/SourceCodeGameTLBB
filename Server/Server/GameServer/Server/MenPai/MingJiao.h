///////////////////////////////////////////////////////////////////////////////
// 文件名：MingJiao.h
// 功能说明：门派:明教的接口声明
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef MINGJIAO_H
#define MINGJIAO_H

#include "MenPai.h"

namespace MenPai_Module
{
	class MingJiao_T: public MenPai_T
	{
		public:
			enum
			{
				ID= MenPai_T::ID_MINGJIAO,
			};
			
			MingJiao_T(){};
			~MingJiao_T(){};
			MenPai_T::MenPaiID_T GetID(VOID) const {return (MenPai_T::MenPaiID_T)ID;};
			//Add Rage And Strike point part here
			//event handler
			virtual VOID OnDamage(Obj_Human& rMe, INT nDamage) const{};
			virtual VOID OnDamageTarget(Obj_Human& rMe, INT nDamage) const;
		protected:
		private:
			INT TransferValueToRage(INT nDamage) const;
	};
};
#endif //MINGJIAO_H


