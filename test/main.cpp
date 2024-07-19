#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
// namespace codeEditorTest {



// }

void init(){

}

void relase(){

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    codeEditorTest::MainWindow w;
    w.show();
    int flag = a.exec();

    return flag;
}
