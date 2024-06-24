#include "breakPointArea.h"

#include <QPainter>
#include "../common/forceAccess.h"
#include "codeEditor.h"

BreakPointArea::BreakPointArea(QWidget *parent)
    : QWidget{parent}
{
}

BreakPointArea::~BreakPointArea()
{
    if(breakPointImg != nullptr){
        delete breakPointImg;
    }
}

QSize BreakPointArea::sizeHint() const
{
    int height = boundingTextEdit->viewport()->height();
    return QSize(boundingTextEdit->countBreakPointWigetWidth(), height);
}

CodeEditor *BreakPointArea::getBoundingTextEdit() const
{
    return boundingTextEdit;
}

void BreakPointArea::setBoundingTextEdit(CodeEditor *newBoundingTextEdit)
{
    boundingTextEdit = newBoundingTextEdit;
}

int BreakPointArea::findPoint(int line)
{
    int size = pointList.size();
    int index = -1;
    for(int i=0; i<size;i++){
        if(pointList.at(i).line == line){
            index = i;
        }
    }
    return index;
}

void BreakPointArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect needUpdateRect = event->rect();
    ForceAccess::ForceQPlainText* boundingTextEditForce = (ForceAccess::ForceQPlainText*)boundingTextEdit;
    // 1.填充背景
    painter.fillRect(event->rect(), Qt::gray);

    // 2.计算行号区域的头和底的坐标(头设置为原点)

    QTextBlock block = boundingTextEditForce->firstVisibleBlock();  // 当前页面, 第一块可见的文本区域
    int blockNumber = block.blockNumber();   // 第一块可见的文本区域的块编号。编辑器视口可见的第一个块
    this->lineHight = qRound(boundingTextEditForce->blockBoundingRect(block).height());

    // 3.保证绘画的起始行号的头和文本块的头平齐
    // qRound: 四舍五入
    // blockBoundingGeometry(block):  返回块的矩形，这里的块的顶部坐标为0；
    // QRectF QRectF::translated(const QPointF &offset) const:
    //                  返回对进行偏移offset的矩形,这里的块的顶部坐标为内容偏移的坐标；
    int top = qRound((boundingTextEditForce->blockBoundingGeometry(block)).translated(boundingTextEditForce->contentOffset()).top());
    //int top = qRound(blockBoundingGeometry(block).top());
    int height = qRound(boundingTextEditForce->blockBoundingRect(block).height()); // 假设每一块的高度一样
    int bottom = top + height;

    painter.setBrush(Qt::red);//设置画刷
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿
    int wholeBottom = needUpdateRect.bottom();
    int wholeTop = needUpdateRect.top();
    // 如果块有效
    while (block.isValid() && top <= wholeBottom)
    {
        if (block.isVisible() && bottom >= wholeTop)
        {
            int index = findPoint(blockNumber);
            if( index >= 0){
                // 设置圆形的左上角坐标和高度宽度
                int originalHeight = fontMetrics().height();
                int newHight = originalHeight * 0.6;
                int radius = newHight * 0.5;
                int x = originalHeight/2 - radius;
                int y = top + originalHeight/2 - radius;

                painter.drawEllipse(x, y, newHight, newHight);
            }
        }
        block = block.next();
        top = bottom;
        bottom = top + height;
        ++blockNumber;
    }

}

void BreakPointArea::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        ForceAccess::ForceQPlainText* boundingTextEditForce = (ForceAccess::ForceQPlainText*)boundingTextEdit;

        QTextBlock block = boundingTextEditForce->firstVisibleBlock();
        int firstNumber = block.blockNumber();
        QPoint pos = event->pos();
        int lineNum = firstNumber + pos.y()/(this->lineHight);
        int index = findPoint(lineNum);
        if(index >=0){
            InfoPoint point = pointList.at(index);
            point.breakPoint = false;
            if(point.isEmputy()){
                pointList.remove(index);
            }else pointList[index] = point;
            emit breakChanged(lineNum, false);
        }else{
            InfoPoint point;
            point.line = lineNum;
            point.breakPoint = true;
            pointList.append(point);
            emit breakChanged(lineNum, true);
        }
        this->update();
    }
}
