#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QList>
#include <QVector>
#include <QTextStream>
#include <QPaintEvent>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QPainter>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *) override;

private slots:
    void on_pushButton_clicked();

private:
    void DeleteSymbolsInLine(QString &line, QString symbol);
    void GetClassChildrens(QString className);
    QString Root();
    int GetAmountOfChildrens(QString className);
    Ui::MainWindow *ui;
    QVector<QList<QString>> diagram;
    QVector<QString> childrens;
    QGraphicsScene *scene;
    QGraphicsSimpleTextItem *textItem;
};
#endif // MAINWINDOW_H
