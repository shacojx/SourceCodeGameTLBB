///////////////////////////////////////////////////////////////////////////////
// 文件名：TianShan.h
// 功能说明：门派:天山的接口声明
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef TIANSHAN_H
#define TIANSHAN_H

#include "MenPai.h"

namespace MenPai_Module
{
	class TianShan_T: public MenPai_T
	{
		public:
			enum
			{
				ID= MenPai_T::ID_TIANSHAN ,
			};
			
			TianShan_T(){};
			~TianShan_T(){};
			MenPai_T::MenPaiID_T GetID(VOID) const {return (MenPai_T::MenPaiID_T)ID;};
			//Add Rage And Strike point part here

		protected:
		private:
	};
};
#endif //TIANSHAN_H


