// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "NewSceneDialog.h"
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

#include "NewSceneDialog.h"

// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------

#include "res/NewScene_wdr.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------

#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreCodec.h>

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

// ----------------------------------------------------------------------------
// NewSceneDialog class
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(NewSceneDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(NewSceneDialog, wxDialog)
    EVT_INIT_DIALOG(NewSceneDialog::OnInitDialog)
END_EVENT_TABLE()

void
NewSceneDialog::OnInitDialog(wxInitDialogEvent &e)
{
    wxDialog::OnInitDialog(e);

    wxSizer* sizer = NewScene(this, true, true);

    /// 获取各控件的引用
    mCmbTexture = wxDynamicCast(this->FindWindow(ID_COMBO), wxComboBox);
    mXsizeTextCtrl = wxDynamicCast(this->FindWindow(ID_XSIZETEXTCTRL),wxTextCtrl);
    mZsizeTextCtrl = wxDynamicCast(this->FindWindow(ID_ZSIZETEXTCTRL),wxTextCtrl);
    mSeedTextCtrl = wxDynamicCast(this->FindWindow(ID_SEEDTEXTCTRL ),wxTextCtrl);
    mScaleTextCtrl = wxDynamicCast(this->FindWindow(ID_SCALETEXTCTRL),wxTextCtrl);
    mHeightScaleTextCtrl = wxDynamicCast(this->FindWindow(ID_HEIGHTSCALECTRL),wxTextCtrl);
    mTileSizeTextCtrl = wxDynamicCast(this->FindWindow(ID_TILESIZETEXTCTRL),wxTextCtrl);
    mAlogrithmCtrl = wxDynamicCast(this->FindWindow(ID_CHOICE),wxComboBox);

    
    InitComboBox();
    InitChoice();

}

static const char* strTbls[] = {
    "Diamond Square",
    "Fluid Simulation",
    "Box Filtering",
    "Hill Generate",
    "Frac SynthPass"
};

void
NewSceneDialog::InitChoice()
{
    mAlogrithmCtrl->Clear();
    for(int i=0;i<sizeof(strTbls)/sizeof(char*);i++)
    {
        mAlogrithmCtrl->AppendString(wxT(strTbls[i]));
    }
    mAlogrithmCtrl->Select(0);
}

void 
NewSceneDialog::InitComboBox()
{
    mCmbTexture->Clear();
    pathNameMap.clear();
    firstJpgItemText.clear();
    ReloadTextureList();
    SetFirstJpg();
    InitEngineSeed();
}


void
NewSceneDialog::ReloadTextureList(void)
{

    if (!Ogre::ResourceGroupManager::getSingletonPtr())
        return;

    Ogre::Codec::CodecIterator it = Ogre::Codec::getCodecIterator();
    while (it.hasMoreElements())
    {
        const Ogre::String& ext = it.peekNextKey();
        Ogre::Codec* codec = it.getNext();
        if (codec->getDataType() != "ImageData")
            continue;

        Ogre::FileInfoListPtr fileInfoList =
            Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
            AS_STRING(mResourceGroup),
            "*." + ext);
        for (Ogre::FileInfoList::const_iterator it = fileInfoList->begin(); it != fileInfoList->end(); ++it)
        {
            Ogre::String path, baseName;
            Ogre::StringUtil::splitFilename(it->filename, baseName, path);


          

            // 把相对路径的最后一个/去掉
            if (!path.empty() && path[path.length()-1] == '/')
                path.erase(path.length() - 1);

            Ogre::String value;
            if(path.empty())
            {
                value = baseName;
            }
            else
            {
                value = path + "/" + baseName;
            }
            
            mCmbTexture->AppendString( wxT(baseName) );
            pathNameMap.insert(make_pair(baseName,value));

            if(ext == "jpg" && firstJpgItemText == "")
            {
                firstJpgItemText = baseName;
            }
        }
    }
}

void
NewSceneDialog::SetFirstJpg()
{
    if(firstJpgItemText != "")
        mCmbTexture->SetLabel(firstJpgItemText);
}

void 
NewSceneDialog::InitEngineSeed()
{
    const int range = 32;
    srand(time(NULL));

    char strBuf[8];
    memset(strBuf,0,8);
    sprintf(strBuf,"%d",rand() % range);

    wxString str = wxT(strBuf);
    mSeedTextCtrl->SetLabel(str);
}