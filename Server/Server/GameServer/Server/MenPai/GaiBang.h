///////////////////////////////////////////////////////////////////////////////
// 文件名：GaiBang.h
// 功能说明：门派:丐帮的接口声明
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef GAIBANG_H
#define GAIBANG_H

#include "MenPai.h"

namespace MenPai_Module
{
	class GaiBang_T: public MenPai_T
	{
		public:
			enum
			{
				ID= MenPai_T::ID_GAIBANG,
			};
			
			GaiBang_T(){};
			~GaiBang_T(){};
			MenPai_T::MenPaiID_T GetID(VOID) const {return (MenPai_T::MenPaiID_T)ID;};
			//Add Rage And Strike point part here
			VOID OnDepleteStrikePoints(Obj_Human& rMe, INT nStrikePoint) const;
		protected:
		private:
			INT TransferValueToRage(INT nDamage) const;
	};
};
#endif //GAIBANG_H


