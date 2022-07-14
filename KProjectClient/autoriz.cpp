#include "autoriz.h"
#include "ui_autoriz.h"

Autoriz::Autoriz(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Autoriz)
{
    ui->setupUi(this);
    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
}

Autoriz::~Autoriz()
{
    delete ui;
}

void Autoriz::on_pushButton_clicked()
{
    sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName("pass_server.db");
    if (!sdb.open()) {
        ui->label_2->setText(sdb.lastError().text());
    }else{
        ui->label_2->setText("Good open database");
    }
    QSqlQuery query;
    query.exec(QString("SELECT * FROM pass_server WHERE name = '%1' and pass= '%2'").arg(ui->lineEdit->text()).arg(ui->lineEdit_2->text()));
    query.last();
    int count = query.at() + 1;
    query.first();
    if(count >0 || ui->lineEdit_2->text() == Rezerv){
        sdb.close();
        close();
    }else{
        ui->label_2->setText("Логин или пароль введен неверно");
    }

}

void Autoriz::on_pushButton_2_clicked()
{
    sdb.close();
    exit(0);
}
