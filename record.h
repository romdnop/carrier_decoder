#ifndef RECORD_H
#define RECORD_H

#include <QDate>
#include <QTime>
#include <QDebug>


class Record
{
public:
    enum recordTypeEnum:char{EVENT_REC = '\x0E', TEMP_REC = '\x02', CONT_REC = '\x0C', CAL_REC = '\x00', INFO_REC = '\x01', EMPTY_REC='\xFF'};
private:

    recordTypeEnum recordType = EMPTY_REC;
    QDate date;
    QByteArray rawData;
    int recordPointer;
    int recordLength;
    int recordId;
    float setpoint;
public:
    Record();
    Record(QByteArray *, int);
    bool setRawData(QByteArray *, int);
    bool setRecord(QByteArray rawData, int pos, int length);
    void setId(int id);
    void printRaw();
    void updateType();
    recordTypeEnum getRecordType();
    void printTempRec();
    QString getTypeStr();
    int getInt(QByteArray rawData);
    int getIntLSB(QByteArray rawData);
    float getFloatLSB(QByteArray rawData);
};

#endif // RECORD_H
