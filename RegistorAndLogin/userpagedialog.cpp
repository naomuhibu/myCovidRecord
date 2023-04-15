#include "userpagedialog.h"
#include "ui_userpagedialog.h"
//#include "qrcorddialog.h"

userPageDialog::userPageDialog(QString userId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userPageDialog),
    m_userId(userId)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this,SLOT(dateFunction()));
    timer->start(1000);

    viewUserData(userId);
    viewTestVaccineData(userId);
}

userPageDialog::~userPageDialog()
{
    delete ui;
}

void userPageDialog::dateFunction()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTimeStr = dateTime.toString("dd-MM-yyyy hh:mm");
    ui->currentDateUserPage->setText(dateTimeStr);
}

QString userPageDialog::viewUserData(QString userId){

    // create a QSqlDatabase object
    userDatabase = QSqlDatabase::addDatabase("QSQLITE");

    // set the database file name
    userDatabase.setDatabaseName("/Users/Shared/RegistorAndLogin/userdata.sqlite3");

    // can not open the database
    if (!userDatabase.open()) {
        QMessageBox::critical(this, "Error", "Unable to open database!" + userDatabase.lastError().text());
    } else {
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
            QString email = qry.value(3).toString();
            QString birthday = qry.value(5).toString();
            QString gender = qry.value(6).toString();
            QString address = qry.value(7).toString();
            QString vaccine = qry.value(9).toString();
            QString mobile = qry.value(8).toString();
            QString note = qry.value(10).toString();

            // display the user data on the dialog's UI using the QLineEdit widgets
            ui->userName->setText(firstName);
            ui->userIdLineEdit->setText(userId);
            ui->nameLineEdit->setText(firstName + " " + lastName);
            ui->emailLineEdit->setText(email);
            ui->genderLineEdit->setText(gender);
            ui->birthdayLineEdit->setText(birthday);
            ui->addressLineEdit->setText(address);
            ui->vaccineLineEdit->setText(vaccine);
            ui->mobileLineEdit->setText(mobile);
            ui->noteLineEdit->setText(note);
        }
    }
    userDatabase.close();
    return userId;
}

void userPageDialog::viewTestVaccineData(QString userId) {
    // create a QSqlDatabase object
    QSqlDatabase testVaccineDatabase = QSqlDatabase::addDatabase("QSQLITE");

    // set the database file name
    testVaccineDatabase.setDatabaseName("/Users/Shared/RegistorAndLogin/covidTestAndVaccineData.db");

    // can not open the database
    if (!testVaccineDatabase.open()) {
        QMessageBox::critical(this, "Error", "Unable to open database: " + testVaccineDatabase.lastError().text());
        return;
    }

    // prepare the query to retrieve test and vaccine data
    QSqlQuery qry;
    qry.prepare("SELECT * FROM covidTestAndVaccineData WHERE userId = :userId AND (test IS NOT NULL OR vaccine IS NOT NULL) ORDER BY date DESC");
    qry.bindValue(":userId", userId);

    // execute the query
    if (!qry.exec()) {
        QMessageBox::critical(this, "Error", "Unable to retrieve covid test and vaccine data: " + qry.lastError().text());
        testVaccineDatabase.close();
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
    ui->testTableView->setModel(testModel);
    ui->vaccineTableView->setModel(vaccineModel);

    // close the database
    testVaccineDatabase.close();
}

//Covid vaccine information button
void userPageDialog::on_covidCommandLinkButton_clicked()
{
    QString link="https://covid19.govt.nz/covid-19-vaccines/get-your-covid-19-vaccination/";
    QDesktopServices::openUrl(QUrl(link));
}

//Boosters information button
void userPageDialog::on_BoosteCommandLinkButton_clicked()
{
    QString link="https://covid19.govt.nz/covid-19-vaccines/covid-19-vaccine-boosters/";
    QDesktopServices::openUrl(QUrl(link));
}

//Updates button
void userPageDialog::on_updateCommandLinkButton_clicked()
{
    QString link="https://covid19.govt.nz/news-and-data/latest-news/";
    QDesktopServices::openUrl(QUrl(link));
}

//Symptoms button
void userPageDialog::on_symptomCommandLinkButton_clicked()
{
    QString link="https://covid19.govt.nz/prepare-and-stay-safe/about-covid-19/covid-19-symptoms/";
    QDesktopServices::openUrl(QUrl(link));
}

/*
void userPageDialog::on_pushButton_released()
{
    qrcordDialog *qrcodedialog = new qrcordDialog(this);
    qrcodedialog->show();
}
*/
