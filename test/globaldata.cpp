#include "globaldata.h"
namespace codeEditorTest {

std::shared_ptr<ProcessLanguageClient> GlobalData::lspClient = std::make_shared<ProcessLanguageClient>(R"(D:\c_workstation\soft_tool\clangd_17.0.3\bin\clangd.exe)", "");
std::shared_ptr<MapMessageHandler> GlobalData::lspHandle = std::make_shared<MapMessageHandler>();
std::shared_ptr<std::thread> GlobalData::clientThread;

GlobalData::GlobalData() {}

}
