#ifndef TEMP_RECORD_H
#define TEMP_RECORD_H

#include <QTime>
#include <QDebug>

class TempRecord
{
private:
    QTime timeStamp;
    int recordId;
    float supAir;
    float retAir;
    float usda1;
    float usda2;
    float usda3;
    float cargo;
    float rh;
    float ambientTemp;

public:
    TempRecord();
    TempRecord(QTime, float supAir, float retAir, float rh);
    //TempRecord(QTime time, float supAir, float retAir, float rh)
    //TempRecord(QTime*, float supAir, float retAir, float rh, float ambientTemp);
    void setSupAir(float supAir);
    float getSupAir();
    void setRetAir(float retAir);
    float getRetAir();
    void setRh(float rh);
    float getRh();
    void toStr();
};

#endif // TEMP_RECORD_H
