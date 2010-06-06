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

#ifndef H2_NOTE_H
#define H2_NOTE_H

#include <cassert>
#include <hydrogen/Object.h>
#include <hydrogen/adsr.h>
#include <hydrogen/basics/instrument.h>

namespace H2Core
{

class ADSR;
class Instrument;

/**
 * \brief A note plays an associated instrument with a velocity left and right pan
 */
class Note : public Object
{
    H2_OBJECT
    public:
        /** \brief possible keys */
        enum Key { C = 0, Cs, D, Ef, E, F, Fs, G, Af, A, Bf, B };
        /**
         * \brief constructor
         * \param instrument the instrument played by this note
         * \param position the position of the note within the pattern
         * \param velocity it's velocity
         * \param pan_l left pan
         * \param pan_r right pan
         * \param length it's length
         * \param pitch it's pitch
         */
        Note(
	        Instrument* instrument,
	        int position,
	        float velocity,
	        float pan_l,
	        float pan_r,
	        int length,
	        float pitch
            );

	    /** \brief copy constructor */
	    Note( Note* other );
        /** \brief destructor */
        ~Note();

        /** \brief output details through logger with DEBUG severity */
        void dump();

        /** \brief set the instrument */
        void set_instrument( Instrument* instrument );
        /** \brief get the instrument */
	    Instrument* get_instrument()                    { return __instrument; }
        /** \brief set the position of the note */
	    void set_position( int position )               { __position = position; }
        /** \brief get the position of the note */
	    int get_position() const                        { return __position; }
        /** brief set the velocity of the note */
	    void set_velocity( float velocity ) {
		    if ( velocity > 1.0 ) {
			    velocity = 1.0;
		    } else if ( velocity < 0 ) {
			    velocity = 0;
		    }
		    __velocity = velocity;
	    }
        /** brief get the velocity of the note */
	    float get_velocity() const                      { return __velocity; }
        /** brief set the left pan of the note */
	    void set_pan_l( float pan ) {
		    if ( pan > 0.5 ) {
			    pan = 0.5;
		    }
		    __pan_l = pan;
        }
        /** brief get the left pan of the note */
        float get_pan_l() const                         { return __pan_l; }
        /** brief set the right pan of the note */
	    void set_pan_r( float pan ) {
		    if ( pan > 0.5 ) {
			    pan = 0.5;
		    }
		    __pan_r = pan;
	    }
        /** brief get the right pan of the note */
        float get_pan_r() const                         { return __pan_r; }
        /** brief set the lead lag of the note */
        void set_lead_lag( float lead_lag ) {
		    if(lead_lag > 1.0) {
			    __lead_lag = 1.0;
		    } else if (lead_lag < -1.0) {
			    __lead_lag = -1.0;
		    } else {
			    __lead_lag = lead_lag;
		    }
    	}
        /** brief get the lead lag of the note */
	    float get_lead_lag() const {
		    assert(__lead_lag <=  1.0);
		    assert(__lead_lag >= -1.0);
		    return __lead_lag;
	    }
        
        void set_length( int length )       { __length = length; }          ///< set length of the note
        int get_length() const              { return __length; }            ///< get length of the note
        void set_pitch( float pitch )       { __pitch = pitch; }            ///< set pitch of the note
        float get_pitch() const             { return __pitch; }             ///< get pitch of the note
        void set_note_off( bool note_off )  { __note_off = note_off; }      ///< set note_off of the note
        bool get_note_off() const           { return __note_off; }          ///< get note_off of the note
        //void set_midi_msg( int midi_msg )   { __midi_msg = midi_msg; }      ///< set midi message of the note
        int get_midi_msg() const            { return __midi_msg; }          ///< get midi message of the note
        void set_id( int id )               { __pat_id = id; }              ///< set id of the note
        int get_id() const                  { return __pat_id; }	        ///< get id of the note
        void set_just_recorded( bool val )  { __just_recorded = val; }      ///< set just recorded
        bool get_just_recorded() const      { return __just_recorded; }	    ///< get just recorded

        /** \brief return a string representation of key-actove */
        QString key_to_string();
        /**
         * \brief parse str and set __key and __octave
         * \param str the string to be parsed
         */
        void set_key_octave( const QString& str );
        /**
         * \brief set __key and __octave only if within acceptable range
         * \param key the key to set
         * \param octave the octave to be set
         */
        void set_key_octave( int key, int octave )  { if(key>=0 && key<=11)__key=(Key)key; if(octave>=-3 && octave<=3)__octave = octave; }
        /**
         * \brief set __key, __octave and __midi_msg only if within acceptable range
         * \param key the key to set
         * \param octave the octave to be set
         */
        void set_midi_info( int key, int octave, int msg)  { if(key>=0 && key<=11)__key=(Key)key; if(octave>=-3 && octave<=3)__octave = octave; __midi_msg = msg; }

        float get_sample_position()         { return __sample_position; }
        void set_humanize_delay(int delay ) { __humanize_delay = delay; }
        int get_humanize_delay()            { return __humanize_delay; }
        float get_cut_off()                 { return __cut_off; }
        float get_resonance()               { return __resonance; }
        float get_bpfb_l()                  { return __bpfb_l; }
        float get_bpfb_r()                  { return __bpfb_r; }
        float get_lpfb_l()                  { return __lpfb_l; }
        float get_lpfb_r()                  { return __lpfb_r; }
        int get_pat_id()                    { return __pat_id; }
        Key get_key()                       { return __key; }
        int get_octave()                    { return __octave; }

        float release_adsr()                { return __adsr.release(); }
		float get_adsr_value(float v)       { return __adsr.get_value( v ); }

        float update_sample_position( float incr ) { __sample_position += incr; return __sample_position; }

        /** \brief return true if instrument, key and octave matches with internal
         * \param instrument the instrument to match with __instrument
         * \param key the key to match with __key
         * \param octave the octave to match with __octave
         */
        bool match( Instrument* instrument, int key, int octave )   { return ((__instrument==instrument) && (__key==key) && (__octave==octave)); }
        /** \brief return scaled velocity for midi output */
        /*
        int midi_velocity()                 { return __velocity*127; }
        int compute_key()                   { return (__octave +3)*12 + __key + __instrument->get_midi_out_note() - 60; }
        int compute_position( int tick_size ) { return __humanize_delay + (__position * tick_size); }
        */
        /** \brief returns octave*12 + key */
        float get_notekey_pitch()           { return __octave * 12 + __key; }
        /** \brief returns octave*12+key+pitch */
        float get_total_pitch()             { return __octave * 12 + __key + __pitch; }

        /** \brief compute left and right output based on filters */
        void compute_lr_values( float* val_l, float* val_r ) {
            float cut_off = __instrument->get_filter_cutoff();
	        float resonance = __instrument->get_filter_resonance();
			__bpfb_l  = ( resonance * __bpfb_l ) + cut_off * ( *val_l - __lpfb_l );
			__lpfb_l += ( cut_off   * __bpfb_l );
			__bpfb_r  = ( resonance * __bpfb_r ) + cut_off * ( *val_r - __lpfb_r );
			__lpfb_r += ( cut_off   * __bpfb_r );
			*val_l = __lpfb_l;
			*val_r = __lpfb_r;
        }

    private:
	    Instrument* __instrument;   ///< the instrument to be played by this note
	    unsigned __position;		///< note position inside the pattern
	    float __velocity;		    ///< velocity (intensity) of the note [0;1]
        float __pan_l;			    ///< pan of the note (left volume) [0;1]
        float __pan_r;			    ///< pan of the note (right volume) [0;1]
        int __length;               ///< the length of the note
        float __pitch;              ///< the frequency of the note
        Key __key;                  ///< the key, [0;11]==[C;B]
        int __octave;               ///< the octave [-3;3]
        ADSR __adsr;                ///< attack decay sustain release
	    float __lead_lag;		    ///< lead or lag offset of the note
        float __cut_off;		    ///< filter cutoff [0;1]
        float __resonance;	        ///< filter resonant frequency [0;1]
        int __humanize_delay;       ///< used in "humanize" function
        float __sample_position;    ///< place marker for overlapping process() cycles
        float __bpfb_l;		        ///< left band pass filter buffer
        float __bpfb_r;		        ///< right band pass filter buffer
        float __lpfb_l;		        ///< left low pass filter buffer
        float __lpfb_r;		        ///< right low pass filter buffer
        int __pat_id;               ///< TODO
        int __midi_msg;             ///< TODO
	    bool __note_off;			///< note type on|off
        bool __just_recorded;       ///< used in record+delete
        ///< used to build QString from __key an __octave
        static const char *__key_str[];
};

};

#endif
