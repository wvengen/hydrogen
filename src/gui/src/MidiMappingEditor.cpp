#include "MidiMappingEditor.h"
#include "ui_MidiMappingEditor.h"

#include <hydrogen/helpers/filesystem.h>

MidiMappingEditor::MidiMappingEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MidiMappingEditor)
{
    ui->setupUi(this);

    ui->midiMappingTree->header()->hide();

    QTreeWidgetItem* midiMappingTreeItem;
    midiMappingTreeItem = new QTreeWidgetItem( ui->midiMappingTree );
    midiMappingTreeItem->setText( 0, trUtf8( "Midi mappings" ) );

    QStringList mappings = H2Core::Filesystem::midi_mappings_list();
    QTreeWidgetItem* pMappingItem = new QTreeWidgetItem( midiMappingTreeItem );
    pMappingItem->setText( 0, trUtf8("Global") );


    for (int i = 0; i < mappings.size(); ++i) {
                pMappingItem = new QTreeWidgetItem( midiMappingTreeItem );
                pMappingItem->setText( 0, mappings[i] );
                pMappingItem->setToolTip( 0, mappings[i] );
    }

    ui->midiMappingTree->setItemExpanded( midiMappingTreeItem, true );
}

MidiMappingEditor::~MidiMappingEditor()
{
    delete ui;
}
