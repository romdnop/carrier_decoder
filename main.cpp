#include <QCoreApplication>
#include <QFile>
#include <QDebug>
#include "decoder.h"

const QString fileName = "TEMU9483543.dcx";


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //qDebug() << "Test";
    //QFile file("C:\\Users\\Dima\\Desktop\\develop app for interrogator\\reports_print\\19_09\\console_app\\build-decoder-Desktop-Debug\\debug\\TEMU9483543.dcx");


    QFile file("C:\\Users\\Dima\\Desktop\\decoder\\carrier_decoder\\CRLU1396469.dcx");

    Decoder *decoder = new Decoder(&file);
    //decoder->getContainerId();
    decoder->getContainerInfo();
    decoder->printLog();
    return a.exec();
}
