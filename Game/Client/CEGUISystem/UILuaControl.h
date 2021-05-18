/****************************\
*							*
*	 用于将控件功能输出		*
*							*
\***************************/

#pragma once
#include <CEGUIWindow.h>
#include "LuaPlus.h"
#include "TDActionSystem.h"
class tScriptSandbox;
class tActionItem;

namespace LUA_CONTROL
{

	//--------------------------------------------------
	//控件基类
	class Window
	{
	public:
		static Window*	CreateControl(CEGUI::Window* pWindow);
		static VOID		DestroyControl(Window* pThis);

	public:
		static VOID		RegisterMetaTable(VOID);
		static VOID		DestroyMetaTable(VOID);

		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }

	public:
		//设置通用属性
		INT		setProperty(LuaPlus::LuaState* pState);
		//取得通用属性
		INT		getProperty(LuaPlus::LuaState* pState);

		//得到窗口类型
		INT		getType(LuaPlus::LuaState* pState)
		{
			pState->PushString(m_pWindow->getWidgetType().c_str());
			return 1;
		}

		//得到窗口名称
		INT		getName(LuaPlus::LuaState* pState)
		{
			pState->PushString(m_pWindow->getName().c_str());
			return 1;
		}

		//得到窗口文字
		INT		getText(LuaPlus::LuaState* pState);

		//设置文字
		INT		setText(LuaPlus::LuaState* pState);
		INT		setTextOriginal(LuaPlus::LuaState* pState);
		INT		TransText(LuaPlus::LuaState* pState);
		INT		setForce(LuaPlus::LuaState* pState);

		//显示
		INT		Show(LuaPlus::LuaState* pState)
		{
			m_pWindow->setVisible(true);
			return 0;
		}
		//隐藏
		INT		Hide(LuaPlus::LuaState* pState);

		//Disable
		INT		Disable(LuaPlus::LuaState* pState)
		{
			m_pWindow->setEnabled(false);
			return 0;
		}

		//Enable
		INT		Enable(LuaPlus::LuaState* pState)
		{
			m_pWindow->setEnabled(true);
			return 0;
		}

		INT		IsVisible( LuaPlus::LuaState* pState )
		{
			pState->PushBoolean( m_pWindow->isVisible() );
			return 1;
		}

		INT		SetToolTip( LuaPlus::LuaState* pState );

		//捕获键盘焦点
		INT		CaptureInput( LuaPlus::LuaState* pState);

		INT		CenterWindow( LuaPlus::LuaState* pState );
		INT		SetWindowCenter( LuaPlus::LuaState* pState ); 
		INT		SetClippedByParent( LuaPlus::LuaState* pState );
		INT		AutoMousePosition( LuaPlus::LuaState* pState );
	
	public:
		Window(CEGUI::Window* pWindow) { m_pWindow = pWindow; }
		virtual ~Window() { }

	protected:
		CEGUI::Window*		m_pWindow;

		static LuaPlus::LuaObject*	s_pMetaTable;
	};


	//-------------------------------------------------------------
	//Progress Bar
	class ProgressBar : public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }

	public:
		//设置进度
		INT		SetProgress(LuaPlus::LuaState* pState);

	protected:
		friend class Window;
		ProgressBar(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~ProgressBar() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	//-------------------------------------------------------------
	//ActionButton
	class ActionButton : public Window, public tActionReference
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }

	public:
		//配置ActionItem
		INT		Lua_SetActionItem(LuaPlus::LuaState* pState);
		//激活
		INT		Lua_DoAction(LuaPlus::LuaState* pState);
		//激活(副操作,一般是鼠标另一个键引起)
		INT		Lua_DoSubAction(LuaPlus::LuaState* pState);
		//进入按下状态
		INT		Lua_SetPushed(LuaPlus::LuaState* pState);
		//进入闪烁状态
		INT		Lua_SetFlash(LuaPlus::LuaState* pState);
		//置灰
		INT		Lua_Gloom(LuaPlus::LuaState* pState);
		//变亮
		INT		Lua_Bright(LuaPlus::LuaState* pState);


	public:
		//ActionSystem　调用
		//逻辑Action已经被销毁
		virtual VOID	BeDestroyed(VOID);
		//按钮按下
		virtual VOID	SetCheck(BOOL bCheck);
		//按钮设置成default的状态
		virtual VOID	SetDefault(BOOL bDefault);
		//进入冷却
		virtual VOID	EnterCoolDown(INT fTime, FLOAT fPercent);
		//进入闪烁
		virtual VOID	EnterFlash(BOOL bFlash);
		//显示数字
		virtual VOID	SetCornerChar(CORNER_NUMBER_POS pos, LPCTSTR szChar);
		//数据更新
		virtual VOID	Update(INT id);
		//屏蔽
		virtual	VOID	Disable(VOID);
		//开启
		virtual	VOID	Enable(VOID);
		//变灰
		virtual	VOID	Gloom(VOID);
		//变亮
		virtual	VOID	Bright(VOID);
		//锁定
		virtual	VOID	Locked(VOID);
		//解除锁定
		virtual	VOID	Unlock(VOID);


	protected:
		friend class Window;
		ActionButton(CEGUI::Window* pWindow):Window(pWindow), m_pActionItem(NULL) { }
		virtual ~ActionButton() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
		tActionItem*		m_pActionItem;
	};

	//-------------------------------------------------------------
	//PopMenu
	class PopMenu : public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }

	public:
		//配置ActionItem
		INT		OpenPopMenu(LuaPlus::LuaState* pState);
		//激活
		INT		ClosePopMenu(LuaPlus::LuaState* pState);

		// 设置位置
		INT		SetPopMenuPos(LuaPlus::LuaState* pState);

	protected:
		friend class Window;
		PopMenu(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~PopMenu() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	//-------------------------------------------------------------
	//ComplexWindow
	class ComplexWindow : public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }

	public:
		//添加文字元素
		INT		AddTextElement(LuaPlus::LuaState* pState);
		//添加选项元素
		INT		AddOptionElement(LuaPlus::LuaState* pState);
		//添加Item元素
		INT		AddItemElement(LuaPlus::LuaState* pState);
		//添加Money元素
		INT		AddMoneyElement(LuaPlus::LuaState* pState);
		//添加Impact元素
		INT		AddImpactElement(LuaPlus::LuaState* pState);
		//添加Action Button元素
		INT		AddActionElement(LuaPlus::LuaState* pState);
		//清空所有元素
		INT		CleanAllElement(LuaPlus::LuaState* pState);
		//更改字体的颜色
		INT		SetTextColour(LuaPlus::LuaState* pState);

		INT		PageEnd(LuaPlus::LuaState* pState);
	protected:
		friend class Window;
		ComplexWindow(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~ComplexWindow() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	
	class ListBox : public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
	public:
		virtual INT AddItem( LuaPlus::LuaState* pState );
		virtual INT SetItemTooltip( LuaPlus::LuaState* pState );
		virtual INT SetItemText( LuaPlus::LuaState* pState );
		
		INT DelItem( LuaPlus::LuaState* pState );
		INT GetItem( LuaPlus::LuaState* pState );
		INT ClearListBox( LuaPlus::LuaState* pState );
		INT	GetFirstSelectItem( LuaPlus::LuaState* pState );
		INT	SetItemSelect( LuaPlus::LuaState* pState );
		INT SetItemSelectByItemID( LuaPlus::LuaState* pState );

	protected:
		friend class Window;
		ListBox(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~ListBox() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};
	class ImageListBox : public ListBox
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
	public:
		virtual INT AddItem( LuaPlus::LuaState* pState );
	protected:
		friend class Window;
		ImageListBox(CEGUI::Window* pWindow):ListBox(pWindow) { }
		virtual ~ImageListBox() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	class ImageListBoxEx : public ListBox
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
	public:
		virtual INT AddItem( LuaPlus::LuaState* pState );
	protected:
		friend class Window;
		ImageListBoxEx(CEGUI::Window* pWindow):ListBox(pWindow) { }
		virtual ~ImageListBoxEx() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	class CheckButton: public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
	public:

	protected:
		friend class Window;
		CheckButton(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~CheckButton() {}


		INT		SetCheck( LuaPlus::LuaState* pState );
		INT		GetCheck( LuaPlus::LuaState* pState );
	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};


	class ComboBox: public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
	public:

	protected:
		friend class Window;
		ComboBox(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~ComboBox() {}


		INT		ComboBoxAddItem( LuaPlus::LuaState* pState );
		INT		ResetList( LuaPlus::LuaState* pState );
		INT		GetCurrentSelect( LuaPlus::LuaState* pState );
		INT		SetCurrentSelect( LuaPlus::LuaState* pState );

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	class StaticImage: public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
	public:

	protected:
		friend class Window;
		StaticImage(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~StaticImage() {}


		INT		SetImage( LuaPlus::LuaState* pState );
		INT		SetImageColor( LuaPlus::LuaState* pState );
		
	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	class ScrollBar: public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
	public:

	protected:
		friend class Window;
		ScrollBar(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~ScrollBar() {}


		INT		GetPosition( LuaPlus::LuaState* pState );
		INT		SetPosition( LuaPlus::LuaState* pState );
	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	class SuperTooltip : public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
	public:

	protected:
		friend class Window;
		SuperTooltip(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~SuperTooltip() {}

		INT		PositionSelf(LuaPlus::LuaState* pState);

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	class MeshWindow: public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
		
	protected:
		friend class Window;
		MeshWindow(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~MeshWindow() {}
	
	
		INT		SetFakeObject(LuaPlus::LuaState* pState);
		INT		RotateBegin(LuaPlus::LuaState* pState);
		INT		RotateEnd(LuaPlus::LuaState* pState);
		INT		ChangeActionBegin(LuaPlus::LuaState* pState);
	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	class ChatHistory : public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
		
	protected:
		friend class Window;
		ChatHistory(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~ChatHistory() {}
	
		INT		InsertChatString(LuaPlus::LuaState* pState);
		INT		RemoveAllChatString(LuaPlus::LuaState* pState);
		INT		ScrollToEnd(LuaPlus::LuaState* pState);
		INT		ExtendClearRegion(LuaPlus::LuaState* pState);

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	class ChatChannel : public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
		
	protected:
		friend class Window;
		ChatChannel(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~ChatChannel() {}
	
		INT		ClearAllChannel(LuaPlus::LuaState* pState);
		INT		AddChannel(LuaPlus::LuaState* pState);

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};
	class Scenemap:public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }

	public:
		INT UpdateViewRect( LuaPlus::LuaState* pState );
		INT SetSceneFileName( LuaPlus::LuaState* pState );
		INT CloseSceneMap( LuaPlus::LuaState* pState );
		INT GetMouseScenePos( LuaPlus::LuaState* pState );
		INT	UpdateSceneMap( LuaPlus::LuaState* pState );
		INT SetSceneZoomMode( LuaPlus::LuaState* pState );
	protected:
		friend class Window;
		Scenemap(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~Scenemap() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};
	
	class Worldmap:public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
	public:
		INT	InitWorldMap( LuaPlus::LuaState* pState );
		INT UpdateWorldMap( LuaPlus::LuaState* pState );
		INT GetCurrentSelectScene( LuaPlus::LuaState* pState );
	protected:
		void UpdatePlayer();
		CEGUI::String Worldmap::GetSceneNameByID( int id );
		friend class Window;
		Worldmap(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~Worldmap() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	class InfoList:public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
	public:
		INT	AddInfo( LuaPlus::LuaState* pState );
	protected:
		friend class Window;
		InfoList(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~InfoList() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	class CtrlList: public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
	public:
		INT AddColumn( LuaPlus::LuaState* pState );			// 添加列
		INT InsertColumn( LuaPlus::LuaState* pState );		// 查入一列
		INT AddNewItem( LuaPlus::LuaState* pState );		// 添加一行
		INT SetItemText( LuaPlus::LuaState* pState );		// 设置一个单元的文本
		INT SetItemData( LuaPlus::LuaState* pState );		// 设置一个单元的数据
		INT DeleteItem( LuaPlus::LuaState* pState );		// 删除一行
		INT RemoveAllItem( LuaPlus::LuaState* pState );		// 删除所有
		
		INT GetItemText( LuaPlus::LuaState* pState );		// 得到一个单元的文本
		INT GetItemCount( LuaPlus::LuaState* pState );		// 得到又多少行
		INT GetColumnCount( LuaPlus::LuaState* pState);		// 得到有多少列
		INT GetSelectItem( LuaPlus::LuaState* pState);		// 得到选中的行

	protected:
		friend class Window;
		CtrlList(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~CtrlList() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	
	class SoftKeyWindow: public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
	public:
		INT SetAimEditBox( LuaPlus::LuaState* pState);	// 得到有多少列

	protected:
		friend class Window;
		SoftKeyWindow(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~SoftKeyWindow() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	class Minimap : public Scenemap
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }

	public:
		INT UpdateFlag( LuaPlus::LuaState* pState );
	protected:
		friend class Window;
		Minimap(CEGUI::Window* pWindow):Scenemap(pWindow) { }
		virtual ~Minimap() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	class AnimateWindow : public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }

	public:
		INT Play( LuaPlus::LuaState* pState );
	protected:
		friend class Window;
		AnimateWindow(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~AnimateWindow() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};
	

}
