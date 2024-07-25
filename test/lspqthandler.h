#ifndef LSPQTHANDLER_H
#define LSPQTHANDLER_H

#include <QObject>
#include <lsp/protocol.h>

namespace codeEditorTest {

class LspQtHandler : public QObject
{
    Q_OBJECT
public:
    static LspQtHandler* instance();
    explicit LspQtHandler(QObject *parent = nullptr);

protected:
    void customEvent(QEvent* e) override;
signals:
    void hoverFinished(QString& content);
    void completionSuggestFinished(LspCore::CompletionList& list);
};

}



#endif // LSPQTHANDLER_H
