/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://ogre.sourceforge.net/

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

#ifndef INCL_GTKCONFIG_H
#define INCL_GTKCONFIG_H

#include "OgreConfigDialog.h"
#include "OgreRoot.h"
#include "OgreRenderSystem.h"

#include <gtkmm/dialog.h> 
#include <gtkmm/menu.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <gtkmm/label.h>
#include <gtkmm/optionmenu.h>
 
namespace Ogre {
    /** GTK+ config */
    class GTKConfig : public ConfigDialog, public SigC::Object
    {
    public:
        GTKConfig()
        { }
 
        /**
         * Displays a message about reading the config and then attempts to
         * read it from a config file
         */
        bool display(void);
 
    protected:
        class ModelColumns : public Gtk::TreeModel::ColumnRecord
        {
        public:
          ModelColumns()
            { add(col_name); add(col_value); }
 
          Gtk::TreeModelColumn<Glib::ustring> col_name;
          Gtk::TreeModelColumn<Glib::ustring> col_value;
        };
 
        bool on_window_delete(GdkEventAny* event);
        void on_option_changed();
        void on_renderer_changed();
        void on_value_changed();
        void on_btn_ok();
        void on_btn_cancel();
    private:
        Gtk::Dialog* _winConfig;
        ModelColumns _columns;
        Glib::RefPtr<Gtk::ListStore> _list_store;
        Gtk::TreeView* _lstOptions;
        Glib::RefPtr<Gtk::TreeSelection> _option_selection;
        int _cur_index;
        Glib::ustring _cur_name;
        Gtk::OptionMenu* _optRenderer;
        Gtk::Label* _lblOptName;
        Gtk::OptionMenu* _optOptValues;
        Gtk::Menu* _opt_menu;
        ConfigOptionMap _options;
        RenderSystemList* _renderers;
        RenderSystem* _selected_renderer;
 
        void update_option_list();
    }; // class GTKConfig

} // namespae Ogre

#endif // INCL_GTKCONFIG_H
