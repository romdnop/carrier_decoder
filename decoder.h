#ifndef DECODER_H
#define DECODER_H

#include <QFile>
#include <QDebug>
#include <QDate>
#include <QTime>

class Decoder
{
    //Q_OBJECT
private:
    QFile *file;
    QByteArray data;
    const static int containerIdOffset = 0x2EA;
    const static int containerIdLen = 11;
    int controllerVersionOffset = 0x20;
    const static int controllerVersionLen = 2;

    const static int controllerIdOffset = 0x2f5;
    const static int controllerIdLen = 8;


    const static int interrogationDateOffset = 0x82;
    bool checkFile(void);
    char recordStart[] = {0x51,0x51,0x51,0x5A,0x5A};
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
};

#endif // DECODER_H
