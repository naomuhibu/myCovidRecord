#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QPixmap>

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
    Ui::MainWindow *ui;
    QSqlDatabase database;
    void connectDatabase();
    void closeDatabase();

private slots:
    void on_LoginBtn_released();

    void on_RegistorBtn_released();

    void on_AdminBtn_released();

    void on_IssueBtn_released();
};
#endif // MAINWINDOW_H
