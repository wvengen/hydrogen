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

#ifndef H2C_SONG_H
#define H2C_SONG_H

#include <vector>

#include <hydrogen/Object.h>

namespace H2Core
{

class XMLNode;
class Sample;
class Instrument;
class InstrumentList;
class PatternList;

#define VOLUME_MIN              0.0f
#define VOLUME_MAX              1.0f
#define CLICK_VOLUME_MIN        0.0f
#define CLICK_VOLUME_MAX        1.0f
#define HUMANIZE_SWING_MIN      0.0f
#define HUMANIZE_SWING_MAX      1.0f
#define HUMANIZE_TIME_MIN       0.0f
#define HUMANIZE_TIME_MAX       1.0f
#define HUMANIZE_VELOCITY_MIN   0.0f
#define HUMANIZE_VELOCITY_MAX   1.0f

/**
 * \brief	Song class
*/
class Song : public Object
{
    H2_OBJECT
    public:
        /** possible song playing modes */
        typedef enum _song_mode {
            PATTERN_MODE,
            SONG_MODE
        } song_mode_t;

        /*
         // internal delay FX
         bool m_bDelayFXEnabled;
         float m_fDelayFXWetLevel;
         float m_fDelayFXFeedback;
         unsigned m_nDelayFXTime;
        //~ internal delay fx
        */

        static Song* get_empty_song();
        static Song* get_default_song();

        /**
         * \brief constructor
         * \param title the song title
         * \param author the author of the song
         * \param bpm beat per minute of he song
         * \parama volume the volume of the song
         */
        Song( const QString& title, const QString& author, float bpm, float volume );
        /** destructor */
        ~Song();

        static Song* load( const QString& song_path );
        bool save( const QString& song_path, bool overwrite=false );
        void readTempPatternList( QString filename );
        /**
         * Remove all the notes in the song that play on instrument.
         * The function is real-time safe (it locks the audio data while deleting notes)
         */
        void purge_instrument( Instrument* instrument );

        void set_bpm( float bpm )                           { __bpm = bpm; }
        float get_bpm() const                               { return __bpm; }
        void set_resolution( int resolution )               { __resolution = resolution; }
        float get_resolution() const                        { return __resolution; }
        void set_volume( float volume );
        float get_volume() const                            { return __volume; }
        void set_click_volume( float volume );
        float get_click_volume() const                      { return __click_volume; }
        void set_is_muted( bool muted )                     { __is_muted = muted; }
        bool is_muted() const                               { return __is_muted; }
        void set_is_modified( bool modified )               { __is_modified = modified; }
        bool is_modified() const                            { return __is_modified; }
        void set_loop_enabled( bool enabled )               { __is_loop_enabled = enabled; }
        bool is_loop_enabled() const                        { return __is_loop_enabled; }
        void set_mode( song_mode_t mode )                   { __song_mode = mode; }
        song_mode_t get_mode() const                        { return __song_mode; }
        void set_humanize_time( float humanize );
        float get_humanize_time() const                     { return __humanize_time; }
        void set_humanize_velocity( float humanize );
        float get_humanize_velocity() const                 { return __humanize_velocity; }
        void set_humanize_swing( float factor );
        float get_humanize_swing() const                    { return __humanize_swing; }
        void set_title( const QString& title )              { __title = title; }
        const QString& get_title() const                    { return __title; }
        void set_author( const QString& author )            { __author = author; }
        const QString& get_author() const                   { return __author; }
        void set_notes( const QString& notes )              { __notes = notes; }
        const QString& get_notes() const                    { return __notes; }
        void set_license( const QString& license )          { __license = license; }
        const QString& get_license() const                  { return __license; }
        void set_filename( const QString& filename )        { __filename = filename; }
        const QString& get_filename() const                 { return __filename; }

        void set_instruments( InstrumentList* instruments )                 { __instruments = instruments; }
        InstrumentList* get_instruments() const                             { return __instruments; }
        void set_patterns( PatternList* patterns )                          { __patterns = patterns; }
        PatternList* get_patterns() const                                   { return __patterns; }
        // TODO
        void set_pattern_group_vector( std::vector<PatternList*>* vect )    { __pattern_group_sequence = vect; }
        std::vector<PatternList*>* get_pattern_group_vector() const         { return __pattern_group_sequence; }

    private:
        float __bpm;			            ///< beats per minute
        int __resolution;	                ///< resolution of the song (ticks per quarter)
        float __volume;						///< volume of the song [0.0;1.0]
        float __click_volume;			    ///< click (metronome) volume
        bool __is_muted;                    ///< true if song is muted
        bool __is_modified;                 ///< true if songis modified
        bool __is_loop_enabled;             ///< true if song is in loop mode
        song_mode_t __song_mode;            ///< song mode pattern / song
        float __humanize_time;              ///< random note timing [0.0;1.0]
        float __humanize_swing;             ///< factor used to shift a few notes back or forward [0.0;1.0]
        float __humanize_velocity;          ///< random velocity [0.0;1.0]
        InstrumentList *__instruments;		///< Instrument list
        PatternList *__patterns;			///< Pattern list
        std::vector<PatternList*>* __pattern_group_sequence;	///< TODO sequence of pattern groups
        QString __title;		            ///< title of the song
        QString __author;	                ///< author of the song
        QString __notes;                    ///< textual notes about the song
        QString __license;	                ///< license of the song
        QString __filename;                 ///< the path to the song file

        /*
         * \brief save the song within the given XMLNode
         * \param node the XMLNode to feed
         */
        void save_to( XMLNode* node );
        /**
         * \brief load a song from an XMLNode
         * \param node the XMLDode to read from
         * \return a new Instrument instance
         */
        static Song* load_from( XMLNode *node );
};

};

#endif // H2C_SONG_H

/* vim: set softtabstop=4 expandtab: */
