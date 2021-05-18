// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "ParticleSystemSelector.h"
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

#include <wx/splitter.h>
//#include <wx/treectrl.h>
#include <wx/listctrl.h>

#include "ParticleSystemSelector.h"
#include "WXParticleEditorDialog.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------

#include <OgreParticleSystemManager.h>

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------

#include "SceneManipulator.h"
#include "Action.h"
#include "EffectSystem\WXEffectManager.h"

// ----------------------------------------------------------------------------
// ParticleSystemSelector class
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

static const int feID_PARTICLESYSTEM_LIST = wxNewId();
static const int feID_RELOAD_PARTICLE_LIST =wxNewId();

BEGIN_EVENT_TABLE(ParticleSystemSelector, wxPanel)
EVT_UPDATE_UI(feID_PARTICLESYSTEM_LIST, ParticleSystemSelector::OnUpdateControls)

EVT_LIST_ITEM_SELECTED(feID_PARTICLESYSTEM_LIST, ParticleSystemSelector::OnParticleSystemListSelected)
EVT_COMMAND_LEFT_DCLICK(feID_PARTICLESYSTEM_LIST, ParticleSystemSelector::OnParticleSystemListLeftDoubleClicked)
EVT_BUTTON(feID_RELOAD_PARTICLE_LIST, ParticleSystemSelector::OnReloadParticleList)

END_EVENT_TABLE()

ParticleSystemSelector::~ParticleSystemSelector()
{
    if (mParticleSystemList)
    {
        mParticleSystemList->Freeze();
        mParticleSystemList->DeleteAllItems();
        mParticleSystemList->Thaw();
    }

	if (mParticleEditorDialog)
	{
		mParticleEditorDialog->Destroy();
		delete mParticleEditorDialog;
		mParticleEditorDialog = NULL;
	}
}

void
ParticleSystemSelector::Init(const wxString& resourceGroup)
{
    mSceneManipulator = NULL;
    mParticleSystemList = NULL;

	mParticleEditorDialog = NULL;

    mResourceGroup = resourceGroup;
}

void
ParticleSystemSelector::SetSceneManipulator(WX::SceneManipulator* sceneManipulator)
{
    mSceneManipulator = sceneManipulator;
}

bool
ParticleSystemSelector::Create(wxWindow *parent, wxWindowID winid,
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
ParticleSystemSelector::Reload(const wxString& resourceGroup)
{
    mResourceGroup = resourceGroup;
    Reload();
}

bool
ParticleSystemSelector::CreateControls(void)
{
    wxFlexGridSizer* sizer = new wxFlexGridSizer(1);
    sizer->AddGrowableCol(0);
    sizer->AddGrowableRow(1);

	wxButton *item2 = new wxButton( this, feID_RELOAD_PARTICLE_LIST, _("更新"), wxDefaultPosition, wxDefaultSize, 0 );
	sizer->Add( item2, 0, wxALIGN_CENTER|wxALL, 5 );

    mParticleSystemList = new wxListCtrl(this, feID_PARTICLESYSTEM_LIST,
        wxDefaultPosition, wxDefaultSize,
        wxLC_LIST | wxLC_SINGLE_SEL,
        wxDefaultValidator);

    sizer->Add(mParticleSystemList, 0, wxGROW|wxALL, 5);

    this->SetSizer(sizer);
    sizer->SetSizeHints(this);

    return true;
}

void
ParticleSystemSelector::Reload(void)
{
    if (!Ogre::ResourceGroupManager::getSingletonPtr())
        return;

    mParticleSystemList->Freeze();

    mParticleSystemList->ClearAll();
    
    Ogre::ParticleSystemManager::ParticleSystemTemplateIterator iterator = Ogre::ParticleSystemManager::getSingleton().getTemplateIterator();

    while ( iterator.hasMoreElements() )
    {
        // 获取到粒子系统的名称

		long index = mParticleSystemList->GetItemCount();
        mParticleSystemList->InsertItem( index, AS_STRING( iterator.peekNextKey() ) );
		mParticleSystemList->SetItemTextColour( index , *wxRED );
        // 使iterator往后移
        iterator.moveNext();
    }

	// 添加特效
	// 填充现在的effect模板的列表
	WX::EffectManager::EffectTemplateIterator it = 
		WX::EffectManager::getSingleton().getEffectTemplateIterator();

	while ( it.hasMoreElements() )
	{
		long index = mParticleSystemList->GetItemCount();

		mParticleSystemList->InsertItem( index, AS_STRING( it.peekNextKey() ) );

		if (it.peekNextKey() == "EffectEmpty")
			mParticleSystemList->SetItemTextColour(index, *wxGREEN);

		it.moveNext();
	}

    mParticleSystemList->Thaw();
}

//////////////////////////////////////////////////////////////////////////

void
ParticleSystemSelector::OnUpdateControls(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
}

void
ParticleSystemSelector::OnParticleSystemListSelected(wxListEvent& e)
{
    long item = mParticleSystemList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (item >= 0)
    {
		wxColour itemColor = mParticleSystemList->GetItemTextColour(item);

		wxString name = mParticleSystemList->GetItemText(item);
		WX::Action* action = GetSceneManipulator()->_getAction("CreateObjectAction");

		// 靠颜色来判断是粒子系统或特效
		if ( itemColor == *wxRED )
		{
			action->setParameter("type", "ParticleSystem");
			action->setParameter("%particle system name", AS_STRING(name));
		}
		else
		{
			action->setParameter("type", "Effect");
			action->setParameter("%effect name", AS_STRING(name));
		}

        GetSceneManipulator()->setActiveAction(action);
    }
}

void
ParticleSystemSelector::OnParticleSystemListLeftDoubleClicked(wxCommandEvent& e)
{
    long item = mParticleSystemList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (item >= 0)
    {
		wxColour itemColor = mParticleSystemList->GetItemTextColour(item);

		wxString name = mParticleSystemList->GetItemText(item);

		// 靠颜色来判断是粒子系统或特效
		if ( itemColor == *wxRED )
		{
			Ogre::ParticleSystem *pTemplate = Ogre::ParticleSystemManager::getSingleton().getTemplate(name.c_str());

			if (pTemplate)
			{
				if ( NULL == mParticleEditorDialog )
				{
					int width = 0;
					int height = 0;
					int x = 0;
					int y = 0;

					GetParent()->GetParent()->GetParent()->GetParent()->GetSize(&width, &height);
					GetParent()->GetParent()->GetParent()->GetParent()->GetPosition(&x, &y);

					mParticleEditorDialog = new ParticleEditorDialog(this, wxID_ANY, _("Fairy Particle Editor"), wxPoint(x, y), wxSize(width / 3, height));
				}
				if ( false == mParticleEditorDialog->IsShown() )
					mParticleEditorDialog->Show();	

				mParticleEditorDialog->InitParticleEditor(pTemplate, Ogre::String(name.c_str()), mSceneManipulator);
			}
		}	

		GetSceneManipulator()->setActiveAction(NULL);
    }
}

void 
ParticleSystemSelector::OnReloadParticleList( wxCommandEvent &event )
{
	Reload();
}

