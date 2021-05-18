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
#include "OgreStableHeaders.h"
#include "OgreStringInterface.h"

namespace Ogre {

    ParamDictionaryMap StringInterface::msDictionary;

    //-----------------------------------------------------------------------
    bool StringInterface::createParamDictionary(const String& className)
    {
        // Don't assert here, since some exist classes create its own
        // parameter dictionary but another one already created by
        // parent class, i.e. BorderPanelOverlayElement.
        //assert(mParamDict == 0 && "You can't create parameter dictionary more than once");

        ParamDictionaryMap::iterator it = msDictionary.find(className);
        if (it != msDictionary.end())
        {
            // Shortest execute path for most case
            mParamDict = &it->second;
            return false;
        }

        // Create one if not exists.
        std::pair<ParamDictionaryMap::iterator, bool> inserted =
            msDictionary.insert(ParamDictionaryMap::value_type(className, ParamDictionary()));
        mParamDict = &inserted.first->second;
        return inserted.second;
    }
    //-----------------------------------------------------------------------
    bool StringInterface::createParamDictionary(const char* className)
    {
        return createParamDictionary(String(className));
    }
    //-----------------------------------------------------------------------
    const ParameterList& StringInterface::getParameters(void) const
    {
        static ParameterList emptyList;

        const ParamDictionary* dict = getParamDictionary();
        if (dict)
            return dict->getParameters();
        else
            return emptyList;

    };
    //-----------------------------------------------------------------------
    bool StringInterface::setParameter(const String& name, const String& value)
    {
        // Get dictionary
        ParamDictionary* dict = getParamDictionary();

        if (dict)
        {
            // Look up command object
            ParamCommand* cmd = dict->getParamCommand(name);
            if (cmd)
            {
                cmd->doSet(this, value);
                return true;
            }
        }
        // Fallback
        return false;
    }
	//-----------------------------------------------------------------------
	void StringInterface::setParameterList(const NameValuePairList& paramList)
	{
		NameValuePairList::const_iterator i, iend;
		iend = paramList.end();
		for (i = paramList.begin(); i != iend; ++i)
		{
			setParameter(i->first, i->second);
		}
	}
    //-----------------------------------------------------------------------
    String StringInterface::getParameter(const String& name) const
    {
        // Get dictionary
        const ParamDictionary* dict = getParamDictionary();

        if (dict)
        {
            // Look up command object
            const ParamCommand* cmd = dict->getParamCommand(name);

            if (cmd)
            {
                return cmd->doGet(this);
            }
        }

        // Fallback
        return StringUtil::BLANK;
    }
    //-----------------------------------------------------------------------
    void StringInterface::copyParametersTo(StringInterface* dest) const
    {
        // Get dictionary
        const ParamDictionary* dict = getParamDictionary();

        if (dict)
        {
#if OGRE_STRING_INTERFACE_COPY
            // Use fast copy method directly if they are the same type.
            if (dict == dest->getParamDictionary())
            {
                // Iterate through own commands
                ParamCommandMap::const_iterator c, cend;
                cend = dict->mParamCommands.end();
                for (c = dict->mParamCommands.begin(); c != cend; ++c)
                {
                    c->second->doCopy(dest, this);
                }
            }
            else
#endif
            {
                // Iterate through own parameters
                ParameterList::const_iterator i, iend;
                iend = dict->mParamDefs.end();
                for (i = dict->mParamDefs.begin(); i != iend; ++i)
                {
                    dest->setParameter(i->name, getParameter(i->name));
                }
            }
        }
    }
    //-----------------------------------------------------------------------
    void StringInterface::cleanupDictionary () {
      msDictionary.clear();
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void ParamCommand::doCopy(void* target, const void* source)
    {
        // Default implementation just get/set, derived class should be implement
        // their special case version for effective (i.e. avoid string conversion).
        doSet(target, doGet(source));
    }
#endif
}
