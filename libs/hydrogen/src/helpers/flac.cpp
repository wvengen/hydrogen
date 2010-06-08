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

#include <hydrogen/config.h>
#include <hydrogen/helpers/flac.h>
#include <hydrogen/basics/sample.h>

#ifdef H2CORE_HAVE_FLAC

#include <FLAC++/all.h>

namespace H2Core
{
#if !defined(FLAC_API_VERSION_CURRENT) || FLAC_API_VERSION_CURRENT < 8
#define LEGACY_FLAC
#else
#undef LEGACY_FLAC
#endif

class FLACLoader : public FLAC::Decoder::File, public Object
{
    H2_OBJECT
public:
	FLACLoader();
	~FLACLoader();

	void load( const QString& filename );
	Sample* get_sample();

protected:
	virtual ::FLAC__StreamDecoderWriteStatus write_callback( const ::FLAC__Frame *frame, const FLAC__int32 * const buffer[] );
	virtual void metadata_callback( const ::FLAC__StreamMetadata *metadata );
	virtual void error_callback( ::FLAC__StreamDecoderErrorStatus status );

private:
	float* __data_l;
	float* __data_r;
    unsigned __frames;
	QString __filename;
};


const char* FLACLoader::__class_name = "FLACLoader";

FLACLoader::FLACLoader() : Object( __class_name ) { }

FLACLoader::~FLACLoader() { }

::FLAC__StreamDecoderWriteStatus FLACLoader::write_callback( const ::FLAC__Frame *frame, const FLAC__int32 * const buffer[] ) {
	int channels = get_channels();
	if ( ( channels!=1 ) && ( channels!=2 ) ) {
		ERRORLOG( QString( "wrong number of channels. channels=%1" ).arg( channels ) );
		return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
	}
    float factor;
	int bits = get_bits_per_sample();
	if ( bits==16 ) {
        factor = 32768.0f;
    } else if (bits==24) {
        factor = 8388608.0f;
    } else {
		ERRORLOG( QString( "FLAC format error. bits per sample %1 not handled" ).arg( bits ) );
		return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
	}
	__frames = frame->header.blocksize;
	__data_l = new float[__frames];
	__data_r = new float[__frames];
    float* l = __data_l;
    float* r = __data_r;
    const FLAC__int32* out = buffer[__frames];
	if ( channels==1 ) {
        const FLAC__int32* data = buffer[0];
        for ( ; data!=out; data++ ) {
            *l = (float)*data/factor;
            *r = (float)*data/factor;
            l++;
            r++;
        }
    } else {
        const FLAC__int32* data_l = buffer[0];
        const FLAC__int32* data_r = buffer[1];
        for ( ; data_l!=out; ) {
            *l = (float)*data_l/factor;
            *r = (float)*data_r/factor;
            data_l+=2;
            data_r+=2;
            l++;
            r++;
        }
    }
	return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

void FLACLoader::metadata_callback( const ::FLAC__StreamMetadata *metadata ) { UNUSED( metadata ); }

void FLACLoader::error_callback( ::FLAC__StreamDecoderErrorStatus status ) { UNUSED( status ); ERRORLOG( "[error_callback]" ); }

void FLACLoader::load( const QString& sFilename ) {
	__filename = sFilename;
	set_metadata_ignore_all();
#ifdef LEGACY_FLAC
	set_filename( sFilename.toLocal8Bit() );
	State s=init();
	if ( s!=FLAC__FILE_DECODER_OK ) {
        ERRORLOG( "Error in init()" );
        return;
    }
	if ( process_until_end_of_file() == false ) {
		ERRORLOG( "Error in process_until_end_of_file(). filename : " + __filename );
        return;
	}
#else
	FLAC__StreamDecoderInitStatus s = init( sFilename.toLocal8Bit() );
	if ( s!=FLAC__STREAM_DECODER_INIT_STATUS_OK ) {
        ERRORLOG( "Error in init()" );
        return;
    }
	if ( process_until_end_of_stream() == false ) {
		ERRORLOG( "Error in process_until_end_of_stream() filename : " + __filename );
        return;
	}
#endif
}

Sample* FLACLoader::get_sample() {
	if ( __frames==0 ) return NULL;
	Sample *sample = new Sample( __frames, __filename, get_sample_rate(), __data_l, __data_r );
    __data_l=0;
    __data_r=0;
	return sample;
}

/****************************************************************/

const char* FLACFile::__class_name = "FLACFile";

FLACFile::FLACFile() : Object( __class_name ) { }

FLACFile::~FLACFile() { }

Sample* FLACFile::load( const QString& sFilename ) {
	FLACLoader *loader = new FLACLoader();
	loader->load( sFilename );
	Sample *sample = loader->get_sample();
	delete loader;
	return sample;
}

};

};
#else

namespace H2Core
{
const char* FLACFile::__class_name = "FLACFile";
}

#endif // H2CORE_HAVE_FLAC

