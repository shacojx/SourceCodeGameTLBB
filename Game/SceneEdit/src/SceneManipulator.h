#ifndef __SCENEMANIPULATOR_H__
#define __SCENEMANIPULATOR_H__

#include "Core/WXSystem.h"
#include "WXSceneListener.h"
#include "PaintAction.h"
#include "TerrainPaintInfoContainer.h"

#include "WXSceneEditingData.h"

#include <OgreString.h>

namespace WX {

// ----------------------------------------------------------------------------
// forward declares
// ----------------------------------------------------------------------------

class XMLParser;
class XMLWriter;

class OperatorManager;

class FrameStatsListener;
class SceneInfo;
class Terrain;
class TerrainData;

class Manip;
class HitIndicator;
class Selection;
class BrushShape;

class Action;

struct HitInfo;

struct FuncParam;
class TerrainGrid;

class TerrainPaintInfoContainer;

class EnviromentObject;

class TerrainTypeInfos;

typedef std::map<Ogre::String, Ogre::Image *> PreviewImageMap;

const Ogre::String FairyAutoEnviromentObjectName = "#Enviroment";
const Ogre::String FairyAutoSkyDomeObjectName = "#SkyDome";
const Ogre::String FairyAutoMainLightOneObjectName = "#MainLightOne";
const Ogre::String FairyAutoMainLightTwoObjectName = "#MainLightTwo";

// ----------------------------------------------------------------------------
// classes
// ----------------------------------------------------------------------------

/** An scene manipulator
@remarks
@par
*/
class SceneManipulator : public System, public SceneListenerManager
{
public:
    void resetScene(void);
    void newScene(Ogre::String& algorithm,size_t width, size_t depth, size_t tileSize = 32,
        const Ogre::String& baseTexture = Ogre::StringUtil::BLANK,
        size_t seed=0,size_t scale =100,Ogre::Real height = 100, Ogre::Real h = 0.85f);
    void loadScene(const String& filename);
    void saveScene(const String &filename);

    bool isModified(void) const;
    bool isEditable(void) const;

    // -------------------------------------------
    // Accessor
    // -------------------------------------------

    XMLParser* getXMLParser(void) const             { return mXMLParser; }
    XMLWriter* getXMLWriter(void) const             { return mXMLWriter; }

    OperatorManager* getOperatorManager(void) const { return mOperatorManager; }

    /** Gets the indicator scene node.
    @remark
        All editor special renderables, movables, scene nodes MUST be attach under this node
    */
    Ogre::SceneNode* getIndicatorRootSceneNode(void) const  { return mIndicatorRootSceneNode; }

    // -------------------------------------------
    // Helper
    // -------------------------------------------

    void moveCameraToUsableLocation(void);

    bool makeTerrainHitInfo(HitInfo& hitInfo, Real winx, Real winy);
    bool makeTerrainHitInfo(HitInfo& hitInfo, const Point& pt);

    // -------------------------------------------
    // Objects accessor
    // -------------------------------------------

    void addObject(const ObjectPtr& object, SceneListener* exclude = 0, bool disableUndo = false);
    void removeObject(const ObjectPtr& object, SceneListener* exclude = 0, bool disableUndo = false);

    void renameObject(const ObjectPtr& object, const String& newName, SceneListener* exclude = 0, bool disableUndo = false);

	void removeObject(const Ogre::String objName);

    typedef std::set<ObjectPtr> ObjectSet;

    const ObjectSet& getSelectedObjects(void) const;
    bool hasSelectedObjects(void) const;
    bool isObjectSelected(const ObjectPtr& object) const;
    bool selectObject(const ObjectPtr& object, SceneListener* exclude = 0);
    bool deselectObject(const ObjectPtr& object, SceneListener* exclude = 0);
    bool clearSelectedObjects(SceneListener* exclude = 0);
    bool removeSelectedObjects(SceneListener* exclude = 0, bool disableUndo = false);
    // 整批地添加object，用于拷贝粘贴
    bool addOjbects(const ObjectSet& objects, SceneListener* exclude = 0, bool disableUndo = false);
    // -------------------------------------------
    // Mouse manip
    // -------------------------------------------

    Manip* getManip(const String& name) const;
    HitIndicator* getHitIndicator(const String& name) const;

    BrushShape* _getBrushShape(const String& type) const;
    Selection* _getSelection(const String& type) const;

    Action* _getAction(const String& name) const;

    void setActiveAction(const String& name);
    void setActiveAction(Action* action);
    Action* getActiveAction(void) const { return mActiveAction; }

    bool isHitPointShown(void) const    { return mHitPointShown; }
    void showHitPoint(bool show);

    Real getBaseScale(void) const       { return mBaseScale; }

    void setMoveFactor(Real value)      { mMoveFactor = value; }
    Real getMoveFactor(void) const      { return mMoveFactor; }
    Real _getMoveFactor(void) const     { return mMoveFactor * mBaseScale; }

    void setRotateFactor(Real value)    { mRotateFactor = value; }
    Real getRotateFactor(void) const    { return mRotateFactor; }

    void setMoveSpeed(Real value)       { mMoveSpeed = value; }
    Real getMoveSpeed(void) const       { return mMoveSpeed; }
    Real _getMoveSpeed(void) const      { return mMoveSpeed * mBaseScale; }

    void setRotateSpeed(Real value)     { mRotateSpeed = value; }
    Real getRotateSpeed(void) const     { return mRotateSpeed; }

    void setWheelSpeed(Real value)      { mWheelSpeed = value; }
    Real getWheelSpeed(void) const      { return mWheelSpeed; }
    Real _getWheelSpeed(void) const     { return mWheelSpeed * mBaseScale; }

    void setCameraHeightLocked(bool value)  { mCameraHeightLocked = value; }
    bool getCameraHeightLocked(void) const  { return mCameraHeightLocked; }

    void setShowStandardModel(bool value)   { mShowStandardModel = value; }
    bool getShowStandardModel(void) const   { return mShowStandardModel; }

    void setHeightAdjustSpeed(Real value)   { mHeightAdjustSpeed = value; }
    Real getHeightAdjustSpeed(void) const   { return mHeightAdjustSpeed; }
    Real _getHeightAdjustSpeed(void) const  { return mHeightAdjustSpeed * mBaseScale; }

    Real getPositionAlignGranularity(void) const    { return mPositionAlignGranularity; }
    Real getRotationAlignGranularity(void) const    { return mRotationAlignGranularity; }
    void setPositionAlignGranularity(Real value)    { mPositionAlignGranularity = value; }
    void setRotationAlignGranularity(Real value)    { mRotationAlignGranularity = value; }
    Ogre::Vector3 _alignPosition(const Ogre::Vector3& value);
    Ogre::Vector3 _alignPosition(const Ogre::Vector3& value, Real granularity);
    void _snapSelectedObjectsPosition(Real granularity);
    Ogre::Quaternion _alignRotation(const Ogre::Quaternion& value);
    Ogre::Quaternion _alignRotation(const Ogre::Quaternion& value, Real granularity);
    void _snapSelectedObjectsRotation(Real granularity,int axis);
    void calcOffsetRadian(const Ogre::Degree& degree, const float& granularity,float& remainder);

    void setBrush(const String& type, size_t xsize, size_t zsize);
    const String& getBrushType(void) const  { return mBrushType; }
    size_t getBrushXSize(void) const        { return mBrushXSize; }
    size_t getBrushZSize(void) const        { return mBrushZSize; }
    void setBrushXSize(size_t size)         { mBrushXSize = size; }
    void setBrushZSize(size_t size)         { mBrushZSize = size; }
    void _buildSelection(Selection* selection, Real xcentre, Real zcentre);
    void _buildSelection(const String& selectionType, Real xcentre, Real zcentre);

    /// 真实摄像机相关的设置函数
	void setDefaultRealCameraHeight( const Real height ) { mDefaultRealCameraHeight = height; }
	void setDefaultRealCameraAngle( const Real angle ) { mDefaultRealCameraAngle = angle; }
	float getDefaultRealCameraHeight(void) { return mDefaultRealCameraHeight; }
	float getDefaultRealCameraAngle(void) { return mDefaultRealCameraAngle; }

    void setRealCameraHeight( const Real height ) { mRealCameraHeight = height; }
    void setRealCameraAngle( const Real angle );

	void setUseRealCameraHeight( bool use )	{	mUseRealCameraHeight = use;	}
	void setUseRealCameraAngle( bool use )	{	mUseRealCameraAngle = use;	}

    float getRealCameraHeight(void) { return mRealCameraHeight; }
    float getRealCameraAngle(void) { return mRealCameraAngle; }
	bool getUseRealCameraHeight(void) { return mUseRealCameraHeight;	}
	bool getUseRealCameraAngle(void)	{	return mUseRealCameraAngle;	}

    /// 在移动摄像机的action中设置真实的摄像机
    void setRealCamera( const Real currentX, const Real currentY );

    /// 填写纹理是否透明的信息
    void AddTransparentTexture( const String &textureName, bool transparent = true );
    /// 判断该纹理是否是透明的
    bool IsTransparentTexture( const String &textureName );

    /// 获取当前选中的物体的属性，构成字符串，放在clipboard中
    void getSelectedObjectInfo( Ogre::String &infoString );

    /// 根据从clipboard中得到的保存物体属性的字符串，来paste物体
    void parseStringAndCreateObject( const Ogre::String &infoString, Ogre::Vector3 &position );

	    // 获取画刷map
    const TerrainPaintInfoContainer* getTerrainPaintInfoContainer(void) const
    {
        return mTerrainPaintInfoContainer;
    }

    // 根据brush名称获取brush数组
    const TextureInfos& getTextureInfos( const Ogre::String &brushName );

    TextureInfo getTextureInfo(const Ogre::String &brushName, const Ogre::String &textureName);

    Ogre::Image * getPreviewImage( const Ogre::String &textureName );

    void setPaintPixmapInfo( PaintPixmap &pixmap, const TextureInfo &texInfo );

    void reloadPaintInfo(void);  

	/// 设置改变地形高度的影响范围
	void setJunctionWeightAffectRange( int range )	{	mAffectRange = range;	}
	int getJunctionWeightAffectRange(void) {	return mAffectRange; }

	/// 设置改变地形高度时所用的k值
	void setJunctionWeightExpValue( float exp ) {	mExponentValue = exp; }
	float getJunctionWeightExpValue(void)	{	return mExponentValue;	}

	/// 根据所选的纹理组名，找出当前地形是否用到了这组纹理，返回用到该组纹理的次数
	size_t findGridPaintInfo( String texGroupName );

	String getTexGroupNameFromTextureName( String &textureName );
	TexType getTexTypeFromTextureName( String &textureName );

	//const GridPaintInfoList &getResultPaintInfoList(void)
	//{
	//	return mResultGridPaintInfos;
	//}

	void setSelectAllObjects( bool set = true )	{	mSelectAllObjects = set;	}
	bool getSelectAllObjects(void)	{	return mSelectAllObjects;	}

	bool IsCenterSelectObject( Ogre::String objectType );

	void fillUnableCenterSelectObject(void);	

	/// 删除地形上用指定纹理刷的地形纹理
	bool eraseTerrainPixmapByTextureName(const String &texName);

    /// 获取场景编辑所需的额外信息
    SceneEditingData& getSceneEditingData(void)
    {
        return mSceneEditingData;
    }

    /// 设置指示器节点的可见性
    void setIndicatorNodeVisible(bool visible);
    bool getIndicatorNodeVisible(void)
    {
        return mIndicatorNodeVisible;
    }

    /// 设置摄像机的位置
    /** 传入目的地坐标，会根据不同情况进行处理（如当前摄像机是否锁定高度）
    */
    void setCameraPosition(const Ogre::Vector3& pos);

    /// 获取地形类别信息
    TerrainTypeInfos* getTerrainTypeInfos(void)
    {
        return mTerrainTypeInfos;
    }

public:

    SceneManipulator(void);
    ~SceneManipulator();

protected:

    void registerManips(void);
    void registerManip(Manip* manip);
    void destoryManips(void);

    void registerHitIndicators(void);
    void registerHitIndicator(HitIndicator* hitIndicator);
    void destoryHitIndicators(void);

    void registerBrushShapes(void);
    void registerBrushShape(BrushShape* brushShape);
    void destoryBrushShapes(void);

    void registerSelections(void);
    void registerSelection(Selection* selection);
    void destorySelections(void);

    void registerActions(void);
    void registerAction(Action* action);
    void destoryActions(void);

    void checkStdObjects(bool autoCreate = true);
    void initFuncMap();

    void registerAddObjectPlugins(void);
    void destroyAddObjectPlugins(void);

	void loadTerrainPaintInfo(XMLParser* parser, const String& name, const String& groupName);

    void clearPreviewImage(void);

protected:

    // Overrides

    void _loadResources(void);
    void _initScene(void);
    void _preprocessScene(void);
    void _postprocessScene(void);

    // Editor special helpers

    void _createTerrain(Ogre::String& algorithm,size_t width, size_t depth, size_t tileSize = 32,
                        const String& baseTexture = Ogre::StringUtil::BLANK,
                        size_t seed = 0, Real scale = 100, Real height = 100, Real h = 0.85f);

    void _checkStdObjects(bool autoCreate = true);

protected:

    // XML load/save
    XMLParser* mXMLParser;
    XMLWriter* mXMLWriter;

    // Configure

    Real mMoveFactor;
    Real mRotateFactor;
    Real mMoveSpeed;
    Real mRotateSpeed;
    Real mWheelSpeed;
    bool mCameraHeightLocked;
    Real mHeightAdjustSpeed;
    Real mPositionAlignGranularity;
    Real mRotationAlignGranularity;
    bool mShowStandardModel;
    String mBrushType;
    size_t mBrushXSize;
    size_t mBrushZSize;

    /// 真实摄像机的高度，角度
    Real mRealCameraHeight;
    Real mRealCameraAngle;
    Real mRealLastCameraAngle;

	Real mDefaultRealCameraAngle;
	Real mDefaultRealCameraHeight;

    /// 是否使用真实摄像机
	bool mUseRealCameraHeight;
	bool mUseRealCameraAngle;

	/// 计算地形画刷权重所需的参数
	int mAffectRange;
	float mExponentValue;
    // Manips

    typedef std::map<String, Manip*> Manips;
    Manips mManips;

    typedef std::map<String, HitIndicator*> HitIndicators;
    HitIndicators mHitIndicators;

    typedef std::map<String, BrushShape*> BrushShapes;
    BrushShapes mBrushShapes;

    typedef std::map<String, Selection*> Selections;
    Selections mSelections;

    typedef std::map<String, Action*> Actions;
    Actions mActions;
    Action* mActiveAction;

    // Undo/redo manage

    typedef size_t Timestamp;
    typedef std::map<String, Timestamp> Snapshot;

    OperatorManager* mOperatorManager;
    Snapshot mSnapshot;

    Ogre::SceneNode* mIndicatorRootSceneNode;

    ObjectSet mSelectedObjects;

    // Flags
    bool mHitPointShown;

    // 场景中不可见的物体的指示器（光源等）
    typedef std::vector<WX::SceneListener *> AddObjectPlugins;
    AddObjectPlugins mAddObjectPlugins;

    // 透明纹理列表，保存带alpha通道的纹理的名称
    typedef std::map<String, bool> TransparentTextures;
    TransparentTextures mTransparentTextures;

    bool mObjectsUseBoxCenter;
    bool mAxisGizmoActive;

	/// 已设置的GridPaintInfo
	//GridPaintInfoList mPaintedGridPaintInfos;
	/// 搜索到的GridPaintInfo
	//GridPaintInfoList mResultGridPaintInfos;
	TerrainGrid* mTerrainGrid;

    PreviewImageMap mPreviewImageMap;
    TerrainPaintInfoContainer *mTerrainPaintInfoContainer;

	/// 场景中可以选择的，但不能用中心点选择的物体列表
	typedef std::list<Ogre::String> UnableCentenSelectObject;
	UnableCentenSelectObject mUnableCentenSelectObject;

	/// 当前的选择状态（是否选择可以用中心点进行选择的物体）
	bool mSelectAllObjects;

    SceneEditingData mSceneEditingData;

    bool mIndicatorNodeVisible;

    TerrainTypeInfos* mTerrainTypeInfos;

public:

    //createScene functables
    typedef void (*algorithmFunc) (FuncParam* param);
    typedef std::map<Ogre::String,algorithmFunc> FuncMap;
    FuncMap funcMap;

    void fillFuncParam(FuncParam* param,int seed,Ogre::Real scale);
    void setUseBoxCenterSelection(int x,int y,bool use);
    bool getUseBoxCenterSelection();
    bool getUseAxis();
    void setUseAxis(bool use);
	TerrainGrid* getTerrainGrid();	
};


}

#endif // __SceneManipulator_H__
