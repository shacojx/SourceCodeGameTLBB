
namespace CEGUI
{

	class IFalagardMeshWindow
	{
	public:
		/*!
		\brief
			Subscribes Shown Event.
		*/
		virtual void	subscribeShownEvent(Event::Subscriber subscriber) = 0;
		/*!
		\brief
			Subscribes Hiden Event.
		*/
		virtual void	subscribeHidenEvent(Event::Subscriber subscriber) = 0;
	};

}