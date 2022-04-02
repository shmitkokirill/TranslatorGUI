#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <codeeditor.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    const QString FILE_OPTIONS = "Исходный код (*.shk)";
    const QString PATH_TO_BNF = "../TranslatorGUI/bnf";

    Ui::MainWindow *ui;
    SourceCodeEdit::CodeEditor *srcCodeEditor{ nullptr };

    void InitMenuBar();
    void InitCodeEditor();
    void ShowBnf();
private slots:
    void OpenFileActionClicked();
    void SaveFileActionClicked();
};
#endif // MAINWINDOW_H
