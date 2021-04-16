#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "jsonData.h"

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
    if(path.isEmpty() == false)
    {
       std::shared_ptr <JsonData> data(new JsonData);
       data->setName(ui->nameLine->text());
       data->setSurname(ui->surnameLine->text());
       data->setAge(ui->ageLine->text().toInt());
       jsdata.push_back(*data);
    }
}

void MainWindow::on_actionOpen_file_triggered()
{
    path = QFileDialog::getOpenFileName(nullptr,"","","*.json");
    file.setFileName(path);
    file.close();
}

void MainWindow::on_loadButton_clicked()
{
    on_actionOpen_file_triggered();
    if(file.open(QIODevice::ReadOnly))
    {
       document = QJsonDocument::fromJson(QByteArray(file.readAll()), &documentErr);
       file.close();

       if(documentErr.errorString().toInt() == QJsonParseError::NoError)
       {
           jsdata.clear();
           QStandardItemModel* model = new QStandardItemModel(nullptr);
           QVector <QString> headers = {"Name", "Surname", "Age"};
           model->setHorizontalHeaderLabels(headers);

           documentArr = QJsonValue(document.object().value("Workers")).toArray();
           for(int i=0; i < documentArr.count(); i++)
           {
              std::shared_ptr <JsonData> data(new JsonData);
              data->setName(documentArr.at(i).toObject().value("Name").toString());
               QStandardItem* name = new QStandardItem(documentArr.at(i).toObject().value("Name").toString());
              data->setSurname(documentArr.at(i).toObject().value("Surname").toString());
               QStandardItem* surname = new QStandardItem(documentArr.at(i).toObject().value("Surname").toString());
              data->setAge(documentArr.at(i).toObject().value("Age").toInt());
               QStandardItem* age = new QStandardItem(QString::number(documentArr.at(i).toObject().value("Age").toInt()));
               model->appendRow(QVector<QStandardItem*>()<< name << surname << age);

               jsdata.push_back(*data);
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
    if(path.isEmpty() == false)
    {
       jsdata.remove(ui->indexLine->text().toInt() - 1);
    }
}

void MainWindow::on_editButton_clicked()
{
    if(path.isEmpty() == false)
    {
       jsdata[ui->indexLine_2->text().toInt() - 1].setName(ui->nameLine_2->text());
       jsdata[ui->indexLine_2->text().toInt() - 1].setSurname(ui->surnameLine_2->text());
       jsdata[ui->indexLine_2->text().toInt() - 1].setAge(ui->ageLine_2->text().toInt());
    }
}

void MainWindow::on_saveButton_clicked()
{
    on_actionOpen_file_triggered();
    if(jsdata.isEmpty() == false)
    {
    QJsonObject jsObj;
    while(documentArr.isEmpty() == false)
    {
       documentArr.removeAt(0);
    }
    for(auto i = jsdata.begin(); i < jsdata.end(); i++)
    {
        jsObj.insert("Name",QJsonValue::fromVariant(i->getName()));
        jsObj.insert("Surname",QJsonValue::fromVariant((i->getSurname())));
        jsObj.insert("Age",QJsonValue::fromVariant(i->getAge()));
        documentArr.push_back(jsObj);
    }
    QJsonDocument dc(documentArr);
    file.open(QIODevice::WriteOnly);
    file.write("{\"Workers\":" + dc.toJson() + "}");
    file.close();
    }
}

