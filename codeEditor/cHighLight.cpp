#include "cHighLight.h"

QString CHighlight::keyWordPattern = QStringLiteral("\\b(double|float|int|short|long|unsigned|signed|struct|union|void|enum|\
return|\
auto|extern|register|static\
const|sizeof|typedef|volatile\
for|do|while|continue|break|if|else|goto|switch|case|default)\\b");

CHighlight::CHighlight(QTextDocument *parent)
    : QSyntaxHighlighter{parent}
{
    HighlightingRule highlightRule;
    //关键字高亮
    HighlightingRule keys_rule;
    keys_rule.pattern.setPattern(keyWordPattern);
    keys_rule.format.setForeground(QColor(0, 0, 255));
    keys_rule.format.setFontWeight(QFont::Bold);
    highlightingRules.append(keys_rule);
    //双引号范围高亮
    HighlightingRule str_rule;
    str_rule.pattern.setPattern("\".*\"");
    str_rule.format.setForeground(QColor(200, 100, 0));
    str_rule.format.setFontWeight(QFont::Bold);
    highlightingRules.append(str_rule);

    // 单行注释
    highlightRule.pattern.setPattern("//.*");
    highlightRule.format.setForeground(QColor("#0ACF00"));
    highlightRule.format.setFontWeight(QFont::Bold);
    highlightingRules.append(str_rule);
}

void CHighlight::highlightBlock(const QString &text)
{
    //QRegularExpression
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    // 跨行的复杂匹配

    QTextCharFormat multiLineCommentFormat;
    multiLineCommentFormat.setForeground(QColor("#0ACF00"));

    QRegularExpression startExpression("/\\*");
    QRegularExpression endExpression("\\*/");

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(startExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch endMatch;
        int endIndex = text.indexOf(endExpression, startIndex, &endMatch);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + endMatch.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(startExpression,
                                  startIndex + commentLength);
    }


}
