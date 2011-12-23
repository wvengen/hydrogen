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

#include <hydrogen/midi_action.h>
#include <hydrogen/midi_action_map.h>

#include <map>

#include <QMutexLocker>


/**
* @class MidiEventMap
*
* @brief The MidiEventMap maps MidiEvents to MidiEvents
*
*
* The MidiEventMap stores the mapping between midi events
* and midi evens. This maps stores only NOTE events, since
* other events can be handled by the midi action mapper directly.
*
* @author Sebastian Moors
*
*/


MidiEventMap * MidiEventMap::__instance = 0;
const char* MidiEventMap::__class_name = "MidiEventMap";

MidiEventMap::MidiEventMap()
 : Object( __class_name )
{
	__instance = this;
	QMutexLocker mx(&__mutex);

	for(int note = 0; note < 128; note++ ) {
                __note_array[ note ] = 256;
	}
}

MidiEventMap::~MidiEventMap()
{
        QMutexLocker mx( &__mutex );

	for( int i = 0; i < 128; i++ ) {
		delete __note_array[ i ];
	}

	__instance = NULL;
}

void MidiEventMap::create_instance()
{
	if( __instance == 0 ) {
                __instance = new MidiEventMap;
	}
}

void MidiEventMap::reset_instance()
{
	create_instance();
	__instance->reset();
}

void MidiEventMap::reset()
{
        QMutexLocker mx( &__mutex );

	int i;
	for( i = 0 ; i < 128 ; ++i ) {
		delete __note_array[ i ];
                __note_array[ i ] = 256 ;
	}

}



void MidiEventMap::registerNoteMapping( int note, int mappedNote )
{
	QMutexLocker mx(&__mutex);
	if( note >= 0 && note < 128 ) {
		delete __note_array[ note ];
                __note_array[ note ] = mappedNote;
	}
}



