#ifndef LSPEVENT_H
#define LSPEVENT_H

#include <QEvent>
#include <QString>
#include <lsp/protocol.h>
namespace codeEditorTest {

const static QEvent::Type qEventJRPCRequest = (QEvent::Type)(QEvent::User + 1);
const static QEvent::Type qEventJRPCResponse = (QEvent::Type)(QEvent::User + 2);
const static QEvent::Type qEventJRPCNotify = (QEvent::Type)(QEvent::User + 3);
const static QEvent::Type qEventJRPCError = (QEvent::Type)(QEvent::User + 4);

class LspEvent: public QEvent {
public:
    LspEvent(json& j, const std::string& method ,QEvent::Type type = qEventJRPCResponse) : QEvent(type)
    {
        jObject = j;
        this->method = method;
    }
    json jObject;
    std::string method;
};

}


#endif // LSPEVENT_H
