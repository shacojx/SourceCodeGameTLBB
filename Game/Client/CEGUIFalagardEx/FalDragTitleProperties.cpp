
#include "FalDragTitleProperties.h"
#include "FalDragTitle.h"
#include "CEGUIPropertyHelper.h"


// Start of CEGUI namespace section
namespace CEGUI
{

	// Start of DragTitleProperties namespace section
	namespace FalagardDragTitleProperties
	{
		
		///////////////////////////////////////////////////////////////////

		String DragTarget::get(const PropertyReceiver* receiver) const
		{
			return static_cast<const FalagardDragTitle*>(receiver)->getDragTarget();
		}

		void DragTarget::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardDragTitle*>(receiver)->setDragTarget(value);
		}

	}
}