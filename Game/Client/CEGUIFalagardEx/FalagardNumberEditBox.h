#pragma once
#include "FalEditbox.h"

// 聊天信息窗口
// Start of CEGUI namespace section
namespace CEGUI
{
	namespace FalagardNumberEditBoxProperties
	{
		class NumberOnly : public Property
		{
		public:
			NumberOnly() : Property(
				"NumberOnly",
				"Property to get/set empty status.",
				"True")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
	}
	class FALAGARDBASE_API FalagardNumberEditBox :	public FalagardEditbox
	{
		bool		m_bNumberOnly;   // 作多记录多少行数据
	public:
		static const utf8   WidgetTypeName[];       //!< type name for this widget.
	public:
		void setNumberOnly( bool bTrue );
		const bool isNumberOnly() const;

		virtual void	onCharacter(KeyEventArgs& e);
		FalagardNumberEditBox(const String& type, const String& name);
		~FalagardNumberEditBox(void);

		static FalagardNumberEditBoxProperties::NumberOnly			m_NumberOnly;
	};

	class FALAGARDBASE_API FalagardNumberEditBoxFactory : public WindowFactory
    {
    public:
        FalagardNumberEditBoxFactory(void) : WindowFactory(FalagardNumberEditBox::WidgetTypeName) { }
        ~FalagardNumberEditBoxFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
};