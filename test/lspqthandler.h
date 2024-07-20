#ifndef LSPQTHANDLER_H
#define LSPQTHANDLER_H

#include <QObject>

class LspQtHandler : public QObject
{
    Q_OBJECT
public:
    static LspQtHandler* instance();
private:
    explicit LspQtHandler(QObject *parent = nullptr);

protected:
    void customEvent(QEvent* e) override;
signals:
    void hoverFinished(QString& content);
};

#endif // LSPQTHANDLER_H
