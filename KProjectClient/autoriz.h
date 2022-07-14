#ifndef AUTORIZ_H
#define AUTORIZ_H

#include <QDialog>
#include <QtSql>


namespace Ui {
class Autoriz;
}

class Autoriz : public QDialog
{
    Q_OBJECT

public:
    explicit Autoriz(QWidget *parent = nullptr);
    ~Autoriz();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Autoriz *ui;
    QSqlDatabase sdb;
    QString Rezerv = "ggwp";
};

#endif // AUTORIZ_H
