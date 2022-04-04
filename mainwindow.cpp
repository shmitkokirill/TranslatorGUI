#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    InitMenuBar();
    InitCodeEditor();
    ShowBnf();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete srcCodeEditor;
}

void MainWindow::InitMenuBar()
{
    QAction* openFileAction = ui->menu->actions().at(0);
    QAction* saveFileAction = ui->menu->actions().at(1);
    connect(
        openFileAction,
        &QAction::triggered,
        this,
        &MainWindow::OpenFileActionClicked
    );
    connect(
        saveFileAction,
        &QAction::triggered,
        this,
        &MainWindow::SaveFileActionClicked
    );
}

void MainWindow::InitCodeEditor()
{
    srcCodeEditor = new SourceCodeEdit::CodeEditor(this);
    ui->srcCodeGroup->layout()->addWidget(srcCodeEditor);
}

void MainWindow::ShowBnf()
{
    QFile bnf(PATH_TO_BNF);
    if (!bnf.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->statusBar->showMessage("Ошибка показа БНФ");
        return;
    }
    QString text;
    QTextStream inputStream(&bnf);
    while (!inputStream.atEnd())
    {
        text.append(inputStream.readAll());
    }
    bnf.close();

    ui->txtBrowserBnf->setFont(QFont("Consolas", 12));
    ui->txtBrowserBnf->setWordWrapMode(QTextOption::NoWrap);
    ui->txtBrowserBnf->setPlainText(text);
}

void MainWindow::OpenFileActionClicked()
{
    const QString FAIL_OPEN_FILE = "Не удалось открыть файл";
    const QString OPEN_FILE = "Открыть файл исходного кода";

    srcFileIsOpened = true;
    QString fileName =
            QFileDialog::getOpenFileName(
                this,
                OPEN_FILE,
                "",
                FILE_OPTIONS
            );
    pathToSrcFile = fileName;
    QFile inputFile(fileName);
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->statusBar->showMessage(FAIL_OPEN_FILE);
        return;
    }
    QString *text = new QString();
    QTextStream inputStream(&inputFile);
    while (!inputStream.atEnd())
    {
        text->append(inputStream.readAll());
    }
    inputFile.close();
    srcCodeEditor->setPlainText(*text);
    delete text;
}

void MainWindow::SaveFileActionClicked()
{
    const QString SAVE_FILE = "Сохранить файл исходного кода";
    QString fileName =
            QFileDialog::getSaveFileName(
                this,
                SAVE_FILE,
                "",
                FILE_OPTIONS
            );
    fileName.append(".shk");
    QFile outputFile(fileName);
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        ui->statusBar->showMessage(FAIL_SAVE_FILE);
        return;
    }
    QString text = srcCodeEditor->toPlainText();
    QTextStream outputStream(&outputFile);
    outputStream << text;
    outputFile.close();
}

void MainWindow::on_btnRunClicked()
{
    if (srcFileIsOpened)
    {
        QFile runFile(pathToSrcFile);
        if (!runFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            ui->statusBar->showMessage(FAIL_SAVE_FILE);
            return;
        }
        QString text = srcCodeEditor->toPlainText();
        QTextStream outputStream(&runFile);
        outputStream << text;
        runFile.close();

        //save the OBJECT file in the same directory
    }
    else
    {
        SaveFileActionClicked();
    }


}
