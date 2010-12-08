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

#ifndef H2_DRUMKIT_H
#define H2_DRUMKIT_H

#include <hydrogen/Object.h>
#include <hydrogen/helpers/xml.h>
#include <hydrogen/basics/instrument_list.h>

namespace H2Core
{

/**
\ingroup H2Core
\brief	Drumkit info
*/
class Drumkit : public Object {
    H2_OBJECT
    public:
        /** \brief drumkit constructor, does nothing */
        Drumkit();
        /** \brief drumkit destructor, delete__ instruments */
	    ~Drumkit();
        
        /**
         * \brief load drumkit information from a path
         * \param dk_path like one returned by Filesystem::drumkit_path
         * \return a Drumkit on success, 0 otherwise
         */
        static Drumkit* load( const QString& dk_path );
        /**
         * \brief save a drumkit
         * \param overwrite allows to write over existing drumkit files
         * \return true on success
         */
        bool save( bool overwrite=false );
        /**
         * \brief save a drumkit
         * \param dk_path the path to save the drumkit into
         * \param overwrite allows to write over existing drumkit files
         * \return true on success
         */
        bool save( const QString& dk_path, bool overwrite=false );
        /**
         * \brief save a drumkit using given parameters and an instrument list
         * \param name the name of the drumkit
         * \param author the author of the drumkit
         * \param info the info of the drumkit
         * \param license the license of the drumkit
         * \þaram instruments the instruments to be saved within the drumkit
         * \oaram overwrite allows to write over existing drumkit files
         * \return true on success
         */
        static bool save( const QString& name, const QString& author, const QString& info, const QString& license, InstrumentList* instruments, bool overwrite=false );
        /**
         * \brief install a drumkit from a filename
         * \param filename the path to the new drumkit
         * \return true on success
         */
        static bool install( const QString& filename );
        /**
         * \brief remove a drumkit from the disk
         * \param name the drumkit name
         * \return true on success
         */
        static bool remove( const QString& name );

        /** \brief set __instruments, delete existing one */
        void set_instruments( InstrumentList* l )   { if(__instruments) { delete __instruments; } __instruments = l; }
        InstrumentList* get_instruments()           { return __instruments; }   ///< returns __instruments

        void set_path( const QString& path )        { __path = path; }          ///< sets __path
        const QString& get_path()                   { return __path; }          ///< returns __path

        void set_name( const QString& name )        { __name = name; }          ///< sets __name
        const QString& get_name()                   { return __name; }          ///< returns __name
        
        void set_author( const QString& author )    { __author = author; }      ///< sets __author
        const QString& get_author()                 { return __author; }        ///< returns __author
        
        void set_info( const QString& info )        { __info = info; }          ///< sets __info
        const QString& get_info()                   { return __info; }          ///< returns __info
        
        void set_license( const QString& license )  { __license = license; }    ///< sets __license
        const QString& get_license()                { return __license; }       ///< returns __license

        void dump();
    
    private:
        QString __path;                 ///< absolute drumkit path
        QString __name;                 ///< drumkit name
        QString __author;               ///< drumkit author
        QString __info;                 ///< drumkit free text
        QString __license;              ///< drumkit license description
        InstrumentList* __instruments;  ///< the list of instruments
        /*
         * \brief save the drumkit within the given XMLNode
         * \param node the XMLNode to feed
         */
        void save_to( XMLNode* node );
        /**
         * \brief load a drumkit from an XMLNode
         * \param node the XMLDode to read from
         */
        static Drumkit* load_from( XMLNode* node );
};

};

#endif // H2_DRUMKIT_H

/* vim: set softtabstop=4 expandtab: */
