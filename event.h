#ifndef EVENT_H
#define EVENT_H

#include <QDebug>
#include <QTime>



class Event
{
public:
    enum eventTypeEnum:char{
        OFF = '\x1E',
        OFF_NO_BTR = '\x1F',
        ON = '\x20',
        ON_NO_BTR = '\x21'
    };
private:
    // this is a map, where the keys are integers and the values are strings
    //std::map<int, std::string> stringLookupTable;
    int code;
    int length;
    QString desc;
    QTime time;
    QDateTime dateTime; //debug
    int id;             //debug
public:
    Event();
    Event(QTime, int, int, QString);
    void toStr(void);
    int getCode();
    void setCode(int code);
};

#endif // EVENT_H
