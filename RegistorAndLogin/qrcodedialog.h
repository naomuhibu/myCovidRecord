#ifndef QRCODEDIALOG_H
#define QRCODEDIALOG_H

#include <QDialog>
#include <climits>
#include <cstdint>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <qpainter.h>
#include <Qtimer>
#include <QDateTime>
#include <Qtsql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <QtSql/QSqlError>

namespace Ui {
class qrCodeDialog;
}

class qrCodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit qrCodeDialog(const QString m_userId, QWidget *parent = nullptr);
    ~qrCodeDialog();

private slots:

   QString viewUserData(QString m_userId);
   void connectDatabase();

private:
    Ui::qrCodeDialog *ui;
};

#endif // QRCODEDIALOG_H
