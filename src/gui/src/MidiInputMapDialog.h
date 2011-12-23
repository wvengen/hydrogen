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
    void on_loadButton_clicked();
    void on_saveButton_clicked();
    void on_saveAsButton_clicked();
    void on_defaultButton_clicked();

private:
    QString currentFilename;

};

#endif // MIDIINPUTMAPDIALOG_H
