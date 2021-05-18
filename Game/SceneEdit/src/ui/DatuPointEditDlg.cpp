#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "DatuPointEditDlg.h"
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

#include <wx/listbox.h>

#include "WXEditorApp.h"
#include "DatuPointEditDlg.h"
#include "SceneManipulator.h"
#include "res/DatuPointEditDlg_wdr.h"

#include "Core/TerrainData.h"

#include <OgreStringConverter.h>
#include <OgreCamera.h>

BEGIN_EVENT_TABLE(DatuPointEditDialog, wxPanel)

    EVT_BUTTON(ID_BUTTON_BEGIN_ADD_POINT,  DatuPointEditDialog::OnBeginAddSound)
    EVT_BUTTON(ID_BUTTON_END_ADD_POINT,  DatuPointEditDialog::OnEndAddSound)
    EVT_LISTBOX(ID_LISTBOX_POINTS, DatuPointEditDialog::OnListBoxDatuPointItemDoubleClick)
    EVT_TEXT_ENTER(ID_TEXTCTRL_RADIUS, DatuPointEditDialog::OnDatuPointItemTextChanged)
    EVT_TEXT_ENTER(ID_TEXTCTRL_XPOS, DatuPointEditDialog::OnDatuPointItemTextChanged)
    EVT_TEXT_ENTER(ID_TEXTCTRL_ZPOS, DatuPointEditDialog::OnDatuPointItemTextChanged)
    EVT_BUTTON(ID_BUTTON_DELETE_POINT,  DatuPointEditDialog::OnDeleteSound)

END_EVENT_TABLE()


DatuPointEditDialog::~DatuPointEditDialog()
{
    if (mSceneManipulator)
    {
        mSceneManipulator->removeSceneListener(this);

        _ClearDatuPointItems();
    }
}

bool DatuPointEditDialog::Create(wxWindow *parent, wxWindowID winid,
                                      const wxPoint& pos, const wxSize& size,
                                      long style, const wxString& name)
{
    if (!wxPanel::Create(parent, winid, pos, size, style, name))
        return false;

    DatuPointEditDlg(this, true, true);

    mSoundListBox = wxDynamicCast(FindWindow(ID_LISTBOX_POINTS), wxListBox);
    mRadiusTextCtrl = wxDynamicCast(FindWindow(ID_TEXTCTRL_RADIUS), wxTextCtrl);
    mXPosTextCtrl = wxDynamicCast(FindWindow(ID_TEXTCTRL_XPOS), wxTextCtrl);
    mZPosTextCtrl = wxDynamicCast(FindWindow(ID_TEXTCTRL_ZPOS), wxTextCtrl);
    mDefaultRadiusTextCtrl = wxDynamicCast(FindWindow(ID_TEXTCTRL_DEFAULT_RADIUS), wxTextCtrl);

    return true;
}

void DatuPointEditDialog::SetSceneManipulator(WX::SceneManipulator* manipulator)
{
    mSceneManipulator = manipulator;

    mSceneManipulator->addSceneListener(this);
}

void DatuPointEditDialog::onUIChanged(void* data, WX::UIChangeType type)
{
    if (type == WX::UIC_DATUPOINTEDIT)
    {
        Ogre::Vector3* pos = static_cast<Ogre::Vector3*>(data);
        std::pair<int, int> gridCoord = mSceneManipulator->getTerrainData()->getGrid(pos->x, pos->z);

        DatuPointItem* item = new DatuPointItem;
        item->mXPos = gridCoord.first;
        item->mZPos = gridCoord.second;

        Ogre::String defaultRadiusValue( mDefaultRadiusTextCtrl->GetValue() );

        item->mRadius = Ogre::StringConverter::parseReal(defaultRadiusValue);

        mDatuPointItems.push_back(item);

        _FillSoundListCtrl();

        mPointInfoFileChanged = true;
    }
}

bool DatuPointEditDialog::Show(bool show)
{
    if (mSceneManipulator)
    {
        WX::Action* action = mSceneManipulator->_getAction("DatuPointEditAction");

        action->setParameter("ShowDatuPointItem", show ? "true" : "false");
    }

    return wxWindow::Show(show);
}

void DatuPointEditDialog::_FillSoundListCtrl(void)
{
    mSoundListBox->Clear();

    for (size_t i=0; i<mDatuPointItems.size(); ++i)
    {
        const DatuPointItem* item = mDatuPointItems[i];

        mSoundListBox->Insert("x=" + Ogre::StringConverter::toString(item->mXPos) + " z="
            + Ogre::StringConverter::toString(item->mZPos) + " r=" + Ogre::StringConverter::toString(item->mRadius), i);
    }
}

void DatuPointEditDialog::OnBeginAddSound( wxCommandEvent &event )
{
    WX::Action* action = mSceneManipulator->_getAction("DatuPointEditAction");

    mSceneManipulator->setActiveAction(action);

    mPointInfoFileChanged = true;
}
void DatuPointEditDialog::OnEndAddSound( wxCommandEvent &event )
{
    WX::Action* action = mSceneManipulator->_getAction("DatuPointEditAction");

    mSceneManipulator->setActiveAction(NULL);
}

void DatuPointEditDialog::OnListBoxDatuPointItemDoubleClick( wxCommandEvent &event )
{
    int index = event.GetInt();

    if (index < (int)(mDatuPointItems.size()))
    {
        mWorkingPointIndex = index;

        DatuPointItem* soundItem = mDatuPointItems[event.GetInt()];

        mRadiusTextCtrl->SetValue( Ogre::StringConverter::toString(soundItem->mRadius) );
        mXPosTextCtrl->SetValue( Ogre::StringConverter::toString(soundItem->mXPos) );
        mZPosTextCtrl->SetValue( Ogre::StringConverter::toString(soundItem->mZPos) );

        mCurrentDatuPointItem = soundItem;

        Ogre::Camera* camera = mSceneManipulator->getCamera();

        WX::TerrainData* terrainData = mSceneManipulator->getTerrainData();

        std::pair<float, float> worldPos = terrainData->gridToWorld(soundItem->mXPos, soundItem->mZPos);
        float worldHeight = terrainData->getHeightAt(worldPos.first, worldPos.second);

        Ogre::Vector3 direction = camera->getDirection();
        float v = Ogre::Math::Sin(Ogre::Math::DegreesToRadians(89.9f));
        float s = Ogre::Math::Sqrt((direction.x*direction.x + direction.z*direction.z) / (1-v*v));
        direction.x /= s;
        direction.z /= s;
        direction.y = -v;
        camera->setDirection(direction);

        camera->setPosition(worldPos.first, worldHeight + 2750.0f, worldPos.second);

        WX::Action* action = mSceneManipulator->_getAction("DatuPointEditAction");

        action->setParameter("ShowRadiusEntity", Ogre::StringConverter::toString(worldPos.first) + " " +
            Ogre::StringConverter::toString(worldHeight) + " " +
            Ogre::StringConverter::toString(worldPos.second) + " " +
            Ogre::StringConverter::toString(soundItem->mRadius));
    }
}

void DatuPointEditDialog::OnDatuPointItemTextChanged(wxCommandEvent &e)
{
    if (mWorkingPointIndex >= 0 && mWorkingPointIndex < (int)(mDatuPointItems.size()))
    {
        DatuPointItem* soundItem = mDatuPointItems[mWorkingPointIndex];
        int textctrlID = e.GetId();

        switch (textctrlID)
        {
        case ID_TEXTCTRL_RADIUS:
            soundItem->mRadius = Ogre::StringConverter::parseInt(mRadiusTextCtrl->GetValue().c_str());
            break;
        case ID_TEXTCTRL_XPOS:
            soundItem->mXPos = Ogre::StringConverter::parseInt(mXPosTextCtrl->GetValue().c_str());
            break;
        case ID_TEXTCTRL_ZPOS:
            soundItem->mZPos = Ogre::StringConverter::parseInt(mZPosTextCtrl->GetValue().c_str());
            break;
        }

        mSoundListBox->SetString(mWorkingPointIndex, "x=" + Ogre::StringConverter::toString(soundItem->mXPos) + " z="
            + Ogre::StringConverter::toString(soundItem->mZPos) + " r=" + Ogre::StringConverter::toString(soundItem->mRadius));

        mPointInfoFileChanged = true;
    }
}

void DatuPointEditDialog::LoadDatuPointsFromFile(const Ogre::String& filename)
{
    ClearDatuPointInfo();

    WX::Action* action = mSceneManipulator->_getAction("DatuPointEditAction");

    std::ifstream stream;
    stream.open( filename.c_str() );

    if (stream)
    {
        Ogre::DataStreamPtr ifStream( new Ogre::FileStreamDataStream(&stream, false) );

        Ogre::String line;

        while (!ifStream->eof())
        {
            line = ifStream->getLine();

            size_t pos = line.find_first_of("x=");

            while (pos != Ogre::String::npos)
            {
                size_t endpos = line.find_first_of(",", pos);

                Ogre::String valueStr = line.substr(pos + 2, endpos - pos - 2);

                DatuPointItem* pointItem = new DatuPointItem;
                pointItem->mXPos = Ogre::StringConverter::parseInt(valueStr);

                pos = line.find_first_of("z=", endpos);
                endpos = line.find_first_of(",", pos);
                valueStr = line.substr(pos + 2, endpos - pos - 2);
                pointItem->mZPos = Ogre::StringConverter::parseInt(valueStr);

                pos = line.find_first_of("r=", endpos);
                endpos = line.find_first_of("}", pos);
                valueStr = line.substr(pos + 2, endpos - pos - 2);
                pointItem->mRadius = Ogre::StringConverter::parseInt(valueStr);

                mDatuPointItems.push_back(pointItem);

                action->setParameter("CreatePointEntity", Ogre::StringConverter::toString(pointItem->mXPos)
                    + " " + Ogre::StringConverter::toString(pointItem->mZPos));

                pos = line.find_first_of("x=", endpos);
            }
        }
    }
   
    _FillSoundListCtrl();
}

void DatuPointEditDialog::_ClearDatuPointItems(void)
{
    for (size_t i=0; i<mDatuPointItems.size(); ++i)
    {
        delete mDatuPointItems[i];
    }

    mDatuPointItems.clear();
}

void DatuPointEditDialog::SaveDatuPointsToFile(const Ogre::String& filename)
{
    if (mPointInfoFileChanged)
    {
        std::ofstream stream(filename.c_str());

        Ogre::String savedStr;

        savedStr += "{\n";

        int count = 0;
        for (size_t i=0; i<mDatuPointItems.size(); ++i)
        {
            DatuPointItem* item = mDatuPointItems[i];

            savedStr += "{x=" + Ogre::StringConverter::toString(item->mXPos) + ", z=" + 
                Ogre::StringConverter::toString(item->mZPos) + ", r=" + 
                Ogre::StringConverter::toString(item->mRadius) + "},";

            ++count;

            if (count == 4)
            {
                count = 0;
                savedStr += '\n';
            }
        }

        if (count != 0)
            savedStr += '\n';

        savedStr += "}";

        stream << savedStr.c_str();

        stream.close();

        mPointInfoFileChanged = false;
    }
}

void DatuPointEditDialog::OnDeleteSound( wxCommandEvent &event )
{
    if (mCurrentDatuPointItem)
    {
        _DeleteDatuPointItem(mCurrentDatuPointItem, mSoundListBox->GetSelection());
        mSoundListBox->Delete(mSoundListBox->GetSelection());

        mCurrentDatuPointItem = NULL;

        mPointInfoFileChanged = true;
    }
}

void DatuPointEditDialog::_DeleteDatuPointItem(DatuPointItem* soundItem, int index)
{
    for (size_t i=0; i<mDatuPointItems.size(); ++i)
    {
        if (mDatuPointItems[i] == soundItem)
        {
            delete mDatuPointItems[i];

            mDatuPointItems.erase(mDatuPointItems.begin() + i);

            WX::Action* action = mSceneManipulator->_getAction("DatuPointEditAction");

            action->setParameter("DeleteDatuPointItem", Ogre::StringConverter::toString(index));
        }
    }
}

void DatuPointEditDialog::ClearDatuPointInfo(void)
{
    _ClearDatuPointItems();

    WX::Action* action = mSceneManipulator->_getAction("DatuPointEditAction");

    action->setParameter("ClearPointEntity", "true");

    mCurrentDatuPointItem = NULL;
}