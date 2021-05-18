/********************************************************************
filename:   MiniMapMakerDialog.cpp

purpose:    用于设置地形材质的对话框
*********************************************************************/

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "MiniMapMakerDialog.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#include <wx/scrolwin.h>
#include <wx/toolbar.h>
#include <wx/stattext.h>
#include <wx/button.h>
#endif

#include <wx/busyinfo.h>

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

#include "MiniMapMakerDialog.h"

// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------

#include "res/MiniMapMaker_wdr.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------
#include <OgreStringConverter.h>

IMPLEMENT_DYNAMIC_CLASS(MiniMapMakerDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(MiniMapMakerDialog, wxDialog)

    EVT_INIT_DIALOG(MiniMapMakerDialog::OnInitDialog)
    EVT_BUTTON(ID_BUTTON_CREATE_MINI_MAP, MiniMapMakerDialog::OnCreateMiniMap)

END_EVENT_TABLE()

void
MiniMapMakerDialog::OnInitDialog(wxInitDialogEvent &e)
{
    wxDialog::OnInitDialog(e);
    wxSizer* sizer = MiniMapMaker(this, true, true);

    mFileNameText = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_FILE_NAME), wxTextCtrl);
    mTileSizeText = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_TILE_SIZE), wxTextCtrl);
    mTileTexWidthText = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_TILE_TEX_WIDTH), wxTextCtrl);
    mTileTexHeightText = wxDynamicCast(this->FindWindow(ID_TEXTCTRL_TILE_TEX_HEIGHT), wxTextCtrl);
    mTexExtensionCombo = wxDynamicCast(this->FindWindow(ID_COMBO_TEX_EXTENSION), wxComboBox);
    mDelTempTexturesCheckBox = wxDynamicCast(this->FindWindow(ID_CHECKBOX_DEL_TEMP_FILES), wxCheckBox);
    mCameraDirType = wxDynamicCast(this->FindWindow(ID_COMBO_CAMERA_DIRECTION), wxComboBox);

    assert ( mFileNameText && mTileSizeText && mTileTexWidthText && mTileTexHeightText && mTexExtensionCombo
        && mDelTempTexturesCheckBox && mCameraDirType );

    if (!mMiniMapMaker)
        mMiniMapMaker = new WX::MiniMapMaker(mManipulator);

  //  mMiniMapMaker->init();

    mMiniMapMaker->setCurrentSceneName( AS_STRING(mCurrentSceneName) );

    // 设置初始值
    mFileNameText->SetValue( mMiniMapMaker->getBaseName().c_str() );
    mTileSizeText->SetValue( Ogre::StringConverter::toString(mMiniMapMaker->getTileSize()).c_str() );
    mTileTexWidthText->SetValue( Ogre::StringConverter::toString(mMiniMapMaker->getTexWidth()).c_str() );
    mTileTexHeightText->SetValue( Ogre::StringConverter::toString(mMiniMapMaker->getTexHeight()).c_str() );
    mTexExtensionCombo->SetValue( mMiniMapMaker->getTexExtension().c_str() );

    mCameraDirType->SetSelection(0);
}

MiniMapMakerDialog::~MiniMapMakerDialog()
{
    if (mMiniMapMaker)
    {
        delete mMiniMapMaker;
        mMiniMapMaker = NULL;
    }
}

void MiniMapMakerDialog::OnCreateMiniMap(wxCommandEvent& event)
{
    wxBusyInfo busyInfo(_("Creating mini map ..."), this);

    mMiniMapMaker->setTileSize(Ogre::StringConverter::parseReal(mTileSizeText->GetValue().c_str()));
    mMiniMapMaker->setTexWidth(Ogre::StringConverter::parseReal(mTileTexWidthText->GetValue().c_str()));
    mMiniMapMaker->setTexHeight(Ogre::StringConverter::parseReal(mTileTexHeightText->GetValue().c_str()));
    mMiniMapMaker->setOutputFileName( mFileNameText->GetValue().c_str() );
    mMiniMapMaker->setTexExtension( mTexExtensionCombo->GetValue().c_str() );

    mMiniMapMaker->setCameraDirectionType( getCameraDirType() );

    bool resultOutput = mMiniMapMaker->outputTextures();

    if (false == resultOutput)
    {
        wxMessageBox( mMiniMapMaker->getLastErrorString().c_str() );
        return;
    }

    // 删除临时文件
    if (mDelTempTexturesCheckBox->IsChecked())
    {
        std::vector<Ogre::String>& fileNames = mMiniMapMaker->getTempFileNames();

        for ( size_t i=0; i<fileNames.size(); ++i )
        {
            ::wxRemoveFile(fileNames[i].c_str());
        }
    }    

    wxMessageBox(_("Success Creating Mini Map!"));
}

WX::MiniMapMaker::CameraDirectionType 
MiniMapMakerDialog::getCameraDirType(void)
{
    wxString cdtString = mCameraDirType->GetValue();

    if (cdtString == "MiniMap")
        return WX::MiniMapMaker::CDT_MiniMap;

    else if (cdtString == "CameraDirection")
        return WX::MiniMapMaker::CDT_CameraDirection;

    else if (cdtString == "North")
        return WX::MiniMapMaker::CDT_North;

    else if (cdtString == "South")
        return WX::MiniMapMaker::CDT_South;

    else if (cdtString == "West")
        return WX::MiniMapMaker::CDT_West;

    else if (cdtString == "East")
        return WX::MiniMapMaker::CDT_East;

    else
    {
        wxMessageBox(_("wrong camera diretion type!"));
        return WX::MiniMapMaker::CDT_MiniMap;
    }
}