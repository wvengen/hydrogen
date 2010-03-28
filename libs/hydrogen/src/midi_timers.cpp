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

#ifdef WIN32
#    include "hydrogen/timeHelper.h"
#else
#    include <unistd.h>
#    include <sys/time.h>
#endif

#include <hydrogen/hydrogen.h>
#include <hydrogen/Preferences.h>
#include <hydrogen/midi_timers.h>

#include <cassert>
#include <cmath>
#include <ctime>
#include <pthread.h>


//----------------------Midi Clock Timer---------------------------------
void* processHIIMBCTimer( void* param )
{
	
	H2Core::HIIMBCTimer *h2Timer = ( H2Core::HIIMBCTimer* )param;
	while( h2Timer->pthread_timer_living == true ){

		if (h2Timer->m_brunTimer){
			h2Timer->doThings();
		}
	
#ifdef WIN32
		Sleep( h2Timer->m_ntimeval / 100 );
#else
		usleep( h2Timer->m_ntimeval );
#endif
	}
	pthread_exit( NULL );
}


namespace H2Core
{

HIIMBCTimer::HIIMBCTimer( int bpm )
		: Object( "HIIMBCTimer" )
		,pthread_timer_living(true)
{
	
	INFOLOG( "INIT" );
	
	float eval = 1.0 / ( (float)bpm / 60.0 / 1000000.0 ) / 24.0 ;

	m_ntimeval = round(eval);
	pthread_attr_t attr;
	pthread_attr_init( &attr );
	pthread_create( &timerThread, &attr, processHIIMBCTimer, this );
}


HIIMBCTimer::~HIIMBCTimer()
{
	INFOLOG( "DESTROY" );
	pthread_timer_living = false;
	pthread_join( timerThread, NULL );

}


void HIIMBCTimer::start()//public
{
	m_brunTimer = true;
}


void HIIMBCTimer::stop()//public
{
	m_brunTimer = false;
	//throttle unused timer
	setNewTimeval(1);
}


void HIIMBCTimer::setNewTimeval( int bpm )//public
{
	int compValue = Preferences::get_instance()->get_sendMidiClockCompensation();
	if(fabs(compValue) < bpm )compValue = bpm;
	float violation = (float)bpm / (float)compValue;
	float eval = 1.0 / ( (float)bpm / (1.0 - violation ) / 60.0 / 1000000.0 ) / 24.0 ;
	m_ntimeval = round(eval);
}


void HIIMBCTimer::doThings()
{
#ifdef H2CORE_HAVE_ALSA
	Hydrogen::get_instance()->getMidiOutput()->handleBeatClock();
#endif // H2CORE_HAVE_ALSA
}

} // namespace H2Core
//~ ----------------------Midi Clock Timer-------------------------------