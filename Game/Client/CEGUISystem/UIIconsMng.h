//图标管理器

#pragma once

#include "TDBasicType.h"
#include <map>

namespace LuaPlus
{
	class LuaState;
}

namespace CEGUI
{
	class Imageset;
	class Image;
}

class CUIIconsManager
{
public:
	STRING					GetIconFullName(LPCTSTR szIconName);
	const CEGUI::Image*		GetIcon(LPCTSTR szIconName);
	HCURSOR					CreateAsWinCursor(LPCTSTR szIconName);

public:
	//---------------------
	//到处Lua函数

	//取得Icon全路径
	static INT				LUA_GetIconFullName(LuaPlus::LuaState* pState);

public:
	typedef std::map< STRING, const CEGUI::Imageset* >	MAP_ICONS;

	//管理器初始化
	VOID				Initial(VOID);

public:
	CUIIconsManager();
	virtual ~CUIIconsManager();
	static CUIIconsManager*	GetMe(VOID) { return s_pMe; }

protected:
	MAP_ICONS				m_mapAllIcons;
	static CUIIconsManager* s_pMe;
};