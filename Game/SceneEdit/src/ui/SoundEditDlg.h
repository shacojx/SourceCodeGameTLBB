#ifndef __SOUNDEDITDIALOG_H__
#define __SOUNDEDITDIALOG_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "SoundEditDialog.h"
#endif

#include <wx/panel.h>

#include "WXSceneListener.h"

#include <OgreFrameListener.h>

class wxListBox;
class wxTextCtrl;
class wxComboBox;

namespace WX
{
    class SceneManipulator;  
}

/// 音效表结构
struct SoundItem
{
    int mInstanceID;
    int mSoundID;
    int mRadius;
    int mXPos;
    int mZPos;
    int mRepeatTime;
    int mRepeatIntervalTime;
    int mNextRepeatTime;

    int mCurrentRepeatTime;
    int mCurrentRepeatIntervalTime;
    int mCurrentPlayIntervalTime;
    int mSoundHandle;
};

/// 环境音效编辑对话框
class SoundEditDialog : public wxPanel, WX::SceneListener, Ogre::FrameListener
{
public:

    SoundEditDialog(wxWindow *parent,
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = "SoundEditDialog")
        : mSceneManipulator(NULL)
        , mSoundListBox(NULL)
        , mRadiusTextCtrl(NULL)
        , mXPosTextCtrl(NULL)
        , mZPosTextCtrl(NULL)
        , mRepeatTimeTextCtrl(NULL)
        , mRepeatIntervalTimeTextCtrl(NULL)
        , mNextRepeatTimeTextCtrl(NULL)
        , mWorkingSoundIndex(-1)
        , mSoundNameComboBox(NULL)
        , mSoundInfoFileChanged(false)
        , mCurrentSoundHandle(-1)
        , mCurrentSoundItem(NULL)
        , mCurrentRepeatTime(0)
        , mCurrentRepeatIntervalTime(0)
        , mCurrentPlayIntervalTime(0)
        , mPlaySound(false)
        , mSoundIDText(NULL)
        , mPlaySoundInGame(false)
    {
        Create(parent, winid, pos, size, style, name);
    }

    ~SoundEditDialog();

    bool Create(wxWindow *parent,
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = "SoundEditDialog");

    void SetSceneManipulator(WX::SceneManipulator* manipulator);

    bool Show(bool show = true);

    virtual void onUIChanged(void* data, WX::UIChangeType type);

    void LoadSoundInfoFromFile(const Ogre::String& filename);
    void SaveSoundInfoFromFile(const Ogre::String& filename);
    void ClearSoundData(void);

    virtual bool frameStarted(const Ogre::FrameEvent& evt);

    DECLARE_EVENT_TABLE()

    void OnBeginAddSound( wxCommandEvent &event );
    void OnEndAddSound( wxCommandEvent &event );
    void OnListBoxSoundItemDoubleClick( wxCommandEvent &event );
    void OnSoundItemTextChanged(wxCommandEvent &e);
    void OnComboSoundNames(wxCommandEvent &e);
    void OnDeleteSound( wxCommandEvent &event );
    void OnSaveSound( wxCommandEvent &event );

    void OnCheckBoxPlayInGame( wxCommandEvent &event );
    void OnUpdateCheckBoxPlayInGame(wxUpdateUIEvent& e);

protected:

    void _FillSoundListCtrl(void);
    void _FillSoundNameComboBox(void);
    void _ClearSoundItems(void);
    void _DeleteSoundItem(SoundItem* soundItem, int index);
    int _PlaySound(const Ogre::String& soundName, int stopSoundHandle, bool loop = true);
    void _StopSound(int soundHandle);
    void _StopAllSounds(void);
    void _SetSoundVolume(int handle, float gain);

protected:

    WX::SceneManipulator* mSceneManipulator;

    typedef std::vector<SoundItem*> SoundItems;
    SoundItems mSoundItems;

    /// 当前所选中的音效索引
    int mWorkingSoundIndex;
    /// 当前所选的音效项
    SoundItem* mCurrentSoundItem;

    wxListBox* mSoundListBox;
    wxTextCtrl* mRadiusTextCtrl;
    wxTextCtrl* mXPosTextCtrl;
    wxTextCtrl* mZPosTextCtrl;
    wxTextCtrl* mRepeatTimeTextCtrl;
    wxTextCtrl* mRepeatIntervalTimeTextCtrl;
    wxTextCtrl* mNextRepeatTimeTextCtrl;
    wxComboBox* mSoundNameComboBox;
    wxStaticText* mSoundIDText;

    /// 从表中读入的音效表，key为音效ID,value为音效的名称
    typedef std::map<int, Ogre::String> SoundNames;
    SoundNames mSoundNames;

    /// 当前场景的音效是否经过了修改，如果是，就需要保存文件
    bool mSoundInfoFileChanged;
    bool mPlaySound;

    /// 当前播放的音效handle
    int mCurrentSoundHandle;

    int mCurrentRepeatTime;
    int mCurrentRepeatIntervalTime;
    int mCurrentPlayIntervalTime;

    /// 这个场景的esd文件名
    Ogre::String mCurrentFileName;

    bool mPlaySoundInGame;
};

#endif