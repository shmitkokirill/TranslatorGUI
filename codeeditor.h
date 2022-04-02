#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>

namespace SourceCodeEdit
{
    class CodeEditor : public QPlainTextEdit
    {
        Q_OBJECT
    public:
        CodeEditor(QWidget *parent = nullptr);
        void lineNumberAreaPaintEvent(QPaintEvent *event);
        int lineNumberAreaWidth();

    protected:
        void resizeEvent(QResizeEvent *event) override;

    private slots:
        void updateLineNumberAreaWidth(int newBlockCount);
        void highlightCurrentLine();
        void updateLineNumberArea(const QRect &rect, int dy);
    private:
        enum Colors
        {
            NUMBER_PANEL = 0xfaf5e8,
            CURRENT_LINE = 0xd4d3d2
        };

        QWidget *lineNumberArea;

    };
}
#endif // CODEEDITOR_H
