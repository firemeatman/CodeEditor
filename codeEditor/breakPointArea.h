#ifndef BREAKPOINTAREA_H
#define BREAKPOINTAREA_H

#include <QWidget>
#include <QList>

class CodeEditor;



class BreakPointArea : public QWidget
{
    Q_OBJECT
public:
    enum class PontType{
        Break,
        Error,
        Warn,
    };
    class InfoPoint{
    public:
        int line = 0;
        bool breakPoint = false;
        bool warnPoint = false;
        bool errorPoint = false;
        bool isEmputy(){
            return (!breakPoint && !warnPoint && !errorPoint);
        };
    };

    explicit BreakPointArea(QWidget *parent = nullptr);
    ~BreakPointArea();

    QSize sizeHint() const override;
    CodeEditor *getBoundingTextEdit() const;
    void setBoundingTextEdit(CodeEditor *newBoundingTextEdit);

private:
    CodeEditor* boundingTextEdit = nullptr;
    QPixmap* breakPointImg = nullptr;

    int lineHight;
    QList<InfoPoint> pointList;

    int findPoint(int line);

signals:
    void breakChanged(int line, bool haveBreak);
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // BREAKPOINTAREA_H
