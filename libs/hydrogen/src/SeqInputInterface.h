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
#ifndef H2CORE_SEQINPUTINTERFACE_H
#define H2CORE_SEQINPUTINTERFACE_H

#include <stdint.h> // uint32_t

namespace H2Core
{
    class SeqScript;
    class TransportPosition;

    /**
     * This is the base class for any classes that serve as inputs for the
     * sequencer (e.g. song, MIDI Input, GUI Input, etc.
     */
    class SeqInputInterface
    {
    public:
        virtual ~SeqInputInterface() {}

        /**
         * Supply data to the sequencer by directly writing them to the
         * SeqScript object.  This function must be realtime safe.
         */
        virtual int process(SeqScript& seq,
                            const TransportPosition& pos,
                            uint32_t nframes) = 0;
    };

} // namespace H2Core

#endif // H2CORE_SEQINPUTINTERFACE_H
