#include "mainwindow.h"
#include "qworld.h"
#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDebug>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCommandLineOption fileOption(QStringList() << "o" << "open", "Open saved scene from <file>.", "file");
    QCommandLineParser parser;
    parser.addOption(fileOption);
    if(!parser.parse(a.arguments()))
        qDebug() << parser.errorText();
    QString fileName = a.applicationDirPath() + "/" + parser.value(fileOption);
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::ExistingOnly))
    {
        world().load(file);
        file.close();
    }
    MainWindow w;
    w.show(); 

    return a.exec();
}
