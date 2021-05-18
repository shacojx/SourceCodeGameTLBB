#include "FalMultiIMEEditBoxProperties.h"
#include "FalMultiIMEEditBox.h"
#include "CEGUIPropertyHelper.h"
#include "CEGUIIme.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	// Start of FalagardMultiIMEEditBoxProperties namespace section
	namespace FalagardMultiIMEEditBoxProperties
	{
		////////////////////////////////////////////////////////////////////////

		String	DefaultEditBox::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::boolToString(Ime::GetActiveIMEEditBox() == static_cast<const Window*>(receiver));
		}

		void DefaultEditBox::set(PropertyReceiver* receiver, const String& value)
		{
			Window* old = static_cast<Window*>(Ime::GetActiveIMEEditBox());

			Ime::SetActiveIMEEditBox(static_cast<Window*>(receiver), PropertyHelper::stringToBool(value));

			if(Ime::GetActiveIMEEditBox() == receiver)
			{
				static_cast<FalagardMultiIMEEditBox*>(Ime::GetActiveIMEEditBox())->resetCaretBlink();
			}
			if(old)
			{
				old->requestRedraw();
			}
		}

	}
}