#ifndef MIDIMAPPINGEDITOR_H
#define MIDIMAPPINGEDITOR_H

#include <QWidget>

namespace Ui {
    class MidiMappingEditor;
}

class MidiMappingEditor : public QWidget
{
    Q_OBJECT

public:
    explicit MidiMappingEditor(QWidget *parent = 0);
    ~MidiMappingEditor();

private:
    Ui::MidiMappingEditor *ui;
};

#endif // MIDIMAPPINGEDITOR_H
