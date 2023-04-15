#include "issues.h"
#include "ui_issues.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

Issues::Issues(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Issues)
{
    ui->setupUi(this);
}

Issues::~Issues()
{
    delete ui;
}
//Send button on Issues Page
void Issues::on_pushButton_clicked()
{
    QFile file("covidDesktopAppuserIssues.txt");

    if (!file.open(QFile::Append | QFile::Text)) {
        QMessageBox::warning(this,"title","File not open");
    } else {
        QMessageBox::information(this,"Success","This message was sent to Administrator");
    }

    QTextStream out(&file);
    QString userName = ui->userName->text();
    QString email = ui->emailAddress->text();
    QString text = ui->plainTextEdit->toPlainText();

    // Write the message csv data to the file as a single line
    out << userName << ',' << email << ',' << text << '\n';

    file.flush();
    file.close();
}


