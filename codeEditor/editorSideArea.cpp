
#include "editorSideArea.h"
#include <QPainter>
#include "../common/forceAccess.h"
#include "codeEditor.h"

EditorSideArea::EditorSideArea(QWidget *parent)
    : QWidget{parent}
{

}


void EditorSideArea::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    ForceAccess::ForceQPlainText* boundingTextEditForce = (ForceAccess::ForceQPlainText*)boundingTextEdit;
    // 1.填充背景
    painter.fillRect(event->rect(), Qt::lightGray);

    // 2.计算行号区域的头和底的坐标(头设置为原点)

    QTextBlock block = boundingTextEditForce->firstVisibleBlock();  // 当前页面, 第一块可见的文本区域
    int blockNumber = block.blockNumber();   // 第一块可见的文本区域的块编号，(编号从0开始，所以最开始是0)

    // 3.保证绘画的起始行号的头和文本块的头平齐
    // qRound: 四舍五入
    // blockBoundingGeometry(block):  返回块的矩形，这里的块的顶部坐标为0；
    // QRectF QRectF::translated(const QPointF &offset) const:
    //                  返回对进行偏移offset的矩形,这里的块的顶部坐标为内容偏移的坐标；
    int top = qRound((boundingTextEditForce->blockBoundingGeometry(block)).translated(boundingTextEditForce->contentOffset()).top());
    //int top = qRound(blockBoundingGeometry(block).top());
    int bottom = top + qRound(boundingTextEditForce->blockBoundingRect(block).height());
    QSize hitSize = sizeHint();
    int tempw = this->width();
    bool tempf = block.isValid();
    int templl = event->rect().bottom();

    // 如果块有效，(用回车键分配了)
    while (block.isValid() && top <= event->rect().bottom())
    {

        // 块可见，底部坐标大于当前事件的顶部坐标
        if (block.isVisible() && bottom >= event->rect().top())
        {
            // 因为块呈序列的，从0开始，所以可以将块号+1，作为行号
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, this->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(boundingTextEditForce->blockBoundingRect(block).height());
        ++blockNumber;
    }

}

QSize EditorSideArea::sizeHint() const
{
    int height = boundingTextEdit->viewport()->height();
    return QSize(boundingTextEdit->lineNumberAreaWidth(), height);
    //return QSize(((ForceAccess::ForceQPlainText*)boundingTextEdit)->viewportMargins().left(), 0);
}

CodeEditor *EditorSideArea::getBoundingTextEdit() const
{
    return boundingTextEdit;
}

void EditorSideArea::setBoundingTextEdit(CodeEditor *newBoundingTextEdit)
{
    boundingTextEdit = newBoundingTextEdit;
}




