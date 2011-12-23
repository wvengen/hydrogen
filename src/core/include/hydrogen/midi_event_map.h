/*
 * Hydrogen
 * Copyright(c) 2002-2008 by Alex >Comix< Cominu [comix@users.sourceforge.net]
 *
 * http://www.hydrogen-music.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY, without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#ifndef MIDI_EVENT_MAP_H
#define MIDI_EVENT_MAP_H

#include <map>
#include <cassert>
#include <hydrogen/object.h>

#include <QtCore/QMutex>

#define H2_INVALID_MIDI_NOTE 1024

class MidiEventMap : public H2Core::Object
{
    H2_OBJECT
	public:
                typedef std::map< int, int > map_t;
                ~MidiEventMap();

		static void create_instance();

		void reset();  // Reinitializes the object.


                void registerNoteMapping( int , int );
                int getNoteMapping( int note );
		void setupNoteArray();

	private:
                MidiEventMap();

                int __note_array[ 128 ];

		QMutex __mutex;
};
#endif
