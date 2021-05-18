#ifndef __FAIRYEDITORFRAME_H__
#define __FAIRYEDITORFRAME_H__

#include <wx/frame.h>

#include <OgreVector3.h>
#include <OgreString.h>

#include "TriInfoInMap.h"
// ----------------------------------------------------------------------------
// forward declares
// ----------------------------------------------------------------------------

class FairySceneCanvas;
class BrushSelector;
class ModelSelector;
class ActorSelector;
class ParticleSystemSelector;
class SceneBrowser;
class ObjectPropertyEditor;

class wxLayoutManager;
struct wxHostInfo;

class wxConfigBase;
class wxSpinEvent;
class wxListEvent;
class SceneInfoMapDialog;

// 不可行走区域编辑对话框
class CRegionEditDlgSector;

// 怪物编辑对话框
class CMonsterEditDlg;

// 生长点编辑
class CGrowPointEditDlg;

// npc巡逻编辑
class CNpcPatrolEditDlg;

// 事件区域编辑
class CEventAreaEditDlg;

// 摊位信息
class CStallInfoDlg;

// 声音编辑
class SoundEditDialog;

// 寻路编辑
class CPathFindEditDlg;

class DatuPointEditDialog;

class AutoSaveSceneController;
class CameraTrackSettingDialog;

namespace WX {
    class SceneManipulator;
}

class GpuProgramParameterSettingDialog;
class SceneInfomationDialog;
// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

// Define a new frame
class FairyEditorFrame : public wxFrame
{
    DECLARE_CLASS(FairyEditorFrame)
    WX_DECLARE_CONTROL_CONTAINER();

public:

	// 不可行走区域编辑对话框.
	CRegionEditDlgSector* m_pRegionEditSelector;
	
	// 怪物编辑对话框。
	CMonsterEditDlg*	  m_pMonsterEditDlg;

	// 生长点编辑对话框。
	CGrowPointEditDlg*	  m_pGrowPointEditDlg;

	// npc巡逻路线编辑
	CNpcPatrolEditDlg*    m_pNpcPatrolEditDlg;

	// 事件区域
	CEventAreaEditDlg*	  m_pEventAreaEditDlg;

	// 摊位信息
	CStallInfoDlg*		  m_pStallInfoDlg;

	// 声音编辑			  
    SoundEditDialog*      mSoundEditDialog;

	// 寻路编辑
	CPathFindEditDlg*    m_pPathFindEditDlg;

	DatuPointEditDialog*  mDatuPointEditDialog;
	// 工具栏
	wxToolBar*		m_pToolbar;
	
	// 保存场景管理器
	WX::SceneManipulator* m_pSceneManipulator;

	// 设置当前的场景逻辑坐标
	void SetCurSceneLogicPos(float fx, float fz);

	// 地图跳转
	void OnSceneGoto(wxCommandEvent &event);


protected:
    FairySceneCanvas* mCanvas;

    BrushSelector* mBrushSelector;
    ModelSelector* mModelSelector;
    ActorSelector* mActorSelector;
    ParticleSystemSelector* mParticleSystemSelector;
    SceneBrowser* mSceneBrowser;
    ObjectPropertyEditor* mObjectPropertyEditor;
    SceneInfoMapDialog* mSceneInfoDlg;
    //CameraTrackSettingDialog* mCameraTrackSettingDialog;

    wxTextCtrl* mLogWindow;

    wxLayoutManager* mLayout;
    wxMenu* mWindowMenu;

    bool mLockBrushSize;
	bool mIsTerrainLightmapUpdated;

    wxString m_lastTitle;
    wxString m_lastSceneDirectory;
    wxString m_sceneFilename;

    int mSpinnerValue;
    wxString mByAxis;
    int mSegment;
    bool mActiveSnapRotation;

	Ogre::String mInfoString;
	Ogre::String mManipInfoString;

	bool mUseRealCamera;
	bool mUseSnapRotation;

    int mTextureFilterOptions;

    int mCreateLevelOptions;

	GpuProgramParameterSettingDialog *mGpuProgramParameterSettingDialog;
    SceneInfomationDialog* mSceneInfomationDialog;

    AutoSaveSceneController* mAutoSaveSceneController;

protected:
    void CreateDockWindow(wxWindow* dockClient, const wxString& title, const wxString& name, wxHostInfo& hostInfo);
    void InitMenuBar(void);
    void InitToolBar(wxToolBar* toolbar);
    void InitStatusBar(wxStatusBar* statusbar);

public:
    FairyEditorFrame(wxFrame *parent, wxWindowID id, const wxString& title,
        const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxFrameNameStr);

    ~FairyEditorFrame();

    FairySceneCanvas* GetCanvas(void) const
    {
        return mCanvas;
    }
    SceneBrowser* GetSceneBrowser(void) const
    {
        return mSceneBrowser;
    }

    WX::SceneManipulator* GetSceneManipulator(void) const;

    void InitScene(WX::SceneManipulator* sceneManipulator);

	void SetKeyboardShortcuts(void);
    //////////////////////////////////////////////////////////////////////////

    void LoadSettings(wxConfigBase* cfg);
    void SaveSettings(wxConfigBase* cfg) const;

    bool Save(void);
    bool SaveAs(void);
    bool SaveModified(bool force = false);

    bool SaveImpl(bool autoSave = false);

    //////////////////////////////////////////////////////////////////////////
    bool ProcessEvent(wxEvent& e);

    void SetSpinnerValue(int value);
    int GetSpinnerValue();

    wxString GetByAxis();
    void SetByAxis(wxString& strAxis);

    void SetSnapRotationValue(int value);
    int GetSnapRotationValue();

    void SetActiveSnapRotation(bool value);
    bool GetActiveSnapRotation();

    AutoSaveSceneController* getAutoSaveController(void)
    {
        return mAutoSaveSceneController;
    }

    // 获取当前选中的地形，构成字符串，放在clipboard中
    //void GetSelectedTerrainInfo( Ogre::String &infoString );

    DECLARE_EVENT_TABLE()

    void OnUpdateWindow(wxUpdateUIEvent& e);
    void OnCloseWindow(wxCloseEvent& e);

    void OnFileNew(wxCommandEvent& e);
    void OnFileOpen(wxCommandEvent& e);
    void OnFileSave(wxCommandEvent& e);
    void OnFileSaveAs(wxCommandEvent& e);

    void OnUpdateFileNew(wxUpdateUIEvent& e);
    void OnUpdateSave(wxUpdateUIEvent& e);
    void OnUpdateSaveAs(wxUpdateUIEvent& e);

    void OnUndo(wxCommandEvent& e);
    void OnRedo(wxCommandEvent& e);
    void OnUpdateUndo(wxUpdateUIEvent& e);
    void OnUpdateRedo(wxUpdateUIEvent& e);

    void OnLockCameraHeight(wxCommandEvent& e);
    void OnUpdateLockCameraHeight(wxUpdateUIEvent& e);

	void OnSelectAllObjects(wxCommandEvent& e);
    void OnUpdateSelectAllObjects(wxUpdateUIEvent& e);

    void OnLockBrushSize(wxCommandEvent& e);
    void OnUpdateLockBrushSize(wxUpdateUIEvent& e);
    void OnBrushSizeChanged(wxSpinEvent& e);
    void OnBrushSizeTextChanged(wxCommandEvent& e);
    void OnUpdateBrushSizeSpin(wxUpdateUIEvent& e);

    void OnTerrainHeightChanged(wxSpinEvent& e);
    void OnTerrainHeightTextChanged(wxCommandEvent& e);
    void OnUpdateTerrainHeightSpin(wxUpdateUIEvent& e);

	void OnTerrainHeightIncrementChanged(wxSpinEvent& e);
	void OnTerrainHeightIncrementTextChanged(wxCommandEvent& e);
	void OnUpdateTerrainHeightIncrementSpin(wxUpdateUIEvent& e);

    void OnHeightActionaffectRangeChanged(wxSpinEvent& e);
    void OnHeightActionaffectRangeTextChanged(wxCommandEvent& e);
    void OnUpdateHeightActionaffectRangeSpin(wxUpdateUIEvent& e);

	void OnHeightActionExpValueTextChanged(wxCommandEvent& e);
	void OnUpdateHeightActionExpValue(wxUpdateUIEvent& e);

	void OnResetCamera(wxCommandEvent& e);

	// hack 临时设置泛光效果
	void OnBlurAmountTextChanged(wxCommandEvent& e);
	void OnShineAmountTextChanged(wxCommandEvent& e);
	void OnUpdateFloodlightingControls(wxUpdateUIEvent& e);

	void OnHeightAdjustSpeedTextChanged(wxCommandEvent& e);
	void OnUpdateHeightAdjustSpeed(wxUpdateUIEvent& e);

    void OnShowStatPanel(wxCommandEvent& e);
    void OnUpdateShowStatPanel(wxUpdateUIEvent& e);
    void OnShowLogoPanel(wxCommandEvent& e);
    void OnUpdateShowLogoPanel(wxUpdateUIEvent& e);

    void OnToggleToolWindows(wxCommandEvent& e);
    void OnUpdateToggleToolWindows(wxUpdateUIEvent& e);
    void OnFullScreen(wxCommandEvent& e);
    void OnUpdateFullScreen(wxUpdateUIEvent& e);

    void OnDeleteStaticObject(wxCommandEvent& e);
    void OnUpdateDeleteStaticObject(wxUpdateUIEvent& e);

    void OnSaveTerrainInfoToFile(wxCommandEvent& e);
    void OnUpdateSaveTerrainInfoToFile(wxUpdateUIEvent& e);
    void OnLoadTerrainInfoFromFile(wxCommandEvent& e);
    void OnUpdateLoadTerrainInfoFromFile(wxUpdateUIEvent& e);

	void OnSaveManipInfoToFile(wxCommandEvent& e);
	void OnUpdateSaveManipInfoToFile(wxUpdateUIEvent& e);
	void OnLoadManipInfoFromFile(wxCommandEvent& e);
	void OnUpdateLoadManipInfoFromFile(wxUpdateUIEvent& e);

	void OnSaveEnviromentInfoToFile(wxCommandEvent& e);
	void OnUpdateSaveEnviromentInfoToFile(wxUpdateUIEvent& e);
	void OnLoadEnviromentInfoFromFile(wxCommandEvent& e);
	void OnUpdateLoadEnviromentInfoFromFile(wxUpdateUIEvent& e);

    void OnShowStandardModel(wxCommandEvent& e);
    void OnUpdateShowStandardModel(wxUpdateUIEvent& e);

    void OnSelectAction(wxCommandEvent& e);
    void OnUpdateSelectAction(wxUpdateUIEvent& e);

	void OnTerrainLightmapOption(wxCommandEvent& e);
	void OnTerrainLightmapHide(wxCommandEvent& e);
	void OnUpdateTerrainLightmapHide(wxUpdateUIEvent& e);
	void OnTerrainLightmapLowQuality(wxCommandEvent& e);
	void OnUpdateTerrainLightmapLowQuality(wxUpdateUIEvent& e);
	void OnTerrainLightmapHighQuality(wxCommandEvent& e);
	void OnUpdateTerrainLightmapHighQuality(wxUpdateUIEvent& e);

    void OnTerrainSurfaceParameters(wxCommandEvent& e);
    void OnSceneAnimationQuality(wxCommandEvent& e);
    void OnObjectCreateLevel(wxCommandEvent& e);

    void OnCreateMiniMap(wxCommandEvent& e);

	void OnSelectBrushShape(wxCommandEvent& e);
	void OnUpdateSelectBrushShape(wxUpdateUIEvent& e);

    void OnAlignPosition(wxCommandEvent& e);
    void OnUpdateAlignPosition(wxUpdateUIEvent& e);

    void OnExit(wxCommandEvent& e);

    //void OnCameraSetting(wxCommandEvent& e);
    void OnPreferSetting(wxCommandEvent& e);

	void OnGpuProgramParameterSetting(wxCommandEvent& e);

	void OnEraseTextureByName(wxCommandEvent& e);

    void OnEntireTerrainEnableDecal(wxCommandEvent& e);

    void OnCut(wxCommandEvent& event);
    void OnCopy(wxCommandEvent& event);
    void OnPaste(wxCommandEvent& event);

	void OnCreateLiquid(wxCommandEvent& e);
	void OnCreateLight(wxCommandEvent& e);

    void OnUpdateCutCopyPaste(wxUpdateUIEvent& e);

	void OnReshapeTerrain(wxCommandEvent& e);
	void OnUpdateReshapeTerrain(wxUpdateUIEvent& e);

	void OnEnable32BitTextures(wxCommandEvent& e);
	void OnUpdateEnable32BitTextures(wxUpdateUIEvent& e);

	void OnReloadTextures(wxCommandEvent& e);
	void OnUpdateReloadTextures(wxUpdateUIEvent& e);

	void OnUnloadUselessResources(wxCommandEvent& e);
	void OnUpdateUnloadUselessResources(wxUpdateUIEvent& e);

    void OnShowAllIndicators(wxCommandEvent& e);
    void OnUpdateShowAllIndicators(wxUpdateUIEvent& e);

    void OnSelectPostFilter(wxCommandEvent& e);
    void OnUpdateSelectPostFilter(wxUpdateUIEvent& e);

    void OnSelectShadowTechnique(wxCommandEvent& e);
    void OnUpdateSelectShadowTechnique(wxUpdateUIEvent& e);

    void OnSelectDefaultTextureFiltering(wxCommandEvent& e);
    void OnUpdateSelectDefaultTextureFiltering(wxUpdateUIEvent& e);

    void OnSelectSceneCreateLevel(wxCommandEvent& e);

    void OnUpdateSelectSceneCreateLevel(wxUpdateUIEvent& e);

	void OnUseRealCamera(wxCommandEvent& e);
	void OnUpdateUseRealCamera(wxUpdateUIEvent& e);

	void OnUseSnapRotation(wxCommandEvent& e);
	void OnUpdateUseSnapRotation(wxUpdateUIEvent& e);

	void OnFrameRateTextChanged(wxCommandEvent& e);

    // 获取当前选中的地形，构成字符串，放在clipboard中
    void GetSelectedTerrainInfo( Ogre::String &infoString );

    void OnSceneInfomation(wxCommandEvent& e);

	///////////////////////////////////////////////////////////////////////////////////
	//
	// 建筑物可行走面数据
	//
public:


	//
	// 建筑物可行走面信息库.
	//
	typedef std::map<std::string, TRI_INFO_VECTOR> BUILDING_COLLISION_MAP;
	BUILDING_COLLISION_MAP	m_buildingCollisionInfoLibMap;

	// 注册在地图中的三角形映射表。
	CTriInfoInMap	m_TriInMapInfoMap;



	//
	// 得到建筑物可行走面的信息库
	// 
	// return 0 -- 成功。非 0 -- 失败 具体错误信息待定.
	//
	DWORD GetBuildingCollisionInfo();


	//
	// 把注册到地图中的行走面信息存储到文件。
	// 文件的名字与场景文件的名字相同， 扩展名字为bcol.
	//
	// 参数：文件名字， 不带扩展名， 带路径信息。
	void SaveBuildingCollisionInfo(wxString strCollisionFile);


	//// 通过物体的指针得到物体模型的名字。
	////
	//// 参数：静态物体模型的名字.
	//// 
	//// return 物体的mesh文件名字.
	//wxString GetMeshNameByObjectPtr(const WX::ObjectPtr& object);//

	//
	// 创建建筑物的行走面数据。
	//
	void CreateBuildingCollisionData();


	//
	// 根据输入的平移， 旋转， 缩放分量创建出位置变换矩阵.
	//
	void BuildTransformMatrix(Ogre::Matrix4& Matrix,  const Ogre::Vector3& position, const Ogre::Quaternion rotate, const  Ogre::Vector3 scale);


	//
	// 根据建筑物的名字把建筑物的行走面注册到地图中.
	//
	// 参数1 const std::string& strMeshName － 建筑物的名字。
	// 
	// 参数2 Ogre::Matrix4& TransformMatrix -  建筑物在地图中的变换矩阵.
	void RegisterCollisionToMap(const std::string& strMeshName, Ogre::Matrix4& TransformMatrix);


	//
	// 从建筑物的行走面数据文件中读取三角形的信息。
	//
	// 参数1： std::string strFileName 建筑物行走面的数据文件。
	//
	// 参数2： TRI_INFO_VECTOR &TriInfoVector 保存三角形的数组.
	//
	// return： 0 - 成功。 非0 失败。具体含义根据需要待定。
	DWORD LoadCollisionDataFromFile(std::string strFileName, TRI_INFO_VECTOR &TriInfoVector);


	//
	// 把三角形信息写到地图的三角形与位置的映射表中。
	//
	// 参数1： 三角形列表。
	// 
	// 参数2： 三角形的变换矩阵。
	//
	void WriteCollisionDataToTriMap(TRI_INFO_VECTOR &TriInfoVector, Ogre::Matrix4& TransformMatrix);


	// 
	// 使用矩形与三角形相交方法把三角形信息写到地图的三角形与位置的映射表中。
	//
	// 参数1： 三角形列表。
	// 
	// 参数2： 三角形的变换矩阵。
	//
	void WriteCollisionDataToTriMap_Rectangle(TRI_INFO_VECTOR &TriInfoVector, Ogre::Matrix4& TransformMatrix);


	
	// 
	// 矩形与三角形是否相交。
	//
	// 参数1： 三角形点1。
	// 
	// 参数2： 三角形点2。
	//
	// 参数3： 三角形点3。
	//
	// 参数1： 矩形点1。
	// 
	// 参数2： 矩形点2。
	//
	// 参数3： 矩形点3。
	//
	// 参数4： 矩形点4。
	//
	// 返回值：1 相交， 0 不相交。
	// 
	bool Is_Rectangle_Triangle_Intersect(
										  POINT_3D triP1, 
										  POINT_3D triP2,
										  POINT_3D triP3,
										  POINT_3D recP1,
										  POINT_3D recP2,
										  POINT_3D recP3,
										  POINT_3D recP4
										  );


	// 
	// 判断点是否在三角形内部
	//
	// 参数1： 点的坐标
	//
	// 参数1： 三角形点1。
	// 
	// 参数2： 三角形点2。
	//
	// 参数3： 三角形点3。
	//
	// 返回值：1 在内部， 0 不在内部。
	// 
	bool IsPointInTriangle(
							POINT_3D point, 
							POINT_3D triP1,
							POINT_3D triP2,
							POINT_3D triP3
							);


	// 判断两条直线是否相交.
	//
	// 参数1： 第1条直线的开始点。
	// 参数2： 第1条直线的结束点。
    // 参数3： 第2条直线的开始点。
	// 参数4： 第2条直线的结束点。
	// 
	bool CheckCross(POINT_3D fvMS, POINT_3D fvME, POINT_3D fvNS, POINT_3D fvNE);




	//
	// 
	// 把场景坐标转换成游戏逻辑坐标。
	//
	void TranslateSceneToGameLogic(float &x, float &y, float &z, Ogre::Matrix4& TransformMatrix);


	// 求两个向量的行列式的值
	float fvMulti(POINT_2D_FLOAT fv1, POINT_2D_FLOAT fv2);

	// 求两个向量的减法
	POINT_2D_FLOAT fvSub(POINT_2D_FLOAT fv1, POINT_2D_FLOAT fv2);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	存储时用到的操作.
	//  

	//得到地图名字, 如果有映射名字就读取映射名字
	std::string GetSceneName();
	


	///////////////////////////////////////////////////////////////////////////////////
	//
	// 不可行走区域
	//

	// 存储建筑物行走面.
	bool SaveBuildingWalk();

	// 清空行走面数据
	void ClearBuildingWalk();
	

	//
	// 建筑物可行走面数据
	//
	///////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////
	//
	// 不可行走区域
	//
	
	public:

		// 读取不可行走区域
		bool ReadRegion();

		// 存储不可行走区域.
		bool SaveRegion();

		// 清空不可行走区域数据
		void ClearRegion();
	
	//
	// 不可行走区域
	//
	///////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////
	//
	// 不npc巡逻区域
	//
	
	public:

		// 读取不可行走区域
		bool ReadNpcPatrol();

		// 存储不可行走区域.
		bool SaveNpcPatrol();

		// 清空不可行走区域数据
		void ClearNpcPatrol();
	
	//
	// 不npc巡逻区域
	//
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	//
	// 寻路编辑
	//
	
	public:

		// 读取寻路点
		bool ReadPathFind();

		// 存储寻路点
		bool SavePathFind();

		// 清空寻路点
		void ClearPathFind();
	
	//
	// 寻路编辑
	//
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	//
	// npc编辑
	//
	
	public:

		// 读取不可行走区域
		bool ReadNpc();

		// 存储不可行走区域.
		bool SaveNpc();

		//清空npc
		void ClearNpc();
	
	//
	//  npc编辑
	//
	///////////////////////////////////////////////////////////////////////////////////


	
	///////////////////////////////////////////////////////////////////////////////////
	//
	// 生长点编辑
	//
	
	public:

		// 读取不可行走区域
		bool ReadGrowPoint();

		// 存储不可行走区域.
		bool SaveGrowPoint();

		//清空npc
		void ClearGrowPoint();
	

	//  生长点编辑
	//
	///////////////////////////////////////////////////////////////////////////////////


		
	///////////////////////////////////////////////////////////////////////////////////
	//
	// 事件区域编辑
	//
	
	public:

		// 读取不可行走区域
		bool ReadEventArea();

		// 存储不可行走区域.
		bool SaveEventArea();

		//清空npc
		void ClearEventArea();
	
	//
	//  事件区域编辑
	//
	///////////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////////
	//
	// 摊位信息
	//
		
		//创建摊位信息
		void CreateStallInfo();

		// 读取摊位信息
		bool ReadStallInfo();

		// 存储摊位信息
		bool SaveStallInfo();


	//
	// 摊位信息
	//
	//////////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////////
	//
	// 声音信息
	//
		//清空声音
		void ClearSoundInfo();


		// 读取摊位信息
		bool ReadSoundInfo();

		// 存储摊位信息
		bool SaveSoundInfo();

        bool ReadDatuPointInfo();
        bool SaveDatuPointInfo();
        void ClearDatuPointInfo();
};

#endif // 
