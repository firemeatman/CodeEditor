#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QWidget>
#include <QPixmap>
#include <QMap>
#include <QApplication>
#include <codeEditor/lineNumberArea.h>
#include <codeEditor/breakPointArea.h>
#include <codeEditor/cHighLight.h>
namespace codeEditor {

struct Loaction
{
    int line{0};
    int col{0};
    Loaction() {}
};
struct Token
{
    Loaction pos;
    int type{0};
    Token() {}
};
struct CodeSuggest{
    int type{0};
    QString str;
};
struct DiagnosisInfo
{
    Loaction pos;
    QString str;
};

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget* parent = nullptr);
    ~CodeEditor();

    // 设置
    void setLineComponentVisible(bool visible = true);
    void setBreakComponentVisible(bool visible = false);
    void setBreakComponentImg(QString& resPath, PointType type);
    void setTokenFormat(int tokenType, QTextCharFormat format);
    void setWordJumpLink(bool enable = true, Qt::KeyboardModifiers triggerBtn = Qt::ControlModifier);

    // 语言特性
    void addHoverInfoWindow(int line, int col, QString& content);
    void updateHighLight(const QList<Token>& tokenList);
    void updateHighLight(const QList<int>& lspTokenList);
    void addCodeCompletionSuggestions(Loaction pos, const QList<CodeSuggest>& suggestions);
    void addDiagnosis(const QList<DiagnosisInfo>& infoList);
    void clearDiagnosis();

    // 小组件
    void addBreak(int line);
    void removeBreak(int line);
    void addNextFlag(int line);
    void removeNextFlag(int line);
    void addWarn(int line, int col, QString& warnStr);
    void removeWarn(int line, int col);
    void addError(int line, int col, QString& errStr);
    void removeError(int line, int col);

public:
    // 计算小组件需要的宽度
    int countLineNumberWigetWidth() const;
    int countBreakPointWigetWidth() const;
    int getLeftComWidth() const;
    // 根据（全局）鼠标指针位置计算对应编辑器中的光标
    QTextCursor cursorPosByGlobalMousePos(QPoint g_mousePos);
    QTextCursor cursorPosByMousePos(QPoint mousePos, QWidget* from);
private:

    LineNumberArea* lineNumberArea = nullptr;
    BreakPointArea* breakPointArea = nullptr;
    CHighlight* cHighLight = nullptr;

    QTextCharFormat errorLineFormat;
    QTextCharFormat errorUnderLineFormat;
    QMap<int, QTextCharFormat> tokenFormatMap;

    // 更新小组件
    void updateSideArea();
    // 根据行列生成光标对象
    QTextCursor generateCursor(int line, int col);
    // 高亮行
    void highLightSelectedLine();
    void highlightLine(int line, QTextCharFormat& format);
    // 给单词添加下划线
    void addWordUnderline(int line, int col, QTextCharFormat& format);
    void addWordUnderline(QTextCursor &cursor, QTextCharFormat& format);

protected:
    void wheelEvent(QWheelEvent *e);
    void resizeEvent(QResizeEvent *event);

signals:
    void breakChanged(int line, bool haveBreakPoint);
    void lineChanged(int rangeLeft, int rangeRight, bool isAddLine);
    void hoverCursorTriggered(int line, int col);


public slots:
    void _on_contentsChange(int position, int charsRemoved, int charsAdded);
    void _on_cursorPositionChanged();
    void _on_blockCountChanged(int newBlockCount);
    void _on_updateRequest(const QRect &rect, int dy);
};

}


#endif // CODEEDITOR_H
