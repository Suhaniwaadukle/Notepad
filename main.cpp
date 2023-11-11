#include "mynotepad.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication EditorApp(argc, argv);
    MyNotepad Editor;
    Editor.show();

    return EditorApp.exec();
}