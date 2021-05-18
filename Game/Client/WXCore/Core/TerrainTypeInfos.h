/********************************************************************
filename:   TerrainTypeInfos.h

purpose:    用于保存，读取地表纹理的类型信息（草地，土地等），用于客户
            端进行各种逻辑计算
*********************************************************************/

#ifndef _TERRAINTYPEINFO_H_
#define _TERRAINTYPEINFO_H_

#include "WXPrerequisites.h"

#include <OgreMath.h>

namespace WX {
    
    class TerrainData;

    class TerrainTypeInfos
    {
    public:

        /// 各种地形类型的标志位
        enum TerrainTypeFlag
        {
            TerrainType_NORMAL          = 1 << 0,   // 普通地形，不发特效
            TerrainType_GRASS           = 1 << 1,   // 草地
            TerrainType_SOIL            = 1 << 2,   // 土地
            TerrainType_SNOW            = 1 << 3,   // 雪地
            TerrainType_LIQUID           = 1 << 4,   // 水
            TerrainType_STONE           = 1 << 5,
        };  

        typedef uchar TerrainType;

        struct TerrainTypeInfo
        {
            TerrainTypeInfo() :
            mTerrainType(TerrainType_NORMAL),
                mExtraData(Ogre::Math::POS_INFINITY)
            {

            }

            TerrainTypeInfo(TerrainType type, float extraData) :
            mTerrainType(type),
            mExtraData(extraData)
            {

            }

            /// 地形类型
            TerrainType mTerrainType;
            /// 额外信息，现在用于保存高度信息
            float mExtraData;
        };
        typedef std::vector<TerrainTypeInfo> TerrainTypeInfoArray;

    public:

        TerrainTypeInfos(void);

        /** 设置地形格的类型信息
        @param x 地形格x坐标
        @param z 地形格z坐标
        @param type 类型信息
        @param extraData 额外信息
        */
        void setTerrainTypeInfo(int x, int z, TerrainType type, float extraData = Ogre::Math::POS_INFINITY);

        /** 获取地形的类型信息
        @param x 地形格x坐标
        @param z 地形格z坐标
        */
        const TerrainTypeInfo& getTerrainTypeInfo(int x, int z);

        /** 获取地形的类型信息
        @param x 世界x坐标
        @param z 世界z坐标
        */
        const TerrainTypeInfo& getTerrainTypeInfo(float x, float z);

        /** 更新当前场景的地形类型信息
        @remarks 在调用这个函数之前必须调用setTerrainData把当前地形数据设置进去
        */
        void updateTerrainTypeInfos(void);

        /** 解析类型信息定义文件
        */
        void parseTerrainTypeConfigFile(const String& filename);

        /// 输出调试文本文件
        void outputDebugText(const String& filename);

        /// 设置当前的地形数据
        void setTerrainData(TerrainData* terrainData)
        {
            mTerrainData = terrainData;
        }

        void getEffectInfo(float x, float z, const String& templateName, String& effectName, float& height);

    protected:

        TerrainTypeInfoArray mTerrainTypeInfos;

        TerrainData* mTerrainData;

        typedef std::map<String, TerrainType> TerrainTypeMap;
        TerrainTypeMap mTerrainTypeMap;

        typedef std::map<String, TerrainType> TextureNameTerrainTypeMap;
        TextureNameTerrainTypeMap mTextureNameTerrainTypeMap;

        struct EffectTemplate
        {
            String mEffectName;
            String mTemplateName;
        };

        typedef std::list<EffectTemplate> EffectTemplateList;
        typedef std::map<TerrainType, EffectTemplateList> TerrainTypeEffectMap;
        TerrainTypeEffectMap mTerrainTypeEffectMap;

    protected:

        /// 根据地形类型获取特效名称
        const String _getEffectName(TerrainType type, const String& templateName);
    };
}

#endif