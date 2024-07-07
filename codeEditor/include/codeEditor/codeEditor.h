#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QWidget>
#include <QPixmap>
#include <codeEditor/lineNumberArea.h>
#include <codeEditor/breakPointArea.h>
#include <codeEditor/cHighLight.h>
namespace codeEditor {

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget* parent = nullptr);
    ~CodeEditor();

    void setLineComponentVisible(bool visible = true);
    void setBreakComponentVisible(bool visible = false);
    void setBreakComponentImg(QString& resPath, PointType type);

    void addBreak(int line);
    void removeBreak(int line);
    void addNextFlag(int line);
    void removeNextFlag(int line);
    void addWarn(int line, int col, QString& warnStr);
    void removeWarn(int line, int col);
    void addError(int line, int col, QString& errStr);
    void removeError(int line, int col);


    // void setHelpInfoWindow(QPoint& pos, QWidget* window = nullptr);
    // void removeHelpInfoWindow(QWidget* window = nullptr);
    // void setCodeCompletionList(QPoint& pos);
    // void removeCodeCompletionList(QPoint& pos);

    // 根据鼠标指针位置计算对应编辑器中的文本位置；返回行号列号
    QPoint countTextPosByMousePos(QPoint& mousePos); //弃用

public:
    // 计算小组件需要的宽度
    int countLineNumberWigetWidth() const;
    int countBreakPointWigetWidth();
private:

    LineNumberArea* lineNumberArea = nullptr;
    BreakPointArea* breakPointArea = nullptr;
    CHighlight* cHighLight = nullptr;

    QTextCharFormat errorLineFormat;
    QTextCharFormat errorUnderLineFormat;

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

public slots:
    void _on_contentsChange(int position, int charsRemoved, int charsAdded);
    void _on_cursorPositionChanged();
    void _on_blockCountChanged(int newBlockCount);
    void _on_updateRequest(const QRect &rect, int dy);
};

}


#endif // CODEEDITOR_H
