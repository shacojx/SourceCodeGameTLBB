#ifndef __MaterialPreviewDialog_H__
#define __MaterialPreviewDialog_H__

#include <wx/image.h>
#include <wx/dialog.h>
#include <list>
#include <OgreStringInterface.h>

const int ID_STATICBITMAP = 10000;
const int ID_TEXT_MATERIAL_NAME = 10001;
const int ID_BUTTON_USE = 10002;

namespace WX	{
	class SceneManipulator;
}

namespace Ogre	{
	class Image;
}

#include "WXMaterialEditorDialog.h"

class MaterialPreviewDialog : public wxDialog
{

	DECLARE_DYNAMIC_CLASS(MaterialPreviewDialog)

public:
	MaterialPreviewDialog() {}

	// full ctor
	MaterialPreviewDialog(wxWindow *parent, wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_DIALOG_STYLE,
		const wxString& name = wxDialogNameStr)
		: wxDialog(parent, id, title, pos, size, style, name),
		EFFECT_PATH("../Media/EffectSystem/"), mPreviewImageWidth(64), mPreviewImageHeight(64),
		mSelectedMatName("")
	{
		mCurrentPreviewImage.Create( mPreviewImageWidth, mPreviewImageHeight, true );
		mParentDialog = static_cast<MaterialEditorDialog *>(parent);
	}

	void InitPreviewDialog( WX::SceneManipulator *manipulator )
	{
		assert (manipulator);
		mSceneManipulator = manipulator;
	}

	~MaterialPreviewDialog();

protected:

	void InitPreviewGrids(void);

	void buildPreviewBitmap( const Ogre::String &texName );
	Ogre::Image * getPreviewImage( const Ogre::String &textureName );
	void clearPreviewImage(void);
	wxBitmap TexturePreview( size_t index );

	DECLARE_EVENT_TABLE()

	void OnInitDialog(wxInitDialogEvent &e);	
	void OnUseButtonDown(wxCommandEvent &e);
protected:
	Ogre::String EFFECT_PATH;

	size_t mPreviewImageWidth;
	size_t mPreviewImageHeight;

	/// 当前用于显示的wxImage
	wxImage mCurrentPreviewImage;

	WX::SceneManipulator *mSceneManipulator;

	typedef std::map<Ogre::String, Ogre::Image *> PreviewImageMap;
	PreviewImageMap mPreviewImageMap;

	Ogre::String mSelectedMatName;

	MaterialEditorDialog *mParentDialog;
};

#endif // __MaterialPreviewDialog_H__
