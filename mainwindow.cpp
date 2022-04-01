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

}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::OpenFileActionClicked()
{
    const QString FAIL_OPEN_FILE = "Не удалось открыть файл";
    const QString OPEN_FILE = "Открыть файл исходного кода";
    QString fileName =
            QFileDialog::getOpenFileName(
                this,
                OPEN_FILE,
                "",
                FILE_OPTIONS
            );
    QFile inputFile(fileName);
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->statusBar->showMessage(FAIL_OPEN_FILE);
        return;
    }
    QString* text = new QString();
    QTextStream inputStream(&inputFile);
    while (!inputStream.atEnd()) {
        text->append(inputStream.readAll());
    }
    inputFile.close();
    ui->sourceCodeTxtEdit->setPlainText(*text);
    delete text;
}

void MainWindow::SaveFileActionClicked()
{
    const QString FAIL_SAVE_FILE = "Не удалось создать файл";
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
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        ui->statusBar->showMessage(FAIL_SAVE_FILE);
        return;
    }
    QString text = ui->sourceCodeTxtEdit->toPlainText();
    QTextStream outputStream(&outputFile);
    outputStream << text;
    outputFile.close();
}

