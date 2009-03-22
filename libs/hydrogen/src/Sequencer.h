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
#ifndef H2CORE_SEQUENCER_H
#define H2CORE_SEQUENCER_H

#include <hydrogen/Object.h>
#include <QtCore/QMutex>

#include <list>

#include "SeqScript.h"

namespace H2Core
{
    class TransportPosition;
    class SeqInputInterface;
    class SeqOutputInterface;

    /**
     * The Sequencer manages inputs and outputs, translating and scheduling the data
     * so that things sound at the right time.
     */
    class Sequencer : public Object
    {
    public:
        Sequencer();
        ~Sequencer();

        int process(const TransportPosition& pos, uint32_t nframes);

        int add_input(SeqInputInterface* i);
        int remove_input(SeqInputInterface* i);
        int add_output(SeqOutputInterface* c);
        int remove_output(SeqOutputInterface* c);

    private:
        typedef std::list<SeqInputInterface*> inputs_list_t;
        typedef std::list<SeqOutputInterface*> outputs_list_t;

        SeqScript m_seq;

        QMutex m_inputs_add_mutex;   // Must be locked when adding inputs
        inputs_list_t m_inputs;
        QMutex m_outputs_add_mutex;  // Must be locked when adding outputs
        outputs_list_t m_outputs;
    };

} // namespace H2Core

#endif // H2CORE_SEQUENCER_H
