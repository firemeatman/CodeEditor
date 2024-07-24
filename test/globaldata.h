#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <thread>
#include <memory>
#include <lsp/client.h>
namespace codeEditorTest {

class GlobalData
{
public:
    GlobalData();
    static std::unique_ptr<LanguageClient> lspClient;
    static std::unique_ptr<MapMessageHandler> lspHandle;
    static std::unique_ptr<PipJsonIO> jsonIO;
    static std::unique_ptr<std::thread> clientThread;
};

}
#endif // GLOBALDATA_H
