///////////////////////////////////////////////////////////////////////////////
// 文件名：XingXiu.h
// 功能说明：门派:大理的接口声明
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef XINGXIU_H
#define XINGXIU_H

#include "MenPai.h"

namespace MenPai_Module
{
	class XingXiu_T: public MenPai_T
	{
		public:
			enum
			{
				ID= MenPai_T::ID_XINGXIU,
			};
			
			XingXiu_T(){};
			~XingXiu_T(){};
			MenPai_T::MenPaiID_T GetID(VOID) const {return (MenPai_T::MenPaiID_T)ID;};
			//Add Rage And Strike point part here

		protected:
		private:
	};
};
#endif //XINGXIU_H


