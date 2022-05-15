#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>
#include "codeeditor.h"

namespace SourceCodeEdit
{
    class LineNumberArea : public QWidget
    {
    public:
        LineNumberArea(CodeEditor *editor);
        ~LineNumberArea();

        QSize sizeHint() const override;

    protected:
        void paintEvent(QPaintEvent *event) override;

    private:
        CodeEditor *codeEditor;
    };
}
#endif // LINENUMBERAREA_H
