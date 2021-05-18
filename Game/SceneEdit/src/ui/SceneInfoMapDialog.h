#ifndef __SCENEINFOMAPDIALOG_H__
#define __SCENEINFOMAPDIALOG_H__



#include <wx/dialog.h>
#include <list>
#include <map>
#include <OgreString.h>
#include <OgreImage.h>

#include <wx/panel.h>

#include <wx/image.h>
#include <wx/scrolwin.h>

#include "Core/WXObject.h"
#include "WXSceneListener.h"

#include <OgreVector2.h>
#include <vector>

namespace WX {

    class SceneManipulator;
	class CGrowPointEditAction;
}


class DnDCanvasBitmap : public wxScrolledWindow
{
public:
    DnDCanvasBitmap(wxWindow *parent,int id);
protected:
    DECLARE_EVENT_TABLE()
    void OnEraseBackground(wxEraseEvent& e);
    void OnSize(wxSizeEvent& event);
    void OnPaint(wxPaintEvent& event);
    void DrawHTBitmap(wxPaintDC& dc);
    void DrawObjectPoints(wxPaintDC& dc);
    void DrawObjectPoints2(wxPaintDC& dc);
    void DrawNavigator(wxPaintDC& dc);
    void DrawPlanes(wxPaintDC& dc,bool up,wxPoint* pPtCorners);
    //void DrawObjectPoints2(wxPaintDC& dc);


    void OnMouseMove(wxMouseEvent& e);
    void OnLeftDown(wxMouseEvent& e);
    void OnLeftUp(wxMouseEvent& e);
	void OnScroll(wxScrollWinEvent& event);


    void GetTheRealPoint(wxPoint& pt,wxRect& mRect,int& width,int& height);
    int GetNum(wxPoint* pt,int num,bool first,bool max);
    bool InNavigator(wxMouseEvent& e);

    bool mBeginDrag;
    wxPoint mDragPt;
    Ogre::Vector2 offVec;
    wxPoint mCameraPos;

    //选取区域
    wxRect upRectBounding;
    wxRect downRectBounding;

    //居中显示
    int offsetx;
    int offsety;

public:    
    void SetRect(const wxRect& rect);
    wxRect mRect;
};

class SceneInfoMapDialog : public wxPanel, WX::SceneListener
{  
    typedef std::map<wxString,wxColour>TextureColourMap;

    typedef std::set<wxString> StringSet;
    typedef std::map<wxString, wxColour> FactoryColourMap;

    enum
    {
        Control_First = 1000,
        Control_Listbox,
        ID_RADIOBOXSIZE,
        ID_RADIOBOXHT,
        ID_SCROLLWIN,
    };

public:
    SceneInfoMapDialog() {}
    SceneInfoMapDialog(wxWindow *parent,
        wxWindowID winid = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = "SceneInfoMap");
    ~SceneInfoMapDialog();

public: 
    typedef std::map<WX::ObjectPtr,wxPoint> Obj2Pos;

    Obj2Pos objPos;
    StringSet selectedTypes;
    FactoryColourMap factoryColour;
    TextureColourMap textureColour;

    DnDCanvasBitmap* mScrWin;
    wxCheckListBox *mListBox;
    wxRadioBox* mRadioSize;
    wxRadioBox* mRadioHTMode;

    wxImage mHTImages[2];
    wxBitmap mHTBitmaps[2];

    wxImage mPointImage;
    wxBitmap mPointBitmap;

    //Terrain
    float minHeight;
    float maxHeight; 
    float sumHeight;

    int mWidth;
    int mHeight;

    int mWidthSize;
    int mHeightSize;
    //Refresh
    bool mEraseBk;
    bool mFirst;
    bool mOK;
    bool mOK2;
    bool again;

    //mode
    enum DisplayMode
    {
        EDSM_FIXSIZE,
        EDSM_SCROLL,
    };    
    DisplayMode modeSize;

    enum HeightTextureMode
    {
        EHTM_HEIGHT,
        EHTM_TEXTURE
    };
    HeightTextureMode modeHT;

    WX::SceneManipulator* mSceneManipulator;
public:
    void CreateBitmapByHTMode();
    void CreateHeightMapBitmap();
    void CreateTextureBitmap();
    void CreateSceneObjectDisplay();

    void InitSceneInfoMap();
    void SetSceneManipulator(WX::SceneManipulator* sceneManipulator);
    WX::SceneManipulator* GetSceneManipulator()
    {
        return mSceneManipulator;
    }
    void RecreatePointImage();
   
protected:

    DECLARE_EVENT_TABLE()
    void OnMouseMove(wxMouseEvent& e);

    void OnInitDialog();

    virtual void onAddObject(const WX::ObjectPtr& object);
    virtual void onRemoveObject(const WX::ObjectPtr& object);
    virtual void onSceneReset();
    virtual void onObjectPropertyChanged(const WX::ObjectPtr& object, const Ogre::String& name);
    virtual void onTerrainHeightChanged(const std::vector<WX::TerrainInfo>& terrInfo);
    virtual void onCameraPosChanged();

    void AdjustColour(int index, const wxColour& colour);  

    void InitCheckListbox(void);
    void OnListboxSelect(wxCommandEvent& event);
    void OnCheckboxToggle(wxCommandEvent& event);
    void OnListboxDblClick(wxCommandEvent& event);

    void OnPatternSize(wxCommandEvent &event );
    void OnPatternHT(wxCommandEvent& event);


    void SetImageRGB(int x,int y,unsigned char r,unsigned char g,unsigned char b,HeightTextureMode mode);
    long XYToIndex(int x, int y) const;
    void SetPointRGB(const WX::ObjectPtr& object,const wxPoint& point,bool erase = false);
	long XYToIndex2(int x,int y) const;
    bool GetRefreshRect(const wxPoint& oldPoint,wxRect& refreshRect);


public:

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 供逻辑数据编辑部分使用.
	//
	
	void SetPointRGB(float fx, float fz, int iR, int iG, int iB);

	// 得到生长点action
	WX::CGrowPointEditAction* GetGrowPointAction();

	// 刷新生长点信息
	void RefreshGrowPointInfo();
	
    
public:
    const wxColour& GetPointColour(const WX::ObjectPtr& object);
    bool InInterestType(const wxString& type);
};

#endif // __SceneInfoMapDialog_H__
