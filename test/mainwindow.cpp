#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFile>
namespace codeEditorTest {
using namespace codeEditor;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    // , ui(new Ui::MainWindow)
{
    // ui->setupUi(this);
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
    QString warn = "D:/c_workstation/projects/QT-projects/CodeEditor/resource/warn_512px.png";
    QString error = "D:/c_workstation/projects/QT-projects/CodeEditor/resource/error_512px.png";
    codeEditor->setBreakComponentImg(arrow, PointType::Arrow);
    codeEditor->setBreakComponentImg(warn, PointType::Warn);
    codeEditor->setBreakComponentImg(error, PointType::Error);
    codeEditor->addBreak(5);
    codeEditor->addNextFlag(5);

    codeEditor->addBreak(6);
    codeEditor->addError(10,4,arrow);
    codeEditor->addNextFlag(7);
    codeEditor->addWarn(8,0,arrow);

    // codeEditor->setBreakComponentVisible(false);
    // codeEditor->setLineComponentVisible(false);
    this->setCentralWidget(codeEditor);
    this->resize(1000,600);
}

MainWindow::~MainWindow()
{
    // delete ui;
}

}
