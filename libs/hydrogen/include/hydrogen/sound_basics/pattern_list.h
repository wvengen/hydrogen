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

#ifndef H2_PATTERN_LIST_H
#define H2_PATTERN_LIST_H

#include <hydrogen/Object.h>
#include <hydrogen/sound_basics/pattern.h>

namespace H2Core
{

/**
\brief PatternList is a collection of patterns
*/
class PatternList : public Object
{
    H2_OBJECT
    public:
        /** \brief constructor */
        PatternList();
        /** \brief destructor */
        ~PatternList();
        
        /**
         * \brief copy constructor, copies the pointers, don't use Pattern::Pattern( Pattern* pattern); constructor
         * \param other 
         */
        PatternList( PatternList* other);

        /** \brief returns the numbers of patterns */
	    int size() { return __patterns.size(); }
        /** 
         * \brief add a pattern to the list, if not allready in
         * \param pattern a pointer to the pattern to add
         */
        void operator<<( Pattern* pattern );
        /** 
         * \brief get a pattern from the list, if not allready in
         * \param idx the index to get the pattern from
         */
        Pattern* operator[]( int idx );
        /** 
         * \brief add a pattern to the list
         * \param pattern a pointer to the pattern to add
         */
        void add( Pattern* pattern );
        /** 
         * \brief get a pattern from  the list
         * \param idx the index to get the pattern from
         */
        Pattern* get( int idx );
        /**
         * \brief remove the pattern at a given index, does not delete it
         * \param idx the index
         * \return a pointer to the removed pattern
         */
        Pattern* del( int idx ); 
        /**
         * \brief remove a pattern from the list, does not delete it
         * \param pattern the pattern to be removed
         * \return a pointer to the removed pattern, 0 if not found
         */
        Pattern* del( Pattern *pattern ); 
        /**
         * \brief get the index of a pattern within the patterns
         * \param pattern a pointer to the pattern to find
         * \return -1 if not found
         */
        int index( Pattern* pattern );

        /** clear the underlying patterns list */
        void clear()    { __patterns.clear(); }
        /** \brief mark all patterns as old, see Pattern::set_to_old */
        void set_to_old();

        /**
         * \brief swap the patterns of two different indexes
         * \param idx_a the first index
         * \param idx_b the second index
         */
	    void swap( int idx_a, int idx_b );
        /**
         * \brief move a pattern from a position to another
         * \param idx_a the start index
         * \param idx_b the finish index
         */
	    void move( int idx_a, int idx_b );

        /**
         * \brief call compute_flattened_virtual_patterns on each pattern
         */
        void compute_flattened_virtual_patterns();
        /**
         * \brief call del_virtual_pattern on each pattern
         * \param pattern the pattern to remove where it's found
         */
        void del_virtual_pattern( Pattern* pattern);

    private:
        std::vector<Pattern*> __patterns;
};

};

#endif // H2_PATTERN_LIST_H

/* vim: set softtabstop=4 expandtab: */
