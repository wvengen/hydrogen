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

class PatternList;

/**
 * \brief The Pattern is a Note container which also may contains virtual patterns which are referecences to other patterns
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
        /** \brief copy constructor */
        Pattern( Pattern *other );
        /** \brief destructor */
	    ~Pattern();

	    /**
         * delete notes that pertain to instrument I.
         * the function is thread safe (it locks the audio data while deleting notes)
         */
        void purge_instrument( Instrument * I );
	
	    /**
         * TODO
         * check if there are any notes pertaining to I
         */
	    bool references_instrument( Instrument * I );
        // TODO
        void set_to_old();
        /** \brief return an empty pattern */
        static Pattern* get_empty_pattern();
        
        /** \brief output details through logger with DEBUG severity */
        void dump();

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

        bool virtual_patterns_empty()                   { return __virtual_patterns.empty(); }      ///< \brief return true if __virtual_patterns is empty
        void clear_virtual_patterns()                   { __virtual_patterns.clear(); }             ///< \brief clear __virtual_patterns
        /**
         * \brief add a pattern to virtual patern set
         * \param pattern the pattern to add
         */
        void add_virtual_pattern( Pattern* pattern )    { __virtual_patterns.insert( pattern); }
        /**
         * \brief remove a pattern from virtual pattern set, flattened virtual patterns have to be rebuilt
         */
        void del_virtual_pattern( Pattern* pattern );

        void clear_flattened_virtual_patterns()         { __flattened_virtual_patterns.clear(); }   ///< \brief clear __flattened_virtual_patterns
        /**
         * \brief compute __flattened_virtual_patterns based on __virtual_patterns
         * __flattened_virtual_patterns must have been cleared before which is the case is called
         * from PatternList::compute_flattened_virtual_patterns
         */
        void compute_flattened_virtual_patterns();
        /**
         * \brief add content of __flatteened_virtual_patterns into patterns
         * \param patterns the pattern list to feed
         */
        void extand_with_flattened_virtual_patterns( PatternList* patterns );
        
        virtual_patterns_t* get_virtual_patterns()              { return &__virtual_patterns; }             ///< \brief get the virtual pattern set
        virtual_patterns_t* get_flattened_virtual_patterns()    { return &__flattened_virtual_patterns; }   ///< \brief get the flattened virtual pattern set
        
    private:
        int __length;                                               ///< the length of the pattern
        QString __name;                                             ///< the name of thepattern
        QString __category;                                         ///< the category of the pattern
	    notes_t __notes;                                            ///< a multimap (hash with possible multiple values for one key) of notes
        virtual_patterns_t __virtual_patterns;                      ///< a list of patterns directly referenced by this one
        virtual_patterns_t __flattened_virtual_patterns;            ///< the complete list of virtual patterns
};

};

#endif // H2_PATTERN_H

/* vim: set softtabstop=4 expandtab: */
