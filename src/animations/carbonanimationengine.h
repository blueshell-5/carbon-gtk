#ifndef carbonanimationengine_h
#define carbonanimationengine_h
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

#include "../carbonapplicationname.h"

namespace Carbon
{

    class AnimationEngine
    {

        public:

        //! constructor
        explicit AnimationEngine( void ):
            _duration( 150 )
        {}

        //! destructor
        virtual ~AnimationEngine( void )
        {}

        //! application name
        virtual void setApplicationName( const ApplicationName& applicationName )
        { _applicationName = applicationName; }

        //! duration
        const int& duration( void ) const
        { return _duration; }

        //! duration
        /*! returns true if changed */
        bool setDuration( int value )
        {
            if( _duration == value ) return false;
            _duration = value;
            return true;
        }

        protected:

        //! returs true if widget is black listed (based notably on application name)
        bool widgetIsBlackListed( GtkWidget* widget ) const
        {

            // for now, only Mozilla applications need blacklisting
            return _applicationName.isXul( widget );

        }

        private:

        //! application name
        ApplicationName _applicationName;

        //! animation duration
        int _duration;

    };
}

#endif
