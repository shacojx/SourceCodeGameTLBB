#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：EMei.cpp
// 功能说明：门派:峨嵋的接口实现
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "EMei.h"

namespace MenPai_Module
{
	//class EMei_T: public MenPai_T
	VOID EMei_T::OnHealTarget(Obj_Human& rMe, INT nHealHp) const
	{
		if(0>=nHealHp)
		{
			return;
		}
		INT nRage = TransferValueToRage(nHealHp);
		rMe.RefixRageRegeneration(nRage);
		rMe.RageIncrement(nRage,NULL);
	}

	INT EMei_T::TransferValueToRage(INT nValue) const
	{
		if(0>=nValue)
		{
			return 0;
		}
		return nValue/20;
	}
		
};

