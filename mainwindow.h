#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

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

    Ui::MainWindow *ui;

    void InitMenuBar();
private slots:
    void OpenFileActionClicked();
    void SaveFileActionClicked();
};
#endif // MAINWINDOW_H
