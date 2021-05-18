#include "TerrainPaintXMLHandler.h"

#include "Core/WXXMLAttributes.h"

#include <OgreException.h>
#include <OgreStringConverter.h>

namespace WX {

    TerrainPaintXMLHandler::TerrainPaintXMLHandler(TerrainPaintInfoContainer *container) : 
    mPaintInfoContainer(container), 
    mErrorInfoString(""),
    mCurrentElementWrong(false)
    {
        mCurrentTextureInfo.clear();       
    }

    /** Destructor for SceneXMLHandler objects */
    TerrainPaintXMLHandler::~TerrainPaintXMLHandler(void)
    {
    }

    /*************************************************************************
    SAX2 Handler overrides
    *************************************************************************/ 

    /** Overridden from XMLHandler */
    void TerrainPaintXMLHandler::characters(const String& chars)
    {
    }

    /** Overridden from XMLHandler */
    void TerrainPaintXMLHandler::startElement(const String& element, const XMLAttributes& attributes)
    {
        if (element == "贴图")
        {
            mCurrentTextureInfo.clear();
        }
        else if (element == "所属贴图名称")
        {
            String attValue = attributes.getValue("名称");
            
            if ( attValue.empty() )
            {
                _pushErrorInfoString(element + " " + attValue +
                    " the value can't be empty!");
                mCurrentElementWrong = true;
                return;
            }

            mCurrentTextureInfo.ownerTextureName = attValue;
        }
        else if (element == "贴图大小")
        {
            String attValue = attributes.getValue("大小");

            if ( attValue.empty() )
            {
                _pushErrorInfoString(element + " " + attValue +
                    " the value can't be empty!");
                mCurrentElementWrong = true;
                return;
            }

            Ogre::StringVector valueVector = Ogre::StringUtil::split(attValue,",");

            if ( valueVector.size() != 2 )
            {
                _pushErrorInfoString(element + " " + attValue +
                    " size of parameter must be two!");
                mCurrentElementWrong = true;
                return;
            }

            mCurrentTextureInfo.width = Ogre::StringConverter::parseUnsignedInt(valueVector[0]);
            mCurrentTextureInfo.height = Ogre::StringConverter::parseUnsignedInt(valueVector[1]);
        }
        else if (element == "贴图坐标")
        {
            String attValue = attributes.getValue("坐标");
            
            if ( attValue.empty() )
            {
                _pushErrorInfoString(element + " " + attValue +
                    " the value can't be empty!");
                mCurrentElementWrong = true;
                return;
            }

            Ogre::StringVector valueVector = Ogre::StringUtil::split(attValue,",");

            if ( valueVector.size() != 2 )
            {
                _pushErrorInfoString(element + " " + attValue +
                    " size of parameter must be two!");
                mCurrentElementWrong = true;
                return;
            }

            mCurrentTextureInfo.leftCorner = Ogre::StringConverter::parseUnsignedInt(valueVector[0]);
            mCurrentTextureInfo.topCorner = Ogre::StringConverter::parseUnsignedInt(valueVector[1]);
        }
        else if (element == "画刷名称")
        {
            String attValue = attributes.getValue("名称");

            if ( attValue.empty() )
            {
                _pushErrorInfoString(element + " " + attValue +
                    " the value can't be empty!");
                mCurrentElementWrong = true;
                return;
            }

            mCurrentTextureInfo.brushName = attValue;
        }
        else if (element == "贴图类型")
        {
            String attValue = attributes.getValue("类型");

            if ( attValue.empty() )
            {
                _pushErrorInfoString(element + " " + attValue +
                    " the value can't be empty!");
                mCurrentElementWrong = true;
                return;
            }

            TerrainPaintInfoContainer::StringTexTypeMap::const_iterator it = mPaintInfoContainer->mStringTexTypeMap.find(attValue);

            if  ( it == mPaintInfoContainer->mStringTexTypeMap.end() )
            {
                _pushErrorInfoString(element + " " + attValue + " can't find the texture type!");
                mCurrentElementWrong = true;
                return;
            }

            // 从对应表中查找出相应的类型字符串
            mCurrentTextureInfo.textureType = it->second;

			attValue = attributes.getValue("额外的类型信息");

            if ( attValue.empty() )
            {
                _pushErrorInfoString(element + " " + attValue +
                    " the value can't be empty!");
                mCurrentElementWrong = true;
                return;
            }

			TerrainPaintInfoContainer::StringExtraTexTypeMap::const_iterator itt = mPaintInfoContainer->mStringExtraTexTypeMap.find(attValue);

			if ( itt == mPaintInfoContainer->mStringExtraTexTypeMap.end() )
            {
                _pushErrorInfoString(element + " " + attValue + " can't find the extra texture info type!");
                mCurrentElementWrong = true;
                return;
            }

			// 从对应表中查找出相应的类型字符串
			mCurrentTextureInfo.extraTexType = itt->second;
        }
		else if (element == "旋转方向")
        {
            String attValue = attributes.getValue("方向类型");

            if ( attValue.empty() )
            {
                _pushErrorInfoString(element + " " + attValue +
                    " the value can't be empty!");
                mCurrentElementWrong = true;
                return;
            }

			mCurrentTextureInfo.rotateType = Ogre::StringConverter::parseUnsignedInt(attValue);

			// 在拼接的大贴图中方向只能有4种选择
			if ( false == (mCurrentTextureInfo.rotateType >= 0 && mCurrentTextureInfo.rotateType <= 3) )
            {
                _pushErrorInfoString(element + " " + attValue + 
                    " rotate type must be between 0 and 3!");
                mCurrentElementWrong = true;
                return;
            }
		}
        else if (element == "贴图信息")
        {
        }
        // anything else is an error which *should* have already been caught by XML validation
        else
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                "Unexpected data was found while parsing the scene file: '" + element + "' is unknown.",
                "SceneXMLHandler::startElement");
        }
    }

    /** Overridden from XMLHandler */
    void TerrainPaintXMLHandler::endElement(const String& element)
    { 
        if (element == "贴图")
        {
            // 如果在解析上一组贴图信息时发生了错误，就不添加这个texture info
            if (mCurrentElementWrong)
            {
                // 把错误的贴图信息记录下来
                _pushErrorInfoString( _getCurrentTextureInfo() );
                mCurrentElementWrong = false;
            }
            else
                // 在每个texture解析完后添加到map中
                mPaintInfoContainer->addTextureInfo(mCurrentTextureInfo);
        }
    }

    void TerrainPaintXMLHandler::_pushErrorInfoString(const String& errorInfo)
    {
        mErrorInfoString += "wrong element : " + errorInfo;
        mErrorInfoString += "\n";
    }

    const String TerrainPaintXMLHandler::_getCurrentTextureInfo(void) const
    {       
        String textureInfoStr;

        textureInfoStr += "\n";
        textureInfoStr += "Owner Texture Name : " + mCurrentTextureInfo.ownerTextureName + "\n";
        textureInfoStr += "Brush Name : " + mCurrentTextureInfo.brushName + "\n";
        return textureInfoStr;
    }

}