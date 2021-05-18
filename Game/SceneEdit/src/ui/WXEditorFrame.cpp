// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "WXEditorFrame.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
    #include <wx/pen.h>
    #include <wx/dcscreen.h>
    #include <wx/dcmemory.h>
    #include <wx/scrolwin.h>
    #include <wx/toolbar.h>
    #include <wx/statusbr.h>
    #include <wx/stattext.h>
    #include <wx/button.h>
#endif

#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/spinctrl.h>
#include <wx/busyinfo.h>
#include <wx/config.h>
#include <wx/stockitem.h>
#include <wx/filename.h>

// copy paste
#include <wx/dataobj.h>
#include <wx/clipbrd.h>

#include "WXEditorFrame.h"
#include "WXSceneCanvas.h"
#include "BrushSelector.h"
#include "ModelSelector.h"
#include "ActorSelector.h"

#include ".\RegioneditdlgSelector.h"		// 不可行走区域编辑对话框.
#include ".\MonsterEditDlg.h"				// 怪物编辑对话框.
#include ".\GrowPointDlg.h"					// 生长点编辑.
#include ".\NpcPatrolDlg.h"					// npc巡逻编辑.
#include ".\CEventAreaEditDlg.h"			// 事件区域
#include ".\stallinfodlg.h"					// 摊位信息
#include ".\PathFindDlg.h"

#include "ParticleSystemSelector.h"
#include "SceneBrowser.h"
#include "ObjectPropertyEditor.h"
#include "TerrainSelections.h"
#include "LiquidCreatorDialog.h"
#include "SceneManipulator.h"
#include "WXOperatorManager.h"
#include "Action.h"
#include "NewSceneDialog.h"
#include "SceneInfoMapDialog.h"
#include "ExceptionInfoDialog.h"
//#include "SpinnerSettingDialog.h"
#include "SettingDialogs.h"
#include "TerrainLightmapOptionDialog.h"
#include "ReshapeDialog.h"

#include "EraseTexDialog.h"
#include "ui/res/EraseTexDialog_wdr.h"

#include "MonsterEditAction.h"		// 怪物编辑action
#include "GrowPointEditaction.h"		// 生长点action

#include "Core/FrameStatsListener.h"
#include "Core/WXSceneInfo.h"
#include "Core/WXObject.h"

#include "GpuProgramParameterSettingDialog.h"
#include "TerrainSurfaceParametersDialog.h"
#include "MiniMapMakerDialog.h"
#include "SceneInfomationDialog.h"
#include "CameraTrackSettingDialog.h"
#include "SoundEditDlg.h"
#include "DatuPointEditDlg.h"

#include "AutoSaveSceneController.h"

// ----------------------------------------------------------------------------
// wxDockIt headers
// ----------------------------------------------------------------------------

#include <wx/layoutmanager.h>
#include <wx/dockwindow.h>
#include <wx/dockhost.h>
#include <wx/accel.h>

//-----------------------------------------------------------------------------
// File header
//-----------------------------------------------------------------------------
#include <wx/file.h>

//-----------------------------------------------------------------------------
// Dialogs
//-----------------------------------------------------------------------------
#include <wx/numdlg.h>

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------

#include <OgreTextureManager.h>
#include <OgreMaterialManager.h>
#include <OgreStringConverter.h>
#include <OgreCamera.h>
#include <OgreException.h>
#include <OgreRenderWindow.h>

#include "PostFilter/WXPostFilterManager.h"
#include "PostFilter/WXPostFilter.h"

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

enum
{
    feID_BASE = wxID_HIGHEST + 1001,

    feID_PROPERTIES_VIEWER = feID_BASE + 100,       // wxPropertyGridManager request id greater than 0
    feID_LOCK_BRUSH_SIZE,
    feID_BRUSH_SIZE_X,
    feID_BRUSH_SIZE_Z,
 
    feID_TERRAIN_HEIGHT,
	feID_TERRAIN_HEIGHT_INCREMENT,
    feID_HEIGHT_ACTION_AFFECT_RANGE,
	feID_HEIGHT_ACTION_EXP_VALUE,
	feID_HEIGHT_ADJUST_SPEED,

	// hack，临时设置泛光效果
	feID_BLURAMOUNT,
	feID_SHINEAMOUNT,
	feID_FRAMERATE,

	feID_BLURAMOUNTSTATICTEXT,
	feID_SHINEAMOUNTSTATICTEXT,

    feID_LOCK_CAMERA_HEIGHT = feID_BASE + 200,
	feID_SELECTALLOBJECTS,
    feID_SHOW_STAT_PANEL,
    feID_SHOW_LOGO_PANEL,
    feID_TOGGLE_TOOL_WINDOWS,
    feID_FULL_SCREEN,
	feID_RESET_CAMERA,

    feID_ACTION_FIRST   = feID_BASE + 300,

    feID_DRAG_HEIGHT_ACTION = feID_ACTION_FIRST,
    feID_RAISE_HEIGHT_ACTION,
    feID_LOWER_HEIGHT_ACTION,
    feID_SMOOTH_HEIGHT_ACTION,
    feID_LEVEL_OFF_HEIGHT_ACTION,
    feID_SET_TERRAIN_HEIGHT_ACTION,
    feID_TERRAIN_LAYER_ONE_ERASER_ACTION,
    feID_MANIP_OBJECT_ACTION,
    feID_FLIP_DIAGONAL_ACTION,
    feID_DISABLE_DECAL_ACTION,
    feID_TERRAIN_SELECTION_ACTION,	
    feID_ACTION_LAST,

	feID_ERASE_BY_TEXTURE_NAME,

    feID_ENTIRE_TERRAIN_ENABLE_DECAL,

	feID_TERRAIN_LIGHTMAP_OPTION,		// 先入驻这里，找到更好的位置再挪过去
	feID_TERRAIN_LIGHTMAP_HIDE,
	feID_TERRAIN_LIGHTMAP_LOW_QUALITY,
	feID_TERRAIN_LIGHTMAP_HIGH_QUALITY,

    feID_TERRAIN_SURFACE_PARAMETERS,
    feID_SCENE_ANIMATION_QUALITY,
    feID_OBJECT_CREATE_LEVEL,

    feID_CREATE_MINI_MAP,

	feID_BOX_BRUSH,
	feID_ELLIPSE_BRUSH,

    feID_ALIGN_POSITION,

    feID_PREFERSETTING,
	feID_GPU_PROGRAM_PARAMETER_SETTING,

    feID_SHOW_STANDARD_MODEL,
    feID_DELETESTATICOBJECT,
    feID_SAVETERRAININFO,
    feID_LOADTERRAININFO,
	feID_SAVEMANIPINFO,
	feID_LOADMANIPINFO,
	feID_SAVEENVIROMENTINFO,
	feID_LOADENVIROMENTINFO,

    feID_SCENE_INFOMATION,
    
	feID_CREATE_LIQUID,
	feID_CREATE_LIGHT,
	
    feID_POST_FILTER_FIRST = feID_BASE + 500,
    feID_PF_HEATVISION = feID_POST_FILTER_FIRST,
    feID_PF_BLOOM,
    feID_PF_FLOODLIGHTING,
    feID_PF_FADE,
    feID_PF_MOTION_BLUR,
    feID_POST_FILTER_LAST,

    feID_SHADOWTYPE_NONE,
    feID_SHADOWTYPE_TEXTURE_ADDITIVE,
    feID_SHADOWTYPE_TEXTURE_MODULATIVE,
    feID_SHADOWTYPE_STENCIL_ADDITIVE,
    feID_SHADOWTYPE_STENCIL_MODULATIVE,

    feID_TFO_NONE,
    feID_TFO_BILINEAR,
    feID_TFO_TRILINEAR,
    feID_TFO_ANISOTROPIC,

    feID_CL_LOW,
    feID_CL_MID,
    feID_CL_HIGH,

	feID_RESHAPE_TERRAIN,
    feID_ENABLE_32BIT_TEXTURES,
    feID_RELOAD_TEXTURES,
    feID_UNLOAD_USELESS_RESOURCES,
    feID_SHOW_INDICATORS,

	// 显示和跳转游戏逻辑坐标
	feID_COORDINATE_FIRST   = feID_BASE + 1000,
	feID_STATIC_SHOW_X,
	feID_STATIC_SHOW_Y,
	feID_SHOW_X,
	feID_SHOW_Y,
	feID_STATIC_GOTO_X,
	feID_STATIC_GOTO_Y,
	feID_GOTO_X,
	feID_GOTO_Y,
	feID_GOTO_BN,


	feID_USE_REAL_CAMERA,
	feID_USE_SNAP_ROTATION,

};

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

IMPLEMENT_CLASS(FairyEditorFrame, wxFrame)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(FairyEditorFrame, wxFrame)
    WX_EVENT_TABLE_CONTROL_CONTAINER(FairyEditorFrame)

    EVT_MENU(wxID_NEW, FairyEditorFrame::OnFileNew)

    EVT_MENU(wxID_OPEN, FairyEditorFrame::OnFileOpen)
    EVT_MENU(wxID_SAVE, FairyEditorFrame::OnFileSave)
    EVT_MENU(wxID_SAVEAS, FairyEditorFrame::OnFileSaveAs)

    EVT_MENU(wxID_CUT, FairyEditorFrame::OnCut)
    EVT_MENU(wxID_COPY, FairyEditorFrame::OnCopy)
    EVT_MENU(wxID_PASTE, FairyEditorFrame::OnPaste)

    EVT_UPDATE_UI(wxID_CUT, FairyEditorFrame::OnUpdateCutCopyPaste)
    EVT_UPDATE_UI(wxID_COPY, FairyEditorFrame::OnUpdateCutCopyPaste)
    EVT_UPDATE_UI(wxID_PASTE, FairyEditorFrame::OnUpdateCutCopyPaste)

    EVT_MENU(wxID_UNDO, FairyEditorFrame::OnUndo)
    EVT_MENU(wxID_REDO, FairyEditorFrame::OnRedo)

    EVT_MENU(feID_LOCK_CAMERA_HEIGHT, FairyEditorFrame::OnLockCameraHeight)
	EVT_MENU(feID_SELECTALLOBJECTS, FairyEditorFrame::OnSelectAllObjects)
    EVT_MENU(feID_SHOW_STAT_PANEL, FairyEditorFrame::OnShowStatPanel)
    EVT_MENU(feID_SHOW_LOGO_PANEL, FairyEditorFrame::OnShowLogoPanel)
    EVT_MENU(feID_PREFERSETTING, FairyEditorFrame::OnPreferSetting)
	EVT_MENU(feID_GPU_PROGRAM_PARAMETER_SETTING, FairyEditorFrame::OnGpuProgramParameterSetting)
	EVT_MENU(feID_ERASE_BY_TEXTURE_NAME, FairyEditorFrame::OnEraseTextureByName)

    EVT_MENU(feID_ENTIRE_TERRAIN_ENABLE_DECAL, FairyEditorFrame::OnEntireTerrainEnableDecal)

	EVT_MENU(feID_RESET_CAMERA, FairyEditorFrame::OnResetCamera)

    EVT_MENU(feID_SHOW_STANDARD_MODEL, FairyEditorFrame::OnShowStandardModel)
    EVT_MENU(feID_DELETESTATICOBJECT,FairyEditorFrame::OnDeleteStaticObject)
    EVT_UPDATE_UI(feID_DELETESTATICOBJECT,FairyEditorFrame::OnUpdateDeleteStaticObject)

    EVT_MENU(feID_SAVETERRAININFO,FairyEditorFrame::OnSaveTerrainInfoToFile)
    EVT_UPDATE_UI(feID_SAVETERRAININFO,FairyEditorFrame::OnUpdateSaveTerrainInfoToFile)
    EVT_MENU(feID_LOADTERRAININFO,FairyEditorFrame::OnLoadTerrainInfoFromFile)
    EVT_UPDATE_UI(feID_LOADTERRAININFO,FairyEditorFrame::OnUpdateLoadTerrainInfoFromFile)

	EVT_MENU(feID_SAVEMANIPINFO,FairyEditorFrame::OnSaveManipInfoToFile)
	EVT_UPDATE_UI(feID_SAVEMANIPINFO,FairyEditorFrame::OnUpdateSaveManipInfoToFile)
	EVT_MENU(feID_LOADMANIPINFO,FairyEditorFrame::OnLoadManipInfoFromFile)
	EVT_UPDATE_UI(feID_LOADMANIPINFO,FairyEditorFrame::OnUpdateLoadManipInfoFromFile)

	EVT_MENU(feID_SAVEENVIROMENTINFO, FairyEditorFrame::OnSaveEnviromentInfoToFile)
	EVT_UPDATE_UI(feID_SAVEENVIROMENTINFO, FairyEditorFrame::OnUpdateSaveEnviromentInfoToFile)
	EVT_MENU(feID_LOADENVIROMENTINFO, FairyEditorFrame::OnLoadEnviromentInfoFromFile)
	EVT_UPDATE_UI(feID_LOADENVIROMENTINFO, FairyEditorFrame::OnUpdateLoadEnviromentInfoFromFile)

    EVT_MENU(feID_SCENE_INFOMATION, FairyEditorFrame::OnSceneInfomation)

    EVT_MENU(feID_TOGGLE_TOOL_WINDOWS, FairyEditorFrame::OnToggleToolWindows)
    EVT_MENU(feID_FULL_SCREEN, FairyEditorFrame::OnFullScreen)
    EVT_UPDATE_UI(feID_TOGGLE_TOOL_WINDOWS, FairyEditorFrame::OnUpdateToggleToolWindows)
    EVT_UPDATE_UI(feID_FULL_SCREEN, FairyEditorFrame::OnUpdateFullScreen)


    EVT_UPDATE_UI(wxID_NEW, FairyEditorFrame::OnUpdateFileNew)
    EVT_UPDATE_UI(wxID_SAVE, FairyEditorFrame::OnUpdateSave)
    EVT_UPDATE_UI(wxID_SAVEAS, FairyEditorFrame::OnUpdateSaveAs)

    EVT_UPDATE_UI(wxID_UNDO, FairyEditorFrame::OnUpdateUndo)
    EVT_UPDATE_UI(wxID_REDO, FairyEditorFrame::OnUpdateRedo)

    EVT_UPDATE_UI(feID_LOCK_CAMERA_HEIGHT, FairyEditorFrame::OnUpdateLockCameraHeight)
    EVT_UPDATE_UI(feID_SELECTALLOBJECTS, FairyEditorFrame::OnUpdateSelectAllObjects)
    EVT_UPDATE_UI(feID_SHOW_STAT_PANEL, FairyEditorFrame::OnUpdateShowStatPanel)
    EVT_UPDATE_UI(feID_SHOW_LOGO_PANEL, FairyEditorFrame::OnUpdateShowLogoPanel)
    EVT_UPDATE_UI(feID_SHOW_STANDARD_MODEL, FairyEditorFrame::OnUpdateShowStandardModel)

    EVT_MENU(feID_LOCK_BRUSH_SIZE, FairyEditorFrame::OnLockBrushSize)
    EVT_UPDATE_UI(feID_LOCK_BRUSH_SIZE, FairyEditorFrame::OnUpdateLockBrushSize)
    EVT_SPINCTRL(feID_BRUSH_SIZE_X, FairyEditorFrame::OnBrushSizeChanged)
    EVT_SPINCTRL(feID_BRUSH_SIZE_Z, FairyEditorFrame::OnBrushSizeChanged)
    EVT_COMMAND(feID_BRUSH_SIZE_X, wxEVT_COMMAND_TEXT_UPDATED, FairyEditorFrame::OnBrushSizeTextChanged)
    EVT_COMMAND(feID_BRUSH_SIZE_Z, wxEVT_COMMAND_TEXT_UPDATED, FairyEditorFrame::OnBrushSizeTextChanged)
    EVT_UPDATE_UI(feID_BRUSH_SIZE_X, FairyEditorFrame::OnUpdateBrushSizeSpin)
    EVT_UPDATE_UI(feID_BRUSH_SIZE_Z, FairyEditorFrame::OnUpdateBrushSizeSpin)

    EVT_SPINCTRL(feID_HEIGHT_ACTION_AFFECT_RANGE, FairyEditorFrame::OnHeightActionaffectRangeChanged)
    EVT_COMMAND(feID_HEIGHT_ACTION_AFFECT_RANGE, wxEVT_COMMAND_TEXT_UPDATED, FairyEditorFrame::OnHeightActionaffectRangeTextChanged)
    EVT_UPDATE_UI(feID_HEIGHT_ACTION_AFFECT_RANGE, FairyEditorFrame::OnUpdateHeightActionaffectRangeSpin)

	EVT_TEXT_ENTER(feID_HEIGHT_ACTION_EXP_VALUE, FairyEditorFrame::OnHeightActionExpValueTextChanged)
	EVT_UPDATE_UI(feID_HEIGHT_ACTION_EXP_VALUE, FairyEditorFrame::OnUpdateHeightActionExpValue)

	EVT_TEXT_ENTER(feID_HEIGHT_ADJUST_SPEED, FairyEditorFrame::OnHeightAdjustSpeedTextChanged)
	EVT_UPDATE_UI(feID_HEIGHT_ADJUST_SPEED, FairyEditorFrame::OnUpdateHeightAdjustSpeed)

	// HACK 设置泛光效果
	EVT_TEXT_ENTER(feID_BLURAMOUNT, FairyEditorFrame::OnBlurAmountTextChanged)
	EVT_UPDATE_UI(feID_BLURAMOUNT, FairyEditorFrame::OnUpdateFloodlightingControls)
	EVT_TEXT_ENTER(feID_SHINEAMOUNT, FairyEditorFrame::OnShineAmountTextChanged)
	EVT_UPDATE_UI(feID_SHINEAMOUNT, FairyEditorFrame::OnUpdateFloodlightingControls)

    EVT_SPINCTRL(feID_TERRAIN_HEIGHT, FairyEditorFrame::OnTerrainHeightChanged)
    EVT_COMMAND(feID_TERRAIN_HEIGHT, wxEVT_COMMAND_TEXT_UPDATED, FairyEditorFrame::OnTerrainHeightTextChanged)
    EVT_UPDATE_UI(feID_TERRAIN_HEIGHT, FairyEditorFrame::OnUpdateTerrainHeightSpin)

	EVT_SPINCTRL(feID_TERRAIN_HEIGHT_INCREMENT, FairyEditorFrame::OnTerrainHeightIncrementChanged)
	EVT_COMMAND(feID_TERRAIN_HEIGHT_INCREMENT, wxEVT_COMMAND_TEXT_UPDATED, FairyEditorFrame::OnTerrainHeightIncrementTextChanged)
	EVT_UPDATE_UI(feID_TERRAIN_HEIGHT_INCREMENT, FairyEditorFrame::OnUpdateTerrainHeightIncrementSpin)

    EVT_MENU_RANGE(feID_ACTION_FIRST, feID_ACTION_LAST-1, FairyEditorFrame::OnSelectAction)
    EVT_UPDATE_UI_RANGE(feID_ACTION_FIRST, feID_ACTION_LAST-1, FairyEditorFrame::OnUpdateSelectAction)

	EVT_MENU(feID_TERRAIN_LIGHTMAP_OPTION, FairyEditorFrame::OnTerrainLightmapOption)
	EVT_MENU(feID_TERRAIN_LIGHTMAP_HIDE, FairyEditorFrame::OnTerrainLightmapHide)
	EVT_UPDATE_UI(feID_TERRAIN_LIGHTMAP_HIDE, FairyEditorFrame::OnUpdateTerrainLightmapHide)
	EVT_MENU(feID_TERRAIN_LIGHTMAP_LOW_QUALITY, FairyEditorFrame::OnTerrainLightmapLowQuality)
	EVT_UPDATE_UI(feID_TERRAIN_LIGHTMAP_LOW_QUALITY, FairyEditorFrame::OnUpdateTerrainLightmapLowQuality)
	EVT_MENU(feID_TERRAIN_LIGHTMAP_HIGH_QUALITY, FairyEditorFrame::OnTerrainLightmapHighQuality)	
	EVT_UPDATE_UI(feID_TERRAIN_LIGHTMAP_HIGH_QUALITY, FairyEditorFrame::OnUpdateTerrainLightmapHighQuality)

    EVT_MENU(feID_TERRAIN_SURFACE_PARAMETERS, FairyEditorFrame::OnTerrainSurfaceParameters)
    EVT_MENU(feID_SCENE_ANIMATION_QUALITY, FairyEditorFrame::OnSceneAnimationQuality)
    EVT_MENU(feID_OBJECT_CREATE_LEVEL, FairyEditorFrame::OnObjectCreateLevel)

    EVT_MENU(feID_CREATE_MINI_MAP, FairyEditorFrame::OnCreateMiniMap)

	EVT_MENU_RANGE(feID_BOX_BRUSH, feID_ELLIPSE_BRUSH, FairyEditorFrame::OnSelectBrushShape)
	EVT_UPDATE_UI_RANGE(feID_BOX_BRUSH, feID_ELLIPSE_BRUSH, FairyEditorFrame::OnUpdateSelectBrushShape)

    EVT_MENU(feID_ALIGN_POSITION, FairyEditorFrame::OnAlignPosition)
    EVT_UPDATE_UI(feID_ALIGN_POSITION, FairyEditorFrame::OnUpdateAlignPosition)

    EVT_MENU(wxID_EXIT, FairyEditorFrame::OnExit)

    EVT_CLOSE(FairyEditorFrame::OnCloseWindow)

	EVT_MENU(feID_CREATE_LIQUID,FairyEditorFrame::OnCreateLiquid)
	EVT_MENU(feID_CREATE_LIGHT,FairyEditorFrame::OnCreateLight)

    EVT_MENU_RANGE(feID_POST_FILTER_FIRST, feID_POST_FILTER_LAST-1, FairyEditorFrame::OnSelectPostFilter)
    EVT_UPDATE_UI_RANGE(feID_POST_FILTER_FIRST, feID_POST_FILTER_LAST-1, FairyEditorFrame::OnUpdateSelectPostFilter)

    EVT_MENU_RANGE(feID_SHADOWTYPE_NONE, feID_SHADOWTYPE_NONE+4, FairyEditorFrame::OnSelectShadowTechnique)
    EVT_UPDATE_UI_RANGE(feID_SHADOWTYPE_NONE, feID_SHADOWTYPE_NONE+4, FairyEditorFrame::OnUpdateSelectShadowTechnique)

    EVT_MENU_RANGE(feID_TFO_NONE, feID_TFO_NONE+3, FairyEditorFrame::OnSelectDefaultTextureFiltering)
    EVT_UPDATE_UI_RANGE(feID_TFO_NONE, feID_TFO_NONE+3, FairyEditorFrame::OnUpdateSelectDefaultTextureFiltering)

    EVT_MENU_RANGE(feID_CL_LOW, feID_CL_LOW + 2, FairyEditorFrame::OnSelectSceneCreateLevel)
    EVT_UPDATE_UI_RANGE(feID_CL_LOW, feID_CL_LOW + 2, FairyEditorFrame::OnUpdateSelectSceneCreateLevel)

	EVT_MENU(feID_RESHAPE_TERRAIN,FairyEditorFrame::OnReshapeTerrain)
	EVT_UPDATE_UI(feID_RESHAPE_TERRAIN,FairyEditorFrame::OnUpdateReshapeTerrain)
    EVT_MENU(feID_ENABLE_32BIT_TEXTURES,FairyEditorFrame::OnEnable32BitTextures)
	EVT_UPDATE_UI(feID_ENABLE_32BIT_TEXTURES,FairyEditorFrame::OnUpdateEnable32BitTextures)
    EVT_MENU(feID_RELOAD_TEXTURES,FairyEditorFrame::OnReloadTextures)
	EVT_UPDATE_UI(feID_RELOAD_TEXTURES,FairyEditorFrame::OnUpdateReloadTextures)
    EVT_MENU(feID_UNLOAD_USELESS_RESOURCES,FairyEditorFrame::OnUnloadUselessResources)
	EVT_UPDATE_UI(feID_UNLOAD_USELESS_RESOURCES,FairyEditorFrame::OnUpdateUnloadUselessResources)
    EVT_MENU(feID_SHOW_INDICATORS, FairyEditorFrame::OnShowAllIndicators)
    EVT_UPDATE_UI(feID_SHOW_INDICATORS, FairyEditorFrame::OnUpdateShowAllIndicators)

	// 地图跳转
	EVT_BUTTON(feID_GOTO_BN, FairyEditorFrame::OnSceneGoto)

	EVT_MENU(feID_USE_REAL_CAMERA, FairyEditorFrame::OnUseRealCamera)
	EVT_UPDATE_UI(feID_USE_REAL_CAMERA, FairyEditorFrame::OnUpdateUseRealCamera)

	EVT_MENU(feID_USE_SNAP_ROTATION, FairyEditorFrame::OnUseSnapRotation)
	EVT_UPDATE_UI(feID_USE_SNAP_ROTATION, FairyEditorFrame::OnUpdateUseSnapRotation)

	EVT_TEXT_ENTER(feID_FRAMERATE, FairyEditorFrame::OnFrameRateTextChanged)

END_EVENT_TABLE()

WX_DELEGATE_TO_CONTROL_CONTAINER(FairyEditorFrame)

static wxSize
CombineBestSize(const wxSize& a, const wxSize& b)
{
    return wxSize(wxMax(a.x, b.x), wxMax(a.y, b.y));
}

FairyEditorFrame::FairyEditorFrame(wxFrame *parent, wxWindowID id, const wxString& title,
                                   const wxPoint& pos, const wxSize& size, long style,
                                   const wxString& name)
    : wxFrame(parent, id, title, pos, size, style | wxTAB_TRAVERSAL, name)

    , mCanvas(NULL)

	, m_pRegionEditSelector(NULL)				//不可行走区域编辑对话框.
	, m_pMonsterEditDlg(NULL)					// 怪物编辑对话框。
	, m_pGrowPointEditDlg(NULL)					// 生长点。
	, m_pNpcPatrolEditDlg(NULL)					// 怪物巡逻路线
	, m_pEventAreaEditDlg(NULL)					// 事件区域编辑
	, m_pStallInfoDlg(NULL)						// 摊位信息.
	, m_pPathFindEditDlg(NULL)

    , mBrushSelector(NULL)
    , mModelSelector(NULL)
    , mActorSelector(NULL)
    , mParticleSystemSelector(NULL)
    , mSceneBrowser(NULL)
    , mObjectPropertyEditor(NULL)
    , mSceneInfoDlg(NULL)
	
    , mLogWindow(NULL)

    , mLayout(NULL)
    , mWindowMenu(NULL)

    , mLockBrushSize(true)
    , mSpinnerValue(1)
    , mByAxis("YAxis")
    , mSegment(4)
    , mActiveSnapRotation(false)

	, mIsTerrainLightmapUpdated(false)

	, m_pToolbar(NULL)
	, m_pSceneManipulator(NULL)

	, mUseRealCamera(false)
	, mUseSnapRotation(false)

    , mTextureFilterOptions(1)
    , mCreateLevelOptions(3)

	, mGpuProgramParameterSettingDialog(NULL)
    , mSceneInfomationDialog(NULL)
    , mAutoSaveSceneController(NULL)
    //, mCameraTrackSettingDialog(NULL)
    , mSoundEditDialog(NULL)
    , mDatuPointEditDialog(NULL)
{
    m_container.SetContainerWindow(this);

    mCanvas = new FairySceneCanvas(this, wxID_ANY);

    m_container.SetDefaultItem(mCanvas);

    wxToolBar* toolbar = CreateToolBar();

	// 记录工具栏
	m_pToolbar = toolbar;
    wxStatusBar* statusbar = CreateStatusBar(2, wxSB_NORMAL);


    InitMenuBar();
    InitToolBar(toolbar);
    InitStatusBar(statusbar);

    //////////////////////////////////////////////////////////////////////////

    mLayout = new wxLayoutManager(this);
    mLayout->AddDefaultHosts();

    mLayout->SetLayout(
        wxDWF_LIVE_UPDATE | wxDWF_SPLITTER_BORDERS,
        mCanvas);

	// the wxLayoutManager can be used to handle automatically a "window menu";
	// that is, it can show/hide all the windows under its control through a menu	
    mLayout->SetWindowMenu(mWindowMenu);

    wxSize bestSize;
    wxHostInfo hostInfo;

    //////////////////////////////////////////////////////////////////////////

    hostInfo = mLayout->GetDockHost(wxDEFAULT_BOTTOM_HOST);

    wxTextCtrl* textCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
        wxDefaultPosition, wxDefaultSize,
        wxTE_MULTILINE);
    delete wxLog::SetActiveTarget(NULL);
    wxLogTextCtrl* logTextCtrl = new wxLogTextCtrl(textCtrl);
    wxLogChain* logChain = new wxLogChain(logTextCtrl);
    CreateDockWindow(textCtrl,
        /*_("Log Window")*/wxT("日志窗口"),
        wxT("日志窗口"),
        hostInfo);
    bestSize = textCtrl->GetBestSize();

    hostInfo.pHost->SetAreaSize(bestSize.y);

    mLogWindow = textCtrl;

    //////////////////////////////////////////////////////////////////////////

    hostInfo = mLayout->GetDockHost(wxDEFAULT_LEFT_HOST);

	wxNotebook* notebookEdit = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_MULTILINE|wxNB_TOP);
    
	// 物体编辑
	mSceneBrowser = new SceneBrowser(notebookEdit, wxID_ANY);
	//notebookEdit->AddPage(mSceneBrowser, _("SceneBrowser"));
	notebookEdit->AddPage(mSceneBrowser, _("场景浏览器"));

	//添加不可行走区域编辑对话框.
	m_pRegionEditSelector = new CRegionEditDlgSector(notebookEdit);
    //notebookEdit->AddPage(m_pRegionEditSelector, _("RegionEdit"));
	notebookEdit->AddPage(m_pRegionEditSelector, wxT("不可行走区域编辑"));

	// 添加怪物编辑区域对话框。
	m_pMonsterEditDlg = new CMonsterEditDlg(notebookEdit);
	//notebookEdit->AddPage(m_pMonsterEditDlg, _("MonsterEdit"));
	notebookEdit->AddPage(m_pMonsterEditDlg, wxT("怪物npc编辑"));


	// 添加生长点编辑区域对话框。
	m_pGrowPointEditDlg = new CGrowPointEditDlg(notebookEdit);
	//notebookEdit->AddPage(m_pGrowPointEditDlg, _("GrowPointEdit"));
	notebookEdit->AddPage(m_pGrowPointEditDlg, wxT("生长点编辑"));
	// 设置父窗口
	m_pGrowPointEditDlg->SetParent(this);



	// 添加ncp巡逻编辑区域对话框。
	m_pNpcPatrolEditDlg = new CNpcPatrolEditDlg(notebookEdit);
	//notebookEdit->AddPage(m_pNpcPatrolEditDlg, _("NpcpatrolEdit"));
	notebookEdit->AddPage(m_pNpcPatrolEditDlg, wxT("npc巡逻路线编辑"));

	// 事件区域编辑对话框。
	m_pEventAreaEditDlg = new CEventAreaEditDlg(notebookEdit);
	notebookEdit->AddPage(m_pEventAreaEditDlg, wxT("事件区域编辑"));
    //notebookEdit->AddPage(m_pEventAreaEditDlg, _("EventAreaEdit"));


	// 摊位信息编辑对话框。
	m_pStallInfoDlg = new CStallInfoDlg(notebookEdit);
	notebookEdit->AddPage(m_pStallInfoDlg, wxT("摊位信息编辑"));
    //notebookEdit->AddPage(m_pStallInfoDlg, _("StallInfoEdit"));

    //mCameraTrackSettingDialog = new CameraTrackSettingDialog(notebookEdit);
	//notebookEdit->AddPage(mCameraTrackSettingDialog, _("CameraTrackSetting"));
	//notebookEdit->AddPage(mCameraTrackSettingDialog, wxT("摄像机轨迹设置"));

	// 声音信息编辑对话框。
    mSoundEditDialog = new SoundEditDialog(notebookEdit);
	notebookEdit->AddPage(mSoundEditDialog, wxT("声音编辑"));
	//notebookEdit->AddPage(mSoundEditDialog, _("Sound Edit"));


	// 添加寻路点对话框。
	m_pPathFindEditDlg = new CPathFindEditDlg(notebookEdit);
	notebookEdit->AddPage(m_pPathFindEditDlg, wxT("寻路路径编辑"));

	mDatuPointEditDialog = new DatuPointEditDialog(notebookEdit);
    notebookEdit->AddPage(mDatuPointEditDialog, _("Datu Point Edit"));

	CreateDockWindow(
						notebookEdit,
						/*_("Scene Objects"),*/ wxT("场景对象"),
						wxT("Object Edit"),
						hostInfo
						);

    bestSize = notebookEdit->GetBestSize();

    mObjectPropertyEditor = new ObjectPropertyEditor(this, wxID_ANY);
    CreateDockWindow(mObjectPropertyEditor,
        /*_("Object Properties")*/ wxT("物体属性"),
        wxT("物体属性编辑"),
        hostInfo);
    bestSize = CombineBestSize(bestSize, mObjectPropertyEditor->GetBestSize());

    //hostInfo.pHost->SetAreaSize(bestSize.x);
    hostInfo.pHost->SetAreaSize(128);

    //////////////////////////////////////////////////////////////////////////

    hostInfo = mLayout->GetDockHost(wxDEFAULT_RIGHT_HOST);

    wxNotebook* notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_MULTILINE|wxNB_BOTTOM);
	
    mModelSelector = new ModelSelector(notebook);
    notebook->AddPage(mModelSelector, /*_("Models")*/wxT("模型"));
    mActorSelector = new ActorSelector(notebook);
    notebook->AddPage(mActorSelector, /*_("Actors")*/wxT("对象"));
    mParticleSystemSelector = new ParticleSystemSelector(notebook);
    notebook->AddPage(mParticleSystemSelector, /*_("ParticleSystems")*/wxT("粒子系统"));
    mBrushSelector = new BrushSelector(notebook);
    notebook->AddPage(mBrushSelector, /*_("画刷")*/wxT("画刷"));

	CreateDockWindow(notebook,
        /*_("Selectors")*/wxT("选择器"),
        wxT("选择器"),
        hostInfo);
    bestSize = notebook->GetBestSize();

    mSceneInfoDlg = new SceneInfoMapDialog(this, wxID_ANY,
        wxDefaultPosition, wxDefaultSize,
        wxCLIP_CHILDREN | wxTAB_TRAVERSAL);
    CreateDockWindow(mSceneInfoDlg,
        /*_("Navigation Picture")*/wxT("导航图"),
        wxT("导航图"),
        hostInfo);
    bestSize = CombineBestSize(bestSize, mSceneInfoDlg->GetBestSize());

    //hostInfo.pHost->SetAreaSize(bestSize.x);
    hostInfo.pHost->SetAreaSize(128);

    //////////////////////////////////////////////////////////////////////////

    this->Connect(GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(FairyEditorFrame::OnUpdateWindow));

    //////////////////////////////////////////////////////////////////////////

    LoadSettings(wxConfigBase::Get(false));

    m_sceneFilename.clear();

    wxLogMessage(_("%s Startup"), wxTheApp->GetAppName().c_str());


	m_pGrowPointEditDlg->m_pNavigationDlg = mSceneInfoDlg;
	// 读取建筑物行走面的信息.
	GetBuildingCollisionInfo();
}

FairyEditorFrame::~FairyEditorFrame()
{
    delete mAutoSaveSceneController;

	if (mGpuProgramParameterSettingDialog)
	{
		mGpuProgramParameterSettingDialog->Destroy();
		delete mGpuProgramParameterSettingDialog;
		mGpuProgramParameterSettingDialog = NULL;
	}

    if (mSceneInfomationDialog)
    {
        mSceneInfomationDialog->Destroy();
        delete mSceneInfomationDialog;
        mSceneInfomationDialog = NULL;
    }

    wxLogMessage(_("%s Shutdown"), wxTheApp->GetAppName().c_str());

    SaveSettings(wxConfigBase::Get(false));

    delete mLayout;
}

void
FairyEditorFrame::LoadSettings(wxConfigBase* cfg)
{
    wxASSERT(cfg != NULL);
    if (!cfg) return;

    cfg->SetPath(wxT("Main"));
    cfg->Read(wxT("Last Scene Directory"), &m_lastSceneDirectory, wxT("../Scene"));
    cfg->Read(wxT("Last Scene File"), &m_sceneFilename, wxEmptyString);
    cfg->SetPath(wxT(".."));
}

void
FairyEditorFrame::SaveSettings(wxConfigBase* cfg) const
{
    wxASSERT(cfg != NULL);
    if (!cfg) return;

    cfg->SetPath(wxT("Main"));
    cfg->Write(wxT("Last Scene Directory"), m_lastSceneDirectory);
    cfg->Write(wxT("Last Scene File"), m_sceneFilename);
    cfg->SetPath(wxT(".."));
}

void
FairyEditorFrame::CreateDockWindow(wxWindow* dockClient, const wxString& title, const wxString& name, wxHostInfo& hostInfo)
{
    wxDockWindow *dockWindow = new wxDockWindow(this, wxID_ANY,
        title,
        wxDefaultPosition, wxDefaultSize,
        name);
    dockWindow->SetClient(dockClient);
    mLayout->AddDockWindow(dockWindow);
    mLayout->DockWindow(dockWindow, hostInfo);
}

void
FairyEditorFrame::InitMenuBar(void)
{
    //// Make a menubar
    wxMenuBar *menu_bar = new wxMenuBar;
#ifdef __WXMAC__
    wxMenuBar::MacSetCommonMenuBar(menu_bar);
#endif //def __WXMAC__
    //// Associate the menu bar with the frame
    SetMenuBar(menu_bar);

    //// Make file menu
    wxMenu *file_menu = new wxMenu;
    menu_bar->Append(file_menu, /*_("&File")*/wxT("[&F]文件"));
    file_menu->Append(wxID_NEW, wxGetStockLabel(wxID_NEW, true, wxT("Ctrl-N")));
    file_menu->Append(wxID_OPEN, wxGetStockLabel(wxID_OPEN, true, wxT("Ctrl-L")));
    file_menu->Append(wxID_SAVE, wxGetStockLabel(wxID_SAVE, true, wxT("Ctrl-S")));
    file_menu->Append(wxID_SAVEAS, wxGetStockLabel(wxID_SAVEAS));

	file_menu->AppendSeparator();

	file_menu->Append(feID_SAVETERRAININFO,_("保存场景信息\tCtrl-T"),_("把场景信息保存到文件"));
	//file_menu->Append(feID_SAVETERRAININFO,_("Save &TerrainInfo\tCtrl-T"),_("Save terrainInfo to file"));
	file_menu->Append(feID_LOADTERRAININFO,_("读取场景信息\tCtrl-R"),_("从文件读取场景信息"));
	//file_menu->Append(feID_LOADTERRAININFO,_("Load &TerrainInfo\tCtrl-R"),_("Load terrainInfo from file"));
	file_menu->AppendSeparator();

	file_menu->Append(feID_SAVEMANIPINFO,_("保存操作信息\tCtrl-M"),_("Save manipObjInfo to file"));
	//file_menu->Append(feID_SAVEMANIPINFO,_("Save &ManipObjInfo\tCtrl-M"),_("Save manipObjInfo to file"));
	file_menu->Append(feID_LOADMANIPINFO,_("读取操作信息\tCtrl-I"),_("Load manipObjInfo from file"));
	//file_menu->Append(feID_LOADMANIPINFO,_("Load &ManipObjInfo\tCtrl-I"),_("Load manipObjInfo from file"));

    file_menu->AppendSeparator();

	//file_menu->Append(feID_SAVEENVIROMENTINFO,_("Save scene enviroment infomation"),
	file_menu->Append(feID_SAVEENVIROMENTINFO,_("保存场景环境信息"),
		_("Save Save scene enviroment infomation to a file,incluing two main lights,enviroment light,fog."));
	//file_menu->Append(feID_LOADENVIROMENTINFO,_("Load scene enviroment infomation"),
	file_menu->Append(feID_LOADENVIROMENTINFO,_("读取场景环境信息"),
		_("Save Save scene enviroment infomation to a file,incluing two main lights,enviroment light,fog."));

    file_menu->AppendSeparator();

    //file_menu->Append(feID_SCENE_INFOMATION,_("Scene Infomation"),
	file_menu->Append(feID_SCENE_INFOMATION,_("场景信息"),
		_("打开场景信息对话框"));

    file_menu->AppendSeparator();

    file_menu->Append(wxID_EXIT, wxGetStockLabel(wxID_EXIT, true, wxT("Alt-X")));

    wxMenu* edit_menu = new wxMenu;
    menu_bar->Append(edit_menu, /*_("&Edit")*/_("[&E]编辑"));
    edit_menu->Append(wxID_UNDO, wxGetStockLabel(wxID_UNDO, true, wxT("Ctrl-Z")));
    edit_menu->Append(wxID_REDO, wxGetStockLabel(wxID_REDO, true, wxT("Ctrl-Y")));
    edit_menu->AppendSeparator();
    edit_menu->Append(wxID_CUT, wxGetStockLabel(wxID_CUT, true, wxT("Ctrl-X")));
    edit_menu->Append(wxID_COPY, wxGetStockLabel(wxID_COPY, true, wxT("Ctrl-C")));
    edit_menu->Append(wxID_PASTE, wxGetStockLabel(wxID_PASTE, true, wxT("Ctrl-V")));
    edit_menu->AppendSeparator();
    edit_menu->Append(feID_DELETESTATICOBJECT,
        //_("&Delete All staticobject and actor"),
        //_("Delete All staticobject and actor"));
        _("删除所有静态物体（包括actor）"),
        _("删除所有静态物体（包括actor）"));
	edit_menu->Append(feID_RESHAPE_TERRAIN,
		//_("&Reshape the Terrain"),
		//_("Reshape the Terrain "));
		_("地形扩容"),
		_("地形扩容"));
    edit_menu->AppendCheckItem(feID_ENABLE_32BIT_TEXTURES,
        //_("Enable &32 bits textures"),
        //_("Toggle 32 or 16 bits textures"));
        _("启用32位纹理"),
        _("强制使用32或16位纹理"));
    edit_menu->Append(feID_RELOAD_TEXTURES,
        //_("Reload all te&xtures"),
        //_("Unload and reload all textures"));
        _("刷新模型纹理"),
        _("重新刷新物体的纹理"));
    edit_menu->Append(feID_UNLOAD_USELESS_RESOURCES,
        //_("&Unload useless resources"),
        //_("Unload all useless resources"));
        _("卸载无用的资源"),
        _("卸载所有无用的资源"));
    edit_menu->AppendCheckItem(feID_SHOW_INDICATORS,
        //_("Show all indicators"),
        //_("Toggle showing all indicators"));
        _("显示所有指示器"),
        _("显示所有指示器"));

    wxMenu* tools_menu = new wxMenu;
    menu_bar->Append(tools_menu, _("[&T]工具"));

    tools_menu->AppendCheckItem(feID_DRAG_HEIGHT_ACTION,
        //_("&Drag Height"),
        _("[&D]拖拉调整地形高度"),
        _("Drag mouse to modify terrain height base on drag distance"));
    tools_menu->AppendCheckItem(feID_RAISE_HEIGHT_ACTION,
        /*_("&Raise Height"),*/_("[&R]提升地形高度"),
        _("Raise height of the terrain on the mouse drag area"));
    tools_menu->AppendCheckItem(feID_LOWER_HEIGHT_ACTION,
        /*_("&Lower Height"),*/_("[&L]降低地形高度"),
        _("Lower height of the terrain on the mouse drag area"));
    tools_menu->AppendCheckItem(feID_SMOOTH_HEIGHT_ACTION,
        /*_("&Smooth Height"),*/_("[&S]平滑地形高度"),
        _("Smooth height of the terrain on the mouse drag area"));
    tools_menu->AppendCheckItem(feID_LEVEL_OFF_HEIGHT_ACTION,
        /*_("Level &Off Height"),*/_("[&O]平整地形高度"),
        _("Level off height of the terrain on the mouse drag area"));
    tools_menu->AppendCheckItem(feID_SET_TERRAIN_HEIGHT_ACTION,
        /*_("Set Terrain &Height"),*/_("[H]设置地形高度"),
        _("set the height of the terrain based on the height setting"));
    tools_menu->AppendSeparator();

    tools_menu->AppendCheckItem(feID_TERRAIN_LAYER_ONE_ERASER_ACTION,
        /*_("&Erase the texture of layer one"),*/_("[&E]删除透明层贴图"),
        _("Erase the texture of layer one on the mouse drag area"));
    tools_menu->AppendSeparator();

    tools_menu->AppendCheckItem(feID_MANIP_OBJECT_ACTION,
        /*_("&Manip Object"),*/_("[&M]摆放物体"),
        /*_("Select, delete, placement objects"));*/_("选择、删除、摆放物体"));

    tools_menu->AppendCheckItem(feID_FLIP_DIAGONAL_ACTION,
        /*_("&Flip Grid Diagonal"),*/_("[&F]反转对角线"),
        _("Flip the diagonal of the selected grids"));

    tools_menu->AppendCheckItem(feID_DISABLE_DECAL_ACTION,
        _("Disable Decal"),
        _("disable decal of the selected grids"));

    tools_menu->AppendCheckItem(feID_TERRAIN_SELECTION_ACTION,
        /*_("Select &Terrain"),*/_("[&T]选择地形"),
        _("Select the grids that you want to manipulate"));

	tools_menu->AppendSeparator();
	tools_menu->Append(feID_ERASE_BY_TEXTURE_NAME,
		/*_("Erase Terrain Texture"),*/_("删除指定的地形纹理"),
		/*_("Erase Terrain Texture By Texture Name"));*/_("删除所指定的纹理"));

    tools_menu->Append(feID_ENTIRE_TERRAIN_ENABLE_DECAL,
        _("Enable decal on entire terrain"),
        _("Enable decal on entire terrain"));

	tools_menu->AppendSeparator();
	tools_menu->Append(feID_TERRAIN_LIGHTMAP_OPTION,
		/*_("Terr&ain shadowmap Option"),*/_("[&A]地形阴影图选项"),
		/*_("Update, show, hide terrain shadowmap"));*/_("更新、显示或隐藏地形阴影图"));
	tools_menu->AppendCheckItem(feID_TERRAIN_LIGHTMAP_HIDE,
		/*_("Hide shadow&map"),*/_("[&M]不显示地形阴影图"),
		/*_("Hide shadowmap"));*/_("不显示地形阴影图"));
	tools_menu->AppendCheckItem(feID_TERRAIN_LIGHTMAP_LOW_QUALITY,
		/*_("Show lo&w quality shadowmap"),*/_("显示低精度地形阴影图"),
		/*_("Show low quality shadowmap"));*/_("显示低精度地形阴影图"));
	tools_menu->AppendCheckItem(feID_TERRAIN_LIGHTMAP_HIGH_QUALITY,
		/*_("Show high &quality shadowmap"),*/_("显示高精度地形阴影图"),
		/*_("Show high quality shadowmap"));*/_("显示高精度地形阴影图"));

    tools_menu->AppendSeparator();
    tools_menu->Append(feID_TERRAIN_SURFACE_PARAMETERS,
        /*_("Terrain Surface Parameters"),*/_("地形材质参数"),
        /*_("Setting the surface parameters of terrain"));*/_("设置地形材质参数"));
    tools_menu->Append(feID_SCENE_ANIMATION_QUALITY,
        /*_("Scene animation quality..."),*/_("场景活泼级别"),
        _("Sets scene animations quality"));

    tools_menu->Append(feID_OBJECT_CREATE_LEVEL,
        /*_("Ojbect create level..."),*/_("对象创建级别"),
        _("Sets object create level"));

    tools_menu->AppendSeparator();
    tools_menu->Append(feID_CREATE_MINI_MAP,
        /*_("Create Mini Map"),*/_("创建小地图"),
        /*_("Create Mini Map"));*/_("创建小地图"));


	wxMenu* creator_menu = new wxMenu;
	menu_bar->Append(creator_menu, /*_("&Creator")*/_("创建物体"));				

	// 液体创建器菜单
	creator_menu->Append(feID_CREATE_LIQUID,
		/*_("Create &Liquid"),*/_("创建液体物体"),
		/*_("Show the liquid creator dialog"));*/_("显示液体物体创建对话框"));

	// 光源创建菜单
	creator_menu->Append(feID_CREATE_LIGHT,
		/*_("Create Light Object"),*/_("创建点光源"),
		/*	_("Create a light object"));*/_("创建点光源物体"));

    wxMenu* options_menu = new wxMenu;
    menu_bar->Append(options_menu, /*_("&Options")*/_("[&O]选项"));
    options_menu->Append(feID_PREFERSETTING,
        /*_("&Configuration..."),*/_("[&C]配置..."),
        /*_("Global sonfiguration"));*/_("选项配置"));
    options_menu->AppendCheckItem(feID_LOCK_CAMERA_HEIGHT,
        /*_("&Lock camera height"),*/_("[&L]锁定镜头高度"),
        _("Lock the camera relative height to terrain when drag move"));
	options_menu->Append(feID_RESET_CAMERA,
		/*_("Reset Camera"),*/_("重置摄像机"),
		_("Reset Camera to default height and default angle."));

	options_menu->Append(feID_GPU_PROGRAM_PARAMETER_SETTING,
		/*_("Gpu Program Parameter..."),*/_("Gpu参数编辑"),
		/*_("Gpu Program Parameter"));*/_("Gpu参数编辑"));

	// 在场景中进行物体的拣取时是选中全部物体（或者是只选中那些不能靠中心点选取的物体）
	options_menu->AppendCheckItem(feID_SELECTALLOBJECTS,
        /*_("Select all objects"),*/_("可以选择所有物体"),
        //_("If you check this, the object Select action will select all objects, otherwise only select the objects that can not center selected."));
        _("可以选择所有物体（在操作物体的状态下）"));

	options_menu->AppendSeparator();
    options_menu->AppendCheckItem(feID_ALIGN_POSITION,
        /*_("&Align to grid"),*/_("[&A]网格对齐"),
        //_("Align to terrain grid when move objects"));
        _("创建、移动物体时以地形网格为单位"));
    options_menu->AppendSeparator();
    options_menu->AppendCheckItem(feID_SHOW_STANDARD_MODEL,
        /*_("&Show the standard model"),*/_("[&S]显示参考模型"),
        _("Show the standard model when mouse move into the scene"));

    wxMenu* debug_panel_menu = new wxMenu;
    options_menu->Append(wxID_ANY, /*_("&Debug Panels")*/_("调试面板"), debug_panel_menu);
    debug_panel_menu->AppendCheckItem(feID_SHOW_STAT_PANEL,
        /*_("Show &statistics"),*/_("显示渲染统计"),
        _("Show the renderer statistics on render window"));
    debug_panel_menu->AppendCheckItem(feID_SHOW_LOGO_PANEL,
        /*_("Show lo&go"),*/_("显示 Logo"),
        _("Show the logo on render window"));

	// 画刷形状选择菜单
	wxMenu* brush_select_menu = new wxMenu;
	options_menu->Append(wxID_ANY, /*_("&Select Brush")*/_("选择画刷类型"), brush_select_menu);
	brush_select_menu->AppendCheckItem(feID_BOX_BRUSH,
		/*_("Box Brush"),*/_("方形画刷"),
		_("Select the box shape brush"));
	brush_select_menu->AppendCheckItem(feID_ELLIPSE_BRUSH,
		/*_("Ellipse Brush"),*/_("椭圆形画刷"),
		_("Select the ellipse shape brush"));

//*/
    wxMenu* shadowtype_menu = new wxMenu;
    options_menu->Append(wxID_ANY, /*_("&Dynamic shadow technique")*/_("动力学阴影技术"), shadowtype_menu);
/*/
    wxMenu* shadowtype_menu = options_menu;
    options_menu->AppendSeparator();
//*/
    shadowtype_menu->AppendRadioItem(feID_SHADOWTYPE_NONE,
        /*_("None"),*/_("关闭"),
        _("Disable dynamic shadows"));
    shadowtype_menu->AppendRadioItem(feID_SHADOWTYPE_TEXTURE_ADDITIVE,
        /*_("Texture additive"),*/_("材质叠加"),
        _("Use texture additive dynamic shadows"));
    shadowtype_menu->AppendRadioItem(feID_SHADOWTYPE_TEXTURE_MODULATIVE,
        /*_("Texture modulative"),*/_("材质方向调整"),
        _("Use texture modulative dynamic shadows"));
    shadowtype_menu->AppendRadioItem(feID_SHADOWTYPE_STENCIL_ADDITIVE,
        /*_("Stencil additive"),*/_("模板叠加"),
        _("Use stencil additive dynamic shadows"));
    shadowtype_menu->AppendRadioItem(feID_SHADOWTYPE_STENCIL_MODULATIVE,
        /*_("Stencil modulative"),*/_("模板方向调整"),
        _("Use stencil modulative dynamic shadows"));

//*/
    wxMenu* tfo_menu = new wxMenu;
    options_menu->Append(wxID_ANY, _("纹理过滤"), tfo_menu);
/*/
    wxMenu* tfo_menu = options_menu;
    options_menu->AppendSeparator();
//*/
    tfo_menu->AppendRadioItem(feID_TFO_NONE,
        _("关闭过滤"),
        _("Disable texture filtering"));
    tfo_menu->AppendRadioItem(feID_TFO_BILINEAR,
        _("双线性过滤"),
        _("Use bi-linear texture filtering"));
    tfo_menu->AppendRadioItem(feID_TFO_TRILINEAR,
        _("三线性过滤"),
        _("Use tri-linear texture filtering"));
    tfo_menu->AppendRadioItem(feID_TFO_ANISOTROPIC,
        _("各向异性过滤"),
        _("Use anisotropic texture filtering"));

    wxMenu* clo_menu = new wxMenu;
    options_menu->Append(wxID_ANY, _("场景创建级别"), clo_menu);
    /*/
    wxMenu* tfo_menu = options_menu;
    options_menu->AppendSeparator();
    //*/
    clo_menu->AppendRadioItem(feID_CL_LOW,
        _("低"),
        _("Use Low Level"));
    clo_menu->AppendRadioItem(feID_CL_MID,
        _("中"),
        _("Use Mid Level"));
    clo_menu->AppendRadioItem(feID_CL_HIGH,
        _("高"),
        _("Use High Level"));

/*/
    wxMenu* post_filter_menu = new wxMenu;
    options_menu->Append(wxID_ANY, _("&Post Filter"), post_filter_menu);
/*/
    wxMenu* post_filter_menu = options_menu;
    options_menu->AppendSeparator();
//*/
    post_filter_menu->AppendCheckItem(feID_PF_HEATVISION,
        _("夜视镜效果") + wxString(wxT("\tCtrl-1")),
        _("Use heat vision post filter"));
    post_filter_menu->AppendCheckItem(feID_PF_BLOOM,
        _("晕光效果") + wxString(wxT("\tCtrl-2")),
        _("Use bloom post filter"));
    post_filter_menu->AppendCheckItem(feID_PF_FLOODLIGHTING,
        _("全局泛光效果") + wxString(wxT("\tCtrl-3")),
        _("Use floodlighting post filter"));
    post_filter_menu->AppendCheckItem(feID_PF_FADE,
        _("死亡效果") + wxString(wxT("\tCtrl-4")),
        _("Use fade post filter"));
    post_filter_menu->AppendCheckItem(feID_PF_MOTION_BLUR,
        _("拖影效果") + wxString(wxT("\tCtrl-5")),
        _("Use motion blur filter"));

    wxMenu* window_menu = new wxMenu;
    menu_bar->Append(window_menu, _("[&W]窗口"));
    window_menu->AppendCheckItem(feID_FULL_SCREEN,
        _("[&F]全屏显示\tCtrl-Enter"),
        _("切换全屏显示模式"));
    window_menu->Append(feID_TOGGLE_TOOL_WINDOWS,
        wxEmptyString);   // Will setup the correct display text on UI update
    window_menu->AppendSeparator();

    mWindowMenu = window_menu;
}

void
FairyEditorFrame::InitToolBar(wxToolBar* toolbar)
{
    toolbar->AddCheckTool(feID_LOCK_BRUSH_SIZE, _("关联画刷大小"),
        wxBITMAP(locker), wxNullBitmap, _("关联画刷大小"));

	wxControl *control = NULL;

	control = new wxSpinCtrl(toolbar, feID_BRUSH_SIZE_X,
		wxString::Format(wxT("%d"), 3),
		wxDefaultPosition, wxSize(40, -1),
		wxSP_ARROW_KEYS,
		1, 9, 3);
	control->SetToolTip(_("X方向画刷大小"));
    toolbar->AddControl(control);

	control = new wxSpinCtrl(toolbar, feID_BRUSH_SIZE_Z,
		wxString::Format(wxT("%d"), 3),
		wxDefaultPosition, wxSize(40, -1),
		wxSP_ARROW_KEYS,
		1, 9, 3);
	control->SetToolTip(_("Z方向画刷大小"));
    toolbar->AddControl(control);

	control = new wxSpinCtrl(toolbar, feID_TERRAIN_HEIGHT,
		wxString::Format(wxT("%d"), 1000),
		wxDefaultPosition, wxSize(60, -1),
		wxSP_ARROW_KEYS,
		-99999, 99999, 1000);
	control->SetToolTip(_("设置地形的高度"));
    toolbar->AddControl(control);

	control = new wxSpinCtrl(toolbar, feID_TERRAIN_HEIGHT_INCREMENT,
		wxString::Format(wxT("%d"), 100),
		wxDefaultPosition, wxSize(60, -1),
		wxSP_ARROW_KEYS,
		-999, 999, 100);
	control->SetToolTip(_("设置地形高度递增值"));
	toolbar->AddControl(control);

	// 以下为改变地形高度的权重所用的ui
	toolbar->AddSeparator();

	control = new wxSpinCtrl(toolbar, feID_HEIGHT_ACTION_AFFECT_RANGE,
		wxString::Format(wxT("%d"), 2),
		wxDefaultPosition, wxSize(40, -1),
		wxSP_ARROW_KEYS,
		0, 10, 2);
	control->SetToolTip(_("设置地形画刷的额外影响范围"));
    toolbar->AddControl(control);

	control = new wxTextCtrl(toolbar, feID_HEIGHT_ACTION_EXP_VALUE,
		wxString::Format(wxT("%f"), 0.5),
		wxDefaultPosition, wxSize(40, -1), wxTE_PROCESS_ENTER);
	control->SetToolTip(_("设置地形画刷额外范围的Exp值（范围越远，影响越小）"));
	toolbar->AddControl(control);

	control = new wxTextCtrl(toolbar, feID_HEIGHT_ADJUST_SPEED,
		wxString::Format(wxT("%f"), 3.0),
		wxDefaultPosition, wxSize(30, -1), wxTE_PROCESS_ENTER);
	control->SetToolTip(_("设置地形改变的速度"));
	toolbar->AddControl(control);

	// hack，设置泛光参数
	toolbar->AddSeparator();

	toolbar->AddControl(new wxStaticText(toolbar, feID_BLURAMOUNTSTATICTEXT,
		"羽化",	wxDefaultPosition));

	toolbar->AddControl(new wxTextCtrl(toolbar, feID_BLURAMOUNT,
		wxString::Format(wxT("%f"), 0.2),
		wxDefaultPosition, wxSize(40, -1), wxTE_PROCESS_ENTER));

	toolbar->AddControl(new wxStaticText(toolbar, feID_SHINEAMOUNTSTATICTEXT,
		"光亮",	wxDefaultPosition));

	toolbar->AddControl(new wxTextCtrl(toolbar, feID_SHINEAMOUNT,
		wxString::Format(wxT("%f"), 0.3),
		wxDefaultPosition, wxSize(40, -1), wxTE_PROCESS_ENTER));


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//
	// 显示鼠标作标(游戏中的逻辑坐标)
	//

	toolbar->AddSeparator();

	toolbar->AddControl(new wxStaticText(toolbar, feID_STATIC_SHOW_X,
		"当前X",	wxDefaultPosition));

	toolbar->AddControl(new wxTextCtrl(toolbar, feID_SHOW_X,
		wxString::Format(wxT("%d"), 0),
		wxDefaultPosition, wxSize(60, -1), wxTE_PROCESS_ENTER));

	toolbar->AddControl(new wxStaticText(toolbar, feID_STATIC_SHOW_Y,
		"当前Z",	wxDefaultPosition));

	toolbar->AddControl(new wxTextCtrl(toolbar, feID_SHOW_Y,
		wxString::Format(wxT("%d"), 0),
		wxDefaultPosition, wxSize(60, -1), wxTE_PROCESS_ENTER));

	
	toolbar->AddControl(new wxTextCtrl(toolbar, feID_GOTO_X,
		wxString::Format(wxT("%d"), 0),
		wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER));
	toolbar->AddControl(new wxTextCtrl(toolbar, feID_GOTO_Y,
		wxString::Format(wxT("%d"), 0),
		wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER));


	// 按钮
	toolbar->AddControl(new wxButton(toolbar, feID_GOTO_BN, _("跳转"), wxDefaultPosition, wxSize(30, -1), wxTE_PROCESS_ENTER));
		

	//
	//
	// 显示鼠标作标(游戏中的逻辑坐标)
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	



	toolbar->AddCheckTool(feID_USE_REAL_CAMERA, _("使用真实游戏镜头"),
		wxBITMAP(USEREALCAMERA), wxNullBitmap, _("使用真实游戏镜头"));

	toolbar->AddCheckTool(feID_USE_SNAP_ROTATION, _("使用整角度旋转"),
		wxBITMAP(USESNAPROTATE), wxNullBitmap, _("使用整角度旋转"));

	control = new wxTextCtrl(toolbar, feID_FRAMERATE,
		wxString::Format(wxT("%d"), 0),
		wxDefaultPosition, wxSize(40, -1), wxTE_PROCESS_ENTER);
	control->SetToolTip(_("帧速率"));
	toolbar->AddControl(control);

    // TODO
    toolbar->Realize();
}

// 设置当前的场景逻辑坐标
void FairyEditorFrame::SetCurSceneLogicPos(float fx, float fz)
{
	if( !m_pToolbar ) return;

    wxTextCtrl* pTexX = (wxTextCtrl*)m_pToolbar->FindControl(feID_SHOW_X);
    if (pTexX)
    {
        pTexX->SetLabel(wxString::Format("%.2f", fx));
    }

    wxTextCtrl* pTexY = (wxTextCtrl*)m_pToolbar->FindControl(feID_SHOW_Y);
    if (pTexX)
    {
        pTexY->SetLabel(wxString::Format("%.2f", fz));
    }
}


void
FairyEditorFrame::InitStatusBar(wxStatusBar* statusbar)
{
    // TODO
}

//////////////////////////////////////////////////////////////////////////

WX::SceneManipulator*
FairyEditorFrame::GetSceneManipulator(void) const
{
    return mCanvas ? mCanvas->GetSceneManipulator() : NULL;
}

void
FairyEditorFrame::InitScene(WX::SceneManipulator* sceneManipulator)
{
    wxASSERT(mCanvas);

	assert (sceneManipulator);

	mCanvas->SetParentFrame(this);
    mCanvas->InitScene(sceneManipulator);
    mBrushSelector->SetSceneManipulator(sceneManipulator);
    mBrushSelector->ReloadTextureList();
    mModelSelector->SetSceneManipulator(sceneManipulator);
    mModelSelector->Reload();
    mActorSelector->SetSceneManipulator(sceneManipulator);
    mActorSelector->Reload();
    mParticleSystemSelector->SetSceneManipulator(sceneManipulator);
    mParticleSystemSelector->Reload();
    mSceneBrowser->SetSceneManipulator(sceneManipulator);
    mObjectPropertyEditor->SetSceneManipulator(sceneManipulator);
    mSceneInfoDlg->SetSceneManipulator(sceneManipulator);
    //mCameraTrackSettingDialog->SetSceneManipulator(sceneManipulator);

	// 给区域编辑
	if (m_pRegionEditSelector)
	{
		m_pRegionEditSelector->SetSceneManipulator(sceneManipulator);
		g_pRegionEdit = m_pRegionEditSelector;
	}

	// 怪物编辑
	if (m_pMonsterEditDlg)
	{
		m_pMonsterEditDlg->SetSceneManipulator(sceneManipulator);
	}

	// 生长点编辑
	if (m_pGrowPointEditDlg)
	{
		m_pGrowPointEditDlg->SetSceneManipulator(sceneManipulator);
	}

	// 区域事件
	if (m_pEventAreaEditDlg)
	{
		m_pEventAreaEditDlg->SetSceneManipulator(sceneManipulator);
	}

	// npc编辑
	if (m_pNpcPatrolEditDlg)
	{
		m_pNpcPatrolEditDlg->SetSceneManipulator(sceneManipulator);
	}

	// 摊位编辑
	if (m_pStallInfoDlg)

	{
		m_pStallInfoDlg->SetSceneManipulator(sceneManipulator);
	}


	//　声音编辑
	if(mSoundEditDialog)
	{
		mSoundEditDialog->SetSceneManipulator(sceneManipulator);
	}

	if( m_pPathFindEditDlg )
	{
		m_pPathFindEditDlg->SetSceneManipulator(sceneManipulator);
	}

    // 打图点
    if(mDatuPointEditDialog)
    {
        mDatuPointEditDialog->SetSceneManipulator(sceneManipulator);
    }

    mAutoSaveSceneController = new AutoSaveSceneController(this, 300.0f);

}

bool
FairyEditorFrame::Save(void)
{
	// 如果当前的action是画刷，就先给出提示，因为现在如果在画刷action下保存会因为pixmap重组的原因而产生问题
	/*if (NULL != GetSceneManipulator()->getActiveAction())
	{
		Ogre::String actionType = GetSceneManipulator()->getActiveAction()->getName();
		if (actionType == "SimplePaintAction" || actionType == "AutoTexPaintAction")
		{
			wxMessageBox(_("It is not safe to save the scene when the active action is paint action!"));
			return false;
		}
	}*/

    if (m_sceneFilename.empty())
    {
        return SaveAs();
    }
    else
    {
        // 存储不可行走区域。
        SaveRegion();

        // 存储建筑物行走面。
        SaveBuildingWalk();

        // 存储npc.
        SaveNpc();

        // 存储生长点
        SaveGrowPoint();

        // 存储npc巡逻路线
        SaveNpcPatrol();

		// 存储寻路点
		SavePathFind();

		// 存储事件区域
        SaveEventArea();

        // 存储摊位信息
        SaveStallInfo();

		// 存储摊位信息
		SaveStallInfo();

		// 存储声音
		SaveSoundInfo();

        SaveDatuPointInfo();
        // 保存场景编辑所需的额外信息
        GetSceneManipulator()->getSceneEditingData().saveData(AS_STRING(m_sceneFilename));

		mIsTerrainLightmapUpdated = false;

        return SaveImpl();

    }
}

bool
FairyEditorFrame::SaveAs(void)
{
	// 如果当前的action是画刷，就先给出提示，因为现在如果在画刷action下保存会因为pixmap重组的原因而产生问题
	/*if (NULL != GetSceneManipulator()->getActiveAction())
	{
		Ogre::String actionType = GetSceneManipulator()->getActiveAction()->getName();
		if (actionType == "SimplePaintAction" || actionType == "AutoTexPaintAction")
		{
			wxMessageBox(_("It is not safe to save the scene when the active action is paint action!"));
			return false;
		}
	}*/

    wxFileDialog fileDialog(this,
        _("保存场景"),
        m_lastSceneDirectory,
        m_sceneFilename,
        wxString::Format(
            _("场景文件 (*.Scene)|*.Scene|场景 XML 文件 (*.Scene.xml)|*.Scene.xml|全部文件 (%s)|%s"),
            wxFileSelectorDefaultWildcardStr, wxFileSelectorDefaultWildcardStr),
        wxSAVE | wxOVERWRITE_PROMPT);

    if (fileDialog.ShowModal() != wxID_OK)
        return false;

    m_sceneFilename = fileDialog.GetPath();
    m_lastSceneDirectory = wxPathOnly(m_sceneFilename);

    // 存储不可行走区域。
    SaveRegion();

	// 存储寻路点
	SavePathFind();

    // 存储建筑物行走面。
    SaveBuildingWalk();

    // 存储npc.
    SaveNpc();

    // 存储生长点
    SaveGrowPoint();

    // 存储npc巡逻路线
    SaveNpcPatrol();

    // 存储事件区域
    SaveEventArea();

    // 存储摊位信息
    SaveStallInfo();

    // 存储声音
    SaveSoundInfo();

    SaveDatuPointInfo();

    return SaveImpl();
}

bool 
FairyEditorFrame::SaveImpl(bool autoSave)
{
    if (autoSave)
    {
        if (!m_sceneFilename.empty())
        {
            Ogre::String baseName, path;
            Ogre::StringUtil::splitFilename(AS_STRING(m_sceneFilename), baseName, path);

            GetSceneManipulator()->saveScene(path + "AutoSave_" + baseName);    

            // 保存场景编辑所需的额外信息
            WX::SceneEditingData& sed = GetSceneManipulator()->getSceneEditingData();

            if (sed.isDataChanged())
            {
                sed.saveData(path + "AutoSave_" + baseName);
            }

			// 保存场景编辑所需的额外信息
			GetSceneManipulator()->getSceneEditingData().saveData(AS_STRING(m_sceneFilename));
            mIsTerrainLightmapUpdated = false;
        }
    }
    else
    {
        GetSceneManipulator()->saveScene(AS_STRING(m_sceneFilename));    


        // 保存场景编辑所需的额外信息
        WX::SceneEditingData& sed = GetSceneManipulator()->getSceneEditingData();

        if (sed.isDataChanged())
        {
            sed.saveData(AS_STRING(m_sceneFilename));
        }

        mIsTerrainLightmapUpdated = false;
    }

    mAutoSaveSceneController->reset();

    return true;
}

bool
FairyEditorFrame::SaveModified(bool force)
{
    if ((!GetSceneManipulator() || !GetSceneManipulator()->isModified()) && !mIsTerrainLightmapUpdated)
        return +1;

    wxString name;
    if (!m_sceneFilename.empty())
        wxFileName::SplitPath(m_sceneFilename, NULL, &name, NULL);
    else
        name = _("未命名");

    wxString msgTitle;
    if (!wxTheApp->GetAppName().empty())
        msgTitle = wxTheApp->GetAppName();
    else
        msgTitle = /*_("Warning")*/wxT("警告");

    wxString prompt;
    prompt.Printf(_("你想保存对文档%s的修改?"), name.c_str());
    int res = wxMessageBox(prompt, msgTitle,
        force ? wxYES_NO|wxICON_QUESTION : wxYES_NO|wxCANCEL|wxICON_QUESTION,
        this);
    switch (res)
    {
    default:
    case wxCANCEL:
        return false;
    case wxNO:		
        return true;
    case wxYES:		
        return Save();
    }
}

// Extend event processing to search the canvas's event table
bool
FairyEditorFrame::ProcessEvent(wxEvent& e)
{
//*/ 
    
    return wxFrame::ProcessEvent(e);
/*/
    static wxEvent *activeEvent = NULL;

    // Break recursion loops
    if (activeEvent == &e)
        return false;

    activeEvent = &e;

    bool ret;
    // Only hand down to the canvas if it's a menu command
    if (!e.IsKindOf(CLASSINFO(wxCommandEvent)) || !mCanvas || !mCanvas->ProcessEvent(e))
        ret = wxFrame::ProcessEvent(e);
    else
        ret = true;

    activeEvent = NULL;
    return ret;
//*/
}

void
FairyEditorFrame::OnExit(wxCommandEvent& e)
{
    Close();
}

void
FairyEditorFrame::OnUpdateWindow(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);

    if (GetSceneManipulator())
    {
        wxString name;
        if (!m_sceneFilename.empty())
            wxFileName::SplitPath(m_sceneFilename, NULL, &name, NULL);
        else
            name = _("未命名");

        wxString modified;
        if (GetSceneManipulator()->isModified() || mIsTerrainLightmapUpdated)
            modified = _("*");

        wxString title = wxTheApp->GetAppName() + _(" - ") + name + modified;
        if (m_lastTitle != title)
        {
            m_lastTitle = title;
            e.SetText(title);
        }
    }
}

void
FairyEditorFrame::OnCloseWindow(wxCloseEvent& e)
{
    if (SaveModified(!e.CanVeto()))
        wxFrame::OnCloseWindow(e);
    else
        e.Veto();
}

void
FairyEditorFrame::OnFileNew(wxCommandEvent& e)
{
    if (!SaveModified())
        return;	

    NewSceneDialog dlg(this, wxID_ANY, _("新场景"));
    dlg.Centre();
    if (dlg.ShowModal() == wxID_OK)
    {
        int xsize,zsize,seed,scale,tileSize,height;
        Ogre::String alogrithm;
        
        alogrithm = dlg.mAlogrithmCtrl->GetLabel();
        xsize = atoi(dlg.mXsizeTextCtrl->GetLabel());
        zsize = atoi(dlg.mZsizeTextCtrl->GetLabel());
        Ogre::String baseTexture  = Ogre::String(dlg.mCmbTexture->GetLabel().GetData());
        seed = atoi(dlg.mSeedTextCtrl->GetLabel());
        scale = atoi(dlg.mScaleTextCtrl->GetLabel());
        height = atoi(dlg.mHeightScaleTextCtrl->GetLabel());
        tileSize = atoi(dlg.mTileSizeTextCtrl->GetLabel());

        wxBusyInfo busyInfo(_("正在生成场景，请等待..."), this);

        NewSceneDialog::PathNameMap::const_iterator iter = dlg.pathNameMap.find(baseTexture);
        if (iter!= (dlg.pathNameMap.end()))
        {
            GetSceneManipulator()->newScene(alogrithm,xsize,zsize,tileSize,iter->second,seed,scale,height);
        }
        else
        {
            GetSceneManipulator()->newScene(alogrithm,xsize,zsize,tileSize,baseTexture,seed);
        }
        m_sceneFilename.clear();

		//// 不可走区域清空.
		//m_pRegionEditSelector->ClearData();
		ClearRegion();

		// 清空npc
		ClearNpc();

		// 清空生长点
		ClearGrowPoint();

		// 清空npc巡逻
		ClearNpcPatrol();

		// 清空寻路点
		ClearPathFind();

		// 清空事件区域
		ClearEventArea();

		// 创建摊位
		CreateStallInfo();

		// 清空声音
		ClearSoundInfo();

        ClearDatuPointInfo();

		mIsTerrainLightmapUpdated = false;
    }

    mAutoSaveSceneController->reset();
}

void
FairyEditorFrame::OnUpdateFileNew(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
}

void
FairyEditorFrame::OnReshapeTerrain(wxCommandEvent& e)
{
    // 终止当前操作
    GetSceneManipulator()->setActiveAction(NULL);

	ReshapeDialog dlg(this, wxID_ANY, _("地形扩容"));
	dlg.Centre();
	if (dlg.ShowModal() == wxID_OK)
	{
		int left,top,right,bottom;
        int rLeft, rRight, rTop, rBottom;
		int pixmap;
		wxString textureName;

		textureName = dlg.mCmbTexture->GetLabel();
		left = atoi(dlg.mLeftCtrl->GetLabel());
		top = atoi(dlg.mTopCtrl->GetLabel());	
		right = atoi(dlg.mRightCtrl->GetLabel());
		bottom = atoi(dlg.mBottomCtrl->GetLabel());

        WX::TerrainData* terrainData = GetSceneManipulator()->getTerrainData();
        assert (terrainData);

        int xSize = terrainData->getXSize();
        int zSize = terrainData->getZSize();

        /**
        对话框的填写规则为：如果想缩小，就填入缩小的格数的负数，如想缩小4格就填-4
        如果想扩大地形，就填入想扩大的格数，如想扩大4格就填4
        所以这里要做相应的转换，把输入的数字转换成_reshape想要的参数
        */
        rLeft = -left;
        rRight = xSize + right;
        rTop = -top;
        rBottom = zSize + bottom;

        if ((rLeft >= rRight) || (rTop >= rBottom))
        {
            wxMessageBox(_("错误的输入!"));
            return;
        }
	
		NewSceneDialog::PathNameMap::const_iterator iter = dlg.pathNameMap.find(Ogre::String(textureName));
		if (iter!= (dlg.pathNameMap.end()))
		{
			pixmap = GetSceneManipulator()->getTerrainData()->_registerPixmap(iter->second,0,0,1,1);
		}
		else
		{
			pixmap = GetSceneManipulator()->getTerrainData()->_registerPixmap(Ogre::String(textureName),0,0,1,1);
		}
		
		wxBusyInfo busyInfo(_("地形扩容 ..."), this);        
        
		terrainData->_reshape(rLeft,rTop,rRight,rBottom,pixmap);

        // 写入reshape的size
        GetSceneManipulator()->getSceneEditingData().setTerrainReshapeSize(left, right, top, bottom);

		GetSceneManipulator()->getTerrain()->buildGeometry(GetSceneManipulator()->getBaseSceneNode(), GetSceneManipulator()->isEditable());
		GetSceneManipulator()->_fireSceneReset();
	}
}

void
FairyEditorFrame::OnUpdateReshapeTerrain(wxUpdateUIEvent& e)
{
	e.Enable(GetSceneManipulator() != 0);
}

void
FairyEditorFrame::OnEnable32BitTextures(wxCommandEvent& e)
{
    Ogre::TextureManager *tm = Ogre::TextureManager::getSingletonPtr();
    if (tm)
    {
        tm->setPreferredIntegerBitDepth(tm->getPreferredIntegerBitDepth() == 16 ? 0 : 16);
    }
}

void
FairyEditorFrame::OnUpdateEnable32BitTextures(wxUpdateUIEvent& e)
{
    Ogre::TextureManager *tm = Ogre::TextureManager::getSingletonPtr();
	e.Enable(tm != 0);
    e.Check(tm && tm->getPreferredIntegerBitDepth() != 16);
}

void
FairyEditorFrame::OnReloadTextures(wxCommandEvent& e)
{
    if (Ogre::TextureManager::getSingletonPtr())
    {
        // Make sure doesn't unload render texture, which is unnecessary,
        // and will cause issues in OGL render system.
        // Now we trust that all render texture just create as manually without loader,
        // in other words, it doesn't reloadable.
        Ogre::TextureManager::getSingleton().unloadAll(true);
        // The texture will reload on demand
    }
}

void
FairyEditorFrame::OnUpdateReloadTextures(wxUpdateUIEvent& e)
{
	e.Enable(GetSceneManipulator() != 0);
}

static void
removeUselessResources(Ogre::ResourceManager& rm)
{
    static const unsigned int usReferenceCountByResourceManager = 3;

    // Collect useless resources first because we can't remove it while
    // iterate through the resource map
    std::list<Ogre::ResourceHandle> uselessResources;

    Ogre::ResourceManager::ResourceMapIterator it = rm.getResourceIterator();
    while (it.hasMoreElements())
    {
        Ogre::ResourceHandle handle = it.peekNextKey();
        Ogre::ResourcePtr res = it.getNext();
        if (!res->isManuallyLoaded() &&
            res.useCount() <= usReferenceCountByResourceManager + 1)
        {
            uselessResources.push_back(handle);
        }
    }

    // Now remove useless resources
    std::list<Ogre::ResourceHandle>::const_iterator it2;
    for (it2 = uselessResources.begin(); it2 != uselessResources.end(); ++it2)
    {
        Ogre::ResourceHandle handle = *it2;
        rm.remove(handle);
    }
}

void
FairyEditorFrame::OnUnloadUselessResources(wxCommandEvent& e)
{
    Ogre::ResourceGroupManager* rgm = Ogre::ResourceGroupManager::getSingletonPtr();
    if (rgm)
    {
        static char const * const unloadableResourceManagerNames[] =
        {
            "Font",
            "GpuProgram",
            "HighLevelGpuProgram",
            "Material",
            0,
        };

        static char const * const removableResourceManagerNames[] =
        {
            "Mesh",
            "Skeleton",
            // Texture can't remove because has exists weak reference to it,
            // comment out until we fixed all weak reference to textures.
            //"Texture",
            0,
        };

        char const * const * p;

        for (p = unloadableResourceManagerNames; *p; ++p)
        {
            rgm->_getResourceManager(*p)->unloadUnreferencedResources();
        }

        for (p = removableResourceManagerNames; *p; ++p)
        {
            removeUselessResources(*rgm->_getResourceManager(*p));
        }
    }
}

void
FairyEditorFrame::OnUpdateUnloadUselessResources(wxUpdateUIEvent& e)
{
	e.Enable(GetSceneManipulator() != 0);
}

void
FairyEditorFrame::OnShowAllIndicators(wxCommandEvent& e)
{
    WX::SceneManipulator* manipulator = GetSceneManipulator();

    if (manipulator)
    {
        manipulator->setIndicatorNodeVisible(!manipulator->getIndicatorNodeVisible());
    }    
}

void
FairyEditorFrame::OnUpdateShowAllIndicators(wxUpdateUIEvent& e)
{
    WX::SceneManipulator* manipulator = GetSceneManipulator();

    e.Enable(manipulator != 0);
    e.Check(manipulator && manipulator->getIndicatorNodeVisible());    
}

void
FairyEditorFrame::OnUpdateSave(wxUpdateUIEvent& e)
{
    e.Enable((GetSceneManipulator() && GetSceneManipulator()->isModified()) || mIsTerrainLightmapUpdated);
}

void
FairyEditorFrame::OnUpdateSaveAs(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() && !GetSceneManipulator()->isEmpty());
}

void
FairyEditorFrame::OnFileOpen(wxCommandEvent& e)
{
    if (!SaveModified())
        return;

    wxFileDialog fileDialog(this,
        _("装载场景"),
        m_lastSceneDirectory,
        m_sceneFilename,
        wxString::Format(
            _("Scene files (*.Scene)|*.Scene|Scene XML files (*.Scene.xml)|*.Scene.xml|All files (%s)|%s"),
            wxFileSelectorDefaultWildcardStr, wxFileSelectorDefaultWildcardStr),
        wxOPEN | wxFILE_MUST_EXIST);

    if (fileDialog.ShowModal() == wxID_OK)
    {
        m_sceneFilename = fileDialog.GetPath();
        m_lastSceneDirectory = wxPathOnly(m_sceneFilename);

        if (mLogWindow)
            mLogWindow->Clear();

        wxBusyInfo busyInfo(_("正在装载场景，请等待..."), this);
      
		GetSceneManipulator()->loadScene(AS_STRING(m_sceneFilename));      

		if (GetSceneManipulator()->hasExceptionWhenLoadScene())
		{
			ExceptionInfoDialog dlg(this, wxID_ANY,
                _("异常信息对话框"), GetSceneManipulator());
		
			if (dlg.ShowModal() != wxID_OK)
			{
				// 如果异常列表中仍有内容，就给出提示
				if (dlg.mListBox->GetCount() > 0)
				{
					wxMessageBox(_("场景文件中仍存在一些错误，请修复！"));
				}
				else
				{
					wxMessageBox(_("场景中的所有错误已修正，请重新保存场景使该改动生效！"));
				}
			}
		}

		char szDebugInfo[200];
		DWORD dwTickCount = GetTickCount();
		// 读取不可行走区域
		ReadRegion();
		dwTickCount = GetTickCount() - dwTickCount;
		sprintf( szDebugInfo, "ReadRegion cost %d sec.\n", dwTickCount );
		OutputDebugString( szDebugInfo );
		
		// 读取生长点
		dwTickCount = GetTickCount();
		ReadGrowPoint();
		dwTickCount = GetTickCount() - dwTickCount;
		sprintf( szDebugInfo, "ReadGrowPoint cost %d sec.\n", dwTickCount );
		OutputDebugString( szDebugInfo );

		// 读取npc
		dwTickCount = GetTickCount();
		ReadNpc();
		dwTickCount = GetTickCount() - dwTickCount;
		sprintf( szDebugInfo, "ReadNpc cost %d sec.\n", dwTickCount );
		OutputDebugString( szDebugInfo );

		// 读取npc巡逻
		dwTickCount = GetTickCount();
		ReadNpcPatrol();
		dwTickCount = GetTickCount() - dwTickCount;
		sprintf( szDebugInfo, "ReadNpcPatrol cost %d sec.\n", dwTickCount );
		OutputDebugString( szDebugInfo );

		//寻路编辑
		dwTickCount = GetTickCount();
		ReadPathFind();
		dwTickCount = GetTickCount() - dwTickCount;
		sprintf( szDebugInfo, "ReadPathFind cost %d sec.\n", dwTickCount );
		OutputDebugString( szDebugInfo );

		// 读取事件区域
		dwTickCount = GetTickCount();
		ReadEventArea();
		dwTickCount = GetTickCount() - dwTickCount;
		sprintf( szDebugInfo, "ReadEventArea cost %d sec.\n", dwTickCount );
		OutputDebugString( szDebugInfo );

		// 读取摊位信息
		dwTickCount = GetTickCount();
		ReadStallInfo();
		dwTickCount = GetTickCount() - dwTickCount;
		sprintf( szDebugInfo, "ReadStallInfo cost %d sec.\n", dwTickCount );
		OutputDebugString( szDebugInfo );

		// 读取声音信息
		dwTickCount = GetTickCount();
		ReadSoundInfo();
		dwTickCount = GetTickCount() - dwTickCount;
		sprintf( szDebugInfo, "ReadSoundInfo cost %d sec.\n", dwTickCount );
		OutputDebugString( szDebugInfo );

        ReadDatuPointInfo();

        // 读取场景编辑所需的额外信息
		dwTickCount = GetTickCount();
        GetSceneManipulator()->getSceneEditingData().readData(AS_STRING(m_sceneFilename));
		dwTickCount = GetTickCount() - dwTickCount;
		sprintf( szDebugInfo, "ReadOtherInfo cost %d sec.\n", dwTickCount );
		OutputDebugString( szDebugInfo );

    }

    mAutoSaveSceneController->reset();
}

void
FairyEditorFrame::OnFileSave(wxCommandEvent& e)
{
    Save();
}

void
FairyEditorFrame::OnFileSaveAs(wxCommandEvent& e)
{
    SaveAs();
}

void
FairyEditorFrame::OnUndo(wxCommandEvent& e)
{
    GetSceneManipulator()->getOperatorManager()->undo();
}

void
FairyEditorFrame::OnRedo(wxCommandEvent& e)
{
    GetSceneManipulator()->getOperatorManager()->redo();
}

void
FairyEditorFrame::OnUpdateUndo(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() &&
        !GetSceneManipulator()->getOperatorManager()->getUndoOperators().empty());
}

void
FairyEditorFrame::OnUpdateRedo(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() &&
        !GetSceneManipulator()->getOperatorManager()->getRedoOperators().empty());
}

void
FairyEditorFrame::OnLockCameraHeight(wxCommandEvent& e)
{
    GetSceneManipulator()->setCameraHeightLocked(!GetSceneManipulator()->getCameraHeightLocked());
}

void
FairyEditorFrame::OnUpdateLockCameraHeight(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
    e.Check(GetSceneManipulator() && GetSceneManipulator()->getCameraHeightLocked());
}

void 
FairyEditorFrame::OnShowStandardModel(wxCommandEvent& e)
{
    GetSceneManipulator()->setShowStandardModel(!GetSceneManipulator()->getShowStandardModel());
}

void 
FairyEditorFrame::OnDeleteStaticObject(wxCommandEvent& e)
{
    int answer = wxMessageBox(_("不能删除键 '%s'"), _("确认"),
        wxYES_NO | wxCANCEL, this);
    if (answer == wxYES)
    {    
        const WX::Scene::Objects& mObjects = GetSceneManipulator()->getSceneInfo()->getObjects();
        std::vector<WX::ObjectPtr> objVec;
        for (WX::Scene::Objects::const_iterator it = mObjects.begin(); it != mObjects.end(); ++it)
        {
            const WX::ObjectPtr& object = *it;

            if (object->getCategory() == "StaticObject" || object->getCategory() == "Actor")
            {
                objVec.push_back(object);
            }
        }

        for(unsigned int i=0; i<objVec.size();i++)
        {
            GetSceneManipulator()->getSceneInfo()->removeObject(objVec[i]);
        }
        GetSceneManipulator()->_fireSceneReset();
    }
}

void 
FairyEditorFrame::OnUpdateDeleteStaticObject(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() !=0);
}

void
FairyEditorFrame::OnUpdateShowStandardModel(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
    e.Check(GetSceneManipulator() && GetSceneManipulator()->getShowStandardModel());
}

void
FairyEditorFrame::OnLockBrushSize(wxCommandEvent& e)
{
    mLockBrushSize = !mLockBrushSize;
    if (mLockBrushSize)
    {
        size_t brushSize = static_cast<size_t>(
            sqrt((double)GetSceneManipulator()->getBrushXSize() * GetSceneManipulator()->getBrushZSize()));
        GetSceneManipulator()->setBrush(
            GetSceneManipulator()->getBrushType(), brushSize, brushSize);
    }
}

void
FairyEditorFrame::OnUpdateLockBrushSize(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
    e.Check(mLockBrushSize);
}

void
FairyEditorFrame::OnBrushSizeChanged(wxSpinEvent& e)
{
    // 手动点击箭头来调节画刷大小
    if (mLockBrushSize)
    {
        GetSceneManipulator()->setBrush(
            GetSceneManipulator()->getBrushType(), e.GetPosition(), e.GetPosition());
    }
    else
    {
        if (e.GetId() == feID_BRUSH_SIZE_X)
            GetSceneManipulator()->setBrush(
                GetSceneManipulator()->getBrushType(), e.GetPosition(), GetSceneManipulator()->getBrushZSize());
        else
            GetSceneManipulator()->setBrush(
                GetSceneManipulator()->getBrushType(), GetSceneManipulator()->getBrushXSize(), e.GetPosition());
    }
}

void
FairyEditorFrame::OnBrushSizeTextChanged(wxCommandEvent& e)
{
    // 手动填写数字来调节画刷大小
    if (mLockBrushSize)
    {
        GetSceneManipulator()->setBrush(
            GetSceneManipulator()->getBrushType(), e.GetInt(), e.GetInt());
    }
    else
    {
        if (e.GetId() == feID_BRUSH_SIZE_X)
            GetSceneManipulator()->setBrush(
                GetSceneManipulator()->getBrushType(), e.GetInt(), GetSceneManipulator()->getBrushZSize());
        else
            GetSceneManipulator()->setBrush(
                GetSceneManipulator()->getBrushType(), GetSceneManipulator()->getBrushXSize(), e.GetInt());
    }
}

void
FairyEditorFrame::OnUpdateBrushSizeSpin(wxUpdateUIEvent& e)
{
    // 扫描画刷大小调节选项所做的更新
    e.Enable(GetSceneManipulator() != 0);

    if (e.GetEnabled())
    {
        // 根据当前的刷子大小来修改ui界面中的数字
        // 根据当前画刷x方向的大小来更新文本框的文字（bug起因），这里的设置会调用到OnBrushSizeTextChanged，
        // 如果遇到纹理大小在x方向上和y方向上不一致时，这时就会把画刷设成在两个方向上都是相同的，所以显示的
        // 结果是不正确的（纹理短的一边会被拉伸），所以这里暂时的改法是把OnBrushSizeTextChanged函数中的内容
        // 注释了，这样，更改文本框时是不会更改画刷大小的（另一个bug，不过无关紧要）
        wxWindow* toolbar = wxDynamicCast(e.GetEventObject(), wxWindow);
        if (toolbar)
        {
            wxSpinCtrl* brushSizeSpinCtrl = wxStaticCast(toolbar->FindWindow(e.GetId()), wxSpinCtrl);
            if (brushSizeSpinCtrl)
            {
                if (e.GetId() == feID_BRUSH_SIZE_X)
                    brushSizeSpinCtrl->SetValue(GetSceneManipulator()->getBrushXSize());
                else
                    brushSizeSpinCtrl->SetValue(GetSceneManipulator()->getBrushZSize());
            }
        }
    }
}

void 
FairyEditorFrame::OnTerrainHeightChanged(wxSpinEvent& e)
{
    WX::Action *activeAction = GetSceneManipulator()->getActiveAction();

    assert (activeAction && activeAction->getName() == "SetTerrainHeightAction");

    activeAction->setParameter("%TerrainHeight", Ogre::StringConverter::toString(e.GetInt()));
}

void 
FairyEditorFrame::OnTerrainHeightTextChanged(wxCommandEvent& e)
{
    WX::Action *activeAction = GetSceneManipulator()->getActiveAction();

    assert (activeAction && activeAction->getName() == "SetTerrainHeightAction");

    activeAction->setParameter("%TerrainHeight", Ogre::StringConverter::toString(e.GetInt()));
}

void 
FairyEditorFrame::OnUpdateTerrainHeightSpin(wxUpdateUIEvent& e)
{
    // 只有SetTerrainHeightAction启动了，这个菜单才可用
    e.Enable(GetSceneManipulator() != 0 && (GetSceneManipulator()->getActiveAction()) &&
        GetSceneManipulator()->getActiveAction()->getName() == "SetTerrainHeightAction");

    if (e.GetEnabled())
    {
        wxWindow* toolbar = wxDynamicCast(e.GetEventObject(), wxWindow);
        if (toolbar)
        {
            wxSpinCtrl* terrainHeightSpinCtrl = wxStaticCast(toolbar->FindWindow(e.GetId()), wxSpinCtrl);
            if (terrainHeightSpinCtrl)
            {
                WX::Action *activeAction = GetSceneManipulator()->getActiveAction();
                assert (activeAction && activeAction->getName() == "SetTerrainHeightAction");
                int height = Ogre::StringConverter::parseInt(activeAction->getParameter("%TerrainHeight"));
                terrainHeightSpinCtrl->SetValue(height);
            }
        }
    }
}

void 
FairyEditorFrame::OnTerrainHeightIncrementChanged(wxSpinEvent& e)
{
	WX::Action *activeAction = GetSceneManipulator()->getActiveAction();

	assert (activeAction && activeAction->getName() == "SetTerrainHeightAction");

	activeAction->setParameter("%TerrainHeightIncrement", Ogre::StringConverter::toString(e.GetInt()));
}

void 
FairyEditorFrame::OnTerrainHeightIncrementTextChanged(wxCommandEvent& e)
{
	WX::Action *activeAction = GetSceneManipulator()->getActiveAction();

	assert (activeAction && activeAction->getName() == "SetTerrainHeightAction");

	activeAction->setParameter("%TerrainHeightIncrement", Ogre::StringConverter::toString(e.GetInt()));
}

void 
FairyEditorFrame::OnUpdateTerrainHeightIncrementSpin(wxUpdateUIEvent& e)
{
	// 只有SetTerrainHeightAction启动了，这个菜单才可用
	e.Enable(GetSceneManipulator() != 0 && (GetSceneManipulator()->getActiveAction()) &&
		GetSceneManipulator()->getActiveAction()->getName() == "SetTerrainHeightAction");

	if (e.GetEnabled())
	{
		wxWindow* toolbar = wxDynamicCast(e.GetEventObject(), wxWindow);
		if (toolbar)
		{
			wxSpinCtrl* terrainHeightSpinCtrl = wxStaticCast(toolbar->FindWindow(e.GetId()), wxSpinCtrl);
			if (terrainHeightSpinCtrl)
			{
				WX::Action *activeAction = GetSceneManipulator()->getActiveAction();
				assert (activeAction && activeAction->getName() == "SetTerrainHeightAction");
				int heightIncrement = Ogre::StringConverter::parseInt(activeAction->getParameter("%TerrainHeightIncrement"));
				terrainHeightSpinCtrl->SetValue(heightIncrement);
			}
		}
	}
}

void 
FairyEditorFrame::OnHeightActionaffectRangeChanged(wxSpinEvent& e)
{
    WX::Action *activeAction = GetSceneManipulator()->getActiveAction();

    assert (activeAction && 
        (activeAction->getName() == "RaiseHeightAction" || activeAction->getName() == "LowerHeightAction"));

 //   activeAction->setParameter("%AffectRange", Ogre::StringConverter::toString(e.GetInt()));
	GetSceneManipulator()->setJunctionWeightAffectRange(e.GetInt());
}

void 
FairyEditorFrame::OnHeightActionaffectRangeTextChanged(wxCommandEvent& e)
{
    WX::Action *activeAction = GetSceneManipulator()->getActiveAction();

    assert (activeAction && 
        (activeAction->getName() == "RaiseHeightAction" || activeAction->getName() == "LowerHeightAction"));

  //  activeAction->setParameter("%AffectRange", Ogre::StringConverter::toString(e.GetInt()));
	GetSceneManipulator()->setJunctionWeightAffectRange(e.GetInt());

}

void 
FairyEditorFrame::OnUpdateHeightActionaffectRangeSpin(wxUpdateUIEvent& e)
{
    // 只有SetTerrainHeightAction启动了，这个菜单才可用
    e.Enable(GetSceneManipulator() != 0 && (GetSceneManipulator()->getActiveAction()) &&
        (GetSceneManipulator()->getActiveAction()->getName() == "RaiseHeightAction" ||
          GetSceneManipulator()->getActiveAction()->getName() == "LowerHeightAction"));

    if (e.GetEnabled())
    {
        wxWindow* toolbar = wxDynamicCast(e.GetEventObject(), wxWindow);
        if (toolbar)
        {
            wxSpinCtrl* terrainHeightSpinCtrl = wxStaticCast(toolbar->FindWindow(e.GetId()), wxSpinCtrl);
            if (terrainHeightSpinCtrl)
            {
                WX::Action *activeAction = GetSceneManipulator()->getActiveAction();
                assert (activeAction && 
                    (activeAction->getName() == "RaiseHeightAction" || activeAction->getName() == "LowerHeightAction"));

           //     int height = Ogre::StringConverter::parseInt(activeAction->getParameter("%AffectRange"));
				int height = GetSceneManipulator()->getJunctionWeightAffectRange();
                terrainHeightSpinCtrl->SetValue(height);
            }
        }
    }
}

void 
FairyEditorFrame::OnHeightActionExpValueTextChanged(wxCommandEvent& e)
{
	WX::Action *activeAction = GetSceneManipulator()->getActiveAction();

	assert (activeAction && 
		(activeAction->getName() == "RaiseHeightAction" || activeAction->getName() == "LowerHeightAction"));

	//activeAction->setParameter("%ExponentValue", e.GetString().c_str());
	GetSceneManipulator()->setJunctionWeightExpValue(Ogre::StringConverter::parseReal(e.GetString().c_str()));
}

void 
FairyEditorFrame::OnUpdateHeightActionExpValue(wxUpdateUIEvent& e)
{
	// 只有SetTerrainHeightAction启动了，这个菜单才可用
	e.Enable(GetSceneManipulator() != 0 && (GetSceneManipulator()->getActiveAction()) &&
		(GetSceneManipulator()->getActiveAction()->getName() == "RaiseHeightAction" ||
		GetSceneManipulator()->getActiveAction()->getName() == "LowerHeightAction"));	
}

void 
FairyEditorFrame::OnBlurAmountTextChanged(wxCommandEvent& e)
{
	WX::PostFilterManager* pfm = GetSceneManipulator()->getPostFilterManager();
	pfm->setPostFilterParameter("Floodlighting", "BlurAmount", e.GetString().c_str());
}

void 
FairyEditorFrame::OnShineAmountTextChanged(wxCommandEvent& e)
{
	WX::PostFilterManager* pfm = GetSceneManipulator()->getPostFilterManager();
	pfm->setPostFilterParameter("Floodlighting", "ShineAmount", e.GetString().c_str());
}

void 
FairyEditorFrame::OnUpdateFloodlightingControls(wxUpdateUIEvent& e)
{
	WX::SceneManipulator *manipulator = GetSceneManipulator();
	e.Enable(
        manipulator &&
        manipulator->getPostFilterManager()->hasPostFilter("Floodlighting") &&
        manipulator->getPostFilterManager()->getPostFilterEnabled("Floodlighting"));
}

void
FairyEditorFrame::OnHeightAdjustSpeedTextChanged(wxCommandEvent& e)
{
	WX::Action *activeAction = GetSceneManipulator()->getActiveAction();

	assert (activeAction && 
		(activeAction->getName() == "RaiseHeightAction" || activeAction->getName() == "LowerHeightAction" ||
		activeAction->getName() == "SmoothHeightAction"));

	GetSceneManipulator()->setHeightAdjustSpeed(Ogre::StringConverter::parseReal(e.GetString().c_str()));
}

void 
FairyEditorFrame::OnUpdateHeightAdjustSpeed(wxUpdateUIEvent& e)
{
	// 只有SetTerrainHeightAction启动了，这个菜单才可用
	e.Enable(GetSceneManipulator() != 0 && (GetSceneManipulator()->getActiveAction()) &&
		(GetSceneManipulator()->getActiveAction()->getName() == "RaiseHeightAction" ||
		GetSceneManipulator()->getActiveAction()->getName() == "LowerHeightAction" ||
		GetSceneManipulator()->getActiveAction()->getName() == "SmoothHeightAction"));	
}

void
FairyEditorFrame::OnShowStatPanel(wxCommandEvent& e)
{
    GetSceneManipulator()->getFrameStatsListener()->showStatPanel(
        !GetSceneManipulator()->getFrameStatsListener()->isStatPanelShown());
}

void
FairyEditorFrame::OnUpdateShowStatPanel(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
    e.Check(GetSceneManipulator() && GetSceneManipulator()->getFrameStatsListener()->isStatPanelShown());
}

void
FairyEditorFrame::OnShowLogoPanel(wxCommandEvent& e)
{
    GetSceneManipulator()->getFrameStatsListener()->showLogoPanel(
        !GetSceneManipulator()->getFrameStatsListener()->isLogoPanelShown());
}

void
FairyEditorFrame::OnUpdateShowLogoPanel(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
    e.Check(GetSceneManipulator() && GetSceneManipulator()->getFrameStatsListener()->isLogoPanelShown());
}

void
FairyEditorFrame::OnToggleToolWindows(wxCommandEvent& e)
{
    bool shown = true;
    for (const DockWindowList::Node* node = mLayout->GetDockWindowList().GetFirst(); node; node = node->GetNext())
    {
        wxDockWindowBase* dockWindow = node->GetData();
        if (dockWindow->IsVisible())
        {
            shown = false;
            break;
        }
    }

    for (const DockWindowList::Node* node = mLayout->GetDockWindowList().GetFirst(); node; node = node->GetNext())
    {
        wxDockWindowBase* dockWindow = node->GetData();
        dockWindow->Show(shown);
    }
}

void
FairyEditorFrame::OnUpdateToggleToolWindows(wxUpdateUIEvent& e)
{
    bool shown = true;
    for (const DockWindowList::Node* node = mLayout->GetDockWindowList().GetFirst(); node; node = node->GetNext())
    {
        wxDockWindowBase* dockWindow = node->GetData();
        if (dockWindow->IsVisible())
        {
            shown = false;
            break;
        }
    }

    e.SetText((shown ? _("[&A]显示所有工具栏") : _("[&A]隐藏所有工具栏")) + wxString(wxT("\tAlt-Enter")));
}

void
FairyEditorFrame::OnFullScreen(wxCommandEvent& e)
{
    ShowFullScreen(!IsFullScreen());
}

void
FairyEditorFrame::OnUpdateFullScreen(wxUpdateUIEvent& e)
{
    e.Check(IsFullScreen());
}

static const char *gActionNames[] = {
    "DragHeightAction",
    "RaiseHeightAction",
    "LowerHeightAction",
    "SmoothHeightAction",
    "LevelOffHeightAction",
    "SetTerrainHeightAction",
    "TerrainLayerOneEraserAction",
    "ManipObjectAction",
    "FlipDiagonalAction",
    "DisableDecalAction",
    "TerrainSelectionAction",
};

void
FairyEditorFrame::OnSelectAction(wxCommandEvent& e)
{
    size_t index = e.GetId() - feID_ACTION_FIRST;
    if (index >= sizeof(gActionNames) / sizeof(*gActionNames))
        return;

    if (GetSceneManipulator()->getActiveAction() &&
        GetSceneManipulator()->getActiveAction()->getName() == gActionNames[index])
        GetSceneManipulator()->setActiveAction(NULL);
    else
        GetSceneManipulator()->setActiveAction(gActionNames[index]);
}

void
FairyEditorFrame::OnUpdateSelectAction(wxUpdateUIEvent& e)
{
    size_t index = e.GetId() - feID_ACTION_FIRST;
    if (index >= sizeof(gActionNames) / sizeof(*gActionNames))
    {
        e.Enable(false);
        return;
    }

    e.Enable(GetSceneManipulator() != 0);
    e.Check(GetSceneManipulator() && GetSceneManipulator()->getActiveAction() &&
            GetSceneManipulator()->getActiveAction()->getName() == gActionNames[index]);
}

void 
FairyEditorFrame::OnTerrainLightmapOption(wxCommandEvent& e)
{
	WX::Terrain* pTerrain = GetSceneManipulator()->getTerrain();
	WX::TerrainData* pTerrainData = pTerrain->getData();
	TerrainLightmapOptionDialog dialog(this, pTerrain->getLightmapQuality());
	if (dialog.ShowModal() == wxID_OK)
	{		
		if (dialog.mIsLightmapUpdated)
		{
			if (!pTerrainData->mLightmapImage)
				pTerrainData->mLightmapImage = new Ogre::Image;
			*pTerrainData->mLightmapImage = dialog.mLightmapImage;
			mIsTerrainLightmapUpdated = true;
		}

		pTerrain->setLightmapQuality(dialog.mLightmapQuality);		
		pTerrain->buildGeometry(GetSceneManipulator()->getBaseSceneNode(), GetSceneManipulator()->isEditable());

		switch (pTerrain->getLightmapQuality())
		{
		case WX::Terrain::LMQ_NONE:
			WritePrivateProfileString(_T("Terrain Lightmap"), _T("Quality"), _T("None"), _T("Startup.ini"));
			break;
		case WX::Terrain::LMQ_LOW:
			WritePrivateProfileString(_T("Terrain Lightmap"), _T("Quality"), _T("Low"), _T("Startup.ini"));
			break;
		case WX::Terrain::LMQ_HIGH:
			WritePrivateProfileString(_T("Terrain Lightmap"), _T("Quality"), _T("High"), _T("Startup.ini"));
			break;
		}
	}
	dialog.Destroy();
}

void 
FairyEditorFrame::OnTerrainLightmapHide(wxCommandEvent& e)
{	
	WX::Terrain* pTerrain = GetSceneManipulator()->getTerrain();
	pTerrain->setLightmapQuality(WX::Terrain::LMQ_NONE);
	pTerrain->buildGeometry(GetSceneManipulator()->getBaseSceneNode(), GetSceneManipulator()->isEditable());
	WritePrivateProfileString(_T("Terrain Lightmap"), _T("Quality"), _T("None"), _T("Startup.ini"));
}
void 
FairyEditorFrame::OnUpdateTerrainLightmapHide(wxUpdateUIEvent& e)
{	
	WX::Terrain* pTerrain = GetSceneManipulator()->getTerrain();
	e.Check(pTerrain->getLightmapQuality() == WX::Terrain::LMQ_NONE);
}
void 
FairyEditorFrame::OnTerrainLightmapLowQuality(wxCommandEvent& e)
{		
	WX::Terrain* pTerrain = GetSceneManipulator()->getTerrain();
	pTerrain->setLightmapQuality(WX::Terrain::LMQ_LOW);
	pTerrain->buildGeometry(GetSceneManipulator()->getBaseSceneNode(), GetSceneManipulator()->isEditable());
	WritePrivateProfileString(_T("Terrain Lightmap"), _T("Quality"), _T("Low"), _T("Startup.ini"));
}
void 
FairyEditorFrame::OnUpdateTerrainLightmapLowQuality(wxUpdateUIEvent& e)
{
	WX::Terrain* pTerrain = GetSceneManipulator()->getTerrain();
	e.Check(pTerrain->getLightmapQuality() == WX::Terrain::LMQ_LOW);
}
void 
FairyEditorFrame::OnTerrainLightmapHighQuality(wxCommandEvent& e)
{
	WX::Terrain* pTerrain = GetSceneManipulator()->getTerrain();
	pTerrain->setLightmapQuality(WX::Terrain::LMQ_HIGH);
	pTerrain->buildGeometry(GetSceneManipulator()->getBaseSceneNode(), GetSceneManipulator()->isEditable());
	WritePrivateProfileString(_T("Terrain Lightmap"), _T("Quality"), _T("High"), _T("Startup.ini"));
}
void 
FairyEditorFrame::OnUpdateTerrainLightmapHighQuality(wxUpdateUIEvent& e)
{
	WX::Terrain* pTerrain = GetSceneManipulator()->getTerrain();
	e.Check(pTerrain->getLightmapQuality() == WX::Terrain::LMQ_HIGH);
}

void 
FairyEditorFrame::OnTerrainSurfaceParameters(wxCommandEvent& e)
{
    TerrainSurfaceParametersDialog dlg(this, wxID_ANY,
        _("Terrain Surface Parameters"), GetSceneManipulator());
    if (dlg.ShowModal() == wxID_OK)
    {       
    }
}

void
FairyEditorFrame::OnSceneAnimationQuality(wxCommandEvent& e)
{
    long level = static_cast<long>(GetSceneManipulator()->getAnimationQuality()) + 50;
    long newLevel = wxGetNumberFromUser(
        _("Please input scene animation level"),
        _("动作等级:"),
        _("Set scene animation level"),
        level, 0, 100,
        this);
    if (newLevel >= 0 && newLevel != level)
    {
        GetSceneManipulator()->setAnimationQuality(static_cast<Ogre::Real>(newLevel - 50));
    }
}

void
FairyEditorFrame::OnObjectCreateLevel(wxCommandEvent& e)
{
    long level = static_cast<long>(GetSceneManipulator()->getCreateLevel());
    long newLevel = wxGetNumberFromUser(
        _("Please input object create level"),
        _("物体创建等级:"),
        _("Set object create level"),
        level, -50, 50,
        this);
    if (newLevel != level)
    {
        GetSceneManipulator()->setCreateLevel(static_cast<Ogre::Real>(newLevel));
    }
}

void 
FairyEditorFrame::OnSelectBrushShape(wxCommandEvent& e)
{
	int id = e.GetId();

	if (GetSceneManipulator())
	{
		if (id == feID_BOX_BRUSH)
		{
			GetSceneManipulator()->setBrush("BoxBrushShape", 
				GetSceneManipulator()->getBrushXSize(), GetSceneManipulator()->getBrushZSize());
		}
		else if (id == feID_ELLIPSE_BRUSH)
		{
			GetSceneManipulator()->setBrush("EllipseBrushShape", 
				GetSceneManipulator()->getBrushXSize(), GetSceneManipulator()->getBrushZSize());
		}
	}	
}

void 
FairyEditorFrame::OnUpdateSelectBrushShape(wxUpdateUIEvent& e)
{
	e.Enable(GetSceneManipulator() != 0);

	int id = e.GetId();

	if (GetSceneManipulator())
	{
		if (id == feID_BOX_BRUSH)
		{
			e.Check(GetSceneManipulator()->getBrushType() == "BoxBrushShape");
		}
		else if (id == feID_ELLIPSE_BRUSH)
		{
			e.Check(GetSceneManipulator()->getBrushType() == "EllipseBrushShape");
		}
	}
}

void
FairyEditorFrame::OnAlignPosition(wxCommandEvent& e)
{
    if (GetSceneManipulator()->getPositionAlignGranularity() > 0)
        GetSceneManipulator()->setPositionAlignGranularity(0);
    else
        GetSceneManipulator()->setPositionAlignGranularity(1);
}

void
FairyEditorFrame::OnUpdateAlignPosition(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
    e.Check(GetSceneManipulator() && GetSceneManipulator()->getPositionAlignGranularity() > 0);
}

void 
FairyEditorFrame::OnFrameRateTextChanged(wxCommandEvent& e)
{
	unsigned long frameRate = Ogre::StringConverter::parseUnsignedLong(e.GetString().c_str());

	if (frameRate < 0)
		frameRate = 0;

	if (mCanvas)
		mCanvas->setTargetFPS(frameRate);
}

void 
FairyEditorFrame::OnPreferSetting(wxCommandEvent& e)
{
    SettingsDialog dialog(this);
    dialog.ShowModal();
}

void 
FairyEditorFrame::OnGpuProgramParameterSetting(wxCommandEvent& e)
{
	if (NULL == mGpuProgramParameterSettingDialog)
	{
		int width = 0;
		int height = 0;
		int x = 0;
		int y = 0;

		GetSize(&width, &height);
		GetPosition(&x, &y);

		mGpuProgramParameterSettingDialog =
			new GpuProgramParameterSettingDialog(this, wxID_ANY, _("Gpu参数编辑器"), wxPoint(x, y), wxSize(width / 3, height));
	}
	if (!mGpuProgramParameterSettingDialog->IsShown())
		mGpuProgramParameterSettingDialog->Show();	
}

void 
FairyEditorFrame::GetSelectedTerrainInfo(Ogre::String &infoString)
{
    Ogre::Vector3 mousePosition;
    wxPoint currentMousePos = mCanvas->mCurrentMousePos;

    bool intersected = GetSceneManipulator()->getTerrainIntersects(currentMousePos.x, currentMousePos.y, mousePosition);

    if (intersected)
    {        
        if (GetSceneManipulator()->getActiveAction() &&
            GetSceneManipulator()->getActiveAction()->getName() == "TerrainSelectionAction")
        {
            infoString += GetSceneManipulator()->getActiveAction()->getParameter("%JunctionHeightInfo").c_str();
            infoString += GetSceneManipulator()->getActiveAction()->getParameter("%GridTextureInfo").c_str();
            infoString += GetSceneManipulator()->getActiveAction()->getParameter("%GridDiagonalInfo").c_str();			

			mInfoString.clear();

			if (GetSceneManipulator()->getActiveAction()->getParameter("%PasteDiagonalInfo") == "true")
				mInfoString += GetSceneManipulator()->getActiveAction()->getParameter("%GridDiagonalInfo").c_str();
			if (GetSceneManipulator()->getActiveAction()->getParameter("%PasteTextureInfo") == "true")
				mInfoString += GetSceneManipulator()->getActiveAction()->getParameter("%GridTextureInfo").c_str();
			if (GetSceneManipulator()->getActiveAction()->getParameter("%PasteHeightInfo") == "true")
				mInfoString += GetSceneManipulator()->getActiveAction()->getParameter("%JunctionHeightInfo").c_str();
		}
    }  
}

void
FairyEditorFrame::OnCut(wxCommandEvent& event)
{


    Ogre::String infoString;

    WX::Action *currentAction = GetSceneManipulator()->getActiveAction();

    if (currentAction && currentAction->getName() == "ManipObjectAction")
    {
        GetSceneManipulator()->getSelectedObjectInfo(infoString);

        wxTextDataObject *data = new wxTextDataObject(infoString);
        wxTheClipboard->SetData(data);

        // 删除原有的物体
        GetSceneManipulator()->removeSelectedObjects();
    }    
}

void 
FairyEditorFrame::OnCopy(wxCommandEvent& event)
{
    Ogre::String infoString;

    WX::Action *currentAction = GetSceneManipulator()->getActiveAction();
	mInfoString.clear();

    if (currentAction)
    {
        // 在不同的操作中使用的拷贝的是不同的信息
        if (currentAction->getName() == "ManipObjectAction")
		{
			GetSceneManipulator()->getSelectedObjectInfo(infoString);
			mManipInfoString = infoString;
		}

        else if (currentAction->getName() == "TerrainSelectionAction")
        {
			// 先获取数据，再设置“拷贝后”的标志
            GetSelectedTerrainInfo(infoString);
			currentAction->setParameter("%AfterCopy", "true");
        }

        wxTextDataObject *data = new wxTextDataObject(infoString);
        wxTheClipboard->SetData(data);
    }   
}

void 
FairyEditorFrame::OnPaste(wxCommandEvent& event)
{
    // 得到鼠标当前与地面的交接点
    Ogre::Vector3 mousePosition;
    wxPoint currentMousePos = mCanvas->mCurrentMousePos;

    bool intersected = GetSceneManipulator()->getTerrainIntersects(currentMousePos.x, currentMousePos.y, mousePosition);

    if (intersected)
    {
        wxTextDataObject data;
        if (wxTheClipboard->IsSupported(data.GetFormat())
            && wxTheClipboard->GetData(data))
        {
            Ogre::String text = data.GetText().c_str();

            if (!text.empty())
            {
                WX::Action *currentAction = GetSceneManipulator()->getActiveAction();

                // 不同的action粘贴不同的东西
                if (currentAction && currentAction->getName() == "TerrainSelectionAction")
                {
                    currentAction->setParameter("%CurrentMousePosition", Ogre::StringConverter::toString(mousePosition));
                }
                else if (currentAction && currentAction->getName() == "ManipObjectAction")

				{
					mousePosition = GetSceneManipulator()->_alignPosition(mousePosition,1.0f);
                    // 粘贴ojbect
                    GetSceneManipulator()->parseStringAndCreateObject(text, mousePosition);
				}

            }
        }
    }    
}

void
FairyEditorFrame::OnSaveTerrainInfoToFile(wxCommandEvent& e)
{
	if (!mInfoString.empty())
	{
		wxFileDialog dialog(this, _("把地形信息保存到文件"), wxT(""), wxT(""), _("所有文件 (%s)|%s"), 0);

		if (dialog.ShowModal() == wxID_OK)
		{
			if (!dialog.GetPath().IsEmpty())
			{
				wxFile fileTerrainInfo;
				if (fileTerrainInfo.Create(dialog.GetPath(),true,wxS_IWUSR |wxS_IWGRP |wxS_IWOTH))
				{
					fileTerrainInfo.Write(mInfoString.c_str());
					fileTerrainInfo.Close();
					wxMessageBox(_("地形文件保存成功"));
				}
				else
					wxMessageBox(_("地形文件保存失败"));
			}

		}
	}
}

void 
FairyEditorFrame::OnUpdateSaveTerrainInfoToFile(wxUpdateUIEvent& e)
{
    e.Enable(true);
}

void
FairyEditorFrame::OnLoadTerrainInfoFromFile(wxCommandEvent& e)
{
    wxFileDialog dialog(this, _("加载地形文件"), wxT(""), wxT(""), _("所有文件 (%s)|%s"), 0);

    if (dialog.ShowModal() == wxID_OK)
    {
        if (!dialog.GetPath().IsEmpty())
        {
            wxFile fileTerrainInfo;            
            fileTerrainInfo.Open(dialog.GetPath(),wxFile::read);
			wxString infoString;
			
			wxChar* buf = infoString.GetWriteBuf(fileTerrainInfo.Length()+1);
			
            fileTerrainInfo.Read((void*)buf,fileTerrainInfo.Length());
			Ogre::String ifStr = infoString;
            fileTerrainInfo.Close();

            wxMessageBox(_("加载地形文件成功"));

			GetSceneManipulator()->setActiveAction("FixPaintAction");
			GetSceneManipulator()->getActiveAction()->setParameter("%InfoString",ifStr);

			mIsTerrainLightmapUpdated = false;
        }

    }
}

void 
FairyEditorFrame::OnUpdateLoadTerrainInfoFromFile(wxUpdateUIEvent& e)
{
    e.Enable(true);
}


void
FairyEditorFrame::OnSaveManipInfoToFile(wxCommandEvent& e)
{
	if (!mManipInfoString.empty())
	{
		wxFileDialog dialog(this, _("保存操作对象信息到文件"), wxT(""), wxT(""), _("MANIPOBJINFO files (*.manipobjinfo)|*.manipobjinfo"), 0);

		if (dialog.ShowModal() == wxID_OK)
		{
			if (!dialog.GetPath().IsEmpty())
			{
				wxFile fileManipInfo;
				if (fileManipInfo.Create(dialog.GetPath(),true,wxS_IWUSR |wxS_IWGRP |wxS_IWOTH))
				{
					fileManipInfo.Write(mManipInfoString.c_str());
					fileManipInfo.Close();
					wxMessageBox(_("操作对象信息文件保存成功"));
				}
				else
					wxMessageBox(_("操作对象信息文件保存失败"));
			}

		}
	}
}

void 
FairyEditorFrame::OnUpdateSaveManipInfoToFile(wxUpdateUIEvent& e)
{
	e.Enable(true);
}

void
FairyEditorFrame::OnLoadManipInfoFromFile(wxCommandEvent& e)
{
	wxFileDialog dialog(this, _("加载操作对象信息文件"), wxT(""), wxT(""), _("MANIPOBJINFO files (*.manipobjinfo)|*.manipobjinfo"), 0);

	if (dialog.ShowModal() == wxID_OK)
	{
		if (!dialog.GetPath().IsEmpty())
		{
			wxFile fileManipInfo;            
			fileManipInfo.Open(dialog.GetPath(),wxFile::read);
			wxString infoString;

			wxChar* buf = infoString.GetWriteBuf(fileManipInfo.Length()+1);

			fileManipInfo.Read((void*)buf,fileManipInfo.Length());
			Ogre::String ifStr = infoString;
			fileManipInfo.Close();

			wxMessageBox(_("加载操作对象信息文件成功"));

			GetSceneManipulator()->setActiveAction("FixManipObjAction");
			GetSceneManipulator()->getActiveAction()->setParameter("%InfoString",ifStr);
		}

	}
}

void 
FairyEditorFrame::OnUpdateLoadManipInfoFromFile(wxUpdateUIEvent& e)
{
	e.Enable(true);
}

void
FairyEditorFrame::OnSaveEnviromentInfoToFile(wxCommandEvent& e)
{
	// 获取当前场景中的环境信息
	WX::SceneInfo *sceneInfo = GetSceneManipulator()->getSceneInfo();
	assert (sceneInfo);

	wxString wrongInfoStr;
	wrongInfoStr.Empty();

	wxString enviInfoStr;
	enviInfoStr.Empty();

	// 获取环境信息
	const WX::ObjectPtr envObject = sceneInfo->findObjectByName(WX::FairyAutoEnviromentObjectName);

	if (!envObject)
	{
		wrongInfoStr << _("环境物体的名称错误！\n")
			<< _("环境物体的名称应为#FairyEnviroment\n");  
	}
	else
	{
		enviInfoStr << "enviroment\n";

		const WX::PropertyList& properties = envObject->getProperties();
        for (WX::PropertyList::const_iterator it = properties.begin(); it != properties.end(); ++it)
        {
            const WX::PropertyDef& propertyDef = *it;

            // 不保存场景的天气信息
            if (propertyDef.name != "weather.particle system name" &&
                propertyDef.name != "weather.render weather")
            {
                enviInfoStr << propertyDef.name.c_str() << '=' 
                    << envObject->getPropertyAsString(propertyDef.name).c_str() << '\n';            
            }
        }
	}

	// 第一个光源的信息
	const WX::ObjectPtr mainLightOneObject = sceneInfo->findObjectByName(WX::FairyAutoMainLightOneObjectName);

	if (!mainLightOneObject)
	{
		wrongInfoStr << _("第一个主光源的名称错误！\n")
			<< _("第一个主光源的名称应为#FairyMainLightOne\n");  
	}
	else
	{
		enviInfoStr << "main_light_one\n";

		const WX::PropertyList& properties = mainLightOneObject->getProperties();
        for (WX::PropertyList::const_iterator it = properties.begin(); it != properties.end(); ++it)
        {
            const WX::PropertyDef& propertyDef = *it;
            enviInfoStr << propertyDef.name.c_str() << '=' 
				<< mainLightOneObject->getPropertyAsString(propertyDef.name).c_str() << '\n';            
        }
	}

	// 获取第二个光源的信息
	const WX::ObjectPtr mainLightTwoObject = sceneInfo->findObjectByName(WX::FairyAutoMainLightTwoObjectName);

	if (!mainLightTwoObject)
	{
		wrongInfoStr << _("第二个主光源的名称错误！\n")
			<< _("第二个主光源的名称应为#FairyMainLightTwo\n");  
	}
	else
	{
		enviInfoStr << "main_light_two\n";

		const WX::PropertyList& properties = mainLightTwoObject->getProperties();
        for (WX::PropertyList::const_iterator it = properties.begin(); it != properties.end(); ++it)
        {
            const WX::PropertyDef& propertyDef = *it;
            enviInfoStr << propertyDef.name.c_str() << '=' 
				<< mainLightTwoObject->getPropertyAsString(propertyDef.name).c_str() << '\n';           
        }
	}	

	if (!wrongInfoStr.IsEmpty())
	{
		wxMessageBox(wrongInfoStr);
	}
	else
	{
		wxFileDialog dialog(this, _T("保存环境信息到文件"), wxT(""), wxT(""), _("Enviroment infomation files (*.envinfo)|*.envinfo"), 0);

		if (dialog.ShowModal() == wxID_OK)
		{
			if (!dialog.GetPath().IsEmpty())
			{
				wxFile infoFile;
				if (infoFile.Create(dialog.GetPath(),true,wxS_IWUSR |wxS_IWGRP |wxS_IWOTH))
				{
					infoFile.Write(enviInfoStr.c_str());
					infoFile.Close();
					wxMessageBox(_("保存环境信息文件成功"));
				}
				else
					wxMessageBox(_("保存环境信息文件失败"));
			}

		}
	}	
}

void 
FairyEditorFrame::OnUpdateSaveEnviromentInfoToFile(wxUpdateUIEvent& e)
{
	e.Enable(true);
}

void
FairyEditorFrame::OnLoadEnviromentInfoFromFile(wxCommandEvent& e)
{
	wxString wrongInfoStr;
	wrongInfoStr.Empty();

	// 获取当前场景中的环境信息
	WX::SceneInfo *sceneInfo = GetSceneManipulator()->getSceneInfo();
	assert (sceneInfo);

	WX::ObjectPtr enviObject = sceneInfo->findObjectByName(WX::FairyAutoEnviromentObjectName);

	if (!enviObject)
	{
		wrongInfoStr << _("环境物体名称错误!\n")
			<< _("环境物体的名称应为 #FairyEnviroment\n");  
	}

	WX::ObjectPtr mainLightOneObject = sceneInfo->findObjectByName(WX::FairyAutoMainLightOneObjectName);

	if (!mainLightOneObject)
	{
		wrongInfoStr << _("第一个主光源的名称错误！\n")
			<< _("第一个主光源的名称应为#FairyMainLightOne\n");  
	}

	WX::ObjectPtr mainLightTwoObject = sceneInfo->findObjectByName(WX::FairyAutoMainLightTwoObjectName);

	if (!mainLightTwoObject)
	{
		wrongInfoStr << _("第二个主光源的名称错误！\n")
			<< _("第二个主光源的名称应为#FairyMainLightTwo\n");  
	}

	if (!wrongInfoStr.IsEmpty())
	{
		wxMessageBox(wrongInfoStr);
	}
	else
	{
		wxFileDialog dialog(this, _("加载环境信息文件"), wxT(""), wxT(""), _("Enviroment infomation files (*.envinfo)|*.envinfo"), 0);

		if (dialog.ShowModal() == wxID_OK)
		{
			if (!dialog.GetPath().IsEmpty())
			{
				wxFile infoFile;            
				infoFile.Open(dialog.GetPath(),wxFile::read);
				wxString infoString;

				wxChar* buf = infoString.GetWriteBuf(infoFile.Length()+1);

				infoFile.Read((void*)buf,infoFile.Length());

				wxMessageBox(_("加载环境信息文件成功"));

				// 设置环境信息
				Ogre::MemoryDataStream stream((void *)(infoString.c_str()), infoFile.Length(), false);
				Ogre::String line;
				WX::ObjectPtr currentObj;
				while(!stream.eof())
				{
					line = stream.getLine();

					if (!line.empty())
					{
						if (line == "enviroment")
						{
							currentObj = enviObject;
						}
						else if (line == "main_light_one")
						{
							currentObj = mainLightOneObject;
						}
						else if (line == "main_light_two")
						{
							currentObj = mainLightTwoObject;
						}
						else
						{
							Ogre::StringVector paraVec = Ogre::StringUtil::split(line, "=");
							assert (paraVec.size() == 2);
							currentObj->setPropertyAsString(paraVec[0], paraVec[1]);
						}
					}					
				}

				infoFile.Close();
			}
		}
	}	
}

void 
FairyEditorFrame::OnUpdateLoadEnviromentInfoFromFile(wxUpdateUIEvent& e)
{
	e.Enable(true);
}

void 
FairyEditorFrame::OnUpdateCutCopyPaste(wxUpdateUIEvent& e)
{
    bool enable = false;

    WX::Action *currentAction = GetSceneManipulator()->getActiveAction();

    switch (e.GetId())
    {
    case wxID_COPY:
    case wxID_PASTE:
        enable = (currentAction && (currentAction->getName() == "ManipObjectAction" || 
            currentAction->getName() == "TerrainSelectionAction"));
        break;

    case wxID_CUT:
        enable = (currentAction && currentAction->getName() == "ManipObjectAction");
        break;
    }

    enable &= (wxWindow::FindFocus() == mCanvas);

    e.Enable(enable);
}

void
FairyEditorFrame::SetSpinnerValue(int value)
{
    mSpinnerValue = value;
}
int 
FairyEditorFrame::GetSpinnerValue()
{
    return mSpinnerValue;
}

wxString
FairyEditorFrame::GetByAxis()
{
    return mByAxis;
}
void 
FairyEditorFrame::SetByAxis(wxString& strAxis)
{
    mByAxis = strAxis;
}

void 
FairyEditorFrame::SetSnapRotationValue(int value)
{
    mSegment = value;
}

int 
FairyEditorFrame::GetSnapRotationValue()
{
    return mSegment;
}

void 
FairyEditorFrame::SetActiveSnapRotation(bool value)
{
    mActiveSnapRotation = value;
}

bool
FairyEditorFrame::GetActiveSnapRotation()
{
    return mActiveSnapRotation;
}

void 
FairyEditorFrame::OnCreateLiquid(wxCommandEvent& e)
{
	// 打开设置水面信息的对话框
	LiquidCreatorDialog dlg(this, wxID_ANY, _("创建液体物体"));
	if (dlg.ShowModal() == wxID_OK)
	{
		WX::Action* action = GetSceneManipulator()->_getAction("CreateComplexObjectAction");
		assert (action);
		action->setParameter("type", "TerrainLiquid");

		action->setParameter("%subdivision", AS_STRING(dlg.mSubDivideTextCtrl->GetValue()));

		action->setParameter("%texture scale", AS_STRING(dlg.mTexScaleTextCtrl->GetValue()));

		action->setParameter("%material", AS_STRING(dlg.mComboBox->GetValue()));

		// 启动复杂物体创建action
		GetSceneManipulator()->setActiveAction(action);
	}
}

void 
FairyEditorFrame::OnCreateLight(wxCommandEvent& e)
{
	WX::Action* action = GetSceneManipulator()->_getAction("CreateObjectAction");
	assert (action);
	action->setParameter("type", "Light");

	action->setParameter("%type", "point");

	// 启动复杂物体创建action
	GetSceneManipulator()->setActiveAction(action);
}

void 
FairyEditorFrame::SetKeyboardShortcuts(void)
{
	wxAcceleratorEntry entries[4];

	// 设置画刷选择的快捷键
	entries[0].Set(wxACCEL_SHIFT, (int) 'Z', feID_BOX_BRUSH);
	entries[1].Set(wxACCEL_SHIFT, (int) 'X', feID_ELLIPSE_BRUSH);

	// 恢复摄像机到初始高度和初始角度
	entries[2].Set(wxACCEL_SHIFT, (int) 'Q', feID_RESET_CAMERA);

	wxAcceleratorTable accel(3, entries);

	this->SetAcceleratorTable(accel);	
}

static const char * const gPostFilterTypes[] = {
    "HeatVision",
    "Bloom",
    "Floodlighting",
    "Fade",
    "MotionBlur",
};

void
FairyEditorFrame::OnSelectPostFilter(wxCommandEvent& e)
{
    size_t index = e.GetId() - feID_POST_FILTER_FIRST;
    if (index >= sizeof(gPostFilterTypes) / sizeof(*gPostFilterTypes))
        return;

    WX::PostFilterManager* pfm = GetSceneManipulator()->getPostFilterManager();
    pfm->setPostFilterEnabled(gPostFilterTypes[index], !pfm->getPostFilterEnabled(gPostFilterTypes[index]));
}

void
FairyEditorFrame::OnUpdateSelectPostFilter(wxUpdateUIEvent& e)
{
    size_t index = e.GetId() - feID_POST_FILTER_FIRST;
    if (index >= sizeof(gPostFilterTypes) / sizeof(*gPostFilterTypes))
    {
        e.Enable(false);
        return;
    }

	WX::SceneManipulator *manipulator = GetSceneManipulator();
    bool enabled = manipulator &&
        manipulator->getPostFilterManager()->hasPostFilter(gPostFilterTypes[index]) &&
        manipulator->getPostFilterManager()->getPostFilter(gPostFilterTypes[index])->getCompositorInstance();
    e.Enable(enabled);
    e.Check(enabled && GetSceneManipulator()->getPostFilterManager()->getPostFilterEnabled(gPostFilterTypes[index]));
}

static const Ogre::ShadowTechnique gsShadowTechniques[] =
{
    Ogre::SHADOWTYPE_NONE,
    Ogre::SHADOWTYPE_TEXTURE_ADDITIVE,
    Ogre::SHADOWTYPE_TEXTURE_MODULATIVE,
    Ogre::SHADOWTYPE_STENCIL_ADDITIVE,
    Ogre::SHADOWTYPE_STENCIL_MODULATIVE,
};

void
FairyEditorFrame::OnSelectShadowTechnique(wxCommandEvent& e)
{
    size_t index = e.GetId() - feID_SHADOWTYPE_NONE;
    if (index >= sizeof(gsShadowTechniques) / sizeof(*gsShadowTechniques))
        return;

	WX::SceneManipulator* manipulator = GetSceneManipulator();
    manipulator->getSceneManager()->setShadowTechnique(gsShadowTechniques[index]);
}

void
FairyEditorFrame::OnUpdateSelectShadowTechnique(wxUpdateUIEvent& e)
{
    size_t index = e.GetId() - feID_SHADOWTYPE_NONE;
    if (index >= sizeof(gsShadowTechniques) / sizeof(*gsShadowTechniques))
    {
        e.Enable(false);
        return;
    }

	WX::SceneManipulator* manipulator = GetSceneManipulator();
    e.Enable(manipulator != 0);
    e.Check(
        manipulator &&
        manipulator->getSceneManager()->getShadowTechnique() == gsShadowTechniques[index]);
}

static const Ogre::TextureFilterOptions gsTextureFilterOptions[] =
{
    Ogre::TFO_NONE,
    Ogre::TFO_BILINEAR,
    Ogre::TFO_TRILINEAR,
    Ogre::TFO_ANISOTROPIC,
};

void
FairyEditorFrame::OnSelectDefaultTextureFiltering(wxCommandEvent& e)
{
    size_t index = e.GetId() - feID_TFO_NONE;
    if (index >= sizeof(gsTextureFilterOptions) / sizeof(*gsTextureFilterOptions))
        return;

    Ogre::MaterialManager* mm = Ogre::MaterialManager::getSingletonPtr();
    if (mm)
    {
        mTextureFilterOptions = index;
        mm->setDefaultTextureFiltering(gsTextureFilterOptions[index]);
        mm->setDefaultAnisotropy(1 << index);
    }
}

void
FairyEditorFrame::OnUpdateSelectDefaultTextureFiltering(wxUpdateUIEvent& e)
{
    size_t index = e.GetId() - feID_TFO_NONE;
    if (index >= sizeof(gsTextureFilterOptions) / sizeof(*gsTextureFilterOptions))
    {
        e.Enable(false);
        return;
    }

    Ogre::MaterialManager* mm = Ogre::MaterialManager::getSingletonPtr();
    e.Enable(mm != 0);
    e.Check(index == mTextureFilterOptions);
}

static const int gsCreateLevelOptions[] =
{
    0.0f,
    40.0f,
    50.0f,
};

void
FairyEditorFrame::OnSelectSceneCreateLevel(wxCommandEvent& e)
{
    size_t index = e.GetId() - feID_CL_LOW;
    if (index >= sizeof(gsCreateLevelOptions) / sizeof(*gsCreateLevelOptions))
        return;

    mCreateLevelOptions = index;
    GetSceneManipulator()->setCreateLevel( gsCreateLevelOptions[index] );
}

void
FairyEditorFrame::OnUpdateSelectSceneCreateLevel(wxUpdateUIEvent& e)
{
    size_t index = e.GetId() - feID_CL_LOW;
    if (index >= sizeof(gsCreateLevelOptions) / sizeof(*gsCreateLevelOptions))
    {
        e.Enable(false);
        return;
    }
    
    e.Check(index == mCreateLevelOptions);
}

void 
FairyEditorFrame::OnUseRealCamera(wxCommandEvent& e)
{
	WX::SceneManipulator *manipulator = GetSceneManipulator();

	assert (manipulator);

	mUseRealCamera = !mUseRealCamera;
	manipulator->setUseRealCameraHeight(mUseRealCamera);
	manipulator->setUseRealCameraAngle(mUseRealCamera);

	if (mUseRealCamera)
	{
		manipulator->setRealCameraHeight(manipulator->getRealCameraHeight());
		manipulator->setRealCameraAngle(manipulator->getRealCameraAngle());

		Ogre::Vector3 currentCameraPos = manipulator->getCamera()->getPosition();
		manipulator->setRealCamera(currentCameraPos.x, currentCameraPos.z);
	}
}
void 
FairyEditorFrame::OnUpdateUseRealCamera(wxUpdateUIEvent& e)
{
	WX::SceneManipulator *manipulator = GetSceneManipulator();
	e.Enable(manipulator != 0);
	if (e.GetEnabled())
		e.Check(manipulator->getUseRealCameraHeight() || manipulator->getUseRealCameraAngle());
}

void 
FairyEditorFrame::OnUseSnapRotation(wxCommandEvent& e)
{
	mUseSnapRotation = !mUseSnapRotation;
	SetActiveSnapRotation(mUseSnapRotation);

	if (mUseSnapRotation)
	{
		int rotation = GetSnapRotationValue();
		if (rotation <= 0)
			rotation = 1;
		GetSceneManipulator()->setRotationAlignGranularity(360.0f / rotation);
	}
	else
		GetSceneManipulator()->setRotationAlignGranularity(0.0f);
}
void 
FairyEditorFrame::OnUpdateUseSnapRotation(wxUpdateUIEvent& e)
{
	e.Enable(GetSceneManipulator() != 0);
	e.Check(GetActiveSnapRotation());
}

void
FairyEditorFrame::OnSceneInfomation(wxCommandEvent& e)
{
    if (NULL == mSceneInfomationDialog)
    {
        int width = 0;
        int height = 0;
        int x = 0;
        int y = 0;

        GetSize(&width, &height);
        GetPosition(&x, &y);

        mSceneInfomationDialog =
            new SceneInfomationDialog(this, wxID_ANY, _("Scene Infomation"), GetSceneManipulator(), wxPoint(x, y), wxSize(width / 3, height));
    }
    if (!mSceneInfomationDialog->IsShown())
        mSceneInfomationDialog->Show();	
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// 不可行走区域
//

// 存储不可行走区域.
bool FairyEditorFrame::SaveRegion()
{

	// 提取主文件名字。
	wxString strFile = "";
	wxString strRegionFile = "";

	int iLen = m_lastSceneDirectory.Length() + 1;
	strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	int iPos = strFile.find('.');
	strFile = strFile.Left(iPos);

	strFile = "\\" + strFile;
	strFile =  m_lastSceneDirectory + strFile;

	// 组合不可行走区域文件名字。
	strRegionFile = strFile + ".Region";

	//存储不可行走区域
	std::string strfile = strRegionFile;
	if (m_pRegionEditSelector)
	{
		m_pRegionEditSelector->SaveData(strfile);
	}

	return true;

}


// 读取不可行走区域
bool FairyEditorFrame::ReadRegion()
{
	
	// 得到不可行走区域的文件， 然后读取数据
	int iLen = m_lastSceneDirectory.Length() + 1;
	wxString strRegionFile = "";

	strRegionFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	int iPos = strRegionFile.find('.');
	strRegionFile = strRegionFile.Left(iPos);

	strRegionFile = "\\" + strRegionFile;
	strRegionFile =  m_lastSceneDirectory + strRegionFile;
	strRegionFile += ".Region";

	std::string strfile = strRegionFile;
	if (m_pRegionEditSelector)
	{
		m_pRegionEditSelector->LoadData(strfile);
	}

	return true;
}


// 清空不可行走区域数据
void FairyEditorFrame::ClearRegion()
{
	// 不可走区域清空.
	if (m_pRegionEditSelector)
	{
		m_pRegionEditSelector->ClearData();
	}
}

//
// 不可行走区域
//
/////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////////
//
// npc 操作.
//

// 存储npc
bool FairyEditorFrame::SaveNpc()
{
	{
		// 得到npc的文件， 然后读取数据
		int iLen = m_lastSceneDirectory.Length() + 1;
		wxString strNpcFile = "";

		strNpcFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
		int iPos = strNpcFile.find('.');

		// 得到文件名字
		strNpcFile = strNpcFile.Left(iPos);
		// 通过映射表得到文件名字
		//strNpcFile = GetSceneName();

		strNpcFile = "\\" + strNpcFile;
		strNpcFile =  m_lastSceneDirectory + strNpcFile;
		strNpcFile += "_monster.ini";//

		std::string strfile = strNpcFile;
		if ((m_pMonsterEditDlg)&&(m_pMonsterEditDlg->m_pMonsterEditAction))
		{
			m_pMonsterEditDlg->SaveMonsterToFile(strfile);
		}

	}

	return true;
}


// 读取npc
bool FairyEditorFrame::ReadNpc()
{
	ClearNpc();

	// 得到npc的文件， 然后读取数据
	int iLen = m_lastSceneDirectory.Length() + 1;
	std::string strNpcFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	strNpcFile = strNpcFile.substr(0,strNpcFile.find('.'));
	strNpcFile = m_lastSceneDirectory + "\\" + strNpcFile + "_monster.ini";

	if ((m_pMonsterEditDlg)&&(m_pMonsterEditDlg->m_pMonsterEditAction))
	{
		m_pMonsterEditDlg->ReadMonsterFromFile(strNpcFile);
	}

	return true;
}


//清空npc
void FairyEditorFrame::ClearNpc()
{
	if (m_pMonsterEditDlg)
	{
		m_pMonsterEditDlg->m_MonsterInfoTable.clear();
		m_pMonsterEditDlg->ClearUI();
		m_pMonsterEditDlg->ClearActionData();
	}
}

//
// npc 操作.
//
//////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////
//
// 建筑物行走面
//

// 存储建筑物行走面.
bool FairyEditorFrame::SaveBuildingWalk()
{
	// 提取主文件名字。
	wxString strFile = "";
	wxString strCollisionFile = "";

	int iLen = m_lastSceneDirectory.Length() + 1;
	strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	int iPos = strFile.find('.');
	strFile = strFile.Left(iPos);

	strFile = "\\" + strFile;
	strFile =  m_lastSceneDirectory + strFile;


	// 组合建筑物行走面碰撞文件名字
	strCollisionFile =  strFile + ".WCollision";

	// 创建建筑物行走面数据.
	CreateBuildingCollisionData();

	// 存储注册的行走面文件。
	SaveBuildingCollisionInfo(strCollisionFile);
	
	return true;
}


// 清空行走面数据
void FairyEditorFrame::ClearBuildingWalk()
{

}

//
// 建筑物行走面
//
///////////////////////////////////////////////////////////////////////////////////////////////

	

	

//////////////////////////////////////////////////////////////////////////////////////////////
//
// 生长点编辑
//

// 读取生长点
bool FairyEditorFrame::ReadGrowPoint()
{
	ClearGrowPoint();
	if ((m_pGrowPointEditDlg)&&(m_pGrowPointEditDlg->m_pGrowPointEditAction))
	{

		m_pGrowPointEditDlg->InitAddedGrowPointTree();
		// 得到npc的文件， 然后读取数据
		int iLen = m_lastSceneDirectory.Length() + 1;
		wxString strGrowPointFile = "";

		strGrowPointFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
		int iPos = strGrowPointFile.find('.');
		
		// 得到文件名字
		strGrowPointFile = strGrowPointFile.Left(iPos);
		// 通过映射表得到文件名字
		//strGrowPointFile = GetSceneName();

		strGrowPointFile = "\\" + strGrowPointFile;
		strGrowPointFile =  m_lastSceneDirectory + strGrowPointFile;
		strGrowPointFile += "_growpoint.txt";

		std::string strfile = strGrowPointFile;
		m_pGrowPointEditDlg->m_pGrowPointEditAction->ReadGrowPointFromFile(strfile);
		
	}
	return true;
}


// 存储
bool FairyEditorFrame::SaveGrowPoint()
{
	if ((m_pGrowPointEditDlg)&&(m_pGrowPointEditDlg->m_pGrowPointEditAction))
	{

		// 得到npc的文件， 然后读取数据
		int iLen = m_lastSceneDirectory.Length() + 1;
		wxString strGrowPointFile = "";

		strGrowPointFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
		int iPos = strGrowPointFile.find('.');
		
		//得到文件名字
		strGrowPointFile = strGrowPointFile.Left(iPos);
		// 通过映射表得到文件名字
		//strGrowPointFile = GetSceneName();

		strGrowPointFile = "\\" + strGrowPointFile;
		strGrowPointFile =  m_lastSceneDirectory + strGrowPointFile;
		strGrowPointFile += "_growpoint.txt";

		std::string strfile = strGrowPointFile;
		m_pGrowPointEditDlg->m_pGrowPointEditAction->SaveGrowPointToFile(strfile);
		
	}//



	//// 存储配置文件的目录
	//if ((m_pGrowPointEditDlg)&&(m_pGrowPointEditDlg->m_pGrowPointEditAction))
	//{

	//	// 得到npc的文件， 然后存储数据
	//	char szServerDataSavePath[512];
	//	memset(szServerDataSavePath, 0, sizeof(szServerDataSavePath));
	//	::GetPrivateProfileString("path", "pathname", "none", szServerDataSavePath, 512, ".\\server_data_save_path.ini");

	//	int iLen = m_lastSceneDirectory.Length() + 1;
	//	wxString strGrowPointFile = "";

	//	strGrowPointFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	//	int iPos = strGrowPointFile.find('.');
	//	
	//	//strGrowPointFile = strGrowPointFile.Left(iPos);
	//	// 通过映射表得到文件名字
	//	strGrowPointFile = GetSceneName();

	//	strGrowPointFile += "_growpoint.txt";

	//	std::string strfile = "";
	//	strfile = szServerDataSavePath;
	//	strfile += strGrowPointFile;

 //       m_pGrowPointEditDlg->m_pGrowPointEditAction->SaveGrowPointToFile(strfile);
	//}//




	return true;
}


//清空
void FairyEditorFrame::ClearGrowPoint()
{
	if (m_pGrowPointEditDlg)
	{
		m_pGrowPointEditDlg->ClearUI();
		if (m_pGrowPointEditDlg->m_pGrowPointEditAction)
		{
			m_pGrowPointEditDlg->m_pGrowPointEditAction->ClearGrowPointData();
		}
	}
}

//
// 生长点编辑
//
//////////////////////////////////////////////////////////////////////////////////////////////


void 
FairyEditorFrame::OnSelectAllObjects(wxCommandEvent& e)
{
	GetSceneManipulator()->setSelectAllObjects(!GetSceneManipulator()->getSelectAllObjects());
}

void 
FairyEditorFrame::OnUpdateSelectAllObjects(wxUpdateUIEvent& e)
{	
	e.Enable(GetSceneManipulator() != 0);
    e.Check(GetSceneManipulator() && GetSceneManipulator()->getSelectAllObjects());
}

void 
FairyEditorFrame::OnResetCamera(wxCommandEvent& e)
{
	if (GetSceneManipulator())
	{
		// 使摄像机恢复到初始的高度及角度，摄像机的xz位置是启用这个命令时摄像机的视线与地面的相交点
		Ogre::Vector3 position;

        Ogre::Ray cameraRay(GetSceneManipulator()->getCamera()->getPosition(), GetSceneManipulator()->getCamera()->getDirection());
        bool hit = GetSceneManipulator()->getTerrainIntersects(cameraRay, position);

		GetSceneManipulator()->setRealCameraHeight(GetSceneManipulator()->getDefaultRealCameraHeight());
		GetSceneManipulator()->setRealCameraAngle(GetSceneManipulator()->getDefaultRealCameraAngle());

		GetSceneManipulator()->setRealCamera(position.x, position.z);
	}
}

void 
FairyEditorFrame::OnEraseTextureByName(wxCommandEvent& e)
{	
    EraseTexDialog dlg(this, wxID_ANY,_("要删除的纹理名称"));
    dlg.Centre();
    if (dlg.ShowModal() == wxID_OK)
    {
        wxString prompt =/* _("The operations before can not be undo,Do you want to continue the operation?")*/wxT("这项操作不可以恢复，您确信要继续进行操作吗？");

        int res = wxMessageBox(prompt, /*_("Warning")*/wxT("警告"),
            wxYES_NO|wxICON_QUESTION,
            this);

        switch (res)
        {
        default:   
        case wxNO:
            return;

        case wxYES:
            {
                bool succeed = false;
                succeed = GetSceneManipulator()->eraseTerrainPixmapByTextureName(dlg.mComboBox->GetStringSelection().c_str());

                if (!succeed)
                {
                    wxMessageBox(_("The Texture you selected was not painted in this terrain."));
                }
            }
        }
    }
}

void 
FairyEditorFrame::OnEntireTerrainEnableDecal(wxCommandEvent& e)
{	
    WX::TerrainData* terrainData = GetSceneManipulator()->getTerrainData();

    for (WX::TerrainData::GridInfoArray::iterator it = terrainData->mGridInfos.begin(); it != terrainData->mGridInfos.end(); ++it)
    {
        unsigned int originFlags = (*it).flags;
        
        unsigned int flags = 0;

        if (originFlags & WX::TerrainData::GridInfo::FlipDiagonal)
            flags ^= WX::TerrainData::GridInfo::FlipDiagonal;

        (*it).flags = flags;
    }

    GetSceneManipulator()->getTerrain()->
        notifyGridInfoModified(0,0,GetSceneManipulator()->getTerrainData()->getXSize(),GetSceneManipulator()->getTerrainData()->getZSize());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 编辑npc巡逻
//

	// 读取不可行走区域
bool FairyEditorFrame::ReadNpcPatrol()
{
	int iLen = m_lastSceneDirectory.Length() + 1;

	std::string strRegionFile = "";
	strRegionFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	strRegionFile = strRegionFile.substr( 0, strRegionFile.find('.') );
	strRegionFile =  m_lastSceneDirectory + "\\" + strRegionFile + "_patrolpoint.ini";

	if (m_pNpcPatrolEditDlg)
	{
		m_pNpcPatrolEditDlg->LoadData(strRegionFile);
	}

	return true;
	
}

// 存储不可行走区域.
bool FairyEditorFrame::SaveNpcPatrol()
{
	{
		// 提取主文件名字。
		wxString strFile = "";
		wxString strRegionFile = "";

		int iLen = m_lastSceneDirectory.Length() + 1;
		strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
		int iPos = strFile.find('.');
		
		// 得到名字
		strFile = strFile.Left(iPos);
		// 通过映射表得到文件名字
		strFile = GetSceneName();

		strFile = "\\" + strFile;
		strFile =  m_lastSceneDirectory + strFile;

		// 组合不可行走区域文件名字。
		strRegionFile = strFile + ".Patrolpoint";

		//存储不可行走区域
		std::string strfile = strRegionFile;
		if (m_pNpcPatrolEditDlg)
		{
			m_pNpcPatrolEditDlg->SaveData(strfile);
		}
	}

	return true;
}

// 清空不可行走区域数据
void FairyEditorFrame::ClearNpcPatrol()
{
	// 不可走区域清空.
	if (m_pNpcPatrolEditDlg)
	{
		m_pNpcPatrolEditDlg->ClearData();
	}
}

//
// 编辑npc巡逻
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 寻路编辑
//

// 读取寻路区域
bool FairyEditorFrame::ReadPathFind()
{
	int iLen = m_lastSceneDirectory.Length() + 1;

	std::string strRegionFile = "";
	strRegionFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	strRegionFile = strRegionFile.substr( 0, strRegionFile.find('.') );
	strRegionFile =  m_lastSceneDirectory + "\\" + strRegionFile + ".Dijkstra";

	if (m_pPathFindEditDlg)
	{
		m_pPathFindEditDlg->LoadData(strRegionFile);
	}

	return true;

}

// 存储寻路点.
bool FairyEditorFrame::SavePathFind()
{
	{
		// 提取主文件名字。
		wxString strFile = "";
		wxString strRegionFile = "";

		int iLen = m_lastSceneDirectory.Length() + 1;
		strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
		int iPos = strFile.find('.');

		// 得到名字
		strFile = strFile.Left(iPos);
		// 通过映射表得到文件名字
		strFile = GetSceneName();

		strFile = "\\" + strFile;
		strFile =  m_lastSceneDirectory + strFile;

		// 组合不可行走区域文件名字。
		strRegionFile = strFile + ".Dijkstra";

		//存储不可行走区域
		std::string strfile = strRegionFile;
		if (m_pPathFindEditDlg)
		{
			m_pPathFindEditDlg->SaveData(strfile);
		}
	}

	return true;
}

// 清空不可行走区域数据
void FairyEditorFrame::ClearPathFind()
{
	 //不可走区域清空.
	if (m_pPathFindEditDlg)
	{
		m_pPathFindEditDlg->ClearData();
	}
}

//
// 寻路编辑
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////////////////////////
//
// 事件区域编辑
//


	// 读取不可行走区域
bool FairyEditorFrame::ReadEventArea()
{

	// 提取主文件名字。
	wxString strFile = "";
	wxString strSaveFile = "";

	int iLen = m_lastSceneDirectory.Length() + 1;
	strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	int iPos = strFile.find('.');
	
	//strFile = strFile.Left(iPos);
	strFile = GetSceneName();

	strFile = "\\" + strFile;
	strFile =  m_lastSceneDirectory + strFile;

	// 组合不可行走区域文件名字。
	strSaveFile = strFile + ".area";

	//存储不可行走区域
	std::string strfile = strSaveFile;
	if (m_pEventAreaEditDlg)
	{
		m_pEventAreaEditDlg->LoadData(strfile);

		return true;

	}

	return false;
}

// 存储不可行走区域.
bool FairyEditorFrame::SaveEventArea()
{
	{
		// 提取主文件名字。
		wxString strFile = "";
		wxString strSaveFile = "";

		int iLen = m_lastSceneDirectory.Length() + 1;
		strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
		int iPos = strFile.find('.');
		
		//strFile = strFile.Left(iPos);
		strFile = GetSceneName();


		strFile = "\\" + strFile;
		strFile =  m_lastSceneDirectory + strFile;

		// 组合不可行走区域文件名字。
		strSaveFile = strFile + ".area";

		//存储不可行走区域
		std::string strfile = strSaveFile;
		if (m_pEventAreaEditDlg)
		{
			m_pEventAreaEditDlg->SaveData(strfile);
	
		}
	}

	//// 存储配置文件的目录
	//{
	//	// 提取主文件名字 然后存储数据
	//	char szServerDataSavePath[512];
	//	memset(szServerDataSavePath, 0, sizeof(szServerDataSavePath));
	//	::GetPrivateProfileString("path", "pathname", "none", szServerDataSavePath, 512, ".\\server_data_save_path.ini");

	//	int iLen = m_lastSceneDirectory.Length() + 1;
	//	wxString strFile = "";

	//	strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	//	int iPos = strFile.find('.');
	//	
	//	//strFile = strFile.Left(iPos);
	//	strFile = GetSceneName();

	//	strFile += ".area";
	//	std::string strSavefile = "";
	//	strSavefile = szServerDataSavePath;
	//	strSavefile += strFile;
	//
	//	if (m_pEventAreaEditDlg)
	//	{
	//		m_pEventAreaEditDlg->SaveData(strSavefile);

	//	}
	//}//

	return true;

}

//清空npc
void FairyEditorFrame::ClearEventArea()
{
	if (m_pEventAreaEditDlg)
	{
		m_pEventAreaEditDlg->ClearUI();
		if (m_pEventAreaEditDlg->m_pEditAction)
		{
			m_pEventAreaEditDlg->ClearData();
		}

	}
}

void FairyEditorFrame::ClearDatuPointInfo()
{
    if (mDatuPointEditDialog)
    {
        mDatuPointEditDialog->ClearDatuPointInfo();
    }
}

//
//  事件区域编辑
//
///////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 摊位信息
//

//创建摊位信息
void FairyEditorFrame::CreateStallInfo()
{
	if (m_pStallInfoDlg)
	{
		m_pStallInfoDlg->CreateStallInfo();
	}
}

// 读取摊位信息
bool FairyEditorFrame::ReadStallInfo()
{
	// 提取主文件名字。
	wxString strFile = "";
	wxString strSaveFile = "";

	int iLen = m_lastSceneDirectory.Length() + 1;
	strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	int iPos = strFile.find('.');
	
	//strFile = strFile.Left(iPos);
	strFile = GetSceneName();

	strFile = "\\" + strFile;
	strFile =  m_lastSceneDirectory + strFile;

	// 组合不可行走区域文件名字。
	strSaveFile = strFile + ".area";

	//存储不可行走区域
	std::string strfile = strSaveFile;
	if (m_pStallInfoDlg)
	{
		m_pStallInfoDlg->LoadData(strfile);
	}

	return true;
}

// 存储摊位信息
bool FairyEditorFrame::SaveStallInfo()
{
	// 提取主文件名字。
	wxString strFile = "";
	wxString strSaveFile = "";

	int iLen = m_lastSceneDirectory.Length() + 1;
	strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	int iPos = strFile.find('.');
	
	//strFile = strFile.Left(iPos);
	strFile = GetSceneName();


	strFile = "\\" + strFile;
	strFile =  m_lastSceneDirectory + strFile;

	// 组合不可行走区域文件名字。
	strSaveFile = strFile + ".stallInfo";

	//存储不可行走区域
	std::string strfile = strSaveFile;
	if (m_pStallInfoDlg)
	{
		m_pStallInfoDlg->SaveData(strfile);
	}

	return true;
}

//
// 摊位信息
//
//////////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 声音信息
//

//清空声音
void FairyEditorFrame::ClearSoundInfo()
{
	if(mSoundEditDialog)
	{
		mSoundEditDialog->ClearSoundData();
	}//
}

// 读取摊位信息
bool FairyEditorFrame::ReadSoundInfo()
{
	// 提取主文件名字。
	wxString strFile = "";
	wxString strSaveFile = "";

	int iLen = m_lastSceneDirectory.Length() + 1;
	strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	int iPos = strFile.find('.');
	
	//strFile = strFile.Left(iPos);
	strFile = GetSceneName();

	strFile = "\\" + strFile;
	strFile =  m_lastSceneDirectory + strFile;

	// 组合不可行走区域文件名字。
	strSaveFile = strFile + ".esd";

	//存储不可行走区域
	std::string strfile = strSaveFile;
	if(mSoundEditDialog)
	{
		mSoundEditDialog->LoadSoundInfoFromFile(strfile);
	}//

	return true;
}

bool FairyEditorFrame::ReadDatuPointInfo()
{
    // 提取主文件名字。
    wxString strFile = "";
    wxString strSaveFile = "";

    int iLen = m_lastSceneDirectory.Length() + 1;
    strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
    int iPos = strFile.find('.');

    //strFile = strFile.Left(iPos);
    strFile = GetSceneName();

    strFile = "\\" + strFile;
    strFile =  m_lastSceneDirectory + strFile;

    // 组合不可行走区域文件名字。
    strSaveFile = strFile + ".datu";

    //存储不可行走区域
    std::string strfile = strSaveFile;
    if(mDatuPointEditDialog)
    {
        mDatuPointEditDialog->LoadDatuPointsFromFile(strfile);
    }//

    return true;
}

// 存储摊位信息
bool FairyEditorFrame::SaveSoundInfo()
{
	// 提取主文件名字。
	wxString strFile = "";
	wxString strSaveFile = "";

	int iLen = m_lastSceneDirectory.Length() + 1;
	strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	int iPos = strFile.find('.');
	
	//strFile = strFile.Left(iPos);
	strFile = GetSceneName();


	strFile = "\\" + strFile;
	strFile =  m_lastSceneDirectory + strFile;

	// 组合不可行走区域文件名字。
	strSaveFile = strFile + ".esd";

	//存储不可行走区域
	std::string strfile = strSaveFile;
	if(mSoundEditDialog)
	{
		mSoundEditDialog->SaveSoundInfoFromFile(strfile);
	}//

	return true;
}

bool FairyEditorFrame::SaveDatuPointInfo()
{
    // 提取主文件名字。
    wxString strFile = "";
    wxString strSaveFile = "";

    int iLen = m_lastSceneDirectory.Length() + 1;
    strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
    int iPos = strFile.find('.');

    //strFile = strFile.Left(iPos);
    strFile = GetSceneName();


    strFile = "\\" + strFile;
    strFile =  m_lastSceneDirectory + strFile;

    // 组合不可行走区域文件名字。
    strSaveFile = strFile + ".datu";

    //存储不可行走区域
    std::string strfile = strSaveFile;
    if(mDatuPointEditDialog)
    {
        mDatuPointEditDialog->SaveDatuPointsToFile(strfile);
    }//

    return true;
}
//
// 声音信息
//
//////////////////////////////////////////////////////////////////////////////////////////////////////




// 地图跳转
void FairyEditorFrame::OnSceneGoto(wxCommandEvent &event)
{
	if( !m_pToolbar ) return;

	float fX = 0;
	float fZ = 0;

	if (mCanvas)
	{
		wxTextCtrl* pTexX = (wxTextCtrl*)m_pToolbar->FindControl(feID_GOTO_X);
		wxTextCtrl* pTexY = (wxTextCtrl*)m_pToolbar->FindControl(feID_GOTO_Y);

		if ((pTexX)&&(pTexY))
		{
			fX = atof(pTexX->GetValue().c_str());
			fZ = atof(pTexY->GetValue().c_str());
			mCanvas->GotoScene(fX, fZ);
		}
	}
}

//得到地图名字
std::string FairyEditorFrame::GetSceneName()
{

	std::map<std::string, std::string> strNameMap;
	// 提取主文件名字 然后存储数据
	char szInfo[512];
	char szName1[128];
	char szName2[128];
	
	strNameMap.clear();
	FILE* pFile = NULL;
	pFile = ::fopen(".\\地图名映射表.txt", "r");
	if (pFile)
	{
		while(!feof(pFile))
		{
			memset(szInfo,  0,  sizeof(szInfo));
			memset(szName1, 0, sizeof(szName1));
			memset(szName2, 0, sizeof(szName2));

			::fgets(szInfo, sizeof(szInfo), pFile);
			sscanf(szInfo, "%s	%s", szName1, szName2);
			strNameMap[szName1] = szName2;
	
		}

		::fclose(pFile);
	}

	int iLen = m_lastSceneDirectory.Length() + 1;
	wxString strFile = "";

	strFile = m_sceneFilename.Right(m_sceneFilename.Length() - iLen);
	int iPos = strFile.find('.');
	strFile = strFile.Left(iPos);

	std::string strReturn = strFile.c_str();
	std::string strKey    = strReturn;
	if (strNameMap.count(strKey))
	{
		strReturn = strNameMap[strKey];
	}

	return strReturn;

}

void FairyEditorFrame::OnCreateMiniMap(wxCommandEvent& e)
{
    GetSceneManipulator()->setIndicatorNodeVisible(false);

    MiniMapMakerDialog dlg(this, wxID_ANY, _("小地图制作器"),
        GetSceneManipulator(), m_sceneFilename);
    dlg.ShowModal();   

    GetSceneManipulator()->setIndicatorNodeVisible(true);
}