// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "SceneInfoMapDialog.h"
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

#include <wx/colordlg.h>

#include "SceneInfoMapDialog.h"

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------

#include <OgreCamera.h>
#include <OgreImage.h>

#include "WXEditorApp.h"
#include "SceneManipulator.h"

#include "WXEditorApp.h"
#include ".\growpointeditaction.h"

#include "Core/TerrainData.h"
#include "Core/WXSceneInfo.h"
#include "Core/WXVariant.h"
#include "Core/WXFactory.h"
#include "Core/WXObjectFactoryManager.h"
#include "Core/WXResources.h"


// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif


static const int pointSize = 3;//颜色点的大小,以像素点为单位
static const int cameraDownY = 50; 

static const wxColour initColorTbl[] =
{
        0xff0000,
        0x00ff00,
        0x0000ff,

        0xffff00,
        0xff00ff,
        0x00ffff,
};
// ----------------------------------------------------------------------------
// SceneInfoMapDialog class
// ----------------------------------------------------------------------------
BEGIN_EVENT_TABLE(SceneInfoMapDialog, wxPanel)
    EVT_CHECKLISTBOX(Control_Listbox, SceneInfoMapDialog::OnCheckboxToggle)
    EVT_LISTBOX_DCLICK(Control_Listbox, SceneInfoMapDialog::OnListboxDblClick)
    EVT_RADIOBOX(ID_RADIOBOXSIZE,SceneInfoMapDialog::OnPatternSize)
    EVT_RADIOBOX(ID_RADIOBOXHT,SceneInfoMapDialog::OnPatternHT)
END_EVENT_TABLE()

SceneInfoMapDialog::SceneInfoMapDialog(wxWindow *parent,
                                       wxWindowID winid,
                                       const wxPoint& pos,
                                       const wxSize& size,
                                       long style,
                                       const wxString& name)
{
    wxPanel::Create(parent, winid, pos, size, style, name);

}

SceneInfoMapDialog::~SceneInfoMapDialog()
{
}

//////////////////////////////////////////////
///initialize
//////////////////////////////////////////////
void
SceneInfoMapDialog::SetSceneManipulator(WX::SceneManipulator* sceneManipulator)
{
    mSceneManipulator = sceneManipulator;
    GetSceneManipulator()->addSceneListener(this);

    OnInitDialog();

    mScrWin->EnableScrolling( false ,false); 
}

void
SceneInfoMapDialog::OnInitDialog()
{     
    static const wxString strModeSize[] =
    {
        _("充满窗口"),
        _("实际大小")
    };
    static const wxString strModeHT[] =
    {
        _("高度图"),
        _("贴图")
    };

    objPos.clear();
    selectedTypes.clear();
    factoryColour.clear();
    textureColour.clear();

    mScrWin = NULL;
    mListBox = NULL;
    mRadioSize = NULL;
    mRadioHTMode = NULL;

    minHeight = maxHeight = sumHeight = 0.0f;
    mWidth = mHeight = 0;
	modeSize = EDSM_FIXSIZE;//__??
	modeHT = EHTM_HEIGHT;//__??

    WX::ObjectFactoryManager* factoryMgr = GetSceneManipulator()->getObjectFactoryManager();
    WX::ObjectFactoryManager::FactoryMap::const_iterator iter;
    for( iter = factoryMgr->getFactories().begin();
        iter != factoryMgr->getFactories().end(); iter++)
    {
        wxString strType = AS_STRING(iter->first);
        factoryColour[strType] = *wxBLACK;
        selectedTypes.insert(strType);
    }

    wxFlexGridSizer *topsizer = new wxFlexGridSizer(1, 0, 0);
    topsizer->AddGrowableCol(0);
    topsizer->AddGrowableRow(0);

    mScrWin =  new DnDCanvasBitmap( this ,ID_SCROLLWIN );
    topsizer->Add(mScrWin, 0, wxGROW|wxALL, 5);
    mScrWin->SetBestFittingSize(wxSize(64, 64));

    //创建CheckList
    mListBox = new wxCheckListBox(this, Control_Listbox, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE);
    topsizer->Add(mListBox, 0, wxGROW|wxALL, 5);
    InitCheckListbox();

    //创建RadioBox
    mRadioSize = new wxRadioBox(this, ID_RADIOBOXSIZE, _("缩略图"), wxDefaultPosition, wxDefaultSize,
        WXSIZEOF(strModeSize), strModeSize, 1, wxRA_SPECIFY_ROWS);
    mRadioHTMode = new wxRadioBox( this, ID_RADIOBOXHT, _("高度图"), wxPoint(100,30), wxSize(-1,-1), 
                              WXSIZEOF(strModeHT), strModeHT, 1, wxRA_SPECIFY_ROWS );

    topsizer->Add(mRadioSize, 0, wxGROW|wxALL, 5);
    topsizer->Add(mRadioHTMode, 0, wxGROW|wxALL, 5);

    SetSizerAndFit(topsizer);
  
    mFirst = true;
    InitSceneInfoMap();
    mFirst = false;
  
}

void SceneInfoMapDialog::InitSceneInfoMap()
{   
    modeSize = EDSM_FIXSIZE;
    modeHT = EHTM_HEIGHT;

    WX::TerrainData* data = GetSceneManipulator()->getTerrainData();

    mWidth = data->getXSize();
    mHeight = data->getZSize();

    mHTImages[0].Create(mWidth,mHeight,true);
    mHTImages[1].Create(mWidth,mHeight,true);

	mScrWin->SetRect(wxRect(wxPoint(0,0),wxSize(mWidth,mHeight)));
	mScrWin->SetScrollRate( 0 ,0 );

	mRadioSize->SetSelection(0);
	mRadioHTMode->SetSelection(0);

    RecreatePointImage();
	CreateBitmapByHTMode();
	mOK = false;

    //CreateSceneObjectDisplay();

    mEraseBk = true;
    mScrWin->Refresh();
}

void SceneInfoMapDialog::CreateBitmapByHTMode()
{
    CreateHeightMapBitmap();
    if(!mFirst)
        CreateTextureBitmap();
}

//设置bitmap,分为两种模式,一为高度图,二为地形的贴图
void SceneInfoMapDialog::CreateHeightMapBitmap()
{
    minHeight = maxHeight = sumHeight = 0.0f;

    WX::TerrainData* data = GetSceneManipulator()->getTerrainData();       
    
    for ( int i=0; i<mHeight; ++i )
    {
        for ( int j=0; j<mWidth; ++j )
        {
            float height = data->getHeight(j,i);
            sumHeight += height;        

            minHeight = std::min(minHeight,height);
            maxHeight = std::max(maxHeight,height);
        }
    }

    float diff = maxHeight - minHeight;

    for ( int i=0; i<mHeight; ++i )
    {
        for ( int j=0; j<mWidth; ++j )
        {
            unsigned char r,g,b;
            float height = data->getHeight(j,i);
            if(diff > 0)
            {
                r = g = b =  (height - minHeight)/diff * 255;

            }
            else if ( 0 == diff )
                r = g = b = 255;
            
            SetImageRGB(j,i,r,g,b,EHTM_HEIGHT);
        }
    }

}

void SceneInfoMapDialog::CreateTextureBitmap()
{
    WX::TerrainData* data = GetSceneManipulator()->getTerrainData();

    for (int i = 0; i < mHeight; ++i)
    {
        for (int j = 0; j < mWidth; ++j)
        {
            int r,g,b;
            r = g = b = 0;

            const WX::TerrainData::GridInfo& grid = data->getGridInfo(j,i);
            size_t pixmapId = grid.layers[0].pixmapId;
            wxASSERT(pixmapId <= data->mPixmaps.size());
            if (pixmapId > 0)
            {
                const WX::TerrainData::Pixmap& pixmap = data->mPixmaps[pixmapId-1];
                size_t textureId = pixmap.textureId;
                wxASSERT(textureId < data->mTextures.size());
                if (textureId < data->mTextures.size())
                {
                    const Ogre::String& textureName = data->mTextures[textureId];
                    TextureColourMap::const_iterator iter = textureColour.find(AS_STRING(textureName));
                    if (iter != textureColour.end())
                    {
                        const wxColour& colour = iter->second;
                        r = colour.Red();
                        g = colour.Green();
                        b = colour.Blue();
                    }
                    else
                    {
                        Ogre::Image image;
                        image.load(textureName, WX::BRUSH_RESOURCE_GROUP_NAME);

                        typedef unsigned char uchar;
                        const uchar* rgb = image.getData();
                        size_t bytesPerPixel = image.getBPP() / 8;
                        for (unsigned int y = 0; y < image.getHeight(); ++y)
                        {
                            for (unsigned int x = 0; x < image.getWidth(); ++x)
                            {
                                b += rgb[0];
                                g += rgb[1];
                                r += rgb[2];
                                rgb += bytesPerPixel;
                            }
                        }

                        // compute the average r,g,b
                        int area = image.getHeight() * image.getWidth();
                        r /= area;
                        g /= area;
                        b /= area;

                        wxColour colour(r,g,b);
                        std::pair<TextureColourMap::iterator, bool> inserted =
                            textureColour.insert(TextureColourMap::value_type(AS_STRING(textureName),colour));
                        assert(inserted.second);
                    }
                }
            }

            SetImageRGB(j,i,r,g,b,EHTM_TEXTURE);
        } 
    }
}

void SceneInfoMapDialog::CreateSceneObjectDisplay()
{
    WX::TerrainData* data = GetSceneManipulator()->getTerrainData();

    objPos.clear();
    if(!GetSceneManipulator()->isEmpty())
    {
        const WX::Scene::Objects& mObjects = GetSceneManipulator()->getSceneInfo()->getObjects();
        for (WX::Scene::Objects::const_iterator it = mObjects.begin(); it != mObjects.end(); ++it)
        {
            const WX::ObjectPtr& object = *it;

            if (InInterestType(AS_STRING(object->getType())) && object->hasProperty("position"))
            {
                Ogre::Vector3 pos = WX::VariantCast<Ogre::Vector3>(object->getProperty("position"));
                std::pair<int,int> par = data->getGrid(pos.x,pos.z);

                wxPoint point(par.first,par.second);
                objPos.insert(Obj2Pos::value_type(object,point));

                wxRect rectRefresh;
                if(GetRefreshRect(point,rectRefresh))
                {
                    SetPointRGB(object,rectRefresh.GetLeftTop());
                }
                
            }

        }
    }

    

}

//对象点的image与控件显示区域等大
void 
SceneInfoMapDialog::SetPointRGB(const WX::ObjectPtr& object,const wxPoint& point,bool erase)
{    
    wxPoint newPoint = point;
    if(modeSize != EDSM_FIXSIZE)
    {
        int offsetx = ( mScrWin->GetSize().GetWidth()-std::min(mScrWin->mRect.GetWidth(),mScrWin->GetSize().GetWidth()))/2;
        int offsety = ( mScrWin->GetSize().GetHeight()-std::min(mScrWin->mRect.GetHeight(),mScrWin->GetSize().GetHeight()))/2;
       
        newPoint.x -= offsetx;
        newPoint.y -= offsety;  
    }
    

    for(int y=newPoint.y;y<newPoint.y+pointSize;y++)
    {
        for(int x=newPoint.x;x<newPoint.x+pointSize;x++)
        {
            long pos = XYToIndex2(x,y);

            if(pos == -1)
                continue;

            if(erase)
            {
                mPointImage.SetRGB(x,y,0,0,0);
            }
            else
            {    
                const wxColour& color = GetPointColour(object);
                mPointImage.SetRGB(x,y,color.Red(),color.Green(),color.Blue());
            }
        }
    }
    

}



void 
SceneInfoMapDialog::SetPointRGB(float fx, float fz, int iR, int iG, int iB)
{   

	WX::TerrainData* data = GetSceneManipulator()->getTerrainData();
    std::pair<int,int> par = data->getGrid(fx, fz);
    wxPoint point(par.first,par.second);

	long index = XYToIndex(par.first,par.second);

    //待添加object在地形外
    bool refresh = false;
    if(index == -1)
    {
       if(modeSize == EDSM_SCROLL)
		   mEraseBk = true;

       refresh = true;
    }
    else
    {
        if(modeSize == EDSM_SCROLL)
        {
            int x,y;
            mScrWin->GetViewStart(&x,&y);

            wxRect rect(x,y,mScrWin->GetSize().GetWidth(),mScrWin->GetSize().GetHeight());
            if(rect.Inside(point))
            {
                refresh = true;
            }
        }
        else
        {
            refresh = true;
        }
    }
	if(refresh)
    {

        wxRect rectRefresh;
        GetRefreshRect(point,rectRefresh);
		wxPoint newPoint = rectRefresh.GetLeftTop();
		if(modeSize != EDSM_FIXSIZE)
		{
			int offsetx = ( mScrWin->GetSize().GetWidth()-std::min(mScrWin->mRect.GetWidth(),mScrWin->GetSize().GetWidth()))/2;
			int offsety = ( mScrWin->GetSize().GetHeight()-std::min(mScrWin->mRect.GetHeight(),mScrWin->GetSize().GetHeight()))/2;
	       
			newPoint.x -= offsetx;
			newPoint.y -= offsety;  
		}
	    

		for(int y=newPoint.y;y<newPoint.y+pointSize;y++)
		{
			for(int x=newPoint.x;x<newPoint.x+pointSize;x++)
			{
				long pos = XYToIndex2(x,y);

				if(pos == -1)
					continue;

				//if(erase)
				//{
				//	mPointImage.SetRGB(x, y, 0, 0, 0);
				//}
				//else//
				{    
					mPointImage.SetRGB(x, y, iR, iG, iB);
				}
			}
		} 
		
		mScrWin->RefreshRect(rectRefresh);
		mOK2 = false;


   }// if(refresh)

  
    

}





////////////////////////////////////////////////////////
///helper functions
////////////////////////////////////////////////////////
bool 
SceneInfoMapDialog::InInterestType(const wxString& type)
{
    return selectedTypes.find(type) != selectedTypes.end();
}

long 
SceneInfoMapDialog::XYToIndex(int x, int y) const
{
    if ( x >= 0 && y >= 0 &&
        x < mWidth && y < mHeight )
    {
        return 1;
    }

    return -1;
}

long
SceneInfoMapDialog::XYToIndex2(int x,int y) const
{
    if ( x >= 0 && y >= 0 &&
        x < mWidthSize && y < mHeightSize )
    {
        return 1;
    }

    return -1;
}

const wxColour& 
SceneInfoMapDialog::GetPointColour(const WX::ObjectPtr& object)
{
    FactoryColourMap::iterator iter = factoryColour.find(AS_STRING(object->getType()));
    if(iter != factoryColour.end())
        return iter->second;
    else
        return *wxBLACK;
}

void 
SceneInfoMapDialog::AdjustColour(int index, const wxColour& colour)
{
    mListBox->GetItem(index)->SetBackgroundColour(colour);
    factoryColour[mListBox->GetString(index)] = colour;
}

void 
SceneInfoMapDialog::SetImageRGB(int x,int y,unsigned char r,unsigned char g,unsigned char b,HeightTextureMode mode)
{    
    long pos = XYToIndex(x, y);

    if(pos != -1)
    {
        mHTImages[mode].SetRGB(x,y,r,g,b);
    }
}
//////////////////////////////////////////////////////////////////////////
/// callback functions
//////////////////////////////////////////////////////////////////////////

void SceneInfoMapDialog::onAddObject(const WX::ObjectPtr& object)
{
    if(InInterestType(AS_STRING(object->getType())) && object->hasProperty("position"))
    {
        WX::TerrainData* data = GetSceneManipulator()->getTerrainData();

        Ogre::Vector3 pos = WX::VariantCast<Ogre::Vector3>(object->getProperty("position"));
        std::pair<int,int> par = data->getGrid(pos.x,pos.z);

        wxPoint point(par.first,par.second);
        objPos.insert(Obj2Pos::value_type(object,point));

        long index = XYToIndex(par.first,par.second);

        //待添加object在地形外
        bool refresh = false;
        if(index == -1)
        {
            if(modeSize == EDSM_SCROLL)
                mEraseBk = true;

            refresh = true;

            
        }
        else
        {
            if(modeSize == EDSM_SCROLL)
            {
                int x,y;
                mScrWin->GetViewStart(&x,&y);

                wxRect rect(x,y,mScrWin->GetSize().GetWidth(),mScrWin->GetSize().GetHeight());
                if(rect.Inside(point))
                {
                   refresh = true;
                }
            }
            else
            {
                refresh = true;
            }
        }
        if(refresh)
        {
            wxRect rectRefresh;
            GetRefreshRect(point,rectRefresh);
            SetPointRGB(object,rectRefresh.GetLeftTop());
            mScrWin->RefreshRect(rectRefresh);
            mOK2 = false;
        }
  
    }
}

void SceneInfoMapDialog::onRemoveObject(const WX::ObjectPtr& object)
{
    if(!(object->hasProperty("position")))
        return;

    Obj2Pos::iterator iter = objPos.find(object);

    if(iter != objPos.end())
    {   
        WX::TerrainData* data = GetSceneManipulator()->getTerrainData();

        float diff = maxHeight - minHeight;

        Ogre::Vector3 pos = WX::VariantCast<Ogre::Vector3>(object->getProperty("position"));
        std::pair<int,int> par = data->getGrid(pos.x,pos.z);

        long index = XYToIndex(par.first,par.second);

        //被删除object在地形外
        bool refresh = false;
        if(index == -1)
        {
            objPos.erase(iter);
            if(modeSize == EDSM_SCROLL)
                mEraseBk = true;
            
            refresh = true;
        }
        else
        {

            wxPoint point = wxPoint(par.first,par.second);   

            objPos.erase(iter);

            if(modeSize == EDSM_SCROLL)
            {
                int x,y;
                mScrWin->GetViewStart(&x,&y);

                wxRect rect(x,y,mScrWin->GetSize().GetWidth(),mScrWin->GetSize().GetHeight());
                if(rect.Inside(point))
                {    
                    refresh = true;
                }
            }
            else
            { 
                refresh = true;   
            }
        }

        if(refresh == true)
        {
            wxRect rectRefresh;
            GetRefreshRect(wxPoint(par.first,par.second),rectRefresh);
            SetPointRGB(object,rectRefresh.GetLeftTop(),true); 
            mScrWin->RefreshRect(rectRefresh);
            mOK2 = false;
        }
  
    }
    else
        wxASSERT(0);

}

void SceneInfoMapDialog::onSceneReset()
{
    InitSceneInfoMap();
}

void SceneInfoMapDialog::onObjectPropertyChanged(const WX::ObjectPtr& object, const Ogre::String& name)
{
    //只处理位置变化
    if(name == "position")
    {
        Obj2Pos::iterator iter =objPos.find(object);
        if(iter != objPos.end())
        {
            WX::TerrainData* data = GetSceneManipulator()->getTerrainData();

            Ogre::Vector3 pos = WX::VariantCast<Ogre::Vector3>(object->getProperty("position"));
            std::pair<int,int> par = data->getGrid(pos.x,pos.z);
            wxPoint oldPoint(iter->second);
            iter->second = wxPoint(par.first,par.second);

            long index = XYToIndex(par.first,par.second);
            //待添加object在地形外
            if(index == -1)
            {
                if(modeSize == EDSM_SCROLL)
                    mEraseBk = true;
            }

            if(!InInterestType(object->getType()))
                return;      

            wxRect rectRefresh;
            GetRefreshRect(oldPoint,rectRefresh);
            SetPointRGB(object,rectRefresh.GetLeftTop(),true);     
            GetRefreshRect(iter->second,rectRefresh);
            SetPointRGB(object,rectRefresh.GetLeftTop());

            mScrWin->Refresh();
            mOK2 = false;
        }
        else
            wxASSERT(0);
    }
}

bool
SceneInfoMapDialog::GetRefreshRect(const wxPoint& oldPoint,wxRect& refreshRect)
{
    int viewx,viewy;
    mScrWin->GetViewStart(&viewx,&viewy);
    wxRect subRect(0,0,mScrWin->GetSize().GetWidth(),mScrWin->GetSize().GetHeight());

    int offsetx,offsety;
    wxPoint point;
    if(modeSize == EDSM_FIXSIZE)
    {
        point.x = (float)oldPoint.x/(float)mScrWin->mRect.GetWidth()*(float)(mScrWin->GetSize().GetWidth());
        point.y = (float)oldPoint.y/(float)mScrWin->mRect.GetHeight()*(float)(mScrWin->GetSize().GetHeight());

        offsetx = offsety = 0;
    }
    else
    {
        point = oldPoint;
        offsetx = ( mScrWin->GetSize().GetWidth()-std::min(mScrWin->mRect.GetWidth(),mScrWin->GetSize().GetWidth()))/2;
        offsety = ( mScrWin->GetSize().GetHeight()-std::min(mScrWin->mRect.GetHeight(),mScrWin->GetSize().GetHeight()))/2;

		point.x -=viewx;
		point.y -=viewy;
    }

    if (subRect.Inside(point))
    {
        refreshRect.x = offsetx + point.x - pointSize / 2;
        refreshRect.y = offsety + point.y - pointSize / 2;
        refreshRect.width = refreshRect.height = pointSize;        
        return true;
    }
    else
        return false;
}

void SceneInfoMapDialog::onCameraPosChanged()
{
    mScrWin->Refresh();
}

void SceneInfoMapDialog::onTerrainHeightChanged(const std::vector<WX::TerrainInfo>& terrInfo)
{
    static wxLongLong oldTime = wxGetLocalTimeMillis();
    static wxLongLong nowTime = oldTime;
    
    nowTime = wxGetLocalTimeMillis();
    if( (nowTime - oldTime) < 100)
        return ;
    else
    {
        oldTime = nowTime;
    }

    FairyEditorApp* editorApp = (FairyEditorApp*) wxTheApp;
    WX::TerrainData* data = editorApp->GetSceneManipulator()->getTerrainData();

    for(unsigned int i=0; i<terrInfo.size();i++)
    {
        sumHeight = sumHeight - terrInfo[i].oldHeight + terrInfo[i].nowHeight;
    
        maxHeight = std::max(maxHeight,terrInfo[i].nowHeight);
        minHeight = std::min(minHeight,terrInfo[i].nowHeight);
    }

    float diff = maxHeight - minHeight;


    for ( int i=0; i<mHeight; ++i )
    {
        for ( int j=0; j<mWidth; ++j )
        {
            unsigned char r,g,b;
            float height = data->getHeight(j,i);
            if(diff > 0)
            {
                r = g = b =  (height - minHeight)/diff * 255;

            }
            else if ( 0 == diff )
                r = g = b = 255;

            SetImageRGB(j,i,r,g,b,EHTM_HEIGHT);
        }
    }

    mOK = false;
    mScrWin->Refresh();
}
//
//void SceneInfoMapDialog::onTextureChanged(int x,int z,int pixmapId)
//{
//    FairyEditorApp* editorApp = (FairyEditorApp*) wxTheApp;
//    WX::TerrainData* data = editorApp->GetSceneManipulator()->getTerrainData();
//
//    const WX::TerrainData::Pixmap& pixmap = data->mPixmaps[--pixmapId];
//    size_t textureId = pixmap.textureId;
//    assert(textureId < data->mTextures.size());
//    if(textureId >= data->mTextures.size())
//        return;
//
//    const Ogre::String& newTexture = data->mTextures[textureId];
//
//    int r,g,b;
//    r = g = b = 0;
//
//    TextureColourMap::const_iterator iter = textureColour.find(newTexture);
//    if( iter != textureColour.end() )
//    {
//        const wxColour& colour = iter->second;
//        r = colour.Red();
//        g = colour.Green();
//        b = colour.Blue();
//    }
//    else
//    {
//        Ogre::Image image;
//        //see FairyResources.cpp
//        const Ogre::String BRUSH_RESOURCE_GROUP_NAME = "Brushes";
//        image.load(newTexture, BRUSH_RESOURCE_GROUP_NAME);
//
//        typedef unsigned char uchar;
//        uchar* rgb = image.getData();
//        for ( unsigned int i=0; i<image.getHeight(); ++i )
//        {
//            for ( unsigned int j=0; j<image.getWidth(); ++j )
//            {
//                r +=*(rgb+0);
//                g +=*(rgb+1);
//                b +=*(rgb+2);
//
//                int bytes = image.getBPP()/8;
//                rgb += bytes;//rgba
//            }
//        }
//
//        //compute the average r,g,b
//        r = r/(image.getHeight()*image.getWidth());
//        g = g/(image.getHeight()*image.getWidth());
//        b = b/(image.getHeight()*image.getWidth());
//
//        wxColour colour(r,g,b);
//        textureColour.insert(make_pair(newTexture,colour));
//    }
//
//    SetImageRGB(x,z,r,g,b,EHTM_TEXTURE);
//
//    mScrWin->Refresh();
//}


///////////////////////////////////////////////////////////
///controls related
///////////////////////////////////////////////////////////
void SceneInfoMapDialog::InitCheckListbox(void)
{
    mListBox->Clear();

    FactoryColourMap::iterator iter;
    for (iter = factoryColour.begin(); iter != factoryColour.end(); ++iter) 
    {
        int index = mListBox->Append(iter->first);
        AdjustColour(index, initColorTbl[index % WXSIZEOF(initColorTbl)]);
        mListBox->Check(index);
    }

	RecreatePointImage();
	mScrWin->Refresh();
}

void SceneInfoMapDialog::RecreatePointImage()
{
    if(modeSize == EDSM_FIXSIZE)
    {
        mWidthSize = mScrWin->GetSize().GetWidth();
        mHeightSize = mScrWin->GetSize().GetHeight();
    }
    else
    {
        mWidthSize = std::min(mScrWin->mRect.GetWidth(),mScrWin->GetSize().GetWidth());
        mHeightSize = std::min(mScrWin->mRect.GetHeight(),mScrWin->GetSize().GetHeight());
    }

    mPointImage.Create(mWidthSize,mHeightSize,true);
    mPointImage.SetMaskColour(0,0,0);
    mPointImage.SetMask(true);

    mOK2 = false;
    CreateSceneObjectDisplay();    
}

//调整需要显示的类型
void SceneInfoMapDialog::OnCheckboxToggle(wxCommandEvent& event)
{
    unsigned int nItem = event.GetInt();
    wxString strItem = mListBox->GetString(nItem);
    
    if(mListBox->IsChecked(nItem))
    {
        selectedTypes.insert(strItem);
    }
    else
    {
        selectedTypes.erase(strItem);
    }

    RecreatePointImage();
    mScrWin->Refresh();
}

//重新配置某个类型的显示颜色
void SceneInfoMapDialog::OnListboxDblClick(wxCommandEvent& event)
{
    int nSel = event.GetSelection();

    wxColour col(255,0,255);

    wxColourData data;
    data.SetColour(col);
    data.SetChooseFull(TRUE);
    for (int i = 0; i < 16; i++)
    {
        wxColour colour(i*16, i*16, i*16);
        data.SetCustomColour(i, colour);
    }

    wxColourDialog dialog(this, &data);
    wxColourData& colourData = dialog.GetColourData();
    wxString factoryName = mListBox->GetString(nSel);
    colourData.SetColour(factoryColour[factoryName]);
    dialog.SetTitle(wxString::Format(_("Choose the display colour for %s"), factoryName.c_str()));
    if (dialog.ShowModal() == wxID_OK)
    {
        col = colourData.GetColour();
        if (factoryColour[factoryName] != col)
		{
			AdjustColour(nSel,col);
			RecreatePointImage();
			mScrWin->Refresh();
		}
    }
}

//调整场景小地图的显示模式
void SceneInfoMapDialog::OnPatternSize( wxCommandEvent &event )
{
    int index = event.GetInt();
    
    switch(index)
    {
    case 0:
        {
            modeSize = EDSM_FIXSIZE;
            mScrWin->SetScrollRate( 0 ,0 );
        }
        break;
    case 1:
        {
            modeSize = EDSM_SCROLL;  

            int scrollRatex = 1;
            int scrollRatey = 1;

            mScrWin->SetScrollRate( scrollRatex, scrollRatey );
            mScrWin->SetVirtualSize( ( mScrWin->mRect.GetWidth() + 1 )  , ( mScrWin->mRect.GetHeight() + 1 )  );

            mEraseBk = true;
        }
        break;
    }  

    RecreatePointImage();
    mOK = false;
    
    mScrWin->Refresh();
}

void SceneInfoMapDialog::OnPatternHT(wxCommandEvent& event)
{
    int index = event.GetInt();

    switch(index)
    {
    case 0:
        modeHT = EHTM_HEIGHT;
        break;
    case 1:
        modeHT = EHTM_TEXTURE; 
        break;
    }  
    
    mOK = false;
    mOK2 = false;
    mScrWin->Refresh();


}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 供逻辑数据编辑部分使用.
//


// 得到生长点action
WX::CGrowPointEditAction* SceneInfoMapDialog::GetGrowPointAction()
{
	// 得到区域编辑的场景控制
	if(GetSceneManipulator())
	{
		WX::Action* action = GetSceneManipulator()->_getAction("GrowPointEdit");
		return (WX::CGrowPointEditAction *)action;
	}

	return NULL;
}

// 刷新生长点信息
void SceneInfoMapDialog::RefreshGrowPointInfo()
{

	WX::CGrowPointEditAction* pAction = NULL;

	pAction = GetGrowPointAction();

	if(pAction)
	{
		WX::GROWPOINT_MAP::iterator it;
		WX::GROWPOINT_MAP::iterator itEnd;
		
		itEnd = pAction->m_GrowPointMap.end();
		
		WX::CGrowPointInfo tempInfo; 
		for(it =  pAction->m_GrowPointMap.begin(); it != itEnd; it++)
		{
			tempInfo = it->second;
			SetPointRGB(tempInfo.m_position.x, tempInfo.m_position.z, 2, 2, 0);
		}
	}


}


//
// 供逻辑数据编辑部分使用.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////













//////////////////////////////////////////////////////////////////////////////////
/// Class DndCanvasBitmap : sceneinfo Display
//////////////////////////////////////////////////////////////////////////////////
BEGIN_EVENT_TABLE(DnDCanvasBitmap, wxScrolledWindow)
EVT_ERASE_BACKGROUND(DnDCanvasBitmap::OnEraseBackground)
EVT_PAINT(DnDCanvasBitmap::OnPaint)
EVT_SIZE(DnDCanvasBitmap::OnSize)
EVT_MOTION(DnDCanvasBitmap::OnMouseMove)
EVT_LEFT_DOWN(DnDCanvasBitmap::OnLeftDown)
EVT_LEFT_UP(DnDCanvasBitmap::OnLeftUp)
EVT_SCROLLWIN(DnDCanvasBitmap::OnScroll)
END_EVENT_TABLE()

DnDCanvasBitmap::DnDCanvasBitmap(wxWindow *parent,int id)
:wxScrolledWindow(parent,id),
mRect(0,0,0,0),
mBeginDrag(false)
{
        
}

/////////////////////////////////////////////////////////
///draw
//////////////////////////////////////////////////////////
void 
DnDCanvasBitmap::OnSize(wxSizeEvent& event)
{
    wxScrolledWindow::OnSize(event);

    SceneInfoMapDialog* dlg = (SceneInfoMapDialog*) GetParent();
    if(dlg->modeSize == dlg->EDSM_SCROLL &&
        (mRect.GetWidth()<GetSize().GetWidth() || mRect.GetHeight() < GetSize().GetHeight() ) )
    {
        dlg->mEraseBk = true;
    }

    dlg->RecreatePointImage();
    dlg->mOK = false;

    Refresh();

	dlg->RefreshGrowPointInfo();
	
}

void
DnDCanvasBitmap::OnEraseBackground(wxEraseEvent& e)
{
    SceneInfoMapDialog* dlg = (SceneInfoMapDialog*) GetParent();
    if(dlg->mEraseBk)
    {
        wxWindow::OnEraseBackground(e);
        dlg->mEraseBk = false;
    }

    if (mRect.IsEmpty())
    {
        wxWindow::OnEraseBackground(e);
    }
}

void 
DnDCanvasBitmap::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    PrepareDC(dc);

    DrawHTBitmap(dc);
    DrawObjectPoints2(dc);
    DrawNavigator(dc);  

	
}

void
DnDCanvasBitmap::DrawHTBitmap(wxPaintDC& dc)
{
    SceneInfoMapDialog* dlg = (SceneInfoMapDialog*) GetParent();
    dlg->again = true;

    wxSize destSize = this->GetSize();
     
    switch(dlg->modeSize) 
    {
    case SceneInfoMapDialog::EDSM_FIXSIZE:
        {
            if (!dlg->mOK)
            {
                if( dlg->mHTImages[dlg->modeHT].GetWidth() != destSize.GetWidth() ||
                    dlg->mHTImages[dlg->modeHT].GetHeight() != destSize.GetHeight())
                {
                    dlg->mHTBitmaps[dlg->modeHT] = wxBitmap(dlg->mHTImages[dlg->modeHT].Scale(destSize.GetWidth(), destSize.GetHeight()));
                }
                else
                {
                    dlg->mHTBitmaps[dlg->modeHT] = wxBitmap(dlg->mHTImages[dlg->modeHT]);
                }
                dlg->mOK = true;
            }
            dc.DrawBitmap(dlg->mHTBitmaps[dlg->modeHT], 0, 0);
        }
        break;
    case SceneInfoMapDialog::EDSM_SCROLL:
        {
            offsetx = ( GetSize().GetWidth()-std::min(mRect.GetWidth(),GetSize().GetWidth()))/2;
            offsety = ( GetSize().GetHeight()-std::min(mRect.GetHeight(),GetSize().GetHeight()))/2;

            if(!dlg->mOK)
            {
                
                dlg->mHTBitmaps[dlg->modeHT] = wxBitmap(dlg->mHTImages[dlg->modeHT]);
                dlg->mOK = true;
            }

            dc.DrawBitmap(dlg->mHTBitmaps[dlg->modeHT],offsetx,offsety);
        }
        break;
    }
}

void
DnDCanvasBitmap::DrawObjectPoints2(wxPaintDC& dc)
{
    SceneInfoMapDialog* dlg = (SceneInfoMapDialog*) GetParent();

    wxSize destSize = this->GetSize();

    switch(dlg->modeSize) 
    {
    case SceneInfoMapDialog::EDSM_FIXSIZE:
        {
            if (!dlg->mOK2)
            {
                if( dlg->mPointImage.GetWidth() != destSize.GetWidth() ||
                    dlg->mPointImage.GetHeight() != destSize.GetHeight())
                {
                    dlg->mPointBitmap = wxBitmap(dlg->mPointImage.Scale(destSize.GetWidth(), destSize.GetHeight()));                   
                }
                else
                {
                    dlg->mPointBitmap = wxBitmap(dlg->mPointImage);

                }
                dlg->mOK2 = true;
            }
           
            wxMemoryDC memDC;
            memDC.SelectObject(dlg->mPointBitmap);
            if(dc.Blit(0, 0,dlg->mPointBitmap.GetWidth(),dlg->mPointBitmap.GetHeight(),&memDC, 0, 0, wxCOPY, TRUE))
                return;

        }
        break;
    case SceneInfoMapDialog::EDSM_SCROLL:
        {
            offsetx = ( GetSize().GetWidth()-std::min(mRect.GetWidth(),GetSize().GetWidth()))/2;
            offsety = ( GetSize().GetHeight()-std::min(mRect.GetHeight(),GetSize().GetHeight()))/2;

			int viewx,viewy;
			GetViewStart(&viewx,&viewy);

            if(!dlg->mOK2)
            {
                dlg->mPointBitmap = wxBitmap(dlg->mPointImage);
                dlg->mOK2 = true;
            }

            wxMemoryDC memDC;
            memDC.SelectObject(dlg->mPointBitmap);
            if(dc.Blit(viewx+offsetx,viewy+offsety,dlg->mPointBitmap.GetWidth(),dlg->mPointBitmap.GetHeight(),&memDC, 0,0, wxCOPY, TRUE))
                return;

        }
        break;
    }
}

void
DnDCanvasBitmap::DrawObjectPoints(wxPaintDC& dc)
{
    SceneInfoMapDialog* dlg = (SceneInfoMapDialog*) GetParent();

    int viewx,viewy;
    GetViewStart(&viewx,&viewy);

    offsetx = ( GetSize().GetWidth()-std::min(mRect.GetWidth(),GetSize().GetWidth()))/2;
    offsety = ( GetSize().GetHeight()-std::min(mRect.GetHeight(),GetSize().GetHeight()))/2;


    wxRect subRect(viewx,viewy,GetSize().GetWidth(),GetSize().GetHeight());

    //创建各种对象的显示点
    dc.SetPen(*wxTRANSPARENT_PEN);
    for(SceneInfoMapDialog::Obj2Pos::const_iterator iter = dlg->objPos.begin();iter != dlg->objPos.end();iter++)
    {        
        if(!dlg->InInterestType(iter->first->getType()))
            continue;

        wxPoint point = iter->second;
        if(dlg->modeSize == SceneInfoMapDialog::EDSM_FIXSIZE)
        {
            point.x = (float)point.x/(float)mRect.GetWidth()*(float)(GetSize().GetWidth());
            point.y = (float)point.y/(float)mRect.GetHeight()*(float)(GetSize().GetHeight());

            offsetx = offsety = 0;
        }

        if (subRect.Inside(point))
        {
            if(!dc.GetBrush().Ok() || 
                (dc.GetBrush().Ok() && dc.GetBrush().GetColour() != dlg->GetPointColour(iter->first) ) 
                )
                dc.SetBrush(wxBrush(dlg->GetPointColour(iter->first), wxSOLID));

            int newx = offsetx + point.x - pointSize / 2 ;
            int newy = offsety + point.y - pointSize / 2;
                       
            dc.DrawRectangle(newx, newy, pointSize, pointSize);
        }
    }    
}

void 
DnDCanvasBitmap::DrawNavigator(wxPaintDC& dc)
{
    //创建navigator
    const int cornerNum = 8;    
    SceneInfoMapDialog* dlg = (SceneInfoMapDialog*) GetParent();
    const Ogre::Vector3* corners = dlg->GetSceneManipulator()->getCamera()->getWorldSpaceCorners();

    //摄像机的位置
    Ogre::Vector3 newPosition = dlg->GetSceneManipulator()->getCamera()->getPosition();
    WX::TerrainData* data = dlg->GetSceneManipulator()->getTerrainData();

    int subs[] = {5,4,1,0,6,7,2,3};
    wxPoint ptCorners[cornerNum];
    for(int i=0;i<cornerNum;i++)
    {
        int sub = subs[i];
        ptCorners[i] = wxPoint(data->getGrid(corners[sub].x,corners[sub].z).first,data->getGrid(corners[sub].x,corners[sub].z).second);
    }

    wxPoint cameraPos(data->getGrid(newPosition.x,newPosition.z).first,data->getGrid(newPosition.x,newPosition.z).second);

    int width;
    int height;

    int x,y;
    if(dlg->modeSize == dlg->EDSM_SCROLL)
    {
        GetViewStart(&x,&y);

        width =  mRect.GetWidth();
        height = mRect.GetHeight();

        offsetx = ( GetSize().GetWidth()-std::min(mRect.GetWidth(),GetSize().GetWidth()))/2;
        offsety = ( GetSize().GetHeight()-std::min(mRect.GetHeight(),GetSize().GetHeight()))/2;

    }
    else
    {    
        x = y = 0;

        width = GetSize().GetWidth();
        height = GetSize().GetHeight();   

        for(int i=0;i<cornerNum;i++)
        {
            GetTheRealPoint(ptCorners[i],mRect,width,height);
        }

        GetTheRealPoint(cameraPos,mRect,width,height);

        offsetx = offsety = 0;

    }


    for(int i=0;i<cornerNum;i++)
    {
        ptCorners[i].x +=offsetx;
        ptCorners[i].y +=offsety;
    }

    cameraPos.x += offsetx;
    cameraPos.y += offsety;

    mCameraPos = cameraPos ;

    wxRegion region(0 + offsetx,0 + offsety,width,height);
    dc.SetClippingRegion(region);

    DrawPlanes(dc,true,ptCorners);
    DrawPlanes(dc,false,ptCorners);

}

void 
DnDCanvasBitmap::DrawPlanes(wxPaintDC& dc,bool up,wxPoint* pPtCorners)
{
    up ? dc.SetPen(*wxRED_PEN) : dc.SetPen(*wxGREEN_PEN);

    int off = 0;
    up ? off = 0 : off = 4;
    
    wxPoint ptArray[] =
    {
        pPtCorners[0+off],
        pPtCorners[1+off],
        pPtCorners[2+off],
        pPtCorners[3+off],
        pPtCorners[0+off]
    };

    //因为dc重置了0,0
    dc.DrawLines(sizeof(ptArray)/sizeof(wxPoint),ptArray,0,0);        

    int minx,miny,maxx,maxy;
    minx = miny = maxx = maxy = 0;

    minx = GetNum(ptArray,sizeof(ptArray)/sizeof(wxPoint),true,false);
    miny = GetNum(ptArray,sizeof(ptArray)/sizeof(wxPoint),false,false);
    maxx = GetNum(ptArray,sizeof(ptArray)/sizeof(wxPoint),true,true);
    maxy = GetNum(ptArray,sizeof(ptArray)/sizeof(wxPoint),false,true);

    up ? upRectBounding = wxRect(wxPoint(minx,miny),wxPoint(maxx,maxy)) : downRectBounding = wxRect(wxPoint(minx,miny),wxPoint(maxx,maxy));
}


//////////////////////////////////////////////////
///control
//////////////////////////////////////////////////
void 
DnDCanvasBitmap::OnLeftDown(wxMouseEvent& e)
{
    if(InNavigator(e))
    {
        mBeginDrag = true;
        CaptureMouse();

        int x,y;
        GetViewStart(&x,&y);
        mDragPt = wxPoint(e.GetX() + x ,e.GetY() + y);
        offVec = Ogre::Vector2(  Ogre::Vector2(mDragPt.x,mDragPt.y) - Ogre::Vector2(mCameraPos.x,mCameraPos.y));
    }
	else
	{
		SceneInfoMapDialog* dlg = (SceneInfoMapDialog*) GetParent();

		int width = 1;
		int height = 1;

		//控件的高和宽
		if(dlg->modeSize == dlg->EDSM_SCROLL)
		{
			width = std::min(mRect.GetWidth(),GetSize().GetWidth());
			height = std::min(mRect.GetHeight(),GetSize().GetHeight());     
		}
		else
		{
			width = GetSize().GetWidth();
			height = GetSize().GetHeight();
		}



		WX::TerrainData* data = dlg->GetSceneManipulator()->getTerrainData();

		//鼠标可以在控件外拖动


		int x,y;
		if(dlg->modeSize == dlg->EDSM_SCROLL)
		{
			GetViewStart(&x,&y);
		}
		else
		{
			x = y = 0;
		}

		x = e.GetX() + x;
		y = e.GetY() + y;

		//摄像机的位置
		x = x - offVec.x;
		y = y - offVec.y;

		//占地图的百分数 
		float percentx ;
		float percenty ;

		if(dlg->modeSize == dlg->EDSM_SCROLL)
		{
			width = std::max(width,mRect.GetWidth());
			height = std::max(height,mRect.GetHeight());

			offsetx = ( GetSize().GetWidth()-std::min(mRect.GetWidth(),GetSize().GetWidth()))/2;
			offsety = ( GetSize().GetHeight()-std::min(mRect.GetHeight(),GetSize().GetHeight()))/2;

			y = std::max(offsety,y);
			y = std::min(y,offsety + (int)(height + cameraDownY));  


			x = x - offsetx;
			y = y - offsety;
		}
		else
		{
			y = std::max(0,y);
			y = std::min(y,height + cameraDownY);
		}




		percentx = (float)x/(float)(width);
		percenty = (float)y/(float)(height);


		int i =  percentx * mRect.GetWidth();
		int j =  percenty * mRect.GetHeight();

		i = std::max(0,std::min(i,mRect.GetWidth()-1));
		j = std::max(0,std::min(j,mRect.GetHeight()-1));

		Ogre::Vector3 newPosition = data->getGridPosition(i,j) +
			Ogre::Vector3(0, 10 * dlg->GetSceneManipulator()->getBaseScale(), 0);

		if(percenty - 1.0 > 0.0)
		{
			int offY = (percenty-1.0) * mRect.GetWidth();
			dlg->GetSceneManipulator()->getCamera()->setPosition(newPosition + Ogre::Vector3(0,0,offY * dlg->GetSceneManipulator()->getBaseScale()));
		}
		else
			dlg->GetSceneManipulator()->getCamera()->setPosition(newPosition);

		//先设置位置后刷新显示
		Refresh();

	}
}

void 
DnDCanvasBitmap::OnLeftUp(wxMouseEvent& e)
{
    if(HasCapture() && mBeginDrag)
    {
        mBeginDrag = false;
        ReleaseMouse();
    }
}

void
DnDCanvasBitmap::OnMouseMove(wxMouseEvent& e)
{
    if(mBeginDrag)
    {
        SceneInfoMapDialog* dlg = (SceneInfoMapDialog*) GetParent();

        int width = 1;
        int height = 1;

        //控件的高和宽
        if(dlg->modeSize == dlg->EDSM_SCROLL)
        {
            width = std::min(mRect.GetWidth(),GetSize().GetWidth());
            height = std::min(mRect.GetHeight(),GetSize().GetHeight());     
        }
        else
        {
            width = GetSize().GetWidth();
            height = GetSize().GetHeight();
        }

    

        WX::TerrainData* data = dlg->GetSceneManipulator()->getTerrainData();

        //鼠标可以在控件外拖动
        

        int x,y;
        if(dlg->modeSize == dlg->EDSM_SCROLL)
        {
            GetViewStart(&x,&y);
        }
        else
        {
            x = y = 0;
        }

        x = e.GetX() + x;
        y = e.GetY() + y;

        //摄像机的位置
        x = x - offVec.x;
        y = y - offVec.y;

        //占地图的百分数 
        float percentx ;
        float percenty ;

        if(dlg->modeSize == dlg->EDSM_SCROLL)
        {
            width = std::max(width,mRect.GetWidth());
            height = std::max(height,mRect.GetHeight());

            offsetx = ( GetSize().GetWidth()-std::min(mRect.GetWidth(),GetSize().GetWidth()))/2;
            offsety = ( GetSize().GetHeight()-std::min(mRect.GetHeight(),GetSize().GetHeight()))/2;

            y = std::max(offsety,y);
            y = std::min(y,offsety + (int)(height + cameraDownY));  
            

            x = x - offsetx;
            y = y - offsety;
        }
        else
        {
            y = std::max(0,y);
            y = std::min(y,height + cameraDownY);
        }

    
    

        percentx = (float)x/(float)(width);
        percenty = (float)y/(float)(height);
        
        
        int i =  percentx * mRect.GetWidth();
        int j =  percenty * mRect.GetHeight();

        i = std::max(0,std::min(i,mRect.GetWidth()-1));
        j = std::max(0,std::min(j,mRect.GetHeight()-1));

        Ogre::Vector3 newPosition = data->getGridPosition(i,j) +
            Ogre::Vector3(0, 10 * dlg->GetSceneManipulator()->getBaseScale(), 0);

        if(percenty - 1.0 > 0.0)
        {
            int offY = (percenty-1.0) * mRect.GetWidth();
            dlg->GetSceneManipulator()->getCamera()->setPosition(newPosition + Ogre::Vector3(0,0,offY * dlg->GetSceneManipulator()->getBaseScale()));
        }
        else
            dlg->GetSceneManipulator()->getCamera()->setPosition(newPosition);
    
        //先设置位置后刷新显示
        Refresh();
        
    }
}

void
DnDCanvasBitmap::OnScroll(wxScrollWinEvent& event)
{
	SceneInfoMapDialog* dlg = (SceneInfoMapDialog*) GetParent();
	dlg->RecreatePointImage();
	Refresh();
	event.Skip();
}
/////////////////////////////////////////////////////////
///helper
/////////////////////////////////////////////////////////
//鼠标是否在导航图内
bool 
DnDCanvasBitmap::InNavigator(wxMouseEvent& e)
{
    int x,y;
    SceneInfoMapDialog* dlg = (SceneInfoMapDialog*) GetParent();

    if(dlg->modeSize == dlg->EDSM_SCROLL)
    {
        GetViewStart(&x,&y);
    }
    else
    {
        x = y = 0;
    }

    x = e.GetX() + x;
    y = e.GetY() + y;

    if(upRectBounding.Inside(x,y) || downRectBounding.Inside(x,y))
        return true;

    return false;
}

//first表示取第几个数据
inline int 
DnDCanvasBitmap::GetNum(wxPoint* pt,int num,bool first,bool max)
{
    int ret ;
    (first)? ret = pt[0].x :ret = pt[0].y;

    for(int i=0;i<num;i++)
    {     
        int* p = (int*)(pt++);

        ret = (max)?(std::max(((first)?*p:*++p),ret)) : std::min(((first)?*p:*++p),ret);
    }
    return ret;
}

void 
DnDCanvasBitmap::GetTheRealPoint(wxPoint& pt,wxRect& mRect,int& width,int& height)
{
    pt.x = (float)pt.x/(float)(mRect.GetWidth())*(float)width;  
    pt.y = (float)pt.y/(float)(mRect.GetHeight())*(float)height;
}

void DnDCanvasBitmap::SetRect(const wxRect& rect)
{
    mRect = rect;

    Refresh();
}


