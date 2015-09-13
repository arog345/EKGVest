#include "remoteviewer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RemoteViewer w;
    w.show();

    return a.exec();
}
