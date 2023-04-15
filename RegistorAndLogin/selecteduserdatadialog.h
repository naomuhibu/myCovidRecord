#ifndef SELECTEDUSERDATADIALOG_H
#define SELECTEDUSERDATADIALOG_H

#include <QDialog>
#include <Qtimer>
#include <QDateTime>
#include <Qtsql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <QtSql/QSqlError>
#include<QSqlTableModel>

namespace Ui {
class selectedUserDataDialog;
}

class selectedUserDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit selectedUserDataDialog(QString userId,QString userName,QString email,QString mobile,QWidget *parent = nullptr);
    ~selectedUserDataDialog();

private slots:
    void on_pushButton_released();

    void on_testResultPushButton_released();

    void on_VaccineRecordPushButton_released();

private:
    Ui::selectedUserDataDialog *ui;
    QTimer *timer;
    QSqlDatabase database;
    QSqlDatabase testVaccineDatabase;
    QSqlTableModel *testModel;
    QSqlTableModel *vaccineModel;
    QString m_userId;

    void closeDatabase();
    QString viewUserData(QString userId,QString userName,QString email,QString mobile);
    void viewTestVaccineData(QString userId);

};

#endif // SELECTEDUSERDATADIALOG_H
