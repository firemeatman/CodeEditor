#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "globaldata.h"

// namespace codeEditorTest {


// }

using namespace codeEditorTest;

void init(){
    GlobalData::clientThread = std::make_shared<std::thread>([](){
        GlobalData::lspClient->loop(*GlobalData::lspHandle);
        std::this_thread::sleep_for(std::chrono::minutes(50));
    });
}

void relase(){

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    init();
    MainWindow w;
    w.show();
    int flag = a.exec();
    relase();
    return flag;
}
