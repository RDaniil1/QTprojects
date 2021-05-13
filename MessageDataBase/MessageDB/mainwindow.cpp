#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Client-1");
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(5433);
    connect(udpSocket, SIGNAL(readyRead()), SLOT(DisplayMessages()));

    db = QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName("practice3");
    if(db.open("test","pass") == false)
        qDebug() << "Can't establish connection to " + db.databaseName() << db.lastError();
    else qDebug() << "Succesfully connected to " + db.databaseName();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_sendMsgButton_clicked()
{
    if(ui->msgLine->text().isEmpty() == false)
    {
    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    QString msg ="Client-1: " + ui->msgLine->text() + "\n" + QDateTime::currentDateTime().toString();
    out << msg;
    udpSocket->writeDatagram(byteArray, QHostAddress::LocalHost, 5432);
    InsertMessage(ui->msgLine->text());
    ui->msgEdit->moveCursor(QTextCursor::End);
    ui->msgEdit->insertPlainText(msg + "\n");
    }
}

void MainWindow::on_showDbButton_clicked()
{
    QSqlQuery query;
    if (query.exec("SELECT * FROM allMsg"))
    {
        qStandardItemModel = new QStandardItemModel();
        qStandardItemModel->setHorizontalHeaderLabels(QStringList()<< "Client id" << "Message" << "Time");

        QSqlRecord rec = query.record();
        QString clientId, message, time;

        while (query.next())
        {
            clientId = query.value(rec.indexOf("client_id")).toString();
            message = query.value(rec.indexOf("message")).toString();
            time = query.value(rec.indexOf("time")).toString();

            QStandardItem* itemCol1(new QStandardItem(clientId));
            QStandardItem* itemCol2(new QStandardItem(message));
            QStandardItem* itemCol3(new QStandardItem(time));

            qStandardItemModel->appendRow(QList<QStandardItem*>()<<itemCol1<<itemCol2<<itemCol3);
        }
        ui->tableView->setModel(qStandardItemModel);
    }
}

void MainWindow::InsertMessage(QString str)
{

    QSqlQuery query;
    QString str_insert = "INSERT INTO allMsg VALUES ('%1', '%2', '%3');";
    QString row = str_insert.arg(_clientId).arg(str).arg(QDateTime::currentDateTime().toString());
    if(query.exec(row) == false)
        QMessageBox::critical(nullptr,"Error","Something went wrong");

}

void MainWindow::DisplayMessages()
{
    QByteArray byteArray;
    do
    {
        byteArray.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(byteArray.data(), byteArray.size());
    }
    while (udpSocket->hasPendingDatagrams());

    QDataStream in(&byteArray,QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_DefaultCompiledVersion);
    QString str;
    in >> str;
    ui->msgEdit->moveCursor(QTextCursor::End);
    ui->msgEdit->insertPlainText(str + "\n");
}
