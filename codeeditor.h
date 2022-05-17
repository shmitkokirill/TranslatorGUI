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
        ~CodeEditor();
        void lineNumberAreaPaintEvent(QPaintEvent *event);
        int lineNumberAreaWidth();
        void highlightTheSymbol(int pos);

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
            CURRENT_LINE = 0xd4d3d2,
            ERROR_SYMBOL = 0xff8f8f
        };

        QWidget *lineNumberArea;

    };
}
#endif // CODEEDITOR_H
