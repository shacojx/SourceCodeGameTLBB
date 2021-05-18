///////////////////////////////////////////////////////////////////////////////
// 文件名：WuMenPai.h
// 功能说明：门派:新手门派（不属于任何门派）接口声明
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WUMENPAI_H
#define WUMENPAI_H

#include "MenPai.h"

namespace MenPai_Module
{
	class WuMenPai_T: public MenPai_T
	{
		public:
			enum
			{
				ID= MenPai_T::ID_WUMENPAI,
			};
			
			WuMenPai_T(){};
			~WuMenPai_T(){};
			MenPai_T::MenPaiID_T GetID(VOID) const {return (MenPai_T::MenPaiID_T)ID;};
			//Add Rage And Strike point part here

		protected:
		private:
	};
};
#endif //WUMENPAI_H


