#include <codeEditor/breakPointArea.h>

#include <QPainter>
#include <codeEditor/common/forceAccess.h>
#include <codeEditor/codeEditor.h>
namespace codeEditor {

BreakPointArea::BreakPointArea(QWidget *parent)
    : QWidget{parent}
{

}

BreakPointArea::~BreakPointArea()
{
}

void BreakPointArea::updatePointLine(int changeLineStart, int changeLineEnd, bool isAddLine)
{
    if(changeLineStart > changeLineEnd) return;
    int changeLineNum = changeLineEnd - changeLineStart;
    int size = pointList.size();
    InfoPoint point;
    for(int i=size-1; i>=0; i--){
        point = pointList.at(i);
        if(point.line < changeLineStart) continue;
        if(isAddLine){
            pointList[i].line = point.line + changeLineNum;
        }else{
            if(point.line > changeLineEnd){
                pointList[i].line = point.line - changeLineNum;
            }else{
                pointList.removeAt(i);
            }
        }
    }
}

void BreakPointArea::addPoint(int line, PointType type)
{
    int index = findPoint(line);
    if(index >= 0){
        InfoPoint point = pointList.at(index);
        point.setPoint(type);
        pointList[index] = point;
    }else{
        InfoPoint point;
        point.line = line;
        point.setPoint(type);
        pointList.append(point);
    }
}

void BreakPointArea::removePoint(int line, PointType type)
{
    int index = findPoint(line);
    if(index >= 0){
        InfoPoint point = pointList.at(index);
        if(point.exist(type)){
            point.resetPoint(type);
        }
        if(point.isEmputy()){
            pointList.removeAt(index);
        }else pointList[index] = point;
    }
}

QSize BreakPointArea::sizeHint() const
{
    //int height = boundingTextEdit->viewport()->height();
    int height = boundingTextEdit->height();
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
    painter.setPen(Qt::transparent);
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿
    int wholeBottom = needUpdateRect.bottom();
    int wholeTop = needUpdateRect.top();

    int originalHeight = fontMetrics().height();
    int newHight = originalHeight * 0.7;
    int radius = newHight * 0.5;
    int x = originalHeight/2 - radius;
    int y = top + originalHeight/2 - radius;
    QPixmap pixmap;
    // 如果块有效
    while (block.isValid() && top <= wholeBottom)
    {
        if (block.isVisible() && bottom >= wholeTop)
        {
            int index = findPoint(blockNumber);
            if( index >= 0){
                InfoPoint point = pointList.at(index);
                y = top + originalHeight/2 - radius;
                if(point.exist(PointType::Break) || point.exist(PointType::Arrow)){

                    if(point.exist(PointType::Break)){
                        painter.drawEllipse(x, y, newHight, newHight);
                    }
                    if(point.exist(PointType::Arrow)){
                        pixmap = this->arrowImg.scaled(originalHeight,originalHeight);
                        painter.drawPixmap(0,top,pixmap);
                    }
                }else if(point.exist(PointType::Warn)){
                    pixmap = this->warnImg.scaled(originalHeight,originalHeight);
                    painter.drawPixmap(0,top,pixmap);
                }else if(point.exist(PointType::Error)){
                    pixmap = this->errorImg.scaled(originalHeight,originalHeight);
                    painter.drawPixmap(0,top,pixmap);
                }
            }
        }
        block = block.next();
        top = bottom;
        bottom = top + qRound(boundingTextEditForce->blockBoundingRect(block).height());;
        ++blockNumber;
    }

}

void BreakPointArea::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        QPointF g_pos = event->globalPosition();
        int leftComWidth = boundingTextEdit->getLeftComWidth();
        g_pos.setX(g_pos.x() + leftComWidth);
        QTextCursor cursor = boundingTextEdit->cursorPosFromGlobalMousePos(g_pos.toPoint());
        int lineNum = cursor.blockNumber();
        int index = findPoint(lineNum);

        if(index >=0){
            InfoPoint point = pointList.at(index);
            if(point.exist(PointType::Break)){
                point.resetPoint(PointType::Break);
            }else point.setPoint(PointType::Break);
            if(point.isEmputy()){
                pointList.removeAt(index);
            }else pointList[index] = point;
            emit breakChanged(lineNum, false);
        }else{
            InfoPoint point;
            point.line = lineNum;
            point.setPoint(PointType::Break);
            pointList.append(point);
            emit breakChanged(lineNum, true);
        }
        this->update();
    }
}

}
