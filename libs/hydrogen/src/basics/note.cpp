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

#include <hydrogen/basics/note.h>
#include <hydrogen/basics/note_key.h>
#include <hydrogen/basics/instrument.h>

#include <cassert>
//#include <cstdlib>

namespace H2Core
{

const char* Note::__class_name = "Note";

Note::Note(
        Instrument *pInstrument,
        unsigned position,
        float velocity,
        float fPan_L,
        float fPan_R,
        int nLength,
        float fPitch,
        NoteKey key
)
    : Object( __class_name )
    , m_fSamplePosition( 0.0 )
    , m_nHumanizeDelay( 0 )
    , m_noteKey( key )
    , m_fCutoff( 1.0 )
    , m_fResonance( 0.0 )
    , m_fBandPassFilterBuffer_L( 0.0 )
    , m_fBandPassFilterBuffer_R( 0.0 )
    , m_fLowPassFilterBuffer_L( 0.0 )
    , m_fLowPassFilterBuffer_R( 0.0 )
    , m_bJustRecorded( false )
    , __position( position )
    , __velocity( velocity )
    , __leadlag( 0.0 )
    , __noteoff( false)
    , __midimsg1(-1)
    , __pat_Id( 0 )
{
    set_pan_l( fPan_L );
    set_pan_r( fPan_R );
    set_length( nLength );
    set_instrument( pInstrument );
    set_pitch( fPitch );
}

Note::Note( const Note* pNote ) : Object( __class_name ) {
    __position = pNote->get_position();
    __velocity = pNote->get_velocity();
    set_pan_l( pNote->get_pan_l() );
    set_pan_r( pNote->get_pan_r() );
    set_leadlag( pNote->get_leadlag() );
    set_length( pNote->get_length() );
    set_pitch( pNote->get_pitch() );
    set_noteoff( pNote->get_noteoff() );
    set_midimsg1( pNote->get_midimsg1() );
    set_ID( pNote->get_ID() );
    m_noteKey = pNote->m_noteKey;
    m_fCutoff = pNote->m_fCutoff;
    m_fResonance = pNote->m_fResonance;
    m_fBandPassFilterBuffer_L = pNote->m_fBandPassFilterBuffer_L;
    m_fBandPassFilterBuffer_R = pNote->m_fBandPassFilterBuffer_R;
    m_fLowPassFilterBuffer_L = pNote->m_fLowPassFilterBuffer_L;
    m_fLowPassFilterBuffer_R = pNote->m_fLowPassFilterBuffer_R;
    m_nHumanizeDelay = pNote->m_nHumanizeDelay;
    m_fSamplePosition = pNote->m_fSamplePosition;
    m_bJustRecorded = pNote->m_bJustRecorded;
    set_instrument( pNote->__instrument );
}

Note::~Note() { }

void Note::set_instrument( Instrument* instrument ) {
    if ( instrument == 0 ) return;
    instrument = instrument;
    assert( __instrument->get_adsr() );
    m_adsr = ADSR( *( __instrument->get_adsr() ) );
}

void Note::dump() {
    INFOLOG( QString("pos: %1\t humanize offset%2\t instr: %3\t key: %4\t pitch: %5")
            .arg( get_position() )
	        .arg( m_nHumanizeDelay )
	        .arg( __instrument->get_name() )
	        .arg( NoteKey::key_to_string( m_noteKey ) )
	        .arg( get_pitch() )
	        .arg( get_noteoff() )
            );
}

Note* Note::copy() {
    Note* note = new Note(
            get_instrument(),
	        get_position(),
	        get_velocity(),
	        get_pan_l(),
	        get_pan_r(),
	        get_length(),
	        get_pitch(),
	        m_noteKey
            );
    note->set_leadlag(get_leadlag());
    return note;
}


};
