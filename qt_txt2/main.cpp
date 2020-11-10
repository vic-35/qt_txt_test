#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <qqmlengine.h>
#include <qqmlcontext.h>
#include <qqml.h>
#include <QtQuick/qquickitem.h>
#include <QtQuick/qquickview.h>


//#include <QTimer>
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <sstream>
//#include <codecvt>
//#include <QFile>
//#include <QTextStream>

#include "myclass.h"




int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    MyClass myClass;
    engine.rootContext()->setContextProperty("myClass", &myClass);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
