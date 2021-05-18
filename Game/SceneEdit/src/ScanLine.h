#ifndef __SCANLINE_H
#define __SCANLINE_H

#include <OgreVector2.h>
#include "Core/WXPrerequisites.h"

namespace WX
{
	class ScanLine
	{
	public:
		ScanLine(void);
		~ScanLine(void);

		void setStartPoint(const Point& pt);
		void setEndPoint(const Point& pt);

		const Point& getStartPoint();
		const Point& getEndPoint();

	private:
		Point mStartPt;
		Point mEndPt;
	};

}

#endif