#ifndef MIDIINPUTMAPDIALOG_H
#define MIDIINPUTMAPDIALOG_H

#include <QDialog>

#include "ui_MidiInputMapper_UI.h"

#include <hydrogen/object.h>

class MidiInputMapDialog : public QDialog, private Ui_MidiInputMapper_UI, public H2Core::Object
{
    Q_OBJECT
public:
    explicit MidiInputMapDialog(QWidget *parent = 0);

signals:

private slots:
    void on_okBtn_clicked();
    void on_cancelBtn_clicked();
};

#endif // MIDIINPUTMAPDIALOG_H
