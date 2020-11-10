#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTimer>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <codecvt>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model.setColumnCount(4);
    model.setHorizontalHeaderLabels(QStringList{tr("Номер"), tr("ID"), tr("ФИО"),tr("Телефон")});

    //ui->tableView1->horizontalHeader()->setStretchLastSection(true);
    ui->tableView1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView1->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //QTimer::singleShot(0, this,SLOT(on_LoadFile_slot())); // показ диалога после загрузки и показа формы сигнал в слот
    QTimer::singleShot(0, this,[=]{on_LoadFile(false);}); // показ диалога после загрузки и показа формы = лямбда без обьявления слота



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_LoadFile_slot()
{
    on_LoadFile(true);
}

void MainWindow::on_LoadFile(bool use_std)
{
    QString fileName;
    try { // Bug windows64

         fileName = QFileDialog::getOpenFileName(this,
             tr("Open Text File"), "", tr("Text Files (*.txt);;All Files  (*.*)"));

    }  catch (...) {

    }

    model.clear();


    if(fileName.length() >0)
    {
        try
        {
            if(use_std) // стандартные потоки std
            {
                std::wstring line;
                #ifdef linux
                std::wifstream in(fileName.toStdString());
                #else
                std::wifstream in(fileName.toStdWString());
                #endif
                in.imbue(std::locale(std::locale(""), new std::codecvt_utf8<wchar_t>)); // тест , не забываем про кодировку

                  if (in.is_open())
                    {
                        while (std::getline(in,line))
                        {
                            std::wstring param;
                            std::wistringstream strBuf(line);
                            QList<QStandardItem*> q_list;

                            while (std::getline(strBuf,param,L'|'))
                            {
                                q_list.append(new QStandardItem(QString::fromStdWString(param)));
                            }
                            model.appendRow( q_list);
                        }
                        in.close();
                        ui->tableView1->setModel(&model);
                        return;
                    }
            }
            else // QFile
            {
                QFile file(fileName);
                if(file.open(QIODevice::ReadOnly|QIODevice::Text))
                {
                    QTextStream in(&file);
                    in.setCodec("UTF-8");
                    while(!in.atEnd())
                    {
                        QString line= in.readLine();
                        QStringList line_param= line.split("|");
                        QList<QStandardItem*> q_list;
                        foreach(QString param ,line_param)
                        {
                            q_list.append(new QStandardItem(param));
                        }
                        model.appendRow( q_list);
                    }
                    file.close();
                    ui->tableView1->setModel(&model);
                    return;
                }
            }
        }
        catch (...) {
            // тест , расширение на будущее
        }
    }

    QApplication::closeAllWindows();

}

