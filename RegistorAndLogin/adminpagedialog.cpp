#include "adminpagedialog.h"
#include "ui_adminpagedialog.h"
#include "selecteduserdatadialog.h"

adminPageDialog::adminPageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adminPageDialog)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this,SLOT(dateTimeFunction()));
    timer->start(1000);

    // create a QSqlDatabase object
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");

    // set the database file name
    database.setDatabaseName("/Users/Shared/RegistorAndLogin/covidTestAndVaccineData.db");

    // open the database
    if (database.open()) {
        QSqlTableModel *tableModel = new QSqlTableModel();
        tableModel->setTable("covidTestAndVaccineData");
        tableModel->select();

       ui->tableView->setModel(tableModel);

    } else {
        QMessageBox::information(this, "Failed", "Data connection failed");
    }

    connectDatabase();
}

adminPageDialog::~adminPageDialog()
{
    closeDatabase();
    delete ui;
}
void adminPageDialog::dateTimeFunction()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTimeStr = dateTime.toString("dd-MM-yyyy hh:mm");
    ui->todayDate->setText(dateTimeStr);
}

void adminPageDialog::connectDatabase()
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

void adminPageDialog::on_userIDPushButton_released()
{
    QString inputUserId = ui->userIdLineEdit->text();
    // prepare the query
    QSqlQuery qry;
    qry.prepare("SELECT * FROM UserData WHERE userId = :userId ");
    qry.bindValue(":userId",inputUserId);

    // execute the query
    if (!qry.exec()) {
        QMessageBox::critical(this, "Error", "Failed to execute query!" + qry.lastError().text());
        return;
    } else {
    // correct the query
        while (qry.next()) {
            QString userId = qry.value(0).toString();
            // check if userId is correct
           if(userId == inputUserId){
               QMessageBox::information(this, "Success", "User was selected!");
               selectedUserDataDialog *selecteduserdatadialog = new selectedUserDataDialog(userId, QString(), QString(), QString(), this);
               selecteduserdatadialog->show();
           } else {
               QMessageBox::warning(this, "Failed", "Invalid user ID!" + qry.lastError().text());
           }
        }
    }

}

void adminPageDialog::on_userNamePushButton_released()
{
    QString inputUserName = ui->userNameLineEdit->text();
    QStringList nameParts = inputUserName.split(" ");
    if (nameParts.size() != 2) {
        QMessageBox::warning(this, "Invalid input", "Please enter a full name (ex:John Smith)");
        return;
    }
    QString firstName = nameParts[0];
    QString lastName = nameParts[1];

    QSqlQuery qry;
    // prepare the query
    qry.prepare("SELECT * FROM UserData WHERE firstName = :firstName AND lastName = :lastName");
    qry.bindValue(":firstName", firstName);
    qry.bindValue(":lastName", lastName);

    // execute the query
    if (!qry.exec()) {
        QMessageBox::critical(this, "Error", "Failed to execute query!" + qry.lastError().text());
        return;
    } else {
    // correct the query
        while (qry.next()) {
            QString userName = qry.value(1).toString()+" "+qry.value(2).toString();
            // check if userName is correct
           if(userName == inputUserName){
               QMessageBox::information(this, "Success", "User was selected!");
               selectedUserDataDialog *selecteduserdatadialog = new selectedUserDataDialog(QString(), userName, QString(), QString(), this);
               selecteduserdatadialog->show();
           } else {
               QMessageBox::warning(this, "Failed", "Invalid user name!" + qry.lastError().text());
           }
        }
    }
}

void adminPageDialog::on_userEmailPushButton_released()
{
    QString inputEmail = ui->userEmailLineEdit->text();
    // prepare the query
    QSqlQuery qry;
    qry.prepare("SELECT * FROM UserData WHERE email = :email");
    qry.bindValue(":email",inputEmail);

    // execute the query
    if (!qry.exec()) {
        QMessageBox::critical(this, "Error", "Failed to execute query!" + qry.lastError().text());
        return;
    } else {
    // correct the query
        while (qry.next()) {
            QString email = qry.value(3).toString();
            // check if email is correct
           if(email == inputEmail){
               QMessageBox::information(this, "Success", "User was selected");
               selectedUserDataDialog *selecteduserdatadialog = new selectedUserDataDialog(QString(), QString(), email, QString(), this);
               selecteduserdatadialog->show();
           } else {
               QMessageBox::warning(this, "Failed", "Invalid email address!" + qry.lastError().text());
           }
        }
    }

}


void adminPageDialog::on_userMobilePushButton_released()
{
    QString inputMobile = ui->userMobileLineEdit->text();
    // prepare the query
    QSqlQuery qry;
    qry.prepare("SELECT * FROM UserData WHERE mobile = :mobile");
    qry.bindValue(":mobile",inputMobile);

    // execute the query
    if (!qry.exec()) {
        QMessageBox::critical(this, "Error", "Failed to execute query!" + qry.lastError().text());
        return;
    } else {
    // correct the query
        while (qry.next()) {
            QString mobile = qry.value(8).toString();
            // check if mobile is correct
           if(mobile == inputMobile){
               QMessageBox::information(this, "Success", "User was selected");
               selectedUserDataDialog *selecteduserdatadialog = new selectedUserDataDialog(QString(), QString(), QString(), mobile, this);
               selecteduserdatadialog->show();
           } else {
               QMessageBox::warning(this, "Failed", "Invalid mobile number!" + qry.lastError().text());
           }
        }
    }

}
void adminPageDialog::closeDatabase()
{
    // close the database
    if (database.isOpen()) {
        database.close();
    }
}
