#pragma once
#include "falmodule.h"
#include "ceguiwindowfactory.h"
#include "falbutton.h"
#include "FalAnimateProperties.h"
#include "IFalagardAnimate.h"
namespace CEGUI
{
	class Animate;
	class FALAGARDBASE_API FalagardAnimate :	public FalagardButton,public IFalagardAnimate
	{
	public:
		static const utf8   WidgetTypeName[];       //!< type name for this widget.
		virtual void	updateSelf(float elapsed);
		virtual void	drawSelf(float z);
	protected:
		int				m_nAnimateStart;
		Animate			*m_pAnimate;
		void			drawNormal(float z);

		float			m_fAlpha;
		bool			m_bPlay;
	public:

		virtual void Play( bool bPlay );
		static FalagardAnimateProperties::Animate    d_AnimateNameProperty;

		void SetAnimate( const String& strAnimate );
		FalagardAnimate(const String& type, const String& name);
		~FalagardAnimate(void);
	};

	class FALAGARDBASE_API FalagardAnimateFactory : public WindowFactory
    {
    public:
        FalagardAnimateFactory(void) : WindowFactory(FalagardAnimate::WidgetTypeName) { }
        ~FalagardAnimateFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
}