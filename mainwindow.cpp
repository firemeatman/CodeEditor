#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "codeEditor/codeEditor.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    codeEditor = new CodeEditor(this);
    this->setCentralWidget(codeEditor);
}

MainWindow::~MainWindow()
{
    delete ui;
}
