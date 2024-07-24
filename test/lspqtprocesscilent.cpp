#include "lspqtprocesscilent.h"
namespace codeEditorTest {

LspQtProcessCilent::LspQtProcessCilent(const QString& processPath, const QStringList& args)
{
    this->lspServerProcess =  new QProcess();
    lspServerProcess->start(processPath, args);
    lspServerProcess->waitForStarted(5000);
}

LspQtProcessCilent::~LspQtProcessCilent()
{
    if(lspServerProcess){
        delete lspServerProcess;
        lspServerProcess = nullptr;
    }
}

bool LspQtProcessCilent::readJson(json &json)
{
    return false;
}

bool LspQtProcessCilent::writeJson(json &json)
{
    return false;
}

}
