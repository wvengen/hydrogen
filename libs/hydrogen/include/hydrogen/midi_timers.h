 
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


#ifndef HIITIMER_H
#define HIITIMER_H

#include <stdint.h> // For uint32_t et al

#include <hydrogen/Object.h>
#include <hydrogen/note.h>
#include <hydrogen/Song.h>
#include <hydrogen/IO/AudioOutput.h>
#include <hydrogen/globals.h>


namespace H2Core
{

///
/// A simple timer...
///
class HIIMBCTimer : public Object
{
public:
	HIIMBCTimer( int bpm );
	~HIIMBCTimer();

	void setNewTimeval( int bpm );
	void start();
	void stop();

	friend void* processHIIMBCTimer( void* param );
	bool m_brunTimer;
	int m_ntimeval;
	bool pthread_timer_living;
	void doThings();



private:

	pthread_t timerThread;
};

} // namespace H2Core

#endif


