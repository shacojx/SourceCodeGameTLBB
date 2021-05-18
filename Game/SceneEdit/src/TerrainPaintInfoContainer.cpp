#include "TerrainPaintInfoContainer.h"

#include <OgreStringConverter.h>
#include <OgreException.h>

namespace WX {

    TerrainPaintInfoContainer::TerrainPaintInfoContainer()
    {
        mStringTexTypeMap["内部纹理"]       = InnerTex;
        mStringTexTypeMap["内角"] = InnerCornerTex;
        mStringTexTypeMap["外角"] = OuterCornerTex;
        mStringTexTypeMap["边缘"]        = EdgeTex;	
        mStringTexTypeMap["对角"]     = DuijiaoTex;

		mStringExtraTexTypeMap["左上角"]        = LT;
		mStringExtraTexTypeMap["右上角"]		= RT;
		mStringExtraTexTypeMap["左下角"]		= LB;
		mStringExtraTexTypeMap["右下角"]        = RB;	
		mStringExtraTexTypeMap["左边缘"]        = LE;
		mStringExtraTexTypeMap["右边缘"]        = RE;
		mStringExtraTexTypeMap["上边缘"]        = TE;
		mStringExtraTexTypeMap["下边缘"]        = BE;
		mStringExtraTexTypeMap["普通"]			= EOther;
    }

    TerrainPaintInfoContainer::~TerrainPaintInfoContainer()
    {
    }

    bool TerrainPaintInfoContainer::isTextureInfoOk( const TextureInfo &info )
    {
        return (info.brushName.empty() == false) &&
            (info.height > 0) &&
            (info.width > 0) &&
            (info.leftCorner >= 0) &&
            (info.topCorner >= 0) &&
            (info.ownerTextureName.empty() == false);
    }

    void TerrainPaintInfoContainer::addTextureInfo( TextureInfo &info )
    {
        assert ( isTextureInfoOk(info) );

        String brushName = info.brushName;

        String pathName = info.ownerTextureName;

		addTexNameAndBrushMapInfo(pathName,brushName);

        size_t pos = pathName.find_last_of('/');

        if (pos != String::npos)
        {
            pathName.erase(pos+1);
            pathName.append(brushName);

            brushName = pathName;
        }

        String texTypeStr;

        // 找到该纹理的类型字符串
        StringTexTypeMap::iterator itForStringTexTypeMap = mStringTexTypeMap.begin();
        while ( itForStringTexTypeMap != mStringTexTypeMap.end() )
        {
            if (itForStringTexTypeMap->second == info.textureType)
            {
                texTypeStr = itForStringTexTypeMap->first;
                break;
            }
            ++itForStringTexTypeMap;
        }

        assert (itForStringTexTypeMap != mStringTexTypeMap.end());

        TextureInfoMap::iterator it = mTextureInfoMap.find(brushName);

        // 如果当前map中没有这个brush，说明是一组新的画刷
        if ( it == mTextureInfoMap.end() )
        {
            TextureInfos newInfos;            

            // 自动生成纹理名称，名称格式为 “组名|当前组的纹理序号|纹理类型”
            String textureName = info.brushName + "|"
                + Ogre::StringConverter::toString(newInfos.size()) + "|" + texTypeStr;

            info.textureName = textureName;

            newInfos.push_back(info);

            std::pair<TextureInfoMap::iterator, bool> inserted = 
                mTextureInfoMap.insert( TextureInfoMap::value_type(brushName, newInfos) );

            assert (inserted.second);
        }
        else
        {
            // 自动生成纹理名称
            String textureName = info.brushName + "|"
                + Ogre::StringConverter::toString(it->second.size()) + "|" + texTypeStr;

            info.textureName = textureName;

            it->second.push_back(info);
        }
    }

	void TerrainPaintInfoContainer::addTexNameAndBrushMapInfo( const String &texName, const String &brushName )
	{
		// 先寻找这个信息是否添加过了
		TexNameAndBrushMap::const_iterator i = mTexNameAndBrushMap.find(texName);

		if ( i == mTexNameAndBrushMap.end() )
		{
			std::pair<TexNameAndBrushMap::iterator, bool> inserted = 
				mTexNameAndBrushMap.insert( TexNameAndBrushMap::value_type(texName, brushName) );
			assert (inserted.second);
		}
	}

	const String &TerrainPaintInfoContainer::findBrushNameByTexName(const String &texName) const
	{
		TexNameAndBrushMap::const_iterator i = mTexNameAndBrushMap.find(texName);

		if (i == mTexNameAndBrushMap.end())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "Can't found brush texture with name '" + texName + "'",
                "TerrainPaintInfoContainer::findBrushNameByTexName");
        }

        return i->second;		
	}

	const TextureInfo &TerrainPaintInfoContainer::findTextureInfoByTexName( const String &texName )
	{
		String brushName = findBrushNameByTexName(texName);

		size_t pos = texName.find_last_of('/');

		// 组成完整的画刷名称
		if (pos != String::npos)
		{
			brushName = texName.substr(0,pos+1) + brushName;
		}

		TextureInfoMap::iterator it = mTextureInfoMap.begin();

		while ( it != mTextureInfoMap.end() )
		{
			if ( it->first == brushName )
			{
				break;
			}
			++it;
		}

		if ( it != mTextureInfoMap.end() )
		{
			TextureInfos &infos = it->second;            

			for ( size_t count = 0; count < infos.size(); ++count )
			{
				if (infos[count].ownerTextureName == texName)
				{
					return infos[count];
				}
			}
		}

		OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
			"Can't found texture with name '" + texName + "'",
			"TerrainPaintInfoContainer::findTextureInfoByTexName");
	}

}