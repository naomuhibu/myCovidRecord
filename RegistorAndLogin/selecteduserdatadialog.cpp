#include "selecteduserdatadialog.h"
#include "ui_selecteduserdatadialog.h"
#include "testresultdialog.h"
#include "vaccinerecorddialog.h"

selectedUserDataDialog::selectedUserDataDialog(QString userId,QString userName,QString email,QString mobile,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::selectedUserDataDialog)

{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this,SLOT(dateFunction()));
    timer->start(1000);

    viewUserData(userId, userName, email, mobile);
    m_userId= userId;
    viewTestVaccineData(userId);
}

selectedUserDataDialog::~selectedUserDataDialog()
{
    delete ui;
    closeDatabase();
}

QString selectedUserDataDialog::viewUserData(QString userId, QString userName, QString email, QString mobile)
{
    QString inputTodayDate = QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm");
    ui->todayDateSelectedPage->setText(inputTodayDate);

    // create QSqlDatabase objects
    database = QSqlDatabase::addDatabase("QSQLITE");
    // set the database file names
    database.setDatabaseName("/Users/Shared/RegistorAndLogin/userdata.sqlite3");
    // can not open the databases
    if (!database.open()) {
        QMessageBox::critical(this, "Error", "Unable to open userdata database: " + database.lastError().text());
    }

    QSqlQuery qry;
    QString queryString;

    if (!userId.isEmpty()) {
        queryString = "SELECT * FROM UserData WHERE userId = :userId";
        qry.prepare(queryString);
        qry.bindValue(":userId", userId);
    } else if (!userName.isEmpty()) {
        queryString = "SELECT * FROM UserData WHERE firstName || ' ' || lastName = :userName";
        qry.prepare(queryString);
        qry.bindValue(":userName", userName);
    } else if (!email.isEmpty()) {
        queryString = "SELECT * FROM UserData WHERE email = :email";
        qry.prepare(queryString);
        qry.bindValue(":email", email);
    } else if (!mobile.isEmpty()) {
        queryString = "SELECT * FROM UserData WHERE mobile = :mobile";
        qry.prepare(queryString);
        qry.bindValue(":mobile", mobile);
    } else {
        return "";
    }

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
        QString password = qry.value(4).toString();
        QString birthday = qry.value(5).toString();
        QString gender = qry.value(6).toString();
        QString address = qry.value(7).toString();
        QString mobile = qry.value(8).toString();
        QString vaccine = qry.value(9).toString();
        QString note = qry.value(10).toString();
        QString latestUpdate = qry.value(11).toString();


        // display the user data on the dialog's UI using the QLineEdit widgets
        ui->userIdLineEdit->setText(userId);
        ui->firstNameLineEdit->setText(firstName);
        ui->lastNameLineEdit->setText(lastName);
        ui->emailLineEdit->setText(email);
        ui->PasswordLineEdit->setText(password);
        ui->birthdayLineEdit->setText(birthday);
        ui->genderLineEdit->setText(gender);
        ui->addressLineEdit->setText(address);
        if (vaccine == "") {
            ui->vaccineLineEdit->setText("Unvaccinated");
        }else{
            ui->vaccineLineEdit->setText(vaccine);
        }
        ui->mobileLineEdit->setText(mobile);
        ui->noteLineEdit->setText(note);
        ui->latestUpdateLineEdit->setText(latestUpdate);

        return userId + "," +firstName + " " + lastName + "," + email + "," + mobile;
    }

    return "";
}

void selectedUserDataDialog::on_pushButton_released()
{
    // create QSqlDatabase objects
    database = QSqlDatabase::addDatabase("QSQLITE");
    // set the database file names
    database.setDatabaseName("/Users/Shared/RegistorAndLogin/userdata.sqlite3");
    // can not open the databases
    if (!database.open()) {
        QMessageBox::critical(this, "Error", "Unable to open userdata database: " + database.lastError().text());
    }

    // open the database
    if (database.open()) {
        // Retrieve data from input fields
        QString userId = ui->userIdLineEdit->text();
        QString firstName = ui->firstNameLineEdit->text();
        QString lastName = ui->lastNameLineEdit->text();
        QString email = ui->emailLineEdit->text();
        QString password = ui->PasswordLineEdit->text();
        QString birthday = ui->birthdayLineEdit->text();
        QString gender = ui->genderLineEdit->text();
        QString address = ui->addressLineEdit->text();
        QString mobile = ui->mobileLineEdit->text();
        QString vaccine = ui->vaccineLineEdit->text();
        QString note = ui->noteLineEdit->text();
        QString latestUpdate = ui->latestUpdateLineEdit->text();

        // Run our update query
        QSqlQuery qry;
        qry.prepare("UPDATE UserData SET firstName = :firstName, lastName = :lastName, email = :email, password = :password, birthday = :birthday, gender = :gender, address = :address, vaccine = :vaccine, mobile = :mobile, note = :note, latestUpdate = :latestUpdate WHERE userId = :userId");
        qry.bindValue(":userId", userId);
        qry.bindValue(":firstName", firstName);
        qry.bindValue(":lastName", lastName);
        qry.bindValue(":email", email);
        qry.bindValue(":password", password);
        qry.bindValue(":birthday", birthday);
        qry.bindValue(":gender", gender);
        qry.bindValue(":address", address);
        qry.bindValue(":mobile", mobile);
        qry.bindValue(":vaccine", vaccine);
        qry.bindValue(":note", note);
        qry.bindValue(":latestUpdate", latestUpdate);

        if (qry.exec()) {
            // update successful
            QMessageBox::information(this, "Success", "User data updated successfully.");
        } else {
            // update failed
            QMessageBox::critical(this, "Error", "Failed to update user data: " + qry.lastError().text());
        }
    } else {
        // failed to open database
        QMessageBox::critical(this, "Error", "Unable to open database: " + database.lastError().text());
    }
}

void selectedUserDataDialog::viewTestVaccineData(QString userId) {

    // create QSqlDatabase objects
    testVaccineDatabase = QSqlDatabase::addDatabase("QSQLITE");
    // set the database file names
    testVaccineDatabase.setDatabaseName("/Users/Shared/RegistorAndLogin/covidTestAndVaccineData.db");
    // can not open the databases
    if (!testVaccineDatabase.open()) {
        QMessageBox::critical(this, "Error", "Unable to open covid test and vaccine database: " + testVaccineDatabase.lastError().text());
        database.close();
        return;
    }
    // prepare the query to retrieve test and vaccine data
    QSqlQuery qry;
    qry.prepare("SELECT * FROM covidTestAndVaccineData WHERE userId = :userId AND (test IS NOT NULL OR vaccine IS NOT NULL) ORDER BY date DESC");
    qry.bindValue(":userId", userId);

    // execute the query
    if (!qry.exec()) {
        QMessageBox::critical(this, "Error", "Unable to retrieve covid test and vaccine data: " + qry.lastError().text());
        return;
    }

    // create models for the test and vaccine data table views
    QSqlTableModel *testModel = new QSqlTableModel();
    testModel->setTable("covidTestAndVaccineData");
    testModel->setFilter(QString("userId = '%1' AND test IS NOT NULL").arg(userId));
    testModel->select();
    testModel->removeColumn(4); // remove unwanted column

    QSqlTableModel *vaccineModel = new QSqlTableModel();
    vaccineModel->setTable("covidTestAndVaccineData");
    vaccineModel->setFilter(QString("userId = '%1' AND vaccine IS NOT NULL").arg(userId));
    vaccineModel->select();
    vaccineModel->removeColumn(5); // remove unwanted column

    // set the models for the test and vaccine table views
    ui->testResultTableView->setModel(testModel);
    ui->vaccineRecordTableView->setModel(vaccineModel);

}

void selectedUserDataDialog::on_testResultPushButton_released()
{
    // create QSqlDatabase objects
    database = QSqlDatabase::addDatabase("QSQLITE");
    // set the database file names
    database.setDatabaseName("/Users/Shared/RegistorAndLogin/userdata.sqlite3");
    // can not open the databases
    if (!database.open()) {
        QMessageBox::critical(this, "Error", "Unable to open userdata database: " + database.lastError().text());
    }

    QSqlQuery qry;
    qry.prepare("SELECT * FROM UserData WHERE userId = :userId ");
    qry.bindValue(":userId",m_userId);

    // execute the query
    if (!qry.exec()) {
        QMessageBox::critical(this, "Error", "Failed to execute query!" + qry.lastError().text());
        return;
    } else {
        // correct the query
        while (qry.next()) {
            QString userId = qry.value(0).toString();
            // check if userId is correct
            if(userId == m_userId){
                testResultDialog *testresultdialog = new testResultDialog(userId,this);
                testresultdialog->show();
            } else {
                QMessageBox::warning(this, "Failed", "Invalid user ID!" + qry.lastError().text());
            }
        }
    }

}

void selectedUserDataDialog::on_VaccineRecordPushButton_released()
{
    // create QSqlDatabase objects
    database = QSqlDatabase::addDatabase("QSQLITE");
    // set the database file names
    database.setDatabaseName("/Users/Shared/RegistorAndLogin/userdata.sqlite3");
    // can not open the databases
    if (!database.open()) {
        QMessageBox::critical(this, "Error", "Unable to open userdata database: " + database.lastError().text());
    }

    QSqlQuery qry;
    qry.prepare("SELECT * FROM UserData WHERE userId = :userId ");
    qry.bindValue(":userId",m_userId);

    // execute the query
    if (!qry.exec()) {
        QMessageBox::critical(this, "Error", "Failed to execute query!" + qry.lastError().text());
        return;
    } else {
        // correct the query
        while (qry.next()) {
            QString userId = qry.value(0).toString();
            // check if userId is correct
            if(userId == m_userId){
                vaccineRecordDialog *vaccinerecorddialog = new vaccineRecordDialog(userId,this);
                vaccinerecorddialog->show();
            } else {
                QMessageBox::warning(this, "Failed", "Invalid user ID!" + qry.lastError().text());
            }
        }
    }
}

void selectedUserDataDialog::closeDatabase()
{
    // close the databases
    if (database.isOpen()) {
        database.close();
    }
    if (testVaccineDatabase.isOpen()) {
        testVaccineDatabase.close();
    }
}
