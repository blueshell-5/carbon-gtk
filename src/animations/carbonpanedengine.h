#ifndef carbonpanedengine_h
#define carbonpanedengine_h
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
#include "carbonpaneddata.h"

#include <gtk/gtk.h>
#include <set>

namespace Carbon
{
    //! forward declaration
    class Animations;

    //! stores data associated to vertical and horizontal paned
    class PanedEngine: public GenericEngine<PanedData>
    {

        public:

        //! constructor
        PanedEngine( Animations* widget ):
            GenericEngine<PanedData>( widget )
            {}

        //! destructor
        virtual ~PanedEngine( void )
        {}

    };

}

#endif
