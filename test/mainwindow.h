#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <codeEditor/codeEditor.h>
namespace codeEditorTest {

// class CodeEditor;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    //Ui::MainWindow *ui;
    codeEditor::CodeEditor* codeEditor = nullptr;
};

}

#endif // MAINWINDOW_H
