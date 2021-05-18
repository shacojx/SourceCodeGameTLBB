
namespace CEGUI
{

	class IFalagardChatHistory
	{
	public:
		/*!
		\brief
			Insert a chat string to chat board list.
		*/
		virtual void			insertChatString(const String& strChat) = 0;

		/*!
		\brief
			Remove all chat boards.
		*/
		virtual void			removeAllChatStrings() = 0;
		/*!
		\brief
			Scroll to ParentWindows's end
		*/
		virtual void			scrollToEnd() = 0;
		/*!
		\brief
		Extend a Clear Region in history window.
		*/
		virtual void			extendClearRegion() = 0;
	};

}