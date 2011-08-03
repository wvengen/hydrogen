#include "MidiInputMapDialog.h"
#include "Skin.h"

#include <hydrogen/hydrogen.h>
using namespace H2Core;


MidiInputMapDialog::MidiInputMapDialog(QWidget *parent)
    : QDialog( parent )
    , Object( "MidiINputMapDialog" )
{
            setupUi( this );

            setWindowTitle( trUtf8( "Preferences" ) );
            setWindowIcon( QPixmap( Skin::getImagePath()  + "/icon16.png" ) );

            setMinimumSize( width(), height() );
            setMaximumSize( width(), height() );

}
