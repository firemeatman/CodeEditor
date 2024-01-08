#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QWidget>
#include "editorSideArea.h"
#include "breakPointArea.h"
class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    CodeEditor(QWidget* parent = nullptr);


    void setBreakComponent(bool visible);


    void setLineHighlight(int lineNum);

    int lineNumberAreaWidth() const;
    int countBreakPointNeedWidth();
private:
    int lineNum;
    int currentLinePos;
    int currentColumPos;



    EditorSideArea* editorSideArea = nullptr;
    BreakPointArea* breakPointArea = nullptr;

protected:
    void wheelEvent(QWheelEvent *e);//滚轮事件

public slots:
    void _on_cursorPositionChanged();
    void updateLineNumberArea(const QRect &rect, int dy);
    void updateLineNumberAreaWidth(int /* newBlockCount */);
};

#endif // CODEEDITOR_H
