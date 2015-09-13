#include "simplestreamer.h"
#include <QCoreApplication>

SimpleStreamer::SimpleStreamer(QHostAddress *ip, quint16 tcpPort, quint16 udpPort, QObject *parent) :
    QObject(parent), ip(ip), tcpPort(tcpPort), udpPort(udpPort), quit(false)
{
    server = new QTcpServer(this);

    listener = new QUdpSocket(this);
    listener->bind(udpPort, QUdpSocket::ShareAddress);

    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    connect(listener, SIGNAL(readyRead()), this, SLOT(broadcastInfo()));

    // Server setup
    if (server->listen(*ip, tcpPort))
    {
        qDebug() << "Server has started.";
        qDebug() << "IP Address: " << server->serverAddress();
        qDebug() << "Port #: " << QString::number(server->serverPort());
    }
    else
    {
        qDebug() << "Server could not start." << endl;
    }

    // SPI Setup
    if (bcm2835_init())
    {
        bcm2835_spi_begin();
        bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
        bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
        bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536); // The default
    }
    else
        qDebug() << "SPI could not start.";

    inputData = new QFile(QCoreApplication::applicationDirPath() + "/sample_input.txt");
    inputData->open(QIODevice::ReadOnly | QIODevice::Text);
    inputStream = new QTextStream(inputData);

    test = new QTimer();
    connect(test, SIGNAL(timeout()), this, SLOT(readData()));
    test->start(10);
}

SimpleStreamer::~SimpleStreamer()
{
    bcm2835_spi_end();
    bcm2835_close();
}

void SimpleStreamer::newConnection()
{

    conn = server->nextPendingConnection();

    qDebug() << "Connection received.";

    connect(conn, SIGNAL(disconnected()), conn, SLOT(deleteLater()));
    connect(conn, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(conn, SIGNAL(readyRead()), this, SLOT(clientRequest()));

    // Write the integers to the socket
    quit = false;

    inputStream->seek(0);

    sendTimestep();
}

void SimpleStreamer::sendTimestep()
{
    if (quit)
        return;

    QDataStream out(conn);
    out.setVersion(QDataStream::Qt_4_0);

    out << (qint64) QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();

    // ADC SPI Code
    /*
    unsigned char spiData[3];
    unsigned char chanBits;
    int chan = 0;

    chanBits = 0b10000000 | (chan << 4);

    spiData[0] = 1; // Start bit
    spiData[1] = chanBits;
    spiData[2] = 0;

    for (int i = 0; i < 16; i++)
    {
        if (i < 8)
        {
            bcm2835_gpio_clr(RPI_GPIO_P1_22);
            bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
            bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
        }
        else if (i < 16)
        {
            bcm2835_gpio_clr(RPI_GPIO_P1_22);
            bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
            bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW);
        }
        else
        {
            bcm2835_gpio_set(RPI_GPIO_P1_22);
            bcm2835_spi_chipSelect(BCM2835_SPI_CS2);
            bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS2, LOW);
        }

        for (int j = 0; j < 3; j++)
            spiData[i] = bcm2835_spi_transfer(spiData[i]);


        *outputStream << QString::number((((spiData[1] << 8) | spiData[2]) & 0x03FF));
        *outputStream << ",";
        qDebug() << (((spiData[1] << 8) | spiData[2]) & 0x03FF);
    }
    *outputStream << "\n";
    */

    // MCP3002 Code
    unsigned char spiData[2];

    spiData[0] = 0b11010000; // Start bit
    spiData[1] = 0b00000000;

    bcm2835_gpio_clr(RPI_GPIO_P1_22);
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);

    for (int j = 0; j < 2; j++)
        spiData[j] = bcm2835_spi_transfer(spiData[j]);

    qDebug() << (((spiData[0] << 7) | spiData[1] >> 1) & 0x3ff);

    if (inputStream->atEnd())
        inputStream->seek(0);

    QString line;
    for (int i = 0; i < 250; i++)
        *inputStream >> line;
    for (int i = 0; i < 18; i++)
        out << line.toDouble();
}

void SimpleStreamer::broadcastInfo()
{
    while (listener->hasPendingDatagrams())
    {
        QHostAddress senderAddress;
        quint16 senderPort;
        QByteArray message;
        message.resize(listener->pendingDatagramSize());
        listener->readDatagram(message.data(), message.size(), &senderAddress,
                               &senderPort);

        QUdpSocket *sendConn = new QUdpSocket(this);

        QByteArray reply;
        reply.append(ip->toString() + ":" + QString::number(tcpPort));
        sendConn->writeDatagram(reply.data(), reply.size(), senderAddress, senderPort);

        qDebug() << "IP Address responded to: " << senderAddress.toString() << ":" << senderPort;
        qDebug() << "IP Adress in response: " << reply.data();
    }
}

void SimpleStreamer::clientRequest()
{
    if (conn->bytesAvailable() > 0)
    {
        conn->readAll();
        sendTimestep();
    }
}

void SimpleStreamer::disconnected()
{
    quit = true;
    conn->flush();
    conn->disconnectFromHost();
    conn->close();
    qDebug() << "Connection closed.";

}

void SimpleStreamer::readData()
{
    unsigned char spiData[2];

    spiData[0] = 0b01101000; // Start bit
    spiData[1] = 0b00000000;

    bcm2835_gpio_clr(RPI_GPIO_P1_22);
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);

    for (int j = 0; j < 2; j++)
        spiData[j] = bcm2835_spi_transfer(spiData[j]);

    qDebug() << ((spiData[0] << 6) | spiData[1]);
}

