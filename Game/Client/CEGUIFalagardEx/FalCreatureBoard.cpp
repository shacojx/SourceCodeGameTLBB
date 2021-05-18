#include "FalCreatureBoard.h"


// Start of CEGUI namespace section
namespace CEGUI
{
	//===================================================================================
	//
	// FalagardCreatureBoard
	//
	//===================================================================================
    const utf8  FalagardCreatureBoard::WidgetTypeName[] = "Falagard/CreatureBoard";

    FalagardCreatureBoard::FalagardCreatureBoard(const String& type, const String& name) :
        Window(type, name)
    {
        addCreatureBoardProperties();
    }

    FalagardCreatureBoard::~FalagardCreatureBoard()
    {
    }

	void FalagardCreatureBoard::addCreatureBoardProperties(void)
    {
    }

    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardCreatureBoardFactory::createWindow(const String& name)
    {
        return new FalagardCreatureBoard(d_type, name);
    }

    void FalagardCreatureBoardFactory::destroyWindow(Window* window)
    {
		delete window;
    }

}