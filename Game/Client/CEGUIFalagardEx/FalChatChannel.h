#ifndef _FalChatChannel_h_
#define _FalChatChannel_h_

#include "FalModule.h"
#include "CEGUIWindowFactory.h"
#include "FalChatChannelProperties.h"
#include "FalSimpleText.h"
#include "IFalChatChannel.h"

// Start of CEGUI namespace section
namespace CEGUI
{

    class FALAGARDBASE_API FalagardChatChannel : public IFalagardChatChannel, public Window
    {
    public:
        static const utf8   WidgetTypeName[];       //!< type name for this widget.

        /*!
        \brief
            Constructor
        */
        FalagardChatChannel(const String& type, const String& name);

        /*!
        \brief
            Destructor
        */
        ~FalagardChatChannel();

		/*************************************************************************
			Accessors
		*************************************************************************/
		Size			getItemSize(void) const { return d_itemSize;}
		void			setItemSize(const Size& itemSize) { d_itemSize = itemSize; }

		Point			getAnchorPosition(void) const { return d_anchorPosition; }
		void			setAnchorPosition(const Point& anchorPosition) { d_anchorPosition = anchorPosition; }

		const Image*	getHoverChannelBkg(void) const { return d_hoverChannelBkg; }
		void			setHoverChannelBkg(const Image* hoverChannelBkg) { d_hoverChannelBkg = hoverChannelBkg; }

		String			getHoverChannel(void) const;
		String			getHoverChannelName(void) const;

        /*************************************************************************
        		Implementation called by game logic
        *************************************************************************/
		/*!
		\brief
			Clear all channel in channel list.
		*/
		virtual void			clearAllChannel(void);

		/*!
		\brief
			Add channel in channel list
		*/
		virtual void			addChannel(const String& strType, const String& strIconName, const String& strName);

    protected:
        /*************************************************************************
        	Implementation of abstract methods in Window
        *************************************************************************/
		virtual void	populateRenderCache(void);
		virtual void	onMouseMove(MouseEventArgs& e);
		virtual void	onMouseButtonDown(MouseEventArgs& e);
		virtual void	onMouseButtonUp(MouseEventArgs& e);
		virtual void	onMouseLeaves(MouseEventArgs& e);


		/*************************************************************************
			Implementation Function
		*************************************************************************/
		void resizeSelf(void);
		void updateInternalState(const Point& pt);

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
			if (class_name==(const utf8*)"FalagardChatChannel")	return true;
			return Window::testClassName_impl(class_name);
		}

		/*************************************************************************
			Implementation Data
		*************************************************************************/
		struct ChannelItem
		{
			String			d_strType;
			const Image*	d_headIcon;
			String			d_strName;
		};

		std::vector< ChannelItem >		d_allChannel;	//!< All channel list.
		Size							d_itemSize;
		Point							d_anchorPosition;
		const Image*					d_hoverChannelBkg;
		int								d_hoverChannel;
		int								d_lastHoverChannel;
		bool							d_pushed;

	private:
        /*************************************************************************
            Static properties for the ChatChannel widget
        *************************************************************************/
		static FalagardChatChannelProperties::ItemSize			d_itemSizeProperty;
		static FalagardChatChannelProperties::AnchorPosition	d_anchorPositionProperty;
		static FalagardChatChannelProperties::HoverChannelBkg	d_hoverChannelBkgProperty;
		static FalagardChatChannelProperties::HoverChannel		d_hoverChannelProperty;
		static FalagardChatChannelProperties::HoverChannelName	d_hoverChannelNameProperty;

        /*************************************************************************
        	Implementation methods
        *************************************************************************/
        /*!
        \brief
            Adds properties specific to the ChatChannel base class.
        
        \return
            Nothing.
        */
        void addChatChannelProperties(void);
    };

    /*!
    \brief
        WindowFactory for FalagardChatChannel type Window objects.
    */
    class FALAGARDBASE_API FalagardChatChannelFactory : public WindowFactory
    {
    public:
        FalagardChatChannelFactory(void) : WindowFactory(FalagardChatChannel::WidgetTypeName) { }
        ~FalagardChatChannelFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
}



#endif