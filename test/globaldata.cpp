#include "globaldata.h"
namespace codeEditorTest {


std::unique_ptr<PipJsonIO> GlobalData::jsonIO = std::make_unique<PipJsonIO>(R"(D:\c_workstation\soft_tool\clangd_17.0.3\bin\clangd.exe)", "");
std::unique_ptr<MapMessageHandler> GlobalData::lspHandle = std::make_unique<MapMessageHandler>();
std::unique_ptr<LanguageClient> GlobalData::lspClient = std::make_unique<LanguageClient>(*lspHandle, *jsonIO);
std::unique_ptr<std::thread> GlobalData::clientThread;

GlobalData::GlobalData() {}

}
