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
#ifndef H2CORE_SEQOUTPUTINTERFACE_H
#define H2CORE_SEQOUTPUTINTERFACE_H

#include "SeqScriptIterator.h"

namespace H2Core
{
    class TransportPosition;

    /**
     * This is the base class for any audio or midi classes that serve as
     * outputs for the sequencer.
     */
    class SeqOutputInterface
    {
    public:
        virtual ~SeqOutputInterface() {}

        /**
         * Process the events supplied by the sequencer.  The events will be
         * sorted by frame number (ascending).  This function must be realtime
         * safe.
         *
         * The 'pos' parameter should *not* unless you are *really* needing to
         * map frames back to B:b.t.  The 'pos' parameter is intended for
         * outputs that are doing something like MIDI recording (mapping
         * realtime events back to the B:b.t location in the song.  Audio
         * (i.e. the sampler) and MIDI outputs should *not* use the pos
         * parameter.  Neither should the pos parameter be used to drive a
         * transport.  Adding the 'pos' parameter here makes for a more flexable
         * design.
         */
        virtual int process(SeqScriptConstIterator begin,
                            SeqScriptConstIterator end,
                            const TransportPosition& pos,
                            uint32_t nframes) = 0;
    };

} // namespace H2Core

#endif // H2CORE_SEQOUTPUTINTERFACE_H
