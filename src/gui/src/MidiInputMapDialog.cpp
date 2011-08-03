#include "MidiInputMapDialog.h"
#include "Skin.h"

#include <hydrogen/hydrogen.h>
using namespace H2Core;


MidiInputMapDialog::MidiInputMapDialog(QWidget *parent)
    : QDialog( parent )
    , Object( "MidiInputMapDialog" )
{
            setupUi( this );

            setWindowTitle( trUtf8( "Midi input map editor" ) );
            setWindowIcon( QPixmap( Skin::getImagePath()  + "/icon16.png" ) );

            setMinimumSize( width(), height() );
            setMaximumSize( width(), height() );

            helpLabel->setText("This editor enables you to define the midi note on which each instrument responds.");
}



void MidiInputMapDialog::on_okBtn_clicked()
{
    accept();
}

void MidiInputMapDialog::on_cancelBtn_clicked()
{
    accept();
}
