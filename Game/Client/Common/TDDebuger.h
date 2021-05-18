/****************************************\
*										*
*			  µ÷ÊÔÆ÷½Ó¿Ú				*
*										*
\****************************************/

#pragma once
#include "TDNode.h"

class tDebuger : public tNode
{
public:
	virtual void	InfoTrack_Reset(INT nWidth, INT nHeight) = 0;
	virtual void	InfoTrack_AddPoint(INT nX, INT nY) = 0;
protected:
	WX_DECLARE_DYNAMIC(tDebuger);
};
