#ifndef USERPAGEDIALOG_H
#define USERPAGEDIALOG_H

#include <QDialog>
#include <Qtimer>
#include <QDateTime>
#include <QSqlDatabase>
#include <QtSql>
#include <QMessageBox>
#include <QSqlError>
#include<QSqlTableModel>
#include<QDesktopServices>
#include <QUrl>

namespace Ui {
class userPageDialog;
}

class userPageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit userPageDialog(QString userId, QWidget *parent = nullptr);
    ~userPageDialog();
public slots:
    void dateFunction();

private slots:
    void on_covidCommandLinkButton_clicked();

    void on_BoosteCommandLinkButton_clicked();

    void on_updateCommandLinkButton_clicked();

    void on_symptomCommandLinkButton_clicked();

    void on_pushButton_released();

private:
    Ui::userPageDialog *ui;
    QTimer *timer;
    QSqlDatabase userDatabase;
    QSqlDatabase testVaccineDatabase;

    QSqlTableModel *testModel;
    QSqlTableModel *vaccineModel;
    QString m_userId;

    QString viewUserData(QString userId);
    void viewTestVaccineData(QString userId);
};

#endif // USERPAGEDIALOG_H
