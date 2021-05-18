#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：GaiBang.cpp
// 功能说明：门派:明教的接口实现
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "GaiBang.h"

namespace MenPai_Module
{
	//class GaiBang_T: public MenPai_T
	VOID GaiBang_T::OnDepleteStrikePoints(Obj_Human& rMe, INT nStrikePoint) const
	{
		if(0>=nStrikePoint)
		{
			return;
		}
		
		INT nRage = TransferValueToRage(nStrikePoint);
		rMe.RefixRageRegeneration(nRage);
		rMe.RageIncrement(nRage,NULL);
	}

	INT GaiBang_T::TransferValueToRage(INT nValue) const
	{
		if(0>=nValue)
		{
			return 0;
		}
		return nValue*10;
	}
		
};


