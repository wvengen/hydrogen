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

#include <hydrogen/sound_basic/pattern_list.h>

namespace H2Core
{

const char* PatternList::__class_name = "PatternList";

PatternList::PatternList() : Object( __class_name ) { }

PatternList::~PatternList() {
    for( int i=0; i<__patterns.size(); i++ ) delete __patterns[i];
}

PatternList::PatternList( PatternList* other) : Object( __class_name ) {
    assert( __patterns.size() == 0 );
    for ( int i=0; i<other->size(); i++ )
        (*this) << (*other)[i];
        //(*this) << ( new Pattern( (*other)[i] ) );    // TODO should we ?!
}

void PatternList::operator<<( Pattern* pattern ) {
    // do nothing if already in __patterns
    for( int i=0; i<__patterns.size(); i++ ) if(__patterns[i]==pattern) return;
	__patterns.push_back( pattern );
}

void PatternList::add( Pattern* pattern ) {
    // do nothing if already in __patterns
    for( int i=0; i<__patterns.size(); i++ ) if(__patterns[i]==pattern) return;
	__patterns.push_back( pattern );
}

Pattern* PatternList::operator[]( int idx ) {
    if ( idx < 0 || idx >= __patterns.size() ) {
		ERRORLOG( QString( "idx %1 out of [0;%2]" ).arg( idx ).arg(__patterns.size()) );
		return 0;
	}
	assert( idx >= 0 && idx < __patterns.size() );
	return __patterns[idx];
}

Pattern* PatternList::get( int idx ) {
    if ( idx < 0 || idx >= __patterns.size() ) {
		ERRORLOG( QString( "idx %1 out of [0;%2]" ).arg( idx ).arg(__patterns.size()) );
		return 0;
	}
	assert( idx >= 0 && idx < __patterns.size() );
	return __patterns[idx];
}

Pattern* PatternList::del( int idx ) {
	assert( idx >= 0 && idx < __patterns.size() );
    Pattern *pattern = __patterns[idx];
	__patterns.erase( __patterns.begin() + idx );
    return pattern;
}

Pattern* PatternList::del( Pattern* pattern ) {
    for( int i=0; i<__patterns.size(); i++ ) {
        if(__patterns[i]==pattern) {
	        __patterns.erase( __patterns.begin() + i );
            return pattern;
        }
    }
    return 0;
}

int PatternList::index( Pattern* pattern ) {
    for(int i=0; i<__patterns.size(); i++) {
        if (__patterns[i]==pattern) return i;
    }
    return -1;
}

void PatternList::set_to_old() {
	for ( int i=0 ; i<__patterns.size() ; i++ ) get(i)->set_to_old();
}

void PatternList::swap( int idx_a, int idx_b ) {
	assert( idx_a >= 0 && idx_a < __patterns.size() );
	assert( idx_b >= 0 && idx_b < __patterns.size() );
    if( idx_a == idx_b ) return;
    //DEBUGLOG(QString("===>> SWAP  %1 %2").arg(idx_a).arg(idx_b) );
    Pattern *tmp = __patterns[idx_a];
    __patterns[idx_a] = __patterns[idx_b];
    __patterns[idx_b] = tmp;
}

void PatternList::move( int idx_a, int idx_b ) {
	//assert( idx_a >= 0 && idx_a < __patterns.size() );
	//assert( idx_b >= 0 && idx_b < __patterns.size() );
    if( idx_a == idx_b ) return;
    //DEBUGLOG(QString("===>> MOVE  %1 %2").arg(idx_a).arg(idx_b) );
    Pattern *tmp = __patterns[idx_a];
    __patterns.erase( __patterns.begin() + idx_a );
	__patterns.insert( __patterns.begin() + idx_b, tmp );
}

};
