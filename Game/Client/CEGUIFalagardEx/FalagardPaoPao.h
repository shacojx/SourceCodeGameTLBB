#pragma once
#include "falagardselffitwindow.h"
#include "ceguirenderableimage.h"
namespace CEGUI
{
	namespace FalagardPaoPaoProperties
	{	
		class ArrowImage : public Property
		{
			
		public:
			ArrowImage() : Property(
				"ArrowImage",
				"Property to get/set the state of the frame enabled setting for the Static widget.  Value is either \"True\" or \"False\".",
				"set: image:")
			{}

			String	get(const PropertyReceiver* receiver) const;
			void	set(PropertyReceiver* receiver, const String& value);

			
		};

		

	};
	class FALAGARDBASE_API FalagardPaoPao :	public FalagardSelfFitWindow
	{
	public:
		static const utf8   WidgetTypeName[];       
		FalagardPaoPao( const String& type, const String& name );
		~FalagardPaoPao(void);
	
		void setArrowImage( const Image* image );

		static FalagardPaoPaoProperties::ArrowImage  d_ArrowProperty;

		virtual void	populateRenderCache();
	protected:
		virtual void updateSelfSize(void);
	private:
		RenderableImage		d_ArrowImage;
	};

	class FALAGARDBASE_API FalagardPaoPaoFactory : public WindowFactory
    {
    public:
        FalagardPaoPaoFactory(void) : WindowFactory(FalagardPaoPao::WidgetTypeName) { }
        ~FalagardPaoPaoFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };

};