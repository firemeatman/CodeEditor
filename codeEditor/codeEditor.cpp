#include "codeEditor.h"

#include <QApplication>
#include <QDebug>
#include <QScrollBar>
#include <QPoint>

QString gloable_testText = "\n\
//测试代码\n\
/*true test\n\
 * code\n\
 */\n\
int mian()\n\
{\n\
\tint a = 10;\n\
\tif(a == 10 ){\n\
\t\t\n\
\t}\n\
\treturn 0;\n\
}\n";

CodeEditor::CodeEditor(QWidget* parent):
    QPlainTextEdit{parent}
{
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

    // 设置语法高亮

    this->cHighLight = new CHighlight(this->document());

    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, &CodeEditor::_on_cursorPositionChanged);

    updateLineNumberAreaWidth(0);
}

CodeEditor::~CodeEditor()
{
    if(cHighLight != nullptr){
        cHighLight->deleteLater();
    }

}

void CodeEditor::setBreakComponent(bool visible)
{
    breakPointArea->setVisible(visible);
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
    extraSelections.append(selection);
    this->setExtraSelections(extraSelections);
}

QPoint CodeEditor::countTextPosByMousePos(QPoint &mousePos)
{
    QPoint textPoint;
    int lineNum;
    int columNum;
    QTextBlock firstBlock =this->firstVisibleBlock();  // 当前页面, 第一块可见的文本区域
    int blockHeight = qRound(this->blockBoundingRect(firstBlock).height());
    int top = qRound((this->blockBoundingGeometry(firstBlock)).translated(this->contentOffset()).top());
    QTextDocument* document = this->document();


    if(blockHeight == 0){
        lineNum = 0;
    }else{
        lineNum =  (mousePos.y() - top) / blockHeight;
    }

    QTextBlock block = document->findBlockByLineNumber(lineNum);
    QTextCharFormat charFormat = block.charFormat();
    QFont font =  charFormat.font();
    int weidth = font.pixelSize();
    columNum = (mousePos.x() - 4) / weidth;

    textPoint.setX(columNum);
    textPoint.setY(lineNum);
    return textPoint;

}
int CodeEditor::countLineNumberWigetWidth() const
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

int CodeEditor::countBreakPointWigetWidth()
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
        dynamic_cast<QWidget*>(editorSideArea)->update(countBreakPointWigetWidth(), rect.y(), dynamic_cast<QWidget*>(editorSideArea)->width(), rect.height());
    }

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::updateLineNumberAreaWidth(int newBlockNum)
{
    int lineNumNeedWidth = countLineNumberWigetWidth();
    int breakpointNeedWidth = countBreakPointWigetWidth();
    setViewportMargins(breakpointNeedWidth + lineNumNeedWidth, 0, 0, 0);
    this->editorSideArea->resize(this->editorSideArea->sizeHint());
    this->breakPointArea->resize(this->breakPointArea->sizeHint());
}

void CodeEditor::_on_cursorPositionChanged()
{

    QTextCursor cursor = this->textCursor();;

    // 清除之前的所有选择区域样式
    QList<QTextEdit::ExtraSelection> selections;
    selections = this->extraSelections();
    selections.clear();
    this->setExtraSelections(selections);

    // 设置高亮
    setLineHighlight(0);
    int lineNumber = cursor.blockNumber();
    int columnNumber = cursor.columnNumber();
}
