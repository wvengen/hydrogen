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

#ifndef H2_INSTRUMENT_H
#define H2_INSTRUMENT_H

#include <hydrogen/Object.h>
#include <hydrogen/globals.h>
#include <hydrogen/helpers/xml.h>
#include <hydrogen/adsr.h>
#include <hydrogen/basics/instrument_layer.h>

namespace H2Core
{

class Drumkit;

/**
\brief Instrument class
*/
class Instrument : public Object
{
    H2_OBJECT
    public:
        /**
         * \brief constructor
         * \param id the id of this instrument
         * \param name the name of the instrument
         * \param adsr attack decay sustain release instance
         */
        Instrument( const int id, const QString& name, ADSR* adsr );
        /** \brief copy constructor */
        Instrument( Instrument *other );
        /** \brief destructor */
        ~Instrument();

        /** \brief create a new empty Instrument */
        static Instrument * create_empty();
        /** \brief creates a new Instrument, loads samples from a given instrument within a given drumkit
         * \param drumkit_name the drumkit to search the instrument in
         * \param instrument_name the instrument within the drumkit to load samples from
         * \return a new Instrument instance
         */
        static Instrument* load_instrument( const QString& drumkit_name, const QString& instrument_name );

        /** \brief loads instrument from a given instrument within a given drumkit into a `live` Instrument object.
         * \param drumkit_name the drumkit to search the instrument in
         * \param instrument_name the instrument within the drumkit to load samples from
         * \param is_live is it performed while playing
         */
        void load_from( const QString& drumkit_name, const QString& instrument_name, bool is_live = true );
        /** \brief loads instrument from a given instrument into a `live` Instrument object.
         * \param drumkit the drumkit the instrument belongs to
         * \param instrument to load samples and members from
         * \param is_live is it performed while playing
         */
        void load_from( Drumkit* drumkit, Instrument* instrument, bool is_live = true );

        /*
         * \brief save the intrument within the given XMLNode
         * \param node the XMLNode to feed
         */
        void save_to( XMLNode* node );
        /**
         * \brief load an instrument from an XMLNode
         * \param node the XMLDode to read from
         * \return a new Instrument instance
         */
        static Instrument* load_from( XMLNode *node );

        /** 
         * \brief get an instrument layer from  the list
         * \param idx the index to get the layer from
         * \return a pointer to the layer
         */
        InstrumentLayer* operator[]( int idx );
        /** 
         * \brief get an instrument layer from  the list
         * \param idx the index to get the layer from
         * \return a pointer to the layer
         */
        InstrumentLayer* get_layer( int idx );

        /**
         * \brief set a layer within the instrument's layer list
         * \param layer the layer to be set
         * \param idx the index within the list
         */
        void set_layer( InstrumentLayer* layer, int idx );

        void set_name( const QString& name )        { __name = name; }  ///< \brief set the name of the instrument
        const QString& get_name()                   { return __name; }  ///< \brief get the name of the instrument

        void set_id( const int id )                 { __id = id; }      ///< \brief set the id of the instrument
        int get_id()                                { return __id; }    ///< \brief get the id of the instrument

        void set_adsr( ADSR* adsr );                                    ///< \brief set the ADSR of the instrument
        ADSR* get_adsr()                            { return __adsr; }  ///< \brief get the ADSR of the instrument

        void set_mute_group( int group )            { __mute_group = group; }   ///< \brief set the mute group of the instrument
        int get_mute_group()                        { return __mute_group; }    ///< \brief get the mute group of the instrument

        void set_midi_out_channel( int channel );                                   ///< \brief set the midi out channel of the instrument
        int get_midi_out_channel()                  { return __midi_out_channel; }  ///< \brief get the midi out channel of the instrument

        void set_midi_out_note( int note );                                         ///< \brief set the midi out note of the instrument
        int get_midi_out_note()                     { return __midi_out_note; }     ///< \brief get the midi out note of the instrument

        void set_muted( bool muted )                { __muted = muted; }    ///< \brief set muted status of the instrument
        bool is_muted()                             { return __muted; }     ///< \brief get muted status of the instrument

        void set_pan_l( float val )                 { __pan_l = val; }      ///< \brief set left pan of the instrument
        float get_pan_l()                           { return __pan_l; }     ///< \brief get left pan of the instrument

        void set_pan_r( float val )                 { __pan_r = val; }      ///< \brief set right pan of the instrument
        float get_pan_r()                           { return __pan_r; }     ///< \brief get right pan of the instrument

        void set_gain( float gain )                 { __gain = gain; }      ///< \brief set gain of the instrument
        float get_gain()                            { return __gain; }      ///< \brief get gain of the instrument

        void set_volume( float volume )             { __volume = volume; }  ///< \brief set the volume of the instrument
        float get_volume()                          { return __volume; }    ///< \brief get the volume of the instrument

        void set_filter_active( bool active )       { __filter_active = active; }   ///< \brief activate the filter of the instrument
        bool is_filter_active()                     { return __filter_active; }     ///< \brief get the status of the filter of the instrument

        void set_filter_resonance( float val )      { __filter_resonance = val; }   ///< \brief set the filter resonance of the instrument
        float get_filter_resonance()                { return __filter_resonance; }  ///< \brief get the filter resonance of the instrument

        void set_filter_cutoff( float val )         { __filter_cutoff = val; }      ///< \brief set the filter cutoff of the instrument
        float get_filter_cutoff()                   { return __filter_cutoff; }     ///< \brief get the filter cutoff of the instrument

        void set_peak_l( float val )                { __peak_l = val; }     ///< \brief set the left peak of the instrument
        float get_peak_l()                          { return __peak_l; }    ///< \brief get the left peak of the instrument

        void set_peak_r( float val )                { __peak_r = val; }     ///< \brief set the right peak of the instrument
        float get_peak_r()                          { return __peak_r; }    ///< \brief get the right peak of the instrument

        void set_fx_level( float level, int index ) { __fx_level[index] = level; }  ///< \brief set the fx level of the instrument
        float get_fx_level( int index )             { return __fx_level[index]; }   ///< \brief get the fx level of the instrument

        void set_random_pitch_factor( float val )   { __random_pitch_factor = val; }    ///< \brief set the random pitch factor of the instrument
        float get_random_pitch_factor()             { return __random_pitch_factor; }   ///< \brief get the random pitch factor of the instrument

        void set_active( bool active )              { __active = active; }      ///< \brief set the active status of the instrument
        bool is_active()                            { return __active; }        ///< \brief get the active status of the instrument

        void set_soloed( bool soloed )              { __soloed = soloed; }      ///< \brief set the soloed status of the instrument
        bool is_soloed()                            { return __soloed; }        ///< \brief get the soloed status of the instrument

        void enqueue()                              { __queued++; }                         ///< \brief enqueue the instrument
        void dequeue()                              { assert( __queued > 0 ); __queued--; } ///< \brief dequeue the instrument
        int is_queued()                             { return __queued; }                    ///< \brief get the queued status of the instrument

        void set_stop_note( bool stopnotes )        { __stop_notes = stopnotes; }   ///< \brief set the stop notes status of the instrument
        bool is_stop_notes()                        { return __stop_notes; }        ///< \brief get the stop notes of the instrument

    private:
        int __id;			                        ///< instrument id, should be unique
        QString __name;			                    ///< instrument name
        float __gain;                               ///< gain of the instrument
        float __volume;			                    ///< volume of the instrument
        float __pan_l;			                    ///< left pan of the instrument
        float __pan_r;			                    ///< right pan of the instrument
        float __peak_l;			                    ///< left current peak value
        float __peak_r;			                    ///< right current peak value
        ADSR* __adsr;                               ///< attack delay sustain release instance
        bool __filter_active;		                ///< is filter active?
        float __filter_cutoff;		                ///< filter cutoff (0..1)
        float __filter_resonance;	                ///< filter resonant frequency (0..1)
        float __random_pitch_factor;                ///< random pitch factor
        int __midi_out_note;		                ///< midi out note
        int __midi_out_channel;		                ///< midi out channel
        bool __stop_notes;		                    ///< will the note automatically generate a note off after beeing on
        bool __active;			                    ///< is the instrument active?
        bool __soloed;                              ///< is the instrument in solo mode?
        bool __muted;                               ///< is the instrument muted?
        int __mute_group;		                    ///< mute group of the instrument
        /** \brief count the number of notes queued within Sampler::__playing_notes_queue or std::priority_queue m_songNoteQueue */
        int __queued;
        float __fx_level[MAX_FX];	                ///< Ladspa FX level array
        InstrumentLayer* __layers[MAX_LAYERS];      ///< InstrumentLayer array
};

};

#endif // H2_INSTRUMENT_H

/* vim: set softtabstop=4 expandtab:  */
