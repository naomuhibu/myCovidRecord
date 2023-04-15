#ifndef ADMINPAGEDIALOG_H
#define ADMINPAGEDIALOG_H

#include <QDialog>
#include <Qtimer>
#include <QDateTime>
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include<QSqlTableModel>


namespace Ui {
class adminPageDialog;
}

class adminPageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit adminPageDialog(QWidget *parent = nullptr);
    ~adminPageDialog();

public slots:
    void dateTimeFunction();

private slots:

    void on_userIDPushButton_released();

    void on_userNamePushButton_released();

    void on_userEmailPushButton_released();

    void on_userMobilePushButton_released();

private:
    Ui::adminPageDialog *ui;
    QTimer *timer;
    QSqlDatabase database;
    QSqlTableModel *tableModel;

    void connectDatabase();
    void closeDatabase();
};

#endif // ADMINPAGEDIALOG_H
