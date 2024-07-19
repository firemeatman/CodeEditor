#include <codeEditor/codeEditor.h>

#include <QApplication>
#include <QDebug>
#include <QScrollBar>
#include <QPoint>

namespace codeEditor {

CodeEditor::CodeEditor(QWidget* parent):
    QPlainTextEdit{parent}
{
    // 设置换行、tab显示宽度
    this->setWordWrapMode(QTextOption::NoWrap);
    this->setTabStopDistance(40);
    // 字体
    QFont gloableFont("Consolas", 12);
    this->setFont(gloableFont);
    this->setCursorWidth(2);
    // 样式
    QBrush brush(Qt::red);
    errorLineFormat.setBackground(brush);
    //errorLineFormat.setProperty(QTextFormat::FullWidthSelection, true);

    errorUnderLineFormat.setUnderlineStyle(QTextCharFormat::WaveUnderline);
    errorUnderLineFormat.setUnderlineColor(Qt::red);
    errorUnderLineFormat.setFontUnderline(true);
    // 小组件a
    lineNumberArea = new LineNumberArea(this);
    lineNumberArea->setBoundingTextEdit(this);
    breakPointArea = new BreakPointArea(this);
    breakPointArea->setBoundingTextEdit(this);

    // QRect frame = breakPointArea->frameGeometry();
    // breakPointArea->resize(20, this->viewport()->height());
    // lineNumberArea->move(frame.x()+20, lineNumberArea->y());
    // lineNumberArea->resize(lineNumberArea->width(), this->viewport()->height());
    // 设置语法高亮
    QTextDocument* doc = this->document();
    this->cHighLight = new CHighlight(doc);


    //===========================信号===================================
    connect(doc, &QTextDocument::contentsChange, this, &CodeEditor::_on_contentsChange);
    connect(this, &QPlainTextEdit::blockCountChanged, this, &CodeEditor::_on_blockCountChanged);
    connect(this, &QPlainTextEdit::updateRequest, this, &CodeEditor::_on_updateRequest);
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, &CodeEditor::_on_cursorPositionChanged);

    updateSideArea();
}

CodeEditor::~CodeEditor()
{
    if(cHighLight != nullptr){
        cHighLight->deleteLater();
    }

}

void CodeEditor::setLineComponentVisible(bool visible)
{
    lineNumberArea->setVisible(visible);
}

void CodeEditor::setBreakComponentVisible(bool visible)
{
    breakPointArea->setVisible(visible);
}

void CodeEditor::addBreak(int line)
{
    if(line >= this->blockCount()) return;
    this->breakPointArea->addPoint(line, PointType::Break);
}

void CodeEditor::removeBreak(int line)
{
    if(line >= this->blockCount()) return;
    this->breakPointArea->removePoint(line, PointType::Break);
}

void CodeEditor::addNextFlag(int line)
{
    if(line >= this->blockCount()) return;
    this->breakPointArea->addPoint(line, PointType::Arrow);
}

void CodeEditor::removeNextFlag(int line)
{
    if(line >= this->blockCount()) return;
    this->breakPointArea->removePoint(line, PointType::Arrow);
}

void CodeEditor::addWarn(int line, int col, QString &warnStr)
{
    if(line >= this->blockCount()) return;
    this->breakPointArea->addPoint(line, PointType::Warn);
}

void CodeEditor::removeWarn(int line, int col)
{
    if(line >= this->blockCount()) return;
    this->breakPointArea->removePoint(line, PointType::Warn);
}

void CodeEditor::addError(int line, int col, QString &errStr)
{
    if(line >= this->blockCount()) return;
    this->breakPointArea->addPoint(line, PointType::Error);

    QTextCursor cursor = this->generateCursor(line, col);
    addWordUnderline(cursor, this->errorUnderLineFormat);
}

void CodeEditor::removeError(int line, int col)
{
    if(line >= this->blockCount()) return;
    this->breakPointArea->removePoint(line, PointType::Error);
}

void CodeEditor::setBreakComponentImg(QString &resPath, PointType type)
{
    QPixmap map(resPath);
    //map = map.scaled(20,20);
    switch (type) {
    case PointType::Arrow:
        this->breakPointArea->arrowImg = map;
        break;
    case PointType::Warn:
        this->breakPointArea->warnImg = map;
        break;
    case PointType::Error:
        this->breakPointArea->errorImg = map;
        break;
    default:
        break;
    }
}

void CodeEditor::setTokenFormat(int tokenType, QTextCharFormat format)
{
    this->tokenFormatMap.insert(tokenType, format);
}

void CodeEditor::setWordJumpLink(bool enable, Qt::KeyboardModifiers triggerBtn)
{

}

void CodeEditor::addHoverInfoWindow(int line, int col, QString &content)
{

}

void CodeEditor::updateHighLight(const QList<Token> &tokenList)
{

}

void CodeEditor::updateHighLight(const QList<int> &lspTokenList)
{

}

void CodeEditor::addCodeCompletionSuggestions(Loaction pos, const QList<CodeSuggest> &suggestions)
{

}

void CodeEditor::addDiagnosis(const QList<DiagnosisInfo> &infoList)
{

}

void CodeEditor::clearDiagnosis()
{

}

QTextCursor CodeEditor::cursorPosByGlobalMousePos(QPoint g_mousePos)
{
    QWidget* viewPort = this->viewport();
    QPoint posInViewPort = viewPort->mapFromGlobal(g_mousePos);
    qDebug()<<"函数全局: "<<g_mousePos;
    qDebug()<<"在视口处的: "<<posInViewPort;

    return cursorForPosition(posInViewPort);
}

QTextCursor CodeEditor::cursorPosByMousePos(QPoint mousePos, QWidget *from)
{
    QWidget* viewPort = this->viewport();
    QPoint posInViewPort = viewPort->mapFrom(from, mousePos);

    return cursorForPosition(posInViewPort);
}

int CodeEditor::countLineNumberWigetWidth() const
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

int CodeEditor::countBreakPointWigetWidth() const
{
    //int breakpointNeedWidth = fontMetrics().height() + 5;
    int breakpointNeedWidth = 20;
    return breakpointNeedWidth;
}

int CodeEditor::getLeftComWidth() const
{
    return countLineNumberWigetWidth() + countBreakPointWigetWidth();
}

void CodeEditor::wheelEvent(QWheelEvent *e)
{
    // 放大缩小字体
    QPoint numDegrees = e->angleDelta() / 8;
    Qt::KeyboardModifiers keyModifiers = QApplication::keyboardModifiers();

    switch (keyModifiers) {
    case Qt::NoModifier:{
        QScrollBar *bar = this->verticalScrollBar();
        bar->setValue(bar->value()-(numDegrees.y()/3));
        break;
    }
    case Qt::ShiftModifier:{
        QScrollBar *bar = this->horizontalScrollBar();
        bar->setValue(bar->value()+numDegrees.y());
        break;
    }
    case Qt::ControlModifier:{
        if(numDegrees.y() > 0){
            zoomIn(2);//放大字体
        }else{
            zoomOut(2);//缩小字体
        }
        break;
    }
    default:
        break;
    }

}

void CodeEditor::resizeEvent(QResizeEvent *event)
{
    // 更新小组件高度
    int height =  event->size().height();
    this->lineNumberArea->resize(lineNumberArea->width(), height);
    this->breakPointArea->resize(breakPointArea->width(), height);

    this->lineNumberArea->update();
    this->breakPointArea->update();
}


void CodeEditor::updateSideArea()
{
    int lineNumNeedWidth = countLineNumberWigetWidth();
    int breakpointNeedWidth = countBreakPointWigetWidth();
    int height = this->height();
    setViewportMargins(breakpointNeedWidth + lineNumNeedWidth, 0, 0, 0);

    QRect frame = breakPointArea->frameGeometry();
    lineNumberArea->move(frame.x()+breakpointNeedWidth, lineNumberArea->y());
    this->lineNumberArea->resize(lineNumNeedWidth, height);
    this->breakPointArea->resize(breakpointNeedWidth, height);

    this->lineNumberArea->update();
    this->breakPointArea->update();

}

QTextCursor CodeEditor::generateCursor(int line, int col)
{
    QTextDocument* doc = this->document();
    QTextBlock block = doc->findBlockByNumber(line);
    QTextCursor cursor(block);

    int endPos = block.position() + col;
    cursor.setPosition(endPos,QTextCursor::MoveMode::MoveAnchor);
    //cursor.movePosition(QTextCursor::MoveOperation::Right, QTextCursor::MoveMode::MoveAnchor,col);

    return cursor;
}

void CodeEditor::highLightSelectedLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;//提供一种方式显示选择的文本
    QTextEdit::ExtraSelection selection;

    QBrush bursh(QColor::fromString("#FFE573"));
    selection.format.setBackground(bursh);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = this->textCursor();
    extraSelections.append(selection);

    this->setExtraSelections(extraSelections);
}

void CodeEditor::highlightLine(int line, QTextCharFormat& format)
{
    QList<QTextEdit::ExtraSelection> extraSelections = this->extraSelections();//提供一种方式显示选择的文本
    QTextEdit::ExtraSelection selection;

    QTextDocument* doc = this->document();
    QTextBlock block = doc->findBlockByNumber(line);
    QTextCursor cursor(block);
    //cursor.movePosition(QTextCursor::EndOfLine);
    //cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::MoveMode::KeepAnchor);

    selection.format = format;
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = cursor;
    extraSelections.append(selection);

    this->setExtraSelections(extraSelections);
}

void CodeEditor::addWordUnderline(int line, int col, QTextCharFormat& format)
{
    QTextCursor cursor = this->generateCursor(line, col);
    cursor.select(QTextCursor::WordUnderCursor);

    QList<QTextEdit::ExtraSelection> extraSelections = this->extraSelections();
    QTextEdit::ExtraSelection selection;
    selection.format = format;
    selection.cursor = cursor;
    extraSelections.append(selection);
    this->setExtraSelections(extraSelections);
}

void CodeEditor::addWordUnderline(QTextCursor &cursor, QTextCharFormat &format)
{
    cursor.select(QTextCursor::WordUnderCursor);

    QList<QTextEdit::ExtraSelection> extraSelections = this->extraSelections();
    QTextEdit::ExtraSelection selection;
    selection.format = format;
    selection.cursor = cursor;
    extraSelections.append(selection);
    this->setExtraSelections(extraSelections);
}

void CodeEditor::_on_contentsChange(int position, int charsRemoved, int charsAdded)
{
    static int lastLineNum = 0;

    QTextDocument* doc = this->document();
    QTextBlock block = doc->findBlock(position);
    int blockNum = block.blockNumber();
    int lineNum = doc->blockCount();
    int changeLineNum = std::abs(lineNum - lastLineNum);
    int rangeLeft = blockNum;
    int rangeRight;
    bool isAdd = false;

    // 行数没变化时跳出
    if(changeLineNum == 0){
        return;
    }
    if(charsRemoved > 0 && charsAdded > 0){ // 采用字符位移计算改变的行范围
        int endPos = position + charsAdded;
        rangeRight = doc->findBlock(endPos).blockNumber();
        if(charsAdded >= charsRemoved) isAdd = true;

    }else{ // 采用行数变化计算改变的行范围
        rangeRight = rangeLeft + changeLineNum;
        if(lineNum >= lastLineNum) isAdd = true;
    }
    // qDebug()<<position<<"; "<<charsRemoved<<"; "<<charsAdded;
    // qDebug("rangeLeft: %d;rangeRight: %d",rangeLeft,rangeRight);
    lastLineNum = lineNum;
    emit lineChanged(rangeLeft, rangeRight, isAdd);
    this->breakPointArea->updatePointLine(rangeLeft, rangeRight, isAdd);
}


void CodeEditor::_on_cursorPositionChanged()
{

    // QTextCursor cursor = this->textCursor();;

    // // 清除之前的所有选择区域样式
    // QList<QTextEdit::ExtraSelection> selections;
    // this->setExtraSelections(selections);

    // // 设置高亮
    // highLightSelectedLine();
    // int lineNumber = cursor.blockNumber();
    // int columnNumber = cursor.columnNumber();
}

void CodeEditor::_on_blockCountChanged(int newBlockCount)
{
    updateSideArea();

}


void CodeEditor::_on_updateRequest(const QRect &rect, int dy)
{
    if (dy){
        lineNumberArea->scroll(0 , dy);
        breakPointArea->scroll(0 , dy);
    }else{
        breakPointArea->update(0, rect.y(),breakPointArea->width(), rect.height());
        lineNumberArea->update(countBreakPointWigetWidth(), rect.y(), lineNumberArea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect())){
        updateSideArea();
    }
}

}
