
#ifndef EDITORSIDEAREA_H
#define EDITORSIDEAREA_H

#include <QWidget>
#include <QTextBlock>
#include <QPlainTextEdit>

class CodeEditor;

class EditorSideArea : public QWidget
{

public:
    EditorSideArea(QWidget* parent = nullptr);


    void lineNumberAreaPaintEvent(QPaintEvent* event);

    QSize sizeHint() const override;

    CodeEditor *getBoundingTextEdit() const;
    void setBoundingTextEdit(CodeEditor *newBoundingTextEdit);

private:
    CodeEditor* boundingTextEdit;

protected:
    void paintEvent(QPaintEvent *event) override
    {
        lineNumberAreaPaintEvent(event);
    }
};


#endif // EDITORSIDEAREA_H
