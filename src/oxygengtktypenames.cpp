/*
* this file is part of the oxygen gtk engine
* Copyright (c) 2010 Hugo Pereira Da Costa <hugo@oxygen-icons.org>
*
* based on the Null Theme Engine for Gtk+.
* Copyright (c) 2008 Robert Staudinger <robert.staudinger@gmail.com>
*
* This  library is free  software; you can  redistribute it and/or
* modify it  under  the terms  of the  GNU Lesser  General  Public
* License  as published  by the Free  Software  Foundation; either
* version 2 of the License, or( at your option ) any later version.
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

#include "oxygengtktypenames.h"

namespace Oxygen
{

    //_______________________________________________
    typedef Gtk::TypeNames::Entry<GtkStateType> StateMap;
    static const StateMap State_map[] =
    {

        { GTK_STATE_NORMAL, "normal" },
        { GTK_STATE_ACTIVE, "active" },
        { GTK_STATE_PRELIGHT, "prelight" },
        { GTK_STATE_SELECTED, "selected" },
        { GTK_STATE_INSENSITIVE, "insensitive" }

    };

    //___________________________________________________________
    GtkStateType Gtk::TypeNames::matchState( const char*  cssState )
    { return Gtk::TypeNames::Finder<GtkStateType>( State_map, 5 ).findGtk( cssState, GTK_STATE_NORMAL ); }

    //___________________________________________________________
    const char*  Gtk::TypeNames::state( GtkStateType gtkState )
    { return Gtk::TypeNames::Finder<GtkStateType>( State_map, 5 ).findCss( gtkState ); }

    //___________________________________________________________
    // Map between shadow types in gtk and css format.
    typedef Gtk::TypeNames::Entry<GtkShadowType> ShadowMap;
    static const ShadowMap Shadow_map[] =
    {
        { GTK_SHADOW_NONE, "none" },
        { GTK_SHADOW_IN, "in" },
        { GTK_SHADOW_OUT, "out" },
        { GTK_SHADOW_ETCHED_IN, "etched-in" },
        { GTK_SHADOW_ETCHED_OUT, "etched-out" }
    };

    //___________________________________________________________
    GtkShadowType Gtk::TypeNames::matchShadow( const char*  cssShadow )
    { return Gtk::TypeNames::Finder<GtkShadowType>( Shadow_map, 5 ).findGtk( cssShadow, GTK_SHADOW_NONE ); }

    //___________________________________________________________
    const char* Gtk::TypeNames::shadow( GtkShadowType gtkShadow )
    { return Gtk::TypeNames::Finder<GtkShadowType>( Shadow_map, 5 ).findCss( gtkShadow ); }

    //___________________________________________________________
    typedef Gtk::TypeNames::Entry<GtkArrowType> ArrowMap;
    static const ArrowMap Arrow_map[] =
    {
        { GTK_ARROW_UP, "up" },
        { GTK_ARROW_DOWN, "down" },
        { GTK_ARROW_LEFT, "left" },
        { GTK_ARROW_RIGHT, "right" },
        { GTK_ARROW_NONE, "none" }
    };

    //___________________________________________________________
    GtkArrowType Gtk::TypeNames::matchArrow( const char*  cssArrow )
    { return Gtk::TypeNames::Finder<GtkArrowType>( Arrow_map, 5 ).findGtk( cssArrow, GTK_ARROW_NONE ); }

    //___________________________________________________________
    const char* Gtk::TypeNames::arrow( GtkArrowType gtkArrow )
    { return Gtk::TypeNames::Finder<GtkArrowType>( Arrow_map, 5 ).findCss( gtkArrow ); }

    //___________________________________________________________
    typedef Gtk::TypeNames::Entry<GtkPositionType> PositionMap;
    static const PositionMap Position_map[] =
    {
        { GTK_POS_LEFT, "left" },
        { GTK_POS_RIGHT, "right" },
        { GTK_POS_TOP, "top" },
        { GTK_POS_BOTTOM, "bottom" },
    };

    //___________________________________________________________
    GtkPositionType Gtk::TypeNames::matchPosition( const char* cssPosition )
    { return Gtk::TypeNames::Finder<GtkPositionType>( Position_map, 4 ).findGtk( cssPosition, GTK_POS_LEFT ); }

    //__________________________________________________________________
    const char* Gtk::TypeNames::position( GtkPositionType gtkPosition )
    { return Gtk::TypeNames::Finder<GtkPositionType>( Position_map, 4 ).findCss( gtkPosition ); }

    //__________________________________________________________________
    typedef Gtk::TypeNames::Entry<GdkWindowEdge> WindowEdgeMap;
    static const WindowEdgeMap WindowEdge_map[] =
    {
        { GDK_WINDOW_EDGE_NORTH, "north" },
        { GDK_WINDOW_EDGE_SOUTH, "south" },
        { GDK_WINDOW_EDGE_WEST, "west" },
        { GDK_WINDOW_EDGE_EAST, "east" },
        { GDK_WINDOW_EDGE_NORTH_WEST, "north-west" },
        { GDK_WINDOW_EDGE_NORTH_EAST, "north-east" },
        { GDK_WINDOW_EDGE_SOUTH_WEST, "south-west" },
        { GDK_WINDOW_EDGE_SOUTH_EAST, "south-east" },
    };

    //__________________________________________________________________
    GdkWindowEdge Gtk::TypeNames::matchWindowEdge( const char*  cssWindowEdge )
    { return Gtk::TypeNames::Finder<GdkWindowEdge>( WindowEdge_map, 8 ).findGtk( cssWindowEdge, GDK_WINDOW_EDGE_SOUTH_EAST ); }

    //__________________________________________________________________
    const char*  Gtk::TypeNames::windowEdge( GdkWindowEdge gdkWindowEdge )
    { return Gtk::TypeNames::Finder<GdkWindowEdge>( WindowEdge_map, 8 ).findCss( gdkWindowEdge ); }

    //__________________________________________________________________
    typedef Gtk::TypeNames::Entry<GtkOrientation> OrientationMap;
    static const OrientationMap Orientation_map[] =
    {
        { GTK_ORIENTATION_HORIZONTAL, "horizontal" },
        { GTK_ORIENTATION_VERTICAL, "vertical" }
    };

    //__________________________________________________________________
    GtkOrientation Gtk::TypeNames::matchOrientation( const char*  cssOrientation )
    { return Gtk::TypeNames::Finder<GtkOrientation>( Orientation_map, 2 ).findGtk( cssOrientation, GTK_ORIENTATION_HORIZONTAL ); }

    //__________________________________________________________________
    const char* Gtk::TypeNames::orientation( GtkOrientation gtkOrientation )
    { return Gtk::TypeNames::Finder<GtkOrientation>( Orientation_map, 2 ).findCss( gtkOrientation ); }

    //__________________________________________________________________
    typedef Gtk::TypeNames::Entry<GtkExpanderStyle> EpanderStyleMap;
    static const EpanderStyleMap ExpanderStyle_map[] =
    {
        { GTK_EXPANDER_COLLAPSED, "collapsed" },
        { GTK_EXPANDER_SEMI_COLLAPSED, "semi-collapsed" },
        { GTK_EXPANDER_SEMI_EXPANDED, "semi-expanded" },
        { GTK_EXPANDER_EXPANDED, "expanded" }
    };

    //__________________________________________________________________
    GtkExpanderStyle Gtk::TypeNames::matchExpanderStyle( const char*  cssExpanderStyle )
    { return Gtk::TypeNames::Finder<GtkExpanderStyle>( ExpanderStyle_map, 4 ).findGtk( cssExpanderStyle, GTK_EXPANDER_COLLAPSED ); }

    //__________________________________________________________________
    const char*  Gtk::TypeNames::expanderStyle( GtkExpanderStyle gtkExpanderStyle )
    { return Gtk::TypeNames::Finder<GtkExpanderStyle>( ExpanderStyle_map, 4 ).findCss( gtkExpanderStyle ); }

}