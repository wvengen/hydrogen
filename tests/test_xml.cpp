
#include <hydrogen/helpers/filesystem.h>
#include <hydrogen/basics/drumkit.h>

int xml_drumkit( int log_level ) {

    ___INFOLOG( "test xml drumkit validation, read and write" );

    H2Core::Drumkit* dk = 0;
    dk = H2Core::Drumkit::load_file( "./tests/data/drumkit.xml" );
    if( !dk ) { return EXIT_FAILURE; }
    dk->dump();
    dk->save( "./tests/data/drumkit2.xml", true );
    delete dk;
    dk = 0;
    dk = H2Core::Drumkit::load_file( "./tests/data/drumkit2.xml" );
    if( !dk ) { return EXIT_FAILURE; }
    H2Core::Filesystem::rm( "./tests/data/drumkit2.xml" );

    return EXIT_SUCCESS;
}
