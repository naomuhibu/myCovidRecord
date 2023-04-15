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

namespace Ui {
class qrCodeDialog;
}

class qrCodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit qrCodeDialog(QWidget *parent = nullptr);
    ~qrCodeDialog();

private slots:
    void on_pushButton_released();

private:
    Ui::qrCodeDialog *ui;
};

#endif // QRCODEDIALOG_H
