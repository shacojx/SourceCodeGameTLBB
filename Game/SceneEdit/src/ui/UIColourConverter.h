/********************************************************************
filename:   UIColourConverter.h

purpose:    由于表示颜色有两种方法，一种是0-255，一种是0-1，所以这个
            文件中的辅助函数用于转换这两种表示方法
*********************************************************************/

#ifndef _UIColourConverter_H_
#define _UIColourConverter_H_

/// 由浮点数表示法转成整数表示法
void FColourToIColour(float fr, float fg, float fb, 
                      unsigned char& ir, unsigned char& ig, unsigned char& ib );

/// 由整数表示法转成浮点数表示法
void IColourToFColour(unsigned char ir, unsigned char ig, unsigned char ib,
                      float& fr, float& fg, float& fb);

#endif