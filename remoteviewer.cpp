#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include <QThread>

#include "remoteviewer.h"
#include "ui_remoteviewer.h"
#include "newvestdialog.h"
#include "ekgconnection.h"
#include "discovervestdialog.h"
#include "settingsdialog.h"

#include <vlc-qt/Common.h>
#include <vlc-qt/Instance.h>
#include <vlc-qt/Media.h>
#include <vlc-qt/MediaPlayer.h>

RemoteViewer::RemoteViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RemoteViewer)
{
    qRegisterMetaType<QVector<QVector<double>>>("QVector<QVector<double>>");
    count = 0;
    ui->setupUi(this);

    // Create a vector of the EKG plots
    ekgPlots.append(ui->plot0_3);
    ekgPlots.append(ui->plot1_3);
    ekgPlots.append(ui->plot2_3);
    ekgPlots.append(ui->plot3_3);
    ekgPlots.append(ui->plot4_3);
    ekgPlots.append(ui->plot5_3);
    ekgPlots.append(ui->plot6_3);
    ekgPlots.append(ui->plot7_3);
    ekgPlots.append(ui->plot8_3);
    ekgPlots.append(ui->plot9_3);
    ekgPlots.append(ui->plot10_3);
    ekgPlots.append(ui->plot11_3);
    ekgPlots.append(ui->plot12_3);
    ekgPlots.append(ui->plot13_3);
    ekgPlots.append(ui->plot14_3);
    ekgPlots.append(ui->plot15_3);
    ekgPlots.append(ui->plot16_3);
    ekgPlots.append(ui->plot17_3);

    for (int i = 0; i < ekgPlots.size(); i++)
    {
        ekgPlots.at(i)->addGraph();
        ekgPlots.at(i)->xAxis->setRange(X_LOWER, X_UPPER);
        ekgPlots.at(i)->yAxis->setRange(Y_LOWER, Y_UPPER);
    }

    // Set-up the embedded VLC player for the webcam player
    instance = new VlcInstance(VlcCommon::args(), this);
    player = new VlcMediaPlayer(instance);
    player->setVideoWidget(ui->videoWidget);

    ui->videoWidget->setMediaPlayer(player);

    connect(ui->stopEKG, SIGNAL(clicked()), player, SLOT(stop()));

    ui->videoWidget->setFixedWidth(160);
    ui->videoWidget->setFixedHeight(160);
}

RemoteViewer::~RemoteViewer()
{
    delete ui;
}

void RemoteViewer::connectToVest(QString &ip, qint16 port)
{
    // Configure EKG connection
    QThread *ekgThread = new QThread();
    EKGConnection *ekgconn = new EKGConnection(ip, port, BUFFER_SIZE, 3, this);
    ekgconn->moveToThread(ekgThread);

    connect(ekgThread, SIGNAL(started()), ekgconn, SLOT(start()));
    connect(ekgThread, SIGNAL(finished()), ekgconn, SLOT(stop()));
    connect(ekgThread, SIGNAL(finished()), ekgconn, SLOT(deleteLater()));
    connect(ekgThread, SIGNAL(finished()), ekgThread, SLOT(deleteLater()));

    connect(ekgconn, SIGNAL(connectionClosed(QString)), ekgThread, SLOT(quit()));
    connect(ekgconn, SIGNAL(connectionClosed(QString)), ekgThread, SLOT(deleteLater()));
    connect(ekgconn, SIGNAL(connectionClosed(QString)), ekgconn, SLOT(deleteLater()));

    connect(ekgconn, SIGNAL(connectionClosed(QString)), this, SLOT(displayError(QString)));
    connect(ekgconn, SIGNAL(error(QString)), this, SLOT(displayError(QString)));

    connect(ui->stopEKG, SIGNAL(clicked()), ekgconn, SLOT(stop()));
    connect(ekgconn, SIGNAL(newData(QVector<QVector<double>>,qint64)),
            this, SLOT(updateGraphs(QVector<QVector<double>>,qint64)));

    ekgThread->start();

    latencyTimer = new QTimer(this);
    connect(latencyTimer, SIGNAL(timeout()), this, SLOT(updateLatency()));
    latencyTimer->start(3000);

    // Configure echo/webcam connection
    media = new VlcMedia("http://" + ip + ":8081", instance);
    media->setOption(":network-caching=0");
    player->open(media);

}

void RemoteViewer::on_actionNew_Vest_triggered()
{
    NewVestDialog win(this);
    if (win.exec())
    {
        QString ip, name;
        qint16 port;
        bool ekg, echo, remember;
        win.getOptions(ip, name, port, ekg, echo, remember);

        connectToVest(ip, port);
    }
}

void RemoteViewer::on_actionPrevious_Vest_triggered()
{
    QDialog win(this);
    win.exec();
}

void RemoteViewer::on_actionDiscover_Vest_triggered()
{
    DiscoverVestDialog win(this);
    if (win.exec())
    {
        QString ip, name;
        qint16 port;
        bool ekg, echo, remember;
        win.getOptions(ip, name, port, ekg, echo, remember);
        connectToVest(ip, port);
    }
}

void RemoteViewer::on_actionQuit_triggered()
{
    close();
}

void RemoteViewer::updateGraphs(const QVector<QVector<double>> data, qint64 timeStamp)
{
    delay = timeStamp;

    QVector<double> time(BUFFER_SIZE);

    for (int i = 0; i < time.size(); i++)
        time[i] = count + (2*i);

    for (int i = 0; i < ekgPlots.size(); i++)
    {
        ekgPlots.at(i)->graph(0)->addData(time, data.at(i));
        ekgPlots.at(i)->replot();

    }


    count = time[time.size()-1] + 2;

    if (count > X_UPPER)
    {
        for (int i = 0; i < ekgPlots.size(); i++)
            ekgPlots.at(i)->graph(0)->clearData();
        count = 0;
    }
}

void RemoteViewer::displayError(const QString &message)
{
    QMessageBox::information(this, tr("Error"), message);
}

void RemoteViewer::updateLatency()
{
    qint64 cur = QDateTime::currentMSecsSinceEpoch();
    ui->delayLabel->setText("Network Delay: " + QString::number(delay) + " ms");
}

void RemoteViewer::on_actionSettings_triggered()
{
    SettingsDialog win(this);
    win.exec();
}
