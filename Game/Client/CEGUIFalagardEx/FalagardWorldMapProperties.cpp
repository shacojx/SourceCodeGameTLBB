#include ".\falagardworldmap.h"
#include ".\falagardworldmapproperties.h"
#include "ceguiimagesetmanager.h"
#include "ceguiimageset.h"
#include "ceguipropertyhelper.h"
#include "windows.h"
namespace CEGUI
{
	namespace FalagardWorldMapProperties
	{
///*-------------------------------------------------------------------------------------------------------------------------------
		String BackgroundName::get(const PropertyReceiver* receiver) const
		{
			return  "";
		}

		void BackgroundName::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardWorldMap*>(receiver)->setBackgroundName( value );
		}
///*-------------------------------------------------------------------------------------------------------------------------------
		String BackgroundSize::get(const PropertyReceiver* receiver) const
		{
			return  "";
		}

		void BackgroundSize::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardWorldMap*>(receiver)->setBackgroundSize( PropertyHelper::stringToSize( value ) );
		}
///*-------------------------------------------------------------------------------------------------------------------------------
		String CityImageNormal::get(const PropertyReceiver* receiver) const
		{
			return  "";
		}

		void CityImageNormal::set(PropertyReceiver* receiver, const String& value)
		{
			char szType[ 128 ];
			char szSet[ 128 ];
			char szImage[ 128 ];
			sprintf( szSet, "" );
			sprintf( szType, "" );
			sprintf( szImage, "" );
			sscanf( value.c_str(), "type:%127s set:%127s image:%127s", szType, szSet, szImage );

			const Image* pImage = &ImagesetManager::getSingleton().getImageset( szSet )->getImage( szImage );
			static_cast<FalagardWorldMap*>(receiver)->setCityNormalImage( szType, pImage );
		}


///*-------------------------------------------------------------------------------------------------------------------------------
		String CityImageHover::get(const PropertyReceiver* receiver) const
		{
			return  "";
		}

		void CityImageHover::set(PropertyReceiver* receiver, const String& value)
		{
			char szType[ 128 ];
			char szSet[ 128 ];
			char szImage[ 128 ];
			sprintf( szSet, "" );
			sprintf( szType, "" );
			sprintf( szImage, "" );
			sscanf( value.c_str(), "type:%127s set:%127s image:%127s", szType, szSet, szImage );
			const Image* pImage = &ImagesetManager::getSingleton().getImageset( szSet )->getImage( szImage );
			static_cast<FalagardWorldMap*>(receiver)->setCityHoverImage( szType, pImage );
		}

		
///*-------------------------------------------------------------------------------------------------------------------------------
	
		String MySelfImage::get(const PropertyReceiver* receiver) const
		{
			return  "";
		}

		void MySelfImage::set(PropertyReceiver* receiver, const String& value)
		{
			const Image* pImage = PropertyHelper::stringToImage( value );
			static_cast<FalagardWorldMap*>(receiver)->setMySelfImage( pImage );
		};
///*-------------------------------------------------------------------------------------------------------------------------------
	
		String FriendImage1::get(const PropertyReceiver* receiver) const
		{
			return  "";
		}

		void FriendImage1::set(PropertyReceiver* receiver, const String& value)
		{
			const Image* pImage = PropertyHelper::stringToImage( value );
			static_cast<FalagardWorldMap*>(receiver)->setFriendImage( 0, pImage );
		};
///*-------------------------------------------------------------------------------------------------------------------------------
		// 二个队友得图标
		String FriendImage2::get(const PropertyReceiver* receiver) const
		{
			return  "";
		}

		void FriendImage2::set(PropertyReceiver* receiver, const String& value)
		{
			const Image* pImage = PropertyHelper::stringToImage( value );
			static_cast<FalagardWorldMap*>(receiver)->setFriendImage( 1, pImage );

		};
///*-------------------------------------------------------------------------------------------------------------------------------
		// 三个队友得图标
		String FriendImage3::get(const PropertyReceiver* receiver) const
		{
			return  "";
		}

		void FriendImage3::set(PropertyReceiver* receiver, const String& value)
		{
			const Image* pImage = PropertyHelper::stringToImage( value );
			static_cast<FalagardWorldMap*>(receiver)->setFriendImage( 2, pImage );

		};
///*-------------------------------------------------------------------------------------------------------------------------------
		// 四个队友得图标
		String FriendImage4::get(const PropertyReceiver* receiver) const
		{
			return  "";
		}

		void FriendImage4::set(PropertyReceiver* receiver, const String& value)
		{
			const Image* pImage = PropertyHelper::stringToImage( value );
			static_cast<FalagardWorldMap*>(receiver)->setFriendImage( 3, pImage );
		};
///*-------------------------------------------------------------------------------------------------------------------------------
		// 五个队友得图标
		String FriendImage5::get(const PropertyReceiver* receiver) const
		{
			return  "";
		}

		void FriendImage5::set(PropertyReceiver* receiver, const String& value)
		{
			const Image* pImage = PropertyHelper::stringToImage( value );
			static_cast<FalagardWorldMap*>(receiver)->setFriendImage( 4, pImage );
		};
///*-------------------------------------------------------------------------------------------------------------------------------
	};
}