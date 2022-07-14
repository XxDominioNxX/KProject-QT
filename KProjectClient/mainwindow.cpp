#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "windows.h"
#include <QTableView>
#include <QStandardItemModel>
#include <QtSql>
#include <QMessageBox>
#include <QAxObject>
#include "autoriz.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    aut.setWindowFlags(Qt::ToolTip);
    aut.exec();
    socket = new QTcpSocket();
    connect(socket, &QTcpSocket::readyRead,this, &MainWindow::slotReadyRead);

    //--------------
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(9);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"IP_adress"<<
                                               "OEM_ID"<< "Number_of_processors"<<
                                               "Page_size"<< "Processor_type"<<
                                               "ProcessorRevision"<<
                                               "AllocationGranularity"<<
                                               "Active_processor_mask"<<
                                               "ProcessorLevel"<<"id");
    sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName("mp.db");
    sdb.open();
    ShowTable();
    kEncryptorString = "Hello world!";
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_clicked()
{
    socket->connectToHost(ui->lineEdit->text(), ui->lineEdit_2->text().toInt());
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(con(QAbstractSocket::SocketError)), Qt::DirectConnection);

}



void MainWindow::con(QAbstractSocket::SocketError){
    QMessageBox::warning(this,"Ошибка","Error connect");
}

void MainWindow::slotReadyRead(){
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_9);
    if(in.status() == QDataStream::Ok){
        QString str;
        in >> str;
        QJsonDocument sd = QJsonDocument::fromJson(cryptString(str).toUtf8());
        QJsonObject item = sd.object();
        ui->textBrowser->setText(QJsonDocument(item).toJson(QJsonDocument::Indented));
        ui->textBrowser->append(cryptString(QJsonDocument(item).toJson(QJsonDocument::Indented)));
        //---------------------------------------------------------------------------
                 //sdb.setDatabaseName("mp.db");
                 QSqlQuery query;
                 query.exec("SELECT * FROM tabl");
                 query.last();
                 int count = query.at();
                 query.first();
                 QString str_insert = "INSERT INTO tabl ("
                                      "IP_adress, "
                                      "OEM_ID, "
                                      "Number_of_processors,"
                                      "Page_size,"
                                      "Processor_type,"
                                      "ProcessorRevision,"
                                      "AllocationGranularity,"
                                      "Active_processor_mask,"
                                      "ProcessorLevel,"
                                      "id)VALUES ('%1', %2, %3, %4, %5, %6, %7, %8, %9, %10)";
                 str_insert = str_insert.arg(ui->lineEdit->text() + ":" + ui->lineEdit_2->text()).
                                 arg(item["OEM_ID"].toString()).
                                 arg(item["Number_of_processors"].toString()).
                                 arg(item["Page_size"].toString()).
                                 arg(item["Processor_type"].toString()).
                                 arg(item["ProcessorRevision"].toString()).
                                 arg(item["AllocationGranularity"].toString()).
                                 arg(item["Active_processor_mask"].toString()).
                                 arg(item["ProcessorLevel"].toString()).
                                 arg(count + 2);
                 bool a = query.exec(str_insert);
                 if(!a){
                     QMessageBox::warning(this, "Внимание","Ошибка при добавлении элемента в Базу данных!");
                 }
                 ShowTable();
        socket->close();
    }
}
void MainWindow::on_pushButton_2_clicked()
{
    QSqlQuery query;
    query.exec("SELECT * FROM tabl");
    query.last();
    int count = query.at();
    query.first();
    SYSTEM_INFO siSysInfo;
    GetSystemInfo(&siSysInfo);
    QString str_insert = "INSERT INTO tabl ("
                         "IP_adress, "
                         "OEM_ID, "
                         "Number_of_processors,"
                         "Page_size,"
                         "Processor_type,"
                         "ProcessorRevision,"
                         "AllocationGranularity,"
                         "Active_processor_mask,"
                         "ProcessorLevel,"
                         "id)VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9','%10')";
    str_insert = str_insert.arg("LocalHost").
                    arg(siSysInfo.dwOemId).
                    arg(siSysInfo.dwNumberOfProcessors).
                    arg(siSysInfo.dwPageSize).
                    arg(siSysInfo.dwProcessorType).
                    arg(siSysInfo.wProcessorRevision).
                    arg(siSysInfo.dwAllocationGranularity).
                    arg(siSysInfo.dwActiveProcessorMask).
                    arg(siSysInfo.wProcessorLevel).
                    arg(count + 2);
    bool a = query.exec(str_insert);
    if(!a){
        QMessageBox::warning(this, "Внимание","Ошибка при добавлении элемента в Базу данных!");
    }
    ShowTable();
}

//-----------------------------------------------------------
void MainWindow::ShowTable(){
    ClearTable();
    QSqlQuery query;
    query.exec("SELECT * FROM tabl");
    query.last();
    QSqlRecord rec = query.record();
    ui->label_2->setText(QString::number(query.at() + 1));
    QVector<QString> Etalon = GetEtalon();
    query.first();
    int count = 0;
        do{
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            QVector<QString> bd;
            bd.append(query.value(rec.indexOf("IP_adress")).toString());
            bd.append(query.value(rec.indexOf("OEM_ID")).toString());
            bd.append(query.value(rec.indexOf("Number_of_processors")).toString());
            bd.append(query.value(rec.indexOf("Page_size")).toString());
            bd.append(query.value(rec.indexOf("Processor_type")).toString());
            bd.append(query.value(rec.indexOf("ProcessorRevision")).toString());
            bd.append(query.value(rec.indexOf("AllocationGranularity")).toString());
            bd.append(query.value(rec.indexOf("Active_processor_mask")).toString());
            bd.append(query.value(rec.indexOf("ProcessorLevel")).toString());

            if(bd[0] == "LocalHost"){
                for (int i = 0; i < bd.size(); i++) {
                    ui->tableWidget->setItem(count,i,new QTableWidgetItem(tr("%1").arg(bd[i])));
                }
            }else{
                for (int i = 0; i < bd.size(); i++) {
                    if(bd[i] == Etalon[i]){
                        ui->tableWidget->setItem(count,i,new QTableWidgetItem(tr("%1").arg("(+) " + bd[i])));
                    }else{
                        ui->tableWidget->setItem(count,i,new QTableWidgetItem(tr("%1").arg("(-) " + bd[i])));
                    }
                }
            }

            count++;
        }while(query.next());

}

void MainWindow::ClearTable(){
    QSqlQuery query;
    query.exec("SELECT * FROM tabl");
    query.last();
    int count = query.at();
    query.first();
    for (int i = 0; i < count + 1; i++) {
        ui->tableWidget->removeRow(0);
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    ClearTable();
    QSqlQuery query;
    query.exec("DELETE FROM tabl");
    ShowTable();
}

void MainWindow::on_pushButton_3_clicked()
{
    if(SelectedRow != -1){
        //интерфейс к MSWord
        QAxObject *wordApp = new QAxObject("Word.Application");
        //интерфейс к подобъекту - коллекция скрытых документов
        QAxObject *wordDoc = wordApp->querySubObject("Documents()");
        //создаем новый документ
        QAxObject *newDoc = wordDoc->querySubObject("Add()");

        ///Заголовок для входных данных
        QAxObject *rangeInputData = newDoc->querySubObject("Range()");
        rangeInputData->dynamicCall("SetRange(int, int)", 0, 100);
        rangeInputData->setProperty("Text", "Отчет об аппаратной части");

        //размер, шрифт, выравнивание
        QAxObject *font_rangeInputData = rangeInputData->querySubObject("Font");
        font_rangeInputData->setProperty("Size", 12);
        font_rangeInputData->setProperty("Name", "Arial");
        QAxObject *alignment_rangeInputData = rangeInputData->querySubObject("ParagraphFormat");
        alignment_rangeInputData->setProperty("Alignment", "wdAlignParagraphCenter");
        alignment_rangeInputData->setProperty("SpaceAfter", 0);

        //отступ
        rangeInputData->dynamicCall("InsertParagraphAfter()");
        rangeInputData->dynamicCall("InsertParagraphAfter()");

        //таблица 1
        QAxObject *tables = newDoc->querySubObject("Tables()");
        QAxObject *rangeTable1 = newDoc->querySubObject("Range()");
        rangeTable1->dynamicCall("SetRange(int, int)", 101, 200);
        QAxObject* table1 = tables->querySubObject("Add(Range, NumRows, NumColumns, DefaultTableBehavior, AutoFitBehavior)", rangeTable1->asVariant(), 10, 3, 1, 1);

        //Заполняем таблицу
        //горизонтальные заголовки
        QAxObject *currentCell = table1->querySubObject("Cell(Row, Column)", 1, 1);
        QAxObject *rangeCurrentCell = currentCell->querySubObject("Range()");
        QAxObject *razmer = rangeCurrentCell->querySubObject("Columns()");
        razmer->setProperty("Width",100);// устанавливаю её размер
        razmer->setProperty("Height",50);
        rangeCurrentCell->dynamicCall("InsertAfter(Text)", "Название");

        currentCell = table1->querySubObject("Cell(Row, Column)", 1, 2);
        rangeCurrentCell = currentCell->querySubObject("Range()");
        QAxObject *razmer2 = rangeCurrentCell->querySubObject("Columns()");
        razmer2->setProperty("Width",150);// устанавливаю её размер
        rangeCurrentCell->dynamicCall("InsertAfter(Text)", "Эталонные значения");

        currentCell = table1->querySubObject("Cell(Row, Column)", 1, 3);
        rangeCurrentCell = currentCell->querySubObject("Range()");
        QAxObject *razmer3 = rangeCurrentCell->querySubObject("Columns()");
        razmer3->setProperty("Width",150);// устанавливаю её размер
        rangeCurrentCell->dynamicCall("InsertAfter(Text)", "Полученные значения");

        //вертикальные заголовки

        currentCell = table1->querySubObject("Cell(Row, Column)", 2, 1);
        rangeCurrentCell = currentCell->querySubObject("Range()");
        razmer = rangeCurrentCell->querySubObject("Rows()");
        razmer->setProperty("Height",25);
        rangeCurrentCell->dynamicCall("InsertAfter(Text)", "IP адресс");

        currentCell = table1->querySubObject("Cell(Row, Column)", 3, 1);
        rangeCurrentCell = currentCell->querySubObject("Range()");
        razmer = rangeCurrentCell->querySubObject("Rows()");
        razmer->setProperty("Height",25);
        rangeCurrentCell->dynamicCall("InsertAfter(Text)", "OEM_ID");

        currentCell = table1->querySubObject("Cell(Row, Column)", 4, 1);
        rangeCurrentCell = currentCell->querySubObject("Range()");
        razmer = rangeCurrentCell->querySubObject("Rows()");
        razmer->setProperty("Height",25);
        rangeCurrentCell->dynamicCall("InsertAfter(Text)", "Number_of_processors");

        currentCell = table1->querySubObject("Cell(Row, Column)", 5, 1);
        rangeCurrentCell = currentCell->querySubObject("Range()");
        razmer = rangeCurrentCell->querySubObject("Rows()");
        razmer->setProperty("Height",25);
        rangeCurrentCell->dynamicCall("InsertAfter(Text)", "Page_size");

        currentCell = table1->querySubObject("Cell(Row, Column)", 6, 1);
        rangeCurrentCell = currentCell->querySubObject("Range()");
        razmer = rangeCurrentCell->querySubObject("Rows()");
        razmer->setProperty("Height",25);
        rangeCurrentCell->dynamicCall("InsertAfter(Text)", "Processor_type");

        currentCell = table1->querySubObject("Cell(Row, Column)", 7, 1);
        rangeCurrentCell = currentCell->querySubObject("Range()");
        razmer = rangeCurrentCell->querySubObject("Rows()");
        razmer->setProperty("Height",25);
        rangeCurrentCell->dynamicCall("InsertAfter(Text)", "ProcessorRevision");

        currentCell = table1->querySubObject("Cell(Row, Column)", 8, 1);
        rangeCurrentCell = currentCell->querySubObject("Range()");
        razmer = rangeCurrentCell->querySubObject("Rows()");
        razmer->setProperty("Height",25);
        rangeCurrentCell->dynamicCall("InsertAfter(Text)", "AllocationGranularity");

        currentCell = table1->querySubObject("Cell(Row, Column)", 9, 1);
        rangeCurrentCell = currentCell->querySubObject("Range()");
        razmer = rangeCurrentCell->querySubObject("Rows()");
        razmer->setProperty("Height",25);
        rangeCurrentCell->dynamicCall("InsertAfter(Text)", "Active_processor_mask");

        currentCell = table1->querySubObject("Cell(Row, Column)", 10, 1);
        rangeCurrentCell = currentCell->querySubObject("Range()");
        razmer = rangeCurrentCell->querySubObject("Rows()");
        razmer->setProperty("Height",25);
        rangeCurrentCell->dynamicCall("InsertAfter(Text)", "ProcessorLevel");

        //Заполняем данные аппаратной части (выбранной строки в таблице)
            for (int i = 0; i < 9; i++) { // тк 9 параметров в строке
                currentCell = table1->querySubObject("Cell(Row, Column)", i+2, 3);
                rangeCurrentCell = currentCell->querySubObject("Range()");
                rangeCurrentCell->dynamicCall("InsertAfter(Text)", ui->tableWidget->item(SelectedRow,i)->text());
            }

        //Заполняем эталонные данные аппаратной части
            QVector<QString> etal;
            etal = GetEtalon();
            for (int i = 0; i < 9; i++) { // тк 9 параметров в строке
                currentCell = table1->querySubObject("Cell(Row, Column)", i+2, 2);
                rangeCurrentCell = currentCell->querySubObject("Range()");
                rangeCurrentCell->dynamicCall("InsertAfter(Text)", etal[i]);
            }


        //делаем ворд видимым
        wordApp->setProperty("Visible", true);

        delete rangeCurrentCell;
        delete currentCell;
        delete table1;
        delete rangeTable1;
        delete tables;

        delete alignment_rangeInputData;
        delete font_rangeInputData;
        delete rangeInputData;
        delete newDoc;
        delete wordDoc;
        delete wordApp;
    }else{
        QMessageBox::warning(this, tr("Ошибка!"),
                                       tr("Выберите строку,\n"
                                          "которую необходимо сравнить."),
                                       QMessageBox::Ok);
    }
}


void MainWindow::on_tableWidget_itemSelectionChanged()
{
    SelectedRow = ui->tableWidget->selectionModel()->selectedRows()[0].row();
}

QVector<QString> MainWindow::GetEtalon(){
    SYSTEM_INFO siSysInfo;
    GetSystemInfo(&siSysInfo);
    QVector<QString> result;
    result.append("LocalHost");
    result.append(QString::number(siSysInfo.dwOemId));
    result.append(QString::number(siSysInfo.dwNumberOfProcessors));
    result.append(QString::number(siSysInfo.dwPageSize));
    result.append(QString::number(siSysInfo.dwProcessorType));
    result.append(QString::number(siSysInfo.wProcessorRevision));
    result.append(QString::number(siSysInfo.dwAllocationGranularity));
    result.append(QString::number(siSysInfo.dwActiveProcessorMask));
    result.append(QString::number(siSysInfo.wProcessorLevel));
    return result;
}

void MainWindow::on_pushButton_5_clicked()
{
    sdb.setDatabaseName("pass_server.db");
    if (!sdb.open()) {
        qDebug() << sdb.lastError().text();
    }else{
        qDebug() <<"Good open database";
    }
    QSqlQuery query;
    QString str_insert = "INSERT INTO pass_server ("
                         "name, pass)VALUES ('%1', '%2')";
    str_insert = str_insert.arg(ui->lineEdit_3->text()).arg(ui->lineEdit_4->text());
    if(query.exec(str_insert)){
        QMessageBox a;
        a.setText("Пользователь успешно создан!");
        a.exec();
        ui->textBrowser->setText(QString("Создан новый пользователь:\nЛогин: %1\nПароль: %2").arg(ui->lineEdit_3->text()).arg(ui->lineEdit_4->text()));
    }else{
        QMessageBox a;
        a.setText("Ошибка регистрации, попробуйте позднее");
        a.exec();
    }


}

void MainWindow::on_pushButton_6_clicked()
{
    sdb.setDatabaseName("pass_server.db");
    if (!sdb.open()) {
        qDebug() << sdb.lastError().text();
    }else{
        qDebug() <<"Good open database";
    }
    QSqlQuery query;
    query.exec("DELETE FROM pass_server");
}

void MainWindow::on_pushButton_7_clicked()
{
    int max = 0;
    int maxIndex = -1;

    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        int counter = 0;
        for (int j = 0; j < ui->tableWidget->columnCount(); j++) {
            if(ui->tableWidget->item(i,j)->text()[2] == QChar('-')){
                ++counter;
            }
        }
        if(counter > max){
            max = counter;
            maxIndex = i + 1;
        }

    }
    if(max != 0){
        QMessageBox::information(this, "Аналитическая функция","Max = " + QString::number(max) + ", Номер строки = " + QString::number(maxIndex));
    }else{
        QMessageBox::information(this, "Аналитическая функция","Все элементы соответствуют эталонным!");
    }

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


