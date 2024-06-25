#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QWidget>
#include <QPixmap>
#include "lineNumberArea.h"
#include "breakPointArea.h"
#include "cHighLight.h"

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    CodeEditor(QWidget* parent = nullptr);
    ~CodeEditor();

    void setLineComponentVisible(bool visible = true);
    void setBreakComponentVisible(bool visible = false);

    void addBreak(int line);
    void removeBreak(int line);
    void addNextFlag(int line);
    void removeNextFlag(int line);
    void addError(int line, int col, QString& errStr);
    void removeError(int line, int col);

    void setBreakComponentImg(QString& resPath, PointType type);

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

    void updateSideArea(const QRect &rect, int dy);
    void updateSideAreaWidth();

    void highLightSelectedLine();

protected:
    void wheelEvent(QWheelEvent *e);//滚轮事件

signals:
    void breakChanged(int line, bool haveBreakPoint);
    void lineChanged(int rangeLeft, int rangeRight, bool isAddLine);

public slots:
    void _on_contentsChange(int position, int charsRemoved, int charsAdded);
    void _on_cursorPositionChanged();
    void _on_blockCountChanged(int newBlockCount);
    void _on_updateRequest(const QRect &rect, int dy);
};

#endif // CODEEDITOR_H
