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
    QString arrow = "D:/c_workstation/projects/QT-projects/CodeEditor/resource/right_arrow_icon_48px.png";
    codeEditor->setBreakComponentImg(arrow, PointType::Arrow);
    codeEditor->addBreak(5);
    codeEditor->addNextFlag(5);

    codeEditor->addBreak(6);
    codeEditor->addError(10,0,arrow);
    codeEditor->addNextFlag(7);

    // codeEditor->setBreakComponentVisible(false);
    // codeEditor->setLineComponentVisible(false);
    this->setCentralWidget(codeEditor);
}

MainWindow::~MainWindow()
{
    delete ui;
}
