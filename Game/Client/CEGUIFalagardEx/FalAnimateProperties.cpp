
#include "FalAnimateProperties.h"
#include "falagardAnimate.h"
#include "CEGUIPropertyHelper.h"


namespace CEGUI
{
namespace FalagardAnimateProperties
{

	String Animate::get(const PropertyReceiver* receiver) const
    {
        return "";//PropertyHelper::boolToString( static_cast<const FalagardAnimate*>(receiver)->IsFlash() );
    }

    void Animate::set(PropertyReceiver* receiver, const String& value)
    {
		static_cast<FalagardAnimate*>(receiver)->SetAnimate( value );
    }

}
}
