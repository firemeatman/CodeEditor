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
    static std::unique_ptr<LspCore::LanguageClient> lspClient;
    static std::unique_ptr<LspCore::MapMessageHandler> lspHandle;
    static std::unique_ptr<LspCore::PipJsonIO> jsonIO;
    static std::unique_ptr<std::thread> clientThread;
};

}
#endif // GLOBALDATA_H
