#ifndef __GpuProgramParameterSettingDialog_H__
#define __GpuProgramParameterSettingDialog_H__

#include <wx/dialog.h>
#include <wx/treebase.h>

#include <OgreMaterial.h>

class wxTreeCtrl;

class wxPropertyGridManager;
class wxPropertyGridEvent;
class wxPGProperty;

class GpuProgramParameterSettingDialog : public wxDialog
{
	typedef std::map< Ogre::String, wxTreeItemId> CatalogMap;

	typedef std::list<Ogre::String> MaterialNames;

	struct MaterialFileInfo
	{
		MaterialNames nameList;
		Ogre::String pathName;
	};

	typedef std::map<Ogre::String, MaterialFileInfo> MaterialFilesMap;

	DECLARE_DYNAMIC_CLASS(GpuProgramParameterSettingDialog)

public:
	GpuProgramParameterSettingDialog() {}

	// full ctor
	GpuProgramParameterSettingDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE | wxFULL_REPAINT_ON_RESIZE,
		const wxString& name = wxDialogNameStr)
		: mMaterialTreeCtrl(NULL), mPropertyGridManager(NULL), mInit(false),
		wxDialog(parent, id, title, pos, size, style, name)
	{
		mCurrentMaterial.setNull();
	}

public:


protected:

	void InitMaterialTreeCtrl(void);

	wxPGProperty * CreateProperty(const Ogre::String &paraName, Ogre::String &paraValue);

	wxTreeItemId  GetParentId( Ogre::String &catalog, CatalogMap &map );

	void InitMaterialFilesMap(void);

	void dealWithSpecialParameterOnChanged( const Ogre::String &name, Ogre::String &value );
	void dealWithSpecialParameterOnCreated( const Ogre::String &name, Ogre::String &value );

	DECLARE_EVENT_TABLE()
	void OnInitDialog(wxInitDialogEvent &e);
//	void OnComboMaterial(wxCommandEvent &e);
	void OnMaterialSelected(wxTreeEvent& e);

	void OnPropertyChanged(wxPropertyGridEvent& e);
	void OnApplyOtherMaterialButtonDown(wxCommandEvent &e);

	void OnClose(wxCloseEvent& event);

	void OnSaveMaterial(wxCommandEvent &e);

protected:

	wxTreeCtrl *mMaterialTreeCtrl;

	wxPropertyGridManager* mPropertyGridManager;

	Ogre::MaterialPtr mCurrentMaterial;

	wxArrayString mChangedMaterialNames;

	MaterialFilesMap mMaterialFilesMap;

	bool mInit;
};

#endif // __GpuProgramParameterSettingDialog_H__