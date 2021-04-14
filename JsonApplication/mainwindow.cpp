#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_exitButton_clicked()
{

}

void MainWindow::on_addButton_clicked()
{
    if(file.open(QIODevice::WriteOnly))
    {
    QVariantMap map;
    map.insert("Name", ui->nameLine->text());
    map.insert("Surname", ui->surnameLine->text());
    map.insert("Age", ui->ageLine->text());
    QJsonObject json = QJsonObject::fromVariantMap(map);
    QJsonArray documentWrite = document.object().value("Workers").toArray();
    documentWrite.append(json);

    document.setArray(documentWrite);

    file.close();

    }
    else
    {
        QMessageBox::critical(nullptr,"Warning","Can't open file");
    }
}

void MainWindow::on_actionOpen_file_triggered()
{
    path = QFileDialog::getOpenFileName(nullptr,"","D:\\JSON","*.json");
    file.setFileName(path);
    file.close();
}

void MainWindow::on_loadButton_clicked()
{
    if(file.open(QIODevice::ReadOnly))
    {
       document = QJsonDocument::fromJson(QByteArray(file.readAll()), &documentErr);
       file.close();

       if(documentErr.errorString().toInt() == QJsonParseError::NoError)
       {
           QStandardItemModel* model = new QStandardItemModel(nullptr);
           QVector <QString> headers = {"Name", "Surname", "Age"};
           model->setHorizontalHeaderLabels(headers);

           documentArr = QJsonValue(document.object().value("Workers")).toArray();
           for(int i=0; i < documentArr.count(); i++)
           {
               QStandardItem* name = new QStandardItem(documentArr.at(i).toObject().value("Name").toString());
               QStandardItem* surname = new QStandardItem(documentArr.at(i).toObject().value("Surname").toString());
               QStandardItem* age = new QStandardItem(documentArr.at(i).toObject().value("Age").toString());
               model->appendRow(QVector<QStandardItem*>()<< name << surname << age);
           }
           ui->tableView->setModel(model);
           ui->tableView->resizeColumnsToContents();
           ui->tableView->resizeRowsToContents();
       }
    }
    else
    {
        QMessageBox::warning(nullptr,"Warning","Can't open file for reading");
    }
}

void MainWindow::on_deleteButton_clicked()
{
    if(file.open(QIODevice::WriteOnly))
    {
        QJsonArray documentWrite = document.object().value("Workers").toArray();
        documentWrite.removeAt(ui->indexLine->text().toInt() - 1);

        document.setArray(documentWrite);

        file.close();
    }
    else
    {
        QMessageBox::critical(nullptr,"Warning","Can't open file for deleting");
    }
}

void MainWindow::on_editButton_clicked()
{
    if(file.open(QIODevice::WriteOnly))
    {
    QVariantMap map;
    map.insert("Name", ui->nameLine_2->text());
    map.insert("Surname", ui->surnameLine_2->text());
    map.insert("Age", ui->ageLine_2->text());
    QJsonObject json = QJsonObject::fromVariantMap(map);
    QJsonArray documentWrite = document.object().value("Workers").toArray();
    documentWrite.replace(ui->indexLine_2->text().toInt() - 1, json);
    document.setArray(documentWrite);
    file.close();
    }
    else
    {
        QMessageBox::critical(nullptr,"Warning","Can't open file");
    }
}

void MainWindow::on_saveButton_clicked()
{
    file.open(QIODevice::WriteOnly);
    file.write("{\"Workers\":" + document.toJson() + "}");
    file.close();
}
