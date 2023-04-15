#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QtWidgets/QDialog>
#include <Qtsql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <QtSql/QSqlError>

namespace Ui {
class registerDialog;
}

class registerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit registerDialog(QWidget *parent = nullptr);
    ~registerDialog();

private slots:

    void on_createAccountButton_released();

private:
    Ui::registerDialog *ui;
    QSqlDatabase database;
};

#endif // REGISTERDIALOG_H
