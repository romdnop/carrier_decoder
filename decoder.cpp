#include "decoder.h"

Decoder::Decoder()
{

}


Decoder::Decoder(QFile *newFile)
{
    addFile(newFile);
}

Decoder::~Decoder()
{
    if(file->isOpen())
    {
        file->close();
    }
    qDebug() << "Object has been deleted";
    delete this;
}


bool Decoder::checkFile()
{
    if(file->exists() && file->isOpen())
    {
        return true;
    }
    return false;
}


bool Decoder::addFile(QFile *newFile)
{
    file = newFile;

    if(file->exists())
    {
        qDebug()<<"File exists";
        if(!file->isOpen())
        {
            qDebug() <<  "File opened";
            if(file->open(QFileDevice::ReadOnly))
            {
                qDebug() << "File Added";
                data = file->readAll();
                return true;
            }
        }
    }
    return false;
}


bool Decoder::getContainerId(void)
{
    if(file->isOpen())
    {
        char arr[containerIdLen] = {0};
        //qDebug() << "CONTAINER ID";
        //data.reserve(ContainerIdLen);
        file->seek(containerIdOffset);
        file->read(static_cast <char *> (arr), static_cast <qint64> (containerIdLen));
        qDebug() << "Container id: " <<  QByteArray(static_cast <char *> (arr),containerIdLen);
        return true;
    }
    return false;
}



bool Decoder::getContainerInfo()
{
    if(checkFile())
    {
        unsigned int softwareVersion = 0;

        //softwareVersion = data.mid(controllerVersionOffset+1,1).toHex().toInt(nullptr,16);
        softwareVersion = static_cast <int> (data.at(controllerVersionOffset+1));
        softwareVersion <<= 8;
        //softwareVersion |= data.mid(controllerVersionOffset,1).toHex().toInt(nullptr,16);
        softwareVersion |= static_cast <int> (data.at(controllerVersionOffset));

        //qDebug() << softwareVersion;
        //qDebug() << QString().number(softwareVersion);


        qDebug() << "Container id: " << data.mid(containerIdOffset,containerIdLen);
        qDebug() << "Controller id: " << data.mid(controllerIdOffset,controllerIdLen);
        qDebug() << "Software version: " << softwareVersion;
        qDebug() << "Interrogation date: " << getDate(data.mid(interrogationDateOffset,3).data()).toString("dd.MM.yyyy");
    }
    return false;
}

//two bytes
float Decoder::getTemp(QByteArray *array)
{
    int temp = 0;
    temp = static_cast <int> (array->at(0));
    temp <<= 8;
    temp |= static_cast <int> (array->at(1));
    return float(temp/100);
}


QDate Decoder::getDate(char *array)
{
    QDate *date = new QDate(static_cast <int>(array[2])+2000, static_cast <int>(array[1]),static_cast <int>(array[0]));
    //date.setDate();
    return *date;
}



