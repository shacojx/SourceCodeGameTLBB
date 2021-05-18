// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "ReshapeDialog.h"
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

// ----------------------------------------------------------------------------
// Other headers
// ----------------------------------------------------------------------------

#include "ReshapeDialog.h"

// ----------------------------------------------------------------------------
// Resources
// ----------------------------------------------------------------------------

#include "res/ReshapeDlg_wdr.h"

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
// ReshapeDialog class
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(ReshapeDialog, wxDialog)

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(ReshapeDialog, wxDialog)
EVT_INIT_DIALOG(ReshapeDialog::OnInitDialog)
END_EVENT_TABLE()

void
ReshapeDialog::OnInitDialog(wxInitDialogEvent &e)
{
	wxDialog::OnInitDialog(e);

	wxSizer* sizer = ReshapeDlg(this, true, true);

	/// 获取各控件的引用
	mCmbTexture = wxDynamicCast(this->FindWindow(ID_TEXTURENAME), wxComboBox);
	mLeftCtrl = wxDynamicCast(this->FindWindow(ID_LEFTVALUE),wxTextCtrl);
	mTopCtrl = wxDynamicCast(this->FindWindow(ID_TOPVALUE),wxTextCtrl);
	mRightCtrl = wxDynamicCast(this->FindWindow(ID_RIGHTVALUE ),wxTextCtrl);
	mBottomCtrl = wxDynamicCast(this->FindWindow(ID_BOTTOMVALUE),wxTextCtrl);

	InitComboBox();
}

void 
ReshapeDialog::InitComboBox()
{
	mCmbTexture->Clear();
	pathNameMap.clear();
	ReloadTextureList();
}


void
ReshapeDialog::ReloadTextureList(void)
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
		}
	}
}



