#ifndef EKGCONNECTION_H
#define EKGCONNECTION_H

#include <QObject>
#include <QVector>

class QTcpSocket;
class IIRFilter;
class QTimer;

class EKGConnection : public QObject
{
    Q_OBJECT
public:
    explicit EKGConnection(QString hostName, qint16 port, int bufferLen,
                           int updateRate, QObject *parent = 0);
    ~EKGConnection();

signals:
    void newData(QVector<QVector<double>> data, qint64 timestamp);
    void connectionClosed(QString message);
    void error(QString errorMessage);

public slots:
    void start();
    void stop();

private slots:
    void readData();
    void connClosed();

private:
    QTcpSocket *socket;

    QString hostName;
    qint16 port;

    QDataStream *in;

    bool quit;

    int bufferLen;
    int position;
    QVector<QVector<double>> dataBuffer;

    qint64 startTimestamp, recentTimestamp;


    QVector<IIRFilter*> filters;
};

#endif // EKGCONNECTION_H
