#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    GetClassesFromFile();
    GetClassChildrens("A");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::GetClassesFromFile()
{
    QFile file("/home/wheatley/Desktop/Lab2/test.h");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text) == false)
    {
        return;
    }
    QTextStream in(&file);
    while (!in.atEnd())
    {
       //Read lines in file
       QString line = in.readLine();
       //Delete ";" at the end of this line
       line.remove(line.count() - 1, 1);
       //Delete "," in the line
       DeleteSymbolsInLine(line, ",");
       //If line contains "class", then split and write in diagram
       if(line.contains("class"))
       {
          QList <QString> children = line.split(" ");
          diagram.push_back(children);
       }
    }
    for(int i = 0;i < diagram.count(); i++)
    {
        diagram[i].pop_front();
    }
    file.close();
}

void MainWindow::DeleteSymbolsInLine(QString &line, QString symbol)
{
    for(int i = 0; i < line.count(); i++)
    {
        if(line.at(i) == symbol)
        {
            line.remove(i, 1);
        }

    }
}

void MainWindow::GetClassChildrens(QString className)
{
    childrens.clear();
    for(int i = 0; i < diagram.count(); i++)
    {
        for(int j = 0; j < diagram[i].count(); j++)
        {
            if(diagram[i][j] == className && j > 1)
            {
                int colonIndex = diagram[i].indexOf(":");
                childrens.push_back(diagram[i][colonIndex - 1]);
            }
        }
    }
    if(childrens.isEmpty() == false)
    {
       qDebug() <<  className + " childrens: " << childrens;
    }
    else qDebug() << className << " don't have any childs";
}

QString MainWindow::Root()
{
    for(int i =0; i < diagram.count(); i++)
    {
        if(diagram[i].contains(":") == false)
        {
            return diagram[i][0];
        }
    }
    return " ";
}

int MainWindow::GetAmountOfChildrens(QString className)
{
    int childrenAmount = 0;
    for(int i = 0; i < diagram.count(); i++)
    {
        for(int j = 0; j < diagram[i].count(); j++)
        {
            if(diagram[i][j] == className && j > 1)
            {
                childrenAmount++;
            }
        }
    }
    return childrenAmount;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    if(diagram.isEmpty() == false)
    {
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 3));
    GetClassChildrens(Root());
    int startX = 75, startY = 75;
    int nodeWidth = 50, nodeHeight = 50;
    int connectLength = 30, indentLength = 30;
    painter.drawRect(startX,startY,nodeWidth,nodeHeight);
    painter.drawText(QRect(startX,startY,nodeWidth,nodeHeight),Qt::AlignCenter,Root());
    int indentFactor = diagram.count() - 2;
    int prevChildsCount = 1;
    QVector <QString> prevChilds = childrens;
    for(int l = 0; l < diagram.count(); l++)
    {
        GetClassChildrens(diagram[l][0]);
        indentFactor = diagram.count() - 2 - l;
        int childTimes = 0;
        //Print line of childrens
        while(prevChildsCount != 0)
        {
            if(l > 0)
                GetClassChildrens(prevChilds[childTimes]);

    //Draw child nodes
    for(int i = 0; i < childrens.count(); i++)
    {
        if(l == 0)
        {
        painter.drawLine(startX+nodeWidth/2+80*childTimes*(indentFactor+1), startY+nodeHeight+(l*80),
                             startX+nodeWidth/2+80*childTimes*(indentFactor+1), startY+nodeHeight+connectLength +(l*80));
        painter.drawRect(startX+(nodeWidth+indentLength)*(i)*indentFactor+80*childTimes*(indentFactor+1),startY+(nodeHeight+connectLength)*(l+1),
                         nodeWidth,nodeHeight);
        painter.drawText(QRect(startX+(nodeWidth+indentLength)*(i)*indentFactor+80*childTimes*(indentFactor+1),startY+(nodeHeight+connectLength)*(l+1),
                               nodeWidth,nodeHeight),Qt::AlignCenter,childrens[i]);
        }
        else
        {
            painter.drawLine(startX+nodeWidth/2+80*childTimes*(indentFactor+1), startY+nodeHeight+(l*80),
                                 startX+nodeWidth/2+80*childTimes*(indentFactor+1), startY+nodeHeight+connectLength +(l*80));
            painter.drawRect(startX+(nodeWidth+indentLength)*(i)*indentFactor/childrens.count()+80*childTimes*(indentFactor+1),startY+(nodeHeight+connectLength)*(l+1),
                             nodeWidth,nodeHeight);
            painter.drawText(QRect(startX+(nodeWidth+indentLength)*(i)*indentFactor/childrens.count()+80*childTimes*(indentFactor+1),startY+(nodeHeight+connectLength)*(l+1),
                                   nodeWidth,nodeHeight),Qt::AlignCenter,childrens[i]);
        }
    }
    //Draw line and connect lines
    for(int i = 0; i < childrens.count() - 1; i++)
    {
        if(l == 0)
        {
        painter.drawLine(startX+nodeWidth/2+1+80*(indentFactor+1)*childTimes, startY+nodeHeight+connectLength/2+80*l,
                         startX+nodeWidth/2+(80*indentFactor)*(i+1)+80*(indentFactor+1)*childTimes, startY+nodeHeight+connectLength/2+80*l);
        painter.drawLine(startX+nodeWidth/2+(80*indentFactor)*(i+1)+80*(indentFactor+1)*childTimes, startY+nodeHeight+connectLength/2+80*l,
                         startX+nodeWidth/2+(80*indentFactor)*(i+1)+80*(indentFactor+1)*childTimes, startY+nodeHeight+connectLength+80*l);
        }
        else
        {
            painter.drawLine(startX+nodeWidth/2+1+80*(indentFactor+1)*childTimes, startY+nodeHeight+connectLength/2+80*l,
                             startX+nodeWidth/2+(80*indentFactor/childrens.count())*(i+1)+80*(indentFactor+1)*childTimes, startY+nodeHeight+connectLength/2+80*l);
            painter.drawLine(startX+nodeWidth/2+(80*indentFactor/childrens.count())*(i+1)+80*(indentFactor+1)*childTimes, startY+nodeHeight+connectLength/2+80*l,
                             startX+nodeWidth/2+(80*indentFactor/childrens.count())*(i+1)+80*(indentFactor+1)*childTimes, startY+nodeHeight+connectLength+80*l);
        }

    }

        prevChildsCount--;
        childTimes++;
        }
        prevChilds = childrens;
        prevChildsCount = childrens.count();
    }
    }
}





