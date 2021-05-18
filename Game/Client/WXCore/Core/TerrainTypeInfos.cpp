/********************************************************************
filename:   TerrainTypeInfos.cpp

purpose:    用于保存，读取地表纹理的类型信息（草地，土地等），用于客户
端进行各种逻辑计算
*********************************************************************/

#include "TerrainTypeInfos.h"
#include "SerializerUtil.h"
#include "TerrainData.h"

#include <OgreResourceGroupManager.h>
#include <OgreException.h>
#include <OgreConfigFile.h>
#include <OgreLogManager.h>
#include <OgreException.h>
#include <OgreStringConverter.h>

#include <fstream>

namespace WX {

    TerrainTypeInfos::TerrainTypeInfos(void) : 
    mTerrainData(NULL)
    {
        mTerrainTypeMap.insert( TerrainTypeMap::value_type("soil", TerrainType_SOIL) );
        mTerrainTypeMap.insert( TerrainTypeMap::value_type("grass", TerrainType_GRASS) );
        mTerrainTypeMap.insert( TerrainTypeMap::value_type("snow", TerrainType_SNOW) );
        mTerrainTypeMap.insert( TerrainTypeMap::value_type("liquid", TerrainType_LIQUID) );
        mTerrainTypeMap.insert( TerrainTypeMap::value_type("stone", TerrainType_STONE) );
        // 添加普通的地形类型所对应的特效（不发特效，所有填一个空字符串）
     //   mTerrainTypeEffectMap.insert( TerrainTypeEffectMap::value_type(TerrainType_NORMAL, "") );
    }
    //-----------------------------------------------------------------------
    void TerrainTypeInfos::setTerrainTypeInfo(int x, int z, TerrainTypeInfos::TerrainType type, float extraData)
    {
        if (extraData < Ogre::Math::POS_INFINITY)
        {
            int count = 0;

            float lt = mTerrainData->getHeight(x,z);

            if (lt > extraData)
                ++count;

            float rt = mTerrainData->getHeight(x+1,z);

            if (rt > extraData)
                ++count;

            float lb = mTerrainData->getHeight(x,z+1);

            if (lb > extraData)
                ++count;

            float rb = mTerrainData->getHeight(x+1,z+1);

            if (rb > extraData)
                ++count;

            if (count >= 1)
            {
                return;
            }
        }

        mTerrainTypeInfos[mTerrainData->_getGridIndex(x, z)] = TerrainTypeInfo(type,extraData);
    }
    //-----------------------------------------------------------------------
    const TerrainTypeInfos::TerrainTypeInfo& TerrainTypeInfos::getTerrainTypeInfo(int x, int z)
    {
        return mTerrainTypeInfos[mTerrainData->_getGridIndex(x, z)];
    }
    //-----------------------------------------------------------------------
    const TerrainTypeInfos::TerrainTypeInfo& TerrainTypeInfos::getTerrainTypeInfo(float x, float z)
    {
        std::pair<int,int> grid = mTerrainData->getGrid(x,z);
        return mTerrainTypeInfos[ mTerrainData->_getGridIndex( grid.first, grid.second ) ];
    }
    //-----------------------------------------------------------------------
    void TerrainTypeInfos::updateTerrainTypeInfos(void)
    {
        /** 遍历所有地形格的纹理，先看第1层纹理，如果第1层纹理不存在，
            才检测第0层纹理
        */
        int xsize = mTerrainData->getXSize();
        int zsize = mTerrainData->getZSize();

        mTerrainTypeInfos.resize(xsize * zsize);

        for ( int x=0; x<xsize; ++x )
        {
            for( int z=0; z<zsize; ++z )
            {
                const TerrainData::GridInfo& gridInfo = mTerrainData->getGridInfo(x,z);

                String layerTexName;    // 保存当前所检查的格子所用的纹理的纹理名称
                int id = 0;				// 保存当前格子某一层的纹理id

                // 检查当前格的第1层纹理
                id = gridInfo.layers[1].pixmapId;

                if (id < 1)
                {
                    id = gridInfo.layers[0].pixmapId;

                    if (id < 1)
                        continue;

                    TerrainData::Pixmap pixmap = mTerrainData->mPixmaps[id-1];
                    layerTexName = mTerrainData->mTextures[ pixmap.textureId ];

                    TextureNameTerrainTypeMap::const_iterator it =
                        mTextureNameTerrainTypeMap.find(layerTexName);

                    TerrainType terrainType = TerrainType_NORMAL;
                    if ( it != mTextureNameTerrainTypeMap.end() )
                    {
                        terrainType = it->second;
                    }

                    setTerrainTypeInfo(x, z, terrainType);
                }
                else
                {
                    // 不考虑两层纹理的地形格
                   // TerrainData::Pixmap pixmap = mTerrainData->mPixmaps[id-1];
                   // layerTexName = mTerrainData->mTextures[ pixmap.textureId ];

                  //  TextureNameTerrainTypeMap::const_iterator it =
                  //      mTextureNameTerrainTypeMap.find(layerTexName);

                    TerrainType terrainType = TerrainType_NORMAL;
                  //  if ( it != mTextureNameTerrainTypeMap.end() )
                  //  {
                 //       terrainType = it->second;
                  //  }

                    setTerrainTypeInfo(x, z, terrainType);
                }
            }
        }
    }
    //-----------------------------------------------------------------------
    void TerrainTypeInfos::parseTerrainTypeConfigFile(const String& filename)
    {
        mTextureNameTerrainTypeMap.clear();
        mTerrainTypeEffectMap.clear();

        Ogre::ConfigFile cf;
        cf.load(filename);

        // Go through all sections & settings in the file
        Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

        String secName, texNames, effectNames;
        while (seci.hasMoreElements())
        {
            secName = seci.peekNextKey();

            seci.getNext();

            if (secName.empty())
                continue;

            TerrainTypeMap::const_iterator terrainTypeIterator =
                mTerrainTypeMap.find(secName);

            if (terrainTypeIterator == mTerrainTypeMap.end())
            {
                Ogre::LogManager::getSingleton().logMessage("can't find the terrain type! TerrainTypeInfos::parseTerrainTypeConfigFile");
                continue;
            }
            
            TerrainType terrainType = terrainTypeIterator->second;

            texNames = cf.getSetting("texture", secName);
            effectNames = cf.getSetting("effect", secName);

            //if (texNames.empty())
            //{
            //    Ogre::LogManager::getSingleton().logMessage("texture names is empty! TerrainTypeInfos::parseTerrainTypeConfigFile");
            ////    continue;
            //}
            //else
            // 如果有纹理名称，就解析（像liquid这种是不需要定义纹理名称的）
            if (false == texNames.empty())
            {
                Ogre::StringVector texNameArray = Ogre::StringUtil::split(texNames);

                for (size_t texNameIndex = 0; texNameIndex < texNameArray.size(); ++texNameIndex)
                {
                    mTextureNameTerrainTypeMap.insert( 
                        TextureNameTerrainTypeMap::value_type(texNameArray[texNameIndex], terrainType) );                
                }
            }
            
            // 解析特效描述
            if (effectNames.empty())
            {
                Ogre::LogManager::getSingleton().logMessage("effect names is empty! TerrainTypeInfos::parseTerrainTypeConfigFile");
                continue;
            }

            EffectTemplateList effectTemplateList;

            Ogre::StringVector effectNameArray = Ogre::StringUtil::split(effectNames);

            for (size_t effectNameIndex = 0; effectNameIndex < effectNameArray.size(); ++effectNameIndex)
            {
                Ogre::String str = effectNameArray[effectNameIndex];
                Ogre::StringVector effectDefine = 
                    Ogre::StringUtil::split(str,":");

                if (effectDefine.size() != 2)
                {
                    Ogre::LogManager::getSingleton().logMessage("the effect define line is wrong! TerrainTypeInfos::parseTerrainTypeConfigFile");
                    continue;
                }

                EffectTemplate effectTemplate;
                effectTemplate.mEffectName = effectDefine[0];
                effectTemplate.mTemplateName = effectDefine[1];

                effectTemplateList.push_back(effectTemplate);
            }

            // 插入这种地形所对应的特效名称
            mTerrainTypeEffectMap.insert( TerrainTypeEffectMap::value_type(terrainType, effectTemplateList) );
        }
    }
    //-----------------------------------------------------------------------
    void TerrainTypeInfos::outputDebugText(const String& filename)
    {
        std::ofstream of;
        of.open(filename.c_str(), std::ios_base::out);

        int xsize = mTerrainData->getXSize();
        int zsize = mTerrainData->getZSize();

        for ( int z=0; z<zsize; ++z )
        {
            for( int x=0; x<xsize; ++x )
            {
                TerrainType terrainType = mTerrainTypeInfos[ z * xsize + x ].mTerrainType;

                switch (terrainType)
                {
                case TerrainType_NORMAL:
                    {
                        of << '*';
                        break;
                    }

                case TerrainType_GRASS:
                    {
                        of << '#';
                        break;
                    }

                case TerrainType_SNOW:
                    {
                        of << '%';
                        break;
                    }

                case TerrainType_SOIL:
                    {
                        of << '+';
                        break;
                    }

                case TerrainType_LIQUID:
                    {
                        of << 'a';
                        break;
                    }
                }
            }

            of << '\n';
        }

        of.close();
    }
    //-----------------------------------------------------------------------
    const String TerrainTypeInfos::_getEffectName(TerrainType type, const String& templateName)
    {
        TerrainTypeEffectMap::const_iterator i = mTerrainTypeEffectMap.find(type);

        if (i != mTerrainTypeEffectMap.end())
        {
            const EffectTemplateList& effectList = i->second;

            EffectTemplateList::const_iterator effectIt = effectList.begin();

            while (effectIt != effectList.end())
            {
                if ( (*effectIt).mTemplateName == templateName )
                    return (*effectIt).mEffectName;

                ++effectIt;
            }            
        }
        //else
        //{
        //    OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
        //        "can't find the effect name by the terrain type '" + 
        //        Ogre::StringConverter::toString(type) + "'",
        //        "TerrainTypeInfos::getEffectName");
        //}

        return "";
    }
    //-----------------------------------------------------------------------
    void TerrainTypeInfos::getEffectInfo(float x, float z, const String& templateName, String& effectName, float& height)
    {
        const TerrainTypeInfo& info = getTerrainTypeInfo(x,z);

        height = info.mExtraData;
        effectName = _getEffectName( info.mTerrainType, templateName );
    }
}