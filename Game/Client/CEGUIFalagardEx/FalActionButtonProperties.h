#ifndef _FalActionButtonProperties_h_
#define _FalActionButtonProperties_h_

#include "CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	/*!
	\brief
		Properties for the Falagard ActionButton
	*/
	namespace FalagardActionButtonProperties
	{
		/*!
		\brief
		Property to get/set empty status

		\par Usage:
			- Name: Empty
			- Format: "true|false".

		*/
		class Empty : public Property
		{
		public:
			Empty() : Property(
				"Empty",
				"Property to get/set empty status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		/*!
		\brief
		Property to get/set empty status

		\par Usage:
			- Name: Empty
			- Format: "true|false".

		*/
		class Checked : public Property
		{
		public:
			Checked() : Property(
				"Checked",
				"Property to get/set checked status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		/*!
		\brief
		Property to get/set default status

		\par Usage:
			- Name: Default
			- Format: "true|false".

		*/
		class Default : public Property
		{
		public:
			Default() : Property(
				"Default",
				"",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		/*!
		\brief
		Property to get/set empty status

		\par Usage:
			- Name: Empty
			- Format: "true|false".

		*/
		class Gloom : public Property
		{
		public:
			Gloom() : Property(
				"Gloom",
				"Property to get/set Gloom status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		class CornerChar : public Property
		{
		public:
			CornerChar() : Property(
				"CornerChar",
				"Property to get/set Corner Character.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		/*!
		\brief
			Property to access the state of the dragging enabled setting.

			\par Usage:
				- Name: DraggingEnabled
				- Format: "[text]".

			\par Where [Text] is:
				- "True" to indicate that dragging is enabled.
				- "False" to indicate that dragging is disabled.
		*/
		class DraggingEnabled : public Property
		{
		public:
			DraggingEnabled() : Property(
				"DraggingEnabled",
				"Property to get/set the state of the dragging enabled setting for the DragContainer.  Value is either \"True\" or \"False\".",
				"True")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);
		};

		/*!
		\brief
			Property to access the dragging threshold value.

			\par Usage:
				- Name: DragThreshold
				- Format: "[float]".

			\par Where:
				- [float] represents the movement threshold (in pixels) which must be exceeded to commence dragging.
		*/
		class DragThreshold : public Property
		{
		public:
			DragThreshold() : Property(
				"DragThreshold",
				"Property to get/set the dragging threshold value.  Value is a float.",
				"8.000000")
			{}

			String get(const PropertyReceiver* receiver) const;
			void set(PropertyReceiver* receiver, const String& value);
		};

		class DragAcceptName : public Property
		{
		public:
			DragAcceptName() : Property(
				"DragAcceptName",
				"Property to get/set the drag accept name.  Value is a String.",
				"")
			{}

			String get(const PropertyReceiver* receiver) const;
			void set(PropertyReceiver* receiver, const String& value);
		};
	}
}

#endif  // end of guard _FalActionButtonProperties_h_
