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

#include <hydrogen/sound_basic/instrument_list.h>

namespace H2Core
{

const char* InstrumentList::__class_name = "InstrumentList";

InstrumentList::InstrumentList()
		: Object( __class_name )
{
}

InstrumentList::InstrumentList( InstrumentList* other) : Object( __class_name ) {
    assert( __instruments.size() == 0 );
    for ( int i=0; i<other->size(); i++ )
        (*this) << ( new Instrument( (*other)[i] ) );
}

InstrumentList::~InstrumentList() {
	for ( int i = 0; i < __instruments.size(); ++i )
        delete __instruments[i];
}

InstrumentList* InstrumentList::load_from( XMLNode* node ) {
    XMLNode instrument_node = node->firstChildElement( "instrument" );
    InstrumentList *instruments = new InstrumentList();
    int count = 0;
    while ( !instrument_node.isNull() ) {
        count++;
        if ( count > MAX_INSTRUMENTS ) {
            ERRORLOG( QString("instrument count >= %2, stop reading instruments").arg(MAX_INSTRUMENTS) );
            break;
        }
        Instrument* instrument = Instrument::load_from( &instrument_node );
        if(instrument) {
            (*instruments) << instrument;
        } else {
            ERRORLOG( QString("Empty ID for instrument %1. The drumkit is corrupted. Skipping instrument").arg(count) );
            count--;
        }
        instrument_node = instrument_node.nextSiblingElement( "instrument" );
    }
    return instruments;
}

void InstrumentList::save_to( XMLNode* node ) {
    for ( int i = 0; i < size(); i++ ) {
        XMLNode instrument_node = XMLDoc().createElement( "instrument" );
        (*this)[i]->save_to( &instrument_node );
        node->appendChild( instrument_node );
    }
}

void InstrumentList::operator<<( Instrument* instrument ) {
	__instruments.push_back( instrument );
}

void InstrumentList::add( Instrument* instrument ) {
	__instruments.push_back( instrument );
}

Instrument* InstrumentList::operator[]( int idx ) {
	assert( idx >= 0 && idx < __instruments.size() );
	/*
    if ( idx >= __instruments.size() ) {
		ERRORLOG( QString( "idx %1 > size() %2" ).arg( idx ).arg(size()) );
		return 0;
	}
    */
	return __instruments[idx];
}

Instrument* InstrumentList::get( int idx ) {
	assert( idx >= 0 && idx < __instruments.size() );
	/*
    if ( idx >= __instruments.size() ) {
		ERRORLOG( QString( "idx %1 > size() %2" ).arg( idx ).arg(size()) );
		return 0;
	}
    */
	return __instruments[idx];
}

int InstrumentList::index( Instrument* instr ) {
    for(int i=0; i<__instruments.size(); i++)
        if (__instruments[i]==instr) return i;
    return -1;
}

Instrument*  InstrumentList::find( const QString& name ) {
    for(int i=0; i<__instruments.size(); i++)
        if (__instruments[i]->get_name()==name) return __instruments[i];
    return 0;
}

void InstrumentList::del( int idx ) {
	assert( idx >= 0 && idx < __instruments.size() );
	__instruments.erase( __instruments.begin() + idx );
}

void InstrumentList::swap( int idx_a, int idx_b ) {
	assert( idx_a >= 0 && idx_a < __instruments.size() );
	assert( idx_b >= 0 && idx_b < __instruments.size() );
    if( idx_a == idx_b ) return;
    //DEBUGLOG(QString("===>> SWAP  %1 %2").arg(idx_a).arg(idx_b) );
    Instrument *tmp = __instruments[idx_a];
    __instruments[idx_a] = __instruments[idx_b];
    __instruments[idx_b] = tmp;
}

void InstrumentList::move( int idx_a, int idx_b ) {
	assert( idx_a >= 0 && idx_a < __instruments.size() );
	assert( idx_b >= 0 && idx_b < __instruments.size() );
    if( idx_a == idx_b ) return;
    //DEBUGLOG(QString("===>> MOVE  %1 %2").arg(idx_a).arg(idx_b) );
    Instrument *tmp = __instruments[idx_a];
    __instruments.erase( __instruments.begin() + idx_a );
    if ( idx_a > idx_b ) {
	    __instruments.insert( __instruments.begin() + idx_b, tmp );
    } else {
	    __instruments.insert( __instruments.begin() + idx_b-1, tmp );
    }
}

};

