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

#include <hydrogen/basics/note_key.h>
#include <hydrogen/Object.h>

namespace H2Core
{

NoteKey NoteKey::string_to_key( const QString& str ) {
    NoteKey key;
    int l = str.length();
    QString s_key = str.left( l-1 );
    QString s_oct = str.mid( l-1, l );
    if ( s_key.endsWith( "-" ) ){
        s_key.replace("-", "");
        s_oct.insert( 0, "-");
    }
    key.m_nOctave = s_oct.toInt();
    if ( s_key == "C" ) {
        key.m_key = NoteKey::C;
    } else if ( s_key == "Cs" ) {
        key.m_key = NoteKey::Cs;
    } else if ( s_key == "D" ) {
        key.m_key = NoteKey::D;
    } else if ( s_key == "Ef" ) {
        key.m_key = NoteKey::Ef;
    } else if ( s_key == "E" ) {
        key.m_key = NoteKey::E;
    } else if ( s_key == "F" ) {
        key.m_key = NoteKey::F;
    } else if ( s_key == "Fs" ) {
        key.m_key = NoteKey::Fs;
    } else if ( s_key == "G" ) {
        key.m_key = NoteKey::G;
    } else if ( s_key == "Af" ) {
        key.m_key = NoteKey::Af;
    } else if ( s_key == "A" ) {
        key.m_key = NoteKey::A;
    } else if ( s_key == "Bf" ) {
        key.m_key = NoteKey::Bf;
    } else if ( s_key == "B" ) {
        key.m_key = NoteKey::B;
    } else {
        ___ERRORLOG( "Unhandled key: " + s_key );
    }
    return key;
}

QString NoteKey::key_to_string( NoteKey key ) {
    QString str;
    switch ( key.m_key ) {
        case NoteKey::C:
            str = "C";
            break;
        case NoteKey::Cs:
            str = "Cs";
            break;
        case NoteKey::D:
            str = "D";
            break;
        case NoteKey::Ef:
            str = "Ef";
            break;
        case NoteKey::E:
            str = "E";
            break;
        case NoteKey::F:
            str = "F";
            break;
        case NoteKey::Fs:
            str = "Fs";
            break;
        case NoteKey::G:
            str = "G";
            break;
        case NoteKey::Af:
            str = "Af";
            break;
        case NoteKey::A:
            str = "A";
            break;
        case NoteKey::Bf:
            str = "Bf";
            break;
        case NoteKey::B:
            str = "B";
            break;
    }
    str = str + QString("%1").arg( key.m_nOctave );
    return str;
}

};
