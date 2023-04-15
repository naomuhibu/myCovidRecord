#ifndef ISSUES_H
#define ISSUES_H

#include <QDialog>

namespace Ui {
class Issues;
}

class Issues : public QDialog
{
    Q_OBJECT

public:
    explicit Issues(QWidget *parent = nullptr);
    ~Issues();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Issues *ui;
};

#endif // ISSUES_H
