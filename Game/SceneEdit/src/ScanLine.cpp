
#include "ScanLine.h"

namespace WX
{

	ScanLine::ScanLine(void)
	{
	}

	ScanLine::~ScanLine(void)
	{
	}


	void ScanLine::setStartPoint(const Point& pt)
	{
		mStartPt = pt;
	}
	void ScanLine::setEndPoint(const Point& pt)
	{
		mEndPt = pt;
	}


	const Point& ScanLine::getStartPoint()
	{
		return mStartPt;
	}
	const Point& ScanLine::getEndPoint()
	{
		return mEndPt;
	}

}