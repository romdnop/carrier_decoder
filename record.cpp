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
    //qDebug() << QString("%1").arg(rawData[recordPointer + 5],0,16);

    switch(rawData[5])
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

bool Record::setRecord(QByteArray rawData, int pos, int length)
{
    recordLength = length;
    recordPointer = pos;
    this->rawData = rawData;
    //qDebug() << rawData.toHex() " " <<pos;
    if(static_cast <int>(rawData.at(5)) == 2){
        //bool ok;
        //date
        int year = getInt(rawData.mid(11,2));
        year = ((year << 8) | (year >> 8)) & 0xFFFF;
        int month = static_cast <int> (rawData.at(10));
        int day = static_cast <int> (rawData.at(9));
        date.setDate(year,month,day);
        //setpoint
        this->setpoint = getFloatLSB(rawData.mid(16,2));

        /*
        int temp =  rawData.mid(16, 2).toHex().toInt(&ok,16);
        temp = ((temp << 8) | (temp >> 8)) & 0xFFFF;
        if(temp & 0xF000){
            temp ^= 0xFFFF;
            temp += 1;
            temp *= -1;
        }
        this->setpoint = float(temp/100);
        */


        //qDebug() << this->setpoint;
        //setpoint = ((setpoint << 8) | (setpoint >> 8)) & 0xFFFF;
        //qDebug() << "Date: " << date.toString("dd.MM.yyyy");
    }

    updateType();
    return true;
}

void Record::setId(int id)
{
    recordId = id;
}


void Record::printRaw()
{
    //qDebug() << rawData;
    qDebug() << "\nId: "<< recordId <<"Pos:" << QString("%1").arg(recordPointer,0,16) << "Length:" << QString("%1").arg(recordLength,0,16) << "Type: " << getTypeStr() << "Date: " << (this->date.isValid() ? this->date.toString("dd.MM.yyyy"):"Not present");
    //qDebug() << int(this->setpoint);
    //qDebug() << "Setpoint: " << ((this->recordType == TEMP_REC) ? QString("%1%2").arg(setpoint > 0 ? '+' : ' ').arg(setpoint,2,'f',2,'0')  : "Not present");
   // (this->recordType == TEMP_REC)?(qDebug() << "Setpoint: " << this->setpoint):"";

    if(this->recordType == TEMP_REC)
    {
        printTempRec();
    }
    qDebug() << rawData.toHex().toUpper() << "\n";
}


void Record::printTempRec()
{
    qDebug() << "Date: " << (this->date.isValid() ? this->date.toString("dd.MM.yyyy"):"Not present");
    qDebug() << "Setpoint: " << QString("%1%2").arg(setpoint > 0 ? '+' : ' ').arg(setpoint,2,'f',2,'0');
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

int Record::getInt(QByteArray rawData)
{
    bool ok;
    return rawData.toHex().toInt(&ok,16);
}

int Record::getIntLSB(QByteArray rawData)
{
    int temp =  getInt(rawData);
    temp = ((temp << 8) | (temp >> 8)) & 0xFFFF;
    return temp;
}


float Record::getFloatLSB(QByteArray rawData)
{
    float value = 0;

    int temp =  getIntLSB(rawData);
    if(temp & 0xF000){
        temp ^= 0xFFFF;
        temp += 1;
        temp *= -1;
    }
    value = float(temp/100);
    return value;
}


Record::recordTypeEnum Record::getRecordType()
{
    return this->recordType;
}


QVector <QTime> Record::findOnTimes(QByteArray rawRecord)
{
    QVector <QTime> onTimes;
    QByteArray events;
    //20, 2 event log block size
    //22, 2 temp log record size
    int ptr = 0x18;
    int eventsBlockSize = getIntLSB(rawRecord.mid(20,2));
    int tempBlockSize = getIntLSB(rawRecord.mid(22,2));
    //starts from 24
    if(tempBlockSize > 0)
    {
        ptr += tempBlockSize;
    }
//    for(int i=0; i<tempBlockSize;i++){

//    }

}
