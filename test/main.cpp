#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include "globaldata.h"

// namespace codeEditorTest {


// }

using namespace codeEditorTest;

static void init(){
    GlobalData::clientThread = std::make_unique<std::thread>([](){
        GlobalData::lspClient->safeLoop();
    });
}

static void relase(){
    GlobalData::lspClient->Exit();
    GlobalData::lspClient->requestStopLoop();

    if(GlobalData::clientThread->joinable()){
        GlobalData::clientThread->join();
    }
}

struct AppResource
{
    AppResource() {
        init();
    }
    ~AppResource(){
        relase();
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    AppResource res;
    w.show();
    int flag = a.exec();
    return flag;
}
