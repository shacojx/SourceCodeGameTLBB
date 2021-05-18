#ifndef __BrushSelector_H__
#define __BrushSelector_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "BrushSelector.h"
#endif

#include <wx/panel.h>
#include <wx/image.h>

#include <OgreString.h>
#include "PaintAction.h"
#include "TerrainPaintInfoContainer.h"

// ----------------------------------------------------------------------------
// forward declares
// ----------------------------------------------------------------------------

class wxSplitterWindow;
class wxTreeCtrl;

namespace WX {
    class SceneManipulator;
    class Action;
}

// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

class BrushSelector : public wxPanel
{
	/// 保存图片所在的相对路径与wxTreectrl中的节点之间的对应关系，这个相对路径不一定是图片的整个相对路径
	/// 比如路径如下：a/b/c.jpg，那么这个map中就会保存有三个元素，分别为a<-->id1，b<-->id2，c.jpg<-->id3
	/// 所以，在用户选取纹理图时，还得从下往上组合路径，组成a/b/c.jpg
	typedef std::map< Ogre::String, wxTreeItemId> CatalogMap;

public:
    BrushSelector(const wxString& resourceGroup = wxT("Brushes"))
    {
        Init(resourceGroup);
    }

	~BrushSelector();

    BrushSelector(wxWindow *parent, const wxString& resourceGroup = wxT("Brushes"),
                  wxWindowID winid = wxID_ANY,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = wxTAB_TRAVERSAL | wxNO_BORDER,
                  const wxString& name = "BrushSelector")
    {
        Init(resourceGroup);

        Create(parent, winid, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID winid = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL | wxNO_BORDER,
                const wxString& name = "BrushSelector");

    void SetSceneManipulator(WX::SceneManipulator* sceneManipulator);

    WX::SceneManipulator* GetSceneManipulator(void) const
    {
        return mSceneManipulator;
    }

	/// 重新检索资源目录中的纹理，并建控件树
    void ReloadTextureList(void);
    void ReloadTextureList(const wxString& resourceGroup);

protected:
    WX::SceneManipulator* mSceneManipulator;
    wxSplitterWindow* mSplitter;

	/// 保存纹理名称的控件
    wxTreeCtrl *mBrushesTree;
	/// 当前用于显示的wxImage
	wxImage mCurrentPreviewImage;
    /// 黑白图
    wxImage mBlackWhitePreviewImage;

    int mPreviewImageWidth;
    int mPreviewImageHeight;

    /// 显示所选纹理缩略图的控件
	wxStaticBitmap *mBrushesPreview;

	/// 纹理对话框中的四个checkbox的引用
	wxCheckBox *mFlipHorizontal;
	wxCheckBox *mFlipVertical;
	wxCheckBox *mRotateRightAngle;
	wxCheckBox *mRandom;
    wxCheckBox *mMirrorDiagonal;

	wxCheckBox *mEnablePreview;

	/// 当前纹理的转向
	int mCurrentOrientationType;
    /// 是否使用随机的方向
    bool mIsRandomOrientation;

	/// 当前纹理的名称
	wxString mCurrentFileName;
   
    wxString mResourceGroup;

    typedef std::vector< WX::PaintPixmap > FullTerrainPaintPixmapArray;

    FullTerrainPaintPixmapArray mPixmapArray;

    bool mPaintInOneGrid;

protected:

    void Init(const wxString& resourceGroup);

	/** 根据传入的路径名，获取它wxTreeCtrl控件的父节点的id，并把相应
	    的对应关系保存到map中
	*/
	wxTreeItemId  GetParentId( Ogre::String &catalog, CatalogMap &map );

    void buildPreviewBitmap( const WX::TextureInfo texInfo );
    //////////////////////////////////////////////////////////////////////////

    DECLARE_EVENT_TABLE()

    void OnUpdateControls(wxUpdateUIEvent& e);

    void OnPixmapListSelected(wxTreeEvent& e);

	void OnCheck( wxCommandEvent &event );

	void OnReloadTextureList( wxCommandEvent &event );

    const WX::Action * OnStartAutoTexAction( const wxTreeItemId &itemId, const wxString &brushName );

    // 在画刷上点击鼠标右键
    void OnRightButtonUp(wxTreeEvent& event);
    // 显示鼠标右键在brush菜单上点击时的菜单（并进行一些检测）
    void ShowBrushRightButtonMenu(const wxTreeItemId &itemId);
    // 进行全地形的纹理平铺操作
    void OnFullTerrainPaint(wxCommandEvent& event);
    void OnPaintInOneGrid(wxCommandEvent& event);
};

#endif // __BrushSelector_H__
