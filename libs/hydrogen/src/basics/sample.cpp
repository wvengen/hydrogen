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

#include <hydrogen/basics/sample.h>

#include <hydrogen/hydrogen.h>
#include <hydrogen/Preferences.h>
#include <hydrogen/helpers/flac.h>
#include <hydrogen/helpers/filesystem.h>

#include <sndfile.h>

using namespace std;

namespace H2Core
{
    
const char* Sample::__class_name = "Sample";
const char* Sample::__loop_modes[] = { "forward", "reverse", "pingpong" };

Sample::Sample( const QString& filename,  int frames, int sample_rate, float* data_l, float* data_r )
    : Object( __class_name ),
    __filename( filename ),
    __frames( frames ),
    __sample_rate( sample_rate ),
    __data_l( data_l ),
    __data_r( data_r ),
    __sample_is_modified( false ),
    __loop_mode( FORWARD ),
    __start_frame( 0 ),
    __end_frame( 0 ),
    __loop_frame( 0 ),
    __repeats( 0 ),
    __velo_pan( SampleVeloPan() ),
    __use_rubber( false ),
    __rubber_pitch( 0.0 ),
    __rubber_divider( 1.0 ),
    __rubber_c_settings( 4 )
{
    /* filename only, no path information */
    assert( filename.lastIndexOf("/")<0);
}

Sample::Sample( Sample* other )
    : Object( __class_name ),
    __filename( other->get_filename() ),
    __frames( other->get_frames() ),
    __sample_rate( other->get_sample_rate() ),
    __data_l( 0 ),
    __data_r( 0 ),
    __sample_is_modified( other->get_is_modified() ),
    __loop_mode( other->get_loop_mode() ),
    __start_frame( other->get_start_frame() ),
    __end_frame( other->get_end_frame() ),
    __loop_frame( other->get_loop_frame() ),
    __repeats( other->get_repeats() ),
    __velo_pan( other->__velo_pan ),
    __use_rubber( other->get_use_rubber() ),
    __rubber_pitch( other->get_rubber_pitch() ),
    __rubber_divider( other->get_rubber_divider() ),
    __rubber_c_settings( other->get_rubber_c_settings() )
{
    __data_l = new float[__frames];
    __data_r = new float[__frames];
    memcpy( __data_l, other->get_data_l(), __frames);
    memcpy( __data_r, other->get_data_r(), __frames);
}

Sample::~Sample() {
    if(__data_l!=0) delete[] __data_l;
    if(__data_r!=0) delete[] __data_r;
}

Sample* Sample::load( const QString& path ) {
    if( !Filesystem::file_readable(path)) {
        ERRORLOG(QString("Unable to read %1").arg(path));
        sleep(1);
        return 0;
    }
#ifdef HAVE_LIBSNDFILE_FLAC_SUPPORT
	return libsndfile_load( path );
#else
	if ( ( path.endsWith( "flac") ) || ( path.endsWith( "FLAC" )) ) {
#ifdef H2CORE_HAVE_FLAC
	    FLACFile file;
	    return file.load( path );
#else
	    ERRORLOG("FLAC support was disabled during compilation");
	    return 0;
#endif
    } else {
		return libsndfile_load( path );
	}
#endif
}

Sample* Sample::libsndfile_load( const QString& path ) {
    SF_INFO sound_info;
	SNDFILE* file = sf_open( path.toLocal8Bit(), SFM_READ, &sound_info );
	if ( !file ) {
		ERRORLOG( QString( "[Sample::load] Error loading file %1" ).arg( path ) );
	}

    float* buffer = new float[ sound_info.frames * sound_info.channels ];
	sf_count_t count = sf_read_float( file, buffer, sound_info.frames * sound_info.channels );
	sf_close( file );
    if(count==0) WARNINGLOG(QString("%1 is an empty sample").arg(path));

	float *data_l = new float[ sound_info.frames ];
	float *data_r = new float[ sound_info.frames ];

    if ( sound_info.channels == 1 ) {
        memcpy(data_l,buffer,sound_info.frames*sizeof(float));
        memcpy(data_r,buffer,sound_info.frames*sizeof(float));
	} else if ( sound_info.channels == 2 ) {
		for ( int i = 0; i < sound_info.frames; i++ ) {
			data_l[i] = buffer[i * 2];
			data_r[i] = buffer[i * 2 + 1];
		}
	} else {
        ERRORLOG(QString("can't handle %1 channels").arg(sound_info.channels));
    }
	delete[] buffer;

    int idx = path.lastIndexOf("/");
    QString filename( (idx>=0) ? path.right( path.size()-1-path.lastIndexOf("/") ) : path );
	Sample *sample = new Sample( filename, sound_info.frames, sound_info.samplerate, data_l, data_r );
	return sample;
}


Sample* Sample::load_edit_wave(
    const QString& path,
    const int start_frame,
    const int loop_frame,
    const int end_frame,
    const int loops,
    const loop_mode_t loop_mode,
    bool use_rubberband,
    float rubber_divider,
    int rubberbandCsettings,
    float rubber_pitch ) {

    if( !Filesystem::file_readable(path)) {
        ERRORLOG(QString("Unable to read %1").arg(path));
        return 0;
    }

	QString program = Preferences::get_instance()->m_rubberBandCLIexecutable;
	if ( !Filesystem::file_executable( program ) && use_rubberband) {
		_ERRORLOG( QString( "Rubberband executable %1 not found" ).arg( program ) );
		return 0;
	}

    Sample* orig_sample = Sample::load( path );

    bool full_loop = start_frame==loop_frame;
	int full_length =  end_frame - start_frame;
	int loop_length =  end_frame - loop_frame;
	int new_length = full_length + loop_length * loops;

	int sample_rate = orig_sample->get_sample_rate();
	float *orig_data_l = orig_sample->get_data_l();
	float *orig_data_r = orig_sample->get_data_r();

	float *new_data_l = new float[ new_length ];
	float *new_data_r = new float[ new_length ];

    // copy full_length frames to new_data
    if ( loop_mode==REVERSE && (loops==0 || full_loop) ) {
        if(full_loop) {
            // copy end => start
            for( int i=0, j=end_frame; i<full_length; i++, j-- ) new_data_l[i]=orig_data_l[j];
            for( int i=0, j=end_frame; i<full_length; i++, j-- ) new_data_r[i]=orig_data_r[j];
        } else {
            // copy start => loop
            int to_loop = loop_frame - start_frame;
            memcpy( new_data_l, orig_data_l+start_frame, sizeof(float)*to_loop );
            memcpy( new_data_r, orig_data_r+start_frame, sizeof(float)*to_loop );
            // copy end => loop
            for( int i=to_loop, j=end_frame; i<full_length; i++, j-- ) new_data_l[i]=orig_data_l[j];
            for( int i=to_loop, j=end_frame; i<full_length; i++, j-- ) new_data_r[i]=orig_data_r[j];
        }
    } else {
        // copy start => end
        memcpy( new_data_l, orig_data_l+start_frame, sizeof(float)*full_length );
        memcpy( new_data_r, orig_data_r+start_frame, sizeof(float)*full_length );
    }
    // copy the loops
    if( loops>0 ) {
        int x = full_length;
        bool ping_pong = (loop_mode==PINGPONG);
        bool forward = ( (loop_mode==FORWARD) ? true : false );
        for( int i=0; i<loops; i++ ) {
            if (forward) {
                // copy loop => end
                memcpy( &new_data_l[x], orig_data_l+loop_frame, sizeof(float)*loop_length );
                memcpy( &new_data_r[x], orig_data_r+loop_frame, sizeof(float)*loop_length );
            } else {
                // copy end => loop
                for( int i=end_frame, y=x; i>loop_frame; i--, y++ ) new_data_l[y]=orig_data_l[i];
                for( int i=end_frame, y=x; i>loop_frame; i--, y++ ) new_data_r[y]=orig_data_r[i];
            }
            x+=loop_length;
            if(ping_pong) forward=!forward;
        }
        assert(x==new_length);
    }
	//create new sample
	Sample *pSample = new Sample( orig_sample->get_filename(), new_length, sample_rate );

	Hydrogen *engine = Hydrogen::get_instance();

	//check for volume vector
	if ( (engine->m_volumen.size() > 2 )|| ( engine->m_volumen.size() == 2 &&  (engine->m_volumen[0].m_hyvalue > 0 || engine->m_volumen[1].m_hyvalue > 0 ))) {

		//1. write velopan into sample
		SampleVeloPan::SampleVeloVector velovec;
		pSample->__velo_pan.m_Samplevolumen.clear();
		for (int i = 0; i < static_cast<int>(engine->m_volumen.size()); i++){
			velovec.m_SampleVeloframe = engine->m_volumen[i].m_hxframe;
			velovec.m_SampleVelovalue = engine->m_volumen[i].m_hyvalue;
			pSample->__velo_pan.m_Samplevolumen.push_back( velovec );
		}
		//2. compute volume
		float divider = new_length / 841.0F;
		for (int i = 1; i  < static_cast<int>(engine->m_volumen.size()); i++){
			
			double y =  (91 - static_cast<int>(engine->m_volumen[i - 1].m_hyvalue))/91.0F;
			double k = (91 - static_cast<int>(engine->m_volumen[i].m_hyvalue))/91.0F;

			int deltastart_frame = engine->m_volumen[i - 1].m_hxframe * divider;
			int deltaend_frame = engine->m_volumen[i].m_hxframe * divider;

			if ( i == static_cast<int>(engine->m_volumen.size()) -1) deltaend_frame = new_length;
			int deltaIdiff = deltaend_frame - deltastart_frame ;
			double subtract = 0.0F;

			if ( y > k ){
				subtract = (y - k) / deltaIdiff;
			}else
			{
				subtract = ( k - y) / deltaIdiff * (-1);
			}

			for ( int z = static_cast<int>(deltastart_frame) ; z < static_cast<int>(deltaend_frame); z++){
				new_data_l[z] = new_data_l[z] * y;
				new_data_r[z] = new_data_r[z] * y;
				y = y - subtract;
			}
		}
		
	}

	//check for pan vector
	if ( (engine->m_pan.size() > 2 )|| ( engine->m_pan.size() == 2 &&  (engine->m_pan[0].m_hyvalue != 45 || engine->m_pan[1].m_hyvalue != 45 ))){
		//first step write velopan into sample
		SampleVeloPan::SamplePanVector panvec;
		pSample->__velo_pan.m_SamplePan.clear();
		for (int i = 0; i < static_cast<int>(engine->m_pan.size()); i++){
			panvec.m_SamplePanframe = engine->m_pan[i].m_hxframe;
			panvec.m_SamplePanvalue = engine->m_pan[i].m_hyvalue;
			pSample->__velo_pan.m_SamplePan.push_back( panvec );
		}

		float divider = new_length / 841.0F;
		for (int i = 1; i  < static_cast<int>(engine->m_pan.size()); i++){
			
			double y =  (45 - static_cast<int>(engine->m_pan[i - 1].m_hyvalue))/45.0F;
			double k = (45 - static_cast<int>(engine->m_pan[i].m_hyvalue))/45.0F;

			int deltastart_frame = engine->m_pan[i - 1].m_hxframe * divider;
			int deltaend_frame = engine->m_pan[i].m_hxframe * divider;

			if ( i == static_cast<int>(engine->m_pan.size()) -1) deltaend_frame = new_length;
			int deltaIdiff = deltaend_frame - deltastart_frame ;
			double subtract = 0.0F;

			
			if ( y > k ){
				subtract = (y - k) / deltaIdiff;
			}else
			{
				subtract = ( k - y) / deltaIdiff * (-1);
			}

			for ( int z = static_cast<int>(deltastart_frame) ; z < static_cast<int>(deltaend_frame); z++){
				if( y < 0 ){
					double k = 1 + y;
					new_data_l[z] = new_data_l[z] * k;
					new_data_r[z] = new_data_r[z];
				}
				else if(y > 0){
					double k = 1 - y;
					new_data_l[z] = new_data_l[z];
					new_data_r[z] = new_data_r[z] * k;
				}
				else if(y == 0){
					new_data_l[z] = new_data_l[z];
					new_data_r[z] = new_data_r[z];
				}
				y = y - subtract;	
			}
		}
		
	}

///rubberband
	if( use_rubberband ){

		int rubberoutframes = 0;
		double ratio = 1.0;
		double durationtime = 60.0 / engine->getNewBpmJTM() * rubber_divider/*beats*/;
		double induration = (double) new_length / (double) sample_rate;
		if (induration != 0.0) ratio = durationtime / induration;
		rubberoutframes = int(new_length * ratio + 0.1);
//		_INFOLOG(QString("ratio: %1, rubberoutframes: %2, rubberinframes: %3").arg( ratio ).arg ( rubberoutframes ).arg ( new_length ));
	
		//create new sample

		SF_INFO rubbersoundInfo;
		rubbersoundInfo.samplerate = sample_rate;
		rubbersoundInfo.frames = rubberoutframes;
		rubbersoundInfo.channels = 2;
		rubbersoundInfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
	
		if ( !sf_format_check( &rubbersoundInfo ) ) {
			_ERRORLOG( "Error in soundInfo" );
			return 0;
		}
		QString outfilePath = QDir::tempPath() + "/tmp_rb_outfile.wav";
		SNDFILE* m_file = sf_open( outfilePath.toLocal8Bit(), SFM_WRITE, &rubbersoundInfo);

		float *infobf = new float[rubbersoundInfo.channels * new_length];
			for (int i = 0; i < new_length; ++i) {
			float value_l = new_data_l[i];
			float value_r = new_data_r[i];
			if (value_l > 1.f) value_l = 1.f;
			if (value_l < -1.f) value_l = -1.f;
			if (value_r > 1.f) value_r = 1.f;
			if (value_r < -1.f) value_r = -1.f;
			infobf[i * rubbersoundInfo.channels + 0] = value_l;
			infobf[i * rubbersoundInfo.channels + 1] = value_r;
		}

		int res = sf_writef_float(m_file, infobf, new_length );
		sf_close( m_file );
		delete[] infobf;


		QObject *parent = 0;

		QProcess *rubberband = new QProcess(parent);

		QStringList arguments;

		QString rCs = QString(" %1").arg(rubberbandCsettings);
		float pitch = pow( 1.0594630943593, ( double)rubber_pitch );
		QString rPs = QString(" %1").arg(pitch);

		QString rubberResultPath = QDir::tempPath() + "/tmp_rb_result_file.wav";
		arguments << "-D" << QString(" %1").arg( durationtime ) 	//stretch or squash to make output file X seconds long
			  << "--threads"					//assume multi-CPU even if only one CPU is identified
			  << "-P"						//aim for minimal time distortion
			  << "-f" << rPs					//pitch
			  << "-c" << rCs					//"crispness" levels
			  << outfilePath 					//infile
			  << rubberResultPath;					//outfile

		rubberband->start(program, arguments);

		while( !rubberband->waitForFinished() ){
			//_ERRORLOG( QString( "prozessing" ));	
		}

		//open the new rubberband created file
		// file exists?
		if ( QFile( rubberResultPath ).exists() == false ) {
			_ERRORLOG( QString( "Rubberband reimporter File %1 not found" ).arg( rubberResultPath ) );
			return NULL;
		}
		
		SF_INFO soundInfoRI;
		SNDFILE* fileRI = sf_open( rubberResultPath.toLocal8Bit(), SFM_READ, &soundInfoRI);
		if ( !fileRI ) {
			_ERRORLOG( QString( "[Sample::load] Error loading file %1" ).arg( rubberResultPath ) );
		}
		
		float *pTmpBufferRI = new float[ soundInfoRI.frames * soundInfoRI.channels ];
	
		//int res = sf_read_float( file, pTmpBuffer, soundInfo.frames * soundInfo.channels );
		sf_read_float( fileRI, pTmpBufferRI, soundInfoRI.frames * soundInfoRI.channels );
		sf_close( fileRI );
	
		float *dataRI_l = new float[ soundInfoRI.frames ];
		float *dataRI_r = new float[ soundInfoRI.frames ];

        if ( soundInfoRI.channels == 1 ) {	// MONO sample
			for ( long int i = 0; i < soundInfoRI.frames; i++ ) {
				dataRI_l[i] = pTmpBufferRI[i];
				dataRI_r[i] = pTmpBufferRI[i];
			}
		} else if ( soundInfoRI.channels == 2 ) { // STEREO sample
			for ( long int i = 0; i < soundInfoRI.frames; i++ ) {
				dataRI_l[i] = pTmpBufferRI[i * 2];
				dataRI_r[i] = pTmpBufferRI[i * 2 + 1];
			}
		}
		delete[] pTmpBufferRI;

		pSample->set_frames( soundInfoRI.frames );
		pSample->__data_l = dataRI_l;
		pSample->__data_r = dataRI_r;
	
		pSample->__sample_rate = soundInfoRI.samplerate;
		pSample->__sample_is_modified = true;
		pSample->__loop_mode = loop_mode;
		pSample->__start_frame = start_frame;
		pSample->__loop_frame = loop_frame;
		pSample->__end_frame = end_frame;
		pSample->__repeats = loops;
		pSample->__use_rubber = true;
		pSample->__rubber_divider = rubber_divider;
		pSample->__rubber_c_settings = rubberbandCsettings;
		pSample->__rubber_pitch = rubber_pitch;

		//delete the tmp files
		if( QFile( outfilePath ).remove() ); 
//			_INFOLOG("remove outfile");
		if( QFile( rubberResultPath ).remove() );
//			_INFOLOG("remove rubberResultFile");

	}else///~rubberband
	{
		pSample->__data_l = new_data_l;
		pSample->__data_r = new_data_r;
	
		pSample->__sample_rate = sample_rate;
		pSample->__sample_is_modified = true;
		pSample->__loop_mode = loop_mode;
		pSample->__start_frame = start_frame;
		pSample->__loop_frame = loop_frame;
		pSample->__end_frame = end_frame;
		pSample->__repeats = loops;
	
	}
		return pSample;
}

Sample::loop_mode_t Sample::parse_loop_mode( const QString& loop_mode ) {
	char* mode = loop_mode.toLocal8Bit().data();
    for( int i=0; i<PINGPONG; i++) {
	    if( 0 == strncasecmp( mode, __loop_modes[i], sizeof(__loop_modes[i]) ) ) return (loop_mode_t)i;
    }
    return (loop_mode_t)0;
}

};

