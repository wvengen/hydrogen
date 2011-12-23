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

#ifndef MIDI_TABLE_H
#define MIDI_TABLE_H

#include <hydrogen/object.h>

#include <QtGui>

/*
 * Possible roles for the MidiTable
 * H2_MIDI_EVENT_MAP: map events to events
 * H2_MIDI_ACTION_MAP: map events to actions
 */
enum H2_MIDI_TABLE_ROLE { H2_MIDI_EVENT_MAP, H2_MIDI_ACTION_MAP };

class MidiTable : public QTableWidget, public H2Core::Object
{
    H2_OBJECT
	Q_OBJECT
	public:
		MidiTable( QWidget* pParent );
		~MidiTable();

                void setRole( H2_MIDI_TABLE_ROLE );
		void setupMidiTable();
		void saveMidiTable();
                void insertNewRow( QString, QString, int, int );

	private slots:
		void updateTable();
		void midiSensePressed( int );
	
	private:
                H2_MIDI_TABLE_ROLE __role;
		int __row_count;
		int currentMidiAutosenseRow;
		QSignalMapper *signalMapper;
		QTimer* m_pUpdateTimer;

};

#endif
