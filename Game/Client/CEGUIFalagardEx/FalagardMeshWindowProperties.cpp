
#include "FalagardMeshWindowProperties.h"
#include "FalagardMeshWindow.h"
#include "CEGUIPropertyHelper.h"

// Start of CEGUI namespace section
namespace CEGUI
{

	// Start of FalagardMeshWindowProperties namespace section
	namespace FalagardMeshWindowProperties 
	{
		//////////////////////////////////////////////////////////////////////////

		String FakeObject::get(const PropertyReceiver* receiver) const
		{
			return static_cast<const FalagardMeshWindow*>(receiver)->getFakeObject();
		}

		void FakeObject::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMeshWindow*>(receiver)->setFakeObject(value);
		}

		//////////////////////////////////////////////////////////////////////////

		String ShowControl::get(const PropertyReceiver* receiver) const
		{
			return static_cast<const FalagardMeshWindow*>(receiver)->getShowControl();
		}

		void ShowControl::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardMeshWindow*>(receiver)->setShowControl(value);
		}
	}

}