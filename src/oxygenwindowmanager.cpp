/*
* oxygenwindowmanager.cpp
* pass some window mouse press/release/move event actions to window manager
* -------------------
*
* Copyright (c) 2010 Cédric Bellegarde <gnumdk@gmail.com>
* Copyright (c) 2010 Hugo Pereira Da Costa <hugo@oxygen-icons.org>
*
* Largely inspired from Qtcurve style
* Copyright (C) Craig Drummond, 2003 - 2010 craig.p.drummond@gmail.com
*
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*/

#include "oxygenwindowmanager.h"
#include "oxygengtkutils.h"
#include "oxygenstyle.h"
#include "config.h"

#include <gdk/gdkx.h>

namespace Oxygen
{


    //_________________________________________________
    WindowManager::WindowManager( void ):
        _mode( Full ),
        _drag( false ),
        _dragDistance( 4 ),
        _dragDelay( 500 ),
        _widget( 0L ),
        _x(-1),
        _y(-1)
    {}

    //_________________________________________________
    WindowManager::~WindowManager( void )
    {
        _map.disconnectAll();
        _map.clear();
    }

    //_________________________________________________
    void WindowManager::registerWidget( GtkWidget* widget )
    {

        if( _map.contains( widget ) ) return;

        #if OXYGEN_DEBUG
        std::cout << "Oxygen::WindowManager::registerWidget - " << widget << "(" << G_OBJECT_TYPE_NAME( widget ) << ")" << std::endl;
        #endif

        // Force widget to listen to relevant events
        gtk_widget_add_events( widget,
            GDK_BUTTON_RELEASE_MASK |
            GDK_BUTTON_PRESS_MASK   |
            GDK_LEAVE_NOTIFY_MASK   |
            GDK_BUTTON1_MOTION_MASK );

        // allocate new Data object
        Data& data( _map.registerWidget( widget ) );

        // connect signals
        if( _mode != Disabled ) connect( widget, data );

    }

    //_________________________________________________
    void WindowManager::unregisterWidget( GtkWidget* widget )
    {
        if( !_map.contains( widget ) ) return;

        #if OXYGEN_DEBUG
        std::cout << "Oxygen::WindowManager::unregisterWidget - " << widget << "(" << G_OBJECT_TYPE_NAME( widget ) << ")" << std::endl;
        #endif

        _map.value( widget ).disconnect( widget );
        _map.erase( widget );

        if( _widget == widget )
        {
            _widget = 0L;
            _x = -1;
            _y = -1;
            _drag = false;
        }

    }

    //_________________________________________________
    void WindowManager::setMode( WindowManager::Mode mode )
    {
        if( mode == _mode ) return;

        // connect/disconnect all data in map, based on new and old mode
        if( mode == Disabled ) { _map.disconnectAll(); }
        else if( _mode == Disabled )
        {
            DataMap<Data>::Map& map( _map.map() );
            for( DataMap<Data>::Map::iterator iter = map.begin(); iter != map.end(); iter++ )
            { connect( iter->first, iter->second ); }
        }

        // assign new mode
        _mode = mode;

    }

    //_________________________________________________
    gboolean WindowManager::wmStyleSet( GtkWidget* widget, GtkStyle* , gpointer data )
    {
        static_cast<WindowManager*>(data)->unregisterWidget( widget );
        return false;
    }

    //_________________________________________________
    gboolean WindowManager::wmDestroy( GtkWidget* widget, gpointer data )
    {
        static_cast<WindowManager*>(data)->unregisterWidget( widget );
        return false;
    }

    //_________________________________________________
    gboolean WindowManager::wmButtonPress(GtkWidget *widget, GdkEventButton* event, gpointer data )
    {

        if( event->type == GDK_BUTTON_PRESS && event->button == 1 )
        {

            return static_cast<WindowManager*>(data)->isWindowDragWidget( widget, event );

        } else return false;

    }

    //_________________________________________________
    gboolean WindowManager::wmButtonRelease(GtkWidget *widget, GdkEventButton*, gpointer data )
    { return static_cast<WindowManager*>( data )->finishDrag( widget ); }

    //_________________________________________________
    gboolean WindowManager::wmLeave(GtkWidget *widget, GdkEventCrossing*, gpointer data )
    { return static_cast<WindowManager*>( data )->finishDrag( widget ); }

    //_________________________________________________
    gboolean WindowManager::wmMotion( GtkWidget *widget, GdkEventMotion* event, gpointer data )
    { return static_cast<WindowManager*>(data)->startDrag( widget, event ); }

    //_________________________________________________________________
    gboolean WindowManager::startDelayedDrag( gpointer data )
    {
        static_cast<WindowManager*>( data )->startDrag();
        return FALSE;
    }

    //_________________________________________________
    bool WindowManager::isWindowDragWidget( GtkWidget* widget, GdkEventButton* event )
    {
        if( _mode == Disabled ) return false;
        else if( (!_drag) && withinWidget(widget, event ) && useEvent( widget, event ) )
        {

            // store widget and event position
            _widget = widget;
            _x = int(event->x_root);
            _y = int(event->y_root);

            // start timer
            if( _timer.isRunning() ) _timer.stop();
            _timer.start( _dragDelay, (GSourceFunc)startDelayedDrag, this );

            // enable drag and accept
            _drag = true;
            return true;

        } else return false;
    }

    //_________________________________________________________________
    bool WindowManager::startDrag( GtkWidget* widget, GdkEventMotion* event )
    {

        // make sure drag is enabled
        if( !_drag ) return false;

        // check displacement with respect to drag start
        const int distance( abs( _x - int(event->x_root) ) + abs( _y - int(event->y_root) ) );

        if( distance > 0 && _timer.isRunning() ) _timer.stop();
        if( distance < _dragDistance ) return false;

        // start drag from current position
        return startDrag( widget, int(event->x_root), int(event->y_root) );

    }

    //_________________________________________________________________
    bool WindowManager::startDrag( GtkWidget* widget, int x, int y )
    {
        // create xevent and send.
        XEvent     xev;
        GtkWindow  *topLevel = GTK_WINDOW( gtk_widget_get_toplevel( widget ) );
        GdkWindow  *window = gtk_widget_get_window( GTK_WIDGET( topLevel ) );
        GdkDisplay *display = gtk_widget_get_display( GTK_WIDGET( topLevel ) );
        GdkWindow  *root = gdk_screen_get_root_window( gtk_window_get_screen( topLevel ) );

        xev.xclient.type = ClientMessage;
        xev.xclient.message_type = gdk_x11_get_xatom_by_name_for_display(display, "_NET_WM_MOVERESIZE");
        xev.xclient.display = GDK_DISPLAY_XDISPLAY(display);
        xev.xclient.window = GDK_WINDOW_XID(window);
        xev.xclient.format = 32;
        xev.xclient.data.l[0] = x;
        xev.xclient.data.l[1] = y;
        xev.xclient.data.l[2] = 8; // NET::Move
        xev.xclient.data.l[3] = Button1;
        xev.xclient.data.l[4] = 0;
        XUngrabPointer(GDK_DISPLAY_XDISPLAY(display), CurrentTime);

        XSendEvent(
            GDK_DISPLAY_XDISPLAY(display),
            GDK_WINDOW_XID(root),
            False,
            SubstructureRedirectMask | SubstructureNotifyMask,
            &xev);

        // force a release as some widgets miss it...
        wmButtonRelease( widget, 0L, this );
        return true;
    }

    //_________________________________________________
    bool WindowManager::finishDrag( GtkWidget* widget )
    {

        _widget = 0L;
        _x = -1;
        _y = -1;

        if( _drag )
        {

            gtk_grab_remove(widget);
            gdk_pointer_ungrab( CurrentTime );
            _drag = false;
            return true;

        } else return false;

    }

    //_________________________________________________
    bool WindowManager::withinWidget( GtkWidget* widget, GdkEventButton* event )
    {

        GdkWindow *window( gtk_widget_get_parent_window( widget ) );

        // Some widgets aren't realized (GeditWindow for exemple) ...
        if( !window ) return true;

        GtkAllocation allocation;
        #if GTK_CHECK_VERSION(2, 18, 0)
        gtk_widget_get_allocation( widget, &allocation );
        #else
        allocation = widget->allocation;
        #endif

        // Need to get absolute coordinates
        int nx(0);
        int ny(0);
        gdk_window_get_origin(window, &nx, &ny );
        allocation.x += nx;
        allocation.y += ny;

        return Gtk::gdk_rectangle_contains( &allocation, int(event->x_root), int(event->y_root) );

    }

    //_________________________________________________
    bool WindowManager::useEvent( GtkWidget* widget, GdkEventButton* event )
    {

        if( _mode == Disabled ) return false;
        else if( _mode == Minimal && !( GTK_IS_TOOLBAR( widget ) || GTK_IS_MENU_BAR( widget ) ) ) return false;

        bool usable( true );
        if( GTK_IS_NOTEBOOK( widget ) )
        {
            // check if there is an hovered tab
            if( Style::instance().animations().tabWidgetEngine().hoveredTab( widget ) != -1 )
            { usable = false; }

        } else if(GTK_IS_CONTAINER( widget ) ) {

            // need to check all children that may be listening to event
            GList *containers(0L);
            containers = g_list_prepend( containers, widget );

            while( g_list_length( containers ) )
            {

                GtkContainer *c = GTK_CONTAINER( g_list_nth_data(containers, 0) );
                GList *children = gtk_container_get_children( GTK_CONTAINER( c ) );
                for( GList *child = g_list_first( children ); child && usable ; child = g_list_next( child ) )
                {

                    // check child
                    if( !GTK_IS_WIDGET( child->data ) ) continue;

                    if( GTK_IS_CONTAINER( child->data ) )
                    { containers = g_list_prepend( containers, child->data ); }

                    GtkWidget* childWidget( GTK_WIDGET( child->data ) );
                    if( gtk_widget_get_state( childWidget ) == GTK_STATE_PRELIGHT ) {

                        // if widget is prelight, we don't need to check where event happen,
                        // any prelight widget indicate we can't do a move
                        usable = false;

                    } else if(
                        !GTK_IS_NOTEBOOK ( childWidget ) &&
                        event &&
                        withinWidget( childWidget, event ) )
                    {

                        GdkWindow *window = gtk_widget_get_window( childWidget );
                        if( window && gdk_window_is_visible ( window ) )
                        {

                            // TODO: one could probably check here whether widget is enabled or not,
                            // and accept if widget is disabled.

                            if( gtk_widget_get_events ( childWidget ) & GDK_BUTTON_PRESS_MASK )
                            {

                                // widget listening to press event
                                usable = false;

                            } else if(
                                GTK_IS_MENU_ITEM( G_OBJECT( child->data ) ) ||
                                GTK_IS_SCROLLED_WINDOW( G_OBJECT( child->data ) ) )
                            {
                                // deal with menu item, GtkMenuItem only listen to
                                // GDK_BUTTON_PRESS_MASK when state == GTK_STATE_PRELIGHT
                                // so previous check are invalids :(
                                //
                                // same for ScrolledWindow, they do not send motion events
                                // to parents so not usable
                                usable = false;

                            }

                        }
                    }
                }

                if( children ) g_list_free( children );
                containers = g_list_remove( containers, c );
            }
        }
        return usable;
    }

    //________________________________________________________________________________
    void WindowManager::connect( GtkWidget* widget, WindowManager::Data& data )
    {
        data._destroyId.connect( G_OBJECT( widget ), "destroy", G_CALLBACK( wmDestroy ), this );
        data._styleId.connect( G_OBJECT( widget ), "style-set", G_CALLBACK( wmStyleSet ), this );

        data._pressId.connect( G_OBJECT( widget ), "button-press-event", G_CALLBACK( wmButtonPress ), this );
        data._releaseId.connect( G_OBJECT( widget ), "button-release-event", G_CALLBACK( wmButtonRelease ), this );
        data._leaveId.connect( G_OBJECT( widget ), "leave-notify-event", G_CALLBACK( wmLeave ), this );
        data._motionId.connect( G_OBJECT( widget ), "motion-notify-event", G_CALLBACK( wmMotion ), this );
    }

    //_________________________________________________
    void WindowManager::Data::disconnect( GtkWidget* )
    {

        _leaveId.disconnect();
        _destroyId.disconnect();
        _pressId.disconnect();
        _releaseId.disconnect();
        _motionId.disconnect();
        _styleId.disconnect();

    }

}