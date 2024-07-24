#include "mainwindow.h"
#include <QFile>
#include <QDir>
#include <QUrl>
#include <QHBoxLayout>
#include <QDockWidget>
#include <iostream>
#include "globaldata.h"
#include "lspqthandler.h"
namespace codeEditorTest {
using namespace codeEditor;

QString workingPath = "D:/c_workstation/projects/QT-projects/CodeEditor/test/testcode";


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //ui
    tabWidget = new QTabWidget(this);
    fileListWidget = new QListWidget(this);
    QDockWidget* doc_w =  new QDockWidget(this);
    doc_w->setWidget(fileListWidget);
    this->addDockWidget(Qt::LeftDockWidgetArea, doc_w);
    this->setCentralWidget(tabWidget);
    this->resize(1000,600);
    // tabWidget设置
    tabWidget->setTabsClosable(true);
    // fileListWidget设置
    QDir rootDir(workingPath);
    QStringList childFileNameList = rootDir.entryList();  //返回所有文件名
    QString parentPath = rootDir.absolutePath();
    int childFileListSize = childFileNameList.count();
    for(int i=0; i<childFileListSize; i++)
    {
        QString fileNameStr = childFileNameList.at(i);
        if(fileNameStr == "." || fileNameStr == ".."){
            continue;
        }
        QFileInfo fileInfo(parentPath+"/"+fileNameStr);

        QListWidgetItem* item = new QListWidgetItem();
        item->setText(fileNameStr);
        item->setToolTip(fileInfo.absoluteFilePath());
        fileListWidget->addItem(item);
    }
    // lsp
    std::string uri = workingPath.toStdString();
    uri.insert(0,"file:///");
    DocumentUri docUri = uri;
    GlobalData::lspClient->Initialize(docUri);

    //==============================信号====================================
    connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::_on_tabCloseRequested);
    connect(fileListWidget, &QListWidget::currentItemChanged, this, &MainWindow::_on_ListCurrentItemChanged);
}

MainWindow::~MainWindow()
{
    tabWidget->clear();
    qDeleteAll(codeEditorMap);

}

void MainWindow::_on_ListCurrentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(!current) return;

    QString path = current->toolTip();
    QString name = current->text();
    CodeEditor* codeEditor = nullptr;

    // 打开和切换文件
    if(!codeEditorMap.contains(path)){
        codeEditor = new CodeEditor(this);
        int index = this->tabWidget->addTab(codeEditor, current->text());
        this->tabWidget->setTabText(index, name);
        this->tabWidget->setTabToolTip(index, path);
        codeEditorMap.insert(path, codeEditor);
        // 设置编辑器
        QString arrow = "D:/c_workstation/projects/QT-projects/CodeEditor/resource/right_arrow_icon_48px.png";
        QString warn = "D:/c_workstation/projects/QT-projects/CodeEditor/resource/warn_512px.png";
        QString error = "D:/c_workstation/projects/QT-projects/CodeEditor/resource/error_512px.png";
        codeEditor->setBreakComponentImg(arrow, PointType::Arrow);
        codeEditor->setBreakComponentImg(warn, PointType::Warn);
        codeEditor->setBreakComponentImg(error, PointType::Error);
        codeEditor->setReadOnly(false);
        QFile file(path);
        if(file.open(QIODeviceBase::ReadOnly | QIODeviceBase::Text)){
            QTextStream stream(&file);
            codeEditor->setPlainText(stream.readAll());
        }
        file.close();
        connect(codeEditor, &CodeEditor::hoverCursorTriggered, this, &MainWindow::_on_hoverCursorTriggered);
        connect(codeEditor, &CodeEditor::completionSuggestTriggered, this, &MainWindow::_on_suggestionTriggered);
        connect(LspQtHandler::instance(), &LspQtHandler::hoverFinished, this, &MainWindow::_on_HoverRecved);
        connect(LspQtHandler::instance(), &LspQtHandler::completionSuggestFinished, this, &MainWindow::_on_completionSuggestFinished);
        // lsp打开文件请求
        std::string uri = path.toStdString();
        uri.insert(0, "file:///");
        GlobalData::lspClient->DidOpen(uri, codeEditor->toPlainText().toStdString());

    }else{
        codeEditor = codeEditorMap[path];
    }
    this->tabWidget->setCurrentWidget(codeEditor);

}

void MainWindow::_on_tabCloseRequested(int index)
{
   // 关闭文件
    QString path = this->tabWidget->tabToolTip(index);
    std::string uri = path.toStdString();
    uri.insert(0, "file:///");
    GlobalData::lspClient->DidClose(uri);

    if(codeEditorMap.contains(path)){
        codeEditorMap.remove(path);
    }
    QWidget* editor = tabWidget->widget(index);
    this->tabWidget->removeTab(index);
    if(editor){
        editor->deleteLater();
    }

}

void MainWindow::_on_tabCurrentChanged(int index)
{
    // qDebug()<<index;
    // qDebug()<<tabWidget->currentIndex();
}

void MainWindow::_on_hoverCursorTriggered(int line, int col, QPoint mouse_pos)
{
    // 悬浮提示
    Position pos = {line, col};
    int index = tabWidget->currentIndex();
    std::string uri = tabWidget->tabToolTip(index).toStdString();
    uri.insert(0,"file:///");
    GlobalData::lspClient->Hover(uri, pos);
}

void MainWindow::_on_suggestionTriggered(int line, int col)
{
    Position pos = {line, col};
    int index = tabWidget->currentIndex();
    std::string uri = tabWidget->tabToolTip(index).toStdString();
    uri.insert(0,"file:///");
    CompletionContext context;
    GlobalData::lspClient->Completion(uri, pos, context);
    GlobalData::lspClient->SignatureHelp(uri, pos);
}

void MainWindow::_on_HoverRecved(QString &str)
{
    CodeEditor* editor =static_cast<CodeEditor*>(this->tabWidget->currentWidget());
    if(!editor) return;
    if(str.isEmpty())return;

    editor->addHoverInfoWindow(str);
}

void MainWindow::_on_completionSuggestFinished(CompletionList &list)
{
    CodeEditor* editor =static_cast<CodeEditor*>(this->tabWidget->currentWidget());
    if(!editor) return;

    editor->updateCodeCompletionSuggestions(list);
}

}
