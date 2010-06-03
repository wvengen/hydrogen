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

#ifndef H2_PATTERN_H
#define H2_PATTERN_H

#include <hydrogen/Object.h>
#include <hydrogen/note.h>

namespace H2Core
{

/**
 * \brief The Pattern is a Note container.
 */
class Pattern : public Object
{
    H2_OBJECT
    public:
        /** \brief constructor
         * \param name the name of the pattern
         * \param category the category this pattern belongs to
         * \param length
         */
	    Pattern( const QString& name, const QString& category, int length = MAX_NOTES );
        /** \brief destructor */
	    ~Pattern();

	    /**
         * delete notes that pertain to instrument I.
         * the function is thread safe (it locks the audio data while deleting notes)
         */
        void purge_instrument( Instrument * I );
	
	    /**
         * check if there are any notes pertaining to I
         */
	    bool references_instrument( Instrument * I );
        void set_to_old();
        static Pattern* get_empty_pattern();
        Pattern* copy();
        
        /** */
        void debug_dump();

        void set_length( unsigned length )              { __length = length; }      ///< \brief set length of the pattern 
        int get_length()                                { return __length; }        ///< \brief get length of the pattern 

        void set_name( const QString& name )            { __name = name; }          ///< \brief set the name of the pattern 
        const QString& get_name() const                 { return __name; }          ///< \brief get the name of the pattern 

        void set_category( const QString& category )    { __category = category; }  ///< \brief set the category of the pattern 
	    const QString& get_category() const             { return __category; }      ///< \brief get the category of the pattern 

	    typedef std::multimap <int, Note*> notes_t;                                 ///< \brief multimap note type
		typedef notes_t::iterator notes_it_t;                                       ///< \brief multimap note iterator type
        typedef notes_t::const_iterator notes_cst_it_t;                             ///< \brief multimap note const iterator type
        notes_t* get_notes()                            { return &__notes; }        ///< \brief get the note multimap

	    typedef std::set <Pattern*> virtual_patterns_t;                             ///< \brief note set type;
		typedef virtual_patterns_t::iterator virtual_patterns_it_t;                 ///< \brief note set iterator type;
		typedef virtual_patterns_t::const_iterator virtual_patterns_cst_it_t;       ///< \brief note set const iterator type;

        virtual_patterns_t* get_virtual_patterns()      { return &__virtual_patterns; } ///< \brief get the virtual pattern set
        
        /** \brief get the virtual pattern transitive closure set */
        virtual_patterns_t* get_virtual_patterns_transitive_closure()   { return &__virtual_patterns_transitive_closure; }
        /** \brief set the virtual pattern transitive closure set */
        //virtual_pattern_set_t set_virtual_pattern_transitive_closure_set( virtual_pattern_set_t set ) { __virtual_pattern_transitive_closure_set = set; }
        void copy_virtual_patterns_to_transitive_closure( );
    
    private:
        int __length;                                               ///< TODO
        QString __name;                                             ///< the name of thepattern
        QString __category;                                         ///< the categora of the pattern
	    notes_t __notes;                                            ///< a multimap (hash with possible multiple values for one key) of notes
        virtual_patterns_t __virtual_patterns;                      ///< TODO
        virtual_patterns_t __virtual_patterns_transitive_closure;   ///< TODO
};

};

#endif // H2_PATTERN_H

/* vim: set softtabstop=4 expandtab: */
