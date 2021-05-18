#ifndef __EVENTAREAINFO_H__
#define __EVENTAREAINFO_H__
#pragma once
#include "baserendershape.h"
#include "LogicDataStructDef.h"

namespace WX 
{


	class CEventAreaInfo : public CBaseRenderShape
	{
	public:
		CEventAreaInfo(void);
		~CEventAreaInfo(void);

	public:

		////////////////////////////////////////////////////////////////////////////////////////
		//
		// Âß¼­Êý¾Ý
		//

		int         m_iId;
		int			m_iEventAreaId;
		int			m_iScriptId;
		AREARECT	m_rect;
	};

}

#endif //__EVENTAREAINFO_H__