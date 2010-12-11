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

#include <hydrogen/basics/pattern.h>

#include <hydrogen/hydrogen.h>
#include <hydrogen/basics/song.h>
#include <hydrogen/basics/note.h>
#include <hydrogen/audio_engine.h>
#include <hydrogen/basics/pattern_list.h>
#include <hydrogen/helpers/filesystem.h>

#include <vector>
#include <cassert>
namespace H2Core
{

const char* Pattern::__class_name = "Pattern";

Pattern::Pattern( const QString& name, const QString& category, int length )
    : Object( __class_name ),
    __length( length ),
    __name( name ),
    __category( category )
{
}

Pattern::Pattern( Pattern *other)
    : Object( __class_name ),
    __length( other->get_length() ),
    __name( other->get_name() ),
    __category( other->get_category() )
{
    for( notes_cst_it_t it=other->get_notes()->begin(); it!=other->get_notes()->end(); ++it ) {
        Note* note = new Note( it->second );
        __notes.insert( std::make_pair( it->first, note ) );
    }
}

Pattern* Pattern::load_file( const QString& pattern_path ) {
    INFOLOG( QString("Load pattern %1").arg(pattern_path) );
    if ( !Filesystem::file_readable( pattern_path ) ) {
        ERRORLOG( QString("%1 is not a readable pattern file").arg(pattern_path) );
        return 0;
    }
    XMLDoc doc;
    if ( !doc.read( pattern_path, Filesystem::pattern_xsd() ) ) return 0;
    XMLNode root = doc.firstChildElement( "drumkit_pattern" );
    if ( root.isNull() ) {
        ERRORLOG( "drumkit_pattern node not found" );
        return 0;
    }
    XMLNode pattern_node = root.firstChildElement( "pattern" );
    if ( pattern_node.isNull() ) {
        ERRORLOG( "pattern node not found" );
        return 0;
    }
    return load_from( &pattern_node );
}

Pattern* Pattern::load_from( XMLNode* node, InstrumentList* instruments ) {
    Pattern *pattern = new Pattern(
            node->read_string( "name", "unknown", false, false ),
            node->read_string( "category", "unknown", false, false ),
            node->read_int( "size", -1, false, false )
            );
    XMLNode note_list_node = node->firstChildElement( "noteList" );
	if ( !note_list_node.isNull() ) {
        XMLNode note_node = note_list_node.firstChildElement( "note" );
        while ( !note_node.isNull() ) {
            Note *note = Note::load_from( &note_node, instruments );
            if(note) {
                pattern->get_notes()->insert( std::make_pair( note->get_position(), note ) );
            }
            note_node = note_node.nextSiblingElement( "note" );
        }
    }
    return pattern;
}

void Pattern::save_to( XMLNode* node ) {
    node->write_string( "name", __name );
    node->write_string( "category", __category );
    node->write_int( "size", __length );
    XMLNode note_list_node = XMLDoc().createElement( "noteList" );
    for( notes_it_t it=__notes.begin(); it!=__notes.end(); ++it ) {
        Note* note = it->second;
        if(note) {
            XMLNode note_node = XMLDoc().createElement( "note" );
            note->save_to( &note_node );
            note_list_node.appendChild( note_node );
        }
    }
    node->appendChild( note_list_node );
}

Pattern::~Pattern() {
    for( notes_it_t it=__notes.begin(); it!=__notes.end(); ++it ) {
        Note* note = it->second;
        delete note;
    }
}

void Pattern::purge_instrument( Instrument* I ) {
    bool locked = false;
    std::list< Note* > slate;
    notes_it_t it = __notes.begin();
    while ( it!=__notes.end() ) {
        Note *note = it->second;
        assert( note );
        if ( note->get_instrument() == I ) {
            if ( !locked ) {
                H2Core::AudioEngine::get_instance()->lock( RIGHT_HERE );
                locked = true;
            }
            slate.push_back( note );
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
        Note *note = it->second;
        assert( note );
        if ( note->get_instrument() == I ) {
            return true;
        }
    }
    return false;
}

void Pattern::set_to_old() {
    for( notes_cst_it_t it=__notes.begin(); it!=__notes.end(); ++it ) {
        Note *note = it->second;
        assert( note );
        note->set_just_recorded( false );
    }
}

Pattern* Pattern::get_empty_pattern() {
    Pattern *pat = new Pattern( "Pattern", "not_categorized" );
    return pat;
}

Note* Pattern::find_note( int idx, Instrument* instrument, Note::Key key, int octave, bool strict ) {
    if (strict) {
        for( notes_cst_it_t it=__notes.lower_bound(idx); it!=__notes.upper_bound(idx); ++it ) {
            Note* note = it->second;
            if (note->match( instrument, key, octave )) return note;
        }
    } else {
        // TODO maybe not start from 0 but idx-X
        for ( int n=0; n<idx; n++ ) {
            for( notes_cst_it_t it=__notes.lower_bound(n); it!=__notes.upper_bound(n); ++it ) {
				Note *note = it->second;
                if (note->match( instrument, key, octave ) && ( (idx<=note->get_position()+note->get_length()) && idx>=note->get_position() ) ) return note;
            }
        }
    }
    return 0;
}

void Pattern::remove_note( Note* note ) {
    notes_it_t it=__notes.begin();
    for( ; it!=__notes.end(); ++it ) {
        if(it->second==note) break;
    }
    if( it!=__notes.end() ) __notes.erase( it );
}
/*
void Pattern::copy_virtual_patterns_to_transitive_closure( ) {
    __virtual_patterns_transitive_closure.clear();
    for( virtual_patterns_cst_it_t it = __virtual_patterns.begin(); it!=__virtual_patterns.end(); ++it ) {
        __virtual_patterns_transitive_closure.insert( *it);
    }
}
*/

void Pattern::del_virtual_pattern( Pattern* pattern ) {
    virtual_patterns_cst_it_t it = __virtual_patterns.find(pattern);
    if ( it!=__virtual_patterns.end() ) __virtual_patterns.erase(it);
}

void Pattern::compute_flattened_virtual_patterns() {
    // __flattened_virtual_patterns must have been clear before
    if( __flattened_virtual_patterns.size() >= __virtual_patterns.size() ) return;
    // for each virtual pattern
    for( virtual_patterns_cst_it_t it0=__virtual_patterns.begin(); it0!=__virtual_patterns.end(); ++it0) {
        __flattened_virtual_patterns.insert(*it0);          // add it
        (*it0)->compute_flattened_virtual_patterns();       // build it's flattened virtual patterns set
        // for each pattern of it's flattened virtual patern set
        for( virtual_patterns_cst_it_t it1=(*it0)->get_flattened_virtual_patterns()->begin(); it1!=(*it0)->get_flattened_virtual_patterns()->end(); ++it1) {
            // add the pattern
            __flattened_virtual_patterns.insert( *it1 );
        }
    }
}

void Pattern::extand_with_flattened_virtual_patterns( PatternList* patterns ) {
    for( virtual_patterns_cst_it_t it=__flattened_virtual_patterns.begin(); it!=__flattened_virtual_patterns.end(); ++it) patterns->add( *it );
}

void Pattern::dump() {
    DEBUGLOG( "Pattern : " + __name );
    DEBUGLOG( "  category: " + __category );
    DEBUGLOG( QString("  length: %1").arg( __length ) );
    DEBUGLOG( "  direct virtual patterns : " );
    for( virtual_patterns_cst_it_t it = __virtual_patterns.begin(); it!=__virtual_patterns.end(); ++it ) { DEBUGLOG( "  "+(*it)->get_name() ); }
    DEBUGLOG( "  flattened virtual patterns : " );
    for( virtual_patterns_cst_it_t it = __flattened_virtual_patterns.begin(); it!=__flattened_virtual_patterns.end(); ++it ) { DEBUGLOG( "  " + (*it)->get_name() ); }
}
};

/* vim: set softtabstop=4 expandtab: */
