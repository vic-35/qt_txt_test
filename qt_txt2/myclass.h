#ifndef MYCLASS_H
#define MYCLASS_H
#include <qqmlengine.h>
#include <qqmlcontext.h>
#include <qqml.h>
#include <QtQuick/qquickitem.h>
#include <QtQuick/qquickview.h>
#include <QStandardItem>

#include <QTimer>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <codecvt>
#include <QFile>
#include <QTextStream>
#include <QUrl>


class MyClass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QVariant> variantList READ variantList )

public:
    explicit MyClass(QObject *parent = nullptr) : QObject(parent) { }
    QList<QVariant> variantList() const { return m_variantList; }

public slots:
    void loadFile(QString fileName_url)
    {

        QString fileName;

        fileName= QUrl(fileName_url).toLocalFile();

        m_variantList.clear();

        if(fileName.length() >0)
        {
            try
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
                            m_variantList.append(line_param);
                        }
                        file.close();

                        return;
                    }

            }
            catch (...) {
                // тест
            }
        }
    }


private:
    QList<QVariant> m_variantList;
};

#endif // MYCLASS_H
