#ifndef REMOTEVIEWER_H
#define REMOTEVIEWER_H

#include <QMainWindow>
#include "qcustomplot.h"

#define X_LOWER 0
#define X_UPPER 330
#define Y_LOWER -1
#define Y_UPPER 1

#define BUFFER_SIZE 1

class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;
class QDateTime;
class QTimer;

namespace Ui {
class RemoteViewer;
}

class RemoteViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit RemoteViewer(QWidget *parent = 0);
    ~RemoteViewer();

private slots:
    void on_actionNew_Vest_triggered();

    void on_actionPrevious_Vest_triggered();

    void on_actionDiscover_Vest_triggered();

    void on_actionQuit_triggered();

    void updateGraphs(const QVector<QVector<double>> data, qint64 timeStamp);

    void displayError(const QString &message);

    void updateLatency();

    void on_actionSettings_triggered();

private:
    Ui::RemoteViewer *ui;
    qint16 count;
    QVector<QCustomPlot*> ekgPlots;

    VlcInstance *instance;
    VlcMedia *media;
    VlcMediaPlayer *player;

    QTimer *latencyTimer;

    void connectToVest(QString &ip, qint16 port);


    qint64 delay;
};

#endif // REMOTEVIEWER_H
