#include "decoder.h"
#include "record.h"

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
    return QDate(static_cast <int>(array[2])+2000, static_cast <int>(array[1]),static_cast <int>(array[0]));
}




bool Decoder::printLog()
{
    int recordsCount = 0;
    int eventRecordsCount = 0;
    int containerRecord = 0;
    int tempRecordsCount = 0;
    int calRecords = 0;
    int headRecords = 0;
    int infoRecords = 0;
    //int j=0;
    qDebug() << "File size: "<< data.size() << " bytes.";
    //QByteArrayMatcher matcher(packageStart);
    int pos = 0;

    Record record;
    while((pos < data.size()) && (pos != -1))
    {
        //qDebug() << pos;
        QByteArray dataChunk = data.mid(pos, data.size());
        record = findNextRecord(static_cast<QByteArray *>(&data),static_cast <int *>(&pos));
        record.setId(recordsCount);
        record.printRaw();
        switch(record.getRecordType())
        {
            case Record::CAL_REC:
                calRecords++;
                break;
            case Record::CONT_REC:
                containerRecord++;
                break;
            case Record::EVENT_REC:
                eventRecordsCount++;
                break;
            case Record::TEMP_REC:
                tempRecordsCount++;
                break;
            case Record::INFO_REC:
                headRecords++;
                break;
            default:
                break;
        }
        recordsCount++;
    }


    //qDebug() << data.indexOf(packageStart);


          qDebug() << "Found "<<recordsCount<<" records.";
          qDebug() << "Container records: "<<containerRecord;
          qDebug() << "Callibration records: "<<calRecords;
          qDebug() << "Head records: "<<headRecords;
          qDebug() << "Events records: "<<eventRecordsCount;
          qDebug() << "Temp records: "<<tempRecordsCount;


    //QDataStream dataStream(file);
    //QString log = dataStream.readAll();
    //qDebug() << log.size();

    return false;
}


Record Decoder::findNextRecord(QByteArray *data, int *pos)
{
    Record record;
    int currentRecordLength = 0;
    //qDebug() << *data;

    *pos = data->indexOf(packageStart,*pos);
    if(*pos == -1)
    {
        //*pos += packageStartLength;
        return record;
    }
    //data->at(0);

    //currentRecordLength = static_cast <unsigned int> (data[*pos+packageStartLength+1]);
    //currentRecordLength |= static_cast <unsigned int> (data[*pos+packageStartLength+2]);

    currentRecordLength = ((static_cast<unsigned int>(data->at(*pos+packageStartLength+packageTypeLength+1)) & 0xFF) << 8);
    currentRecordLength |= (static_cast<unsigned int>(data->at(*pos+packageStartLength+packageTypeLength)) & 0xFF);
    //currentRecordLength = *data[(*pos+packageStartLength+1)];
    QByteArray chunk = *data;
    record.setRecord(chunk.mid(*pos, currentRecordLength), *pos, currentRecordLength);
    *pos+=(currentRecordLength+5+2+2+4);
    //qDebug() << "Pos:" << QString("%1").arg(*pos,0,16) << "Length:" << QString("%1").arg(currentRecordLength,0,16);
    //record.setRawData(data, (currentRecordLength+packageStartLength+4+2));

    return record;
}






