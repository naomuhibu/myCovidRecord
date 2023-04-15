#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "registerdialog.h"
#include "userpagedialog.h"
#include "adminpagedialog.h"
#include"issues.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lineEditUserId->setPlaceholderText("Enter your ID");
    ui->lineEditPassword->setPlaceholderText("Enter your password");

    connectDatabase();
}

MainWindow::~MainWindow()
{
    closeDatabase();
    delete ui;
}

void MainWindow::connectDatabase()
{
    // create a QSqlDatabase object
    database = QSqlDatabase::addDatabase("QSQLITE");

    // set the database file name
    database.setDatabaseName("/Users/Shared/RegistorAndLogin/userdata.sqlite3");

    // can not open the database
    if (!database.open()) {
        QMessageBox::critical(this, "Error", "Unable to open database!" + database.lastError().text());
        return;
    }
}


void MainWindow::on_RegistorBtn_released()
{
    registerDialog *registerdialog = new registerDialog(this);
    registerdialog->show();
}

void MainWindow::on_LoginBtn_released()
{
    QString inputUserId = ui->lineEditUserId->text();
    QString inputPassword = ui->lineEditPassword->text();

    // prepare the query
    QSqlQuery qry;
    qry.prepare("SELECT * FROM UserData WHERE userId = :userId AND password = :password");
    qry.bindValue(":userId", inputUserId);
    qry.bindValue(":password",inputPassword);

    // execute the query
    if (!qry.exec()) {
        QMessageBox::critical(this, "Error", "Failed to execute query!" + qry.lastError().text());
    } else {
    // correct the query
        while (qry.next()) {
            QString userId = qry.value(0).toString();
            QString password = qry.value(4).toString();

             // check if userId and password are correct
            if(userId == inputUserId && password == inputPassword){
                QMessageBox::information(this, "Success", "Login successful!");
                hide();
                userPageDialog *userpagedialog = new userPageDialog(userId,this);
                userpagedialog->show();
            } else {
                QMessageBox::warning(this, "Failed", "Invalid user ID or password!");
            }
        }
    }
}

void MainWindow::on_AdminBtn_released()
{
    QString inputAdminrId = ui->lineEditUserId->text();
    QString inputAdminPassword = ui->lineEditPassword->text();

    // prepare the query
    QSqlQuery qry;
    qry.prepare("SELECT * FROM userData WHERE userId = :userId AND password = :password");
    qry.bindValue(":userId", inputAdminrId);
    qry.bindValue(":password", inputAdminPassword);

    // execute the query
    if (!qry.exec()) {
        QMessageBox::critical(this, "Error", "Failed to execute query!" + qry.lastError().text());
    } else {
        // correct the query
        while (qry.next()) {
            QString adminId = qry.value(0).toString();
            QString adminPassword = qry.value(4).toString();

            // check if userId and password are correct
            if(adminId == inputAdminrId && adminPassword == inputAdminPassword){
                QMessageBox::information(this, "Success", "Login successful!");
                adminPageDialog *adminpagedialog = new adminPageDialog(this);
                adminpagedialog->show();
            } else {
                // show a warning message
                QMessageBox::warning(this, "Failed", "Invalid admin ID or password!");
            }
        }
    }
}

void MainWindow::closeDatabase()
{
    // close the database
    if (database.isOpen()) {
        database.close();
    }
}

void MainWindow::on_IssueBtn_released()
{
    Issues *issues = new Issues(this);
    issues->show();

}

