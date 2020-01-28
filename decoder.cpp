#include "decoder.h"
#include "record.h"
#include "event.h"

Decoder::Decoder()
{

}


Decoder::Decoder(QFile *inputFile)
{
    addFile(inputFile);
    cmdDb.addDatabase("QSQLITE");
}

Decoder::~Decoder()
{
    if(file->isOpen())
    {
        file->close();
    }

    if(logFile->isOpen())
    {
        logFile->close();
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
        softwareVersion = static_cast <int> (data.at(controllerVersionOffset+1));
        softwareVersion <<= 8;
        softwareVersion |= static_cast <int> (data.at(controllerVersionOffset));

        qDebug() << "Container id: " << data.mid(containerIdOffset,containerIdLen);
        qDebug() << "Controller id: " << data.mid(controllerIdOffset,controllerIdLen);
        qDebug() << "Software version: " << softwareVersion;
        qDebug() << "Interrogation date: " << getDate(data.mid(interrogationDateOffset,3).data()).toString("dd.MM.yyyy");

        //QTextStream ts(this->log);
        log << "Container id: " << data.mid(containerIdOffset,containerIdLen) << endl;
        log << "Controller id: " << data.mid(controllerIdOffset,controllerIdLen) << endl;
        log << "Software version: " << softwareVersion << endl;
        log << "Interrogation date: " << getDate(data.mid(interrogationDateOffset,3).data()).toString("dd.MM.yyyy") << endl;


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
    //int infoRecords = 0;


    log << "File size: "<< data.size() << " bytes.\n";
    qDebug() << "File size: "<< data.size() << " bytes.";

    int pos = 0;

    Record record;
    while((pos < data.size()) && (pos != -1))
    {
        //qDebug() << pos;
        QByteArray dataChunk = data.mid(pos, data.size());
        record = findNextRecord(static_cast<QByteArray *>(&data),static_cast <int *>(&pos));

        record.setId(recordsCount);
        record.printRaw();
        printRecord(record);

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
                parseRecord(record);
                break;
            case Record::INFO_REC:
                headRecords++;
                break;
            default:
                break;
        }

        recordsCount++;
    }

    //print out ON and OFF events
    qDebug() << "\n------------------------- TURNED ON/OFF AT --------------------------------\n";
    log << "\n------------------------- TURNED ON/OFF AT --------------------------------\n";
    for (auto const& pair: runHoursTable)
    {
        //std::cout << "{" << pair.first << ": " << pair.second << "}\n";
        QString str;
        str = "unknown";
        if((pair.second >= 0x1E) & (pair.second <= 0x1F))
        {
            str = "OFF";
        }
        else
        {
            str = "ON";
        }
        qDebug() << "{" << pair.first.toString("dd.MM.yyyy hh:mm") << ": " << str << "}";
        log << "{" << pair.first.toString("dd.MM.yyyy hh:mm") << ": " << str << "}\n";
    }
    qDebug() << "Last event logged on: " << lastEventDateTime.toString("dd.MM.yyyy hh:mm") << endl;
    log << "Last event logged on: " << lastEventDateTime.toString("dd.MM.yyyy hh:mm") << endl;

    qDebug() << "Found "<<recordsCount<<" records.";
    log << "\n\nFound "<<recordsCount<<" records." << endl;
    qDebug() << "Container records: "<<containerRecord;
    log << "Container records: "<<containerRecord << endl;
    qDebug() << "Callibration records: "<<calRecords;
    log << "Callibration records: "<<calRecords << endl;
    qDebug() << "Head records: "<<headRecords;
    log << "Head records: "<<headRecords << endl;
    qDebug() << "Events records: "<<eventRecordsCount;
    log << "Events records: "<<eventRecordsCount << endl;
    qDebug() << "Temp records: "<<tempRecordsCount;
    log << "Temp records: "<<tempRecordsCount << endl;

    return false;
}


void Decoder::printRecord(Record rec)
{
    log << "\nId:"<< rec.getId() <<" Pos:" << QString("%1").arg(rec.getRecordPointer(),0,16) << " Length:" << QString("%1").arg(rec.getRecordLength(),0,16) << " Type: " << rec.getTypeStr() << " Date: " << (rec.getDate().isValid() ? rec.getDate().toString("dd.MM.yyyy"):"Not present") << endl;
    if(rec.getRecordType() == Record::TEMP_REC)
    {
        log << "Date: " << (rec.getDate().isValid() ? rec.getDate().toString("dd.MM.yyyy"):"Not present");
        log << " Setpoint: " << QString("%1%2").arg(rec.getSetpoint() > 0 ? '+' : ' ').arg(rec.getSetpoint(),2,'f',2,'0');
    }
    log << "\n" << rec.getRawData().toHex().toUpper() << "\n";
}

Record Decoder::findNextRecord(QByteArray *data, int *pos)
{
    Record record;
    QDate date;
    int currentRecordLength = 0;

    *pos = data->indexOf(packageStart,*pos);
    if(*pos == -1)
    {
        return record;
    }

    currentRecordLength = getIntLSB(data->mid(*pos+packageStartLength+packageTypeLength, 2));
    //currentRecordLength = ((static_cast<unsigned int>(data->at(*pos+packageStartLength+packageTypeLength+1)) & 0xFF) << 8);
    //currentRecordLength |= (static_cast<unsigned int>(data->at(*pos+packageStartLength+packageTypeLength)) & 0xFF);
   // currentRecordLength += 12;
    QByteArray chunk = *data;
    //find raw records data
    chunk = chunk.mid(*pos, currentRecordLength+13); //data before length bytes
    //position
    record.setRecordPosition(*pos, currentRecordLength+13);
    //raw data
    record.setRawData(chunk);
    //set record type
    record.setType(chunk.at(5));
    //find date
    if(record.getType() == Record::TEMP_REC){
        int year = getInt(chunk.mid(11,2));
        year = ((year << 8) | (year >> 8)) & 0xFFFF;
        int month = static_cast <int> (chunk.at(10));
        int day = static_cast <int> (chunk.at(9));
        date.setDate(year,month,day);
        currentDateTime.setDate(date);
        lastEventDateTime.setDate(date);
        //date
        record.setDate(date);
        //setpoint
        float setpoint = getFloatLSB(chunk.mid(16,2));
        record.setSetpoint(setpoint);
    }
    //record.setRecord(chunk.mid(*pos, currentRecordLength), *pos, currentRecordLength);

    *pos+=(currentRecordLength+5+2+2+4);

    return record;
}


bool Decoder::findNextLogTemp(QByteArray *data, int *pos)
{
    *pos = data->indexOf(0x64,*pos);
    if(*pos == -1)
    {
        return false;
    }
    *pos += 26;
    return true;
}



void Decoder::parseRecord(Record record)
{
    QByteArray data = record.getRawData();
    int totalSize = getIntLSB(data.mid(7,2));
    int tempBlockSize = getIntLSB(data.mid(22,2));
    int eventBlockSize = getIntLSB(data.mid(20,2));
    qDebug() << "Total data size: " << totalSize << "Temperature logs size: " << tempBlockSize << "Events block size: " << eventBlockSize << "\n";
    log<< "\nTotal data size: " << totalSize << " Temperature logs size: " << tempBlockSize << " Events block size: " << eventBlockSize << "\n";
    int pos = 0;
    if(tempBlockSize > 0){

       QByteArray temp = data.mid(24, tempBlockSize);

       qDebug() << "Temp log block:\n\n" << temp.toHex().toUpper() << "\n";
       log << "\nTemp log block:\n" << temp.toHex().toUpper() << endl;
       //findNextLogTemp(&temp,pos);
       TempRecord record;
       do{
           record = parseTempBlock(temp.mid(pos, 26));
           //qDebug() << "At: " << pos;
           record.toStr();
       }while(findNextLogTemp(static_cast<QByteArray *>(&temp), static_cast <int *>(&pos)) && (pos < temp.size()) && (pos != -1));
    }

    if(eventBlockSize > 0){
       QByteArray events = data.mid(24+tempBlockSize, eventBlockSize);

       qDebug() << "\nEvents block:\n\n" << events.toHex().toUpper() << "\n";
       log << "\nEvents block:\n" << events.toHex().toUpper() << endl;
       parseEventBlock(events);
    }
}


TempRecord Decoder::parseTempBlock(QByteArray data)
{
    QTime time;
    int hours = getInt(data.mid(1, 1));
    int minutes = getInt(data.mid(2, 1));
    if(hours == 24)
    {
        hours = 23;
        minutes = 59;
    }

    time.setHMS(hours, minutes,0);
    currentDateTime.setTime(time);
    float supAir = getFloatLSB(data.mid(6, 2));
    //float supAir = float(getIntLSB(data.mid(6, 2)))/100;
    float retAir = getFloatLSB(data.mid(9, 2));
    float rh = getFloatLSB(data.mid(24, 2));
    TempRecord record(time, supAir, retAir, rh);
    return record;
}


void Decoder::parseEventBlock(QByteArray data)
{
    //Event event();
    //return event;
    int pos = 0;
    int length = 0;
    length = data.at(pos+1);
    int dataSize = data.size();
    qDebug() << "Data size: " << dataSize;
    while(dataSize > (pos+length))
    {
        char cmd = data.at(pos);
        length = int(data.at(pos+1)) & 0xFF;
        QTime time;
        time.setHMS(int(data.at(pos+2)), int(data.at(pos+3)), 0);
        currentDateTime.setTime(time);
        lastEventDateTime.setTime(time);

        /*
        0x1E    //PD_BP
        0x1F    //PD_BNP
        0x20    //PU_BP
        0x21    //PU_BNP
        */
        if((cmd >= 0x1E) && (cmd <= 0x21))
        {
            runHoursTable.insert(std::pair<QDateTime,char>(currentDateTime,cmd));
        }

        if(cmd == '\xFF')
        {
            break;
        }
        qDebug() << "At: " << pos << "Time: " << time.toString("hh:mm") << "Cmd: " << QString("%1").arg(cmd, 2, 16, QLatin1Char( '0' )).toUpper() << "Length: " << length;
        log << "At: " << pos << "\tTime: " << time.toString("hh:mm") << "\tCmd: " << QString("%1").arg(cmd, 2, 16, QLatin1Char( '0' )).toUpper() << "\tLength: " << length << endl;

        pos += length;
        pos += 1;
    }
    //Event event;
    //do{
        //record = parseTempBlock(temp.mid(pos, 26));
        //qDebug() << "At: " << pos;
        //event.setCode(data.at(pos));
        //event.toStr();
    //}while(findNextEvent(static_cast<QByteArray *>(&data), static_cast <int *>(&pos)) && (pos < data.size()) && (pos != -1));
}

bool Decoder::findNextEvent(QByteArray *data, int *pos)
{
    int length = data->at(*pos+1);
    if((*pos+length) >= data->size())
    {
        return false;
    }
   // *pos += data[*pos+1];

    //int newPos = int(data->at(*pos+1)) & 0xFF;

    *pos += (length + 1);
    if((int(data->at(*pos))&0xFF) != 0xFF)
    {
        return true;
    }
    return false;
}



int Decoder::getInt(QByteArray rawData)
{
    bool ok;
    return rawData.toHex().toInt(&ok,16);
}


int Decoder::getIntLSB(QByteArray rawData)
{
    int temp =  getInt(rawData);
    temp = ((temp << 8) | (temp >> 8)) & 0xFFFF;
    return temp;
}



float Decoder::getFloatLSB(QByteArray rawData)
{
    float value = 0;

    int temp =  getIntLSB(rawData);
    if(temp & 0xF000){
        temp ^= 0xFFFF;
        temp += 1;
        temp *= -1;
    }
    value = float(temp)/100;
    return value;
}

bool Decoder::initLog(QFile *file)
{
    this->logFile = file;
    //this->logFile->rename(container);
    this->logFile->open(QFileDevice::WriteOnly | QFile::Text);
    this->log.setDevice(logFile);
    return true;
}


bool Decoder::initDB(QString path)
{
    cmdDb.setDatabaseName("C:/Users/Dima/Desktop/decoder/v1567.sqlite3");
    if(cmdDb.open())
    {
        qDebug() << "File opened!";
        return true;
    }
    qDebug() << "Could not open database file!";
    return false;
}


QString Decoder::findInDB(int cmd)
{
    if(cmdDb.open())
    {
        QSqlQuery query;
        query.prepare("SELECT desc FROM commands WHERE id=':cmd'");
        query.bindValue(":cmd", cmd);
        if(query.exec())
        {
            qDebug() << query.next();
        }
    }
    return QString("unknown");
}


QDateTime Decoder::getLastEventDateTime()
{
    return lastEventDateTime;
}


//bool Decoder::addLogFile(QFile *file)
//{
//    this->logFile = file;

//    this->log->open(QFileDevice::WriteOnly);
//    return true;
//}

/*
bool Decoder::logToFile(QString str)
{
    if(this->log->exists())
    {
        QTextStream stream(this->log);
        stream << str;
        return true;
    }
    return false;
}

*/







