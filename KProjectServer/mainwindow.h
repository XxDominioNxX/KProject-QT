#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTimer>
#include <QTime>
#include <QTcpServer>
#include <QTcpSocket>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTcpServer * m_server;
    QTcpSocket * m_socket;
    void SendMess();

private slots:
    void on_pushButton_clicked();
    void updateTime(); //Слот для обновления данных
    void on_pushButton_2_clicked();
    void slotConnection();




private:
    Ui::MainWindow *ui;
    QTimer *tmr;
    QJsonObject jsonSina;
    QByteArray Data;
    QString kEncryptorString;
    QString cryptString(QString toCrypt);
    int keyIndex(int index);

};
#endif // MAINWINDOW_H
