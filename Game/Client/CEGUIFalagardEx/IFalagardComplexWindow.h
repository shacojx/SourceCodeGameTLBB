
namespace CEGUI
{
	class IFalagardComplexWindow
	{
	public:
		enum ChildType
		{
			CT_Text,
			CT_Option,
			CT_Item,
			CT_Action,
			CT_Money,
			CT_HyperLink,
		};

		//Add ChildElement (Text)
		virtual void			AddChildElement_Text(String strText, FontBase* font, TextFormatting typeset=WordWrapLeftAligned) = 0;
		
		//Add ChildElement (Option)
		virtual void			AddChildElement_Option(String strText,String strText2,String strText3, String strText4, FontBase* font=NULL) = 0;
		
		//Add ChildElement (Link)
		virtual void			AddChildElement_HyperLink( String strText, int nLinkID, bool bReArrange, FontBase* font=NULL) = 0;

		//Add ChildElement (Item)
		virtual void			AddChildElement_Item(String strIconImage, String strText, bool bRadio,bool bReArrange, FontBase* font=NULL) = 0;

		//Add ChildElement (Action)
		virtual Window*			AddChildElement_Action(String strIconImage, int ActionID, bool bRadio, bool bReArrange, FontBase* font=NULL) = 0;

		//Add ChildElement (Money)
		virtual void			AddChildElement_Money(int money, FontBase* font=NULL) = 0;
		//Clear
		virtual void			ClearAllElement(void) = 0;

		virtual void			PageEnd() = 0;
	};
}