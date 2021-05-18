#ifndef _FalActionButton_h_
#define _FalActionButton_h_

#include "FalButton.h"
#include "FalActionButtonProperties.h"
#include "IFalagardActionButton.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	struct AnimateRuntime;
	class Animate;

    class FALAGARDBASE_API FalagardActionButton : public FalagardButton, public IFalagardActionButton
    {
	public:
		/*!
		\brief
			EventArgs based class used for certain drag/drop notifications
		*/
		class ActionButtonDragDropEventArgs : public WindowEventArgs
		{
		public:
			ActionButtonDragDropEventArgs(Window* wnd) : WindowEventArgs(wnd) {}
			FalagardActionButton*	dragDropItem; //<! pointer to the FalagardActionButton window being dragged / dropped.
		};

    public:
        static const utf8  WidgetTypeName[];		//!< type name for this widget.
        static const String EventNamespace;			//!< Namespace for global events

        static const String EventDragStarted;			//!< Name of the event fired when the user begins dragging the thumb.
        static const String EventDragEnded;				//!< Name of the event fired when the user releases the thumb.
        static const String EventDragPositionChanged;   //!< Event fired when the drag position has changed.
        static const String EventDragEnabledChanged;    //!< Event fired when dragging is enabled or disabled.
        static const String EventDragThresholdChanged;  //!< Event fired when the drag pixel threshold is changed.
        static const String EventDragDropTargetChanged; //!< Event fired when the drop target changes.
		static const String EventDropItemEnters;		//!< A ActionButton has been dragged over this window.
		static const String EventDropItemLeaves;		//!< A ActionButton has left this window.
		static const String EventDropItemDropped;		//!< A ActionButton was dropped on this Window.
		static const String EventRBClicked;				//!< The button was clicked by right button.

		/*!
        \brief
            Constructor
        */
		FalagardActionButton(const String& type, const String& name);

		/*!
        \brief
            Destructor
        */
        ~FalagardActionButton();

        /*************************************************************************
        	Public Interface to FalagardActionButton
        *************************************************************************/
		bool			isEmpty() const { return d_empty; }
		void			setEmpty(bool setting) { d_empty = setting; }

		bool			isChecked() const { return d_checked; }
		void			setChecked(bool checked) { d_checked = checked; }

		bool			isGloom() const { return d_gloom; }
		void			setGloom(bool isgloom) { d_gloom = isgloom; }

		bool			isDefault(void) const { return d_default; }
		void			setDefault(bool bDefault) { d_default = bDefault; }

		//Character at corner
		/*
		\brief
			0 - TopLeft		1 - TopRight	2 - BotLeft		3 - BotRight
		*/
		String			getCornerChar(int nPos) const;
		void			setCornerChar(int nPos, const String& strChar );

		virtual void	setAnimate(const String& name, int totalTime, float percent);
		virtual void	cleanAnimate(void);

        /*!
        \brief
            Return whether dragging is currently enabled for this DragContainer.

        \return
            - true if dragging is enabled and the DragContainer may be dragged.
            - false if dragging is disabled and the DragContainer may not be dragged.
        */
		bool			isDraggingEnabled(void) const { return d_draggingEnabled; }

        /*!
        \brief
            Set whether dragging is currently enabled for this DragContainer.

        \param setting
            - true to enable dragging so that the DragContainer may be dragged.
            - false to disabled dragging so that the DragContainer may not be dragged.

        \return
            Nothing.
        */
		void			setDraggingEnabled(bool setting) { d_draggingEnabled = setting; }

        /*!
        \brief
            Return the current drag threshold in pixels.

            The drag threshold is the number of pixels that the mouse must be
            moved with the left button held down in order to commence a drag
            operation.
        
        \return
            float value indicating the current drag threshold value.
        */
		float			getPixelDragThreshold(void) const { return d_dragThreshold; }

        /*!
        \brief
            Set the current drag threshold in pixels.

            The drag threshold is the number of pixels that the mouse must be
            moved with the left button held down in order to commence a drag
            operation.

        \param pixels
            float value indicating the new drag threshold value.

        \return
            Nothing.
        */
		void			setPixelDragThreshold(float pixels) { d_dragThreshold = pixels; }

		/*!
		\brief
			Subscribes DragdropStarted Event.
		*/
		virtual void	subscribeDragDropStartedEvent(Event::Subscriber subscriber);
		/*!
		\brief
			Subscribes MouseEnter Event.
		*/
		virtual void	subscribeMouseEnterEvent(Event::Subscriber subscriber);
		/*!
		\brief
			Subscribes MouseLeave Event.
		*/
		virtual void	subscribeMouseLeaveEvent(Event::Subscriber subscriber);

		/*!
		\brief
			Notify the action button in draging that the draggging is done!.
		*/
		virtual void	notifyDragingEnd(void);

		/*!
		\brief
			Notify a action button drag drop in.
		*/
		virtual void	notifyDragDrop_Droped(FalagardActionButton* pBtn);
		/*!
		\brief
			Set/Get Logic data (ActionItem)
		*/
		virtual void			setLogicItemData(void* pData)	{ m_pLogicData = pData; };
		virtual void*			getLogicItemData(void) const { return m_pLogicData; }

		/*!
		\brief 
			Set/Get drag accept name.
		*/
		virtual const char*		getDragAcceptName(void) const { return d_dragAcceptName.c_str(); }
		virtual void			setDragAcceptName(const String& value) { d_dragAcceptName = value; }

	protected:
        /*************************************************************************
        	Implementation of abstract methods in Window
        *************************************************************************/
		virtual void	drawSelf(float z);

        /*************************************************************************
        	Overrides for Event handler methods
        *************************************************************************/
		virtual void	initialise(void);
		virtual void	onMouseButtonDown(MouseEventArgs& e);
		virtual void	onMouseButtonUp(MouseEventArgs& e);
        virtual void	onMouseMove(MouseEventArgs& e);
		virtual void	onRBClicked(WindowEventArgs& e);
		virtual void	onClicked(WindowEventArgs& e);
		virtual void	updateSelf(float elapsed);
		virtual void	prepareBigchar(void);

        /*************************************************************************
        	New Event handler methods
        *************************************************************************/

    protected:
        /*************************************************************************
        	Protected Implementation Methods
        *************************************************************************/
		void	drawCorner(float z);
		void	drawAnimate(float z);
		void	drawDefault(float z);

		struct ANIMATE
		{
			Animate*		d_animate;
			int				d_time_start;
			int				d_time_total;
			const Image*	d_currentFrame;
			float			d_alpha;
		};

		void	updateAnimate(ANIMATE& animate);

        /*!
        \brief
            Return whether the required minimum movement threshold before initiating dragging
            has been exceeded.

        \param local_mouse
            Mouse position as a pixel offset from the top-left corner of this window.

        \return
            - true if the threshold has been exceeded and dragging should be initiated.
            - false if the threshold has not been exceeded.
        */		
        bool isDraggingThresholdExceeded(const Point& local_mouse);


		/*!
		\brief
			Return whether this window was inherited from the given class name at some point in the inheritance heirarchy.

		\param class_name
			The class name that is to be checked.

		\return
			true if this window was inherited from \a class_name. false if not.
		*/
		virtual bool	testClassName_impl(const String& class_name) const
		{
			if (class_name==(const utf8*)"FalagardActionButton")	return true;
			return FalagardButton::testClassName_impl(class_name);
		}

	protected:
        /*************************************************************************
        	Data
        *************************************************************************/
		void*	m_pLogicData;		//!< Logic data refrence store here.
        bool	d_empty;			//!< True if button empty
		bool	d_checked;			//!< True if button is checked
		bool	d_gloom;			//!< True if button is gloom
		bool	d_default;			//!< True if button is default action button

        bool    d_draggingEnabled;  //!< True when dragging is enabled.
        bool    d_leftMouseDown;    //!< True when left mouse button is down.
        bool    d_dragging;         //!< true when being dragged.
        Point   d_dragPoint;        //!< point we are being dragged at.
        float   d_dragThreshold;    //!< Pixels mouse must move before dragging commences.

		String	d_dragAcceptName;	//!< The name of drag accept name.

		struct CORNER_CHAR
		{
			String			d_Char; 				//String at ActionButton Corner
		};

		CORNER_CHAR			d_CornerChar_TopLeft;	//!< TopLeft Corner
		CORNER_CHAR			d_CornerChar_TopRight;	//!< TopRight Corner
		CORNER_CHAR			d_CornerChar_BotLeft;	//!< BottomLeft Corner
		CORNER_CHAR			d_CornerChar_BotRight;	//!< BottomRight Corner

		MouseButton			d_ActiveButton;			//!< MouseButton that do "Clicked" event
		MouseButton			d_DragButton;			//!< MouseButton that do "Drag" event;

		ANIMATE				d_animate1;				//!< Button animate layout1
		bool				m_bCoolDown;			// 说明现在正是cool down 状态
	private:
        /*************************************************************************
            Static properties for the Spinner widget
        *************************************************************************/
        static FalagardActionButtonProperties::Empty			d_emptyProperty;
        static FalagardActionButtonProperties::Checked			d_checkedProperty;
		static FalagardActionButtonProperties::Gloom			d_gloomProperty;
		static FalagardActionButtonProperties::Default			d_defaultProperty;
		static FalagardActionButtonProperties::CornerChar		d_cornerCharProperty;
        static FalagardActionButtonProperties::DraggingEnabled	d_dragEnabledProperty;
        static FalagardActionButtonProperties::DragThreshold	d_dragThresholdProperty;
		static FalagardActionButtonProperties::DragAcceptName	d_dragAcceptNameProperty;

        /*************************************************************************
        	Implementation methods
        *************************************************************************/

        /*!
        \brief
            Adds properties specific to the DragContainer base class.
        
        \return
            Nothing.
        */
        void addActionButtonProperties(void);

	}; 

    /*!
    \brief
        WindowFactory for FalagardButton type Window objects.
    */
    class FALAGARDBASE_API FalagardActionButtonFactory : public WindowFactory
    {
    public:
        FalagardActionButtonFactory(void) : WindowFactory(FalagardActionButton::WidgetTypeName) { }
        ~FalagardActionButtonFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
}

#endif

