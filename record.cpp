#include "record.h"

Record::Record()
{
}


Record::Record(QByteArray *rawData, int length)
{
    setRawData(rawData,length);
}


bool Record::setRawData(QByteArray *rawData, int length)
{
    recordLength = length;
    this->rawData = rawData->mid(0, recordLength);
    updateType();

    return true;
}


void Record::updateType(void)
{
    switch(rawData[5])
    {
        case '\x0E':
            recordType = EVENT_REC;
            break;
        case '\x0C':
            recordType = CONT_REC;
            break;
        case '\x01':
            recordType = TEMP_REC;
            break;
        case '\x00':
            recordType = CAL_REC;
            break;
        case '\x02':
            recordType = INFO_REC;
            break;
        default:
            recordType = EMPTY_REC;
            break;
    }
}

bool Record::setRecord(QByteArray *rawData, int pos, int length)
{
    recordLength = length;
    recordPointer = pos;
    this->rawData = rawData->mid(0, recordLength);
    updateType();
    return true;
}


void Record::printRaw()
{
    //qDebug() << rawData;
    qDebug() << "Pos:" << QString("%1").arg(recordPointer,0,16) << "Length:" << QString("%1").arg(recordLength,0,16) << "Type: " << recordType;
}
