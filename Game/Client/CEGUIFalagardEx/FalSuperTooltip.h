#ifndef _FalSuperTooltip_h_
#define _FalSuperTooltip_h_

#include "FalModule.h"
#include "CEGUIWindowFactory.h"
#include "FalSuperTooltipProperties.h"
#include "FalSimpleText.h"
#include "IFalSuperTooltip.h"

// Start of CEGUI namespace section
namespace CEGUI
{

    class FALAGARDBASE_API FalagardSuperTooltip : public Window, public IFalagardSuperTooltip
    {
    public:
        static const utf8   WidgetTypeName[];       //!< type name for this widget.

        /*!
        \brief
            Constructor
        */
        FalagardSuperTooltip(const String& type, const String& name);

        /*!
        \brief
            Destructor
        */
        ~FalagardSuperTooltip();


        /*************************************************************************
        		Implementation called by game logic
        *************************************************************************/
		/*!
		\brief
			Recaculate all subwindow
		*/
		virtual void			resizeSelf(void);


		/*************************************************************************
			Accessors
		*************************************************************************/

		String					getPageElementsName(void) const;
		void					setPageElementsName(const String& strName);

	public:
        /*************************************************************************
        		Class of SubWindow
        *************************************************************************/

		class ResizeText : public FalagardSimpleText
		{
		public:
	        static const utf8   WidgetTypeName[];       //!< type name for this widget.

			ResizeText(const String& type, const String& name);
			~ResizeText();

		public:
			/*!
			\brief
				Recaculate size.
			*/
			float		_resizeSelf(void);

			/*!
			\brief
				Set/Get Vert frame
			*/
			const Image*	getTopFrame(void) const { return d_topFrame; }
			void			setTopFrame(const Image* topFrame) { d_topFrame = topFrame; }

			const Image*	getMiddleFrame(void) const { return d_middleFrame; }
			void			setMiddleFrame(const Image* middleFrame) { d_middleFrame = middleFrame; }

			const Image*	getBottomFrame(void) const { return d_bottomFrame; }
			void			setBottomFrame(const Image* bottomFrame) { d_bottomFrame = bottomFrame; }

			URect			getTextArea(void) const { return d_textArea; }
			void			setTextArea(const URect& textArea) { d_textArea = textArea; }

		protected:
			/*************************************************************************
        		Implementation of abstract methods in Window
			*************************************************************************/
			virtual void			populateRenderCache();
			virtual Rect			getTextRenderArea(void) const;

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
				if (class_name==(const utf8*)"FalagardSuperTooltip/ResizeText")	return true;
				return Window::testClassName_impl(class_name);
			}

			/*!
			\brief
				Adds properties specific to the ResizeText base class.
	        
			\return
				Nothing.
			*/
			void addResizeTextProperties(void);

			/*************************************************************************
				Implementation Data
			*************************************************************************/
			const Image*	d_topFrame;
			const Image*	d_middleFrame;
			const Image*	d_bottomFrame;
			URect			d_textArea;

			/*************************************************************************
				Static properties for the SuperTooltip widget
			*************************************************************************/
			static FalagardResizeTextProperties::TopFrame		d_topFrameProperty;
			static FalagardResizeTextProperties::MiddleFrame	d_middleFrameProperty;
			static FalagardResizeTextProperties::BottomFrame	d_bottomFrameProperty;
			static FalagardResizeTextProperties::TextArea		d_textAreaProperty;
		};

		class ResizeTextFactory : public WindowFactory
		{
		public:
			ResizeTextFactory(void) : WindowFactory(ResizeText::WidgetTypeName) { }
			~ResizeTextFactory(void){}
			Window* createWindow(const String& name);
			void destroyWindow(Window* window);
		};

    protected:
        /*************************************************************************
        	Implementation of abstract methods in Window
        *************************************************************************/
		virtual void			populateRenderCache();
		virtual void			onSized(WindowEventArgs& e);

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
			if (class_name==(const utf8*)"FalagardSuperTooltip")	return true;
			return Window::testClassName_impl(class_name);
		}

		/*************************************************************************
			Implementation Data
		*************************************************************************/
		struct Elements
		{
			Window*		pWindow;			//!< Point of window
			bool		bDynamicSize;		//!< Is window size dynamic(ResizeText)
		};
		typedef std::vector< Elements > ElementsVector;

		ElementsVector	d_VectorElements;	//!< The vector of page elements window.

	private:
        /*************************************************************************
            Static properties for the SuperTooltip widget
        *************************************************************************/
		static FalagardSuperTooltipProperties::PageElements	d_pageElementsProperty;
		
        /*************************************************************************
        	Implementation methods
        *************************************************************************/

        /*!
        \brief
            Adds properties specific to the SuperTooltip base class.
        
        \return
            Nothing.
        */
        void addSuperTooltipProperties(void);
    };

    /*!
    \brief
        WindowFactory for FalagardSimpleText type Window objects.
    */
    class FALAGARDBASE_API FalagardSuperTooltipFactory : public WindowFactory
    {
    public:
        FalagardSuperTooltipFactory(void) : WindowFactory(FalagardSuperTooltip::WidgetTypeName) { }
        ~FalagardSuperTooltipFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };

}

#endif