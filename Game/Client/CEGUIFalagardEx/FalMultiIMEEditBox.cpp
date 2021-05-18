#include "FalMultiIMEEditBox.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "CEGUIIme.h"

// Start of CEGUI namespace section
namespace CEGUI
{

	//===================================================================================
	//
	// FalagardMultiIMEEditBox
	//
	//===================================================================================
    const utf8  FalagardMultiIMEEditBox::WidgetTypeName[] = "Falagard/MultiIMEEditBox";

	FalagardMultiIMEEditBoxProperties::DefaultEditBox	FalagardMultiIMEEditBox::d_defaultEditBoxProperty;

    FalagardMultiIMEEditBox::FalagardMultiIMEEditBox(const String& type, const String& name) :
        FalagardMultiLineEditbox(type, name),
		d_lastTextOffset(0),
		d_caretOn(true),
		d_lastTextLen(0)
    {
        addMultiIMEEditBoxProperties();

		d_blinkTime = Ime::GetCaretBlinkTime()*0.001f;
		d_lastBlink = System::getSingleton().getCurTimeElapsed();
	}

    FalagardMultiIMEEditBox::~FalagardMultiIMEEditBox()
    {
    }

	void FalagardMultiIMEEditBox::addMultiIMEEditBoxProperties(void)
    {
		CEGUI_START_ADD_STATICPROPERTY( FalagardMultiIMEEditBox )
		   CEGUI_ADD_STATICPROPERTY( &d_defaultEditBoxProperty );
		CEGUI_END_ADD_STATICPROPERTY
	}

	bool FalagardMultiIMEEditBox::hasInputFocus(void) const
	{
		return Ime::GetActiveIMEEditBox() == this;
	}
	void FalagardMultiIMEEditBox::updateSelf(float elapsed)
	{
		float time_now = System::getSingleton().getCurTimeElapsed();

		int textLen = (int)getText().size();
		if(textLen != d_lastTextLen) 
		{
			resetCaretBlink();
			d_lastTextLen = textLen;
		}

		//
		// Blink the caret
		//
		if( time_now - d_lastBlink >= d_blinkTime )
		{
			d_caretOn = !d_caretOn;
			d_lastBlink = time_now;

			requestRedraw();
		}

		FalagardMultiLineEditbox::updateSelf(elapsed);
	}

	void FalagardMultiIMEEditBox::cacheCaratImagery(const Rect& textArea)
	{
		if( hasInputFocus() && d_caretOn )
			FalagardMultiLineEditbox::cacheCaratImagery( textArea );
	}
    
	void FalagardMultiIMEEditBox::resetCaretBlink(void)
	{
		d_caretOn = true;
		d_lastBlink = System::getSingleton().getCurTimeElapsed();
		requestRedraw();
	}

	void FalagardMultiIMEEditBox::onActivated(ActivationEventArgs& e)
	{
		FalagardMultiLineEditbox::onActivated(e);

		Ime::SetActiveIMEEditBox(this, true);
	}
		
	void FalagardMultiIMEEditBox::onDeactivated(ActivationEventArgs& e)
	{
		FalagardMultiLineEditbox::onDeactivated(e);

		//Ime::SetActiveIMEEditBox(this, false);
	}

	/*************************************************************************
		Handler for mouse button pushed events
	*************************************************************************/
	void FalagardMultiIMEEditBox::onMouseButtonDown(MouseEventArgs& e)
	{
		// base class handling
		FalagardMultiLineEditbox::onMouseButtonDown(e);

		Ime::SetActiveIMEEditBox(this, true);
	}

	void FalagardMultiIMEEditBox::onParentHidden(void)
	{
		// base class handling
		FalagardMultiLineEditbox::onParentHidden();

		Ime::SetActiveIMEEditBox(this, false);
	}

    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardMultiIMEEditBoxFactory::createWindow(const String& name)
    {
        return new FalagardMultiIMEEditBox(d_type, name);
    }

    void FalagardMultiIMEEditBoxFactory::destroyWindow(Window* window)
    {
		delete window;
    }

}

	