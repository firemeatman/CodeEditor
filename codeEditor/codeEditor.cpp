#include "codeEditor.h"

#include <QApplication>
#include <QDebug>
#include <QScrollBar>
CodeEditor::CodeEditor(QWidget* parent):
    QPlainTextEdit{parent}
{
    this->setPlainText("\nint mian()\n{\n\treturn 0;\n}\n");
    // 设置换行、tab显示宽度
    this->setWordWrapMode(QTextOption::NoWrap);
    this->setTabStopDistance(40);

    QFont gloableFont("Consolas", 12);
    this->setFont(gloableFont);
    this->setCursorWidth(2);
    editorSideArea = new EditorSideArea(this);
    editorSideArea->setBoundingTextEdit(this);

    breakPointArea = new BreakPointArea(this);
    breakPointArea->setBoundingTextEdit(this);

    QRect frame = breakPointArea->frameGeometry();
    breakPointArea->resize(20, this->viewport()->height());
    editorSideArea->move(frame.x()+20, editorSideArea->y());
    editorSideArea->resize(editorSideArea->width(), this->viewport()->height());

    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, &CodeEditor::_on_cursorPositionChanged);

    updateLineNumberAreaWidth(0);
}

void CodeEditor::setLineHighlight(int lineNum)
{
    QList<QTextEdit::ExtraSelection> extraSelections;//提供一种方式显示选择的文本
    extraSelections = this->extraSelections();//获取之前高亮的设置
    QTextEdit::ExtraSelection selection;
    QBrush bursh(QColor::fromString("#FFE573"));
    selection.format.setBackground(bursh);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = this->textCursor();
    // selection.cursor.movePosition(QTextCursor::Up);//光标移动到某一行。此处移动到上一行，上一行将高亮。
    extraSelections.append(selection);
    this->setExtraSelections(extraSelections);
}
int CodeEditor::lineNumberAreaWidth() const
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

int CodeEditor::countBreakPointNeedWidth()
{
    //int breakpointNeedWidth = fontMetrics().height() + 5;
    int breakpointNeedWidth = 20;
    return breakpointNeedWidth;
}

void CodeEditor::wheelEvent(QWheelEvent *e)
{
    QPoint numDegrees = e->angleDelta() / 8;
    Qt::KeyboardModifiers keyModifiers = QApplication::keyboardModifiers();

    switch (keyModifiers) {
    case Qt::NoModifier:{
        QScrollBar *bar = this->verticalScrollBar();
        bar->setValue(bar->value()-(numDegrees.y()/3));
        break;
    }
    case Qt::ShiftModifier:{
        QScrollBar *bar = this->horizontalScrollBar();
        bar->setValue(bar->value()+numDegrees.y());
        break;
    }
    case Qt::ControlModifier:{
        if(numDegrees.y() > 0){
            zoomIn(2);//放大字体
        }else{
            zoomOut(2);//缩小字体
        }
        break;
    }
    default:
        break;
    }

}


void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy){
        dynamic_cast<QWidget*>(editorSideArea)->scroll(0 , dy);
        dynamic_cast<QWidget*>(breakPointArea)->scroll(0 , dy);
    }else{
        dynamic_cast<QWidget*>(breakPointArea)->update(0, rect.y(), dynamic_cast<QWidget*>(breakPointArea)->width(), rect.height());
        dynamic_cast<QWidget*>(editorSideArea)->update(countBreakPointNeedWidth(), rect.y(), dynamic_cast<QWidget*>(editorSideArea)->width(), rect.height());
    }

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::updateLineNumberAreaWidth(int newBlockNum)
{
    int lineNumNeedWidth = lineNumberAreaWidth();
    int breakpointNeedWidth = countBreakPointNeedWidth();
    setViewportMargins(breakpointNeedWidth + lineNumNeedWidth, 0, 0, 0);
    this->editorSideArea->resize(this->editorSideArea->sizeHint());
    this->breakPointArea->resize(this->breakPointArea->sizeHint());
}

void CodeEditor::_on_cursorPositionChanged()
{
    // 清除之前的所有选择区域样式
    QList<QTextEdit::ExtraSelection> selections;
    selections = this->extraSelections();
    selections.clear();
    this->setExtraSelections(selections);

    // 设置高亮
    setLineHighlight(0);
}
