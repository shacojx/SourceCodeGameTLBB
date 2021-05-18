// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "SceneBrowser.h"
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
#include <wx/toolbar.h>

#include "MyTree.h"
#include "SceneBrowser.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------

#include "SceneManipulator.h"

#include "ActorAnimationSettingDialog.h"
#include "WXEffectEditDialog.h"

#include "Core/WXSceneInfo.h"
#include "Core/WXEffectObject.h"

// ----------------------------------------------------------------------------
// SceneBrowser class
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

static const wxWindowID feID_OBJECTS_TREE_ID = wxNewId();

enum {
    feID_MODEL_CREATE = wxID_HIGHEST + 4000,
    feID_MODEL_DELETE,
    feID_MODEL_CREATE_FIRST,
    feID_MODEL_CREATE_SECOND,
};

BEGIN_EVENT_TABLE(SceneBrowser, wxPanel)
EVT_TREE_SEL_CHANGED(feID_OBJECTS_TREE_ID, SceneBrowser::OnTreeSelChanged)
EVT_TREE_ITEM_RIGHT_CLICK(feID_OBJECTS_TREE_ID,SceneBrowser::OnRightUp)
EVT_TREE_BEGIN_LABEL_EDIT(feID_OBJECTS_TREE_ID, SceneBrowser::OnBeginLabelEdit)
EVT_TREE_END_LABEL_EDIT(feID_OBJECTS_TREE_ID, SceneBrowser::OnEndLabelEdit)
EVT_TREE_KEY_DOWN(feID_OBJECTS_TREE_ID, SceneBrowser::OnTreeKeyDown)
EVT_MENU(feID_MODEL_CREATE, SceneBrowser::OnCreateModel)
EVT_MENU(feID_MODEL_DELETE, SceneBrowser::OnDeleteModel)
END_EVENT_TABLE()

namespace {

    class ObjectItemData : public wxTreeItemData
    {
    public:
        ObjectItemData(const WX::ObjectPtr& object)
            : object(object)
        {
        }

        WX::ObjectWeakPtr object;
    };

    class CountLocker
    {
    private:
        int& counter;

    public:
        CountLocker(int& counter) : counter(counter) { ++counter; }
        ~CountLocker() { --counter; }
    };
}

void
SceneBrowser::Init(void)
{
    mSceneManipulator = NULL;

    mObjectsTree = NULL;
    mActivedItem = NULL;
    mDisableNotifyEvent = 0;

	mEffectDialog = NULL;
}

void
SceneBrowser::AddObjectsToTree(void)
{
    wxASSERT(mSceneManipulator != NULL);

    mObjectsTree->DeleteChildren(mObjectsTree->GetRootItem());

    typedef WX::SceneInfo::Objects Objects;
    const WX::SceneInfo* sceneInfo = mSceneManipulator->getSceneInfo();
    const Objects& objects = sceneInfo->getObjects();
    for (Objects::const_iterator it = objects.begin(); it != objects.end(); ++it)
    {
        AddObjectToTree(*it);
    }
}

void
SceneBrowser::AddObjectToTree(const ObjectPtr& object)
{
    const String& category = object->getCategory();
    const String& type = object->getType();
    const String& name = object->getName();

    wxTreeItemId parent = createCategoryTreeItem(mObjectsTree->GetRootItem(), category);
    wxTreeItemId item = mObjectsTree->AppendItem(parent, AS_STRING(name),
        -1, -1, new ObjectItemData(object));
}

void
SceneBrowser::RemoveObjectFromTree(const ObjectPtr& object)
{
    wxTreeItemId item = getObjectTreeItem(object);
    wxASSERT(item.IsOk());

    if (item == mActivedItem)
        mActivedItem.Unset();

    CountLocker locker(mDisableNotifyEvent);

    mObjectsTree->Delete(item);
}

wxTreeItemId
SceneBrowser::getObjectTreeItem(const ObjectPtr& object)
{
    wxTreeItemId parent = getCategoryTreeItem(mObjectsTree->GetRootItem(), object->getCategory());
    if (!parent.IsOk())
        return parent;

    wxTreeItemIdValue cookie;
    wxTreeItemId item = mObjectsTree->GetFirstChild(parent, cookie);
    while (item.IsOk())
    {
        ObjectItemData* data = static_cast<ObjectItemData*>(mObjectsTree->GetItemData(item));
        wxASSERT(data != NULL);
        if (data->object.lock() == object)
            break;
        item = mObjectsTree->GetNextChild(parent, cookie);
    }
    return item;
}

wxTreeItemId
SceneBrowser::getCategoryTreeItem(const wxTreeItemId& parent, const String& category)
{
    wxASSERT(parent.IsOk());

    wxTreeItemIdValue cookie;
    wxTreeItemId item = mObjectsTree->GetFirstChild(parent, cookie);
    while (item.IsOk())
    {
        wxTreeItemData* data = mObjectsTree->GetItemData(item);
        wxASSERT(data == NULL);
        wxString text = mObjectsTree->GetItemText(item);
        if (text == AS_STRING(category))
            break;
        item = mObjectsTree->GetNextChild(parent, cookie);
    }
    return item;
}

wxTreeItemId
SceneBrowser::createCategoryTreeItem(const wxTreeItemId& parent, const String& category)
{
    wxASSERT(parent.IsOk());

    wxTreeItemId item = getCategoryTreeItem(parent, category);
    if (!item.IsOk())
    {
        item = mObjectsTree->AppendItem(parent, AS_STRING(category),
            -1, -1, NULL);
    }

    return item;
}

//////////////////////////////////////////////////////////////////////////

SceneBrowser::~SceneBrowser()
{
	if (mEffectDialog)
	{
		mEffectDialog->Destroy();
		delete mEffectDialog;
		mEffectDialog = NULL;
	}
}

bool
SceneBrowser::Create(wxWindow *parent, wxWindowID winid,
                     const wxPoint& pos, const wxSize& size,
                     long style, const wxString& name)
{
    if (!wxPanel::Create(parent, winid, pos, size, style, name))
        return false;

    wxFlexGridSizer *sizer = new wxFlexGridSizer(1, 0, 0);
    sizer->AddGrowableCol(0);
    sizer->AddGrowableRow(1);

    wxToolBar* toolbar = new wxToolBar(this, wxID_ANY);
    sizer->Add(toolbar, 0, wxGROW|wxALL, 2);

    mObjectsTree = new MyTreeCtrl(this, feID_OBJECTS_TREE_ID,
            wxDefaultPosition, wxDefaultSize,
            wxTR_DEFAULT_STYLE | wxTR_MULTIPLE | wxTR_EDIT_LABELS | wxTR_HIDE_ROOT);
    sizer->Add(mObjectsTree, 0, wxGROW|wxALL, 2);

    this->SetSizer(sizer);
    sizer->SetSizeHints(this);

    mObjectsTree->AddRoot(_("Scene"));

    return true;
}

void
SceneBrowser::SetSceneManipulator(WX::SceneManipulator* sceneManipulator)
{
    mSceneManipulator = sceneManipulator;
    mSceneManipulator->addSceneListener(this);

    AddObjectsToTree();
}

//////////////////////////////////////////////////////////////////////////

void
SceneBrowser::OnTreeSelChanged(wxTreeEvent& e)
{
    if (mDisableNotifyEvent)
        return;

    wxTreeCtrl* tree = wxStaticCast(e.GetEventObject(), wxTreeCtrl);
    mActivedItem = e.GetItem();

    wxArrayTreeItemIds selections;
    int count = tree->GetSelections(selections);

    // TODO: minimize select status change

    mSceneManipulator->clearSelectedObjects(this);
    for (int i = 0; i < count; ++i)
    {
        wxTreeItemId item = selections[i];
        ObjectItemData* data = static_cast<ObjectItemData*>(tree->GetItemData(item));
        if (data)
        {
            ObjectPtr object = data->object.lock();
            if (object)
                mSceneManipulator->selectObject(object);
        }
    }
}

void SceneBrowser::OnTreeKeyDown(wxTreeEvent& event)
{
    switch (event.GetKeyCode())
    {
    case WXK_F2:
        wxStaticCast(event.GetEventObject(), wxTreeCtrl)->EditLabel(mActivedItem);
        break;

    case WXK_DELETE:
        {
            mSceneManipulator->removeSelectedObjects(this);

            CountLocker locker(mDisableNotifyEvent);

            wxTreeCtrl* tree = wxStaticCast(event.GetEventObject(), wxTreeCtrl);
            wxArrayTreeItemIds selections;
            int count = tree->GetSelections(selections);
            for (int i = 0; i < count; ++i)
            {
                wxTreeItemId item = selections[i];
                ObjectItemData* data = static_cast<ObjectItemData*>(tree->GetItemData(item));
                if (data)
                {
                    wxASSERT(data->object.expired());
                    tree->Delete(item);
                }
            }
        }
        break;
    }
}

void SceneBrowser::OnRightUp(wxTreeEvent& event)
{
    wxTreeItemId item = event.GetItem();
    if (item.IsOk())
    {
        ObjectItemData* data = static_cast<ObjectItemData*>(mObjectsTree->GetItemData(item));
        ObjectPtr object;
        if (data)
            object = data->object.lock();
        // TODO: show context menu at event.GetPoint()
		if (object)
			OnContextMenu(object, event);
	}
}

void SceneBrowser::OnBeginLabelEdit(wxTreeEvent& event)
{
    wxTreeItemId item = event.GetItem();
    wxASSERT(item.IsOk());

    ObjectItemData* data = static_cast<ObjectItemData*>(mObjectsTree->GetItemData(item));
    if (!data || data->object.expired())
        event.Veto();
}

void SceneBrowser::OnEndLabelEdit(wxTreeEvent& event)
{
    wxTreeItemId item = event.GetItem();
    wxASSERT(item.IsOk());

    if (event.IsEditCancelled())
        return;

    String newName = AS_STRING(event.GetLabel());
    if (mSceneManipulator->getSceneInfo()->isAutoName(newName))
    {
        event.Veto();
        wxMessageBox(
            wxString::Format(_("Cannot rename %s: The name is reserved for auto naming object. Specify a different name."), newName.c_str()),
            _("Error Renaming Object"),
            wxOK|wxCENTRE|wxICON_ERROR, this);
        return;
    }

    ObjectItemData* data = static_cast<ObjectItemData*>(mObjectsTree->GetItemData(item));
    if (!data)
    {
        event.Veto();
        return;
    }

    ObjectPtr object = data->object.lock();
    if (!object)
    {
        event.Veto();
        return;
    }

    if (newName.empty())
    {
        // Always veto
        event.Veto();
        if (mSceneManipulator->getSceneInfo()->isAutoName(object->getName()))
            return;

        // Generate auto name and set tree item text manual
        newName = mSceneManipulator->getSceneInfo()->generateAutoName(object);
        mObjectsTree->SetItemText(item, AS_STRING(newName));
    }
    else
    {
        if (object->getName() == newName)
        {
            event.Veto();
            return;
        }

        ObjectPtr other = mSceneManipulator->getSceneInfo()->findObjectByName(newName);
        if (other)
        {
            wxASSERT(other != object);
            event.Veto();
            wxMessageBox(
                wxString::Format(_("Cannot rename %s: An object with the name you specified already exists. Specify a different name."), newName.c_str()),
                _("Error Renaming Object"),
                wxOK|wxCENTRE|wxICON_ERROR, this);
            return;
        }
    }

    mSceneManipulator->renameObject(object, newName, this);
}

//////////////////////////////////////////////////////////////////////////

void SceneBrowser::OnCreateModel(wxCommandEvent& event)
{
    // TODO
}

void SceneBrowser::OnDeleteModel(wxCommandEvent& event)
{
    // TODO
}

//////////////////////////////////////////////////////////////////////////

void SceneBrowser::onSceneReset(void)
{
    AddObjectsToTree();
}

void SceneBrowser::onAddObject(const ObjectPtr& object)
{
    AddObjectToTree(object);
}

void SceneBrowser::onRemoveObject(const ObjectPtr& object)
{
    RemoveObjectFromTree(object);
}

void SceneBrowser::onSelectObject(const ObjectPtr& object)
{
    wxTreeItemId item = getObjectTreeItem(object);
    wxASSERT(item.IsOk());

    mObjectsTree->SelectItem(item);
}

void SceneBrowser::onDeselectObject(const ObjectPtr& object)
{
    wxTreeItemId item = getObjectTreeItem(object);
    wxASSERT(item.IsOk());

    mObjectsTree->UnselectItem(item);
}

void SceneBrowser::onDeselectAllObjects(void)
{
    mObjectsTree->UnselectAll();
}

void SceneBrowser::onRenameObject(const ObjectPtr& object, const String& oldName)
{
    wxTreeItemId item = getObjectTreeItem(object);
    wxASSERT(item.IsOk());

    mObjectsTree->SetItemText(item, AS_STRING(object->getName()));
}

void SceneBrowser::OnContextMenu( ObjectPtr &object, wxTreeEvent& event )
{
	const Ogre::String &type = object->getType();

	if ( type == "Actor" )
	{
		ActorAnimationSettingDialog *dlg = new ActorAnimationSettingDialog(this, wxID_ANY,
			_("ActorAnimationSetting Dialog"), object, mSceneManipulator, event.GetPoint());

		int result = dlg->Show(true);
		//if (result == wxID_CANCEL)
		//{
		//	// 如果异常列表中仍有内容，就给出提示
		//	if (dlg->mListBox->GetCount() > 0)
		//	{
		//		wxMessageBox(_("Thera are still some wrongs in scene,remember to fix them!"));
		//	}
		//	else
		//	{
		//		wxMessageBox(_("All wrongs in scene have been fixed,remember to save the scene!"));
		//	}
		//}

	}
	else if ( type == "Effect" )
	{
		
		WX::Effect *effect = static_cast<WX::EffectObject *>(object.get())->getEffect();

		if ( effect )
		{
			if ( NULL == mEffectDialog )
			{
				mEffectDialog = new WXEffectEditDialog(this, wxID_ANY, _("Effect Setting"));
			}
			if ( false == mEffectDialog->IsShown() )
				mEffectDialog->Show();			
			mEffectDialog->InitEffectDialog(effect, mSceneManipulator);
		}			
	}

	mSceneManipulator->setActiveAction(NULL);
}
