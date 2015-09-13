#ifndef DISCOVERVESTDIALOG_H
#define DISCOVERVESTDIALOG_H

#include <QDialog>
#include "vestfinder.h"

class QDialogButtonBox;
class QPushButton;
class QScrollArea;
class QListView;
class QCheckBox;
class QStringListModel;
class QItemSelection;

class DiscoverVestDialog : public QDialog
{
    Q_OBJECT

public:
    DiscoverVestDialog(QWidget *parent = 0);
    ~DiscoverVestDialog();
    void getOptions(QString &ip, QString &name, qint16 &port,
                    bool &ekg, bool &echo, bool &remember);

private slots:
    void populateList();
    void vestDiscovered(QString &ip, quint16 port, QString &name);
    void listSelectionChanged(const QItemSelection &current);

private:
    QCheckBox *ekgCheckBox;
    QCheckBox *echoCheckBox;
    QCheckBox *rememberCheckBox;
    QDialogButtonBox *buttonBox;
    QPushButton *findButton;
    QPushButton *connectButton;
    QPushButton *closeButton;
    QPushButton *moreButton;
    QWidget *extension;
    QScrollArea *listScroll;
    QListView *vestView;
    QStringListModel *vests;

    VestFinder *findVests;
    QStringList *vestsList;
};

#endif // DISCOVERVESTDIALOG_H
