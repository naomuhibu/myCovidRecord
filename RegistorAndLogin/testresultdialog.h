#ifndef TESTRESULTDIALOG_H
#define TESTRESULTDIALOG_H

#include <QDialog>
#include <QDialog>
#include <Qtimer>
#include <QDateTime>
#include <Qtsql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <QtSql/QSqlError>

namespace Ui {
class testResultDialog;
}

class testResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit testResultDialog(QString userId, QWidget *parent = nullptr);
    ~testResultDialog();

private slots:
    void on_pushButton_released();

private:
    Ui::testResultDialog *ui;
    QTimer *timer;
    QSqlDatabase database;
    QString m_userId;

    QString viewUserData(QString userId);
    void connectDatabase();
    void closeDatabase();
};

#endif // TESTRESULTDIALOG_H
