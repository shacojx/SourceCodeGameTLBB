/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/


#include "OgreSDLConfig.h"
#include "OgreException.h"

using namespace Ogre;

bool SDLConfig::display(void)
{
    std::cout << "OGRE Configuration" << std::endl << "------------------" <<  std::endl;

    // Get the renderer
    std::cout << "Select Renderer:" << std::endl;
    int x = 1, choice = 0;
    RenderSystemList* renderers = Root::getSingleton().getAvailableRenderers();
    for (RenderSystemList::iterator pRend = renderers->begin();
            pRend != renderers->end(); pRend++)
    {
        std::cout << "    " << x << ") " << (*pRend)->getName() << std::endl;;
        x++;
    }

    std::cin >> choice;
	
	if (choice<=0 || choice>=x) {
		OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
			"Invalid RenderSystem number",
			"SDLConfig::display");
	}
	RenderSystem* renderer = (*renderers)[choice-1];

    ConfigOptionMap options = renderer->getConfigOptions();

    // Process each option
    for (ConfigOptionMap::iterator it = options.begin(); 
            it != options.end(); it++)
    {
        std::cout << it->second.name << ": " << std::endl;
        x = 1;
        StringVector::iterator opt_it;
        for (opt_it = it->second.possibleValues.begin();
                opt_it != it->second.possibleValues.end(); opt_it++)
        {
            if ((*opt_it) == it->second.currentValue)
                std::cout << "--> ";
            else
                std::cout << "    ";
            std::cout << x << ") " << (*opt_it) << std::endl;
            x++;
        }

        choice = 0;
		std::cin >> choice;
		if (choice<=0 || choice>=x) {
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"Invalid number chosen for '"+it->second.name+"' option",
				"SDLConfig::display");
		}
        opt_it = it->second.possibleValues.begin();
        renderer->setConfigOption(it->second.name, opt_it[choice-1]);
    }

    // All done
    Root::getSingleton().setRenderSystem(renderer);
    Root::getSingleton().saveConfig();

    return true;
}
