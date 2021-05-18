/********************************************************************
filename:   WXMiniMapMaker.h

purpose:    根据场景内容，自动生成一张mini map
*********************************************************************/

#ifndef _MINIMAPMAKER_H_
#define _MINIMAPMAKER_H_

#include "Core/WXPrerequisites.h"

#include <OgrePixelFormat.h>
#include <OgreVector3.h>

namespace Ogre  {
    class RenderTexture;
    class Camera;
}

namespace WX {

    class SceneManipulator;

    class MiniMapMaker
    {
    public:

        enum CameraDirectionType
        {
            CDT_MiniMap,
            CDT_CameraDirection,
            CDT_North,
            CDT_South,
            CDT_West,
            CDT_East,
        };

    public:

        MiniMapMaker( SceneManipulator* manipulator );

        ~MiniMapMaker();

        /// 初始化，包括创建render texture，camera等
        void init(void);
        /// 销毁render texture
        void destroy(void);

        /// 输出mini map
        bool outputTextures(void);

        /**
        设置每块小纹理上覆盖的边长
        @param tileSize 边长，世界坐标，如1600表示每块小纹理拍下的区域就是1600*1600
        */
        void setTileSize(Real tileSize)
        {
            mTileSize = tileSize;
        }
        const Real getTileSize(void)
        {
            return mTileSize;
        }

        void setCameraPerAngle(Real angle);

        /// 设置输出文件扩展名，如“png”
        void setTexExtension( const String& extension );
        const String& getTexExtension(void)
        {
            return mTexExtension;
        }

        /// 设置小纹理的宽度
        void setTexWidth(uint width);
        const uint getTexWidth(void)
        {
            return mTexWidth;
        }

        /// 设置小纹理的高度
        void setTexHeight(uint height);
        const uint getTexHeight(void)
        {
            return mTexHeight;
        }

        /**
            设置输出文件名
            默认名称是场景名称
        */
        void setOutputFileName(const String& fileName)
        {
            mOutFileName = fileName;
        }
        const String& getOutputFileName(void)
        {
            return mOutFileName;
        }

        /// 设置当前场景的名称（包括路径）
        void setCurrentSceneName(const String& sceneName);

        /// 获取场景名称
        const String& getBaseName(void)
        {
            return mSceneBaseName;
        }

        /// 获取中间生成的小纹理的文件名，用于删除
        std::vector<String>& getTempFileNames(void)
        {
            return mTempOutputFileNames;
        }

        void setUseRealCameraAngle(bool use)
        {
            mUseRealCameraAngle = use;
        }

        void setCameraDirectionType(CameraDirectionType dirType)
        {
            mCameraDirectionType = dirType;
        }

        const String& getLastErrorString(void)
        {
            return mLastErrorString;
        }

    private:
        
        void _computeCameraDirQuadrant(void);

        void _setCameraAttribute(void);    

    private:

        Ogre::Camera* mCamera;
        Ogre::RenderTexture* mRenderTexture;

        uint mTexHeight;
        uint mTexWidth;

        SceneManipulator* mManipulator;

        Real mTileSize;
        Real mPerAngle;

        /// 摄像机方向与地面的夹角
        Ogre::Radian mCamDirAngle;

        String mTexExtension;
        String mOutFileName;

        String mSceneBaseName;
        String mPath;

        std::vector<String> mTempOutputFileNames;

        bool mNeedRecreate;

        Ogre::PixelFormat mOutPutFormat;

        bool mUseRealCameraAngle;

        Ogre::Vector3 mMoveZDir;

        /// 当前场景中摄像机的方向
        Ogre::Vector3 mCameraDir;

        /// 摄像机方向的反向
        Ogre::Vector3 mInvertCameraDir;

        enum CameraDirQuadrant
        {
            WestNorth,
            EastNorth,
            EastSouth,
            WestSouth,
            None,
        };

        CameraDirQuadrant mCameraDirQuadrant; 

        CameraDirectionType mCameraDirectionType;

        String mLastErrorString;
    };
}

#endif