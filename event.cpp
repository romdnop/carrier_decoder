#include "event.h"

Event::Event()
{
}


Event::Event(QTime time, int code, int length, QString desc)
{
    this->time = time;
    this->code = code;
    this->length = length;
    this->desc = desc;
}


void Event::toStr(void)
{
    qDebug() << this->time.toString("hh:mm") << " " << code << "\n";
}


int Event::getCode()
{
    return this->code;
}


void Event::setCode(int code)
{
    this->code = code;
}

