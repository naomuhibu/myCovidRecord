#include "qrcodedialog.h"
#include "ui_qrcodedialog.h"
#include "qrcodegen.hpp"

qrCodeDialog::qrCodeDialog(const QString m_userId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qrCodeDialog)
{
    ui->setupUi(this);

    connectDatabase(); // call the connectDatabase() function instead of declaring it
    viewUserData(m_userId);
}

qrCodeDialog::~qrCodeDialog()
{
    delete ui;
}

using qrcodegen::QrCode;
using qrcodegen::QrSegment;

void qrCodeDialog::connectDatabase()
{
    // create a QSqlDatabase object
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    // set the database file name
    db.setDatabaseName("/Users/Shared/RegistorAndLogin/userdata.sqlite3");

    // can not open the database
    if (!db.open()) {
        QMessageBox::critical(this, "Error", "Unable to open database!" + db.lastError().text());
        return;
    }
}

QString qrCodeDialog::viewUserData(QString m_userId){

    // Update the input date to the current date and time
    ui->inputDate->setText(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm"));

    // prepare the query
    QSqlQuery qry;
    qry.prepare("SELECT * FROM UserData WHERE userId = :userId");
    qry.bindValue(":userId", m_userId);

    // execute the query
    if (!qry.exec()) {
        QMessageBox::critical(this, "Error", "Failed to execute query!" + qry.lastError().text());
        return "";
    }

    // retrieve the user data from the query result
    if (qry.next()) {
        QString userId = qry.value(0).toString();
        QString firstName = qry.value(1).toString();
        QString lastName = qry.value(2).toString();
        QString email = qry.value(3).toString();
        QString birthday = qry.value(5).toString();
        QString gender = qry.value(6).toString();
        QString address = qry.value(7).toString();
        QString vaccine = qry.value(9).toString();
        QString mobile = qry.value(8).toString();
        QString note = qry.value(10).toString();

        // Get text from the plainTextEdit widget
        QString text = userId + "," + firstName + "," + lastName + "," + email + "," + birthday + "," + gender + "," + address + "," + vaccine + "," + mobile;

        // Generate QR code with error correction level low
        QrCode::Ecc errCorLv1 = QrCode::Ecc::LOW;
        QrCode qr = QrCode::encodeText(text.toStdString().c_str(), errCorLv1);

        // Set scaling factor and calculate size of image
        int scale = 10;
        int size = qr.getSize() * scale;

        // Calculate byte width of image
        int byteWidth = (size + 7) / 8;

        // Create char array to store image data
        char bitsChar[size * byteWidth];

        // Iterate over image data and fill char array with 1s and 0s
        for (int y = 0; y < size; y++) {
            for (int xByte = 0; xByte < byteWidth; xByte++) {
                char bitChar = 0X00;
                if (text != "") {
                    for (int xBit = 0; xBit < 8; xBit++) {
                        int x = xByte * 8 + xBit;
                        int xModule = x / scale;
                        int yModule = y / scale;
                        bool black = qr.getModule(xModule, yModule);
                        bitChar += black << (xBit % 8);
                    }
                }
                bitsChar[y * byteWidth + xByte] = bitChar;
            }
        }

        // Create QImage and fill it with black and white pixels
        QImage image(QSize(size, size), QImage::Format_Mono);
        image.fill(Qt::color0);
        for (int y = 0; y < size; y++) {
            for (int xByte = 0; xByte < byteWidth; xByte++) {
                char byte = bitsChar[y * byteWidth + xByte];
                for (int xBit = 0; xBit < 8; xBit++) {
                    int x = xByte * 8 + xBit;
                    if (x < size) {
                        bool bit = (byte >> (xBit % 8)) & 1;
                        image.setPixel(x, y, bit ? 0 : 1);
                    }
                }
            }
        }

        // Display QImage in a QLabel
        QPixmap pixmap = QPixmap::fromImage(image);
        int w = ui->QRcord->width();
        int h = ui->QRcord->height();
        ui->QRcord->setPixmap(pixmap.scaled(w, h, Qt::KeepAspectRatio));


        // display the user data on the dialog's UI using the QLineEdit widgets

        ui->userIdLineEdit->setText(userId);
        ui->nameLineEdit->setText(firstName + " " + lastName);

        return userId;

    }

    return "";
}
