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

#ifndef H2_NOTE_KEY_H
#define H2_NOTE_KEY_H

#include <QtCore/QString>

namespace H2Core
{

/**
 * \brief NoteKey is nothing more than a key and an octave
 */
class NoteKey
{
    public:
        enum Key {
		    C = 0,
		    Cs,
		    D,
		    Ef,
		    E,
		    F,
		    Fs,
		    G,
		    Af,
		    A,
		    Bf,
		    B,
	    };
        Key m_key;              ///< the key
        int m_nOctave;          ///< the octave

        /** \brief constructor */
        NoteKey() { m_key = C; m_nOctave = 0; }
        /** \brief copy constructor */
        NoteKey( const NoteKey& key ) { m_key = key.m_key; m_nOctave = key.m_nOctave; }

        /**
         * \brief converts a string into a NoteKey
         * \param str the string to convert
         */
	    static NoteKey string_to_key( const QString& str );
        /**
         * \brief convert a NoteKey into a string
         * \param key the NoteKey to convert
         */
	    static QString key_to_string( NoteKey key );
};

};

#endif // H2_NOTE_KEY_H
