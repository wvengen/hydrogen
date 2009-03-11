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

#include "SeqSong.h"
#include "SeqScript.h"
#include "SeqEvent.h"

#include <hydrogen/Song.h>
#include <hydrogen/TransportPosition.h>
#include <hydrogen/hydrogen.h>
#include <hydrogen/note.h>
#include <hydrogen/Pattern.h>

#include "transport/songhelpers.h"

using namespace H2Core;
using namespace std;

SeqSong::SeqSong()
{
}

SeqSong::~SeqSong()
{
}

int SeqSong::process(SeqScript& seq,
                     const TransportPosition& pos,
                     uint32_t nframes)
{
    Song* pSong = Hydrogen::get_instance()->getSong();
    TransportPosition cur;
    uint32_t end_frame = pos.frame + nframes;  // 1 past end of this process() cycle
    uint32_t this_tick;
    Note* pNote;
    SeqEvent ev;
    uint32_t pat_grp;
    PatternList* patterns;
    Pattern::note_map_t::const_iterator n;
    int k;
    uint32_t default_note_length, length;

    // See below "DEAD CODE" for a start.
    #warning "NEED TO IMPLEMENT LEAD/LAG AND HUMANIZE FEATURE"

    cur = pos;
    cur.ceil(TransportPosition::TICK);
    // Default note length is 1/2 beat (i.e. an 8th note)
    default_note_length = cur.frames_per_tick() * cur.ticks_per_beat / 2;

    while( cur.frame < end_frame ) {
        this_tick = cur.tick_in_bar();
        pat_grp = pattern_group_index_for_bar(pSong, pos.bar);
        patterns = pSong->get_pattern_group_vector()->at(pat_grp);

        for( k=0 ; unsigned(k) < patterns->get_size() ; ++k ) {
            for( n = patterns->get(k)->note_map.begin() ;
                 n != patterns->get(k)->note_map.end() ;
                 ++n ) {
                if( n->first != this_tick ) continue;
                pNote = n->second;
                ev.frame = cur.frame;
                ev.type = SeqEvent::NOTE_ON;
                ev.channel = 0;
                #warning "get_pitch() returns a float.  Need to sort out the"
                #warning "whole pitch/instrument thing."
                ev.note = pNote->get_pitch();
                ev.velocity = pNote->get_velocity();
                ev.pan_l = pNote->get_pan_l();
                ev.pan_r = pNote->get_pan_r();
                if( pNote->get_length() < 0 ) {
                    length = default_note_length;
                } else {
                    length = unsigned(pNote->get_length()) * cur.frames_per_tick();
                }
                seq.insert_note(ev, length);
            }
        }
        ++cur;
    }

    return 0;
}

#if 0

//////////////////////////////////////////////////////////////////////
/////////// BEGIN DEAD CODE //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/* For the initial implementation, this is where I was with getting the
 * lookahead code to work.  I abandonded it for now so that I could get
 * something simple working.  When adding back in the lookahead, this
 * would be a good place to start.  - gabriel
 *
 * It requires SeqSong to have two private members:
 *
    private:
        // These two are used to keep track of how much we have sequenced up to
        // now.
        Song* m_song_cached;
        TransportPosition m_pos_cached;        

 */

virtual int SeqSong::process(SeqScript& seq,
                             const TransportPosition& pos,
                             uint32_t nframes)
{
    Song* pSong = Hydrogen::get_instance()->getSong();
    TransportPosition left, right;
    bool include_left_tick;
    float leadlag_ticks = 5.0; // ticks
    float humanize_time = 0.050; // seconds

    uint32_t lookahead_frames = round(
        float(pos.frame_rate)
        * (humanize_time
           + (leadlag_ticks / float(pos.ticks_per_beat) / float(pos.beats_per_minute) * 60.0))
        );

    // Detect relocations and take any necc. actions.
    if( (pSong != m_song_cached) || pos.new_position ) {
        #warning "What *DO* we need to do if the song changes?"
        #warning "What *DO* we need to do when the transport relocates?"
        // Temporary code:
        left = pos;
        right = pos;
        include_left_tick = true;
    } else {
        left = m_pos_cached;
        right = pos;
        include_left_tick = false;
    }

    // Figure out if any future ticks could overlap this process() cycle because
    // of lead/lag or humanize.  If any overlap... schedule that whole tick.
    // right = foo();

    // For every tick after left, and through right... schedule the events:


    // Update the cached info
    m_song_cached = pSong;
    m_pos_cached = right;

    return 0;
}

//////////////////////////////////////////////////////////////////////
/////////// END DEAD CODE ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#endif // 0
