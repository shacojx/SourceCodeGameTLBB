#pragma once
#include "FalModule.h"
#include "faleditbox.h"

namespace CEGUI
{
	namespace FalagardEditBoxExProperties
	{
		class TragatString : public Property
		{
			public:
			TragatString() : Property(
				"TragetString",
				"Property to get/set empty status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class ChatString : public Property
		{
			public:
			ChatString() : Property(
				"ChatString",
				"Property to get/set empty status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

	}
	class FALAGARDBASE_API FalagardEditBoxEx :	public FalagardEditbox
	{
		typedef std::list< String>   MessageHistory;
		MessageHistory   m_listMessage;
		int				m_nMessagePointer; // 当前记录的话

		void			SetMessageLast(); // 设置成上一次说的话
		void			SetMessageNext(); // 设置成下一次说的话
		void			SetCurMessage( int nIndex );

		String			d_strText;
		String			d_strTraget;

		
	public:
		static const String EventNamespace;
		static const utf8	WidgetTypeName[];
		static const String EventReturn; // 发送消息

		const String&  GetText() const  { return d_strText; };
		const String&  GetTraget() const { return d_strTraget; }; // 得到斜杠后面的字符串
		void			SetMessage( const String& str );
		void			SetTraget( const String& str );
		void			UpdateCurText();
		void     TranslateText();
		
		
		FalagardEditBoxEx(const String& type, const String& name);
		~FalagardEditBoxEx(void);

		static FalagardEditBoxExProperties::ChatString    d_ChatString;
		static FalagardEditBoxExProperties::TragatString  d_TragetString;
	protected:
		virtual void	onKeyDown(KeyEventArgs& e);
		virtual void	onHidden(WindowEventArgs& e);

		
	};

	class FALAGARDBASE_API FalagardEditBoxExFactory : public WindowFactory
    {
    public:
        FalagardEditBoxExFactory(void) : WindowFactory(FalagardEditBoxEx::WidgetTypeName) { }
        ~FalagardEditBoxExFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
};