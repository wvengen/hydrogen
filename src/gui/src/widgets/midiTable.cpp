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

#include "../Skin.h"
#include "MidiSenseWidget.h"
#include "midiTable.h"

#include <hydrogen/midi_action_map.h>
#include <hydrogen/Preferences.h>
#include <hydrogen/globals.h>
#include <hydrogen/midi_action.h>
#include <hydrogen/hydrogen.h>

const char* MidiTable::__class_name = "MidiTable";

/**
* @class MidiTable
*
* @brief A widget which maps midi events to other midi events or actions.
*
*
* The MidiTable widget gets used in two places: the MidiAction dialog
* and the midi input mapping dialog. It enables a user to create a relation
* between a midi event and an action or between two events.
*
* @author Sebastian Moors
*
*/

MidiTable::MidiTable( QWidget *pParent )
 : QTableWidget( pParent )
 , Object( __class_name )
{
	__row_count = 0;
	setupMidiTable();

	m_pUpdateTimer = new QTimer( this );
	currentMidiAutosenseRow = 0;
}


MidiTable::~MidiTable()
{
	for( int myRow = 0; myRow <=  __row_count ; myRow++ ) {
		delete cellWidget( myRow, 0 );
		delete cellWidget( myRow, 1 );
		delete cellWidget( myRow, 2 );
		delete cellWidget( myRow, 3 );
	}
}

void MidiTable::setRole( H2_MIDI_TABLE_ROLE role )
{
    this->__role = role;
}

void MidiTable::midiSensePressed( int row ){

	currentMidiAutosenseRow = row;
	MidiSenseWidget mW( this );
	mW.exec();

	QComboBox * eventCombo =  dynamic_cast <QComboBox *> ( cellWidget( row, 1 ) );
	QSpinBox * eventSpinner = dynamic_cast <QSpinBox *> ( cellWidget( row, 2 ) );


	eventCombo->setCurrentIndex( eventCombo->findText( mW.lastMidiEvent ) );
	eventSpinner->setValue( mW.lastMidiEventParameter );

	m_pUpdateTimer->start( 100 );	
}


void MidiTable::updateTable()
{
	if( __row_count > 0 ) {
		QComboBox * eventCombo =  dynamic_cast <QComboBox *> ( cellWidget( __row_count - 1, 1 ) );
		QComboBox * actionCombo = dynamic_cast <QComboBox *> ( cellWidget( __row_count - 1, 3 ) );

		if( eventCombo == NULL || actionCombo == NULL) return;

		if( actionCombo->currentText() != "" && eventCombo->currentText() != "" ) {
			insertNewRow("", "", 0, 0);
		}
	}
}


void MidiTable::insertNewRow(QString actionString , QString eventString, int eventParameter , int actionParameter)
{
        MidiActionManager *aH = MidiActionManager::get_instance();

	insertRow( __row_count );
	
	int oldRowCount = __row_count;

	++__row_count;

	

	QPushButton *midiSenseButton = new QPushButton(this);
	midiSenseButton->setIcon(QIcon(Skin::getImagePath() + "/preferencesDialog/rec.png"));
	midiSenseButton->setToolTip( trUtf8("press button to record midi event") );

	QSignalMapper *signalMapper = new QSignalMapper(this);

	connect(midiSenseButton, SIGNAL( clicked()), signalMapper, SLOT( map() ));
	signalMapper->setMapping( midiSenseButton, oldRowCount );
	connect( signalMapper, SIGNAL(mapped( int ) ),
         this, SLOT( midiSensePressed(int) ) );
	setCellWidget( oldRowCount, 0, midiSenseButton );



	QComboBox *eventBox = new QComboBox();
	connect( eventBox , SIGNAL( currentIndexChanged( int ) ) , this , SLOT( updateTable() ) );

        if( __role == H2_MIDI_ACTION_MAP )
        {
            eventBox->insertItems( oldRowCount , aH->getEventList() );
            eventBox->setCurrentIndex( eventBox->findText(eventString) );
        } else {
            eventBox->insertItem( oldRowCount , "NOTE" );
        }

        setCellWidget( oldRowCount, 1, eventBox );
	
	
	QSpinBox *eventParameterSpinner = new QSpinBox();
	setCellWidget( oldRowCount , 2, eventParameterSpinner );
	eventParameterSpinner->setMaximum( 999 );
	eventParameterSpinner->setValue( eventParameter );


	QComboBox *actionBox = new QComboBox();
	connect( actionBox , SIGNAL( currentIndexChanged( int ) ) , this , SLOT( updateTable() ) );


        if( __role == H2_MIDI_ACTION_MAP )
        {
            actionBox->insertItems( oldRowCount, aH->getActionList());
            actionBox->setCurrentIndex ( actionBox->findText( actionString ) );
        } else {
            actionBox->insertItem( oldRowCount , "NOTE" );
        }


        setCellWidget( oldRowCount , 3, actionBox );
	

	QSpinBox *actionParameterSpinner = new QSpinBox();
	
	setCellWidget( oldRowCount , 4, actionParameterSpinner );
	actionParameterSpinner->setValue( actionParameter);
	actionParameterSpinner->setMaximum( 999 );


}


void MidiTable::loadFromFile( QString filename )
{
    if( __role == H2_MIDI_EVENT_MAP ){
        //load it..
    }

}

void MidiTable::saveToFile( QString filename )
{
    if( __role == H2_MIDI_EVENT_MAP ){
        //save it..
    }

}

void MidiTable::setupMidiTable()
{
	MidiActionMap *mM = MidiActionMap::get_instance();

	QStringList items;

        __row_count = 0;

        if( __role == H2_MIDI_ACTION_MAP ){
            items << "" << trUtf8("Event")  <<  trUtf8("Param.")  <<  trUtf8("Action") <<  trUtf8("Param.") ;
        } else {
            items << "" << trUtf8("Input Event")  <<  trUtf8("Param.")  <<  trUtf8("Output Event") <<  trUtf8("Param.") ;
        }

	setRowCount( 0 );
    	setColumnCount( 5 );

	verticalHeader()->hide();

	setHorizontalHeaderLabels( items );
	
        /*
        setFixedWidth( 600 );

	setColumnWidth( 0 , 25 );
	setColumnWidth( 1 , 155 );
	setColumnWidth( 2, 73 );
	setColumnWidth( 3, 175 );
	setColumnWidth( 4 , 73 );
        */

	bool ok;
	std::map< QString , MidiAction* > mmcMap = mM->getMMCMap();
	std::map< QString , MidiAction* >::iterator dIter( mmcMap.begin() );

        for( int note = 0; note < 128; note++ ) {
                MidiAction * pAction = mM->getNoteAction( note );
                QString actionParameter;
                int actionParameterInteger = 0;

                actionParameter = pAction->getParameter1();
                actionParameterInteger = actionParameter.toInt(&ok,10);


                if ( pAction->getType() == "NOTHING" ) continue;

                insertNewRow(pAction->getType() , "NOTE" , note , actionParameterInteger );
        }
	
        if( __role == H2_MIDI_ACTION_MAP )
        {

            for( dIter = mmcMap.begin(); dIter != mmcMap.end(); dIter++ ) {
                    MidiAction * pAction = dIter->second;
                    QString actionParameter;
                    int actionParameterInteger = 0;

                    actionParameter = pAction->getParameter1();
                    actionParameterInteger = actionParameter.toInt(&ok,10);

                    insertNewRow(pAction->getType() , dIter->first , 0 , actionParameterInteger );
            }


            for( int parameter = 0; parameter < 128; parameter++ ){
                    MidiAction * pAction = mM->getCCAction( parameter );
                    QString actionParameter;
                    int actionParameterInteger = 0;

                    actionParameter = pAction->getParameter1();
                    actionParameterInteger = actionParameter.toInt(&ok,10);

                    if ( pAction->getType() == "NOTHING" ) continue;

                    insertNewRow(pAction->getType() , "CC" , parameter , actionParameterInteger );
            }
        }
	
	insertNewRow( "", "", 0, 0 );
}


void MidiTable::saveMidiTable()
{
	MidiActionMap *mM = MidiActionMap::get_instance();
	
	for ( int row = 0; row < __row_count; row++ ) {

		QComboBox * eventCombo =  dynamic_cast <QComboBox *> ( cellWidget( row, 1 ) );
		QSpinBox * eventSpinner = dynamic_cast <QSpinBox *> ( cellWidget( row, 2 ) );
		QComboBox * actionCombo = dynamic_cast <QComboBox *> ( cellWidget( row, 3 ) );
		QSpinBox * actionSpinner = dynamic_cast <QSpinBox *> ( cellWidget( row, 4 ) );

		QString eventString;
		QString actionString;
                QString outputEventString;

                if( !eventCombo->currentText().isEmpty() && !actionCombo->currentText().isEmpty() ){

                    if( __role == H2_MIDI_ACTION_MAP ){

                            eventString = eventCombo->currentText();

                            actionString = actionCombo->currentText();

                            MidiAction* pAction = new MidiAction( actionString );

                            if( actionSpinner->cleanText() != ""){
                                    pAction->setParameter1( actionSpinner->cleanText() );
                            }

                            if( eventString.left(2) == "CC" ){
                                    mM->registerCCEvent( eventSpinner->cleanText().toInt() , pAction );
                            }

                            if( eventString.left(3) == "MMC" ){
                                    mM->registerMMCEvent( eventString , pAction );
                            }

                            if( eventString.left(4) == "NOTE" ){
                                    mM->registerNoteEvent( eventSpinner->cleanText().toInt() , pAction );
                            }
                    } else {

                            eventString = eventCombo->currentText();

                            outputEventString = actionCombo->currentText();

                            MidiAction* pAction = new MidiAction( actionString );

                            if( eventString.left(4) == "NOTE" ){
                                    mM->registerNoteEvent( eventSpinner->cleanText().toInt() , pAction );
                            }
                      }
                }
	}
}
