#include ".\FalagardEditBox.h"
namespace CEGUI
{
	const String FalagardEditBoxEx::EventNamespace("EditBoxEx");

	const utf8 FalagardEditBoxEx::WidgetTypeName[] = "Falagard/EditBoxEx";


	const String FalagardEditBoxEx::EventReturn( (utf8*)"SendMessage" ); // 发送消息
	
	FalagardEditBoxExProperties::TragatString  FalagardEditBoxEx::d_TragetString;
	FalagardEditBoxExProperties::ChatString    FalagardEditBoxEx::d_ChatString;

	FalagardEditBoxEx::FalagardEditBoxEx(const String& type, const String& name) :
		FalagardEditbox(type, name)
	{
		addEvent( EventReturn );
		addProperty( &d_ChatString );
		addProperty( &d_TragetString );
		m_nMessagePointer = 0;
	}

	FalagardEditBoxEx::~FalagardEditBoxEx(void)
	{
	}

	//const String&  FalagardEditBoxEx::GetText() const
	//{
	//	return d_strText;
	//}
	//const String&  FalagardEditBoxEx::GetTraget() const// 得到斜杠后面的字符串
	//{
	//	return d_strTraget;
	//}
	void FalagardEditBoxEx::SetMessage( const String& str )
	{
		TranslateText();
		d_strText = str;
		UpdateCurText();
	}
	void FalagardEditBoxEx::SetTraget( const String& str )
	{
		TranslateText();
		d_strTraget = str;
		UpdateCurText();
	}
	void FalagardEditBoxEx::UpdateCurText()
	{
		String strTmp;
		strTmp = d_strTraget + d_strText;
		setText( strTmp );
		TranslateText();
	}
	void FalagardEditBoxEx::SetCurMessage( int nCurPointer )
	{
		MessageHistory::iterator   it;
		int nIndex = 0;
		for( it = m_listMessage.begin(); it != m_listMessage.end(); it ++, nIndex ++ )
		{
			if( nIndex == nCurPointer )
			{
				setText( (*it) );
				TranslateText();
				return;
			}
		}
		setText( "" );
		
	}
	void FalagardEditBoxEx::SetMessageLast() // 设置成上一次说的话
	{
		m_nMessagePointer --;
		if( m_nMessagePointer < 0 )
			m_nMessagePointer = 0;
		SetCurMessage( m_nMessagePointer );
		
	}
	void FalagardEditBoxEx::SetMessageNext() // 设置成下一次说的话
	{
		m_nMessagePointer ++;
		if( m_nMessagePointer > int( m_listMessage.size() ) )
			m_nMessagePointer = 0;
		SetCurMessage( m_nMessagePointer );
	}
	void FalagardEditBoxEx::TranslateText()
	{
		d_strTraget = "-1";
		d_strText = "";
		const char KeySpace	= ' ';
		String strTmp = getText();
		if( strTmp[ 0 ] == '/' ) // 如果第一个字符是 /, 说明接下来的字符串是目标字符串
		{
			String::size_type	word_start = strTmp.find_first_of(KeySpace, 1 ); 
			if ( word_start == String::npos ) // 如果到最后了
				word_start = strTmp.size() - 1;
			d_strTraget = strTmp.substr( 1, word_start - 1 );
			d_strText = strTmp.substr( word_start + 1, strTmp.size() - word_start );
		}
		else
			d_strText = strTmp;
	}
	void	FalagardEditBoxEx::onKeyDown(KeyEventArgs& e)
	{
		if (hasInputFocus() && !isReadOnly())
		{
			WindowEventArgs args(this);
			switch (e.scancode)
			{
			case Key::Return:
			case Key::NumpadEnter: // 如果是这个消息就截获了。
				TranslateText();
				m_listMessage.push_back( getText() );
				// 纪录当前写的话
				break;
			case Key::ArrowUp:// 上箭头按下
				SetMessageLast();
				break;
			case Key::ArrowDown:// 下箭头
				SetMessageNext();
				break;
			default:
				break;
			}
		}
		FalagardEditbox::onKeyDown( e );
	}

	void FalagardEditBoxEx::onHidden(WindowEventArgs& e)
	{
		FalagardEditbox::onHidden(e);

		releaseInput();
	}
		 //////////////////////////////////////////////////////////////////////////
    Window* FalagardEditBoxExFactory::createWindow(const String& name)
    {
        return new FalagardEditBoxEx(d_type, name);
    }

    void FalagardEditBoxExFactory::destroyWindow(Window* window)
    {
//        if (window->getType() == d_type)
            delete window;
    }




	namespace FalagardEditBoxExProperties
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////
		String TragatString::get(const PropertyReceiver* receiver) const
		{
			return static_cast<const FalagardEditBoxEx*>(receiver)->GetTraget();
		}

		void TragatString::set(PropertyReceiver* receiver, const String& value)
		{
			static_cast<FalagardEditBoxEx*>(receiver)->SetTraget( value );
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////
		String ChatString::get(const PropertyReceiver* receiver) const
		{
			return static_cast<const FalagardEditBoxEx*>(receiver)->GetText();
		}

		void ChatString::set(PropertyReceiver* receiver, const String& value)
		{	
			static_cast< FalagardEditBoxEx*>(receiver)->SetMessage( value );
		}
	}
};