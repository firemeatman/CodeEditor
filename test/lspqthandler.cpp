#include "lspqthandler.h"
#include <QCoreApplication>
#include "lspevent.h"
#include "globaldata.h"
#include <iostream>
namespace codeEditorTest {

LspQtHandler* LspQtHandler::instance()
{
    static std::shared_ptr<LspQtHandler> object = std::make_shared<LspQtHandler>(nullptr);
    return object.get();
}

LspQtHandler::LspQtHandler(QObject *parent)
    : QObject{parent}
{
    MapMessageHandler::Accessor accessor = GlobalData::lspHandle->access();
    accessor.bindAnyJsonRPC([](MessageHandler::MsgType type, value& j, string_ref method){
        QEvent::Type eventType = qEventJRPCResponse;
        switch (type) {
        case MessageHandler::MsgType::Request:
        {
            eventType = qEventJRPCRequest;
            break;
        }
        case MessageHandler::MsgType::Response:
        {
            eventType = qEventJRPCResponse;
            break;
        }
        case MessageHandler::MsgType::Notify:
        {
            eventType = qEventJRPCNotify;
            break;
        }
        case MessageHandler::MsgType::Error:
        {
            eventType = qEventJRPCError;
            //std::cout<<j<<std::endl;
            break;
        }
        default:
            break;
        }
        LspEvent* event = new LspEvent(j,method.str(),eventType);
        QCoreApplication::postEvent(LspQtHandler::instance(), event);
    });
}

void LspQtHandler::customEvent(QEvent *e)
{
    qDebug()<<"发生事件";
    LspEvent* lspEvent = dynamic_cast<LspEvent*>(e);
    if(!lspEvent){return;}

    switch (e->type()) {
    case codeEditorTest::qEventJRPCResponse:
    {
        if(lspEvent->method == METHOD_Hover){
            Hover hover = lspEvent->jObject.get<Hover>();
            QString str = QString::fromStdString(hover.contents.value);
            emit hoverFinished(str);
        }else if(lspEvent->method == METHOD_Completion){
            try {
                //std::cout<<lspEvent->jObject<<std::endl;
                // CompletionList completionList = lspEvent->jObject.get<CompletionList>();
            } catch (...) {
                //std::cout<<"异常"<<std::endl;
            }

            // emit completionSuggestFinished(completionList);
        }else if(lspEvent->method == METHOD_SignatureHelp){
            //qDebug()<<lspEvent->jObject.dump();
            SignatureHelp signatureHelp = lspEvent->jObject.get<SignatureHelp>();

        }



        break;
    }
    case codeEditorTest::qEventJRPCNotify:
    {
        if(lspEvent->method == METHOD_PublishDiagnostics){

        }
        break;
    }
    default:
        break;
    }
    e->accept();
}

}


