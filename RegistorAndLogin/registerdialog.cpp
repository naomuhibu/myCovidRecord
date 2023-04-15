#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "userpagedialog.h"

// create contractor for registerdialog
registerDialog::registerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registerDialog)
{
    ui->setupUi(this);

    ui->firstNameLineEdit->setPlaceholderText("Enter your first name");
    ui->lastNameLineEdit->setPlaceholderText("Enter your last name");
    ui->emailLineEdit->setPlaceholderText("Enter your email address");
    ui->userIdLineEdit->setPlaceholderText("Enter your ID");
    ui->passwordLineEdit->setPlaceholderText("Enter your password");
    ui->confirmLineEdit->setPlaceholderText("confirm the password");

}
// create distractor for registerdialog
registerDialog::~registerDialog()
{
    delete ui;
}

void registerDialog::on_createAccountButton_released()
{
    // create a QSqlDatabase object
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");

    // set the database file name
    database.setDatabaseName("/Users/Shared/RegistorAndLogin/userdata.sqlite3");

    // open the database
    if (database.open()) {
        // Retrieve data From input fields
        QString firstName = ui->firstNameLineEdit->text();
        QString lastName = ui->lastNameLineEdit->text();
        QString email = ui->emailLineEdit->text();
        QString userId = ui->userIdLineEdit->text();
        QString password = ui->passwordLineEdit->text();
        QString confirmPassword = ui->confirmLineEdit->text();

        // Check if the password and confirm password fields match
        if (password != confirmPassword) {
            QMessageBox::warning(this, "Error", "Passwords do not match");
            return;
        }

        QString gender = "";
        if (ui->maleRadioButton->isChecked()) {
            gender = "Male";
        }
        else if (ui->femaleRadioButton->isChecked()) {
            gender = "Female";
        }
        else if (ui->diverseRadioButton->isChecked()) {
            gender = "Diverse";
        }

        QDate dateOfBirth = ui->dateEdit->date();
        QString dateString = dateOfBirth.toString("dd-MM-yyyy");

        // Run our insert Query
        QSqlQuery qry;

        qry.prepare("INSERT INTO UserData (userId, firstName, lastName, email, password, birthday, gender) "
                    "VALUES (:userId, :firstName, :lastName, :email, :password, :birthday, :gender)");
        qry.bindValue(":userId", userId);
        qry.bindValue(":firstName", firstName);
        qry.bindValue(":lastName", lastName);
        qry.bindValue(":email", email);
        qry.bindValue(":password", password);
        qry.bindValue(":birthday", dateString);
        qry.bindValue(":gender", gender);

        if (qry.exec()) {
            // insert successful
            QMessageBox::information(this, "Success", "User registered successfully!");
            userPageDialog *userpagedialog = new userPageDialog(userId, this);

            userpagedialog->show();
        } else {
            // insert failed
            QMessageBox::warning(this, "Error", "Failed to register: " + qry.lastError().text());
        }

        // close the database
        database.close();
    }else {
        QMessageBox::warning(this,"Not connected", "Failed to open database: " + database.lastError().text());
    }
}

