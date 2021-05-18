///////////////////////////////////////////////////////////////////////////////
// 文件名：XiaoYao.h
// 功能说明：门派:逍遥的接口声明
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef XIAOYAO_H
#define XIAOYAO_H

#include "MenPai.h"

namespace MenPai_Module
{
	class XiaoYao_T: public MenPai_T
	{
		public:
			enum
			{
				ID= MenPai_T::ID_XIAOYAO,
			};
			
			XiaoYao_T(){};
			~XiaoYao_T(){};
			MenPai_T::MenPaiID_T GetID(VOID) const {return (MenPai_T::MenPaiID_T)ID;};
			//Add Rage And Strike point part here
			virtual VOID OnMyTrapActivated(Obj_Human& rMe, Obj_Special& rTrap) const;

		protected:
		private:
			virtual INT TransferValueToRage(INT nDamage) const;
	};
};
#endif //XIAOYAO_H


