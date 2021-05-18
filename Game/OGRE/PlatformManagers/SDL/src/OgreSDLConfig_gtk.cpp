/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the 
Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along 
with this program; if not, write to the Free Software Foundation, Inc., 59 
Temple Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/licenses/lgpl.html.
-----------------------------------------------------------------------------
*/

#include "OgreSDLConfig.h"
#include "OgreLogManager.h"

#include <libglademm/xml.h>

#include <gtkmm/main.h>
#include <gtkmm/menushell.h>
#include <gtkmm/window.h>

using namespace Ogre;
using namespace Gtk::Menu_Helpers;

bool SDLConfig::display(void)
{
    Gtk::Main kit(0, NULL);

    std::string sharedir( SHAREDIR );
    Glib::RefPtr<Gnome::Glade::Xml> xml = Gnome::Glade::Xml::create( sharedir + "/ogre-config.glade");
    if (!xml)
    {
        LogManager::getSingleton().logMessage("Problem loading config");
        exit(1);
    }

    _winConfig = NULL;
    xml->get_widget("winConfig", _winConfig);
    if (!_winConfig)
    {
        LogManager::getSingleton().logMessage("Invalid window.");
        exit(1);
    }

    xml->get_widget("lstOptions", _lstOptions);
    xml->get_widget("optRenderer", _optRenderer);
    xml->get_widget("lblOptName", _lblOptName);
    xml->get_widget("optOptValues", _optOptValues);
    Gtk::Button* btn_ok;
    xml->get_widget("btnOk", btn_ok);
    Gtk::Button* btn_cancel;
    xml->get_widget("btnCancel", btn_cancel);

    _opt_menu = NULL;

    // Hookup signals
    _winConfig->signal_delete_event().connect(sigc::mem_fun(this, &SDLConfig::on_window_delete));
    _option_selection = _lstOptions->get_selection();
    _option_selection->signal_changed().connect(sigc::mem_fun(this,
                &SDLConfig::on_option_changed));
    _optRenderer->signal_changed().connect(sigc::mem_fun(this,
                &SDLConfig::on_renderer_changed));
    _optOptValues->signal_changed().connect(sigc::mem_fun(this,
                &SDLConfig::on_value_changed));
    btn_ok->signal_clicked().connect(sigc::mem_fun(this, &SDLConfig::on_btn_ok));
    btn_cancel->signal_clicked().connect(sigc::ptr_fun(&Gtk::Main::quit));


    // Initialize
    _list_store = Gtk::ListStore::create(_columns);
    _lstOptions->set_model(_list_store);
    _lstOptions->append_column("Option", _columns.col_name);
    _lstOptions->append_column("Value", _columns.col_value);

    // Setup initial values
    _renderers = Root::getSingleton().getAvailableRenderers();
    Gtk::Menu* menu = Gtk::manage(new Gtk::Menu());
    MenuList items = menu->items();
    for (RenderSystemList::iterator pRend = _renderers->begin(); 
            pRend != _renderers->end(); pRend++)
    {
        items.push_back(MenuElem((*pRend)->getName()));
    }
    _optRenderer->set_menu(*menu);
    _selected_renderer = *(_renderers->begin());

    update_option_list();

    _option_selection->select(_list_store->children().begin());

    _winConfig->show();

    kit.run();

    return true;
}

bool SDLConfig::on_window_delete(GdkEventAny* event)
{
    Gtk::Main::quit();

    return true;
}

void SDLConfig::on_option_changed()
{
    Gtk::TreeModel::iterator treeIT = _option_selection->get_selected();
    if (!treeIT)
        return;

    Gtk::TreeModel::Row row = *(treeIT);
    Glib::ustring name = row[_columns.col_name];

    if (name == _cur_name)
        return;
    _cur_name = name;

    Glib::ustring value = row[_columns.col_value];
    _lblOptName->set_text(name);
    ConfigOption opt = _options[name.raw()];

    if (!_opt_menu)
        _opt_menu = Gtk::manage(new Gtk::Menu());

    MenuList items = _opt_menu->items();
    items.erase(items.begin(), items.end());

    _cur_index = -1;
    int i = 0;
    for (StringVector::iterator it = opt.possibleValues.begin(); 
            it != opt.possibleValues.end(); it++)
    {
        if ((*it) == value.raw())
            _cur_index = i;
        else
            i++;

        items.push_back(MenuElem((*it)));
    }

    _optOptValues->set_menu(*_opt_menu);
    _optOptValues->set_history(_cur_index);
}

void SDLConfig::on_renderer_changed()
{
    RenderSystemList::iterator pRend = _renderers->begin();
    _selected_renderer = pRend[_optRenderer->get_history()];
    if (!_selected_renderer)
    {
        std::cerr << "Selected no renderer!" << std::endl;
        return;
    }

    update_option_list();
}

void SDLConfig::on_value_changed()
{
    static int last_history = -1;    
    
    int hist = _optOptValues->get_history();
    if (hist == _cur_index)
        return;

    _cur_index = hist;

    ConfigOption opt = _options[_cur_name.raw()];
    StringVector::iterator pos_it = opt.possibleValues.begin();

    _selected_renderer->setConfigOption(opt.name, pos_it[hist]);

    update_option_list();
}

void SDLConfig::on_btn_ok()
{
    Root::getSingleton().setRenderSystem(_selected_renderer);
    Root::getSingleton().saveConfig();

    _winConfig->hide();
    

    Gtk::Main::quit();
}

void SDLConfig::update_option_list()
{
    _options = _selected_renderer->getConfigOptions();

    _list_store->clear();
    for (ConfigOptionMap::iterator it = _options.begin(); 
            it != _options.end(); it++)
    {
        Gtk::TreeModel::Row row = *(_list_store->append());
        row[_columns.col_name] = it->second.name;
        row[_columns.col_value] = it->second.currentValue;
    }
}
