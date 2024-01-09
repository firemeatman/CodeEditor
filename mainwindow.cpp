#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFile>
#include "codeEditor/codeEditor.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QFile file("D:/c_workstation/projects/QT-projects/CodeEditor/resource/testcode.txt");
    QString text;
    if(file.open(QIODeviceBase::ReadOnly | QIODeviceBase::Text)){
        QTextStream stream(&file);
        text = stream.readAll();
    }
    file.close();
    codeEditor = new CodeEditor(this);
    codeEditor->setPlainText(text);
    this->setCentralWidget(codeEditor);
}

MainWindow::~MainWindow()
{
    delete ui;
}
