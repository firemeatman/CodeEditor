#ifndef CHIGHLIGHT_H
#define CHIGHLIGHT_H


#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QRegularExpression>
namespace codeEditor {

class CHighlight : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit CHighlight(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    static QString keyWordPattern;
    static QString macroPattern;
    static QString varPattern;


};

}


#endif // CHIGHLIGHT_H
