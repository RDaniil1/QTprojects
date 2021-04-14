#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>
#include <QFileDialog>
#include <QStandardItem>
#include <QMessageBox>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QJsonDocument document;
    QJsonArray documentArr;
    QJsonParseError documentErr;

    QString path;
    QFile file;

private slots:
    void on_exitButton_clicked();

    void on_addButton_clicked();

    void on_actionOpen_file_triggered();

    void on_loadButton_clicked();

    void on_deleteButton_clicked();

    void on_editButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::MainWindow *ui;
    void ButtonCl();
};

#endif // MAINWINDOW_H
