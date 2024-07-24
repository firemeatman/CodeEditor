#ifndef LSPQTPROCESSCILENT_H
#define LSPQTPROCESSCILENT_H
#include <QProcess>
#include <lsp/client.h>
namespace codeEditorTest {

class LspQtProcessCilent : public JsonIOLayer
{

public:
    LspQtProcessCilent(const QString& processPath, const QStringList& args = {});
    ~LspQtProcessCilent();

private:
    QProcess* lspServerProcess{nullptr};
public:
    bool readJson(json &json) override;
    bool writeJson(json &json) override;

};

}
#endif // LSPQTPROCESSCILENT_H
