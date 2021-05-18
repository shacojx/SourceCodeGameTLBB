
#include "FalMinimapProperties.h"
#include "falagardMinimap.h"
#include "CEGUIPropertyHelper.h"
#include <stdio.h>


namespace CEGUI
{
	namespace FalagardMinimapProperties
	{

		String RenderSize::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::sizeToString( static_cast<const FalagardMiniMap*>(receiver)->getMinimapRenderSize() );
		}

		void RenderSize::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setMinimapRenderSize( PropertyHelper::stringToSize(value) );
		}

		
		String MinimapImageSet::get(const PropertyReceiver* receiver) const
		{
			return  static_cast<const FalagardMiniMap*>(receiver)->getImageset();
		}

		void MinimapImageSet::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setImageset( value );
		}

		String SetMinimapName::get(const PropertyReceiver* receiver) const
		{
			return  static_cast<const FalagardMiniMap*>(receiver)->getImageset();
		}

		void SetMinimapName::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setImagesetByFileName( value );
		}



		// 敌人npc的图标指定
		String AnimyImage::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::imageToString(static_cast<const FalagardMiniMap*>(receiver)->getAnimyImage());
		}

		void AnimyImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setAnimyImage(PropertyHelper::stringToImage(value));
		}

		// // 特殊npc的图标指定
		String ExpNpcImage::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::imageToString(static_cast<const FalagardMiniMap*>(receiver)->getExpNpcImage());
		}

		void ExpNpcImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setExpNpcImage(PropertyHelper::stringToImage(value));
		}
		// 队友的图标
		String TeammateImage::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::imageToString(static_cast<const FalagardMiniMap*>(receiver)->getTeammateImage());
		}

		void TeammateImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setTeammateImage(PropertyHelper::stringToImage(value));
		}
		// 玩家的图标
		String PlayerImage::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::imageToString(static_cast<const FalagardMiniMap*>(receiver)->getPlayerImage());
		}

		void PlayerImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setPlayerImage(PropertyHelper::stringToImage(value));
		}
		// Obj的图标
		String ObjImage::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::imageToString(static_cast<const FalagardMiniMap*>(receiver)->getObjImage());
		}

		void ObjImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setObjImage(PropertyHelper::stringToImage(value));
		}
		// 自己的图标,这个是动画
		String MySelfImage::get(const PropertyReceiver* receiver) const
		{
			return static_cast<const FalagardMiniMap*>(receiver)->getMyselfImage();
		}

		void MySelfImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setMyselfImage( value );
		}

			// 自己的图标,这个是动画
		String OpenFlagType::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void OpenFlagType::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setEnabel( PropertyHelper::stringToInt( value ) );
		}

					// 自己的图标,这个是动画
		String CloseFlagType::get(const PropertyReceiver* receiver) const
		{
			return "";
		}

		void CloseFlagType::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setDisable( PropertyHelper::stringToInt( value ) );
		}
		// 是否开启了一中类型
		String IsFlagOpen::get(const PropertyReceiver* receiver) const
		{
			return  static_cast< const FalagardMiniMap*>(receiver)->getEnableType();
		}

		void IsFlagOpen::set(PropertyReceiver* receiver, const String& value)
		{
			
		}
		// 是否开启了一中类型
		String ClearFlagList::get(const PropertyReceiver* receiver) const
		{
			return  "";//static_cast< const FalagardMiniMap*>(receiver)->getEnableType();
		}

		void ClearFlagList::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->clearFlagList();
		}






					// 是否开启了一中类型
		String SceneSize::get(const PropertyReceiver* receiver) const
		{
			return  "";//static_cast< const FalagardMiniMap*>(receiver)->getEnableType();
		}

		void SceneSize::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->SetSceneSize( PropertyHelper::stringToSize( value ) );
		}


					// 是否开启了一中类型
		String ScenePosImage::get(const PropertyReceiver* receiver) const
		{
			return  "";//static_cast< const FalagardMiniMap*>(receiver)->getEnableType();
		}

		void ScenePosImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setScenePosImage( PropertyHelper::stringToImage( value ) );
		}


					// 是否开启了一中类型
		String GetCurCoordinate::get(const PropertyReceiver* receiver) const
		{
			return  PropertyHelper::pointToString( ( static_cast< const FalagardMiniMap*>(receiver) )->GetSceneCoordinate() );
		}

		void GetCurCoordinate::set(PropertyReceiver* receiver, const String& value)
		{
		}

					// 是否开启了一中类型
		String ActiveImage::get(const PropertyReceiver* receiver) const
		{
			return  "";//static_cast< const FalagardMiniMap*>(receiver)->getEnableType();
		}

		void ActiveImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setActiveImage( value );
		}

					// 是否开启了一中类型
		String PetImage::get(const PropertyReceiver* receiver) const
		{
			return  "";//static_cast< const FalagardMiniMap*>(receiver)->getEnableType();
		}

		void PetImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setPetmage( PropertyHelper::stringToImage( value ) );
		}

		String FlashImage::get(const PropertyReceiver* receiver) const
		{
			return  "";//static_cast< const FalagardMiniMap*>(receiver)->getEnableType();
		}

		void FlashImage::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMiniMap*>(receiver)->setFlashImage( value );
		}
	};
}
