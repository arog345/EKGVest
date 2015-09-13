#ifndef SIMPLESTREAMER_H
#define SIMPLESTREAMER_H

#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QThread>
#include <QHostInfo>
#include <QTimer>
#include <QFile>
#include <QTextStream>

#include "bcm2835.h"

extern "C" {
    int bcm2835_init();
    void bcm2835_spi_begin();
    void bcm2835_spi_setBitOrder(uint8_t order);
    void bcm2835_spi_setClockDivider(uint16_t divider);
    void bcm2835_spi_chipSelect(uint8_t cs);
    void bcm2835_spi_setChipSelectPolarity(uint8_t cs, uint8_t active);
    uint8_t bcm2835_spi_transfer(uint8_t value);
    void bcm2835_spi_end();
    int bcm2835_close();
    void bcm2825_gpio_set(uint8_t pin);
    void bcm2835_gpio_clr(uint8_t pin);
}

class SimpleStreamer : public QObject
{
    Q_OBJECT
public:
    SimpleStreamer(QHostAddress *ip, quint16 tcpPort, quint16 udpPort,
                   QObject *parent = 0);

   ~SimpleStreamer();

signals:

private slots:
    void newConnection();
    void broadcastInfo();
    void disconnected();
    void sendTimestep();
    void clientRequest();
    void readData();

private:
    QTcpServer *server;
    QUdpSocket *listener;
    QHostAddress *ip;
    quint16 tcpPort;
    quint16 udpPort;
    QTcpSocket *conn;
    bool quit;

    QFile *inputData;
    QTextStream *inputStream;

    QTimer *test;
};

#endif // SIMPLESTREAMER_H
