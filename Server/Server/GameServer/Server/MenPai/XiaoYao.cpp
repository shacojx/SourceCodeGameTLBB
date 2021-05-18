#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：XiaoYao.cpp
// 功能说明：门派:逍遥的接口实现
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "XiaoYao.h"

namespace MenPai_Module
{
	//class XiaoYao_T: public MenPai_T
	VOID XiaoYao_T::OnMyTrapActivated(Obj_Human& rMe, Obj_Special& rTrap) const
	{
		INT nRage = TransferValueToRage(1);
		rMe.RefixRageRegeneration(nRage);
		rMe.RageIncrement(nRage,NULL);
	}

	INT XiaoYao_T::TransferValueToRage(INT nValue) const
	{
		if(0>=nValue)
		{
			return 0;
		}
		return nValue*10;
	}
		
};


