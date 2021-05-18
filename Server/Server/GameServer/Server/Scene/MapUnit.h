

#ifndef __MAPUNIT_H__
#define __MAPUNIT_H__

/*

	.map 文件

文件头

	struct MAP_UNIT_FILEHEAD
	{
		UINT		uMagic;		// 'MAPU'   0X5550414D
		INT			nXSize;			// X方向大小
		INT			nZSize;			// Z方向大小
	};
	
文件结构

     [MAP_UNIT_FILEHEAD] [_MAP_UNIT] [_MAP_UNIT] [_MAP_UNIT] ...
                         \   ___________________________ ...
                                   nXSize*nZSize

*/

//场景编辑器生成的每个地图块中的信息
struct _MAP_UNIT
{
	WORD	m_Flag ;	//标志信息
						// 如果ISSET0(m_Flag)的值位真，则地图块位不可走 
						//		    
                        //     00000000|00000000
                        //                    ||_  WalkDisable  -是否禁止地面上行走的OBJ穿越  [0 可穿越  1不可穿越]
	                    //                    |__  FlyDisable   -是否禁止空中飞行的OBJ穿越    [0 可穿越  1不可穿越]
                        //

	BYTE	m_Height ;	//高度信息
	BYTE	m_Reserved ;


	_MAP_UNIT( )
	{
		m_Flag = 0 ;
		m_Height = 0 ;
		m_Reserved = 0 ;
	};
};

#endif
