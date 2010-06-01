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

#ifndef H2_INSTRUMENT_LIST_H
#define H2_INSTRUMENT_LIST_H

#include <hydrogen/Object.h>
#include <hydrogen/helpers/xml.h>
#include <hydrogen/sound_basic/instrument.h>

namespace H2Core
{

/**
\brief InstrumentList is a collection of instruments used within a song, a drumkit, ...
*/
class InstrumentList : public Object
{
    H2_OBJECT
    public:
        /** \brief constructor */
	    InstrumentList();
        /** \brief destructor */
	    ~InstrumentList();
        /**
         * \brief copy constructor
         * \param other 
         */
        InstrumentList( InstrumentList* other);

        /** \brief returns the numbers of instruments */
	    int size() { return __instruments.size(); }
        /** 
         * \brief add an instrument to the list
         * \param instrument a pointer to the instrument to add
         */
        void operator<<( Instrument* instrument );
        /** 
         * \brief get an instrument from  the list
         * \param idx the index to get the instrument from
         */
        Instrument* operator[]( int idx );
        /** 
         * \brief add an instrument to the list
         * \param instrument a pointer to the instrument to add
         */
        void add( Instrument* instrument );
        /** 
         * \brief get an instrument from  the list
         * \param idx the index to get the instrument from
         */
        Instrument* get( int idx );
        /**
         * \brief remove the instrument at a given index, does not delete it
         * \param idx the index
         * \return a pointer to the removed instrument
         */
	    Instrument* del( int idx );
        /**
         * \brief remove an instrument from the list, does not delete it
         * \param instrument the instrument to be removed
         * \return a pointer to the removed instrument, 0 if not found
         */
	    Instrument* del( Instrument* instrument );
        /**
         * \brief get the index of an instrument within the instruments
         * \param instrument a pointer to the instrument to find
         * \return -1 if not found
         */
	    int index( Instrument* instrument );
        /**
         * \brief find an instrument within the instruments
         * \param id the id of the instrument to find
         * \return 0 if not found
         */
	    Instrument* find( const int i );
        /**
         * \brief find an instrument within the instruments
         * \param name the name of the instrument to find
         * \return 0 if not found
         */
	    Instrument* find( const QString& name );
        /**
         * \brief swap the instruments of two different indexes
         * \param idx_a the first index
         * \param idx_b the second index
         */
	    void swap( int idx_a, int idx_b );
        /**
         * \brief move an instrument from a position to another
         * \param idx_a the start index
         * \param idx_b the finish index
         */
	    void move( int idx_a, int idx_b );

        /*
         * \brief save the intrument list within the given XMLNode
         * \param node the XMLNode to feed
         */
        void save_to( XMLNode* node );
        /**
         * \brief load an instrument list from an XMLNode
         * \param node the XMLDode to read from
         * \return a new InstrumentList instance
         */
        static InstrumentList* load_from( XMLNode* node );

    private:
	    std::vector<Instrument*> __instruments;            ///< the list of instruments
};

};

#endif // H2_INSTRUMENT_LIST_H

