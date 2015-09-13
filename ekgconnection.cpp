#include "ekgconnection.h"

#include <QDebug>
#include <QDateTime>
#include <QTcpSocket>
#include <QTimer>

#include "iirfilter.h"

EKGConnection::EKGConnection(QString hostName, qint16 port, int bufferLen,
                             int updateRate, QObject *parent) :
    QObject(parent), hostName(hostName), port(port), quit(false), position(0),
    bufferLen(bufferLen), recentTimestamp(0)
{


    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(stop()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(connClosed()));

    // bufferLen is the number of timesteps written to 'data' before being emitted
    // to the GUI thread
    dataBuffer.resize(18);
    filters.resize(18);
    for (int i = 0; i < dataBuffer.size(); i++)
    {
        dataBuffer[i] = QVector<double>(bufferLen, 0);
        filters[i] = new IIRFilter;
    }

}

EKGConnection::~EKGConnection()
{
    socket->disconnectFromHost();

    for (int i = 0; i < filters.size(); i++)
        delete filters[i];
}

void EKGConnection::start()
{
    socket->connectToHost(hostName, port);
    in = new QDataStream(socket);
    in->setVersion(QDataStream::Qt_4_0);
    startTimestamp = QDateTime::currentMSecsSinceEpoch();
}

void EKGConnection::readData()
{
    if (socket->bytesAvailable() < (sizeof(qint64)+18*sizeof(double)))
        return;

    qint64 dataTimestamp;
    *in >> dataTimestamp;

    double temp;
    *in >> dataBuffer[0][position];
    for (int i = 1; i < dataBuffer.size(); i++)
    {
        *in >> dataBuffer[i][position];
        //filters[i]->filterInput(temp);
        //dataBuffer[i][position] = filters[i]->recentOutput();
    }
    socket->write("1");

    position++;

    if (position >= bufferLen)
    {
        position = 0;
        emit newData(dataBuffer, QDateTime::currentMSecsSinceEpoch() - recentTimestamp);
    }
    recentTimestamp = QDateTime::currentMSecsSinceEpoch();
}

void EKGConnection::stop()
{
    socket->disconnectFromHost();
}

void EKGConnection::connClosed()
{
    emit connectionClosed("Connection closed. Time active: " + QString::number((recentTimestamp-startTimestamp)/1000.0));
}

