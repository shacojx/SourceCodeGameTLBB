#include ".\falagardchateditbox.h"
#include "CEGUIScrollbar.h"
// Start of CEGUI namespace section
namespace CEGUI
{

	const utf8 FalagardChatEditBox::WidgetTypeName[] = "Falagard/ChatEditBox";

	FalagardChatProperties::MaxLineNumber			FalagardChatEditBox::m_MaxLineNumber;
	FalagardChatProperties::AddText					FalagardChatEditBox::m_AddText;
	FalagardChatProperties::TextClear				FalagardChatEditBox::m_Clear;
	FalagardChatProperties::LineUp					FalagardChatEditBox::m_LineUp;
	FalagardChatProperties::LineDown				FalagardChatEditBox::m_LineDown;

	FalagardChatEditBox::FalagardChatEditBox(const String& type, const String& name):
		FalagardStaticText(type, name)
	{
		addProperty( &m_MaxLineNumber );
		addProperty( &m_AddText );
		addProperty( &m_Clear );
		addProperty( &m_LineUp );
		addProperty( &m_LineDown );
	}

	FalagardChatEditBox::~FalagardChatEditBox(void)
	{
	}

	void FalagardChatEditBox::initialise(void)
	{
		FalagardStaticText::initialise();
	}

	void FalagardChatEditBox::setMaxLineNumber( int nMaxLine )
	{
		m_nMaxLine = nMaxLine;
	}
	const int	 FalagardChatEditBox::getMaxLineNumber() const
	{
		return m_nMaxLine;
	}

	void FalagardChatEditBox::AddNewText( const String& text )
	{
		if( text.size() == 0 )
			return;
		d_text += "\n";
		d_text += text;
		configureScrollbars();
		requestRedraw();
	}
	void FalagardChatEditBox::updateSelf( float elapsed )
	{
		requestRedraw();
	}
	void FalagardChatEditBox::Clear()
	{
		d_text = "";
		requestRedraw();
	}

	void FalagardChatEditBox::LineDown()
	{
	}

	void FalagardChatEditBox::LineUp()
	{
	}
	void FalagardChatEditBox::PageDown()
	{
	}
	void FalagardChatEditBox::PageUp()
	{
	}
	// 滚动到指定位置
	void FalagardChatEditBox::ScrollTo( float fPos )
	{
		if( d_vertScrollbar )
			d_vertScrollbar->setScrollPosition( fPos );
	}


	Window* FalagardChatEditBoxFactory::createWindow(const String& name)
    {
        return new FalagardChatEditBox(d_type, name);
    }

    void FalagardChatEditBoxFactory::destroyWindow(Window* window)
    {
		delete window;
    }
}