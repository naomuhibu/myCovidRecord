#include "qrcodedialog.h"
#include "ui_qrcodedialog.h"
#include "qrcodegen.hpp"

qrCodeDialog::qrCodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qrCodeDialog)
{
    ui->setupUi(this);
}

qrCodeDialog::~qrCodeDialog()
{
    delete ui;
}

using qrcodegen::QrCode;
using qrcodegen::QrSegment;

void qrCodeDialog::on_pushButton_released()
{
    // Get text from the plainTextEdit widget
    QString text = ui->plainTextEdit->toPlainText();

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
    int w = ui->label->width();
    int h = ui->label->height();
    ui->label->setPixmap(pixmap.scaled(w, h, Qt::KeepAspectRatio));
}
