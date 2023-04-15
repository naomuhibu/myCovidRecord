#ifndef VACCINERECORDDIALOG_H
#define VACCINERECORDDIALOG_H

#include <QDialog>
#include <QDialog>
#include <Qtimer>
#include <QDateTime>
#include <Qtsql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <QtSql/QSqlError>

namespace Ui {
class vaccineRecordDialog;
}

class vaccineRecordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit vaccineRecordDialog(QString userId, QWidget *parent = nullptr);
    ~vaccineRecordDialog();

private slots:
    void on_pushButton_released();

private:
    Ui::vaccineRecordDialog *ui;
    QTimer *timer;
    QSqlDatabase database;
    QString m_userId;

    QString viewUserData(QString userId);
    void connectDatabase();
    void closeDatabase();
};

#endif // VACCINERECORDDIALOG_H
