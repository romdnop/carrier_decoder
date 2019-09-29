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


    QFile file("C:\\Users\\Dima\\Desktop\\develop app for interrogator\\reports_print\\18_09\\CRLU1395760_1909172325_30d.dcx");
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
//        qDebug() << "Cannot open file";
        //return;

   Decoder *decoder = new Decoder(&file);
   //decoder->getContainerId();
    decoder->getContainerInfo();
    return a.exec();
}
