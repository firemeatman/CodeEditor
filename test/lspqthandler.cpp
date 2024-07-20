#include "lspqthandler.h"
#include "lspevent.h"

LspQtHandler *LspQtHandler::instance()
{
    static LspQtHandler* object = new LspQtHandler();
    return object;
}

LspQtHandler::LspQtHandler(QObject *parent)
    : QObject{parent}
{}

void LspQtHandler::customEvent(QEvent *e)
{
    switch (e->type()) {
    case qEventRecvMsg:
    {
        LspEvent* lspEvent = dynamic_cast<LspEvent*>(e);
        if(!lspEvent){break;}
        if(lspEvent->method == METHOD_Hover){
            Hover hover = lspEvent->jObject.get<Hover>();
            QString str = QString::fromStdString(hover.contents.value);
            emit hoverFinished(str);
        }

        break;
    }
    default:
        break;
    }
    e->accept();
}
