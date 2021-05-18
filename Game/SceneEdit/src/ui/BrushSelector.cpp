// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "BrushSelector.h"
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

#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>

#include "BrushSelector.h"


// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------

#include <OgreCodec.h>
#include <OgreResourceGroupManager.h>
#include <OgreStringConverter.h>
#include <OgreImage.h>

// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
#include "Core/Terrain.h"
#include "Core/TerrainData.h"

#include "SceneManipulator.h"
#include "Action.h"
#include "WXOperatorManager.h"
#include "TerrainPaintInfoContainer.h"

#include "res/BrushSelector_wdr.h"
// ----------------------------------------------------------------------------
// BrushSelector class
// ----------------------------------------------------------------------------

#if wxUSE_STL
#define AS_STRING(s)    (s)
#else
#define AS_STRING(s)    ((s).c_str())
#endif

//static const int feID_PIXMAP_LIST = wxNewId();

BEGIN_EVENT_TABLE(BrushSelector, wxPanel)
    EVT_UPDATE_UI(feID_PIXMAP_LIST, BrushSelector::OnUpdateControls)

	EVT_CHECKBOX(feID_FLIP_HORIZONTAL, BrushSelector::OnCheck) 
	EVT_CHECKBOX(feID_FLIP_VERTICAL, BrushSelector::OnCheck) 
	EVT_CHECKBOX(feID_ROTATE_RIGHT_ANGLE, BrushSelector::OnCheck) 
	EVT_CHECKBOX(feID_RANDOM, BrushSelector::OnCheck) 
    EVT_CHECKBOX(feID_MIRROR_DIAGONAL, BrushSelector::OnCheck)
    EVT_CHECKBOX(feID_ENABLE_PIXMAP_PREVIEW, BrushSelector::OnCheck) 

	EVT_BUTTON(feID_RELOAD_PIXMAP_LIST, BrushSelector::OnReloadTextureList)
	EVT_TREE_SEL_CHANGED(feID_PIXMAP_LIST, BrushSelector::OnPixmapListSelected)
    EVT_TREE_ITEM_RIGHT_CLICK(feID_PIXMAP_LIST, BrushSelector::OnRightButtonUp)

    EVT_MENU(feID_PIXMAP_FULLTERRAINPAINT, BrushSelector::OnFullTerrainPaint)
    EVT_MENU(feID_PIXMAP_PAINTINONEGRID, BrushSelector::OnPaintInOneGrid)

END_EVENT_TABLE()

void
BrushSelector::Init(const wxString& resourceGroup)
{
    mSceneManipulator = NULL;
    mSplitter = NULL;
    mBrushesTree = NULL;
	mBrushesPreview = NULL;

	mFlipHorizontal = NULL;
	mFlipVertical = NULL;
	mRotateRightAngle = NULL;
	mRandom = NULL;
    mMirrorDiagonal = NULL;

	mEnablePreview = NULL;

	mCurrentOrientationType = 0;
	mCurrentFileName = "";

    // 定义预览图的大小
    mPreviewImageWidth = 64;
    mPreviewImageHeight = 64;
	// 初始化image，数据为黑色
	mCurrentPreviewImage.Create( mPreviewImageWidth, mPreviewImageHeight, true );

    // 获得黑白图
    mBlackWhitePreviewImage = CellGridBitmaps(1).ConvertToImage();

    mResourceGroup = resourceGroup;

    mPaintInOneGrid = false;
    mIsRandomOrientation = false;
}

BrushSelector::~BrushSelector()
{
    if (mBrushesTree)
    {
        mBrushesTree->Freeze();
        mBrushesTree->DeleteAllItems();
        mBrushesTree->Thaw();
    }
}

void
BrushSelector::SetSceneManipulator(WX::SceneManipulator* sceneManipulator)
{
    mSceneManipulator = sceneManipulator;
}

bool
BrushSelector::Create(wxWindow *parent, wxWindowID winid,
                      const wxPoint& pos, const wxSize& size,
                      long style, const wxString& name)
{
    if (!wxPanel::Create(parent, winid, pos, size, style, name))
        return false;

	InitPixmapSelector(this, true, true);

	/// 获取各控件的引用
	mBrushesTree = wxDynamicCast(this->FindWindow(feID_PIXMAP_LIST), wxTreeCtrl);
	mBrushesPreview = wxDynamicCast(this->FindWindow(feID_PIXMAP_PREVIEW), wxStaticBitmap);
	mFlipHorizontal = wxDynamicCast(this->FindWindow(feID_FLIP_HORIZONTAL), wxCheckBox);
	mFlipVertical = wxDynamicCast(this->FindWindow(feID_FLIP_VERTICAL), wxCheckBox);
	mRotateRightAngle = wxDynamicCast(this->FindWindow(feID_ROTATE_RIGHT_ANGLE), wxCheckBox);
	mRandom = wxDynamicCast(this->FindWindow(feID_RANDOM), wxCheckBox);
    mMirrorDiagonal = wxDynamicCast(this->FindWindow(feID_MIRROR_DIAGONAL), wxCheckBox);

	mEnablePreview = wxDynamicCast(this->FindWindow(feID_ENABLE_PIXMAP_PREVIEW), wxCheckBox);

    ReloadTextureList();

    // 初始化时不能选择画刷的方向选项
    mFlipHorizontal->Enable(false);
    mFlipVertical->Enable(false);
    mRotateRightAngle->Enable(false);
    mMirrorDiagonal->Enable(false);
    mRandom->Enable(false);
    mEnablePreview->Enable(false);

    // 缩略图用黑白图
    mBrushesPreview->SetBitmap(mBlackWhitePreviewImage);

    return true;
}

void
BrushSelector::ReloadTextureList(const wxString& resourceGroup)
{
    mResourceGroup = resourceGroup;
    ReloadTextureList();
}

void
BrushSelector::ReloadTextureList(void)
{
	CatalogMap catalogMap;

    if (!Ogre::ResourceGroupManager::getSingletonPtr())
        return;

    mBrushesTree->Freeze();

    mBrushesTree->DeleteAllItems();

	wxTreeItemId rootId = mBrushesTree->AddRoot(/*_("Brushes")*/wxT("画刷列表"));
 
    // 重新解析定义文件
    GetSceneManipulator()->reloadPaintInfo();

    const WX::TerrainPaintInfoContainer *paintInfoContainer = GetSceneManipulator()->getTerrainPaintInfoContainer();

    assert (paintInfoContainer);

    const WX::TextureInfoMap &textureInfoMap = paintInfoContainer->getTextureInfoMap();

    WX::TextureInfoMap::const_iterator it = textureInfoMap.begin();

    // 遍历画刷数组
    while ( it != textureInfoMap.end() )
    {
        const WX::TextureInfos &textureInfos = it->second;
        
        // 遍历该画刷下的所有纹理
        for ( size_t i=0; i<textureInfos.size(); ++i )
        {
            // 取出所属的大纹理的名称
            Ogre::String ownerTexName = textureInfos[i].ownerTextureName;

            // 如果该纹理名称中包含了/，说明它是在一个文件夹中的
            size_t pos = ownerTexName.find_last_of('/');

            // 在文件夹中
            if (pos != Ogre::String::npos)
            {
                // 先去除纹理文件名，剩下路径名
                ownerTexName.erase(pos+1);
                // 加上画刷的名称
                ownerTexName.append(textureInfos[i].brushName);
                mBrushesTree->AppendItem( GetParentId(ownerTexName, catalogMap), wxT(textureInfos[i].textureName) );
            }
            else
            {
                Ogre::String brushName = textureInfos[i].brushName;
                // 如果是在根目录下，就直接用画刷名称来作为路径名
                mBrushesTree->AppendItem( GetParentId(brushName, catalogMap), wxT(textureInfos[i].textureName) );
            }
        }
        ++it;
    }

    mBrushesTree->Thaw();
}

wxTreeItemId  
BrushSelector::GetParentId( Ogre::String &catalog, CatalogMap &map )
{
	// 如果送入的相对路径为空，说明这个纹理放在资源目录的根目录下
	if ( catalog == "" )
		// 所以，它的节点应该被建在根目录下
		return mBrushesTree->GetRootItem();
	else
	{
		// 否则，先试图找它的上一级目录，如果有的话（说明这个目录在树中已经建好，可能是因为有同个目录下的纹理
		// 被处理过了），就返回这个目录
		CatalogMap::iterator i = map.find(catalog);

		if ( i != map.end() )
			return i->second;
		// 如果没有，就得递归往上找，并建树
		else
		{
			Ogre::String path, baseName;
			Ogre::StringUtil::splitFilename(catalog, baseName, path);
			if (!path.empty() && path[path.length()-1] == '/')
				path.erase(path.length() - 1);

			wxTreeItemId id = mBrushesTree->AppendItem( GetParentId(path, map), wxT(baseName) );

			map.insert( CatalogMap::value_type( catalog, id ) );

			return id;
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void
BrushSelector::OnUpdateControls(wxUpdateUIEvent& e)
{
    e.Enable(GetSceneManipulator() != 0);
}

const WX::Action * 
BrushSelector::OnStartAutoTexAction( const wxTreeItemId &itemId, const wxString &brushName )
{
    // 启动自动纹理拼接action
    WX::Action* action = GetSceneManipulator()->_getAction("AutoTexPaintAction");

    // 获取组的名称
    wxString groupName = mBrushesTree->GetItemText(itemId);

    // 设置当前组的名称
    action->setParameter( "%GroupName", AS_STRING(brushName) );
 
    // 把之前的路径加上去
    wxTreeItemId parentId = mBrushesTree->GetItemParent(itemId);
    wxTreeItemId rootId = mBrushesTree->GetRootItem();

    while ( parentId != rootId )
    {
        wxString parentText = mBrushesTree->GetItemText(parentId);
        parentText += '/';
        groupName.Prepend( parentText );

        parentId = mBrushesTree->GetItemParent(parentId);   
    }

    groupName += "/";

    // 获取该目录下的第一个节点
    wxTreeItemIdValue dummy;
    wxTreeItemId childId = mBrushesTree->GetFirstChild(itemId,dummy);

    while ( childId.IsOk() == true )
    {
        // 遍历每个节点，设置纹理名称
        wxString texName = mBrushesTree->GetItemText(childId);

        childId = mBrushesTree->GetNextChild(itemId,dummy);

        if ( texName.find('|') == wxString::npos )
            continue;

        action->setParameter( "%TextureName", AS_STRING(texName) );        
    }

    // 输入InputFinish表示纹理名称输入完毕
    action->setParameter( "%TextureName", "InputFinish" );
    
    // 判断纹理是否正确地进行了初始化
    if ( action->getParameter("%IsInit") == "Yes" )
    {
        GetSceneManipulator()->setActiveAction(action);
        return action;
    }
    else
    {
        // 启用simplepaintAction
        action = GetSceneManipulator()->_getAction("SimplePaintAction");

        action->setParameter( "%TextureName", "InputBegin" );

        // 获取该目录下的第一个节点
        wxTreeItemIdValue dummy;
        wxTreeItemId childId = mBrushesTree->GetFirstChild(itemId,dummy);

        while ( childId.IsOk() == true )
        {
            // 遍历每个节点，设置纹理名称
            wxString texName = mBrushesTree->GetItemText(childId);

            childId = mBrushesTree->GetNextChild(itemId,dummy);

            if ( texName.find('|') == wxString::npos )
                continue;

            const WX::TextureInfos &currentPaintInfos = 
                GetSceneManipulator()->getTextureInfos(mCurrentFileName.c_str());

            const WX::TextureInfo &currentPaintInfo =
                GetSceneManipulator()->getTextureInfo(mCurrentFileName.c_str(), texName.c_str());

            Ogre::Image *previewImage = GetSceneManipulator()->getPreviewImage(currentPaintInfo.ownerTextureName);

            // 组成纹理信息字符串
            Ogre::String texInfoString;
            texInfoString += currentPaintInfo.ownerTextureName;
            texInfoString += "|";
            texInfoString += Ogre::StringConverter::toString(currentPaintInfo.height);
            texInfoString += "|";
            texInfoString += Ogre::StringConverter::toString(currentPaintInfo.width);
            texInfoString += "|";
            texInfoString += Ogre::StringConverter::toString(currentPaintInfo.leftCorner);
            texInfoString += "|";
            texInfoString += Ogre::StringConverter::toString(currentPaintInfo.topCorner);
            texInfoString += "|";
            texInfoString += Ogre::StringConverter::toString(previewImage->getHeight());
            texInfoString += "|";
            texInfoString += Ogre::StringConverter::toString(previewImage->getWidth());
			texInfoString += "|";
			texInfoString += Ogre::StringConverter::toString(currentPaintInfo.rotateType);

            action->setParameter( "%TextureName", AS_STRING(texInfoString) );  

            // 这里有个错误？？，因为没有设置该纹理是否为普通纹理大小，所以如果这一个组中有一个大纹理，那么
            // 它可能会只缩放到一格
        }

        if ( action->getParameter("%IsInit") == "Yes" )
        {
            GetSceneManipulator()->setActiveAction(action);
            return action;
        }
        else
        {
            // 为能成功初始化，先把active action设为NULL
            GetSceneManipulator()->setActiveAction(NULL);
            return NULL;
        }
    }
}

void
BrushSelector::OnPixmapListSelected(wxTreeEvent& event)
{
	try
	{
    // 以下是改成定义文件后的修改
    //  mPaintInOneGrid = false;
    // 保存当前选中的纹理的大小
    int selectedTexXSize = 0;
    int selectedTexZSize = 0;

    // 当画刷被选中时，要进行判断
    wxTreeItemId itemId = event.GetItem();

    wxTreeItemId rootId = mBrushesTree->GetRootItem();

    // 如果选中根，就返回
    if ( itemId == rootId )
    {
        // 选中根目录名时，所有选项都变灰
        mFlipHorizontal->Enable(false);
        mFlipVertical->Enable(false);
        mRotateRightAngle->Enable(false);
        mMirrorDiagonal->Enable(false);
        mRandom->Enable(false);
        mEnablePreview->Enable(false);

        // 缩略图用黑白图
        mBrushesPreview->SetBitmap(mBlackWhitePreviewImage);

        return;
    }

    if (itemId.IsOk())
    {
        mCurrentOrientationType = 0;

        // 如果选项的父是root，说明该画刷是放在根目录下的，也有可能是根目录下的子目录
        if ( mBrushesTree->GetItemParent(itemId) == rootId )
        {
            mCurrentFileName = mBrushesTree->GetItemText(itemId);
        }
        else
        {
            // 以下处理不是放在根目录下的画刷

            wxTreeItemId parentId = mBrushesTree->GetItemParent(itemId);
            mCurrentFileName = mBrushesTree->GetItemText(itemId);

            // 以此取出父目录的目录名，组合成相对路径名，并加上文件名
            while ( parentId != rootId )
            {
                wxString parentText = mBrushesTree->GetItemText(parentId);
                parentText += '/';
                mCurrentFileName.Prepend( parentText );

                parentId = mBrushesTree->GetItemParent(parentId);
            }		
        }

        // 如果名字中有.，说明是文件名，不是目录名
        if ( mCurrentFileName.find_first_of('|') != wxString::npos )
        {
            mFlipHorizontal->Enable(true);
            mFlipVertical->Enable(true);
            mRotateRightAngle->Enable(true);
            mMirrorDiagonal->Enable(true);
            mRandom->Enable(true);
            mEnablePreview->Enable(true);

            Ogre::String currentFileName(mCurrentFileName.c_str());

            size_t pos = currentFileName.find_last_of('/');

            // 取出brush名称
            Ogre::String brushName = currentFileName.substr(0,pos);
            Ogre::String textureName = currentFileName.substr(pos+1);

            const WX::TextureInfos &currentPaintInfos = 
                GetSceneManipulator()->getTextureInfos(brushName);

            const WX::TextureInfo &currentPaintInfo = GetSceneManipulator()->getTextureInfo(brushName, textureName);

            Ogre::Image *previewImage = GetSceneManipulator()->getPreviewImage(currentPaintInfo.ownerTextureName);

            // 组成纹理信息字符串
            Ogre::String texInfoString;
            texInfoString += currentPaintInfo.ownerTextureName;
            texInfoString += "|";
            texInfoString += Ogre::StringConverter::toString(currentPaintInfo.height);
            texInfoString += "|";
            texInfoString += Ogre::StringConverter::toString(currentPaintInfo.width);
            texInfoString += "|";
            texInfoString += Ogre::StringConverter::toString(currentPaintInfo.leftCorner);
            texInfoString += "|";
            texInfoString += Ogre::StringConverter::toString(currentPaintInfo.topCorner);
            texInfoString += "|";
            texInfoString += Ogre::StringConverter::toString(previewImage->getHeight());
            texInfoString += "|";
            texInfoString += Ogre::StringConverter::toString(previewImage->getWidth());
			texInfoString += "|";
			texInfoString += Ogre::StringConverter::toString(currentPaintInfo.rotateType);

            WX::Action* action = GetSceneManipulator()->_getAction("SimplePaintAction");
            action->setParameter( "%TextureName", "InputBegin" );
            action->setParameter( "%TextureName", AS_STRING(texInfoString) );
            action->setParameter( "%OrientationType", Ogre::StringConverter::toString(mCurrentOrientationType) );

            if ( (currentPaintInfo.width > 1 || currentPaintInfo.height > 1) && !mPaintInOneGrid )
            {               
                action->setParameter( "%TexNormalSize", "false" );
                mFlipHorizontal->Enable(false);
                mFlipVertical->Enable(false);
                mRotateRightAngle->Enable(false);
                mMirrorDiagonal->Enable(false);
                mRandom->Enable(false);
            }
            else
            {
                action->setParameter("%TexNormalSize","true");
            }

            GetSceneManipulator()->setActiveAction(action);

            buildPreviewBitmap(currentPaintInfo);
         
            if ( mEnablePreview->IsChecked() == false )
            {
                // 如果不显示缩略图，就显示默认的黑白图
                mBrushesPreview->SetBitmap(mBlackWhitePreviewImage);
            }
        }
        // 如果名字中有.，说明不是文件名，是目录名
        else
        {
            mFlipHorizontal->Enable(false);
            mFlipVertical->Enable(false);
            mRotateRightAngle->Enable(false);
            mMirrorDiagonal->Enable(false);
            mRandom->Enable(false);
            mEnablePreview->Enable(true);

            // 把整个目录的纹理进行判断，并加入到透明纹理列表

            const WX::Action* action = OnStartAutoTexAction(itemId, mCurrentFileName);

            if ( action != 0 )
            {
                // 从该组纹理的实心纹理中随机用一个来做为缩略图
                // 读取这张纹理，并缩放到mPreviewImageWidth*mPreviewImageHeight，用于缩略图
              //  Ogre::String tempTexName = action->getParameter( "%TextureName" );

                const WX::TextureInfos &currentPaintInfos = 
                    GetSceneManipulator()->getTextureInfos(mCurrentFileName.c_str());

              //  const WX::TextureInfo &currentPaintInfo = currentPaintInfos[0];

                // 构建preview image
                buildPreviewBitmap(currentPaintInfos[0]);                
            }
            else
            {
                mEnablePreview->Enable(false);
                mBrushesPreview->SetBitmap(mBlackWhitePreviewImage);
            }

            if ( mEnablePreview->IsChecked() == false )
            {
                // 如果不显示缩略图，就显示默认的黑白图
                mBrushesPreview->SetBitmap(mBlackWhitePreviewImage);
            }
        }
    }
   
    // 让所有的checkbox回复未选择的状态
    mFlipHorizontal->SetValue(false);
    mFlipVertical->SetValue(false);
    mRotateRightAngle->SetValue(false);
    mMirrorDiagonal->SetValue(false);
    mRandom->SetValue(false);
	}
	catch (Ogre::Exception &e)
	{
		wxMessageBox(
			e.getDescription().c_str(),
           /* _("Texture Wrong")*/wxT("纹理定义出错或找不到纹理图片"),
            wxOK|wxCENTRE|wxICON_ERROR, this);
	}
}

void
BrushSelector::OnCheck( wxCommandEvent &event )
{
	if ( event.GetId() == feID_RANDOM )
	{
		// 禁止其他按钮
		if ( mRandom->IsChecked() )
		{
            mIsRandomOrientation = true;

			mFlipHorizontal->Enable(false);
			mFlipVertical->Enable(false);
			mRotateRightAngle->Enable(false);
            mMirrorDiagonal->Enable(false);
		}
		else
		{
            mIsRandomOrientation = false;

			mFlipHorizontal->Enable(true);
			mFlipVertical->Enable(true);
			mRotateRightAngle->Enable(true);
            mMirrorDiagonal->Enable(true);
		}
	}

	else
	{
		mCurrentOrientationType = 0;

		// 以下是checkbox状态的判断
		if ( mFlipHorizontal->IsChecked() == true )
		{
			mCurrentOrientationType |= 1;
		}

		if ( mFlipVertical->IsChecked() == true )
		{
			mCurrentOrientationType |= 2;
		}

		if ( mRotateRightAngle->IsChecked() == true )
		{
			mCurrentOrientationType |= 4;
		}

        if ( mMirrorDiagonal->IsChecked() == true )
        {
            mCurrentOrientationType |= 8;
        }
	}
	
	WX::Action* action = GetSceneManipulator()->_getAction("SimplePaintAction");

    assert (action);

    action->setParameter( "%UseRandomOrientation", mIsRandomOrientation ? "true" : "false" );
	action->setParameter( "%OrientationType", Ogre::StringConverter::toString(mCurrentOrientationType) );

	GetSceneManipulator()->setActiveAction(action);

 //   wxImage tempPreview;
	if ( mEnablePreview->IsChecked() )
	{
		// 选中按钮时，缩略图也要相应变化
		if ( event.GetId() == feID_FLIP_HORIZONTAL )
		{
			mCurrentPreviewImage = mCurrentPreviewImage.Mirror();
			mBrushesPreview->SetBitmap(mCurrentPreviewImage);
		}

		if ( event.GetId() == feID_FLIP_VERTICAL )
		{
			mCurrentPreviewImage = mCurrentPreviewImage.Mirror(false);
			mBrushesPreview->SetBitmap(mCurrentPreviewImage);
		}

		if ( event.GetId() == feID_ROTATE_RIGHT_ANGLE )
		{
			if ( mRotateRightAngle->IsChecked() == true )
			{
				mCurrentPreviewImage = mCurrentPreviewImage.Rotate90(false);
				mBrushesPreview->SetBitmap(mCurrentPreviewImage);
			}
			else
			{
				mCurrentPreviewImage = mCurrentPreviewImage.Rotate90();
				mBrushesPreview->SetBitmap(mCurrentPreviewImage);
			}
		}
	}
	else
	{
		// 如果不显示缩略图，就显示默认的黑白图
        mBrushesPreview->SetBitmap(mBlackWhitePreviewImage);
	}

    // 预览图的开关
	if ( event.GetId() == feID_ENABLE_PIXMAP_PREVIEW )
	{
		if ( mEnablePreview->IsChecked() )
		{
            // 如果用户打开开关，就用前面OnPixmapListSelected中计算好的预览图来显示
            // 这也是为什么前面OnPixmapListSelected中无论这个开关是否打开都要计算预览图的原因
            // 不然的话，这里打开开关后显示的是上次计算的预览图，而不是当前所选纹理的预览图
            mBrushesPreview->SetBitmap(mCurrentPreviewImage);
		}
		else
		{
            mBrushesPreview->SetBitmap(mBlackWhitePreviewImage);
		}
	}
}

void 
BrushSelector::OnReloadTextureList( wxCommandEvent &event )
{
	ReloadTextureList();
}

void 
BrushSelector::OnRightButtonUp(wxTreeEvent& event)
{
    wxTreeItemId selectedId = event.GetItem();
    
    ShowBrushRightButtonMenu(selectedId);
}

void
BrushSelector::ShowBrushRightButtonMenu(const wxTreeItemId &itemId)
{
    // 清空全地形平铺的画刷数组
    mPixmapArray.clear();

    wxMenu menu;

    // 增加菜单项
    menu.Append(feID_PIXMAP_FULLTERRAINPAINT, _T("&Full Terrain Paint"));
    // 初始时为不可用
    menu.Enable(feID_PIXMAP_FULLTERRAINPAINT,false);

    // 以下是检测该目录下或该文件是否符合要求（不可为tga文件）
    wxTreeItemId rootId = mBrushesTree->GetRootItem();

    if ( itemId == rootId )
        return;

    if (itemId.IsOk())
    {
        // 如果选项的父是root，说明该画刷是放在根目录下的，也有可能是根目录下的子目录
        if ( mBrushesTree->GetItemParent(itemId) == rootId )
        {
            mCurrentFileName = mBrushesTree->GetItemText(itemId);
        }
        else
        {
            // 以下处理不是放在根目录下的画刷
            wxTreeItemId parentId = mBrushesTree->GetItemParent(itemId);
            mCurrentFileName = mBrushesTree->GetItemText(itemId);

            // 以此取出父目录的目录名，组合成相对路径名，并加上文件名
            while ( parentId != rootId )
            {
                wxString parentText = mBrushesTree->GetItemText(parentId);
                parentText += '/';
                mCurrentFileName.Prepend( parentText );

                parentId = mBrushesTree->GetItemParent(parentId);
            }		
        }

        // 如果名字中有.，说明是文件名，不是目录名
        if ( mCurrentFileName.find_first_of('|') != wxString::npos )
        {
            Ogre::String currentFileName(mCurrentFileName.c_str());

            size_t pos = currentFileName.find_last_of('/');

            // 取出brush名称
            Ogre::String brushName = currentFileName.substr(0,pos);
            Ogre::String textureName = currentFileName.substr(pos+1);

            const WX::TextureInfos &currentPaintInfos = 
                GetSceneManipulator()->getTextureInfos(brushName);

            const WX::TextureInfo &currentPaintInfo = GetSceneManipulator()->getTextureInfo(brushName, textureName);

            // 如果该纹理不是透明纹理，可以用于平铺整个地形
            if ( !mSceneManipulator->IsTransparentTexture(currentPaintInfo.ownerTextureName) )
            {
                WX::PaintPixmap pixmap;
                
                mSceneManipulator->setPaintPixmapInfo(pixmap, currentPaintInfo);
                mPixmapArray.push_back(pixmap);      
            }
        }
        // 如果名字中有.，说明不是文件名，是目录名
        else
        {
            // 获取组的名称
            wxString groupName = mBrushesTree->GetItemText(itemId);

            // 把之前的路径加上去
            wxTreeItemId parentId = mBrushesTree->GetItemParent(itemId);
            wxTreeItemId rootId = mBrushesTree->GetRootItem();

            while ( parentId != rootId )
            {
                wxString parentText = mBrushesTree->GetItemText(parentId);
                parentText += '/';
                groupName.Prepend( parentText );

                parentId = mBrushesTree->GetItemParent(parentId);   
            }

            groupName += "/";

            // 获取该目录下的第一个节点
            wxTreeItemIdValue dummy;
            wxTreeItemId childId = mBrushesTree->GetFirstChild(itemId,dummy);

            while ( childId.IsOk() == true )
            {
                // 遍历每个节点，设置纹理名称
                wxString texName = mBrushesTree->GetItemText(childId);

                childId = mBrushesTree->GetNextChild(itemId,dummy);

                if ( texName.find('|') == wxString::npos )
                    continue;

                texName.Prepend(groupName);

                Ogre::String currentFileName(texName.c_str());

                size_t pos = currentFileName.find_last_of('/');

                // 取出brush名称
                Ogre::String brushName = currentFileName.substr(0,pos);
                Ogre::String textureName = currentFileName.substr(pos+1);

                const WX::TextureInfos &currentPaintInfos = 
                    GetSceneManipulator()->getTextureInfos(brushName);

                const WX::TextureInfo &currentPaintInfo = GetSceneManipulator()->getTextureInfo(brushName, textureName);

                // 如果该纹理不是透明纹理，可以用于平铺整个地形
                if ( !mSceneManipulator->IsTransparentTexture(currentPaintInfo.ownerTextureName) )
                {
                    WX::PaintPixmap pixmap;

                    mSceneManipulator->setPaintPixmapInfo(pixmap, currentPaintInfo);
                    mPixmapArray.push_back(pixmap);      
                }
            }
        }

        if ( mPixmapArray.size() > 0 )
        {
            // 让菜单显示
            menu.Enable(feID_PIXMAP_FULLTERRAINPAINT,true);
        }
        else
            menu.Enable(feID_PIXMAP_FULLTERRAINPAINT,false);
    }

    // 增加一项，选中后可以让一张纹理总是对应一个格（可能是hack的代码，可能该成配置文件，或可以自定义一张纹理对应多少格）
    // 增加菜单项
    menu.AppendCheckItem(feID_PIXMAP_PAINTINONEGRID, _T("&Paint this texture in one grid"));
    // 初始时为不可用
   // menu.Enable(feID_PIXMAP_PAINTINONEGRID,false);
    menu.Check(feID_PIXMAP_PAINTINONEGRID,mPaintInOneGrid);

    PopupMenu(&menu);
}

void
BrushSelector::OnFullTerrainPaint(wxCommandEvent& event)
{
    // 出现对话框让用户选择是否继续此操作
    wxString prompt = /*_("The operations before can not be undo,Do you want to continue the operation?")*/wxT("这项操作不可以恢复，您确信要继续进行操作吗？");

    int res = wxMessageBox(prompt, /*_("Warning")*/wxT("警告"),
        wxYES_NO|wxICON_QUESTION,
        this);

    switch (res)
    {
    default:   
    case wxNO:
        return;

    case wxYES:
        {
            int texCount = mPixmapArray.size();
            int pixmapId = 0;

            const WX::TerrainData::GridInfoArray &gridInfos = mSceneManipulator->getTerrainData()->mGridInfos;

            if (!gridInfos.empty())
            {
                // 如果只选择一张纹理，或一个文件夹下只有一张纹理
                if ( texCount == 1 )
                {
                    WX::PaintPixmap &pixmap = mPixmapArray[0];
                    pixmapId = mSceneManipulator->getTerrainData()->_registerPixmap(
                        pixmap.textureName, pixmap.left, pixmap.top, pixmap.right, pixmap.bottom );
                }

                size_t gridCount = gridInfos.size();

                for ( size_t i = 0; i < gridCount; ++i )
                {
                    WX::TerrainData::GridInfo info = gridInfos[i];

                    if ( texCount > 1 )
                    {
                    //    Ogre::String texName;
						int index = static_cast<int>(Ogre::Math::RangeRandom(0,texCount));
						if (index == texCount)
						{
							index = 0;
						}
						assert (index < texCount);
                        const WX::PaintPixmap &pixmap = mPixmapArray[ index ];

                        // 用这个id来索引pixmap
                        pixmapId = mSceneManipulator->getTerrainData()->_registerPixmap(
                            pixmap.textureName, pixmap.left, pixmap.top, pixmap.right, pixmap.bottom );        
                    }

                    info.layers[0].pixmapId = pixmapId;
                    info.layers[0].orientation = 0;

                    info.layers[1].pixmapId = 0;
                    info.layers[1].orientation = 0;         

                    mSceneManipulator->getTerrainData()->setGridInfo(i,info);

                } 

                mSceneManipulator->getTerrain()->notifyGridInfoModified(0,0,mSceneManipulator->getTerrainData()->getXSize(),mSceneManipulator->getTerrainData()->getZSize());
            }

            // 清除redo undo列表
            mSceneManipulator->getOperatorManager()->reset();
        }
    }
}

void 
BrushSelector::OnPaintInOneGrid(wxCommandEvent& event)
{
    mPaintInOneGrid = event.IsChecked();
    wxTreeEvent treeEvent;
    treeEvent.SetItem(mBrushesTree->GetSelection());
    OnPixmapListSelected(treeEvent);
}

void 
BrushSelector::buildPreviewBitmap( const WX::TextureInfo texInfo )
{
    const Ogre::uchar BytePerPixel = 8;

    // 读取原始image
    Ogre::Image *oriImage = GetSceneManipulator()->getPreviewImage(texInfo.ownerTextureName);
    // 源大纹理的大小
    size_t oriImageHeight = oriImage->getHeight();
    size_t oriImageWidth = oriImage->getWidth();

    Ogre::uchar *oriImageData = oriImage->getData();
    // 所选纹理的大小
    size_t newImageWidth = texInfo.width*TexTileSize;
    size_t newImageHeight = texInfo.height*TexTileSize;

    // 分配一个足够大的空间来保存新建的image的数据
    size_t newImagegetRowSpan = newImageWidth*oriImage->getBPP()/BytePerPixel;  // 新建的image的行宽（单位为字节）
    Ogre::uchar *newImageData = new Ogre::uchar[newImageHeight*newImagegetRowSpan];
    
    Ogre::uchar *newImageDataPointer = newImageData;
    // 得知起始像素点
    size_t startPoint = ( oriImageWidth * texInfo.topCorner + texInfo.leftCorner )
        * TexTileSize * oriImage->getBPP()/BytePerPixel;

    Ogre::uchar *oriImagedataPointer = oriImageData + startPoint;

    // 把所选的纹理的数据提取出来，并创建一个新的image
    for ( Ogre::uint i=0; i<newImageHeight; ++i )
    {
        memcpy(newImageDataPointer, oriImagedataPointer, newImagegetRowSpan);
        newImageDataPointer += newImagegetRowSpan;
        oriImagedataPointer += oriImage->getRowSpan();
    }

    Ogre::Image newImage;
    newImage.loadDynamicImage(newImageData,newImageWidth,newImageHeight,1,oriImage->getFormat(),true);

    // 如果所选纹理大于64*64，就先resize
    if ( texInfo.width > 1 || texInfo.height > 1 )
        newImage.resize(mPreviewImageWidth, mPreviewImageHeight);

    // 如果有alpha，要与黑白图进行混合
    if ( newImage.getHasAlpha() )
    {
        Ogre::uchar *tempPtr = newImage.getData();

        assert (tempPtr);

        for ( int i=0; i<mPreviewImageHeight; ++i )
            for ( int j=0; j<mPreviewImageWidth; ++j )
            {
                // 取出alpha值
                float alphaValue = (float)tempPtr[3] / 255.0f;

                // 计算出经过alpha混合后的颜色值
                unsigned char r = tempPtr[2] * alphaValue + mBlackWhitePreviewImage.GetRed(i,j) * ( 1.0f - alphaValue);
                unsigned char g = tempPtr[1] * alphaValue + mBlackWhitePreviewImage.GetGreen(i,j) * ( 1.0f - alphaValue);
                unsigned char b = tempPtr[0] * alphaValue + mBlackWhitePreviewImage.GetBlue(i,j) * ( 1.0f - alphaValue);

                // 设置到image中
                mCurrentPreviewImage.SetRGB(j,i,r,g,b);
                tempPtr += 4;
            }

            // 要把指针移回到图片数据的
            tempPtr -= mPreviewImageHeight * mPreviewImageWidth * 4;
            // 设置到缩略图控件中
            mBrushesPreview->SetBitmap(mCurrentPreviewImage);
    }
    // 没有alpha，就直接拷贝数据
    else
    {
        memcpy ( mCurrentPreviewImage.GetData(), newImage.getData(), newImage.getSize() );
        mBrushesPreview->SetBitmap(mCurrentPreviewImage);    
    }
}


