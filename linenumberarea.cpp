#include "linenumberarea.h"
#include "codeeditor.h"
using namespace SourceCodeEdit;

LineNumberArea::LineNumberArea(CodeEditor *editor) :
    QWidget(editor),
    codeEditor(editor)
{
}

LineNumberArea::~LineNumberArea() = default;

QSize LineNumberArea::sizeHint() const
{
    return QSize(codeEditor->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event)
{
    codeEditor->lineNumberAreaPaintEvent(event);
}
