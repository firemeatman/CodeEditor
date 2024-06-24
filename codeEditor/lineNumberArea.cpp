#include "lineNumberArea.h"

#include <QPainter>
#include "../common/forceAccess.h"
#include "codeEditor.h"

LineNumberArea::LineNumberArea(QWidget *parent)
    : QWidget{parent}
{

}


QSize LineNumberArea::sizeHint() const
{
    int height = boundingTextEdit->viewport()->height();
    return QSize(boundingTextEdit->countLineNumberWigetWidth(), height);
    //return QSize(((ForceAccess::ForceQPlainText*)boundingTextEdit)->viewportMargins().left(), 0);
}

CodeEditor *LineNumberArea::getBoundingTextEdit() const
{
    return boundingTextEdit;
}

void LineNumberArea::setBoundingTextEdit(CodeEditor *newBoundingTextEdit)
{
    boundingTextEdit = newBoundingTextEdit;
}

void LineNumberArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect needUpdateRect = event->rect();

    ForceAccess::ForceQPlainText* boundingTextEditForce = (ForceAccess::ForceQPlainText*)boundingTextEdit;
    // 1.填充背景
    painter.fillRect(needUpdateRect, Qt::lightGray);

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
    int height = qRound(boundingTextEditForce->blockBoundingRect(block).height()); // 假设每一块的高度一样
    int bottom = top + height;

    // 如果块有效
    int wholeBottom = needUpdateRect.bottom();
    int wholeTop = needUpdateRect.top();
    QString number;
    while (block.isValid() && top <= wholeBottom)
    {
        number = QString::number(blockNumber + 1);
        // 如果块可见并且没超过底部就绘制
        if (block.isVisible() && bottom >= wholeTop)
        {
            painter.setPen(Qt::black);
            painter.drawText(0, top, this->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + height;
        ++blockNumber;
    }
}




