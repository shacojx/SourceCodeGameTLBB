#pragma once
#include "FalStatictext.h"
#include "falchatproperties.h"

// 聊天信息窗口
// Start of CEGUI namespace section
namespace CEGUI
{
	class FALAGARDBASE_API FalagardChatEditBox :	public FalagardStaticText
	{
		int		m_nMaxLine;   // 作多记录多少行数据
	public:
		static const utf8   WidgetTypeName[];       //!< type name for this widget.
	public:
		FalagardChatEditBox(const String& type, const String& name);
		~FalagardChatEditBox(void);

		void setMaxLineNumber( int nMaxLine );
		const int	 getMaxLineNumber() const;
		virtual void	initialise(void);

		void			LineDown();
		void			LineUp();
		void			PageDown();
		void			PageUp();
		void			ScrollTo( float fPos );

		void AddNewText( const String& text );
		void Clear();
		static FalagardChatProperties::MaxLineNumber			m_MaxLineNumber;
		static FalagardChatProperties::AddText					m_AddText;
		static FalagardChatProperties::TextClear				m_Clear;
		static FalagardChatProperties::LineUp					m_LineUp;
		static FalagardChatProperties::LineDown					m_LineDown;

		virtual void updateSelf( float elapsed );
	};

	class FALAGARDBASE_API FalagardChatEditBoxFactory : public WindowFactory
    {
    public:
        FalagardChatEditBoxFactory(void) : WindowFactory(FalagardChatEditBox::WidgetTypeName) { }
        ~FalagardChatEditBoxFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
};