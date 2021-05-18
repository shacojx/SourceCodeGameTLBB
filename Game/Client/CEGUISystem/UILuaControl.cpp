#include "StdAfx.h"
#include "UIGlobal.h"
#include "UILuaControl.h"
#include "TDScriptSystem.h"
#include "TDException.h"
#include "UISystem.h"
#include "UIIconsMng.h"
#include <OgreMemoryManager.h>
#include <elements\CEGUIProgressBar.h>
#include "ceguipropertyhelper.h"
#include <CEGUIFontManager.h>
#include "UIString.h"
#include <elements\CEGUIPopupMenu.h>
#include <elements\ceguilistbox.h>
#include <elements\CEGUIScrollbar.h>
#include <elements\ceguilistboxtextitem.h>
#include <elements\ceguicombobox.h>
#include <elements\ceguimulticolumnlist.h>
#include "..\CEGUIFalagardEx\FalActionButton.h"
#include "..\CEGUIFalagardEx\FalagardComplexWindow.h"
#include "..\CEGUIFalagardEx\FalagardEditbox.h"
#include "..\CEGUIFalagardEx\FalSuperTooltip.h"
#include "..\CEGUIFalagardEx\Falagardinfolist.h"
#include "..\CEGUIFalagardEx\Falagardminimap.h"
#include "..\CEGUIFalagardEx\Falagardradar.h"
#include "..\CEGUIFalagardEx\FalagardScenemap.h"
#include "..\CEGUIFalagardEx\FalagardAnimate.h"
#include "CEGUIPropertyHelper.h"
#include "TDObjectSystem.h"
#include "tdworldsystem.h"
#include "TDDBC_Struct.h"
#include "UIWindowMng.h"
#include "TDEventDefine.h"
#include "TDFakeObjSystem.h"
#include "..\CEGUIFalagardEx\FalChatHistory.h"
#include "..\CEGUIFalagardEx\FalChatChannel.h"
#include "..\CEGUIFalagardEx\FalagardWorldmap.h"
#include "..\CEGUIFalagardEx\FalagardSoftKeyBoard.h"
#include "tddatabase.h"
#include "AxTrace.h"
namespace LUA_CONTROL
{

LuaPlus::LuaObject* Window::s_pMetaTable = NULL;
Window*	Window::CreateControl(CEGUI::Window* pWindow)
{
	CEGUI::String strType = pWindow->getWidgetType();

	if(strType == "Falagard/ActionButton")
	{
		return new ActionButton(pWindow);
	}
	else if(strType == "Falagard/ProgressBar")
	{
		return new ProgressBar(pWindow);
	}
	else if(strType == "Falagard/PopMenu")
	{
		return new PopMenu(pWindow);
	}
	else if( strType == "Falagard/Radar" )
	{
		return new Minimap( pWindow );
	}
	else if( strType == "Falagard/ComplexWindow" )
	{
		return new ComplexWindow( pWindow );
	}
	else if( strType == "Falagard/Listbox" )
	{
		return new ListBox( pWindow );
	}
	else if ( strType == "Falagard/ImageListbox" )
	{
		return new ImageListBox( pWindow );
	}
	else if ( strType == "Falagard/ImageListboxEx" )
	{
		return new ImageListBoxEx( pWindow );
	}
	else if( strType =="Falagard/CheckBox" )
	{
		return new CheckButton( pWindow );
	}
	else if( strType == "Falagard/Combobox" )
	{
		return new ComboBox( pWindow );
	}
	else if( strType == "Falagard/StaticImage" )
	{
		return new StaticImage( pWindow );
	}
	else if( strType == "Falagard/Scrollbar" )
	{
		return new ScrollBar( pWindow );
	}
	else if( strType == "Falagard/SuperTooltip" )
	{
		return new SuperTooltip( pWindow );
	}
	else if( strType == "Falagard/MeshWindow" )
	{
		return new MeshWindow( pWindow );
	}
	else if(strType == "Falagard/ChatHistory")
	{
		return new ChatHistory(pWindow);
	}
	else if(strType == "Falagard/ChatChannel")
	{
		return new ChatChannel(pWindow);
	}
	else if( strType == "Falagard/SceneMap" )
	{
		return new Scenemap( pWindow );
	}
	else if( strType == "Falagard/WorldMap" )
	{
		return new Worldmap( pWindow );
	}
	else if( strType == "Falagard/InfoList" )
	{
		return new InfoList( pWindow );
	}
	else if( strType == "Falagard/MultiColumnList" )
	{
		return new CtrlList( pWindow );
	}
	else if( strType == "Falagard/SoftKeyWindow" )
	{
		return new SoftKeyWindow( pWindow );
	}
	else if( strType == "Falagard/Animate" )
	{
		return new AnimateWindow( pWindow );
	}
	else
	{
		return new Window(pWindow);
	}
}

VOID Window::DestroyControl(Window* pThis)
{
	delete pThis;
}


VOID Window::RegisterMetaTable(VOID)
{
	LuaPlus::LuaState* pLuaState = g_pScriptSys->GetLuaState();
	TDAssert(pLuaState);

	//-----------------------------------------------------------
	//0. Window
	Window::s_pMetaTable = new LuaPlus::LuaObject;
	*Window::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_Window");
	Window::s_pMetaTable->SetObject("__index", *Window::s_pMetaTable);

	Window::s_pMetaTable->RegisterObjectFunctor("SetProperty", &Window::setProperty);
	Window::s_pMetaTable->RegisterObjectFunctor("GetProperty", &Window::getProperty);
	Window::s_pMetaTable->RegisterObjectFunctor("GetType"	, &Window::getType);
	Window::s_pMetaTable->RegisterObjectFunctor("GetName"	, &Window::getName);
	Window::s_pMetaTable->RegisterObjectFunctor("GetText"	, &Window::getText);
	Window::s_pMetaTable->RegisterObjectFunctor("SetText"	, &Window::setText);
	Window::s_pMetaTable->RegisterObjectFunctor("SetTextOriginal"	, &Window::setTextOriginal);
	Window::s_pMetaTable->RegisterObjectFunctor("TransText"	, &Window::TransText);
	Window::s_pMetaTable->RegisterObjectFunctor("Show"		, &Window::Show);
	Window::s_pMetaTable->RegisterObjectFunctor("Hide"		, &Window::Hide);
	Window::s_pMetaTable->RegisterObjectFunctor("Disable"	, &Window::Disable);
	Window::s_pMetaTable->RegisterObjectFunctor("Enable"	, &Window::Enable);
	Window::s_pMetaTable->RegisterObjectFunctor("IsVisible"	, &Window::IsVisible);
	Window::s_pMetaTable->RegisterObjectFunctor("SetForce"	, &Window::setForce);
	Window::s_pMetaTable->RegisterObjectFunctor("SetToolTip", &Window::SetToolTip);
	Window::s_pMetaTable->RegisterObjectFunctor("CaptureInput", &Window::CaptureInput);
	Window::s_pMetaTable->RegisterObjectFunctor("CenterWindow", &Window::CenterWindow);
	Window::s_pMetaTable->RegisterObjectFunctor("SetWindowCenter", &Window::SetWindowCenter);
	Window::s_pMetaTable->RegisterObjectFunctor("SetClippedByParent", &Window::SetClippedByParent);
    Window::s_pMetaTable->RegisterObjectFunctor("AutoMousePosition", &Window::AutoMousePosition);
	

	//-----------------------------------------------------------
	//1. Progress
	ProgressBar::s_pMetaTable = new LuaPlus::LuaObject;
	*ProgressBar::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_ProgressBar");
	*ProgressBar::s_pMetaTable = *Window::s_pMetaTable;
	ProgressBar::s_pMetaTable->SetObject("__index", *ProgressBar::s_pMetaTable);

	ProgressBar::s_pMetaTable->RegisterObjectFunctor("SetProgress"	, &ProgressBar::SetProgress);

	//-----------------------------------------------------------
	//2. ActionButton
	ActionButton::s_pMetaTable = new LuaPlus::LuaObject;
	*ActionButton::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_ActionButton");
	*ActionButton::s_pMetaTable = *Window::s_pMetaTable;
	ActionButton::s_pMetaTable->SetObject("__index", *ActionButton::s_pMetaTable);

	ActionButton::s_pMetaTable->RegisterObjectFunctor("SetActionItem"	, &ActionButton::Lua_SetActionItem);
	ActionButton::s_pMetaTable->RegisterObjectFunctor("DoAction"		, &ActionButton::Lua_DoAction);
	ActionButton::s_pMetaTable->RegisterObjectFunctor("DoSubAction"		, &ActionButton::Lua_DoSubAction);
	ActionButton::s_pMetaTable->RegisterObjectFunctor("SetPushed"		, &ActionButton::Lua_SetPushed);
	ActionButton::s_pMetaTable->RegisterObjectFunctor("SetFlash"		, &ActionButton::Lua_SetFlash);
	ActionButton::s_pMetaTable->RegisterObjectFunctor("Gloom"			, &ActionButton::Lua_Gloom);
	ActionButton::s_pMetaTable->RegisterObjectFunctor("Bright"			, &ActionButton::Lua_Bright);


	//-----------------------------------------------------------
	//3. PopMenu
	PopMenu::s_pMetaTable = new LuaPlus::LuaObject;
	*PopMenu::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_PopMenu");
	*PopMenu::s_pMetaTable = *Window::s_pMetaTable;
	PopMenu::s_pMetaTable->SetObject("__index", *PopMenu::s_pMetaTable);

	PopMenu::s_pMetaTable->RegisterObjectFunctor("OpenPopMenu"	,	&PopMenu::OpenPopMenu);
	PopMenu::s_pMetaTable->RegisterObjectFunctor("ClosePopMenu"	,	&PopMenu::ClosePopMenu);
	PopMenu::s_pMetaTable->RegisterObjectFunctor("SetPopMenuPos",	&PopMenu::SetPopMenuPos);

	//-----------------------------------------------------------
	//4. ComplexWindow
	ComplexWindow::s_pMetaTable = new LuaPlus::LuaObject;
	*ComplexWindow::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_ComplexWindow");
	*ComplexWindow::s_pMetaTable = *Window::s_pMetaTable;
	ComplexWindow::s_pMetaTable->SetObject("__index", *ComplexWindow::s_pMetaTable);

	ComplexWindow::s_pMetaTable->RegisterObjectFunctor("AddTextElement"	, &ComplexWindow::AddTextElement);
	ComplexWindow::s_pMetaTable->RegisterObjectFunctor("AddOptionElement"	, &ComplexWindow::AddOptionElement);
	ComplexWindow::s_pMetaTable->RegisterObjectFunctor("AddItemElement"	, &ComplexWindow::AddItemElement);
	ComplexWindow::s_pMetaTable->RegisterObjectFunctor("AddActionElement"	, &ComplexWindow::AddActionElement);
	ComplexWindow::s_pMetaTable->RegisterObjectFunctor("AddMoneyElement"	, &ComplexWindow::AddMoneyElement);
	ComplexWindow::s_pMetaTable->RegisterObjectFunctor("AddImpactElement"	, &ComplexWindow::AddImpactElement);
	ComplexWindow::s_pMetaTable->RegisterObjectFunctor("ClearAllElement"	, &ComplexWindow::CleanAllElement);
	ComplexWindow::s_pMetaTable->RegisterObjectFunctor("SetTextColour"	, &ComplexWindow::SetTextColour);
	ComplexWindow::s_pMetaTable->RegisterObjectFunctor("PageEnd"	, &ComplexWindow::PageEnd);

	//-----------------------------------------------------------
	//5.Minimap
	Minimap::s_pMetaTable = new LuaPlus::LuaObject;
	*Minimap::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_Minimap");
	*Minimap::s_pMetaTable = *Window::s_pMetaTable;
	Minimap::s_pMetaTable->SetObject("__index", *Minimap::s_pMetaTable);

	Minimap::s_pMetaTable->RegisterObjectFunctor( "UpdateFlag", &Minimap::UpdateFlag);

	//-----------------------------------------------------------
	//6. ListBox
	ListBox::s_pMetaTable = new LuaPlus::LuaObject;
	*ListBox::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_Listbox");
	*ListBox::s_pMetaTable = *Window::s_pMetaTable;
	ListBox::s_pMetaTable->SetObject("__index", *ListBox::s_pMetaTable);

	ListBox::s_pMetaTable->RegisterObjectFunctor( "SetItemTooltip", &ListBox::SetItemTooltip );
	ListBox::s_pMetaTable->RegisterObjectFunctor( "AddItem", &ListBox::AddItem );
	ListBox::s_pMetaTable->RegisterObjectFunctor( "SetListItemText", &ListBox::SetItemText );
	ListBox::s_pMetaTable->RegisterObjectFunctor( "GetItem", &ListBox::GetItem);
	ListBox::s_pMetaTable->RegisterObjectFunctor( "DelItem", &ListBox::DelItem);
	ListBox::s_pMetaTable->RegisterObjectFunctor( "ClearListBox", &ListBox::ClearListBox);
	ListBox::s_pMetaTable->RegisterObjectFunctor( "GetFirstSelectItem", &ListBox::GetFirstSelectItem);
	ListBox::s_pMetaTable->RegisterObjectFunctor( "SetItemSelect", &ListBox::SetItemSelect);
	ListBox::s_pMetaTable->RegisterObjectFunctor( "SetItemSelectByItemID", &ListBox::SetItemSelectByItemID);


	//-----------------------------------------------------------
	//7. ChatWindows

	//-----------------------------------------------------------
	//8. ChatInputWindow

	//-----------------------------------------------------------
	//9. ImageListBox
	ImageListBox::s_pMetaTable = new LuaPlus::LuaObject;
	*ImageListBox::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_ImageListbox");
	*ImageListBox::s_pMetaTable = *Window::s_pMetaTable;
	ImageListBox::s_pMetaTable->SetObject("__index", *ImageListBox::s_pMetaTable);

	ImageListBox::s_pMetaTable->RegisterObjectFunctor( "AddItem", &ImageListBox::AddItem );

	//-----------------------------------------------------------
	//10. ImageListBoxEx
	ImageListBoxEx::s_pMetaTable = new LuaPlus::LuaObject;
	*ImageListBoxEx::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_ImageListboxEx");
	*ImageListBoxEx::s_pMetaTable = *Window::s_pMetaTable;
	ImageListBoxEx::s_pMetaTable->SetObject("__index", *ImageListBoxEx::s_pMetaTable);

	ImageListBoxEx::s_pMetaTable->RegisterObjectFunctor( "AddItem", &ImageListBoxEx::AddItem );

	//-----------------------------------------------------------
	//11. CheckBox
	CheckButton::s_pMetaTable = new LuaPlus::LuaObject;
	*CheckButton::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_ImageCheckbox");
	*CheckButton::s_pMetaTable = *Window::s_pMetaTable;
	CheckButton::s_pMetaTable->SetObject("__index", *ImageListBoxEx::s_pMetaTable);

	CheckButton::s_pMetaTable->RegisterObjectFunctor( "SetCheck", &CheckButton::SetCheck );
	CheckButton::s_pMetaTable->RegisterObjectFunctor( "GetCheck", &CheckButton::GetCheck );

	//-----------------------------------------------------------
	//12. ComboBox
	ComboBox::s_pMetaTable = new LuaPlus::LuaObject;
	*ComboBox::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_ComboBox");
	*ComboBox::s_pMetaTable = *Window::s_pMetaTable;
	ComboBox::s_pMetaTable->SetObject("__index", *ComboBox::s_pMetaTable);

	ComboBox::s_pMetaTable->RegisterObjectFunctor( "ComboBoxAddItem", &ComboBox::ComboBoxAddItem);
	ComboBox::s_pMetaTable->RegisterObjectFunctor( "ResetList", &ComboBox::ResetList);
	ComboBox::s_pMetaTable->RegisterObjectFunctor( "GetCurrentSelect", &ComboBox::GetCurrentSelect);
	ComboBox::s_pMetaTable->RegisterObjectFunctor( "SetCurrentSelect", &ComboBox::SetCurrentSelect);
	//-----------------------------------------------------------
	//13. StaticImage
	StaticImage::s_pMetaTable = new LuaPlus::LuaObject;
	*StaticImage::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_StaticImage");
	*StaticImage::s_pMetaTable = *Window::s_pMetaTable;
	StaticImage::s_pMetaTable->SetObject("__index", *StaticImage::s_pMetaTable);

	StaticImage::s_pMetaTable->RegisterObjectFunctor( "SetImage", &StaticImage::SetImage);
	StaticImage::s_pMetaTable->RegisterObjectFunctor( "SetImageColor", &StaticImage::SetImageColor);

	//-----------------------------------------------------------
	//14. ScrollBar
	ScrollBar::s_pMetaTable = new LuaPlus::LuaObject;
	*ScrollBar::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_ScrollBar");
	*ScrollBar::s_pMetaTable = *Window::s_pMetaTable;
	ScrollBar::s_pMetaTable->SetObject("__index", *ScrollBar::s_pMetaTable);

	ScrollBar::s_pMetaTable->RegisterObjectFunctor( "SetPosition", &ScrollBar::SetPosition );
	ScrollBar::s_pMetaTable->RegisterObjectFunctor( "GetPosition", &ScrollBar::GetPosition );

	//-----------------------------------------------------------
	//15. SuperTooltip
	SuperTooltip::s_pMetaTable = new LuaPlus::LuaObject;
	*SuperTooltip::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_SuperTooltip");
	*SuperTooltip::s_pMetaTable = *Window::s_pMetaTable;
	SuperTooltip::s_pMetaTable->SetObject("__index", *SuperTooltip::s_pMetaTable);

	SuperTooltip::s_pMetaTable->RegisterObjectFunctor( "PositionSelf", &SuperTooltip::PositionSelf );

	//-----------------------------------------------------------
	//16. MeshWindow
	MeshWindow::s_pMetaTable = new LuaPlus::LuaObject;
	*MeshWindow::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_MeshWindow");
	*MeshWindow::s_pMetaTable = *Window::s_pMetaTable;
	MeshWindow::s_pMetaTable->SetObject("__index", *MeshWindow::s_pMetaTable);

	MeshWindow::s_pMetaTable->RegisterObjectFunctor("SetFakeObject", &MeshWindow::SetFakeObject );
	MeshWindow::s_pMetaTable->RegisterObjectFunctor("RotateBegin", &MeshWindow::RotateBegin );
	MeshWindow::s_pMetaTable->RegisterObjectFunctor("RotateEnd", &MeshWindow::RotateEnd );
	MeshWindow::s_pMetaTable->RegisterObjectFunctor("ChangeActionBegin", &MeshWindow::ChangeActionBegin );
	//-----------------------------------------------------------
	//17. ChatHistory
	ChatHistory::s_pMetaTable = new LuaPlus::LuaObject;
	*ChatHistory::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_ChatHistory");
	*ChatHistory::s_pMetaTable = *Window::s_pMetaTable;
	ChatHistory::s_pMetaTable->SetObject("__index", *ChatHistory::s_pMetaTable);

	ChatHistory::s_pMetaTable->RegisterObjectFunctor("InsertChatString", &ChatHistory::InsertChatString);
	ChatHistory::s_pMetaTable->RegisterObjectFunctor("RemoveAllChatString", &ChatHistory::RemoveAllChatString);
	ChatHistory::s_pMetaTable->RegisterObjectFunctor("ScrollToEnd", &ChatHistory::ScrollToEnd);
	ChatHistory::s_pMetaTable->RegisterObjectFunctor("ExtendClearRegion", &ChatHistory::ExtendClearRegion);

	//-----------------------------------------------------------
	//18. ChatChannel
	ChatChannel::s_pMetaTable = new LuaPlus::LuaObject;
	*ChatChannel::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_ChatChannel");
	*ChatChannel::s_pMetaTable = *Window::s_pMetaTable;
	ChatChannel::s_pMetaTable->SetObject("__index", *ChatChannel::s_pMetaTable);

	ChatChannel::s_pMetaTable->RegisterObjectFunctor("ClearAllChannel", &ChatChannel::ClearAllChannel);
	ChatChannel::s_pMetaTable->RegisterObjectFunctor("AddChannel", &ChatChannel::AddChannel);

	//-----------------------------------------------------------
	//19. Scenemap
	Scenemap::s_pMetaTable = new LuaPlus::LuaObject;
	*Scenemap::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_SceneMapWindow");
	*Scenemap::s_pMetaTable = *Window::s_pMetaTable;
	Scenemap::s_pMetaTable->SetObject("__index", *Scenemap::s_pMetaTable);

	
	Scenemap::s_pMetaTable->RegisterObjectFunctor("SetSceneFileName", &Scenemap::SetSceneFileName );
	Scenemap::s_pMetaTable->RegisterObjectFunctor("CloseSceneMap", &Scenemap::CloseSceneMap );
	Scenemap::s_pMetaTable->RegisterObjectFunctor("GetMouseScenePos", &Scenemap::GetMouseScenePos );
	Scenemap::s_pMetaTable->RegisterObjectFunctor("UpdateSceneMap", &Scenemap::UpdateSceneMap );
	Scenemap::s_pMetaTable->RegisterObjectFunctor("SetSceneZoomMode", &Scenemap::SetSceneZoomMode );
	Scenemap::s_pMetaTable->RegisterObjectFunctor("UpdateViewRect", &Scenemap::UpdateViewRect);

	//20. Worldmap
	Worldmap::s_pMetaTable = new LuaPlus::LuaObject;
	*Worldmap::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_WorldMapWindow");
	*Worldmap::s_pMetaTable = *Window::s_pMetaTable;
	Worldmap::s_pMetaTable->SetObject("__index", *Worldmap::s_pMetaTable);

	Scenemap::s_pMetaTable->RegisterObjectFunctor("InitWorldMap", &Worldmap::InitWorldMap );
	Scenemap::s_pMetaTable->RegisterObjectFunctor("GetCurrentSelectScene", &Worldmap::GetCurrentSelectScene );
	Scenemap::s_pMetaTable->RegisterObjectFunctor("UpdateWorldMap", &Worldmap::UpdateWorldMap );
	

	// 21. InfoList
	InfoList::s_pMetaTable = new LuaPlus::LuaObject;
	*InfoList::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_InfoListWindow");
	*InfoList::s_pMetaTable = *Window::s_pMetaTable;
	InfoList::s_pMetaTable->SetObject("__index", *InfoList::s_pMetaTable);

	InfoList::s_pMetaTable->RegisterObjectFunctor("AddInfo", &InfoList::AddInfo );

	// 22 CtrlList
	CtrlList::s_pMetaTable = new LuaPlus::LuaObject;
	*CtrlList::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_CtrlListWindow");
	*CtrlList::s_pMetaTable = *Window::s_pMetaTable;
	CtrlList::s_pMetaTable->SetObject("__index", *CtrlList::s_pMetaTable);

	CtrlList::s_pMetaTable->RegisterObjectFunctor("AddColumn", &CtrlList::AddColumn );
	CtrlList::s_pMetaTable->RegisterObjectFunctor("InsertColumn", &CtrlList::InsertColumn );
	CtrlList::s_pMetaTable->RegisterObjectFunctor("AddNewItem", &CtrlList::AddNewItem );
	CtrlList::s_pMetaTable->RegisterObjectFunctor("SetItemText", &CtrlList::SetItemText );
	CtrlList::s_pMetaTable->RegisterObjectFunctor("SetItemData", &CtrlList::SetItemData );
	CtrlList::s_pMetaTable->RegisterObjectFunctor("DeleteItem", &CtrlList::DeleteItem );
	CtrlList::s_pMetaTable->RegisterObjectFunctor("RemoveAllItem", &CtrlList::RemoveAllItem );
	CtrlList::s_pMetaTable->RegisterObjectFunctor("GetItemText", &CtrlList::GetItemText );
	CtrlList::s_pMetaTable->RegisterObjectFunctor("GetItemCount", &CtrlList::GetItemCount );
	CtrlList::s_pMetaTable->RegisterObjectFunctor("GetColumnCount", &CtrlList::GetColumnCount );
	CtrlList::s_pMetaTable->RegisterObjectFunctor("GetSelectItem", &CtrlList::GetSelectItem );

	// 23 SoftKeyWindow
	SoftKeyWindow::s_pMetaTable = new LuaPlus::LuaObject;
	*SoftKeyWindow::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_SoftKeyWindow");
	*SoftKeyWindow::s_pMetaTable = *Window::s_pMetaTable;
	SoftKeyWindow::s_pMetaTable->SetObject("__index", *SoftKeyWindow::s_pMetaTable);

	SoftKeyWindow::s_pMetaTable->RegisterObjectFunctor("SetAimEditBox", &SoftKeyWindow::SetAimEditBox );

	// 24 Animate
	AnimateWindow::s_pMetaTable = new LuaPlus::LuaObject;
	*AnimateWindow::s_pMetaTable = pLuaState->GetGlobals().CreateTable("MetaTable_Control_SoftKeyWindow");
	*AnimateWindow::s_pMetaTable = *Window::s_pMetaTable;
	AnimateWindow::s_pMetaTable->SetObject("__index", *AnimateWindow::s_pMetaTable);

	AnimateWindow::s_pMetaTable->RegisterObjectFunctor("Play", &AnimateWindow::Play);

}

VOID Window::DestroyMetaTable(VOID)
{
	#define safe_delete_metaTable( x ) if( x ){ delete( x ); x = NULL; }
	// 24 
	safe_delete_metaTable( AnimateWindow::s_pMetaTable );
	// 23 
	safe_delete_metaTable( SoftKeyWindow::s_pMetaTable );
	// 22
	safe_delete_metaTable( CtrlList::s_pMetaTable );
	// 21
	safe_delete_metaTable( InfoList::s_pMetaTable );
	// 20
	safe_delete_metaTable( Worldmap::s_pMetaTable );
	// 19
	safe_delete_metaTable( Scenemap::s_pMetaTable );
	// 18
	safe_delete_metaTable( ChatChannel::s_pMetaTable );
	// 17
	safe_delete_metaTable( ChatHistory::s_pMetaTable );
	// 16
	safe_delete_metaTable( MeshWindow::s_pMetaTable );
	// 15
	safe_delete_metaTable( SuperTooltip::s_pMetaTable );
	// 14
	safe_delete_metaTable( ScrollBar::s_pMetaTable );
	// 13
	safe_delete_metaTable( StaticImage::s_pMetaTable );
	// 12
	safe_delete_metaTable( ComboBox::s_pMetaTable );
	// 11.
	safe_delete_metaTable( CheckButton::s_pMetaTable );
	//10.
	safe_delete_metaTable( ImageListBoxEx::s_pMetaTable );
	//9.
	safe_delete_metaTable( ImageListBox::s_pMetaTable )
	//8.
	//7.

	//6.
	safe_delete_metaTable( ListBox::s_pMetaTable );
	
	//5.
	safe_delete_metaTable( Minimap::s_pMetaTable );

	//4.
	safe_delete_metaTable( ComplexWindow::s_pMetaTable );

	//3. 
	safe_delete_metaTable( PopMenu::s_pMetaTable );
	
	//2.
	safe_delete_metaTable( ActionButton::s_pMetaTable );

	//1.
	safe_delete_metaTable( ProgressBar::s_pMetaTable );

	//0.
	safe_delete_metaTable( Window::s_pMetaTable );
}

INT Window::setProperty(LuaPlus::LuaState* pState)
{
	LuaPlus::LuaStack args(pState);

	if (!(args[2].IsString())) return 0;
	if (!(args[3].IsString())) return 0;

	LPCTSTR szPropertyName = args[2].GetString();
	LPCTSTR szPropertyValue = args[3].GetString();

	STRING strIconFullName,strUtf8_IconName;
	char value[ 128 ];
	sprintf( value, "%s", szPropertyName);
	if(strcmp(value, "ShortImage") == 0)
	{
		szPropertyName = "Image";
		strIconFullName = CUIIconsManager::GetMe()->GetIconFullName( args[3].GetString() );
		CUIStringSystem::mbcs_to_utf8(strIconFullName.c_str(), strUtf8_IconName);
		szPropertyValue = strUtf8_IconName.c_str();
	}
	
	STRING strValue_utf8;
	if(strcmp(value, "Text") == 0)
	{
		STRING strValue_mbcs(args[3].GetString());
		CUIStringSystem::mbcs_to_utf8(strValue_mbcs, strValue_utf8);
		szPropertyValue = strValue_utf8.c_str();
	}

	try
	{
		m_pWindow->setProperty((CEGUI::utf8*)szPropertyName, (CEGUI::utf8*)szPropertyValue);
	}
	catch(...) { }

	return 0;
}
// 将窗口居中显示，相对与父窗口
INT		Window::CenterWindow( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	CEGUI::Window* pParent = m_pWindow->getParent();
	if( pParent )
	{
		CEGUI::Point pos;
		CEGUI::Rect rect = pParent->getAbsoluteRect();
		pos.d_x = ( rect.d_left + rect.d_right) / 2 - m_pWindow->getAbsoluteRect().getWidth() / 2;
		pos.d_y = ( rect.d_top + rect.d_bottom ) / 2 - m_pWindow->getAbsoluteRect().getHeight() / 2;
		m_pWindow->setPosition( CEGUI::Absolute, pos );
	}
	return 0;
}
// 将窗口居中显示，相对与父窗口
INT		Window::SetWindowCenter( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	if( !args[ 2 ].IsInteger() ) return 0;
	if( !args[ 3 ].IsInteger() ) return 0;

	CEGUI::Point pos;
	pos.d_x = args[ 2 ].GetInteger() - m_pWindow->getAbsoluteRect().getWidth() / 2;
	pos.d_y = args[ 3 ].GetInteger() - m_pWindow->getAbsoluteRect().getHeight() / 2;
	m_pWindow->setPosition( CEGUI::Absolute, pos );
	return 0;
}
//设置窗口的随父属性
INT		Window::SetClippedByParent( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	if( !args[ 2 ].IsInteger() ) return 0;
	
	INT n = args[ 2 ].GetInteger();

	if(n == 0)
	{
		m_pWindow->setClippedByParent(false);
		m_pWindow->setAlwaysOnTop(true);
	}
	else
		m_pWindow->setClippedByParent(true);

	return 0;
}
//自动设置窗口的位置
INT		Window::AutoMousePosition( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsString())||(!(args[3].IsString())))
	{
		return 0;
	}

	FLOAT fX = args[2].GetFloat();
	FLOAT fY = args[3].GetFloat();

	//重新计算
	FLOAT fWidth	= m_pWindow->getAbsoluteWidth();
	FLOAT fHeight	= m_pWindow->getAbsoluteHeight();

	FLOAT fScreenWidth	= CUIWindowMng::GetClientScreen()->getAbsoluteWidth();
	FLOAT fScreenHeight = CUIWindowMng::GetClientScreen()->getAbsoluteHeight();

	if(fX + fWidth + 5.0f> fScreenWidth) 
		fX -= fWidth+5.0f;
	else 
		fX += 5.0f;

	if(fY + fHeight + 5.0f > fScreenHeight) 
		fY -= fHeight+5.0f;
	else
		fY += 5.0f;

	CEGUI::Point tmpPos;
	tmpPos.d_x = fX;
	tmpPos.d_y = fY;

	((CEGUI::Window*)m_pWindow)->setPosition(CEGUI::Absolute, tmpPos);

	return 0;
}

INT Window::getProperty(LuaPlus::LuaState* pState)
{
	LuaPlus::LuaStack args(pState);

	if (!(args[2].IsString())) return 0;

	try
	{
		CEGUI::String strUtf32 = m_pWindow->getProperty((CEGUI::utf8*)args[2].GetString());
		
		STRING strUtf8;
		CUIStringSystem::utf8_to_mbcs(strUtf32.c_str(), strUtf8);

		pState->PushString(strUtf8.c_str());
	}
	catch(...)
	{
		pState->PushString("");
	}

	return 1;
}

INT	Window::setText(LuaPlus::LuaState* pState)
{
	LuaPlus::LuaStack args(pState);

	INT t = args[2].GetType();
	if( args[ 2 ].IsInteger() ) // 如果是个数字
	{
		char value[ 128 ];
		sprintf( value, "%d", args[ 2 ].GetInteger() );
		std::string strUtf8;
		CUIStringSystem::mbcs_to_utf8(value, strUtf8);

		m_pWindow->setText((CEGUI::String)(CEGUI::utf8*)(strUtf8.c_str()));
	}
	else
	if ( args[2].IsString() )
	{

		//代码转化
		std::string strMbcs = args[2].GetString();
		CEGUI::String strUtf32;
		CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs, strUtf32);

		m_pWindow->setText(strUtf32);
	}

	return 0;
}

INT Window::Hide(LuaPlus::LuaState* pState)
{
	m_pWindow->setVisible(false);
	if(m_pWindow->testClassName((const CEGUI::utf8*)"Editbox"))
	{
		m_pWindow->releaseInput();
	}
	return 0;
}

INT	Window::getText(LuaPlus::LuaState* pState)
{
	std::string strUtf8 = m_pWindow->getText().c_str();//utf::mbcs_to_utf8(strMbcs);
	std::string strMbcs;
	CUIStringSystem::utf8_to_mbcs(strUtf8, strMbcs);
	pState->PushString( strMbcs.c_str() );
	return 1;
}
INT	Window::setTextOriginal(LuaPlus::LuaState* pState)
{
	LuaPlus::LuaStack args(pState);

	INT t = args[2].GetType();
	if (!(args[2].IsString())) return 0;

	//代码转化
	std::string strMbcs = args[2].GetString();
	std::string strUtf8;
	CUIStringSystem::mbcs_to_utf8(strMbcs, strUtf8);

	m_pWindow->setTextOriginal((CEGUI::String)(CEGUI::utf8*)(strUtf8.c_str()));

	return 0;
}
INT	Window::setForce( LuaPlus::LuaState* pState)
{
	m_pWindow->activate();

	return 0;
}
INT	Window::SetToolTip( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	if( !( args[ 2 ].IsString() ) ) return 0;
	std::string strMbcs = args[2].GetString();
	CEGUI::String  strLast;
	CUIStringSystem::GetMe()->ParserString_Runtime( strMbcs, strLast );

	m_pWindow->setProperty("Tooltip", strLast );
	
	return 0;
}

INT	Window::TransText(LuaPlus::LuaState* pState)
{
	CEGUI::String str = m_pWindow->getTextOriginal();
	if(!str.empty())
	{
		STRING strMBCS_ToTrans;
		CUIStringSystem::utf8_to_mbcs(str.c_str(), strMBCS_ToTrans);
		// 经过special转化的string，已经是utf32格式。
		CEGUI::String strMBCS_Transed_Done;
		
		CUIStringSystem::GetMe()->ParserString_Runtime(strMBCS_ToTrans, strMBCS_Transed_Done);

		m_pWindow->setText(strMBCS_Transed_Done);
	}
	return 0;
}

INT Window::CaptureInput( LuaPlus::LuaState* pState)
{
	m_pWindow->activate();
	m_pWindow->captureInput();
	return 0;
}

//==============================================================================
LuaPlus::LuaObject* ProgressBar::s_pMetaTable = NULL;
INT ProgressBar::SetProgress(LuaPlus::LuaState* pState)
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsNumber())) return 0;
	if (!(args[3].IsNumber())) return 0;

	FLOAT fNow = (FLOAT)args[2].GetNumber();
	FLOAT fMax = (FLOAT)args[3].GetNumber();

	((CEGUI::ProgressBar*)m_pWindow)->setProgress((FLOAT)fNow/fMax);

	return 0;
}

//==============================================================================
LuaPlus::LuaObject* ActionButton::s_pMetaTable = NULL;
INT ActionButton::Lua_SetActionItem(LuaPlus::LuaState* pState)
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsInteger())) return 0;
	
	INT idItem = (INT)args[2].GetInteger();

	Update(idItem);
	return 0;
}

INT ActionButton::Lua_DoAction(LuaPlus::LuaState* pState)
{
	if(m_pActionItem)
		m_pActionItem->DoAction();
	return 0;
}

INT ActionButton::Lua_DoSubAction(LuaPlus::LuaState* pState)
{
	if(m_pActionItem)
		m_pActionItem->DoSubAction();
	return 0;
}

INT ActionButton::Lua_SetPushed(LuaPlus::LuaState* pState)
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsInteger())) return 0;
	
	BOOL bCheck = args[2].GetInteger() == 1;

	SetCheck(bCheck);

	return 0;
}

INT	ActionButton::Lua_SetFlash(LuaPlus::LuaState* pState)
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsInteger())) return 0;
	
	BOOL bFlash = args[2].GetInteger();

	EnterFlash(bFlash);

	return 0;
}
//锁定
VOID ActionButton::Locked(VOID)
{
	m_pWindow->setProperty("Locked", "False");
	m_pWindow->requestRedraw();
}
//解除锁定
VOID ActionButton::Unlock(VOID)
{
	m_pWindow->setProperty("Locked", "False");
	m_pWindow->requestRedraw();
}

VOID ActionButton::BeDestroyed(VOID)
{
	m_pWindow->setProperty("Empty", "True");
	m_pWindow->setProperty("Tooltip", (CEGUI::utf8*)"");
	((CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)m_pWindow)->setLogicItemData((void*)(INT_PTR)(-1));
	((CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)m_pWindow)->cleanAnimate();

	//隐藏Tooltip
	if(g_pActionSys->GetTooltipsFocus() && g_pActionSys->GetTooltipsFocus() == m_pActionItem)
	{
		AxTrace(0, 0, "***CleanSupertooltips");
		g_pEventSys->PushEvent(GE_SUPERTOOLTIP, "0");
	}

	m_pWindow->requestRedraw();
	m_pActionItem = NULL;
}

VOID ActionButton::Update(INT idItem)
{
	//取得ActionItem
	tActionItem* pActionItem = g_pActionSys->GetAction(idItem);

	//断开和原来的Action的联系
	if(m_pActionItem && pActionItem != m_pActionItem)
	{
		m_pActionItem->RemoveRefrence(this);
	}

	if(NULL == pActionItem)
	{
		//如果逻辑项为空, 清空显示
		BeDestroyed();
		return;
	}

	//设置有效无效标记
	if(pActionItem->IsEnabled())
	{
		Bright();
	}
	else
	{
		Gloom();
	}

	//设置新的Action
	((CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)m_pWindow)->setLogicItemData((void*)(INT_PTR)pActionItem->GetID());

	m_pActionItem = pActionItem;
	LPCTSTR DragName = ((CEGUI::FalagardActionButton*)m_pWindow)->getDragAcceptName();

	if(DragName[0] == 'M' || DragName[0] == 'F')
	{
		pActionItem->AddReference(this,TRUE);
	}
	else
	{
		pActionItem->AddReference(this,FALSE);
	}

	const STRING strIconName = 
		CUISystem::GetMe()->GetIconManager()->GetIconFullName(m_pActionItem->GetIconName());

	m_pWindow->show();
	m_pWindow->setProperty("Empty", "False");
	if(!strIconName.empty())
	{
		m_pWindow->setProperty("NormalImage", strIconName);
	}
}

//屏蔽
VOID	ActionButton::Disable(VOID)
{
	m_pWindow->setProperty("Disabled", "True");
	m_pWindow->requestRedraw();
}

//开启
VOID	ActionButton::Enable(VOID)
{
	m_pWindow->setProperty("Disabled", "False");
	m_pWindow->requestRedraw();
}

//变灰
VOID	ActionButton::Gloom(VOID)
{
	m_pWindow->setProperty("Gloom", "True");
	m_pWindow->requestRedraw();
}

//变亮
VOID	ActionButton::Bright(VOID)
{
	m_pWindow->setProperty("Gloom", "False");
	m_pWindow->requestRedraw();
}

INT	ActionButton::Lua_Gloom(LuaPlus::LuaState* pState)
{
	Gloom();

	return 0;
}

INT	ActionButton::Lua_Bright(LuaPlus::LuaState* pState)
{
	Bright();

	return 0;
}

VOID ActionButton::SetCheck(BOOL bCheck)
{
	m_pWindow->setProperty("Checked", bCheck ? "True" : "False");

	m_pWindow->requestRedraw();
}

//按钮设置成default的状态
VOID ActionButton::SetDefault(BOOL bDefault)
{
	m_pWindow->setProperty("Default", bDefault ? "True" : "False");

	m_pWindow->requestRedraw();
}

VOID ActionButton::SetCornerChar(CORNER_NUMBER_POS pos, LPCTSTR szChar)
{
	CHAR szTemp[MAX_PATH];
	switch(pos)
	{
	case ANP_TOPLEFT:
		{
			_snprintf(szTemp, MAX_PATH, "TopLeft %s", szChar);
			m_pWindow->setProperty("CornerChar", szTemp);
		}
		break;
	case ANP_TOPRIGHT:
		{
			_snprintf(szTemp, MAX_PATH, "TopRight %s", szChar);
			m_pWindow->setProperty("CornerChar", szTemp);
		}
		break;
	case ANP_BOTLEFT:
		{
			_snprintf(szTemp, MAX_PATH, "BotLeft %s", szChar);
			m_pWindow->setProperty("CornerChar", szTemp);
		}
		break;
	case ANP_BOTRIGHT:
		{
			_snprintf(szTemp, MAX_PATH, "BotRight %s", szChar);
			m_pWindow->setProperty("CornerChar", szTemp);
		}
		break;
	}
}

VOID ActionButton::EnterCoolDown(INT nTime, FLOAT fPercent)
{
	if(nTime <= 0)
	{
		((CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)m_pWindow)->cleanAnimate();
	}
	else
	{
		((CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)m_pWindow)->setAnimate(
			"CoolDown", nTime, fPercent);
	}
}

VOID ActionButton::EnterFlash(BOOL bFlash)
{
	if(bFlash)
	{
		((CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)m_pWindow)->setAnimate(
			"Flash", 1000, 0.0);
	}
	else
	{
		((CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)m_pWindow)->cleanAnimate();
	}
}


//==============================================================================
LuaPlus::LuaObject* PopMenu::s_pMetaTable = NULL;
INT PopMenu::OpenPopMenu(LuaPlus::LuaState* pState)
{
	((CEGUI::PopupMenu*)m_pWindow)->openPopupMenu();
	return 0;
}

INT PopMenu::ClosePopMenu(LuaPlus::LuaState* pState)
{
	((CEGUI::PopupMenu*)m_pWindow)->closePopupMenu();
	return 0;
}

// 设置位置
INT	PopMenu::SetPopMenuPos(LuaPlus::LuaState* pState)
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsString())||(!(args[3].IsString())))
	{
		return 0;
	}

	FLOAT fX = args[2].GetFloat();
	FLOAT fY = args[3].GetFloat();

	//重新计算
	FLOAT fWidth	= m_pWindow->getAbsoluteWidth();
	FLOAT fHeight	= m_pWindow->getAbsoluteHeight();

	FLOAT fScreenWidth	= CUIWindowMng::GetClientScreen()->getAbsoluteWidth();
	FLOAT fScreenHeight = CUIWindowMng::GetClientScreen()->getAbsoluteHeight();

	if(fX + fWidth > fScreenWidth) fX -= fWidth;
	if(fY + fHeight > fScreenHeight) fY -= fHeight;

	((CEGUI::PopupMenu*)m_pWindow)->setXPosition(CEGUI::Relative, fX/fScreenWidth);
	((CEGUI::PopupMenu*)m_pWindow)->setYPosition(CEGUI::Relative, fY/fScreenHeight);

	m_pWindow->requestRedraw();
	return 0;
}

//==============================================================================
LuaPlus::LuaObject* ComplexWindow::s_pMetaTable = NULL;
INT ComplexWindow::AddTextElement(LuaPlus::LuaState* pState)
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsString())) return 0;

	//文字转化
	std::string strMbcs = args[2].GetString();

//	std::string strUtf8 = utf::mbcs_to_utf8(strMbcs);

	CEGUI::String strUtf32;
	
	CEGUI::FontBase* theFont = NULL;
	//创建新的文字节
	CEGUI::IFalagardComplexWindow* pComplexWindow = (CEGUI::IFalagardComplexWindow*)(CEGUI::FalagardComplexWindow*)m_pWindow;
	if(args[3].IsString())
	{
		std::string strMbcsFont = args[3].GetString();
		CEGUI::String strUtf32Font;
		CUIStringSystem::GetMe()->ParserString_Runtime(strMbcsFont, strUtf32Font);
		
		CEGUI::FontManager* pFontManager = CEGUI::FontManager::getSingletonPtr();
		theFont = pFontManager->getFont(strUtf32Font);

	}

	INT	Typeset = 4;

	std::string strMbcsExtra1;	// = args[3].GetString();

	//马匪首领@4
	const char* strTemp = args[2].GetString();
	const char* strDelim1 = strchr(strTemp,'@');
	
	if(strDelim1)
	{
		strMbcs.assign(strDelim1+2);
		strMbcsExtra1.assign(strDelim1+1,strDelim1+2);
		Typeset = atoi(&strMbcsExtra1.at(0));
	}

	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs, strUtf32);
	
	pComplexWindow->AddChildElement_Text(strUtf32,theFont,(CEGUI::TextFormatting)Typeset);
	

	return 0;
}

INT ComplexWindow::AddOptionElement(LuaPlus::LuaState* pState)
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsString())) return 0;

	//文字转化
	std::string strMbcs;// = args[2].GetString();
	std::string strMbcsExtra1;// = args[3].GetString();
	std::string strMbcsExtra2;// = args[4].GetString();
	std::string strMbcsExtra3;// = args[5].GetString();
	//马匪首领#21107,-1$-1
	const char* strTemp = args[2].GetString();
	const char* strDelim1 = strchr(strTemp,'#');
	const char* strDelim2 = strchr(strTemp,',');
	const char* strDelim3 = strchr(strTemp,'$');

	if( (strDelim1 && (!strDelim2 || !strDelim3) ) ||
		(strDelim2 && (!strDelim1 || !strDelim3) ) ||
		(strDelim3 && (!strDelim1 || !strDelim2) ) )
	{
		TDAssert(strDelim1 && strDelim2 && strDelim3);
	}

	if(strDelim1 && strDelim2 && strDelim3)
	{
		strMbcs.assign(strTemp,strDelim1-strTemp);
		strMbcsExtra1.assign(strDelim1+1,strDelim2-1-strDelim1);
		strMbcsExtra2.assign(strDelim2+1,strDelim3-1-strDelim2);
		strMbcsExtra3.assign(strDelim3+1);
	}
	else
	{
		strMbcs.assign(strTemp);
		strMbcsExtra1.assign("-1");
		strMbcsExtra2.assign("-1");
		strMbcsExtra3.assign("-1");
	}

	CEGUI::String strUtf32;
	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs, strUtf32);

	CEGUI::String strExtra1;
	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcsExtra1, strExtra1);
	
	CEGUI::String strExtra2;
	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcsExtra2, strExtra2);

	CEGUI::String strExtra3;
	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcsExtra3, strExtra3);

	//创建新的选项节
	CEGUI::IFalagardComplexWindow* pComplexWindow = (CEGUI::IFalagardComplexWindow*)(CEGUI::FalagardComplexWindow*)m_pWindow;
	pComplexWindow->AddChildElement_Option(strUtf32,strExtra1,strExtra2,strExtra3);

	return 0;
}
INT ComplexWindow::AddItemElement(LuaPlus::LuaState* pState)
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsInteger())) return 0;
	if (!(args[3].IsInteger())) return 0;
	if (!(args[4].IsInteger())) return 0;

	INT  idItem	= args[2].GetInteger();
	INT	 nNum	= args[3].GetInteger();
	BOOL bRadio		= (args[4].GetInteger() == 1);
	BOOL bReArrange = FALSE;
	if(args[5].IsInteger())
		bReArrange = (args[5].GetInteger() == 1);

	tObject_Item* pItem = g_pObjectSystem->FindItem(idItem);

	STRING strIconFullName = CUIIconsManager::GetMe()->GetIconFullName(pItem->GetIconName());
	STRING strUtf8_IconName;
	CUIStringSystem::mbcs_to_utf8(strIconFullName.c_str(), strUtf8_IconName);

	STRING strUtf8_Desc;
	CUIStringSystem::mbcs_to_utf8(pItem->GetName(), strUtf8_Desc);

	CEGUI::IFalagardComplexWindow* pComplexWindow = (CEGUI::IFalagardComplexWindow*)(CEGUI::FalagardComplexWindow*)m_pWindow;
	pComplexWindow->AddChildElement_Item( (CEGUI::String)(CEGUI::utf8*)(strUtf8_IconName.c_str()), (CEGUI::String)(CEGUI::utf8*)(strUtf8_Desc.c_str()), bRadio==TRUE, bReArrange==TRUE);
//	pComplexWindow->AddChildElement_Item( idItem, (CEGUI::String)(CEGUI::utf8*)(strUtf8_Desc.c_str()), bRadio==TRUE, bReArrange==TRUE);


	return 0;
}

INT ComplexWindow::AddActionElement(LuaPlus::LuaState* pState)
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsInteger())) return 0;
	if (!(args[3].IsInteger())) return 0;
	if (!(args[4].IsInteger())) return 0;

	INT  idItem	= args[2].GetInteger();
	INT	 nNum	= args[3].GetInteger();
	BOOL bRadio		= (args[4].GetInteger() == 1);
	BOOL bReArrange = FALSE;
	if(args[5].IsInteger())
		bReArrange = (args[5].GetInteger() == 1);

	tActionItem* pActionItem = g_pActionSys->GetAction(idItem);

	if(NULL == pActionItem)
		return 0;

	const STRING strIconFullName = 
		CUISystem::GetMe()->GetIconManager()->GetIconFullName(pActionItem->GetIconName());

	STRING strUtf8_IconName;
	CUIStringSystem::mbcs_to_utf8(strIconFullName.c_str(), strUtf8_IconName);

	CEGUI::IFalagardComplexWindow* pComplexWindow = (CEGUI::IFalagardComplexWindow*)(CEGUI::FalagardComplexWindow*)m_pWindow;
	CEGUI::IFalagardActionButton* pActionButton = (CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)pComplexWindow->AddChildElement_Action( strUtf8_IconName, idItem, bRadio==TRUE, bReArrange==TRUE);

	if(pActionItem->GetDefineID() != 20005020)//随机奖励物品不显示supertooltip
	{
	//MouseEnter
		pActionButton->subscribeMouseEnterEvent(CEGUI::Event::Subscriber(&CUISystem::handleActionButtonMouseEnter, CUISystem::GetMe()));
	//MouseLeave
		pActionButton->subscribeMouseLeaveEvent(CEGUI::Event::Subscriber(&CUISystem::handleActionButtonMouseLeave, CUISystem::GetMe()));
	//ParentHidden
		((CEGUI::Window*)(CEGUI::FalagardActionButton*)pActionButton)->
			subscribeEvent((CEGUI::utf8*)"ParentHidden", CEGUI::Event::Subscriber(&CUISystem::handleActionButtonParentHidden, CUISystem::GetMe()));
	}
	return 0;
}

//struct SBuffImpact;
INT ComplexWindow::AddImpactElement(LuaPlus::LuaState* pState)
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsString())) return 0;
//	if (!(args[3].IsInteger())) return 0;
//	if (!(args[4].IsInteger())) return 0;

//	args[2].GetString()  idImpact	= args[2].GetString();
//	INT		nNum	= args[3].GetInteger();
//	BOOL bRadio		= (args[4].GetInteger() == 1);

//	tObject_Item* pItem = g_pObjectSystem->FindItem(idItem);
	//SBuffImpact *pBuffImpact = GetBuffImpactMgr()->GetBuffImpact( idImpact );

	STRING strIconFullName = CUIIconsManager::GetMe()->GetIconFullName( args[2].GetString() );
	STRING strUtf8_IconName;
	CUIStringSystem::mbcs_to_utf8(strIconFullName, strUtf8_IconName);

	STRING strUtf8_Desc;
	CUIStringSystem::mbcs_to_utf8("", strUtf8_Desc);

	CEGUI::IFalagardComplexWindow* pComplexWindow = (CEGUI::IFalagardComplexWindow*)(CEGUI::FalagardComplexWindow*)m_pWindow;
//	pComplexWindow->AddChildElement_Item( (CEGUI::String)(CEGUI::utf8*)(strUtf8_IconName.c_str()), (CEGUI::String)(CEGUI::utf8*)(strUtf8_Desc.c_str()), bRadio);
	//	改了使用方法
	//	pComplexWindow->AddChildElement_Item( (CEGUI::String)(CEGUI::utf8*)(strUtf8_IconName.c_str()),(CEGUI::String)(CEGUI::utf8*)(strUtf8_Desc.c_str()),0,0);
	return 0;
}

INT ComplexWindow::AddMoneyElement(LuaPlus::LuaState* pState)
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsString())) return 0;

	CEGUI::IFalagardComplexWindow* pComplexWindow = (CEGUI::IFalagardComplexWindow*)(CEGUI::FalagardComplexWindow*)m_pWindow;

	pComplexWindow->AddChildElement_Money(atoi(args[2].GetString()));

	return 0;
}

INT ComplexWindow::CleanAllElement(LuaPlus::LuaState* pState)
{
	CEGUI::IFalagardComplexWindow* pComplexWindow = (CEGUI::IFalagardComplexWindow*)(CEGUI::FalagardComplexWindow*)m_pWindow;

	pComplexWindow->ClearAllElement();

	return 0;
}

//更改字体的颜色
INT ComplexWindow::SetTextColour(LuaPlus::LuaState* pState)
{
	return 0;
}
INT ComplexWindow::PageEnd(LuaPlus::LuaState* pState)
{
	CEGUI::IFalagardComplexWindow* pComplexWindow = (CEGUI::IFalagardComplexWindow*)(CEGUI::FalagardComplexWindow*)m_pWindow;
	pComplexWindow->PageEnd();
	return 0;
}

//==============================================================================
LuaPlus::LuaObject* Minimap::s_pMetaTable = NULL;

LPCTSTR FLAG_CLASS_NAME[] = {"Animate","ExpNPC","Teamate","OtherPlayer","ExpObj",};
INT Minimap::UpdateFlag( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	
	CEGUI::IFalagardSceneMapWindow* pWindow= (CEGUI::IFalagardSceneMapWindow*)(CEGUI::FalagardRadar*)m_pWindow;
	fVector3 MyselfPos = g_pWorldSystem->GetMyselfPos();
	int		 nDir = g_pWorldSystem->GetMyselfDir( );
	pWindow->setMyselfPos( CEGUI::Point( MyselfPos.x, MyselfPos.z ) );
	pWindow->setMyselfDir( nDir );
	pWindow->setMAXJuLi(args[2].GetFloat());
	if( m_pWindow->isVisible() )
	{
		// 跟新所有的标志位置
		pWindow->Clear();

		for( INT i = 0; i < 9; i ++ )
		{
			std::list< MAP_POS_DEFINE >* listObj = g_pWorldSystem->GetObjectListByClass( i );
			// 得到了当前类型所有的物体的链表
			std::list< MAP_POS_DEFINE >::iterator it;
			for( it = listObj->begin(); it != listObj->end(); it ++ )
			{
				std::string strUtf8;
				std::string strMbcs = (*it).name;
				CUIStringSystem::mbcs_to_utf8(strMbcs, strUtf8 );
				if( i == 2 ) // 如果是队友，需要单独处理
					if( (*it).dwSceneID != g_pWorldSystem->GetActiveSceneID() ) // 在同一场景才会添加
						continue;
				pWindow->setMinimapData( i, (*it).pos.x, (*it).pos.z,  (CEGUI::String)(CEGUI::utf8*)strUtf8.c_str() );
			}
		}
	}
	return 0;
}
//==============================================================================
// ListBox
LuaPlus::LuaObject* ListBox::s_pMetaTable = NULL;
INT ListBox::AddItem( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsString())) 
		return 0;
	if (!(args[3].IsInteger()))
		return 0;

	//Text
	std::string strMbcs = args[2].GetString();
	std::string strUtf8;
	CUIStringSystem::mbcs_to_utf8(strMbcs, strUtf8);

	//TextIndex
	INT nIndex = args[3].GetInteger();

	//TextColor
	CEGUI::colour colorText = CEGUI::colour(1.0f, 1.0f, 1.0f);
	if(args[4].IsString())
	{
		colorText = CEGUI::PropertyHelper::stringToColour((CEGUI::utf8*)args[4].GetString());
	}
	CEGUI::String strUtf32;
	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs, strUtf32);

	CEGUI::ListboxTextItem* pItem = new CEGUI::ListboxTextItem( strUtf32,  nIndex);
	pItem->setTextColours(colorText);
	pItem->setSelectionColours(CEGUI::colour(0xFFC8B88E));
	pItem->setSelectionBrushImage((CEGUI::utf8*)"Common", (CEGUI::utf8*)"HoverTextBak");
	if( args[ 5 ].IsInteger() )
		pItem->setHorzFomate( CEGUI::TextFormatting( args[ 5 ].GetInteger() ) );
	((CEGUI::Listbox*)m_pWindow)->addItem( pItem );
	return 0;
}
INT ListBox::SetItemTooltip( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsInteger())) 
		return 0;
	if( !( args[ 3 ].IsString() ) )
		return 0;
	((CEGUI::Listbox*)m_pWindow)->setItemTooltipsEnabled( true );
	CEGUI::ListboxItem* pItem = ((CEGUI::Listbox*)m_pWindow)->getListboxItemFromIndex( args[ 2 ].GetInteger() );
	pItem->setTooltipText( args[ 3 ].GetString() );
	return 0;
}
INT ListBox::SetItemText( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsInteger())) 
		return 0;
	if( !( args[ 3 ].IsString() ) )
		return 0;
	CEGUI::ListboxItem* pItem = ((CEGUI::Listbox*)m_pWindow)->getListboxItemFromIndex( args[ 2 ].GetInteger() );
	std::string strMbcs = args[3].GetString();
	CEGUI::String strUtf32;
	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs, strUtf32);
	pItem->setText( strUtf32 );
	((CEGUI::Listbox*)m_pWindow)->requestRedraw();
	return 0;
}
INT ListBox::DelItem( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsInteger())) 
		return 0;
	CEGUI::ListboxItem* pItem = ((CEGUI::Listbox*)m_pWindow)->getListboxItemFromIndex( args[ 2 ].GetInteger() );
	((CEGUI::Listbox*)m_pWindow)->removeItem( pItem );
	return 0;
}

INT ListBox::GetItem( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsInteger())) 
		return 0;
	if( args[ 2 ].GetInteger() < 0 )
		return 0;
	CEGUI::ListboxItem* pItem = ((CEGUI::Listbox*)m_pWindow)->getListboxItemFromIndex( args[ 2 ].GetInteger() - 1 );
	std::string strMbcs;
	CUIStringSystem::utf8_to_mbcs(pItem->getText().c_str(), strMbcs);
	pState->PushString( strMbcs.c_str() );
	pState->PushInteger( pItem->getID() );
	return 2;
}

INT ListBox::ClearListBox( LuaPlus::LuaState* pState )
{
	((CEGUI::Listbox*)m_pWindow)->resetList();
	return 0;
}

INT	ListBox::GetFirstSelectItem( LuaPlus::LuaState* pState )
{
	CEGUI::ListboxTextItem* pItem = (CEGUI::ListboxTextItem*)((CEGUI::Listbox*)m_pWindow)->getFirstSelectedItem();
	if(!pItem) 
		pState->PushInteger(-1);
	else
		pState->PushInteger(pItem->getID());

	return 1;
}

INT	ListBox::SetItemSelect( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsInteger())) 
		return 0;
	
	INT nItemIndex = args[2].GetInteger();

	((CEGUI::Listbox*)m_pWindow)->setItemSelectState(nItemIndex, true);
	return 0;
}

INT	ListBox::SetItemSelectByItemID( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsInteger())) 
		return 0;
	
	INT nItemID = args[2].GetInteger();

	((CEGUI::Listbox*)m_pWindow)->setItemSelectStateByItemID(nItemID, true);
	return 0;
}

//==============================================================================
// ImageListBox
LuaPlus::LuaObject* ImageListBox::s_pMetaTable = NULL;
INT ImageListBox::AddItem( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsString())) 
		return 0;

	CEGUI::String strText;
	char value[ 128 ];
	sprintf( value, "id=%d text=%s", args[ 3 ].GetInteger(), args[2].GetString() );
	m_pWindow->setProperty( "AddItem", value );
	return 0;
}
//==============================================================================
// ImageListBoxEx
LuaPlus::LuaObject* ImageListBoxEx::s_pMetaTable = NULL;
INT ImageListBoxEx::AddItem( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsString())) 
		return 0;
	if( !( args[ 3 ].IsInteger() ) )
		return 0;
	if( !( args[ 4 ].IsInteger() ) )
		return 0;

	CEGUI::String strText;
	char value[ 128 ];
	
	sprintf( value, "id=%d state=%d text=%s", args[ 3 ].GetInteger(), args[ 4 ].GetInteger(), args[2].GetString() );
	std::string strMbcs = value;
	std::string strUtf8;
	CUIStringSystem::mbcs_to_utf8(strMbcs, strUtf8);
	m_pWindow->setProperty( "AddItem", (CEGUI::String)(CEGUI::utf8*)(strUtf8.c_str()) );
	return 0;
}
//==============================================================================
//CheckButton
LuaPlus::LuaObject* CheckButton::s_pMetaTable = NULL;
INT CheckButton::SetCheck( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	if( !args[ 2 ].IsInteger() ) return 0;
	
	if( args[ 2 ].GetInteger() == 1 )

		m_pWindow->setProperty( "Selected", "True" );
	else
		m_pWindow->setProperty( "Selected", "False" );

	return 0;
}
INT CheckButton::GetCheck( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	if(CEGUI::PropertyHelper::stringToBool(  m_pWindow->getProperty( "Selected" ) ))
		pState->PushInteger(TRUE);
	else
		pState->PushInteger(FALSE);

//	pState->PushBoolean( CEGUI::PropertyHelper::stringToBool(  m_pWindow->getProperty( "Selected" ) ) );
	return 1;
}

LuaPlus::LuaObject* ComboBox::s_pMetaTable = NULL;
INT ComboBox::ComboBoxAddItem( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsString())) 
		return 0;
	std::string strMbcs = args[2].GetString();
	std::string strUtf8;
	CUIStringSystem::mbcs_to_utf8(strMbcs, strUtf8);

	CEGUI::ListboxTextItem* pItem = new CEGUI::ListboxTextItem( (CEGUI::String)(CEGUI::utf8*)(strUtf8.c_str()), args[ 3 ].GetInteger() );

	pItem->setSelectionColours(CEGUI::colour(1.0f, 0.0f, 0.0f));
	pItem->setSelectionBrushImage((CEGUI::utf8*)"Common", (CEGUI::utf8*)"HoverTextBak");
	((CEGUI::Combobox*)m_pWindow)->addItem( pItem );

	return 0;
}
INT		ComboBox::ResetList( LuaPlus::LuaState* pState )
{
	((CEGUI::Combobox*)m_pWindow)->resetList();
	return 0;
}
INT		ComboBox::GetCurrentSelect( LuaPlus::LuaState* pState )
{
	if( ((CEGUI::Combobox*)m_pWindow)->getSelectedItem() )
	{
		pState->PushString( ((CEGUI::Combobox*)m_pWindow)->getSelectedItem()->getText().c_str() );
		pState->PushInteger( int( ((CEGUI::Combobox*)m_pWindow)->getSelectedItem()->getID() ) );
	}
	else
	{
		pState->PushString( "-1" );
		pState->PushInteger( -1 );
	}
		
	return 2;
}
INT		ComboBox::SetCurrentSelect( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);	
	CEGUI::ListboxItem* pItem = ((CEGUI::Combobox*)m_pWindow)->getListboxItemFromIndex( args[ 2 ].GetInteger() );
	((CEGUI::Combobox*)m_pWindow)->setItemSelectState( args[ 2 ].GetInteger() , true );
	((CEGUI::Combobox*)m_pWindow)->setText( pItem->getText() );
	return 0;

}

////////////////////////////////////////////////////////////////////////////////////
LuaPlus::LuaObject* StaticImage::s_pMetaTable = NULL;
INT StaticImage::SetImageColor( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsString())) 
		return 0;
	CEGUI::colour color = CEGUI::PropertyHelper::stringToColour( args[ 2 ].GetString() );
	CEGUI::ColourRect colorRect( color );
	m_pWindow->setProperty( "ImageColours", CEGUI::PropertyHelper::colourRectToString( colorRect ) );

	return 0;
}
INT StaticImage::SetImage( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsString())) 
		return 0;
	if( args[3].IsString() ) // 说明这个图片是全路径
	{
		char strImageName[128];
		_snprintf("set:%s image:%s", 128, args[2].GetString(), args[3].GetString());
		m_pWindow->setProperty("Image", strImageName );
	}
	else// 说明是icon
	{
		STRING strIconName;
		try
		{
			strIconName = CUISystem::GetMe()->GetIconManager()->GetIconFullName(args[2].GetString());
			m_pWindow->setProperty("Image", (CEGUI::utf8*)strIconName.c_str());
		}
		catch(...)
		{
			CHAR szDebug[1024] = {0};
			_snprintf(szDebug, 1024, 
				"传说中的LuaError，一定要告诉我，我找它很久了\n"
				"arg2=%s\n"
				"Full=%s\n"
				"pWindow=%08X\n"
				"this=%08X",
				args[2].GetString(), strIconName.c_str(), (DWORD)(DWORD_PTR)m_pWindow, (DWORD)(DWORD_PTR)this);

			MessageBox(g_hMainWnd, szDebug, "LUA", MB_OK|MB_ICONINFORMATION);
		}
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////
// ScrollBar
LuaPlus::LuaObject* ScrollBar::s_pMetaTable = NULL;
INT ScrollBar::GetPosition( LuaPlus::LuaState* pState )
{
	pState->PushNumber( ((CEGUI::Scrollbar*)m_pWindow)->getScrollPosition() );
	return 1;
}

INT ScrollBar::SetPosition( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsNumber())) 
		return 0;
	((CEGUI::Scrollbar*)m_pWindow)->setScrollPosition((float)args[2].GetNumber());

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////
// SuperTooltip
LuaPlus::LuaObject* SuperTooltip::s_pMetaTable = NULL;

INT SuperTooltip::PositionSelf( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsString())||(!(args[3].IsString())))
	{
		return 0;
	}

	//强制SuperTooltip重新计算组件大小
	((CEGUI::IFalagardSuperTooltip*)(CEGUI::FalagardSuperTooltip*)m_pWindow)->resizeSelf();

	FLOAT fX = args[2].GetFloat();
	FLOAT fY = args[3].GetFloat();

	//重新计算
	FLOAT fWidth	= m_pWindow->getAbsoluteWidth();
	FLOAT fHeight	= m_pWindow->getAbsoluteHeight();

	FLOAT fScreenWidth	= CUIWindowMng::GetClientScreen()->getAbsoluteWidth();
	FLOAT fScreenHeight = CUIWindowMng::GetClientScreen()->getAbsoluteHeight();

	if(fX + fWidth + 5.0f> fScreenWidth) 
		fX -= fWidth+5.0f;
	else 
		fX += 5.0f;

	if(fY + fHeight + 5.0f > fScreenHeight) 
		fY -= fHeight+5.0f;
	else
		fY += 5.0f;

	((CEGUI::PopupMenu*)m_pWindow)->setXPosition(CEGUI::Absolute, fX);
	((CEGUI::PopupMenu*)m_pWindow)->setYPosition(CEGUI::Absolute, fY);

	return 0;
}


////////////////////////////////////////////////////////////////////////////////////
// MeshWindow
LuaPlus::LuaObject* MeshWindow::s_pMetaTable = NULL;
INT MeshWindow::SetFakeObject( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	if( !args[2].IsString())
		return 0;

	//通知FakeObject系统
	g_pFakeSystem->OnUIHook(m_pWindow->getName().c_str(), args[2].GetString());
	//通知UI
	m_pWindow->setProperty((CEGUI::utf8*)"FakeObject", (CEGUI::utf8*)args[2].GetString());

	return 0;
}

INT MeshWindow::RotateBegin(LuaPlus::LuaState* pState)
{
	LuaPlus::LuaStack args(pState);
	if( !args[2].IsNumber())
		return 0;

	FLOAT fAngle = args[2].GetFloat();

	//通知FakeObject系统
	g_pFakeSystem->RotateObj_Begin(m_pWindow->getProperty((CEGUI::utf8*)"FakeObject").c_str(), fAngle);

	return 0;
}

INT MeshWindow::ChangeActionBegin(LuaPlus::LuaState* pState)
{
	LuaPlus::LuaStack args(pState);
	if( !args[2].IsNumber())
		return 0;
	if( !args[3].IsNumber())
		return 0;
	INT ActionNUM = args[2].GetInteger();
	INT ActionLoop = args[3].GetInteger();

	//通知FakeObject系统
	g_pFakeSystem->Obj_ChangeAction(m_pWindow->getProperty((CEGUI::utf8*)"FakeObject").c_str(), ActionNUM,ActionLoop);

	return 0;
}

INT MeshWindow::RotateEnd(LuaPlus::LuaState* pState)
{
	LuaPlus::LuaStack args(pState);

	//通知FakeObject系统
	g_pFakeSystem->RotateObj_End(m_pWindow->getProperty((CEGUI::utf8*)"FakeObject").c_str());

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////
// ChatHistory
LuaPlus::LuaObject* ChatHistory::s_pMetaTable = NULL;
INT ChatHistory::InsertChatString(LuaPlus::LuaState* pState)
{
	LuaPlus::LuaStack args(pState);
	if( !args[2].IsString())
		return 0;

	//代码转化
	std::string strMbcs = args[2].GetString();
	CEGUI::String strUtf32;
	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs, strUtf32);
	((CEGUI::IFalagardChatHistory*)(CEGUI::FalagardChatHistory*)m_pWindow)->insertChatString(strUtf32);

	return 0;
}

INT ChatHistory::RemoveAllChatString(LuaPlus::LuaState* pState)
{
	((CEGUI::IFalagardChatHistory*)(CEGUI::FalagardChatHistory*)m_pWindow)->removeAllChatStrings();
	return 0;
}

INT	ChatHistory::ScrollToEnd(LuaPlus::LuaState* pState)
{
	((CEGUI::IFalagardChatHistory*)(CEGUI::FalagardChatHistory*)m_pWindow)->scrollToEnd();
	return 0;
}

INT ChatHistory::ExtendClearRegion(LuaPlus::LuaState* pState)
{
	((CEGUI::IFalagardChatHistory*)(CEGUI::FalagardChatHistory*)m_pWindow)->extendClearRegion();
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////
// ChatChannel
LuaPlus::LuaObject* ChatChannel::s_pMetaTable = NULL;
INT ChatChannel::ClearAllChannel(LuaPlus::LuaState* pState)
{
	((CEGUI::IFalagardChatChannel*)(CEGUI::FalagardChatChannel*)m_pWindow)->clearAllChannel();

	return 0;
}

INT ChatChannel::AddChannel(LuaPlus::LuaState* pState)
{
	LuaPlus::LuaStack args(pState);
	if( !args[2].IsString() || !args[3].IsString() || !args[4].IsString())
		return 0;

	//代码转化
	std::string strMbcs_ChannelType =  args[2].GetString();
	CEGUI::String strUtf32_ChannelType;
	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs_ChannelType, strUtf32_ChannelType);

	std::string strMbcs_IconName =  args[3].GetString();
	CEGUI::String strUtf32_IconName;
	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs_IconName, strUtf32_IconName);

	std::string strMbcs_Name =  args[4].GetString();
	CEGUI::String strUtf32_Name;
	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs_Name, strUtf32_Name);

	((CEGUI::IFalagardChatChannel*)(CEGUI::FalagardChatChannel*)m_pWindow)->addChannel(
		strUtf32_ChannelType, strUtf32_IconName, strUtf32_Name);

	m_pWindow->moveToFront();
	return 0;
}

//==============================================================================
LuaPlus::LuaObject* Scenemap::s_pMetaTable = NULL;
INT Scenemap::SetSceneZoomMode( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	if( !args[ 2 ].IsNumber() ) return 0;
	CEGUI::IFalagardSceneMapWindow* pWindow= (CEGUI::IFalagardSceneMapWindow*)(CEGUI::FalagardSceneMap*)m_pWindow;
	pWindow->SetZoomMode( float( args[ 2 ].GetNumber()) );
	return 0;
}
INT Scenemap::UpdateViewRect( LuaPlus::LuaState* pState )
{
	CEGUI::IFalagardSceneMapWindow* pWindow= (CEGUI::IFalagardSceneMapWindow*)(CEGUI::FalagardSceneMap*)m_pWindow;
	fVector3 MyselfPos = g_pWorldSystem->GetMyselfPos();
	CEGUI::Point pos;
	pos.d_x = MyselfPos.x;
	pos.d_y = MyselfPos.z;
	pWindow->setMyselfPos( pos );

	pWindow->setSceneMapCenter( );
	return 0;
}
INT Scenemap::SetSceneFileName( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	if( !args[ 2 ].IsNumber() ) return 0;
	if( !args[ 3 ].IsNumber() ) return 0;
	if( !args[ 4 ].IsString() ) return 0;
	CEGUI::IFalagardSceneMapWindow* pWindow= (CEGUI::IFalagardSceneMapWindow*)(CEGUI::FalagardSceneMap*)m_pWindow;
	pWindow->SetSceneSize( CEGUI::Size( float( args[ 2 ].GetNumber() ), float( args[ 3 ].GetNumber() ) ) );
	pWindow->SetSceneFileName( args[ 4 ].GetString(), args[ 5 ].GetNumber() );
	return 0;
}
INT Scenemap::CloseSceneMap( LuaPlus::LuaState* pState )
{
	CEGUI::IFalagardSceneMapWindow* pWindow= (CEGUI::IFalagardSceneMapWindow*)(CEGUI::FalagardSceneMap*)m_pWindow;
	pWindow->Clear();
	return 0;
}

INT Scenemap::GetMouseScenePos( LuaPlus::LuaState* pState )
{
	CEGUI::IFalagardSceneMapWindow* pWindow= (CEGUI::IFalagardSceneMapWindow*)(CEGUI::FalagardSceneMap*)m_pWindow;
	const CEGUI::Point pos = pWindow->GetMouseScenePos();
	pState->PushInteger( int( pos.d_x ) );
	pState->PushInteger( int( pos.d_y ) );
	return 2;
}


INT	Scenemap::UpdateSceneMap( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);

	if( !m_pWindow->isVisible() )
		return 0;
	CEGUI::IFalagardSceneMapWindow* pWindow= (CEGUI::IFalagardSceneMapWindow*)(CEGUI::FalagardRadar*)m_pWindow;
	fVector3 MyselfPos = g_pWorldSystem->GetMyselfPos();
	int		 nDir = g_pWorldSystem->GetMyselfDir( );
	pWindow->setMyselfPos( CEGUI::Point( MyselfPos.x, MyselfPos.z ) );
	pWindow->setMyselfDir( nDir );
	pWindow->setMAXJuLi(args[2].GetFloat());
	if( m_pWindow->isVisible() )
	{
		// 跟新所有的标志位置
		pWindow->Clear();
		for( INT i = 0; i < 9; i ++ )
		{
			if( i == 2 || i == 7 || i == 5 )
			{
				std::list< MAP_POS_DEFINE >* listObj = g_pWorldSystem->GetObjectListByClass( i );
				// 得到了当前类型所有的物体的链表
				std::list< MAP_POS_DEFINE >::iterator it;
				for( it = listObj->begin(); it != listObj->end(); it ++ )
				{
					std::string strUtf8;
					std::string strMbcs = (*it).name;
					CUIStringSystem::mbcs_to_utf8(strMbcs, strUtf8 );
					if( i == 2 ) // 如果是队友，需要单独处理
						if( (*it).dwSceneID != g_pWorldSystem->GetActiveSceneID() ) // 在同一场景才会添加
							continue;
					pWindow->setMinimapData( i, (*it).pos.x, (*it).pos.z,  (CEGUI::String)(CEGUI::utf8*)strUtf8.c_str() );
				}
			}
		}
	}
	
	

	return 0;
}

// 初始化世界地图
LuaPlus::LuaObject* Worldmap::s_pMetaTable = NULL;
INT	Worldmap::InitWorldMap( LuaPlus::LuaState* pState )
{
	const tDataBase* pdbcbase = g_pDataBaseSystem->GetDataBase( DBC_SCENE_DEFINE );
	int nNum = pdbcbase->GetRecordsNum();
	std::list< MAP_POS_DEFINE >* listObj = g_pWorldSystem->GetObjectListByClass( 2 );
	std::list< MAP_POS_DEFINE >::iterator it;
	
	CEGUI::IFalagardWorldMap* pWindow= (CEGUI::IFalagardWorldMap*)(CEGUI::FalagardWorldMap*)m_pWindow;

	pWindow->createBackground();
	for( int i = 0; i < nNum; i ++ )
	{
		const _DBC_SCENE_DEFINE* pLine = (const _DBC_SCENE_DEFINE*)((tDataBase*)pdbcbase)->Search_LineNum_EQU(i);
		if( pLine )
		{
			if( pLine->nWroldMapPosX == 0 ||pLine->nWroldMapPosY == 0 )
				continue;
			pWindow->setCityPos( pLine->nServerID, CEGUI::Point( float( pLine->nWroldMapPosX ), float( pLine->nWroldMapPosY ) ) );
			pWindow->setCityNamePos( pLine->nServerID, CEGUI::Point( float( pLine->nNameWroldMapPosX ),float( pLine->nNameWroldMapPosY ) ) );
			pWindow->setCityType( pLine->nServerID, pLine->szSceneType );
			
			pWindow->setCityNameNormalImage( pLine->nServerID, pLine->szCityNameNormalImageSet, pLine->szCityNameNormalImage  );
			pWindow->setCityNameHoverImage( pLine->nServerID, pLine->szCityNameHoverImageSet, pLine->szCityNameHoverImage );

			// 判断是不是当前场景
			//
			if( pLine->nServerID == g_pWorldSystem->GetActiveScene()->GetSceneDefine()->nServerID )
				pWindow->setCityCurrentScene( pLine->nServerID, 1 );
			else
				pWindow->setCityCurrentScene( pLine->nServerID, 0 );
		}

	}
	UpdatePlayer();
	pWindow->resetWorldMap();

	return 0;
}

INT	Worldmap::UpdateWorldMap( LuaPlus::LuaState* pState )
{
	UpdatePlayer();
	return 0;
}
INT Worldmap::GetCurrentSelectScene( LuaPlus::LuaState* pState )
{
	CEGUI::IFalagardWorldMap* pWindow= (CEGUI::IFalagardWorldMap*)(CEGUI::FalagardWorldMap*)m_pWindow;
	pState->PushInteger( pWindow->getCurrentSelectScene() );
	return 1;
}
CEGUI::String Worldmap::GetSceneNameByID( int id )
{
	const tDataBase* pdbcbase = g_pDataBaseSystem->GetDataBase( DBC_SCENE_DEFINE );
	int nNum = pdbcbase->GetRecordsNum();
	for( int i = 0; i < nNum; i ++ )
	{
		const _DBC_SCENE_DEFINE* pLine = (const _DBC_SCENE_DEFINE*)((tDataBase*)pdbcbase)->Search_First_Column_Equ(_DBC_SCENE_DEFINE::SCENE_SERVER_ID_COLUMN, i);
		if( pLine->nServerID == id )
			return pLine->szName;
	}
	return "";
}
void Worldmap::UpdatePlayer()
{
	const tDataBase* pdbcbase = g_pDataBaseSystem->GetDataBase( DBC_SCENE_DEFINE );
	int nNum = pdbcbase->GetRecordsNum();
	std::list< MAP_POS_DEFINE >* listObj = g_pWorldSystem->GetObjectListByClass( 2 );
	std::list< MAP_POS_DEFINE >::iterator it;
	// 遍历所有的好友
	CEGUI::IFalagardWorldMap* pWindow= (CEGUI::IFalagardWorldMap*)(CEGUI::FalagardWorldMap*)m_pWindow;
	pWindow->clearPlayerInfo();
	for( it = listObj->begin(); it != listObj->end(); it ++ )
	{
		std::string strUtf8;
		std::string strMbcs = ( *it ).name;
		CUIStringSystem::mbcs_to_utf8(strMbcs, strUtf8 );
		pWindow->addPlayerToCity(  (*it ).dwSceneID, (CEGUI::String)(CEGUI::utf8*)strUtf8.c_str() );

	}
}
LuaPlus::LuaObject* InfoList::s_pMetaTable = NULL;
INT InfoList::AddInfo( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	if (!(args[2].IsString())) 
		return 0;
	//TextIndex
	std::string strMbcs = args[2].GetString();
	std::string strUtf8;
	CUIStringSystem::mbcs_to_utf8(strMbcs, strUtf8);

	CEGUI::String strUtf32;
	CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs, strUtf32);

	m_pWindow->setProperty( "AddInfo", strUtf32 );
	return 0;
}

LuaPlus::LuaObject* CtrlList::s_pMetaTable = NULL;

INT CtrlList::AddColumn( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	std::string caption = args[2].GetString();
	CEGUI::String strUtf32;
	CUIStringSystem::GetMe()->ParserString_Runtime(caption, strUtf32);
	((CEGUI::MultiColumnList*)m_pWindow)->addColumn( strUtf32, args[ 3 ].GetInteger(), args[ 4 ].GetFloat() );

	return 0;
}
INT CtrlList::InsertColumn( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	std::string caption = args[2].GetString();
	CEGUI::String strUtf32;
	CUIStringSystem::GetMe()->ParserString_Runtime(caption, strUtf32);
	((CEGUI::MultiColumnList*)m_pWindow)->insertColumn(	strUtf32, args[ 3 ].GetInteger(), args[ 4 ].GetFloat(), args[ 5 ].GetInteger() );
	
	return 0;
}
INT CtrlList::AddNewItem( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	std::string caption = args[2].GetString();
	CEGUI::String strUtf32;
	CUIStringSystem::GetMe()->ParserString_Runtime(caption, strUtf32);
	CEGUI::ListboxTextItem* pItem = new CEGUI::ListboxTextItem( strUtf32, args[ 3 ].GetInteger() );
	CEGUI::colour colorText = CEGUI::colour(1.0f, 1.0f, 1.0f);
	pItem->setTextColours(colorText);
	pItem->setSelectionColours(CEGUI::colour(1.0f, 0.0f, 0.0f));
	pItem->setSelectionBrushImage((CEGUI::utf8*)"Common", (CEGUI::utf8*)"HoverTextBak");

	//Find first listboxitem in row.
	CEGUI::MCLGridRef  grid_ref( args[ 4 ].GetInteger(), 0 );
	CEGUI::ListboxItem* pExistItem = 0;

	unsigned int rowC = ((CEGUI::MultiColumnList*)m_pWindow)->getRowCount();
	unsigned int columC = ((CEGUI::MultiColumnList*)m_pWindow)->getColumnCount();
	if(grid_ref.row < rowC && grid_ref.column < columC)
	{
		pExistItem = ((CEGUI::MultiColumnList*)m_pWindow)->getItemAtGridReference( grid_ref );
	}
	if(pExistItem)
	{//exist, add subItem
		((CEGUI::MultiColumnList*)m_pWindow)->setItem(pItem, args[3].GetInteger(), args[4].GetInteger());
	}
	else
	{//not exist, add new Item
		//set firt listboxitem's ID as row idx. for CtrlList::GetSelectItem
		pItem->setID(args[4].GetInteger());
		((CEGUI::MultiColumnList*)m_pWindow)->addRow( pItem, args[ 3 ].GetInteger(), args[ 4 ].GetInteger() );
	}
	return 0;
}
INT CtrlList::SetItemText( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	CEGUI::MCLGridRef  grid_ref( args[ 2 ].GetInteger(), args[ 3 ].GetInteger() );
	CEGUI::ListboxItem* pItem = 0;

	unsigned int rowC = ((CEGUI::MultiColumnList*)m_pWindow)->getRowCount();
	unsigned int columC = ((CEGUI::MultiColumnList*)m_pWindow)->getColumnCount();
	if(grid_ref.row < rowC && grid_ref.column < columC)
	{
		pItem = ((CEGUI::MultiColumnList*)m_pWindow)->getItemAtGridReference( grid_ref );
	}
	if( pItem )
	{
		std::string caption = args[4].GetString();
		CEGUI::String strUtf32;
		CUIStringSystem::GetMe()->ParserString_Runtime(caption, strUtf32);
		pItem->setText( strUtf32 );
	}
	return 0;
}
INT CtrlList::SetItemData( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	CEGUI::MCLGridRef  grid_ref( args[ 2 ].GetInteger(), args[ 3 ].GetInteger() );
	CEGUI::ListboxItem* pItem = 0;

	unsigned int rowC = ((CEGUI::MultiColumnList*)m_pWindow)->getRowCount();
	unsigned int columC = ((CEGUI::MultiColumnList*)m_pWindow)->getColumnCount();
	if(grid_ref.row < rowC && grid_ref.column < columC)
	{
		pItem = ((CEGUI::MultiColumnList*)m_pWindow)->getItemAtGridReference( grid_ref );
	}
	if( pItem )
	{
	}
	return 0;
}
INT CtrlList::DeleteItem( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	((CEGUI::MultiColumnList*)m_pWindow)->removeRow( args[ 2 ].GetInteger() );
	return 0;
}
INT CtrlList::RemoveAllItem( LuaPlus::LuaState* pState )
{
	((CEGUI::MultiColumnList*)m_pWindow)->resetList();
	return 0;
}
INT CtrlList::GetItemText( LuaPlus::LuaState* pState )
{
	LuaPlus::LuaStack args(pState);
	CEGUI::MCLGridRef  grid_ref( args[ 2 ].GetInteger(), args[ 3 ].GetInteger() );
	CEGUI::ListboxItem* pItem = 0;

	unsigned int rowC = ((CEGUI::MultiColumnList*)m_pWindow)->getRowCount();
	unsigned int columC = ((CEGUI::MultiColumnList*)m_pWindow)->getColumnCount();
	if(grid_ref.row < rowC && grid_ref.column < columC)
	{
		pItem = ((CEGUI::MultiColumnList*)m_pWindow)->getItemAtGridReference( grid_ref );
	}
	if( pItem )
	{
		std::string strUtf8 = pItem->getText().c_str();//utf::mbcs_to_utf8(strMbcs);
		std::string strMbcs;
		CUIStringSystem::utf8_to_mbcs(strUtf8, strMbcs);
		pState->PushString( strMbcs.c_str() );
	}
	
	return 1;
}
INT CtrlList::GetItemCount( LuaPlus::LuaState* pState )
{
	pState->PushInteger( ((CEGUI::MultiColumnList*)m_pWindow)->getRowCount() );
	return 1;
}
INT CtrlList::GetColumnCount( LuaPlus::LuaState* pState )
{
	pState->PushInteger( ((CEGUI::MultiColumnList*)m_pWindow)->getColumnCount() );
	return 1;
}
INT CtrlList::GetSelectItem( LuaPlus::LuaState* pState )
{
	CEGUI::ListboxItem* pItem = (CEGUI::ListboxItem*)((CEGUI::MultiColumnList*)m_pWindow)->getFirstSelectedItem();
	if(!pItem) 
		pState->PushInteger(-1);
	else
		pState->PushInteger(pItem->getID());

	return 1;
}

LuaPlus::LuaObject* SoftKeyWindow::s_pMetaTable = NULL;

INT SoftKeyWindow::SetAimEditBox( LuaPlus::LuaState* pState)
{
	LuaPlus::LuaStack args(pState);
	if( !args[ 2 ].IsString() ) return 0;
	CEGUI::IFalagardSoftKeyBoard * pWindow= (CEGUI::IFalagardSoftKeyBoard *)(CEGUI::FalagardSoftKeyBoard*)m_pWindow;
	pWindow->SetActiveEditBox( (CEGUI::String)( CEGUI::utf8*)(args[2].GetString()) );
	return 0;
}

LuaPlus::LuaObject* AnimateWindow::s_pMetaTable = NULL;

INT AnimateWindow::Play( LuaPlus::LuaState* pState)
{
	CEGUI::IFalagardAnimate * pWindow= (CEGUI::IFalagardAnimate*)(CEGUI::FalagardAnimate*)m_pWindow;
	LuaPlus::LuaStack args(pState);
	pWindow->Play( args[ 2 ].GetBoolean() );
	return 0;
}
} // end namespace
