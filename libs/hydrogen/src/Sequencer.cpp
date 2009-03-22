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

#include "Sequencer.h"
#include "SeqInputInterface.h"
#include "SeqOutputInterface.h"

#include <hydrogen/TransportPosition.h>

#include <QtCore/QMutexLocker>

#include <algorithm>

using namespace H2Core;
using namespace std;

Sequencer::Sequencer() : Object("Sequencer")
{
}

Sequencer::~Sequencer()
{
}

int Sequencer::process(const TransportPosition& pos, uint32_t nframes)
{
    inputs_list_t::iterator i;
    outputs_list_t::iterator o;
    int tmp, rv = 0;

    for( i = m_inputs.begin() ; i != m_inputs.end() ; ++i ) {
        tmp = (*i)->process(m_seq, pos, nframes);
        if( tmp ) rv = -1;
    }

    for( o = m_outputs.begin() ; o != m_outputs.end() ; ++o ) {
        tmp = (*o)->process(m_seq.begin_const(), m_seq.end_const(nframes), pos, nframes);
        if( tmp ) rv = -1;
    }

    m_seq.consumed(nframes);

    return rv;
}

int Sequencer::add_input(SeqInputInterface* i)
{
    QMutexLocker lk(&m_inputs_add_mutex);
    int rv = -1;
    inputs_list_t::iterator k;
    k = find(m_inputs.begin(), m_inputs.end(), i);
    if( k != m_inputs.end() ) {
        m_inputs.push_back(i);
        rv = 0;
    }
    return rv;
}

int Sequencer::remove_input(SeqInputInterface* i)
{
    #warning "When we return... it's assumed that it's OK to delete i... but it could still"
    #warning "be in i->process().  How do we protect against that?"
    m_inputs.remove(i);
    return 0;
}

int Sequencer::add_output(SeqOutputInterface* o)
{
    QMutexLocker lk(&m_outputs_add_mutex);
    int rv = -1;
    outputs_list_t::iterator k;
    k = find(m_outputs.begin(), m_outputs.end(), o);
    if( k != m_outputs.end() ) {
        m_outputs.push_back(o);
        rv = 0;
    }
    return rv;
}

int Sequencer::remove_output(SeqOutputInterface* o)
{
    #warning "When we return... it's assumed that it's OK to delete o... but it could still"
    #warning "be in o->process().  How do we protect against that?"
    m_outputs.remove(o);
    return 0;
}
