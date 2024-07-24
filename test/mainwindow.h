#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QMap>
#include <QTabWidget>
#include <codeEditor/codeEditor.h>
#include <lsp/client.h>
namespace codeEditorTest {

// class CodeEditor;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    //codeEditor::CodeEditor* codeEditor{nullptr};
    QTabWidget* tabWidget{nullptr};
    QMap<QString,codeEditor::CodeEditor*> codeEditorMap;
    QListWidget* fileListWidget{nullptr};
public slots:
    void _on_ListCurrentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void _on_tabCloseRequested(int index);
    void _on_tabCurrentChanged(int index);
    void _on_hoverCursorTriggered(int line, int col, QPoint mouse_pos);
    void _on_suggestionTriggered(int line, int col);
    void _on_HoverRecved(QString& str);
    void _on_completionSuggestFinished(CompletionList& list);
};

}

#endif // MAINWINDOW_H
