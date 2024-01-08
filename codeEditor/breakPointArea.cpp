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
    breakPointList.clear();
}

QSize BreakPointArea::sizeHint() const
{
    int height = boundingTextEdit->viewport()->height();
    return QSize(boundingTextEdit->countBreakPointNeedWidth(), height);
}

CodeEditor *BreakPointArea::getBoundingTextEdit() const
{
    return boundingTextEdit;
}

void BreakPointArea::setBoundingTextEdit(CodeEditor *newBoundingTextEdit)
{
    boundingTextEdit = newBoundingTextEdit;
}

void BreakPointArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    ForceAccess::ForceQPlainText* boundingTextEditForce = (ForceAccess::ForceQPlainText*)boundingTextEdit;
    // 1.填充背景
    painter.fillRect(event->rect(), Qt::gray);

    // 2.计算行号区域的头和底的坐标(头设置为原点)

    QTextBlock block = boundingTextEditForce->firstVisibleBlock();  // 当前页面, 第一块可见的文本区域
    int blockNumber = block.blockNumber();   // 第一块可见的文本区域的块编号，(编号从0开始，所以最开始是0)
    this->lineHight = qRound(boundingTextEditForce->blockBoundingRect(block).height());

    // 3.保证绘画的起始行号的头和文本块的头平齐
    // qRound: 四舍五入
    // blockBoundingGeometry(block):  返回块的矩形，这里的块的顶部坐标为0；
    // QRectF QRectF::translated(const QPointF &offset) const:
    //                  返回对进行偏移offset的矩形,这里的块的顶部坐标为内容偏移的坐标；
    int top = qRound((boundingTextEditForce->blockBoundingGeometry(block)).translated(boundingTextEditForce->contentOffset()).top());
    //int top = qRound(blockBoundingGeometry(block).top());
    int bottom = top + qRound(boundingTextEditForce->blockBoundingRect(block).height());

    painter.setBrush(Qt::red);//设置画刷
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿
    // 如果块有效
    // while (block.isValid() && top <= event->rect().bottom())
    // {

    //     // 块可见，底部坐标大于当前事件的顶部坐标
    //     if (block.isVisible() && bottom >= event->rect().top())
    //     {
    //         // 因为块呈序列的，从0开始，所以可以将块号+1，作为行号
    //         QString number = QString::number(blockNumber + 1);

    //         // 设置矩形的左上角坐标和高度宽度
    //         int height = fontMetrics().height();
    //         painter.drawEllipse(0, top, height*0.8,height*0.8);
    //     }

    //     block = block.next();
    //     top = bottom;
    //     bottom = top + qRound(boundingTextEditForce->blockBoundingRect(block).height());
    //     ++blockNumber;
    // }



    for(int linenum : breakPointList){

        // QTextDocument* document = boundingTextEditForce->document();
        // block = document->findBlockByLineNumber(linenum);
        int height = fontMetrics().height();
        int y = top + lineHight*linenum;
        painter.drawEllipse(0, y, height*0.8,height*0.8);
    }
}

void BreakPointArea::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        QPoint pos = event->pos();
        int lineNum = pos.y()/(this->lineHight);
        if(breakPointList.contains(lineNum)){
            for(int i=0; i<breakPointList.size();i++){
                if(breakPointList[i] == lineNum){
                    breakPointList.removeAt(i);
                    break;
                }
            }
        }else{
            breakPointList.append(lineNum);
        }
        this->update();
    }
}
