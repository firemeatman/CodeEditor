#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QWidget>
#include "editorSideArea.h"
#include "breakPointArea.h"
#include "cHighLight.h"

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    CodeEditor(QWidget* parent = nullptr);
    ~CodeEditor();

    void setBreakComponent(bool visible);

    // 常用的函数

    void setLineHighlight(int lineNum);
    /* 根据鼠标指针位置计算对应编辑器中的文本位置
     * return: QPonit 列号、行号
     */
    QPoint countTextPosByMousePos(QPoint& mousePos);


    // 计算小组件需要的宽度
    int countLineNumberWigetWidth() const;
    int countBreakPointWigetWidth();
private:
    // int lineNum;
    // int currentLinePos;
    // int currentColumPos;

    EditorSideArea* editorSideArea = nullptr;
    BreakPointArea* breakPointArea = nullptr;
    CHighlight* cHighLight = nullptr;

protected:
    void wheelEvent(QWheelEvent *e);//滚轮事件

public slots:
    void _on_cursorPositionChanged();
    void updateLineNumberArea(const QRect &rect, int dy);
    void updateLineNumberAreaWidth(int /* newBlockCount */);
};

#endif // CODEEDITOR_H
