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

#include <hydrogen/sound_basics/pattern.h>

#include <hydrogen/Song.h>
#include <hydrogen/note.h>
#include <hydrogen/audio_engine.h>

#include <vector>
#include <cassert>
namespace H2Core
{

const char* Pattern::__class_name = "Pattern";

Pattern::Pattern( const QString& name, const QString& category, int length )
    : Object( __class_name ),
    __length( length ),
    __category( category ),
    __name( name )
{
}

Pattern::~Pattern() {
    for( notes_it_t it=__notes.begin(); it!=__notes.end(); ++it ) {
        Note *pNote = it->second;
        delete pNote;
    }
}

void Pattern::purge_instrument( Instrument * I ) {
    bool locked = false;
    std::list< Note* > slate;
    notes_it_t it = __notes.begin();
    while ( it!=__notes.end() ) {
        Note *pNote = it->second;
        assert( pNote );
        if ( pNote->get_instrument() == I ) {
            if ( !locked ) {
                H2Core::AudioEngine::get_instance()->lock( RIGHT_HERE );
                locked = true;
            }
            slate.push_back( pNote );
            __notes.erase( it++ );
        } else {
            ++it;
        }
    }
    if ( locked ) {
        H2Core::AudioEngine::get_instance()->unlock();
        while ( slate.size() ) {
            delete slate.front();
            slate.pop_front();
        }
    }
}

bool Pattern::references_instrument( Instrument * I ) {
    for( notes_cst_it_t it=__notes.begin(); it!=__notes.end(); ++it ) {
        Note *pNote = it->second;
        assert( pNote );
        if ( pNote->get_instrument() == I ) {
            return true;
        }
    }
    return false;
}

void Pattern::set_to_old() {
    for( notes_cst_it_t it=__notes.begin(); it!=__notes.end(); ++it ) {
        Note *pNote = it->second;
        assert( pNote );
        pNote->m_bJustRecorded = false ;
    }
}

Pattern* Pattern::get_empty_pattern() {
    Pattern *pat = new Pattern( "Pattern", "not_categorized" );
    return pat;
}

Pattern* Pattern::copy() {
    Pattern *newPat = new Pattern( __name, __category );
    // TODO newPat->set_length( get_length() );
    for( notes_cst_it_t it=__notes.begin(); it!=__notes.end(); ++it ) {
        Note *pNote = new Note( it->second );
        newPat->__notes.insert( std::make_pair( it->first, pNote ) );
    }
    return newPat;
}

void Pattern::copy_virtual_patterns_to_transitive_closure( ) {
    __virtual_patterns_transitive_closure.clear();
    for( virtual_patterns_cst_it_t it = __virtual_patterns.begin(); it!=__virtual_patterns.end(); ++it ) {
        __virtual_patterns_transitive_closure.insert( *it);
    }
}

void Pattern::debug_dump() {
    INFOLOG( "Pattern dump" );INFOLOG( "Pattern name: " + __name );
    INFOLOG( "Pattern category: " + __category );
    INFOLOG( QString("Pattern length: %1").arg( __length ) );
}

};

/* vim: set softtabstop=4 expandtab: */
