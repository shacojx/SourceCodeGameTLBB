
namespace CEGUI
{
	class FalagardActionButton;
	class FalagardSuperTooltip;

	class IFalagardActionButton
	{
	public:
		/*!
		\brief
			Subscribes DragdropStarted Event.
		*/
		virtual void			subscribeDragDropStartedEvent(Event::Subscriber subscriber) = 0;
		/*!
		\brief
			Subscribes MouseEnter Event.
		*/
		virtual void			subscribeMouseEnterEvent(Event::Subscriber subscriber) = 0;
		/*!
		\brief
			Subscribes MouseLeave Event.
		*/
		virtual void			subscribeMouseLeaveEvent(Event::Subscriber subscriber) = 0;

		/*!
		\brief
			Set the first lay animate
		*/
		virtual void			setAnimate(const String& name, int totalTime, float percent) = 0;
		virtual void			cleanAnimate(void) = 0;

		/*!
		\brief
			Notify the action button in draging that the draggging is done!.
		*/
		virtual void			notifyDragingEnd(void) = 0;

		/*!
		\brief
			Notify a action button drag drop in.
		*/
		virtual void			notifyDragDrop_Droped(FalagardActionButton* pBtn) = 0;

		/*!
		\brief
			Set/Get Logic data (ActionItem)
		*/
		virtual void			setLogicItemData(void* pData) = 0;
		virtual void*			getLogicItemData(void) const = 0;

		/*!
		\brief
			Get Logic Name
		*/
		virtual const char*		getDragAcceptName(void) const = 0;
	};
}