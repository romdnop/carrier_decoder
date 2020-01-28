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

    return true;
}


//void Record::setType(Record::recordTypeEnum type)
//{
//    this->recordType = type;
//}


//void Record::setRecord(Record::recordTypeEnum type, int ptr, int length)
//{
//    recordLength = length;
//    recordPointer = pos;
//}


void Record::setSetpoint(float setpoint)
{
    this->setpoint = setpoint;
}


void Record::setDate(QDate date)
{
    this->date = date;
}


void Record::setType(char type)
{
    //qDebug() << QString("%1").arg(rawData[recordPointer + 5],0,16);

    switch(type)
    {
        case '\x0E':
            recordType = EVENT_REC;
            break;
        case '\x0C':
            recordType = CONT_REC;
            break;
        case '\x02':
            recordType = TEMP_REC;
            break;
        case '\x00':
            recordType = CAL_REC;
            break;
        case '\x01':
            recordType = INFO_REC;
            break;
        default:
            recordType = EMPTY_REC;
            break;
    }
}


Record::recordTypeEnum Record::getType()
{
    return this->recordType;
}

bool Record::setRecordPosition(int pos, int length)
{
    recordLength = length;
    recordPointer = pos;
    return true;
}

int Record::getRecordPointer()
{
    return recordPointer;
}

int Record::getRecordLength()
{
    return recordLength;
}


void Record::setRecordLength(int len)
{
    recordLength = len;
}

int Record::getId()
{
    return recordId;
}

void Record::setId(int id)
{
    recordId = id;
}


void Record::setRawData(QByteArray data)
{
    this->rawData = data;
}

QByteArray Record::getRawData()
{
    return this->rawData;
}

QDate Record::getDate()
{
    return date;
}

void Record::printRaw()
{

    qDebug() << "\nId: "<< recordId <<"Pos:" << QString("%1").arg(recordPointer,0,16) << "Length:" << QString("%1").arg(recordLength,0,16) << "Type: " << getTypeStr() << "Date: " << (this->date.isValid() ? this->date.toString("dd.MM.yyyy"):"Not present");
    //qDebug() << int(this->setpoint);
    //qDebug() << "Setpoint: " << ((this->recordType == TEMP_REC) ? QString("%1%2").arg(setpoint > 0 ? '+' : ' ').arg(setpoint,2,'f',2,'0')  : "Not present");
   // (this->recordType == TEMP_REC)?(qDebug() << "Setpoint: " << this->setpoint):"";

    if(this->recordType == TEMP_REC)
    {
        printTempRec();
    }
    qDebug() << this->rawData.toHex().toUpper() << "\n";
}


//void Record::printInStream(QTextStream &ts)
//{
//    ts << "\nId: "<< recordId << "Pos:" << QString("%1").arg(recordPointer,0,16) << "Length:" << QString("%1").arg(recordLength,0,16) << "Type: " << getTypeStr() << "Date: " << (this->date.isValid() ? this->date.toString("dd.MM.yyyy"):"Not present");
//}


void Record::printTempRec()
{
    qDebug() << "Date: " << (this->date.isValid() ? this->date.toString("dd.MM.yyyy"):"Not present");
    qDebug() << "Setpoint: " << QString("%1%2").arg(setpoint > 0 ? '+' : ' ').arg(setpoint,2,'f',2,'0');
}

float Record::getSetpoint()
{
    return setpoint;
}

QString Record::getTypeStr()
{
    switch(recordType){
    case '\x0E':
        return QString("Event record");
    case '\x0C':
        return QString("Container record");
    case '\x02':
        return QString("Log record");
    case '\x00':
        return QString("Calibration record");
    case '\x01':
        return QString("Info record");
    default:
        return QString("Unknown record type");
    }
}



Record::recordTypeEnum Record::getRecordType()
{
    return this->recordType;
}


//QVector <QTime> Record::findOnTimes(QByteArray rawRecord)
//{
//    QVector <QTime> onTimes;
//    QByteArray events;
//    //20, 2 event log block size
//    //22, 2 temp log record size
//    int ptr = 0x18;
//    int eventsBlockSize = getIntLSB(rawRecord.mid(20,2));
//    int tempBlockSize = getIntLSB(rawRecord.mid(22,2));
//    //starts from 24
//    if(tempBlockSize > 0)
//    {
//        ptr += tempBlockSize;
//    }
////    for(int i=0; i<tempBlockSize;i++){

////    }

//}
