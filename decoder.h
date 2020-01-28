#ifndef DECODER_H
#define DECODER_H

#include <QFile>
#include <QDebug>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QByteArrayMatcher>
#include "record.h"
#include "temp_record.h"
#include "event.h"
#include <QtSql>


class Decoder
{
    //Q_OBJECT
private:

    QFile *file;
    QFile *logFile;
    QTextStream log;
    QByteArray data;
    QSqlDatabase cmdDb;

    QList <Record> recordsList;
    QList <Event> eventsList;

    std::map<QDateTime, char> runHoursTable;

    QDateTime lastEventDateTime;

    const static int containerIdOffset = 0x2EA;
    const static int containerIdLen = 11;
    int controllerVersionOffset = 0x20;
    const static int controllerVersionLen = 2;

    const static int controllerIdOffset = 0x2f5;
    const static int controllerIdLen = 8;


    const static int interrogationDateOffset = 0x82;
    bool checkFile(void);
    const int packageStartLength = 5;
    const QByteArray packageStart = QByteArray("\x51\x51\x51\x5A\x5A");
    const int packageTypeLength = 2;
    QDateTime currentDateTime;

    //char recordStart[] = {0x51,0x51,0x51,0x5A,0x5A};
public:
    Decoder();
    Decoder(QFile *);
    ~Decoder();
    bool addFile(QFile *);
    bool getContainerId();
    bool getContainerInfo();
    float getTemp(QByteArray *);
    QDate getDate(char *); //use template!
    QDate getExtDate(QByteArray *);
    bool runDecoder();
    bool printLog();
    Record findNextRecord(QByteArray *data, int *pos);
    bool findNextLogTemp(QByteArray *data, int *pos);
    TempRecord parseTempBlock(QByteArray data);
    int getInt(QByteArray rawData);
    int getIntLSB(QByteArray rawData);
    float getFloat(QByteArray rawData);
    float getFloatLSB(QByteArray rawData);
    void parseRecord(Record record);
    void parseEventBlock(QByteArray data);
    bool findNextEvent(QByteArray *data, int *pos);
    //bool addLogFile(QFile *file);
    //bool logToFile(QString str);
    bool initLog(QFile *file);
    void printRecord(Record rec);
    QString findInDB(int cmd);
    bool initDB(QString path);
    QDateTime getLastEventDateTime();
};

#endif // DECODER_H
