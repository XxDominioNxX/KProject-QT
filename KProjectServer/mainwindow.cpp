#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <windows.h>
#include <QTimer>
#include <QTime>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tmr = new QTimer(this);
    tmr->setInterval(ui->lineEdit_2->text().toInt()*1000); // Задаем интервал таймера c формы
    connect(tmr, SIGNAL(timeout()), this, SLOT(updateTime())); // Подключаем сигнал таймера к нашему слоту
    tmr->start();
    //----------------------------
    m_socket = 0;
    m_server = new QTcpServer(this);
    connect(m_server, SIGNAL(newConnection()), this, SLOT(slotConnection()));
    kEncryptorString = "Hello world!";

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    tmr->start();
    m_server->listen(QHostAddress::Any, ui->lineEdit->text().toInt());
    ui->label_4->setText("Работает");


}

void MainWindow::updateTime()
{
    SYSTEM_INFO siSysInfo;
    GetSystemInfo(&siSysInfo);


    QJsonObject textObject;
    textObject["OEM_ID"] = QString::number(siSysInfo.dwOemId);
    textObject["Number_of_processors"] = QString::number(siSysInfo.dwNumberOfProcessors);
    textObject["Page_size"] = QString::number(siSysInfo.dwPageSize);
    textObject["Processor_type"] = QString::number(siSysInfo.dwProcessorType);
    textObject["ProcessorRevision"] = QString::number(siSysInfo.wProcessorRevision);
    textObject["AllocationGranularity"] = QString::number(siSysInfo.dwAllocationGranularity);
    textObject["Active_processor_mask"] = QString::number(siSysInfo.dwActiveProcessorMask);
    textObject["ProcessorLevel"] = QString::number(siSysInfo.wProcessorLevel);
    jsonSina = textObject;

}

void MainWindow::on_pushButton_2_clicked()
{
    ui->label_4->setText("Остановлен");
}


void MainWindow::slotConnection()
{
    m_socket = m_server->nextPendingConnection();
    ui->textBrowser->append(m_socket->peerAddress().toString().split("::ffff:")[1] + " - Connected");
    ui->textBrowser->append(m_socket->peerAddress().toString().split("::ffff:")[1] + " - Disconnected");
    SendMess();
}


void MainWindow::SendMess()
{
    QString str = QJsonDocument(jsonSina).toJson(QJsonDocument::Compact);
    QDataStream out (& Data, QIODevice :: ReadWrite);
    out <<  cryptString(str);
    m_socket->write(Data);
}

//================Для шифрования=========================

QString MainWindow::cryptString(QString toCrypt)
    {
        QString resultString = "";
        for ( int i = 0; i < toCrypt.length(); i++)
        {
            resultString.append(QString(QChar(toCrypt[i]).unicode()^QChar(kEncryptorString[keyIndex(i)]).unicode()));
        }
        return resultString;
    }

int MainWindow::keyIndex(int index)
    {
        int len = kEncryptorString.length();
        int multiple = index / len;
        if ( index >=  len ) {
            return index - (len * multiple);
        }
        return index;
    }
