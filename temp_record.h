#ifndef TEMP_RECORD_H
#define TEMP_RECORD_H
#include <QDateTime>

class TempRecord
{
private:
    QDateTime timeStamp;
    int recordId;
    float supAir;
    float retAir;
    float rh;
    float ambientTemp;
public:
    TempRecord();
    TempRecord(QDateTime*, float supAir, float retAir, float rh);
    TempRecord(QDateTime*, float supAir, float retAir, float rh, float ambientTemp);
    void printRecord();
};

#endif // TEMP_RECORD_H
