
#ifndef _CEGUIIme_h_
#define _CEGUIIme_h_

#include "CEGUIBase.h"

// Start of Ime namespace section
namespace Ime
{

/*******************************************
			Initialize/Uninitialize
********************************************/
void CEGUIEXPORT InitializeImm(unsigned int hWnd);
void CEGUIEXPORT UninitializeImm(void);


/*******************************************
			Process window message
********************************************/
bool CEGUIEXPORT ImeMsgProc(unsigned int uMsg, unsigned int wParam, unsigned int lParam );


/*******************************************
				CEGUI Interface
********************************************/
void CEGUIEXPORT SetActiveIMEEditBox(CEGUI::Window* imeEditBox, bool set);
CEGUIEXPORT CEGUI::Window*	GetActiveIMEEditBox(void);


unsigned int	CEGUIEXPORT GetCaretBlinkTime(void);


enum IMESTATE { IMEUI_STATE_OFF, IMEUI_STATE_ON, IMEUI_STATE_ENGLISH };
//Get the current state.
IMESTATE		CEGUIEXPORT GetState(void);
CEGUIEXPORT std::list< CEGUI::Window* >*  GetActiveExitBox(void);
}

#endif