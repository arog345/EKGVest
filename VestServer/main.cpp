#include <QCoreApplication>
#include <QHostAddress>
#include "simplestreamer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Enter the ip address for the server: ";

    QTextStream s(stdin);
    QString ipStr = s.readLine();
    QHostAddress ip(ipStr);


    SimpleStreamer server(&ip, 1234, 45678);

    return a.exec();
}
