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
    static std::shared_ptr<ProcessLanguageClient> lspClient;
    static std::shared_ptr<MapMessageHandler> lspHandle;
    static std::shared_ptr<std::thread> clientThread;
};

}
#endif // GLOBALDATA_H
