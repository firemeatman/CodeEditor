#ifndef BREAKPOINTAREA_H
#define BREAKPOINTAREA_H

#include <QWidget>
#include <QList>

class CodeEditor;

class BreakPointArea : public QWidget
{
    Q_OBJECT
public:
    explicit BreakPointArea(QWidget *parent = nullptr);
    ~BreakPointArea();

    QSize sizeHint() const override;


    CodeEditor *getBoundingTextEdit() const;
    void setBoundingTextEdit(CodeEditor *newBoundingTextEdit);

private:
    CodeEditor* boundingTextEdit = nullptr;
    QPixmap* breakPointImg = nullptr;

    int lineHight;
    QList<int> breakPointList;


protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // BREAKPOINTAREA_H
