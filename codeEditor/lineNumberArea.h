
#ifndef EDITORSIDEAREA_H
#define EDITORSIDEAREA_H

#include <QWidget>
#include <QTextBlock>
#include <QPlainTextEdit>

class CodeEditor;

class LineNumberArea : public QWidget
{

public:
    LineNumberArea(QWidget* parent = nullptr);

    QSize sizeHint() const override;

    CodeEditor *getBoundingTextEdit() const;
    void setBoundingTextEdit(CodeEditor *newBoundingTextEdit);

private:
    CodeEditor* boundingTextEdit;

protected:
    void paintEvent(QPaintEvent *event) override;
};


#endif // EDITORSIDEAREA_H
