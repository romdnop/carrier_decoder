#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include "decoder.h"

const QString fileName = "TEMU9483543.dcx";
QString dbPath = "C:\\Users\\Dima\\Desktop\\decoder\\v1567.sqlite3";

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //qDebug() << "Test";
    //QFile file("C:\\Users\\Dima\\Desktop\\develop app for interrogator\\reports_print\\19_09\\console_app\\build-decoder-Desktop-Debug\\debug\\TEMU9483543.dcx");


    QFile file("C:\\Users\\Dima\\Desktop\\decoder\\carrier_decoder\\CRLU1396469.dcx");

    QFile log("C:\\Users\\Dima\\Desktop\\decoder\\carrier_decoder\\output_log.txt");

    //QFile file("C:\\Users\\Dima\\Desktop\\decoder\\carrier_decoder\\TRIU8060312.dcx");


    Decoder *decoder = new Decoder(&file);
    //decoder->addLogFile(&log);
    decoder->initLog(&log);


    //decoder->getContainerId();
    decoder->getContainerInfo();
    decoder->printLog();


    decoder->initDB(dbPath);
    decoder->findInDB(31);

    //decoder->file->close();
    //decoder->log->close();
    return a.exec();
}
