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

#include <hydrogen/basics/pattern_list.h>

#include <hydrogen/helpers/xml.h>
#include <hydrogen/basics/pattern.h>

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
        (*this) << ( new Pattern( (*other)[i] ) );
}

PatternList* PatternList::load_from( XMLNode* node, InstrumentList* instruments ) {
    PatternList *patterns = new PatternList();
    XMLNode pattern_node = node->firstChildElement( "pattern" );
    while ( !pattern_node.isNull() ) {
        Pattern *pattern = Pattern::load_from( &pattern_node, instruments );
        (*patterns) << pattern;
        pattern_node = pattern_node.nextSiblingElement( "pattern" );
    }
    return patterns;
}

void PatternList::load_virtuals_from( XMLNode* node ) {
    XMLNode virtual_pattern_node = node->firstChildElement( "pattern" );
    while ( !virtual_pattern_node.isNull() ) {
        QString name = virtual_pattern_node.read_string( "name", "unknown", false, false );
        Pattern *pattern = find( name );
        if( pattern ) {
            XMLNode virtual_node = node->firstChildElement( "virtual" );
		    while ( !virtual_node.isNull() ) {
			    QString virtual_name = virtual_node.firstChild().nodeValue();
                Pattern *virtual_pattern = find( virtual_name );
                if( virtual_pattern ) {
			        pattern->add_virtual_pattern( virtual_pattern );
                } else {
		            ERRORLOG( QString("Virtual pattern (data) %1 does not belong to pattern list)").arg( virtual_name ) );
                }
            }
        } else {
		    ERRORLOG( QString("Virtual pattern (virtual) %1 does not belong to pattern list)").arg( name ) );
        }
        virtual_pattern_node = virtual_pattern_node.nextSiblingElement( "pattern" );
    }
}

void PatternList::save_to( XMLNode* node ) {
    for ( int i = 0; i < size(); i++ ) {
        XMLNode pattern_node = XMLDoc().createElement( "pattern" );
        (*this)[i]->save_to( &pattern_node );
        node->appendChild( pattern_node );
    }
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

void PatternList::insert( int idx, Pattern* pattern ) {
    // do nothing if already in __patterns
    for( int i=0; i<__patterns.size(); i++ ) if(__patterns[i]==pattern) return;
	__patterns.insert( __patterns.begin() + idx, pattern );
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
	for ( int i=0 ; i<__patterns.size() ; i++ ) __patterns[i]->set_to_old();
}

Pattern*  PatternList::find( const QString& name ) {
    for(int i=0; i<__patterns.size(); i++)
        if (__patterns[i]->get_name()==name) return __patterns[i];
    return 0;
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

void PatternList::compute_flattened_virtual_patterns() {
	for ( int i=0 ; i<__patterns.size() ; i++ ) __patterns[i]->clear_flattened_virtual_patterns();
	for ( int i=0 ; i<__patterns.size() ; i++ ) __patterns[i]->compute_flattened_virtual_patterns();
}

void PatternList::del_virtual_pattern( Pattern* pattern ) {
	for( int i=0; i<__patterns.size(); i++ ) __patterns[i]->del_virtual_pattern( pattern );
}

};

/* vim: set softtabstop=4 expandtab: */
