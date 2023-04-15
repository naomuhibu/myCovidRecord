#include "vaccinerecorddialog.h"
#include "ui_vaccinerecorddialog.h"

vaccineRecordDialog::vaccineRecordDialog(QString userId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::vaccineRecordDialog),
    m_userId(userId)
{
    ui->setupUi(this);
    //timer = new QTimer(this);
   // connect(timer, SIGNAL(timeout()),this,SLOT(dateFunction()));
   // timer->start(1000);

    connectDatabase();
    viewUserData(userId);
}

vaccineRecordDialog::~vaccineRecordDialog()
{
    delete ui;
}

void vaccineRecordDialog::connectDatabase()
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
QString vaccineRecordDialog::viewUserData(QString userId){

    // Update the input date to the current date and time
    ui->inputDate->setText(QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm"));

    // prepare the query
    QSqlQuery qry;
    qry.prepare("SELECT * FROM UserData WHERE userId = :userId");
    qry.bindValue(":userId", userId);

    // execute the query
    if (!qry.exec()) {
        QMessageBox::critical(this, "Error", "Failed to execute query!" + qry.lastError().text());
        return "";
    }
    // retriece the user data a from the query result
    if (qry.next()) {
        QString userId = qry.value(0).toString();
        QString firstName = qry.value(1).toString();
        QString lastName = qry.value(2).toString();

    // display the user data on the dialog's UI using the QLineEdit widgets

        ui->userIdLineEdit->setText(userId);
        ui->nameLineEdit->setText(firstName + " " + lastName);

        return userId;

    }

    return "";
}

void vaccineRecordDialog::closeDatabase()
{
    // close the database
    if (database.isOpen()) {
        database.close();
    }
}

void vaccineRecordDialog::on_pushButton_released()
{
    QString vaccineRecord = "Unvaccinated";
    if (ui->firstRadioButton->isChecked()) {
        vaccineRecord = "Partial";
    }
    else if (ui->secondRadioButton->isChecked()) {
        vaccineRecord = "Completed";
    }

    // create a QSqlDatabase object
    QSqlDatabase vaccineDatabase = QSqlDatabase::addDatabase("QSQLITE", "vaccineConnection");

    // set the database file name
    vaccineDatabase.setDatabaseName("/Users/Shared/RegistorAndLogin/covidTestAndVaccineData.db");

    // open the database
    if (vaccineDatabase.open()) {
        // Retrieve data from input fields
        QString userId = ui->userIdLineEdit->text();
        QString userName = ui->nameLineEdit->text();

        // Get vaccine date
        QDate vaccineDate = ui->vaccineDateEdit->date();
        QString dateString = vaccineDate.toString("dd-MM-yyyy");

        //input date to the current date and time
        QString inputTodayDate = QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm");

        // Run our update query
        QSqlQuery qry(vaccineDatabase);
        qry.prepare("INSERT INTO covidTestAndVaccineData (inputDate, userId, userName, date, vaccine, test)"
                    "VALUES (:inputDate, :userId, :userName, :date, :vaccine, :test)");
        qry.bindValue(":inputDate", inputTodayDate);
        qry.bindValue(":userId", userId);
        qry.bindValue(":userName", userName);
        qry.bindValue(":date", dateString);
        qry.bindValue(":vaccine", vaccineRecord);

        if (qry.exec()) {
            // update successful
            QMessageBox::information(this, "Success", "Vaccine record updated successfully.");
        } else {
            // update failed
            QMessageBox::critical(this, "Error", "Failed to update user data: " + qry.lastError().text());
        }
        // close the database
        vaccineDatabase.close();

    } else {
        // failed to open database
        QMessageBox::critical(this, "Error", "Unable to open database: " + vaccineDatabase.lastError().text());
    }

    // Set vaccine record in userdata.sqlite3
    QSqlDatabase userDatabase = QSqlDatabase::database();
    QSqlQuery userQuery(userDatabase);

    userQuery.prepare("UPDATE UserData SET vaccine = :vaccine WHERE userId = :userId");
    userQuery.bindValue(":userId", m_userId);
    userQuery.bindValue(":vaccine", vaccineRecord);

    if (!userQuery.exec()) {
        QMessageBox::critical(this, "Error", "Failed to update user data: " + userQuery.lastError().text());
    }

    // Close the database connection
    userDatabase.close();

}
