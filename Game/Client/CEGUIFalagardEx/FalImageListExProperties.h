#ifndef _FalImageListBoxPropertiesEx_h_
#define _FalImageListBoxPropertiesEx_h_

#include "CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	// 带图标的list的属性
	// ImageNormal	图标
	// ImageHorver	高亮
	// ImageChecked 选中的
	// ImageDisable 关闭
	// Animate 动画图标
	// AutoScale 是否缩放图标到当前合适的高度

/*!
\brief
    Properties for the Falagard ListHeaderSegment
*/
	namespace FalagardImageListBoxExProperties
	{
		/*!
		\brief
		Property to get/set empty status

		\par Usage:
			- Name: Empty
			- Format: "true|false".

		*/
		
		class SetStateImage : public Property
		{
		public:
			SetStateImage() : Property(
				"SetStateImage",
				"Property to get/set ImageNormal .",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		class NormalImage : public Property
		{
		public:
			NormalImage() : Property(
				"NormalImage",
				"Property to get/set ImageNormal .",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		
		class AddItem : public Property
		{
		public:
			AddItem() : Property(
				"AddItem",
				"Property to get/set Animate name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class SelectImage: public Property
		{
		public:
			SelectImage() : Property(
				"SelectImage",
				"Property to get/set Animate name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class HorverImage: public Property
		{
		public:
			HorverImage() : Property(
				"HorverImage",
				"Property to get/set Animate name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
	}
}

#endif  // end of guard _FalActionButtonProperties_h_
