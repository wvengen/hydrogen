
#include <hydrogen/helpers/filesystem.h>
#include <hydrogen/basics/drumkit.h>

int xml_drumkit( int log_level ) {

    ___INFOLOG( "test xml drumkit validation, read and write" );

    H2Core::Drumkit* dk0 = 0;
    H2Core::Drumkit* dk1 = 0;
    H2Core::Drumkit* dk2 = 0;

    dk0 = H2Core::Drumkit::load( "./tests/data/drumkit" );
    if( !dk0 ) { return EXIT_FAILURE; }
    dk0->dump();
    if( !dk0->load_samples() ) { return EXIT_FAILURE; }
    dk0->save( "./tests/data/drumkit1.xml", true );

    dk1 = H2Core::Drumkit::load_file( "./tests/data/drumkit1.xml" );
    dk1->dump();
    if( !dk1 ) { return EXIT_FAILURE; }

    dk2 = new H2Core::Drumkit( dk1 );
    dk2->dump();
    dk2->set_name("COPY");
    if( !dk2 ) { return EXIT_FAILURE; }
    dk2->save( "./tests/data/drumkit2.xml", true );

    delete dk0;
    delete dk1;
    dk2->dump();
    delete dk2;

    H2Core::Filesystem::rm( "./tests/data/drumkit1.xml" );
    H2Core::Filesystem::rm( "./tests/data/drumkit2.xml" );

    return EXIT_SUCCESS;
}
