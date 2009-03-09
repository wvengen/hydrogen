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
#ifndef H2CORE_SEQCLIENTINTERFACE_H
#define H2CORE_SEQCLIENTINTERFACE_H

#include "SeqScriptIterator.h"

namespace H2Core
{

    /**
     * This is the base class for any audio or midi classes that serve as
     * outputs for the sequencer.
     */
    class SeqClientInterface
    {
    public:
        virtual ~SeqClientInterface() {}

        /**
         * Process the events supplied by the sequencer.  The events will be
         * sorted by frame number (ascending).
         */
        virtual int process(SeqScriptConstIterator begin,
                            SeqScriptConstIterator end,
                            uint32_t nframes) = 0;
    };

} // namespace H2Core

#endif // H2CORE_SEQCLIENTINTERFACE_H
