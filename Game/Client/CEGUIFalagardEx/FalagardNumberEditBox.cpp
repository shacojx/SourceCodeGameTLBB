#include "FalagardNumberEditBox.h"
#include "CEGUIPropertyHelper.h"
// Start of CEGUI namespace section
namespace CEGUI
{

	const utf8 FalagardNumberEditBox::WidgetTypeName[] = "Falagard/NumberEditBox";

	FalagardNumberEditBoxProperties::NumberOnly			FalagardNumberEditBox::m_NumberOnly;

	FalagardNumberEditBox::FalagardNumberEditBox(const String& type, const String& name):
		FalagardEditbox(type, name)
	{
		addProperty( &m_NumberOnly );
		m_bNumberOnly = true;
	}

	FalagardNumberEditBox::~FalagardNumberEditBox(void)
	{
	}

	void FalagardNumberEditBox::setNumberOnly( bool bNumberOnly )
	{
		m_bNumberOnly = bNumberOnly;
	}
	const bool FalagardNumberEditBox::isNumberOnly() const
	{
		return m_bNumberOnly;
	}
	void	FalagardNumberEditBox::onCharacter(KeyEventArgs& e)
	{
		// 检查是不是数字
		if( m_bNumberOnly )
		{
			// 如果是数字才能走过去,否则就返回了

			switch( e.scancode )
			{
			case Key::One:
			case Key::Two:
			case Key::Three:    
			case Key::Four:
			case Key::Five:
			case Key::Six:
			case Key::Seven:
			case Key::Eight:
			case Key::Nine:
			case Key::Zero:
			case Key::Numpad1: 
			case Key::Numpad2: 
			case Key::Numpad3: 
			case Key::Numpad4: 
			case Key::Numpad5: 
			case Key::Numpad6: 
			case Key::Numpad7: 
			case Key::Numpad8: 
			case Key::Numpad9: 
			case Key::Numpad0:
				break;
			default:
				return;
			}
		}
		Editbox::onCharacter( e );
	}

	Window* FalagardNumberEditBoxFactory::createWindow(const String& name)
    {
        return new FalagardNumberEditBox(d_type, name);
    }

    void FalagardNumberEditBoxFactory::destroyWindow(Window* window)
    {
		delete window;
    }


	namespace FalagardNumberEditBoxProperties
	{
		String NumberOnly::get(const PropertyReceiver* receiver) const
		{
			return PropertyHelper::boolToString(static_cast<const FalagardNumberEditBox*>(receiver)->isNumberOnly());
		}

		void NumberOnly::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardNumberEditBox*>(receiver)->setNumberOnly( PropertyHelper::stringToBool(value) );
		}

	}
}