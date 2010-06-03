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

#include <hydrogen/sound_basics/instrument_layer.h>

namespace H2Core
{

const char* InstrumentLayer::__class_name = "InstrumentLayer";

InstrumentLayer::InstrumentLayer( Sample* sample )
    : Object( __class_name )
    , __start_velocity( 0.0 )
    , __end_velocity( 1.0 )
    , __pitch( 0.0 )
    , __gain( 1.0 )
    , __sample( sample )
{
}

InstrumentLayer::InstrumentLayer( InstrumentLayer* other )
    : Object( __class_name )
    , __start_velocity( other->get_start_velocity() )
    , __end_velocity( other->get_end_velocity() )
    , __pitch( other->get_pitch() )
    , __gain( other->get_gain() )
    , __sample( new Sample( 0, other->get_sample()->get_filename(), 0 ) )       // is not a real sample, it contains only the filename information
{
}

InstrumentLayer::InstrumentLayer( InstrumentLayer* other, Sample* sample )
    : Object( __class_name )
    , __start_velocity( other->get_start_velocity() )
    , __end_velocity( other->get_end_velocity() )
    , __pitch( other->get_pitch() )
    , __gain( other->get_gain() )
    , __sample( sample )
{
}

InstrumentLayer::~InstrumentLayer()
{
	delete __sample;
	__sample = 0;
}

InstrumentLayer* InstrumentLayer::load_from( XMLNode* node ) {
    Sample* sample = new Sample( 0, node->read_string( "filename", "" ), 0 );
    InstrumentLayer* layer = new InstrumentLayer( sample );
    layer->set_start_velocity( node->read_float( "min", 0.0 ) );
    layer->set_end_velocity( node->read_float( "max", 1.0 ) );
    layer->set_gain( node->read_float( "gain", 1.0, true, false ) );
    layer->set_pitch( node->read_float( "pitch", 0.0, true, false ) );
    return layer;
}

void InstrumentLayer::save_to( XMLNode* node ) {
    // TODO have to do this cause in Sample::load_wave, a new sample is created with an absolute path instead of relative path
    QString path = get_sample()->get_filename();
    int idx = path.lastIndexOf("/");
    if(idx>=0) {
        node->write_string( "filename", path.right( path.size()-1-path.lastIndexOf("/") ) );
    } else {
        node->write_string( "filename", path );
    }
    node->write_float( "min", __start_velocity );
    node->write_float( "max", __end_velocity );
    node->write_float( "gain", __gain );
    node->write_float( "pitch", __pitch );
}

};

/* vim: set softtabstop=4 expandtab: */
