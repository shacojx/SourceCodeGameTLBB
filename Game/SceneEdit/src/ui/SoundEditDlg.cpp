#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "SoundEditDialog.h"
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
#include "SoundEditDlg.h"
#include "SceneManipulator.h"
#include "res/SoundEditDlg_wdr.h"

#include "Core/TerrainData.h"
#include "TLBB_DBC.h"

#include <OgreStringConverter.h>
#include <OgreCamera.h>

BEGIN_EVENT_TABLE(SoundEditDialog, wxPanel)

    EVT_BUTTON(ID_BUTTON_BEGIN_ADD_SOUND,  SoundEditDialog::OnBeginAddSound)
    EVT_BUTTON(ID_BUTTON_END_ADD_SOUND,  SoundEditDialog::OnEndAddSound)
    EVT_LISTBOX(ID_LISTBOX_SCENE_SOUND, SoundEditDialog::OnListBoxSoundItemDoubleClick)
    EVT_TEXT_ENTER(ID_TEXTCTRL_RADIUS, SoundEditDialog::OnSoundItemTextChanged)
    EVT_TEXT_ENTER(ID_TEXTCTRL_XPOS, SoundEditDialog::OnSoundItemTextChanged)
    EVT_TEXT_ENTER(ID_TEXTCTRL_ZPOS, SoundEditDialog::OnSoundItemTextChanged)
    EVT_TEXT_ENTER(ID_TEXTCTRL_REPEAT_TIME, SoundEditDialog::OnSoundItemTextChanged)
    EVT_TEXT_ENTER(ID_TEXTCTRL_REPEAT_INTERVAL_TIME, SoundEditDialog::OnSoundItemTextChanged)
    EVT_TEXT_ENTER(ID_TEXTCTRL_NEXT_REPEAT_TIME, SoundEditDialog::OnSoundItemTextChanged)
    EVT_COMBOBOX(ID_COMBO_SOUND_NAME, SoundEditDialog::OnComboSoundNames)
    EVT_BUTTON(ID_BUTTON_DELETE_SOUND,  SoundEditDialog::OnDeleteSound)
    EVT_BUTTON(ID_BUTTON_SAVE_ESD,  SoundEditDialog::OnSaveSound)

 //   EVT_CHECKBOX(ID_CHECKBOX_PLAYINGAME, SoundEditDialog::OnCheckBoxPlayInGame)
 //   EVT_UPDATE_UI(ID_CHECKBOX_PLAYINGAME, SoundEditDialog::OnUpdateCheckBoxPlayInGame)

END_EVENT_TABLE()


SoundEditDialog::~SoundEditDialog()
{
    if (mSceneManipulator)
    {
        Ogre::Root::getSingleton().removeFrameListener(this);

        mSceneManipulator->removeSceneListener(this);

        _ClearSoundItems();
    }
}

bool SoundEditDialog::Create(wxWindow *parent, wxWindowID winid,
                                      const wxPoint& pos, const wxSize& size,
                                      long style, const wxString& name)
{
    if (!wxPanel::Create(parent, winid, pos, size, style, name))
        return false;

    SoundEditDlg(this, true, true);

    mSoundListBox = wxDynamicCast(FindWindow(ID_LISTBOX_SCENE_SOUND), wxListBox);
    mRadiusTextCtrl = wxDynamicCast(FindWindow(ID_TEXTCTRL_RADIUS), wxTextCtrl);
    mXPosTextCtrl = wxDynamicCast(FindWindow(ID_TEXTCTRL_XPOS), wxTextCtrl);
    mZPosTextCtrl = wxDynamicCast(FindWindow(ID_TEXTCTRL_ZPOS), wxTextCtrl);
    mRepeatTimeTextCtrl = wxDynamicCast(FindWindow(ID_TEXTCTRL_REPEAT_TIME), wxTextCtrl);
    mRepeatIntervalTimeTextCtrl = wxDynamicCast(FindWindow(ID_TEXTCTRL_REPEAT_INTERVAL_TIME), wxTextCtrl);
    mNextRepeatTimeTextCtrl = wxDynamicCast(FindWindow(ID_TEXTCTRL_NEXT_REPEAT_TIME), wxTextCtrl);
    mSoundNameComboBox = wxDynamicCast(FindWindow(ID_COMBO_SOUND_NAME), wxComboBox);
    mSoundIDText = wxDynamicCast(FindWindow(ID_TEXT_SOUNDIDTEXT), wxStaticText);

    return true;
}

void SoundEditDialog::SetSceneManipulator(WX::SceneManipulator* manipulator)
{
    mSceneManipulator = manipulator;

    mSceneManipulator->addSceneListener(this);

    Ogre::Root::getSingleton().addFrameListener(this);
}

void SoundEditDialog::onUIChanged(void* data, WX::UIChangeType type)
{
    if (type == WX::UIC_SOUNDEDIT)
    {
        Ogre::Vector3* pos = static_cast<Ogre::Vector3*>(data);
        std::pair<int, int> gridCoord = mSceneManipulator->getTerrainData()->getGrid(pos->x, pos->z);

        SoundItem* item = new SoundItem;
        item->mInstanceID = 0;
        item->mSoundID = 0;
        item->mXPos = gridCoord.first;
        item->mZPos = gridCoord.second;
        item->mRadius = 0;
        item->mRepeatTime = 0;
        item->mRepeatIntervalTime = 0;
        item->mNextRepeatTime = 0;
        item->mCurrentRepeatTime = 0;
        item->mCurrentRepeatIntervalTime = 0;
        item->mCurrentPlayIntervalTime = 0;
        item->mSoundHandle = -1;

        mSoundItems.push_back(item);

        _FillSoundListCtrl();

        mSoundInfoFileChanged = true;
    }
}

bool SoundEditDialog::Show(bool show)
{
    if (mSceneManipulator)
    {
        WX::Action* action = mSceneManipulator->_getAction("SoundEditAction");

        action->setParameter("ShowSoundItem", show ? "true" : "false");
    }

    mPlaySound = show;

    if (!mPlaySound)
        _StopAllSounds();

    return wxWindow::Show(show);
}

void SoundEditDialog::_FillSoundListCtrl(void)
{
    mSoundListBox->Clear();

    for (size_t i=0; i<mSoundItems.size(); ++i)
    {
        const SoundItem* item = mSoundItems[i];

        mSoundListBox->Insert("x= " + Ogre::StringConverter::toString(item->mXPos) + " z= "
            + Ogre::StringConverter::toString(item->mZPos) + " sound id= "
            + Ogre::StringConverter::toString(item->mSoundID) + " index= "
            + Ogre::StringConverter::toString(i), i);
    }
}

void SoundEditDialog::OnBeginAddSound( wxCommandEvent &event )
{
    WX::Action* action = mSceneManipulator->_getAction("SoundEditAction");

    mSceneManipulator->setActiveAction(action);

    mSoundInfoFileChanged = true;
}
void SoundEditDialog::OnEndAddSound( wxCommandEvent &event )
{
    WX::Action* action = mSceneManipulator->_getAction("SoundEditAction");

    mSceneManipulator->setActiveAction(NULL);
}

void SoundEditDialog::OnListBoxSoundItemDoubleClick( wxCommandEvent &event )
{
    _StopAllSounds();

    mPlaySoundInGame = false;

    int index = event.GetInt();

    if (index < (int)(mSoundItems.size()))
    {
        mWorkingSoundIndex = index;

        SoundItem* soundItem = mSoundItems[event.GetInt()];

        if (mSoundNameComboBox->IsEmpty())
            _FillSoundNameComboBox();

        SoundNames::iterator it = mSoundNames.find(soundItem->mSoundID);

        if (it != mSoundNames.end())
        {
            const Ogre::String& soundName = it->second;

            if (mSoundNameComboBox->FindString(soundName))
                mSoundNameComboBox->SetValue(soundName);
            else
                mSoundNameComboBox->SetSelection(0);

            mCurrentSoundHandle = _PlaySound(it->second, mCurrentSoundHandle, soundItem->mRepeatTime == 0);
        }
        else
        {
            mSoundNameComboBox->SetSelection(0);
        }

        mRadiusTextCtrl->SetValue( Ogre::StringConverter::toString(soundItem->mRadius) );
        mXPosTextCtrl->SetValue( Ogre::StringConverter::toString(soundItem->mXPos) );
        mZPosTextCtrl->SetValue( Ogre::StringConverter::toString(soundItem->mZPos) );
        mRepeatTimeTextCtrl->SetValue( Ogre::StringConverter::toString(soundItem->mRepeatTime) );
        mRepeatIntervalTimeTextCtrl->SetValue( Ogre::StringConverter::toString(soundItem->mRepeatIntervalTime) );
        mNextRepeatTimeTextCtrl->SetValue( Ogre::StringConverter::toString(soundItem->mNextRepeatTime) );
        mSoundIDText->SetLabel( Ogre::StringConverter::toString(soundItem->mSoundID) );

        mCurrentSoundItem = soundItem;

        mCurrentRepeatTime = 0;
        mCurrentRepeatIntervalTime = 0;
        mCurrentPlayIntervalTime = 0;

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

        WX::Action* action = mSceneManipulator->_getAction("SoundEditAction");

        action->setParameter("ShowRadiusEntity", Ogre::StringConverter::toString(worldPos.first) + " " +
            Ogre::StringConverter::toString(worldHeight) + " " +
            Ogre::StringConverter::toString(worldPos.second) + " " +
            Ogre::StringConverter::toString(soundItem->mRadius));
    }
}

void SoundEditDialog::OnSoundItemTextChanged(wxCommandEvent &e)
{
    if (mWorkingSoundIndex >= 0 && mWorkingSoundIndex < (int)(mSoundItems.size()))
    {
        SoundItem* soundItem = mSoundItems[mWorkingSoundIndex];
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
        case ID_TEXTCTRL_REPEAT_TIME:
            soundItem->mRepeatTime = Ogre::StringConverter::parseInt(mRepeatTimeTextCtrl->GetValue().c_str());
            break;
        case ID_TEXTCTRL_REPEAT_INTERVAL_TIME:
            soundItem->mRepeatIntervalTime = Ogre::StringConverter::parseInt(mRepeatIntervalTimeTextCtrl->GetValue().c_str());
            break;
        case ID_TEXTCTRL_NEXT_REPEAT_TIME:
            soundItem->mNextRepeatTime = Ogre::StringConverter::parseInt(mNextRepeatTimeTextCtrl->GetValue().c_str());
            break;
        }

        mSoundInfoFileChanged = true;
    }
}

void SoundEditDialog::_FillSoundNameComboBox(void)
{
    assert (mSoundNameComboBox);

    if (wxGetApp().funcEnumSoundFile)
    {
        int nIndex = 0;
        do
        {
            char szTemp[100];
            int id;

            bool bHave = wxGetApp().funcEnumSoundFile(nIndex++, szTemp, 100, id);
            if(!bHave) break;

            mSoundNameComboBox->AppendString(szTemp);
            mSoundNames.insert( SoundNames::value_type(id, szTemp) );
        }while(true);
    }
}

void SoundEditDialog::LoadSoundInfoFromFile(const Ogre::String& filename)
{
    ClearSoundData();

    mCurrentFileName = filename;

    WX::Action* action = mSceneManipulator->_getAction("SoundEditAction");

    DBC::DBCFile excelDBFile(1);
    if(!excelDBFile.OpenFromTXT(filename.c_str()))
    {
        mSoundInfoFileChanged = false;
        return;
    }

    int iRecordCount = excelDBFile.GetRecordsNum(); 
    int iLastColum   = excelDBFile.GetFieldsNum();
    const DBC::DBCFile::FIELD* pInfo = NULL;	

    for(int i = 0; i < iRecordCount; i++)
    {
        SoundItem* soundItem = new SoundItem;
        // 读取ID
        pInfo = excelDBFile.Search_Posistion(i, 0);
        soundItem->mInstanceID = pInfo->iValue;

        // 类型id
        pInfo = excelDBFile.Search_Posistion(i, 1);
        soundItem->mSoundID = pInfo->iValue;

        // 位置x
        pInfo = excelDBFile.Search_Posistion(i, 2);
        soundItem->mXPos = pInfo->iValue;

        // 位置z
        pInfo = excelDBFile.Search_Posistion(i, 3);
        soundItem->mZPos = pInfo->iValue;

        // 有效半径
        pInfo = excelDBFile.Search_Posistion(i, 4);
        soundItem->mRadius = pInfo->iValue;

        // 连播次数
        pInfo = excelDBFile.Search_Posistion(i, 5);
        soundItem->mRepeatTime = pInfo->iValue;

        // 连播间隔时间
        pInfo = excelDBFile.Search_Posistion(i, 6);
        soundItem->mRepeatIntervalTime = pInfo->iValue;

        // 下次播放间隔时间
        pInfo = excelDBFile.Search_Posistion(i, 7);
        soundItem->mNextRepeatTime = pInfo->iValue;

        mSoundItems.push_back(soundItem);


        action->setParameter("CreateSoundEntity", Ogre::StringConverter::toString(soundItem->mXPos)
            + " " + Ogre::StringConverter::toString(soundItem->mZPos));
    }

    _FillSoundListCtrl();
}

void SoundEditDialog::_ClearSoundItems(void)
{
    for (size_t i=0; i<mSoundItems.size(); ++i)
    {
        delete mSoundItems[i];
    }

    mSoundItems.clear();
}

void SoundEditDialog::OnComboSoundNames(wxCommandEvent &e)
{
    if (mWorkingSoundIndex >= 0 && mWorkingSoundIndex < (int)(mSoundItems.size()))
    {
        SoundItem* soundItem = mSoundItems[mWorkingSoundIndex];

        for (SoundNames::iterator it = mSoundNames.begin(); it != mSoundNames.end(); ++it)
        {
            if (it->second == mSoundNameComboBox->GetStringSelection())
            {
                soundItem->mSoundID = it->first;
                mSoundInfoFileChanged = true;

                mSoundIDText->SetLabel( Ogre::StringConverter::toString(soundItem->mSoundID) );

                return;
            }
        }

        wxMessageBox("can't find the sound id by the sound name");
    }
}

void SoundEditDialog::SaveSoundInfoFromFile(const Ogre::String& filename)
{
    if (mSoundInfoFileChanged)
    {
        char buf[1024];
        FILE* pFile = NULL;
        pFile = ::fopen(filename.c_str(), "w");
        if(NULL == pFile)
        {
            wxMessageBox("Can't open the file, Maybe the file is missing or it is read only!");
            return;
        }

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "INT\tINT\tINT\tINT\tINT\tINT\tINT\tINT\n");
        ::fwrite(buf, strlen(buf), 1, pFile);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "ID\t声音ID\t声源位置X\t声源位置Z\t有效距离\t连播次数\t间隔时间(ms)\t下一次连播间隔时间(ms)\n");
        ::fwrite(buf, strlen(buf), 1, pFile);
        
        memset(buf, 0, sizeof(buf));

        for(size_t i=0; i<mSoundItems.size(); ++i)
        {
            SoundItem* soundItem = mSoundItems[i];

            if (soundItem->mRepeatTime == 0)
            {
                sprintf(buf, "%d\t%d\t%d\t%d\t%d\n",
                    i, soundItem->mSoundID, soundItem->mXPos, soundItem->mZPos, soundItem->mRadius);
            }
            else
            {
                sprintf(buf, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
                    i, soundItem->mSoundID, soundItem->mXPos, soundItem->mZPos, soundItem->mRadius,
                    soundItem->mRepeatTime, soundItem->mRepeatIntervalTime, soundItem->mNextRepeatTime);
            }

            ::fwrite(buf, strlen(buf), 1, pFile);
        }

        ::fclose(pFile);

        mSoundInfoFileChanged = false;
    }
}

int SoundEditDialog::_PlaySound(const Ogre::String& soundName, int stopSoundHandle, bool loop)
{
    _StopSound(stopSoundHandle);

    if (wxGetApp().funcPlay)
    {
        mPlaySound = true;
        return wxGetApp().funcPlay(soundName.c_str(), NULL, loop);
    }
    else
    {
        mPlaySound = false;
        return -1;
    }

}

void SoundEditDialog::_StopSound(int soundHandle)
{
    if (wxGetApp().funcStop)
    {
        wxGetApp().funcStop(soundHandle);
    }

    mPlaySound = false;
}

bool SoundEditDialog::frameStarted(const Ogre::FrameEvent& evt)
{
    if (!mPlaySound)
        return true;

    if (/*mPlaySoundInGame*/0)
    {
        Ogre::Camera* camera = mSceneManipulator->getCamera();
        const Ogre::Vector3& camPos = camera->getPosition();

        WX::TerrainData* terrainData = mSceneManipulator->getTerrainData();

        std::pair<int, int> camGrid = terrainData->getGrid(camPos.x, camPos.z);

        for (size_t i=0; i<mSoundItems.size(); ++i)
        {
            SoundItem* workingItem = mSoundItems[i];

            if (workingItem->mRepeatTime != 0)
            {
                FLOAT deltaTime = evt.timeSinceLastFrame * 1000;

                // 如果当前的播放次数已达到重复播放次数，就累加时间，直到达到下次播放的时间，就播放
                if (workingItem->mCurrentRepeatTime == workingItem->mRepeatTime)
                {
                    workingItem->mCurrentPlayIntervalTime += (INT)deltaTime;

                    if (workingItem->mCurrentPlayIntervalTime >= workingItem->mNextRepeatTime)
                    {
                        workingItem->mCurrentRepeatTime = 0;
                        workingItem->mCurrentPlayIntervalTime = 0;

                        SoundNames::iterator it = mSoundNames.find(workingItem->mSoundID);

                        if (it != mSoundNames.end())
                        {
                            workingItem->mSoundHandle = _PlaySound(it->second, workingItem->mSoundHandle, false);
                        }

                        ++workingItem->mCurrentRepeatTime;
                    }
                }
                else
                {
                    // 累加重复播放之间的间隔时间
                    workingItem->mCurrentRepeatIntervalTime += (INT)deltaTime;

                    if (workingItem->mCurrentRepeatIntervalTime >= workingItem->mRepeatIntervalTime)
                    {
                        workingItem->mCurrentRepeatIntervalTime = 0;

                        SoundNames::iterator it = mSoundNames.find(workingItem->mSoundID);

                        if (it != mSoundNames.end())
                        {
                            workingItem->mSoundHandle = _PlaySound(it->second, workingItem->mSoundHandle, false);
                        }

                        ++workingItem->mCurrentRepeatTime;
                    }
                }
            }
            else
            {
                if (workingItem->mSoundHandle == -1)
                {
                    SoundNames::iterator it = mSoundNames.find(workingItem->mSoundID);

                    if (it != mSoundNames.end())
                    {
                        workingItem->mSoundHandle = _PlaySound(it->second, workingItem->mSoundHandle, false);
                    }

                    workingItem->mSoundHandle = _PlaySound(it->second, workingItem->mSoundHandle, true);
                }
            }

            if (workingItem->mRadius > 0)
            {
                float dis = Ogre::Math::Sqrt( (camGrid.first - workingItem->mXPos) * (camGrid.first - workingItem->mXPos) +
                    (camGrid.second - workingItem->mZPos) * (camGrid.second - workingItem->mZPos) );

                float volume = 0;

                if(dis <= workingItem->mRadius) 
                    volume = 1.0f-(dis/workingItem->mRadius);

                _SetSoundVolume(workingItem->mSoundHandle, volume);
            }
        }
    }
    else
    {
        if (mPlaySound && mCurrentSoundItem && mCurrentSoundItem->mRepeatTime != 0)
        {
            FLOAT deltaTime = evt.timeSinceLastFrame * 1000;

            // 如果当前的播放次数已达到重复播放次数，就累加时间，直到达到下次播放的时间，就播放
            if (mCurrentRepeatTime == mCurrentSoundItem->mRepeatTime)
            {
                mCurrentPlayIntervalTime += (INT)deltaTime;

                if (mCurrentPlayIntervalTime >= mCurrentSoundItem->mNextRepeatTime)
                {
                    mCurrentRepeatTime = 0;
                    mCurrentPlayIntervalTime = 0;

                    SoundNames::iterator it = mSoundNames.find(mCurrentSoundItem->mSoundID);

                    if (it != mSoundNames.end())
                    {
                        mCurrentSoundHandle = _PlaySound(it->second, mCurrentSoundHandle, false);
                    }

                    ++mCurrentRepeatTime;
                }
            }
            else
            {
                // 累加重复播放之间的间隔时间
                mCurrentRepeatIntervalTime += (INT)deltaTime;

                if (mCurrentRepeatIntervalTime >= mCurrentSoundItem->mRepeatIntervalTime)
                {
                    mCurrentRepeatIntervalTime = 0;

                    SoundNames::iterator it = mSoundNames.find(mCurrentSoundItem->mSoundID);

                    if (it != mSoundNames.end())
                    {
                        mCurrentSoundHandle = _PlaySound(it->second, mCurrentSoundHandle, false);
                    }

                    ++mCurrentRepeatTime;
                }
            }
        }
    }

    return true;
}

void SoundEditDialog::OnDeleteSound( wxCommandEvent &event )
{
    if (mCurrentSoundItem)
    {
        _StopSound(mCurrentSoundItem->mSoundHandle);

        _DeleteSoundItem(mCurrentSoundItem, mSoundListBox->GetSelection());
        mSoundListBox->Delete(mSoundListBox->GetSelection());

        mCurrentSoundItem = NULL;

        mSoundInfoFileChanged = true;
    }
}

void SoundEditDialog::_DeleteSoundItem(SoundItem* soundItem, int index)
{
    for (size_t i=0; i<mSoundItems.size(); ++i)
    {
        if (mSoundItems[i] == soundItem)
        {
            delete mSoundItems[i];

            mSoundItems.erase(mSoundItems.begin() + i);

            WX::Action* action = mSceneManipulator->_getAction("SoundEditAction");

            action->setParameter("DeleteSoundItem", Ogre::StringConverter::toString(index));

            _FillSoundListCtrl();
        }
    }
}

void SoundEditDialog::OnSaveSound( wxCommandEvent &event )
{
    if (false == mCurrentFileName.empty())
        SaveSoundInfoFromFile(mCurrentFileName);
    else
        wxMessageBox("No Scene Name");
}

void SoundEditDialog::ClearSoundData(void)
{
    _ClearSoundItems();

    _StopAllSounds();

    WX::Action* action = mSceneManipulator->_getAction("SoundEditAction");

    action->setParameter("ClearSoundEntity", "true");

    mCurrentSoundItem = NULL;
    mCurrentSoundHandle = -1;
    mCurrentRepeatTime = 0;
    mCurrentRepeatIntervalTime = 0;
    mCurrentPlayIntervalTime = 0;

    mCurrentFileName = "";
}

void SoundEditDialog::OnCheckBoxPlayInGame( wxCommandEvent &event )
{
    mPlaySoundInGame = event.IsChecked();

    _StopAllSounds();

    mCurrentSoundItem = NULL;

    mPlaySound = true;
}

void SoundEditDialog::OnUpdateCheckBoxPlayInGame(wxUpdateUIEvent& e)
{
    e.Check(mPlaySoundInGame);
}

void SoundEditDialog::_StopAllSounds(void)
{
    for (size_t i=0; i<mSoundItems.size(); ++i)
    {
        SoundItem* workingItem = mSoundItems[i];

        workingItem->mCurrentPlayIntervalTime = 0;
        workingItem->mCurrentRepeatIntervalTime = 0;
        workingItem->mCurrentRepeatTime = 0;
        
        _StopSound(workingItem->mSoundHandle);

        workingItem->mSoundHandle = -1;
    }
}

void SoundEditDialog::_SetSoundVolume(int handle, float gain)
{
    if (wxGetApp().funcSetSoundVolume)
    {
        wxGetApp().funcSetSoundVolume(handle, gain);
    }
}
