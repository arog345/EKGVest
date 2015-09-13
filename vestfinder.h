#ifndef VESTFINDER_H
#define VESTFINDER_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>

class VestFinder : public QObject
{
    Q_OBJECT
public:
    explicit VestFinder(quint16 listenPort, quint16  sendPort, QObject *parent = 0);
    ~VestFinder();

public slots:
    void findVests();

signals:
    void vestFound(QString &hostName, quint16 port, QString &name);
    void finished();

private slots:
    void newDatagram();

private:
    QUdpSocket *socket;
    quint16 sendPort;
    quint16 listenPort;
};

#endif // VESTFINDER_H
