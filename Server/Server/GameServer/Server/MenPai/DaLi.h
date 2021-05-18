///////////////////////////////////////////////////////////////////////////////
// 文件名：DaLi.h
// 功能说明：门派:大理的接口声明
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef DALI_H
#define DALI_H
#include "MenPai.h"
namespace MenPai_Module
{
	class DaLi_T: public MenPai_T
	{
		public:
			enum
			{
				ID= MenPai_T::ID_DALI,
			};
			
			DaLi_T(){};
			~DaLi_T(){};
			MenPai_T::MenPaiID_T GetID(VOID) const {return (MenPai_T::MenPaiID_T)ID;};
			//Add Rage And Strike point part here
			
		protected:
		private:
	};
};
#endif //DALI_H

