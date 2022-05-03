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

    connect(
        ui->btnRun,
        &QPushButton::clicked,
        this,
        &MainWindow::on_btnRunClicked
    );

    stringManager = new SHK_Translator::StringManager();
    connect(
        stringManager,
        &SHK_Translator::StringManager::sendMsgToBar,
        this,
        &MainWindow::receiveMsg
    );
    translator = new SHK_Translator::Translator();
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

void MainWindow::SaveFile(const QString filePath, const QString *whatToSave)
{
    QFile outputFile(filePath);
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        ui->statusBar->showMessage(FAIL_SAVE_FILE);
        return;
    }
    QTextStream outputStream(&outputFile);
    outputStream << *whatToSave;
    outputFile.close();
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
    pathToSrcFile = fileName;
    if (!fileName.contains(".shk"))
        fileName.append(".shk");
    QString text = srcCodeEditor->toPlainText();
    SaveFile(fileName, &text);
}

void MainWindow::on_btnRunClicked()
{
    ui->statusBar->clearMessage();
    QString text = srcCodeEditor->toPlainText();
    if (srcFileIsOpened)
    {
        SaveFile(pathToSrcFile, &text);
    }
    else
    {
        SaveFileActionClicked();
    }
    QString fileDirectory = stringManager->TrimThePathFromRight(pathToSrcFile);
    QStringList *objectFile = new QStringList();
    if (stringManager->TrimFile(&text, objectFile))
    {
        SaveFile(fileDirectory + FILE_OBJECT_NAME, &text);
    }
    else
    {
        ui->statusBar->showMessage("Не удалось получить объектный файл");
        return ;
    }

    int resMain = translator->Main(&text);
    if (resMain)
    {
        ui->statusBar->showMessage("Ошибка " + QString::number(resMain));
        return;
    }
    ui->outputTxtBrowser->setText(
        stringManager->getOutputResult(translator->getVariables())
    );
}

void MainWindow::receiveMsg(QString message)
{
    ui->statusBar->showMessage(message);
}
