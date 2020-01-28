#include "temp_record.h"


TempRecord::TempRecord()
{

}



TempRecord::TempRecord(QTime time, float supAir, float retAir, float rh)
{
    this->timeStamp = time;
    this->supAir = supAir;
    this->retAir = retAir;
    this->rh = rh;
}


void TempRecord::setSupAir(float supAir)
{
    this->supAir = supAir;
}


float TempRecord::getSupAir()
{
    return this->supAir;
}



void TempRecord::setRetAir(float retAir)
{
    this->retAir = retAir;
}


float TempRecord::getRetAir()
{
    return this->retAir;
}



void TempRecord::setRh(float rh)
{
    this->rh = rh;
}


float TempRecord::getRh()
{
    return this->rh;
}

void TempRecord::toStr()
{
    qDebug() << "Time: " << timeStamp.toString("hh:mm")
             << " SupAir: " << QString("%1%2").arg(supAir > 0 ? '+' : ' ').arg(supAir,2,'f',2,'0')
             << " RetAir: " << QString("%1%2").arg(retAir > 0 ? '+' : ' ').arg(retAir,2,'f',2,'0')
             << " RH: " << QString("%1%2").arg(rh > 0 ? '+' : ' ').arg(rh,2,'f',2,'0');
}
