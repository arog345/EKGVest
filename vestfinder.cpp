#include <QTimer>

#include "vestfinder.h"

VestFinder::VestFinder(quint16 listenPort, quint16 sendPort, QObject *parent)
    : QObject(parent), listenPort(listenPort), sendPort(sendPort)
{
    socket = new QUdpSocket(this);
    socket->bind(listenPort);

    connect(socket, SIGNAL(readyRead()), this, SLOT(newDatagram()));
}

VestFinder::~VestFinder()
{
}

void VestFinder::newDatagram()
{

    while (socket->hasPendingDatagrams())
    {
        QByteArray message;
        message.resize(socket->pendingDatagramSize());
        socket->readDatagram(message.data(), message.size());

        QString messageStr = message;
        QStringList messageParts = messageStr.split(":");

        QString name, ip;
        quint16 port;
        if (messageParts.size() == 2)
        {
            ip = messageParts.value(0).trimmed();
            port = messageParts.value(1).trimmed().toInt();
            name = "<N/A>";
        }
        else
        {
            ip = messageParts.value(0).trimmed();
            port = messageParts.value(1).trimmed().toInt();
            name = messageParts.value(2).trimmed();
        }

        emit vestFound(ip, port, name);
    }

}

void VestFinder::findVests()
{
    QByteArray message = "Looking for all vests!";
    socket->writeDatagram(message.data(), message.size(),
                          QHostAddress::Broadcast, sendPort);
}
