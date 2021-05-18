///////////////////////////////////////////////////////////////////////////////
// 文件名：WuDang.h
// 功能说明：门派:武当的接口声明
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WUDANG_H
#define WUDANG_H

#include "MenPai.h"

namespace MenPai_Module
{
	class WuDang_T: public MenPai_T
	{
		public:
			enum
			{
				ID= MenPai_T::ID_WUDANG,
			};
			
			WuDang_T(){};
			~WuDang_T(){};
			MenPai_T::MenPaiID_T GetID(VOID) const {return (MenPai_T::MenPaiID_T)ID;};
			//Add Rage And Strike point part here

		protected:
		private:
	};
};
#endif //WUDANG_H


