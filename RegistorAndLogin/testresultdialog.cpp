#include "testresultdialog.h"
#include "ui_testresultdialog.h"

testResultDialog::testResultDialog(QString userId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::testResultDialog),
    m_userId(userId)
{
    ui->setupUi(this);
    //timer = new QTimer(this);
   // connect(timer, SIGNAL(timeout()),this,SLOT(dateFunction()));
   // timer->start(1000);

    connectDatabase();
    viewUserData(userId);
}

testResultDialog::~testResultDialog()
{
    delete ui;
}

void testResultDialog::connectDatabase()
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
QString testResultDialog::viewUserData(QString userId){

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

void testResultDialog::closeDatabase()
{
    // close the database
    if (database.isOpen()) {
        database.close();
    }
}

void testResultDialog::on_pushButton_released()
{
    QString testResult = "";
    if (ui->positiveCheckBox->isChecked()) {
        testResult = "Positive";
    }
    else if (ui->negatiiveCheckBox->isChecked()) {
        testResult = "negative";
    }

    // Get the input date from the UI
    QDate testResultDate = ui->testResultDateEdit->date();
    QString dateString = testResultDate.toString("dd-MM-yyyy");

    //input date to the current date and time
    QString inputTodayDate = QDateTime::currentDateTime().toString("dd-MM-yyyy hh:mm");

    // create a QSqlDatabase object
    database = QSqlDatabase::addDatabase("QSQLITE");

    // set the database file name
    database.setDatabaseName("/Users/Shared/RegistorAndLogin/covidTestAndVaccineData.db");

    // open the database
    if (database.open()) {
        // Retrieve data from input fields
        QString userId = ui->userIdLineEdit->text();
        QString userName = ui->nameLineEdit->text();

        // Run our update query
        QSqlQuery qry;
        qry.prepare("INSERT INTO covidTestAndVaccineData (inputDate, userId, userName, date, vaccine, test)"
                    "VALUES (:inputDate, :userId, :userName, :date, :vaccine, :test)");
        qry.bindValue(":inputDate", inputTodayDate);
        qry.bindValue(":userId", userId);
        qry.bindValue(":userName", userName);
        qry.bindValue(":date", dateString);
        qry.bindValue(":test", testResult);



        if (qry.exec()) {
            // update successful
            QMessageBox::information(this, "Success", "Test Result updated successfully.");
        } else {
            // update failed
            QMessageBox::critical(this, "Error", "Failed to update user data: " + qry.lastError().text());
        }
        // close the database
        database.close();

    } else {
        // failed to open database
        QMessageBox::critical(this, "Error", "Unable to open database: " + database.lastError().text());
    }
}
