/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#ifndef _CEGUIHyperLink_h_
#define _CEGUIHyperLink_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUIRect.h"
#include "CEGUIWindow.h"
#include <vector>
#include <map>
// Start of CEGUI namespace section
namespace CEGUI
{
class Window;
const int MAX_CELL_NUM = 256;

struct CEGUIEXPORT HyperLink
{
	String		d_name;
	//	点击区域后，push out的key word

	Rect		d_rectBuf[MAX_CELL_NUM];

	const Window*	d_pOwnerWindow[MAX_CELL_NUM];

	int			d_length;

	void addNewRect(const Window* pOwnerWindow,const Rect newRect)
	{

		d_rectBuf[d_length] = newRect;
		d_pOwnerWindow[d_length] = pOwnerWindow;
		d_length++;
	
	}

	void CleanUp()
	{
		d_name = "";
		d_length = 0;
		for(int i = 0;i < MAX_CELL_NUM;i++)
		{
			Rect newRect(0,0,0,0);
			d_rectBuf[i] = newRect;
			d_pOwnerWindow[i] = NULL;
		}
	}

	bool isInRange(const Window* pOwnerWindow,const Point& position)
	{

		for(int i = 0; i < MAX_CELL_NUM; i++)
		{
			bool a = d_rectBuf[i].isPointInRect(position);
//			Window* b = d_pOwnerWindow[i]->getCaptureWindow();
			bool c = d_pOwnerWindow[i]->isCapturedByThis();

			if(	(d_rectBuf[i].isPointInRect(position)) && 
//				(pOwnerWindow != NULL) &&
				(d_pOwnerWindow[i] == pOwnerWindow))
				return true;
		}
		return false;
	}

};

}// End of  CEGUI namespace section

#endif	// end of guard _CEGUIHyperLink_h_