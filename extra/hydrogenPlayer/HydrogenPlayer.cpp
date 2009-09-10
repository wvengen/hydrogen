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

#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <hydrogen/Object.h>
#include <hydrogen/hydrogen.h>
#include <hydrogen/LocalFileMng.h>
#include <hydrogen/Preferences.h>
#include <hydrogen/fx/Effects.h>
#include <hydrogen/event_queue.h>
#include <hydrogen/audio_engine.h>

using std::cout;
using std::endl;
#include <QApplication>

void usage()
{
	cout << "Usage: hydrogenPlayer song.h2song" << endl;
	cout << "Usage: hydrogenPlayer no_song" << endl;
	exit(0);
}

void quit()
{
	_INFOLOG( "Quitting..." );
	sleep(1);
	cout << "\nBye..." << endl;
	delete Logger::get_instance();
	exit(0);
}

int main(int argc, char** argv)
{
	if (argc != 2 ) {
		usage();
	}

	QApplication a(argc, argv);

	QString filename = argv[1];

	if( !QFile( filename ).exists() && filename != QString("no_song") ){//first check
		cout << "File not exists!" << endl;
		cout << "Quit!" << endl;
		exit(0);	
	}

	cout << "Hydrogen player starting..." << endl << endl;
	H2Core::Hydrogen::create_instance();
	H2Core::Hydrogen *hydrogen = H2Core::Hydrogen::get_instance();
	H2Core::AudioEngine::get_instance();

	H2Core::Song *pSong;
	pSong = H2Core::Song::load( filename );
	if (!pSong) {//second check
		pSong = H2Core::Song::get_empty_song();
		if (!pSong) {
			cout << "Error loading song!" << endl;
			cout << "Quit!" << endl;
			pSong->get_default_song();
			delete hydrogen;
			delete pSong;
			delete H2Core::EventQueue::get_instance();
			delete H2Core::AudioEngine::get_instance();
			delete Logger::get_instance();
			std::cout << std::endl << std::endl << Object::get_objects_number() << " alive objects" << std::endl << std::endl;
			Object::print_object_map();
			exit(0);
		}
	}

	hydrogen->setSong(pSong);
	H2Core::Preferences *preferences = H2Core::Preferences::get_instance();
	cout << endl << endl;
	cout << "- Commands - hydrogen player 0.9.4" << endl;
	cout << "-----------------------------------" << endl;
	cout << "TRANSPORT:" << endl;
	cout << " Press 0 for rewind from beginning" << endl;
	cout << " Press p for play" << endl;
	cout << " Press s for stop" << endl;
	cout << " Press q for quit" << endl;
	cout << " Press f for show frames" << endl;
	cout << " Press + for bpm +" << endl;
	cout << " Press - for bpm -" << endl;
	cout << " Press b(bpm) to set bpm (example: b120)" << endl;
	cout << " Press t for toggle transport slave" << endl;
	cout << " Press m for toggle transport master" << endl;
	cout << "-----------------------------------" << endl;
	cout << "SOUNDLIBRARIES" << endl;
	cout << " Press l for a list of available soundlibraries" << endl;
	cout << " Press L for load soundlibrary by index (example: L4)" << endl;
	cout << "-----------------------------------" << endl;	
	cout << "HELP" << endl;
	cout << " Press ? to display available commands" << endl;
	cout << "-----------------------------------" << endl;		
	int drumkits = 1;
	
	char pippo;

	std::vector<QString> systemList = H2Core::Drumkit::getSystemDrumkitList();
	std::vector<QString> userList = H2Core::Drumkit::getUserDrumkitList();
	Object::set_logging_level( "None" );
	int systemdrumkitlistsize = systemList.size();
	int userdrumkitlistsize = userList.size();
	int sndindex = 0;

	while (true) {
		pippo = getchar();
		switch( pippo ) {
			case 'q':
				cout << endl << "Quit? (y)es (n)o" << endl;
				char quit;
				scanf ("%s",&quit); 
				if ( quit != 121){
					break;
				}else
				{
					cout << endl << "HydrogenPlayer shutdown..." << endl;
					hydrogen->sequencer_stop();
					delete hydrogen;
					delete pSong;
					delete H2Core::EventQueue::get_instance();
					delete H2Core::AudioEngine::get_instance();
					delete preferences;
					delete Logger::get_instance();
	
					std::cout << std::endl << std::endl << Object::get_objects_number() << " alive objects" << std::endl << std::endl;
					Object::print_object_map();
	
					exit(0);
				}
				break;

			case 'p':
				hydrogen->sequencer_play();
				break;

			case 's':
				hydrogen->sequencer_stop();
				break;

			case '0':
				hydrogen->setPatternPos( 0 );
				break;

			case 'f':
				cout << "Frames = " << hydrogen->getTotalFrames() << endl;
				break;

			case '+':
				H2Core::AudioEngine::get_instance()->lock( RIGHT_HERE );
				hydrogen->setBPM( pSong->__bpm + 0.1 );
				H2Core::AudioEngine::get_instance()->unlock();
				cout << "BPM = " <<  pSong->__bpm << endl;
				break;

			case '-':
				H2Core::AudioEngine::get_instance()->lock( RIGHT_HERE );
				hydrogen->setBPM( pSong->__bpm - 0.1 );
				H2Core::AudioEngine::get_instance()->unlock();
				cout << "BPM = " <<  pSong->__bpm << endl;
				break;

			case 'b':
				float bpm;
				scanf ("%f",&bpm);
				if( bpm < 30.0) bpm = 30.0;
				if( bpm > 500.0) bpm = 500.0;
				hydrogen->setBPM( bpm );
				cout << "Set BPM to: " <<  pSong->__bpm << endl;
				break;

			case 'l':
				for (uint i = 0; i < systemList.size(); i++) {
					QString absPath = systemList[i];
					H2Core::Drumkit *info = H2Core::Drumkit::load( absPath );
					if (info) {
						QString name = info->getName();
						string stlstring = name.toStdString().c_str();
						cout << drumkits << " - " << stlstring <<endl;
						drumkits++;
					}else {	
						QString name = "-----------------------";
						string stlstring = name.toStdString().c_str();
						cout  << drumkits << " - " << stlstring << endl;
						drumkits++;
					}
					delete info;
				}
				for (uint i = 0; i < userList.size(); i++) {
					QString absPath = userList[i];
					H2Core::Drumkit *info = H2Core::Drumkit::load( absPath );
					if (info) {
						QString name = info->getName();
						string stlstring = name.toStdString().c_str();
						cout  << drumkits << " - " << stlstring << endl;
						drumkits++;
					}else {	
						QString name = "-----------------------";
						string stlstring = name.toStdString().c_str();
						cout  << drumkits << " - " << stlstring << endl;
						drumkits++;
					}
					delete info;
				}
				drumkits = 1;
				break;

			case 'L':
				int sndLibIndex;
				scanf ("%d",&sndLibIndex);
				sndindex = sndLibIndex - 1;
				if( ( sndindex ) < 0 || ( sndindex ) >= ( ( systemdrumkitlistsize ) + ( userdrumkitlistsize ) ) ){
					cout  << "Invalid index" <<  endl;
					break;
				}
				if( ( sndindex ) < systemdrumkitlistsize  ) {
					QString absPath = systemList[( sndindex )];
					H2Core::Drumkit *info = H2Core::Drumkit::load( absPath );
					if( info ){
						QString name = info->getName();
						string stlstring = name.toStdString().c_str();
						cout  << "Load:  " << stlstring << endl;
						hydrogen->loadDrumkit( info );
						delete info;
					}
					break;
				}	
				else {
					QString absPath = userList[ sndindex - ( systemdrumkitlistsize )];
					H2Core::Drumkit *info = H2Core::Drumkit::load( absPath );
					if ( info ) {
						QString name = info->getName();
						string stlstring = name.toStdString().c_str();
						cout  << "Load:  " << stlstring << endl;
						hydrogen->loadDrumkit( info );
						delete info;
					}else{
						cout << "No sondlibrary " << endl;
					}
					break;

				}

			case 't':
				if ( preferences->m_bJackTransportMode == H2Core::Preferences::USE_JACK_TRANSPORT ) {
					if ( preferences->m_bJackMasterMode == H2Core::Preferences::USE_JACK_TIME_MASTER ) {
						cout << "Warning, jack transport master is enabled," << endl;
						cout << "disable transport master first." << endl;
						break;
					}
					H2Core::AudioEngine::get_instance()->lock( RIGHT_HERE );
					preferences->m_bJackTransportMode = H2Core::Preferences::NO_JACK_TRANSPORT;
					H2Core::AudioEngine::get_instance()->unlock();
					cout << "Jack transport slave disabled" << endl;
				}
				else {
					H2Core::AudioEngine::get_instance()->lock( RIGHT_HERE );
					preferences->m_bJackTransportMode = H2Core::Preferences::USE_JACK_TRANSPORT;
					H2Core::AudioEngine::get_instance()->unlock();
					cout << "Jack transport slave enabled" << endl;
				}				
				break;

			case 'm':
				if ( preferences->m_bJackMasterMode == H2Core::Preferences::USE_JACK_TIME_MASTER ) {
					H2Core::AudioEngine::get_instance()->lock( RIGHT_HERE );
					preferences->m_bJackMasterMode = H2Core::Preferences::NO_JACK_TIME_MASTER;;
					H2Core::AudioEngine::get_instance()->unlock();
					hydrogen->offJackMaster();
					cout << "Jack transport master disabled" << endl;
				}
				else {
					H2Core::AudioEngine::get_instance()->lock( RIGHT_HERE );
					preferences->m_bJackTransportMode = H2Core::Preferences::USE_JACK_TRANSPORT;
					preferences->m_bJackMasterMode = H2Core::Preferences::USE_JACK_TIME_MASTER;
					H2Core::AudioEngine::get_instance()->unlock();
					hydrogen->onJackMaster();
					cout << "Jack transport slave enabled" << endl;
					cout << "Jack transport master enabled" << endl;
				}
				break;

			case '?':
				cout << "- Commands - hydrogen player 0.9.4" << endl;
				cout << "-----------------------------------" << endl;
				cout << "TRANSPORT:" << endl;
				cout << " Press 0 for rewind from beginning" << endl;
				cout << " Press p for play" << endl;
				cout << " Press s for stop" << endl;
				cout << " Press q for quit" << endl;
				cout << " Press f for show frames" << endl;
				cout << " Press + for bpm +" << endl;
				cout << " Press - for bpm -" << endl;
				cout << " Press b(bpm) to set bpm (example: b120)" << endl;
				cout << " Press t for toggle transport slave" << endl;
				cout << " Press m for toggle transport master" << endl;
				cout << "-----------------------------------" << endl;
				cout << "SOUNDLIBRARIES" << endl;
				cout << " Press l for a list of available soundlibraries" << endl;
				cout << " Press L for load soundlibrary by index (example: L4)" << endl;
				cout << "-----------------------------------" << endl;	
				cout << "HELP" << endl;
				cout << " Press ? to display available commands" << endl;
				cout << "-----------------------------------" << endl;	
				break;
/*
//------8<=====================
			case 'd':
				cout << "DEBUG" << endl;
				Object::print_object_map();
				int nObj = Object::get_objects_number();
				std::cout << std::endl << std::endl << nObj << " alive objects" << std::endl << std::endl;
				break;
//------8<=====================
*/
		}
	}
}



