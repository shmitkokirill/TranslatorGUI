#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <codeeditor.h>
#include "stringmanager.h"
#include "translator.h"

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
    const QString FILE_OBJECT_NAME = "object.oshk";
    const QString FILE_OPTIONS = "Исходный код (*.shk)";
    const QString PATH_TO_BNF = "../TranslatorGUI/bnf";
    const QString FAIL_SAVE_FILE = "Не удалось сохранить файл";

    Ui::MainWindow *ui;
    SourceCodeEdit::CodeEditor *srcCodeEditor { nullptr };
    bool srcFileIsOpened { false };
    QString pathToSrcFile { QString() };
    SHK_Translator::StringManager* stringManager;
    SHK_Translator::Translator* translator;

    void InitMenuBar();
    void InitCodeEditor();
    void ShowBnf();
    /**
     * @brief SaveFile
     * Save the file
     * @param[in] filePath
     * Path to file
     * @param[in] whatToSave
     * Text for saving
     */
    void SaveFile(const QString filePath, const QString *whatToSave);
private slots:
    void OpenFileActionClicked();
    void SaveFileActionClicked();
    void on_btnRunClicked();
    void receiveMsg(QString message);
};
#endif // MAINWINDOW_H
