/********************************************************************
filename:   UIColourConverter.cpp

purpose:    由于表示颜色有两种方法，一种是0-255，一种是0-1，所以这个
文件中的辅助函数用于转换这两种表示方法
*********************************************************************/

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "MiniMapMakerDialog.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "UIColourConverter.h"

//----------------------------------------------------------------------------
void FColourToIColour(float fr, float fg, float fb, 
                      unsigned char& ir, unsigned char& ig, unsigned char& ib )
{
    ir = (unsigned char)(fr * 255);
    ig = (unsigned char)(fg * 255);
    ib = (unsigned char)(fb * 255);
}
//----------------------------------------------------------------------------
void IColourToFColour(unsigned char ir, unsigned char ig, unsigned char ib,
                      float& fr, float& fg, float& fb)
{
    fr = ir / 255.0f;
    fg = ig / 255.0f;
    fb = ib / 255.0f;
}