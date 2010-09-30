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

#include "hydrogen/version.h"

#include <cassert>

#include <hydrogen/adsr.h>
#include <hydrogen/data_path.h>
#include <hydrogen/LocalFileMng.h>
#include <hydrogen/helpers/filesystem.h>
#include <hydrogen/Preferences.h>

#include <hydrogen/fx/Effects.h>
#include <hydrogen/globals.h>
#include <hydrogen/basics/song.h>
#include <hydrogen/basics/sample.h>
#include <hydrogen/basics/instrument.h>
#include <hydrogen/basics/pattern.h>
#include <hydrogen/basics/pattern_list.h>
#include <hydrogen/basics/note.h>
#include <hydrogen/hydrogen.h>

#include <QDomDocument>

namespace H2Core
{

const char* Song::__class_name = "Song";

Song::Song( const QString& title, const QString& author, float bpm, float volume )
    : Object( __class_name ),
    __bpm( bpm ),
    __resolution( 48 ),
    __volume( volume ),
    __click_volume( 0.5 ),
    __is_muted( false ),
    __is_modified( false ),
    __is_loop_enabled( false ),
    __song_mode( PATTERN_MODE ),
    __humanize_time( 0.0 ),
    __humanize_swing( 0.0 ),
    __humanize_velocity( 0.0 ),
    __instruments( 0 ),
    __patterns( 0 ),
    __pattern_group_sequence( 0 ),
    __title( title ),
    __author( author ),
    __notes(""),
    __license(""),
    __filename( "" )
{
}

Song::~Song() {
	delete __patterns;
	delete __instruments;
	if ( __pattern_group_sequence ) {
		for ( int i = 0; i < __pattern_group_sequence->size(); i++ ) {
			PatternList* group = ( *__pattern_group_sequence )[i];
			group->clear();
			delete group;
		}
		delete __pattern_group_sequence;
	}
}

static inline float check_boundary( float v, float min, float max ) {
    if (v>max) return max;
    if (v<min) return min;
    return v;
}

void Song::set_volume( float volume ) { __volume = check_boundary( volume, VOLUME_MIN, VOLUME_MAX ); }

void Song::set_click_volume( float click_volume ) { __click_volume = check_boundary( click_volume, CLICK_VOLUME_MIN, CLICK_VOLUME_MAX ); }

void Song::set_humanize_time( float humanize ) { __humanize_time = check_boundary( humanize, HUMANIZE_TIME_MIN, HUMANIZE_TIME_MAX ); }

void Song::set_humanize_swing( float humanize ) { __humanize_swing = check_boundary( humanize, HUMANIZE_SWING_MIN, HUMANIZE_SWING_MAX ); }

void Song::set_humanize_velocity( float humanize ) { __humanize_velocity = check_boundary( humanize, HUMANIZE_VELOCITY_MIN, HUMANIZE_VELOCITY_MAX ); }

void Song::purge_instrument( Instrument* instrument ) {
    for( int i=0; i<__patterns->size(); i++ ) __patterns->get(i)->purge_instrument( instrument );
}

Song* Song::load( const QString& song_path ) {
    INFOLOG( QString("Load song %1").arg(song_path) );
    if ( !Filesystem::file_readable( song_path ) ) {
        ERRORLOG( QString("%1 is not a readable song file").arg(song_path) );
        return 0;
    }
    XMLDoc doc;
    if ( !doc.read( song_path ) ) return 0;
    // TODO XML VALIDATION !!!!!!!!!!
    XMLNode root = doc.firstChildElement( "song" );
    if ( root.isNull() ) {
        ERRORLOG( "song node not found" );
        return 0;
    }
    QString version = root.read_string( "version", "Unknown version" );
	if ( version != QString( get_version().c_str() ) ) {
		WARNINGLOG( "Trying to load a song created with a different version of hydrogen." );
		WARNINGLOG( QString("Song [%1] saved with version %2").arg(song_path).arg(version) );
	}
    Song* song = Song::load_from( &root );
	song->set_is_modified( false );
	song->set_filename( song_path );
    return song;
}

Song* Song::load_from( XMLNode* node ) {

	float bpm = node->read_float( "bpm", 120 );

    Hydrogen::get_instance()->setNewBpmJTM( bpm ); 

    Song *song = new Song(
        node->read_string( "name", "Untitled Song" ),
        node->read_string( "author", "Unknown Author" ),
        bpm,
        node->read_float( "volume", 0.5 )
    );
	song->set_click_volume( node->read_float("metronomeVolume", 0.5 ) );
	song->set_notes( node->read_string( "notes", "..." ) );
	song->set_license( node->read_string( "license", "Unknown License") );
	song->set_loop_enabled( node->read_bool( "loopEnabled", false ) );
    QString mode = node->read_string( "mode", "pattern" );
	song->set_mode( (mode=="song" ? Song::SONG_MODE : Song::PATTERN_MODE ) );
	song->set_humanize_time( node->read_float( "humanize_time", 0.0 ) );
	song->set_humanize_velocity( node->read_float( "humanize_velocity", 0.0 ) );
	song->set_humanize_swing( node->read_float( "swing_factor", 0.0 ) );
    /* TODO
      QString sDrumkit = LocalFileMng::readXmlString( instrumentNode, "drumkit", "" );	// drumkit
      Hydrogen::get_instance()->setCurrentDrumkitname( sDrumkit ); 
    */
    // Instruments
	XMLNode instrument_list_node = node->firstChildElement( "instrumentList" );
    if ( instrument_list_node.isNull() ) {
		ERRORLOG( "Error reading song: instrumentList node not found" );
		delete song;
		return 0;
    }
    InstrumentList *instruments = new InstrumentList();
    instruments->load_from( &instrument_list_node );
	song->set_instruments( instruments );
	// Patterns
	XMLNode pattern_list_node = node->firstChildElement( "patternList" );
    if ( pattern_list_node.isNull() ) {
		ERRORLOG( "Error reading song: patternList node not found" );
		delete song;
		return 0;
    }
    PatternList *patterns = new PatternList();
    patterns->load_from( &pattern_list_node, song->get_instruments() );
	XMLNode virtual_pattern_list_node = node->firstChildElement( "virtualPatternList" );
    if ( virtual_pattern_list_node.isNull() ) {
		ERRORLOG( "Error reading song: virtualPatternList node not found" );
		delete song;
		return 0;
    }
    // Virtual Patterns
    patterns->load_virtuals_from( &virtual_pattern_list_node );
	patterns->compute_flattened_virtual_patterns();
	song->set_patterns( patterns );
	// Pattern sequence
	XMLNode pattern_sequence_node = node->firstChildElement( "patternSequence" );
    std::vector<PatternList*>* pattern_groups = new std::vector<PatternList*>;
    if ( pattern_sequence_node.isNull() ) {
        ERRORLOG( "Error reading song : patternSequence node not found" );
        delete song;
        return 0;
    }
    XMLNode group_node = pattern_sequence_node.firstChildElement( "group" );
    while ( !group_node.isNull() ) {
        PatternList *pattern_sequence = new PatternList();
        XMLNode pattern_id_node = group_node.firstChildElement( "patternID" );
        while ( !pattern_id_node.isNull() ) {
            QString pattern_name = pattern_id_node.firstChild().nodeValue();
            Pattern *pattern = patterns->find( pattern_name );
            if ( !pattern ) {
                WARNINGLOG( QString("patternid %1 not found in patternSequence").arg(pattern_name) );
            } else {
                pattern_sequence->add( pattern );
            }
            pattern_id_node = ( QDomNode ) pattern_id_node.nextSiblingElement( "patternID" );
        }
        pattern_groups->push_back( pattern_sequence );
        group_node = group_node.nextSiblingElement( "group" );
    }
	song->set_pattern_group_vector( pattern_groups );
    // LADSPA
#ifdef H2CORE_HAVE_LADSPA
	Effects::get_instance()->reset();
#endif
    XMLNode ladspa_node = node->firstChildElement( "ladspa" );
	if ( ladspa_node.isNull() ) {
	    WARNINGLOG( "ladspa node not found" );
    } else {
#ifdef H2CORE_HAVE_LADSPA
		int fx_count = 0;
        XMLNode fx_node = ladspa_node.firstChildElement( "fx" );
		while ( !fx_node.isNull() ) {
			QString fx_name = fx_node.read_string( "name", "" );
			if ( !fx_name.isEmpty() ) {
				// FIXME: the sample rate should be set by the engine
				LadspaFX* fx = LadspaFX::load( fx_node.read_string( "filename", "", false, false ), fx_name, 44100 );
				if ( fx ) {
					fx->setEnabled( fx_node.read_bool( "enabled", false ) );
					fx->setVolume( fx_node.read_float( "volume", 1.0f ) );
					XMLNode input_control_node = fx_node.firstChildElement( "inputControlPort" );
					while ( !input_control_node.isNull() ) {
						QString port_name = input_control_node.read_string( "name", "", false, false );
						for ( int i=0; i < fx->inputControlPorts.size(); i++ ) {
							LadspaControlPort *port = fx->inputControlPorts[i];
							if ( QString( port->sName ) == port_name ) {
								port->fControlValue = input_control_node.read_float( "value", 0.0 );
							}
						}
                        input_control_node = input_control_node.nextSiblingElement( "inputControlPort" );
					}
				    Effects::get_instance()->setLadspaFX( fx, fx_count );
				}
			}
			fx_count++;
            if( fx_count>=MAX_FX) {
                WARNINGLOG( QString("Maximum number (%1) of ladspa fx has been reached, don't do further").arg(MAX_FX) );
                break;
            }
			fx_node = fx_node.nextSiblingElement( "fx" );
		}
#endif
	}
    // BPM Time Line
    Hydrogen::get_instance()->m_timelinevector.clear();
	Hydrogen::HTimelineVector tlvector;
	XMLNode bpm_time_line_node = node->firstChildElement( "BPMTimeLine" );
	if ( bpm_time_line_node.isNull() ) {
		WARNINGLOG( "BPMTimeLine node not found" );
    } else {
		XMLNode bpm_node = bpm_time_line_node.firstChildElement( "newBPM" );
		while ( !bpm_node.isNull() ) {
			tlvector.m_htimelinebeat = bpm_node.read_int( "BAR", 0 );
			tlvector.m_htimelinebpm = bpm_node.read_float( "BPM", 120.0 );	
			Hydrogen::get_instance()->m_timelinevector.push_back( tlvector );
			Hydrogen::get_instance()->sortTimelineVector();
			bpm_node = bpm_node.nextSiblingElement( "newBPM" );
		}
	}
    // Time Line Tag
	Hydrogen::get_instance()->m_timelinetagvector.clear();
	Hydrogen::HTimelineTagVector tltagvector;
	XMLNode time_line_tag_node = node->firstChildElement( "timeLineTag" );
	if ( time_line_tag_node.isNull() ) {
		WARNINGLOG( "timeLineTag node not found" );
    } else {
		XMLNode tag_node = time_line_tag_node.firstChildElement( "newTAG" );
		while( !tag_node.isNull() ) {
			tltagvector.m_htimelinetagbeat = tag_node.read_int( "BAR", 0 );
			tltagvector.m_htimelinetag = tag_node.read_string( "TAG", "" );	
			Hydrogen::get_instance()->m_timelinetagvector.push_back( tltagvector );
			Hydrogen::get_instance()->sortTimelineTagVector();
			tag_node = tag_node.nextSiblingElement( "newTAG" );
		}
	}
    return song;
}

/// Save a song to file
bool Song::save( const QString& song_path, bool overwrite ) {
    // TODO
    /*
    INFOLOG( "Saving song" );
    if( Filesystem::file_exists( song_path ) && !overwrite ) {
        ERRORLOG( QString("song %1 already exists").arg(song_path) );
        return false;
    }
    XMLDoc doc;
    QDomProcessingInstruction header = doc.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild( header );
    XMLNode root = doc.createElement( "song" );
    save_to( &root );
    doc.appendChild( root );
    return doc.write( Filesystem::drumkit_file(dk_path) );
    */
    return true;
}

/// Create default song
Song* Song::get_default_song(){

	Song *song = new Song( "empty", "hydrogen", 120, 0.5 );

	song->set_click_volume( 0.5 );
	song->set_notes( "..." );
	song->set_license( "" );
	song->set_loop_enabled( false );
	song->set_mode( Song::PATTERN_MODE );
	song->set_humanize_time( 0.0 );
	song->set_humanize_swing( 0.0 );
	song->set_humanize_velocity( 0.0 );
	song->__is_modified = false;
	song->set_filename( "empty_song" );
    /* instrument list */
	InstrumentList* pList = new InstrumentList();
	Instrument *pNewInstr = new Instrument(EMPTY_INSTR_ID, "New instrument", new ADSR());
	pList->add( pNewInstr );
	song->set_instruments( pList );
	#ifdef H2CORE_HAVE_JACK
	Hydrogen::get_instance()->renameJackPorts();
	#endif
    /* pattern list */
	PatternList *patternList = new PatternList();
	Pattern *emptyPattern = Pattern::get_empty_pattern(); 
	emptyPattern->set_name( QString("Pattern 1") ); 
	emptyPattern->set_category( QString("not_categorized") );
	patternList->add( emptyPattern );
	song->set_patterns( patternList );
    /* pattern group */
	std::vector<PatternList*>* pPatternGroupVector = new std::vector<PatternList*>;
	PatternList *patternSequence = new PatternList();
	patternSequence->add( emptyPattern );
	pPatternGroupVector->push_back( patternSequence );
	song->set_pattern_group_vector( pPatternGroupVector );
	return song;
}

/// Return an empty song
Song* Song::get_empty_song()
{
	QString dataDir = DataPath::get_data_path();	
	QString filename = dataDir + "/DefaultSong.h2song";
	if( ! QFile::exists( filename ) ){
		_ERRORLOG("File " + filename + " exists not. Failed to load default song.");
		filename = dataDir + "/DefaultSong.h2song";
	}
	Song *song = Song::load( filename );
	/* if file DefaultSong.h2song not accessible
	 * create a simple default song.
	 */
	if(!song){
		song = Song::get_default_song();
	}
	return song;
}


void Song::readTempPatternList( QString filename )
{
	Hydrogen *engine = Hydrogen::get_instance();

	//AudioEngine::get_instance()->lock( RIGHT_HERE );

	Song *song = engine->getSong();

	if (QFile( filename ).exists() == false ) {
		ERRORLOG( "tep file " + filename + " not found." );
		return;
	}

	QDomDocument doc = LocalFileMng::openXmlDocument( filename );
	QDomNodeList nodeList = doc.elementsByTagName( "tempPatternList" );
	

	if( nodeList.isEmpty() ){
		ERRORLOG( "Error reading tmp file" );
		return;
	}

	QDomNode songNode = nodeList.at(0);


	
	 // Virtual Patterns
	QDomNode  virtualPatternListNode = songNode.firstChildElement( "virtualPatternList" ); 
	QDomNode virtualPatternNode = virtualPatternListNode.firstChildElement( "pattern" );
	if ( !virtualPatternNode.isNull() ) {

	    	while (  ! virtualPatternNode.isNull()  ) {
		QString sName = "";
		sName = LocalFileMng::readXmlString(virtualPatternNode, "name", sName);
		
		Pattern *curPattern = NULL;
		unsigned nPatterns = song->get_patterns()->size();
		for ( unsigned i = 0; i < nPatterns; i++ ) {
		    Pattern *pat = song->get_patterns()->get( i );
		    
		    if (pat->get_name() == sName) {
			curPattern = pat;
			break;
		    }//if
		}//for
		
		if (curPattern != NULL) {
		    QDomNode  virtualNode = virtualPatternNode.firstChildElement( "virtual" );
		    while (  !virtualNode.isNull()  ) {
			QString virtName = virtualNode.firstChild().nodeValue();
			
			Pattern *virtPattern = NULL;
			for ( unsigned i = 0; i < nPatterns; i++ ) {
			    Pattern *pat = song->get_patterns()->get( i );
		    
			    if (pat->get_name() == virtName) {
				virtPattern = pat;
				break;
			    }//if
			}//for
			
			if (virtPattern != NULL) {
			    curPattern->add_virtual_pattern(virtPattern);
			} else {
			    ERRORLOG( "Song had invalid virtual pattern list data (virtual)" );
			}//if
			virtualNode = ( QDomNode ) virtualNode.nextSiblingElement( "virtual" );
		    }//while
		} else {
		    ERRORLOG( "Song had invalid virtual pattern list data (name)" );
		}//if
		virtualPatternNode = ( QDomNode ) virtualPatternNode.nextSiblingElement( "pattern" );
	    }//while
	}//if

    song->get_patterns()->compute_flattened_virtual_patterns();

	// Pattern sequence
	QDomNode patternSequenceNode = songNode.firstChildElement( "patternSequence" );

	std::vector<PatternList*> *pPatternGroupVector = song->get_pattern_group_vector();
	pPatternGroupVector->clear();
	
	PatternList *patternSequence;
	QDomNode groupNode = patternSequenceNode.firstChildElement( "group" );
	while (  !groupNode.isNull()  ) {
		patternSequence = new PatternList();
		QDomNode patternId = groupNode.firstChildElement( "patternID" );
		while (  !patternId.isNull()  ) {
			QString patId = patternId.firstChild().nodeValue();

			Pattern *pat = NULL;
			for ( unsigned i = 0; i < song->get_patterns()->size(); i++ ) {
				Pattern *tmp = song->get_patterns()->get( i );
				if ( tmp ) {
					if ( tmp->get_name() == patId ) {
						pat = tmp;
						break;
					}
				}
			}
			if ( pat == NULL ) {
				WARNINGLOG( "patternid not found in patternSequence" );
				patternId = ( QDomNode ) patternId.nextSiblingElement( "patternID" );
				continue;
			}
			patternSequence->add( pat );
			patternId = ( QDomNode ) patternId.nextSiblingElement( "patternID" );
		}
		pPatternGroupVector->push_back( patternSequence );

		groupNode = groupNode.nextSiblingElement( "group" );
	}

	song->set_pattern_group_vector( pPatternGroupVector );

}

};


/*
Pattern* SongReader::getPattern( QDomNode pattern, InstrumentList* instrList )
{
	Pattern *pPattern = NULL;

	QString sName;	// name
	sName = LocalFileMng::readXmlString( pattern, "name", sName );

	QString sCategory = ""; // category
	sCategory = LocalFileMng::readXmlString( pattern, "category", sCategory ,false ,false);
	int nSize = -1;
	nSize = LocalFileMng::readXmlInt( pattern, "size", nSize, false, false );

	pPattern = new Pattern( sName, sCategory, nSize );



	QDomNode pNoteListNode = pattern.firstChildElement( "noteList" );
	if ( ! pNoteListNode.isNull() ) {
		// new code :)
		QDomNode noteNode = pNoteListNode.firstChildElement( "note" );
		while ( ! noteNode.isNull()  ) {

			Note* pNote = NULL;

			unsigned nPosition = LocalFileMng::readXmlInt( noteNode, "position", 0 );
			float fLeadLag = LocalFileMng::readXmlFloat( noteNode, "leadlag", 0.0 , false , false );
			float fVelocity = LocalFileMng::readXmlFloat( noteNode, "velocity", 0.8f );
			float fPan_L = LocalFileMng::readXmlFloat( noteNode, "pan_L", 0.5 );
			float fPan_R = LocalFileMng::readXmlFloat( noteNode, "pan_R", 0.5 );
			int nLength = LocalFileMng::readXmlInt( noteNode, "length", -1, true );
			float nPitch = LocalFileMng::readXmlFloat( noteNode, "pitch", 0.0, false, false );
			QString sKey = LocalFileMng::readXmlString( noteNode, "key", "C0", false, false );
			QString nNoteOff = LocalFileMng::readXmlString( noteNode, "note_off", "false", false, false );

			int instrId = LocalFileMng::readXmlInt( noteNode, "instrument", EMPTY_INSTR_ID );

			Instrument *instrRef = NULL;
			// search instrument by ref
			for ( unsigned i = 0; i < instrList->size(); i++ ) {
				Instrument *instr = instrList->get( i );
				if ( instrId == instr->get_id() ) {
					instrRef = instr;
					break;
				}
			}
			if ( !instrRef ) {
				ERRORLOG( QString("Instrument with ID: '%1' not found. Note skipped.").arg(instrId) );
				noteNode = ( QDomNode ) noteNode.nextSiblingElement( "note" );
				continue;
			}
			//assert( instrRef );
			bool noteoff = false;
			if ( nNoteOff == "true" ) 
				noteoff = true;
            pNote = new Note( instrRef, nPosition, fVelocity, fPan_L, fPan_R, nLength, nPitch );
            pNote->set_key_octave( sKey );
            pNote->set_lead_lag( fLeadLag );
            pNote->set_note_off( noteoff );
            pPattern->get_notes()->insert( std::make_pair( pNote->get_position(), pNote ) );
            noteNode = ( QDomNode ) noteNode.nextSiblingElement( "note" );
		}
	} else {
		// Back compatibility code. Version < 0.9.4
		QDomNode sequenceListNode = pattern.firstChildElement( "sequenceList" );

		int sequence_count = 0;
		QDomNode sequenceNode = sequenceListNode.firstChildElement( "sequence" );
		while ( ! sequenceNode.isNull()  ) {
			sequence_count++;

			QDomNode noteListNode = sequenceNode.firstChildElement( "noteList" );
			QDomNode noteNode = noteListNode.firstChildElement( "note" );
			while (  !noteNode.isNull() ) {

				Note* pNote = NULL;

				unsigned nPosition = LocalFileMng::readXmlInt( noteNode, "position", 0 );
				float fLeadLag = LocalFileMng::readXmlFloat( noteNode, "leadlag", 0.0 , false , false );
				float fVelocity = LocalFileMng::readXmlFloat( noteNode, "velocity", 0.8f );
				float fPan_L = LocalFileMng::readXmlFloat( noteNode, "pan_L", 0.5 );
				float fPan_R = LocalFileMng::readXmlFloat( noteNode, "pan_R", 0.5 );
				int nLength = LocalFileMng::readXmlInt( noteNode, "length", -1, true );
				float nPitch = LocalFileMng::readXmlFloat( noteNode, "pitch", 0.0, false, false );

				int instrId = LocalFileMng::readXmlInt( noteNode, "instrument", EMPTY_INSTR_ID );

				Instrument *instrRef = NULL;
				// search instrument by ref
				for ( unsigned i = 0; i < instrList->size(); i++ ) {
					Instrument *instr = instrList->get( i );
					if ( instrId == instr->get_id() ) {
						instrRef = instr;
						break;
					}
				}
				assert( instrRef );

				pNote = new Note( instrRef, nPosition, fVelocity, fPan_L, fPan_R, nLength, nPitch );
				pNote->set_lead_lag(fLeadLag);

				//infoLog( "new note!! pos: " + toString( pNote->m_nPosition ) + "\t instr: " + instrId );
				pPattern->get_notes()->insert( std::make_pair( pNote->get_position(), pNote ) );
			
				noteNode = ( QDomNode ) noteNode.nextSiblingElement( "note" );

				
			}
			sequenceNode = ( QDomNode ) sequenceNode.nextSiblingElement( "sequence" );
		}
	}

	return pPattern;
}
*/
