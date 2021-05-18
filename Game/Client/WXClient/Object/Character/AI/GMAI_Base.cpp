#include "StdAfx.h"
#include "GMAI_Base.h"
#include "TDException.h"

CAI_Base::CAI_Base(CObject_Character* pCharObj)
{
	TDAssert(pCharObj);

	m_pCharacterObj = pCharObj;
}

CAI_Base::~CAI_Base( VOID )
{
}
