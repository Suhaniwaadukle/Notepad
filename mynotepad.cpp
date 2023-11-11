#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printer)
#if QT_CONFIG(printdialog)
#include <QPrintDialog>
#endif // QT_CONFIG(printdialog)
#include <QPrinter>
#endif // QT_CONFIG(printer)
#endif // QT_PRINTSUPPORT_LIB
#include <QFont>
#include <QFontDialog>

#include "mynotepad.h"
#include "ui_mynotepad.h"

MyNotepad::MyNotepad(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyNotepad)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);

    connect(ui->actionNew,&QAction::triggered,this,    &MyNotepad::newDocument);
    connect(ui->actionOpen,&QAction::triggered, this, &MyNotepad::open);                connect(ui->actionSave,&QAction::triggered, this, &MyNotepad::save);      connect(ui->actionSave_as,&QAction::triggered, this, &MyNotepad::saveAs);
    connect(ui->actionPrint, &QAction::triggered, this, &MyNotepad::print);
    connect(ui->actionExit, &QAction::triggered, this, &MyNotepad::exit);     connect(ui->actionCopy, &QAction::triggered, this, &MyNotepad::copy);
    connect(ui->actionCut, &QAction::triggered, this, &MyNotepad::cut);
    connect(ui->actionPaste, &QAction::triggered, this, &MyNotepad::paste);
    connect(ui->actionUndo, &QAction::triggered, this, &MyNotepad::undo);
    connect(ui->actionRedo, &QAction::triggered, this, &MyNotepad::redo);
    connect(ui->actionFont, &QAction::triggered, this, &MyNotepad::selectFont);
    connect(ui->actionBold, &QAction::triggered, this, &MyNotepad::setFontBold);
    connect(ui->actionUnderline, &QAction::triggered, this, &MyNotepad::setFontUnderline);
    connect(ui->actionItalic, &QAction::triggered, this, &MyNotepad::setFontItalic);
    connect(ui->actionAbout, &QAction::triggered, this, &MyNotepad::about);

// Disable menu actions for unavailable features
#if !QT_CONFIG(printer)
    ui->actionPrint->setEnabled(false);
#endif

#if !QT_CONFIG(clipboard)
    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionPaste->setEnabled(false);
#endif
}

MyNotepad::~MyNotepad()
{
    delete ui;
}

void MyNotepad::newDocument()
{
    currentFile.clear();
    ui->textEdit->setText(QString());
}

void MyNotepad::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
}

void MyNotepad::save()
{
    QString fileName;
    // If we don't have a filename from before, get one.
    if (currentFile.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this, "Save");
        currentFile = fileName;
    } else {
        fileName = currentFile;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}

void MyNotepad::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    currentFile = fileName;
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}

void MyNotepad::print()
{
#if QT_CONFIG(printer)
    QPrinter printDev;
#if QT_CONFIG(printdialog)
    QPrintDialog dialog(&printDev, this);
    if (dialog.exec() == QDialog::Rejected)
        return;
#endif // QT_CONFIG(printdialog)
    ui->textEdit->print(&printDev);
#endif // QT_CONFIG(printer)
}

void MyNotepad::exit()
{
    QCoreApplication::quit();
}

void MyNotepad::copy()
{
#if QT_CONFIG(clipboard)
    ui->textEdit->copy();
#endif
}

void MyNotepad::cut()
{
#if QT_CONFIG(clipboard)
    ui->textEdit->cut();
#endif
}

void MyNotepad::paste()
{
#if QT_CONFIG(clipboard)
    ui->textEdit->paste();
#endif
}

void MyNotepad::undo()
{
     ui->textEdit->undo();
}

void MyNotepad::redo()
{
    ui->textEdit->redo();
}

void MyNotepad::selectFont()
{
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, this);
    if (fontSelected)
        ui->textEdit->setFont(font);
}

void MyNotepad::setFontUnderline(bool underline)
{
    ui->textEdit->setFontUnderline(underline);
}

void MyNotepad::setFontItalic(bool italic)
{
    ui->textEdit->setFontItalic(italic);
}

void MyNotepad::setFontBold(bool bold)
{
    bold ? ui->textEdit->setFontWeight(QFont::Bold) :
           ui->textEdit->setFontWeight(QFont::Normal);
}

void MyNotepad::about()
{
   QMessageBox::about(this, tr("About MDI"),
                tr("Notepad in QT C++ By CppBuzz.com. Users are allowed to download & modify it."
                   "text editor using QtWidgets"));
}