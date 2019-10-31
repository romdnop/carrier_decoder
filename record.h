#ifndef RECORD_H
#define RECORD_H

#include <QDate>
#include <QDebug>

class Record
{
private:
    enum recordTypeEnum:char{EVENT_REC = '\x0E', TEMP_REC = '\x02', CONT_REC = '\x0C', CAL_REC = '\x00', INFO_REC = '\x01', EMPTY_REC='\xFF'};
    recordTypeEnum recordType = EMPTY_REC;
    QDate date;
    QByteArray rawData;
    int recordPointer;
    int recordLength;
public:
    Record();
    Record(QByteArray *, int);
    bool setRawData(QByteArray *, int);
    bool setRecord(QByteArray *rawData, int pos, int length);
    void printRaw();
    void updateType();
};

#endif // RECORD_H
