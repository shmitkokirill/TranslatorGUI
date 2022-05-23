#include "codeeditor.h"
#include "qtextcursor.h"
#include "qtextedit.h"
#include <QFont>
#include <QPainter>
#include <QTextBlock>
#include <linenumberarea.h>
using namespace SourceCodeEdit;

SourceCodeEdit::CodeEditor::CodeEditor(QWidget *parent)
    : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(
        this,
        &CodeEditor::blockCountChanged,
        this,
        &CodeEditor::updateLineNumberAreaWidth
    );
    connect(
        this,
        &CodeEditor::updateRequest,
        this,
        &CodeEditor::updateLineNumberArea
    );
    connect(
        this,
        &CodeEditor::cursorPositionChanged,
        this,
        &CodeEditor::highlightCurrentLine
    );

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
    setFont(QFont("Consolas", 18));
}

CodeEditor::~CodeEditor()
{
    delete lineNumberArea;
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter(lineNumberArea);
    const QRgb numberPanelColor = NUMBER_PANEL;
    painter->fillRect(event->rect(), QColor::fromRgb(numberPanelColor));
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top =
        qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter->setPen(Qt::black);
            painter->drawText(
                0,
                top,
                lineNumberArea->width(),
                fontMetrics().height(),
                Qt::AlignRight,
                number
            );
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void CodeEditor::highlightTheSymbol(int pos)
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection symbol;
        const QRgb sColor = ERROR_SYMBOL;
        QColor symColor = QColor(sColor);

        symbol.format.setBackground(symColor);
        QTextCursor cursorTxt = textCursor();
        cursorTxt.setPosition(pos);
        symbol.cursor = cursorTxt;
        symbol.cursor.select(QTextCursor::WordUnderCursor);
        extraSelections.append(symbol);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        const QRgb clColor = CURRENT_LINE;
        QColor lineColor = QColor(clColor);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(
        QRect(
            cr.left(),
            cr.top(),
            lineNumberAreaWidth(),
            cr.height()
        )
    );
}

void CodeEditor::updateLineNumberAreaWidth(int newBlockCount)
{
    Q_UNUSED(newBlockCount);
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(
            0,
            rect.y(),
            lineNumberArea->width(),
            rect.height()
        );

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}
