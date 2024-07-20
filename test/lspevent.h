#ifndef LSPEVENT_H
#define LSPEVENT_H

#include <QEvent>
#include <QString>
#include <lsp/protocol.h>

const static QEvent::Type qEventRecvMsg = (QEvent::Type)(QEvent::User + 1);

class LspEvent: public QEvent {
public:
    LspEvent(json& j, const std::string& method ,QEvent::Type type = qEventRecvMsg) : QEvent(type)
    {
        jObject = j;
        this->method = method;
    }
    json jObject;
    std::string method;
};

#endif // LSPEVENT_H
