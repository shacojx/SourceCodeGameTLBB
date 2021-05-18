// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "ModelSelector.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#endif

#include <wx/treectrl.h>

#include "ModelSelector.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------

#include <OgreCodec.h>
#include <OgreResourceGroupManager.h>

// ----------------------------------------------------------------------------

#include "SceneManipulator.h"
#include "Action.h"

// ----------------------------------------------------------------------------
// ModelSelector class
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

static const int feID_MODEL_LIST = wxNewId();

BEGIN_EVENT_TABLE(ModelSelector, wxPanel)
    EVT_UPDATE_UI(feID_MODEL_LIST, ModelSelector::OnUpdateControls)

    EVT_TREE_SEL_CHANGED(feID_MODEL_LIST, ModelSelector::OnModelListSelChanged)
    EVT_CHILD_FOCUS(ModelSelector::OnChildFocus)
END_EVENT_TABLE()

namespace {

    class ModelItemData : public wxTreeItemData
    {
    public:
        ModelItemData(const WX::String& name, const WX::String& typeName, const WX::String& parameterName)
            : name(name)
            , typeName(typeName)
            , parameterName(parameterName)
        {
        }

        WX::String name;
        WX::String typeName;
        WX::String parameterName;
    };
}


ModelSelector::~ModelSelector()
{
    if (mModelList)
    {
        mModelList->Freeze();
        mModelList->DeleteAllItems();
        mModelList->Thaw();
    }
}

void
ModelSelector::Init(const wxString& resourceGroup)
{
    mSceneManipulator = NULL;
    mModelList = NULL;

    mResourceGroup = resourceGroup;
}

void
ModelSelector::SetSceneManipulator(WX::SceneManipulator* sceneManipulator)
{
    mSceneManipulator = sceneManipulator;
}

bool
ModelSelector::Create(wxWindow *parent, wxWindowID winid,
                      const wxPoint& pos, const wxSize& size,
                      long style, const wxString& name)
{
    if (!wxPanel::Create(parent, winid, pos, size, style, name))
        return false;

    if (!CreateControls())
        return false;

    Reload();

    return true;
}

void
ModelSelector::Reload(const wxString& resourceGroup)
{
    mResourceGroup = resourceGroup;
    Reload();
}

bool
ModelSelector::CreateControls(void)
{
    wxFlexGridSizer* sizer = new wxFlexGridSizer(1);
    sizer->AddGrowableCol(0);
    sizer->AddGrowableRow(0);

    mModelList = new wxTreeCtrl(this, feID_MODEL_LIST,
        wxDefaultPosition, wxDefaultSize,
        wxTR_DEFAULT_STYLE | wxTR_SINGLE | wxTR_HIDE_ROOT);

    mModelList->AddRoot(wxT("Root"));

    sizer->Add(mModelList, 0, wxGROW|wxALL, 5);

    this->SetSizer(sizer);
    sizer->SetSizeHints(this);

    return true;
}

wxTreeItemId
ModelSelector::AddLeaf(const wxTreeItemId& root, const wxString& name)
{
    wxString::size_type pos = name.find_last_of(wxT('/'));
    if (pos != wxString::npos)
    {
        wxTreeItemId parent = AddBranch(root, name.substr(0, pos));
        return mModelList->AppendItem(parent, name.substr(pos+1, wxString::npos));
    }
    else
    {
        return mModelList->AppendItem(root, name);
    }
}

wxTreeItemId
ModelSelector::AddBranch(const wxTreeItemId& root, const wxString& name)
{
    wxString::size_type pos = name.find_last_of(wxT('/'));
    if (pos != wxString::npos)
    {
        wxTreeItemId parent = AddBranch(root, name.substr(0, pos));
        return CreateChildItem(parent, name.substr(pos+1, wxString::npos));
    }
    else
    {
        return CreateChildItem(root, name);
    }
}

wxTreeItemId
ModelSelector::CreateChildItem(const wxTreeItemId& parent, const wxString& name)
{
    wxTreeItemIdValue cookie;
    wxTreeItemId item = mModelList->GetFirstChild(parent, cookie);
    while (item.IsOk())
    {
        if (mModelList->GetItemText(item) == name)
            return item;
        item = mModelList->GetNextChild(parent, cookie);
    }

    return mModelList->AppendItem(parent, name);
}

void
ModelSelector::Reload(void)
{
    if (!Ogre::ResourceGroupManager::getSingletonPtr())
        return;

    mModelList->Freeze();

    wxTreeItemId root = mModelList->GetRootItem();
    mModelList->DeleteChildren(root);

    addResources(root, "*.model", "Model", "%model name");
    addResources(root, "*.mesh", "StaticEntity", "%mesh name");

    mModelList->Thaw();
}

void
ModelSelector::addResources(const wxTreeItemId& root, const wxString& pattern, const wxString& typeName, const wxString& parameterName)
{
    Ogre::FileInfoListPtr fileInfoList =
        Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
        AS_STRING(mResourceGroup),
        AS_STRING(pattern));
    for (Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
    {
        const WX::String& name = it->filename;
        wxTreeItemId item = AddLeaf(root, AS_STRING(name));
        mModelList->SetItemData(item, new ModelItemData(name, AS_STRING(typeName), AS_STRING(parameterName)));
    }
}

//////////////////////////////////////////////////////////////////////////

void
ModelSelector::OnUpdateControls(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
}

void
ModelSelector::OnModelListSelChanged(wxTreeEvent& e)
{
    wxTreeItemId item = e.GetItem();
    wxASSERT(item.IsOk());
    DoActiveItem(item);
}

void
ModelSelector::OnChildFocus(wxChildFocusEvent& e)
{
    e.Skip();

    if (e.GetWindow() != mModelList)
        return;

    wxTreeItemId item = mModelList->GetSelection();
    DoActiveItem(item);
}

void
ModelSelector::DoActiveItem(const wxTreeItemId& item)
{
    if (item.IsOk())
    {
        ModelItemData* data = static_cast<ModelItemData*>(mModelList->GetItemData(item));
        if (data)
        {
            const WX::String& name = data->name;
            WX::Action* action = GetSceneManipulator()->_getAction("CreateObjectAction");
            action->setParameter("type", data->typeName);
            action->setParameter(data->parameterName, name);
            GetSceneManipulator()->setActiveAction(action);
        }
    }
}
