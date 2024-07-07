#include "mainwindow.h"

#include <QApplication>
// namespace codeEditorTest {



// }

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    codeEditorTest::MainWindow w;
    w.show();
    return a.exec();
}
