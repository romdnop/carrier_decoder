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
    bool setRecordPosition(int pos, int length);
    int getRecordPointer();
    void setRecordLength(int);
    int getRecordLength();

    void setId(int id);
    int getId();
    QDate getDate();
    void printRaw();
    void updateType();
    recordTypeEnum getRecordType();
    void printTempRec();
    QString getTypeStr();
    void setType(char type);
    recordTypeEnum getType();
    void setDate(QDate date);
    void setSetpoint(float setpoint);
    float getSetpoint();
    void setRawData(QByteArray data);
    QByteArray getRawData();


};

#endif // RECORD_H
