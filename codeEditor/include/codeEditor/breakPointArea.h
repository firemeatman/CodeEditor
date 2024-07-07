#ifndef BREAKPOINTAREA_H
#define BREAKPOINTAREA_H

#include <QWidget>
#include <QList>
namespace codeEditor {

class CodeEditor;

enum class PointType : uint8_t{
    Break = 0x01,
    Arrow = 0x02,
    Warn = 0x04,
    Error = 0x08,
};
typedef uint8_t PointGroup;
class InfoPoint{
public:
    int line = 0;
    PointGroup pointGroup = 0;
    bool isEmputy(){
        return (pointGroup == 0);
    };
    void resetPoint(PointType p){
        pointGroup &= ~((uint8_t)p);
    }
    void setPoint(PointType p){
        pointGroup |= (uint8_t)p;
    }
    bool exist(PointType p){
        return (pointGroup & (uint8_t)p);
    }
};

class BreakPointArea : public QWidget
{
    Q_OBJECT
public:
    explicit BreakPointArea(QWidget *parent = nullptr);
    ~BreakPointArea();

    //void addPoint();
    // 行改变时，可以调用此函数更新点的信息
    void updatePointLine(int changeLineStart, int changeLineEnd, bool isAddLine);
    void addPoint(int line, PointType type);
    void removePoint(int line, PointType type);


    QSize sizeHint() const override;
    CodeEditor *getBoundingTextEdit() const;
    void setBoundingTextEdit(CodeEditor *newBoundingTextEdit);
public:
    QPixmap arrowImg;
    QPixmap warnImg;
    QPixmap errorImg;
private:
    CodeEditor* boundingTextEdit = nullptr;
    int lineHight;
    QList<InfoPoint> pointList;

    int findPoint(int line);

signals:
    void breakChanged(int line, bool haveBreak);
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};

}



#endif // BREAKPOINTAREA_H
