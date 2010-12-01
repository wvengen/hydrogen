/*
 * Copyright(c) 2010 by Zurcher Jérémy
 *
 * Hydrogen
 * Copyright(c) 2002-200/ Alessandro Cominu
 *
 * http://www.hydrogen-music.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef H2_XML_H
#define H2_XML_H

#include <hydrogen/Object.h>
#include <QtCore/QString>
#include <QtXml/QDomDocument>

namespace H2Core
{

/**
\ingroup H2Core
\brief	XMLNode is a subclass of QDomNode with read and write values methods
*/
class XMLNode : public Object, public QDomNode
{
    H2_OBJECT
    public:
        /** \brief basic constructor */
        XMLNode( );
        /** \brief to wrap a QDomNode */
        XMLNode( QDomNode node );

        /** \brief reads an integer stored into a child node
         * \param node the name of the child node to read into
         * \param default_value the value returned if something goes wrong
         * \param inexistent_ok if set to false output a DEBUG log line if the node dosen't exists
         * \param empty_ok if set to false output a DEBUG log lline if the child node is empty
         * \return the value */
        int read_int( const QString& node, int default_value, bool inexistent_ok=true, bool empty_ok=true );
        /** \brief reads a boolean stored into a child node
         * \param node the name of the child node to read into
         * \param default_value the value returned if something goes wrong
         * \param inexistent_ok if set to false output a DEBUG log line if the node dosen't exists
         * \param empty_ok if set to false output a DEBUG log lline if the child node is empty
         * \return the value */
        bool read_bool( const QString& node, bool default_value, bool inexistent_ok=true, bool empty_ok=true );
        /** \brief reads a float stored into a child node
         * \param node the name of the child node to read into
         * \param default_value the value returned if something goes wrong
         * \param inexistent_ok if set to false output a DEBUG log line if the node dosen't exists
         * \param empty_ok if set to false output a DEBUG log lline if the child node is empty
         * \return the value */
        float read_float( const QString& node, float default_value, bool inexistent_ok=true, bool empty_ok=true );
        /** \brief reads a string stored into a child node
         * \param node the name of the child node to read into
         * \param default_value the value returned if something goes wrong
         * \param inexistent_ok if set to false output a DEBUG log line if the node dosen't exists
         * \param empty_ok if set to false output a DEBUG log lline if the child node is empty
         * \return the value */
        QString read_string( const QString& node, const QString& default_value, bool inexistent_ok=true, bool empty_ok=true );
        
        /** \brief write an integer into a child node
         * \param node the name of the child node to create
         * \param value the value to write */
        void write_int( const QString& node, const int value );
        /** \brief write a boolean into a child node
         * \param node the name of the child node to create
         * \param value the value to write */
        void write_bool( const QString& node, const bool value );
        /** \brief write a float into a child node
         * \param node the name of the child node to create
         * \param value the value to write */
        void write_float( const QString& node, const float value );
        /** \brief write a string into a child node
         * \param node the name of the child node to create
         * \param value the value to write */
        void write_string( const QString& node, const QString& value );
    private:
        /** \brief reads a string stored into a child node
         * \param node the name of the child node to read into
         * \param inexistent_ok if set to false output a DEBUG log line if the node dosen't exists
         * \param empty_ok if set to false output a DEBUG log lline if the child node is empty
         * \return the read text */
        QString read_child_node( const QString& node, bool inexistent_ok, bool empty_ok );
        /** \brief write a string into a child node
         * \param node the name of the child node to create
         * \param text the text to write */
        void write_child_node( const QString& node, const QString& text );
};

/**
\ingroup H2Core
\brief	XMLDoc is a subclass of QDomDocument with read and write methods
*/
class XMLDoc : public Object, public QDomDocument
{
    H2_OBJECT
    public:
        /** \brief basic constructor */
        XMLDoc( );
        /** \brief read the content of an xml file
         * \param filepath the path to the file to read from
         * \param schema_path the path to the XML Schema file
         * \return true on success */
        bool read( const QString& filepath, const QString& schemapath=0 );
        /** \brief write itself into a file
         * \param filepath the path to the file to write to
         * \return true an success */
        bool write( const QString& filepath );
};

};

#endif  // H2_XML_H

/* vim: set softtabstop=4 expandtab: */
