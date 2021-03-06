#ifndef carboncomboboxengine_h
#define carboncomboboxengine_h
/*
* this file is part of the carbon gtk engine
* Copyright (c) 2010 Hugo Pereira Da Costa <hugo.pereira@free.fr>
*
* This  library is free  software; you can  redistribute it and/or
* modify it  under  the terms  of the  GNU Lesser  General  Public
* License  as published  by the Free  Software  Foundation; either
* version 2 of the License, or(at your option ) any later version.
*
* This library is distributed  in the hope that it will be useful,
* but  WITHOUT ANY WARRANTY; without even  the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License  along  with  this library;  if not,  write to  the Free
* Software Foundation, Inc., 51  Franklin St, Fifth Floor, Boston,
* MA 02110-1301, USA.
*/


#include "carbongenericengine.h"
#include "carbondatamap.h"
#include "carboncomboboxdata.h"

#include <gtk/gtk.h>

namespace Carbon
{
    //! forward declaration
    class Animations;

    //! stores data associated to editable comboboxes
    /*!
    ensures that the text  and the button of editable comboboxes
    gets hovered and focus flags at the same time
    */
    class ComboBoxEngine: public GenericEngine<ComboBoxData>
    {

        public:

        //! constructor
        ComboBoxEngine( Animations* widget ):
            GenericEngine<ComboBoxData>( widget )
            {}

        //! destructor
        virtual ~ComboBoxEngine( void )
        {}

        //!@name modifiers
        //@{

        //! assign button to data matching widget
        void setButton( GtkWidget* widget, GtkWidget* value )
        { data().value( widget ).setButton( value ); }

        //! button focus
        void setButtonFocus( GtkWidget* widget, bool value )
        { data().value( widget ).setButtonFocus( value ); }

        //! register child
        void registerChild( GtkWidget* widget, GtkWidget* child )
        { data().value( widget ).registerChild( child ); }

        //@}

        //!@name accessors
        //@{

        //! find combobox matching widget
        /*! the widget can be any of those in a visible list */
        inline GtkWidget* find( GtkWidget* );

        //! true if either button or is pressed
        bool pressed( GtkWidget* widget )
        { return data().value( widget ).pressed(); }

        //! true if either button or entry has focus
        bool hasFocus( GtkWidget* widget )
        { return data().value( widget ).hasFocus(); }

        //! true if comboBox is hovered
        bool hovered( GtkWidget* widget )
        { return data().value( widget ).hovered(); }

        //@}

    };

    //_________________________________________________
    GtkWidget* ComboBoxEngine::find( GtkWidget* value )
    {
        GtkWidget* topLevel( gtk_widget_get_toplevel( value ) );
        DataMap<ComboBoxData>::Map& dataMap( data().map() );
        for( DataMap<ComboBoxData>::Map::iterator iter = dataMap.begin(); iter != dataMap.end(); iter++ )
        {
            if( iter->second.pressed() )
            {
                iter->second.setList( topLevel );
                return iter->first;
            }
        }

        for( DataMap<ComboBoxData>::Map::iterator iter = dataMap.begin(); iter != dataMap.end(); iter++ )
        { if( iter->second.list() == topLevel ) return iter->first; }

        return 0L;

    }

}

#endif
