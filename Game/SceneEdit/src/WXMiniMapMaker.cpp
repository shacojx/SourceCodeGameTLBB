/********************************************************************
filename:   WXMiniMapMaker.cpp

purpose:    根据场景内容，自动生成一张mini map
*********************************************************************/

#include "WXMiniMapMaker.h"
#include "SceneManipulator.h"

#include <OgreRenderTexture.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreRenderSystem.h>
#include <OgreViewport.h>
#include <OgreColourValue.h>
#include <OgreStringConverter.h>
#include <OgreTextureManager.h>
#include <OgreEntity.h>
#include <OgreMath.h>
#include <OgreHardwarePixelBuffer.h>

#include "Core/TerrainData.h"

namespace WX {

    MiniMapMaker::MiniMapMaker( SceneManipulator* manipulator ) :
    mManipulator(manipulator),
    mRenderTexture(NULL),
    mCamera(NULL),
    mTexHeight(128),
    mTexWidth(128),
    mTileSize(800.0f),
    mPerAngle(90.0f),
    mTexExtension("jpg"),
    mOutFileName(""),
    mSceneBaseName(""),
    mPath(""),
    mNeedRecreate(true),
    mOutPutFormat(Ogre::PF_BYTE_RGB),
    mUseRealCameraAngle(false),
    mMoveZDir(Ogre::Vector3::ZERO),
    mCameraDir(Ogre::Vector3::ZERO),
    mCameraDirQuadrant(None),
    mCamDirAngle(0.0f),
    mInvertCameraDir(Ogre::Vector3::ZERO),
    mCameraDirectionType(CDT_MiniMap),
    mLastErrorString("")
    {
        mTempOutputFileNames.clear();
    }

    MiniMapMaker::~MiniMapMaker()
    {
        destroy();
    }

    void MiniMapMaker::init(void)
    {
        // 创建纹理
        Ogre::TexturePtr pTexture = Ogre::TextureManager::getSingleton().createManual(
            "RttTex", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            Ogre::TEX_TYPE_2D,
            mTexWidth, mTexHeight, 1, 0, mOutPutFormat,
            Ogre::TU_RENDERTARGET, 0);

        Ogre::HardwarePixelBufferSharedPtr pBuffer = pTexture->getBuffer(0, 0);

        mRenderTexture = pBuffer->getRenderTarget(0);
        {
            mCamera = mManipulator->getSceneManager()->createCamera("RttCam");

            // 设置摄像机的基本属性
            mCamera->setAspectRatio(1);
            mCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
            mCamera->setFOVy(Ogre::Degree(90));            

            _setCameraAttribute();

            Ogre::Viewport *v = mRenderTexture->addViewport( mCamera );
            v->setClearEveryFrame( true );
            v->setBackgroundColour( Ogre::ColourValue::Black );   
            v->setOverlaysEnabled(false);
            v->setSkiesEnabled(false);
            v->setShadowsEnabled(true);
        }
    }

    void MiniMapMaker::destroy(void)
    {
        if (mRenderTexture)
        {
            mManipulator->getSceneManager()->destroyCamera("RttCam");

            mRenderTexture->removeAllViewports();

            Ogre::TextureManager::getSingleton().remove("RttTex");

            mRenderTexture = NULL;
        }
    }

    bool MiniMapMaker::outputTextures(void)
    {
        // 如果需要（纹理大小改变了或第一次输出文件时），就重建render texture
        if (mNeedRecreate)
        {
            destroy();
            init();
        }

        mTempOutputFileNames.clear();

        static const String TEMP_GROUP_NAME = "#TEMP#";

        // 创建临时的资源组
        Ogre::ResourceGroupManager& rgm = Ogre::ResourceGroupManager::getSingleton();
        rgm.addResourceLocation(mPath, "FileSystem", TEMP_GROUP_NAME, false);

        // 合并所有物体的包围盒
        Ogre::AxisAlignedBox aabb;
        Ogre::SceneManager::MovableObjectIterator itm =
            mManipulator->getSceneManager()->getMovableObjectIterator(Ogre::EntityFactory::FACTORY_TYPE_NAME);
        while (itm.hasMoreElements())
        {
            Ogre::MovableObject* movable = itm.getNext();
            aabb.merge(movable->getWorldBoundingBox(true));
        }

        mCamera->setFarClipDistance(mCamera->getNearClipDistance() + 2 * (aabb.getMaximum().y - aabb.getMinimum().y ));
        mCamera->setNearClipDistance(mTileSize/2);

        // 设置摄像机的高度
        Real yPos = mCamera->getNearClipDistance() + aabb.getMaximum().y;
        
        TerrainData* terrainData = mManipulator->getTerrainData();
        assert (terrainData);

        float terrainHeight = terrainData->mMaxZ - terrainData->mMinZ;
        float terrainWidth = terrainData->mMaxX - terrainData->mMinX;

        // 投影的真正面积
        Real projectSize = 0.0f;

        // 最终切割成小块纹理的块数
        int xIndex = 0;
        int zIndex = 0;

        Ogre::Vector3 originPoint(Ogre::Vector3::ZERO);

        if (mUseRealCameraAngle)
        {
            float outerSquareWidth = 0.0f;
            float outerSquareHeight = 0.0f;

            Ogre::Radian alphaAngle = Ogre::Math::ATan( Ogre::Math::Abs(mMoveZDir.z / mMoveZDir.x) );

            switch (mCameraDirQuadrant)
            {
            case WestNorth :
                {
                    float leftWidth = Ogre::Math::Sin(alphaAngle) * terrainHeight;
                    float rightWidth = Ogre::Math::Cos(alphaAngle) * terrainWidth;
                    outerSquareWidth = leftWidth + rightWidth;

                    float topHeight = Ogre::Math::Cos(alphaAngle) * terrainHeight;
                    float bottomHeight = Ogre::Math::Sin(alphaAngle) * terrainWidth;

                    outerSquareHeight = topHeight + bottomHeight;

                    originPoint = Ogre::Vector3(terrainData->mMinX,0,terrainData->mMinZ) +
                        (-mMoveZDir * leftWidth);

                    float projectOffset = yPos / Ogre::Math::Tan(mCamDirAngle);

                    originPoint.x += (mInvertCameraDir * projectOffset ).x;
                    originPoint.z += (mInvertCameraDir * projectOffset ).z;

                    break;
                }
            case EastNorth :
                {
                    float leftWidth = Ogre::Math::Cos(alphaAngle) * terrainWidth;
                    float rightWidth = Ogre::Math::Sin(alphaAngle) * terrainHeight;
                    outerSquareWidth = leftWidth + rightWidth;

                    float topHeight = Ogre::Math::Cos(alphaAngle) * terrainHeight;
                    float bottomHeight = Ogre::Math::Sin(alphaAngle) * terrainWidth;

                    outerSquareHeight = topHeight + bottomHeight;

                    originPoint = Ogre::Vector3(terrainData->mMaxX,0,terrainData->mMinZ) +
                        (-mMoveZDir * leftWidth);

                    float projectOffset = yPos / Ogre::Math::Tan(mCamDirAngle);

                    originPoint.x += (mInvertCameraDir * projectOffset ).x;
                    originPoint.z += (mInvertCameraDir * projectOffset ).z;

                    break;
                }
            case EastSouth :
                {
                    float leftWidth = Ogre::Math::Sin(alphaAngle) * terrainHeight;
                    float rightWidth = Ogre::Math::Cos(alphaAngle) * terrainWidth;
                    outerSquareWidth = leftWidth + rightWidth;

                    float topHeight = Ogre::Math::Sin(alphaAngle) * terrainWidth;
                    float bottomHeight = Ogre::Math::Cos(alphaAngle) * terrainHeight;

                    outerSquareHeight = topHeight + bottomHeight;

                    originPoint = Ogre::Vector3(terrainData->mMaxX,0,terrainData->mMaxZ) +
                        (-mMoveZDir * topHeight);

                    float projectOffset = yPos / Ogre::Math::Tan(mCamDirAngle);

                    originPoint.x += (mInvertCameraDir * projectOffset ).x;
                    originPoint.z += (mInvertCameraDir * projectOffset ).z;

                    break;
                }
            case WestSouth :
                {
                    float leftWidth = Ogre::Math::Sin(alphaAngle) * terrainHeight;
                    float rightWidth = Ogre::Math::Cos(alphaAngle) * terrainWidth;
                    outerSquareWidth = leftWidth + rightWidth;

                    float topHeight = Ogre::Math::Sin(alphaAngle) * terrainWidth;
                    float bottomHeight = Ogre::Math::Cos(alphaAngle) * terrainHeight;

                    outerSquareHeight = topHeight + bottomHeight;

                    originPoint = Ogre::Vector3(terrainData->mMinX,0,terrainData->mMaxZ) +
                        (-mMoveZDir * rightWidth);

                    float projectOffset = yPos / Ogre::Math::Tan(mCamDirAngle);

                    originPoint.x += (mInvertCameraDir * projectOffset ).x;
                    originPoint.z += (mInvertCameraDir * projectOffset ).z;

                    break;
                }

            default:
                {
                    OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        " wrong camera dir " + Ogre::StringConverter::toString(mCameraDir),
                        "MiniMapMaker::outputTextures");

                    break;
                }
            }

            // 计算投影的长度
            Real factor = Ogre::Math::Sin(mCamDirAngle);

            if (factor > 0.0f && factor != 1.0f)
                projectSize = mTileSize / factor;

            // 根据当前场景的大小，计算需要的分块数
            xIndex = Ogre::Math::Ceil( (outerSquareWidth) / mTileSize ) + 1;
            zIndex = Ogre::Math::Ceil( (outerSquareHeight) / projectSize ) + 1;
        }
        else
        {
            xIndex = Ogre::Math::Ceil( (terrainData->mMaxX - terrainData->mMinX) / mTileSize ) + 1;
            zIndex = Ogre::Math::Ceil( (terrainData->mMaxZ - terrainData->mMinZ) / mTileSize ) + 1;

            originPoint.x = terrainData->mMinX;
            originPoint.z = terrainData->mMinZ;
        }

        // 计算最终的mini map的大小
        uint miniMapWidth = xIndex * mTexWidth;
        uint miniMapHeight = zIndex * mTexHeight;

        if ( miniMapWidth > 10000 || miniMapHeight > 10000 )
        {
            mLastErrorString = "texture size is out of range!";
            return false;
        }
        // 创建mini map所需的内存空间
        uchar* miniMapData = new uchar[miniMapWidth * miniMapHeight * Ogre::PixelUtil::getNumElemBytes(mOutPutFormat)];

        //// 初始的摄像机位置
        Real xPos = originPoint.x;
        Real zPos = originPoint.z;

        for ( int i=0; i<xIndex; ++i )
        {
            for ( int j=0; j<zIndex; ++j )
            {
                // 设置摄像机位置，并更新render texture的内容
                mCamera->setPosition(xPos, yPos, zPos);
                mRenderTexture->update();

                String fileName = mPath + mSceneBaseName + Ogre::StringConverter::toString(i)
                    + "_" + Ogre::StringConverter::toString(j) + "." + mTexExtension;

                // 输出小纹理文件
                mRenderTexture->writeContentsToFile(fileName);

                mTempOutputFileNames.push_back(fileName);

                // 读取刚创建的纹理
                Ogre::Image* tempImage = new Ogre::Image;
                
                tempImage->load(mSceneBaseName + Ogre::StringConverter::toString(i)
                    + "_" + Ogre::StringConverter::toString(j) + "." + mTexExtension, TEMP_GROUP_NAME);

                // 获取render texture中的内容
                uchar* tempImageData = tempImage->getData();

                // 定位在mini map中的左上角
                uint miniMapIndex = ( j * mTexHeight * miniMapWidth + i * mTexWidth ) * Ogre::PixelUtil::getNumElemBytes(mOutPutFormat);

                uchar* startData = miniMapData + miniMapIndex;

                for ( size_t height = 0; height < tempImage->getHeight(); ++height )
                {
                    for ( size_t width = 0; width < tempImage->getWidth(); ++width )
                    {
                        memcpy(startData, tempImageData, Ogre::PixelUtil::getNumElemBytes(mOutPutFormat));

                        startData += Ogre::PixelUtil::getNumElemBytes(mOutPutFormat);
                        tempImageData += Ogre::PixelUtil::getNumElemBytes( tempImage->getFormat() );
                    }
                    startData += (miniMapWidth - tempImage->getWidth()) * Ogre::PixelUtil::getNumElemBytes(mOutPutFormat);
                }

                delete tempImage;

                // 移动摄像机的z坐标
                if (mUseRealCameraAngle)
                {
                    zPos += (mInvertCameraDir * (projectSize)).z;
                    xPos += (mInvertCameraDir * (projectSize)).x;
                }
                else
                   zPos += mTileSize;
            }
            
            if (mUseRealCameraAngle)
            {
                xPos = originPoint.x;
                zPos = originPoint.z;

                xPos += (mMoveZDir * (mTileSize) * (i+1)).x;
                zPos += (mMoveZDir * (mTileSize) * (i+1)).z;
            }
            else
            {
                // 操作完每一列之后，重置摄像机的z坐标
                zPos = terrainData->mMinZ;

                // 移动摄像机的x坐标
                xPos += mTileSize;
            }
        } 

        // 保存mini map并输出
        Ogre::Image* miniMapImage = new Ogre::Image;
        miniMapImage->loadDynamicImage(miniMapData, miniMapWidth, miniMapHeight, 1, mOutPutFormat, true);

        miniMapImage->save(mPath + mOutFileName + "." + mTexExtension);

        delete miniMapImage;

        rgm.destroyResourceGroup(TEMP_GROUP_NAME);

        return true;
    }

    void MiniMapMaker::setCurrentSceneName(const String& sceneName)
    {
        // 获取路径和场景名称
        Ogre::StringUtil::splitFilename(sceneName, mSceneBaseName, mPath);

        if (Ogre::StringUtil::endsWith(mSceneBaseName, ".scene"))
            mSceneBaseName.erase(mSceneBaseName.length() - (sizeof(".scene") - 1));
    }

    void MiniMapMaker::setTexWidth(uint width)
    {
        mTexWidth = width;
        mNeedRecreate = true;
    }

    void MiniMapMaker::setTexHeight(uint height)
    {
        mTexHeight = height;
        mNeedRecreate = true;
    }

    void MiniMapMaker::setTexExtension( const String& extension )
    {
        mTexExtension = extension;

        if (mTexExtension == "png" || mTexExtension == "jpg")
            mOutPutFormat = Ogre::PF_BYTE_RGB;
        else if (mTexExtension == "bmp")
            mOutPutFormat = Ogre::PF_BYTE_BGRA;

        mNeedRecreate;
    }
    void MiniMapMaker::setCameraPerAngle(Real angle)
    {
        mPerAngle = angle;   

        /** dir为目标方向
        已知条件 : dir.y == -sin(alpha) ， 原有方向 curDir
        /
        /
        /   
        alpha   \/angle
        -------------------------------
        现在要求出dir的x和z，
        (curDir.x/s)^2 + dir.y^2 + (curDir.y/s)^2 == 1
        */
        Ogre::Vector3 direction = mCamera->getDirection();
        Real v = Ogre::Math::Sin(Ogre::Math::DegreesToRadians(angle));
        Real s = Ogre::Math::Sqrt((direction.x*direction.x + direction.z*direction.z) / (1-v*v));
        direction.x /= s;
        direction.z /= s;
        direction.y = -v;
        mCamera->setDirection(direction);

    }

    void MiniMapMaker::_computeCameraDirQuadrant(void)
    {
        float x = mCameraDir.x;
        float z = mCameraDir.z;

        if ( x < 0.0f && z < 0.0f )
            mCameraDirQuadrant = WestNorth;

        else if ( x > 0.0f && z < 0.0f )
            mCameraDirQuadrant = EastNorth;

        else if ( x < 0.0f && z > 0.0f )
            mCameraDirQuadrant = WestSouth;

        else if ( x > 0.0f && z > 0.0f )
            mCameraDirQuadrant = EastSouth;
        else
            mCameraDirQuadrant = None;
    }

    void MiniMapMaker::_setCameraAttribute(void)
    {
        switch (mCameraDirectionType)
        {
        case CDT_MiniMap :
            {
                // 设置摄像机的视角垂直向下
                Ogre::Quaternion orientation;
                orientation.FromAxes(Ogre::Vector3::UNIT_X, Ogre::Vector3::NEGATIVE_UNIT_Z, Ogre::Vector3::UNIT_Y);
                mCamera->setOrientation(orientation);

                break;
            }

        case CDT_CameraDirection :
            {
                // 获取当前摄像机的方向
                mCameraDir = mManipulator->getCamera()->getDirection();

                // 求出这个方向与地面所成的角度
                float tanValue = 
                    Ogre::Math::Abs(mCameraDir.y) / Ogre::Math::Sqrt(mCameraDir.x*mCameraDir.x + mCameraDir.z*mCameraDir.z);

                mCamDirAngle = Ogre::Math::ATan(tanValue);

                // 设置要进行小地图拍照的摄像机的方向
                mCamera->setDirection(mCameraDir);

                // 求出摄像机拍照时的横向移动的方向（该方向与摄像机的方向是垂直的）
                mMoveZDir = mCameraDir.crossProduct(Ogre::Vector3::UNIT_Y).normalisedCopy();

                // 求出摄像机拍照时的纵向移动的方向（该方向与摄像机的方向是相反的）
                mInvertCameraDir = -mCameraDir;
                // 因为我们只关心在xz平面的移动，所以y分量置0
                mInvertCameraDir.y = 0.0f;
                mInvertCameraDir.normalise();

                // 判断当前摄像机方向是指向哪个方向的
                _computeCameraDirQuadrant();

                mUseRealCameraAngle = true;

                break;
            }

        case CDT_North :
            {
                break;
            }

        case CDT_South :
            {
                break;
            }

        case CDT_West :
            {
                break;
            }

        case CDT_East :
            {
                break;
            }

        default :
            {
                break;
            }
        }        
    }
}