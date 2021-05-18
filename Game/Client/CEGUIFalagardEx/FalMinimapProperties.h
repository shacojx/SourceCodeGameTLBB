#ifndef _FalMinimapProperties_h_
#define _FalMinimapProperties_h_

#include "CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Properties for the Falagard ListHeaderSegment
*/
	namespace FalagardMinimapProperties
	{
	/*
	\brief
	Property to get/set Minimap name status

	\par Usage:
		- Name: Empty
		- Format: "aniamte name".

	*/
		
		class SetMinimapName: public Property
		{
		public:
			SetMinimapName() : Property(
				"SetMinimapName",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		class MinimapImageSet: public Property
		{
		public:
			MinimapImageSet() : Property(
				"MinimapImageSet",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		class RenderSize: public Property
		{
			public:
			RenderSize() : Property(
				"RenderSize",
				"Property to get/set Minimap name status.",
				"w=0 h=0")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		
		// 敌人npc的图标指定
		class AnimyImage: public Property
		{
			public:
			AnimyImage() : Property(
				"AnimyImage",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		// 特殊npc的图标指定
		class ExpNpcImage: public Property
		{
			public:
			ExpNpcImage() : Property(
				"ExpNpcImage",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		// 队友的图标
		class TeammateImage: public Property
		{
			public:
			TeammateImage() : Property(
				"TeammateImage",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		
		// Obj的图标
		class ObjImage: public Property
		{
			public:
			ObjImage() : Property(
				"ObjImage",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		// 自己的图标,这个是动画
		class MySelfImage: public Property
		{
			public:
			MySelfImage() : Property(
				"MySelfImage",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		// 玩家的图标
		class PlayerImage: public Property
		{
			public:
			PlayerImage() : Property(
				"PlayerImage",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		// 开启显示种类
		class OpenFlagType: public Property
		{
			public:
			OpenFlagType() : Property(
				"OpenFlagType",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		// 开启显示种类
		class CloseFlagType: public Property
		{
			public:
			CloseFlagType() : Property(
				"CloseFlagType",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		// 开启显示种类
		class IsFlagOpen: public Property
		{
			public:
			IsFlagOpen() : Property(
				"IsFlagOpen",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class ClearFlagList: public Property
		{
			public:
			ClearFlagList() : Property(
				"ClearFlagList",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class SceneSize: public Property
		{
			public:
			SceneSize() : Property(
				"SceneSize",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		
		class ScenePosImage: public Property
		{
			public:
			ScenePosImage() : Property(
				"ScenePosImage",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		class GetCurCoordinate: public Property
		{
			public:
			GetCurCoordinate() : Property(
				"GetCurCoordinate",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		class ActiveImage: public Property
		{
			public:
			ActiveImage() : Property(
				"ActiveImage",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class PetImage: public Property
		{
			public:
			PetImage() : Property(
				"PetImage",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class FlashImage: public Property
		{
			public:
			FlashImage() : Property(
				"FlashImage",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
	};
};

#endif  // end of guard _FalMinimapProperties_h_
